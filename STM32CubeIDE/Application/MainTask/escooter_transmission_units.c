/*
 * Drive.c
 *
 *  Created on: Sep 2, 2024
 *      Author: TerenceLeung
 */

/***********************************************ESCOOTER TRANSMISSION UNITS (ETU)*********************************************/

#include <backgroundTask.h>
#include <GearControl.h>
#include "ETU_OBD.h"
#include "ETU_StateMachine.h"
#include "escooter_transmission_units.h"
#include "SleepAndWake.h"
#include "brake_and_throttle.h"
#include "battery_current_sensors.h"
#include "tail_light.h"
#include "PROTOCOL_HANDLER.h"
#include "ERROR_REPORT.h"
#include "flash_internal.h"
#include "UDHAL_TIMEOUT.h"
#include "UDHAL_MOTOR.h"
#include "motor_param.h"
#include "hardwareParam.h"
#include "mc_tasks.h"
#include "cmsis_os.h"

/*Assign ETU_StateHandle_t object*/
static ETU_StateHandle_t ETU_State;
/*Assign GearMode_Handle_t object*/
static GearMode_Handle_t transmissionMode;
/*Assign ETU_Status_Handle_t object*/
ETU_Status_Handle_t status;


/*RTOS Handler*/
static osThreadId driveHandle;
static osPriority priority;

/*N1_TIME --> 100 ms
 *N2_TIME --> 200 ms
 *N3_TIME --> 300 ms
 *For example, if GeneralTask_TIME = 0.02 seconds, N2_TIME = 0.2 seconds, then N2_ticks = 10
 * */
static uint8_t N1_ticks = N1_TIME / GeneralTask_TIME;
static uint8_t N2_ticks = N2_TIME / GeneralTask_TIME;
static uint8_t N3_ticks = N3_TIME / GeneralTask_TIME;


//Those variables should be centralised in typedef struct ETU_Status_Handle_t
//tail light variables
static volatile uint32_t taskSleepCount = 0;  //2025-05-12
static void GeneralTasks(void const * argument);

static uint8_t faultRecordCount = 0;

void power_flag_register(bool *ptrShutDownFlag)
{
	status.ptrPower = ptrShutDownFlag;
}

void motor_fault_signal(uint8_t *ptrMotorFaultFlag)
{
	status.ptrMotorFault = ptrMotorFaultFlag;
}

void connection_flag_register(uint8_t *ptrConnectionFlag)
{
	status.ptrConnectFault = ptrConnectionFlag;
}

void obd_mode_flag_register(bool *ptrOBDFlag)
{
	status.OBD = ptrOBDFlag;
}

void ETU_StatusHandlerInit()
{
	/*Initialize all variables in ETU_Status_Handle_t*/
	POWER_FLAG_INIT();
	MOTOR_FAULT_FLAG_INIT();
	CONNECTION_FAIL_FLAG_INIT();
	status.inConnectionLED = 0;
	for(uint8_t i = 0; i < DECISION_MAKER; i++)
	{
		status.DriveDecisionMaker[i] = 0xFF;
	}
	for(uint8_t i = 0; i < FAULT_RECORD; i++)
	{
		status.ETU_Fault_Record[i] = 0xFF;
	}
	status.gearStop   = 0xFF;
	status.systemExit  = 0x00;
}

void GoInit()
{
	ETU_StatusHandlerInit();

	GearInit(&transmissionMode);

	brake_and_throttle_init();

	motor_param_init();

	tail_light_status_Init();

	set_tail_light_off();

	/*Handles transmission timeout when the connection is lost*/
	UDHAL_TIMEOUT_init();

	/*Idle Task*/
	powerManagementInit();

    /*UART Connection Status is also monitored in Background Task*/
	BACKGROUND_CONNECTION_MONITOR_INIT();

	/*Idle Task*/
	run_background_tasks();

	/*It is the main task for controlling the E-Scooter*/
	createDrivingTasks();

	/*Initialize the ETU_State, let's go to BOOT_CHECK first*/
	ETU_Init(&ETU_State);

	etu_state_ptr_register(&ETU_State);
}

void createDrivingTasks(void)
{
	/*Starts Driving Task*/
	osThreadDef(drive, GeneralTasks,osPriorityBelowNormal,0,128);
	driveHandle = osThreadCreate(osThread(drive), NULL);
}

void ETU_BootRoutine(uint8_t diagnosis)
{
	if(ETU_GetState(&ETU_State) == BOOT_CHECK)
	{
		if(getBootSource() == CONNECT_BATTERY) status.isPowerOnBoot = 0x01;

		if(status.isPowerOnBoot == 0x00)
		{
			/* ETU doesn't go into OBD Mode when the E-Scooter is operating, to enter into OBD
			 * Backdoor command should be given externally, i.e. software means
			 */
			if(diagnosis == 0x00) { ETU_NextState(&ETU_State,ETU_START);}
			else if(diagnosis == 0x01){ ETU_NextState(&ETU_State,ETU_OBD);}
		}
		else if(status.isPowerOnBoot == 0x01)
		{
			error_indicator_on();
			*status.ptrPower = false;
			ETU_PowerShutDown();
			status.inConnectionLED = led_indicator_off();
			set_tail_light_off();
			error_indicator_off();
			ETU_NextState(&ETU_State,ETU_OFF);
		}
	}
}

void saveFaultRecord(uint8_t errorCode)
{
	status.ETU_Fault_Record[faultRecordCount] = errorCode;
	faultRecordCount++;
	if (faultRecordCount > FAULT_RECORD)
	{
		faultRecordCount = 0;
	}
}

uint8_t parking = 0x01;
uint16_t driving = 0x00;
/* Safety Power Cutting 2025-03-25 */
void ETU_PowerShutDown()
{
	parking = 0x01;
	status.systemExit  = 0x01;
	driving = 0x00;
	Motor_ShutDown();
}

void ETU_connectionFaultHandler()
{
	/*IQ should be set to zero*/
	ETU_PowerShutDown();
	status.inConnectionLED = led_indicator_off();
	set_tail_light_off();  //2025-05-03
	error_indicator_off(); //2025-05-03
	timeOutStop();         //2025-05-03
	ETU_NextState(&ETU_State,ETU_FAULT);
}

void ETU_GearTransmitStart()
{
	driving += 1;
	throttleSignalInput();
}

void ETU_SignalIndicatorOFF()
{
	status.inConnectionLED = led_indicator_off();
	set_tail_light_off();
	error_indicator_off();
}

uint8_t gearAvailable()
{
   /*  Make a bit array/register. To ready for driving (D-Mode), the following conditions should be met:
    *  https://www.cs.emory.edu/~cheung/Courses/255/Syllabus/1-C-intro/bit-array.html
    *  1. *ptr_error_report == 0  --> gear is ready
    *  2. *software_error_report == 0 --> gear is ready
    *  3.  --> If Iq = 0 / ptr_brakeAndThrottle.throttleTriggered = false --> gear is not ready; If Iq != 0 / ptr_brakeAndThrottle.throttleTriggered = true --> gear is ready
    *  If any of bits in GearReady[] register is 0, the gear is not ready yet, throttleSignalInput() is not triggered --> go back to N (Neutral) mode.
    *       DriveDecisionMaker REGISTER   (uint8_t)                          -------------------------------
    *       ----------------------------------------       | UF Bit | ETUF Bit | IQT Bit |
    *       |      2      |      1     |      0     |      -------------------------------
    *       |---------------------------------------|          0         0         0
    *       | UART FAULTS | ETU FAULTS | IQ_TRIGGER |
    *       |     R/W     |    R/W     |    R/W     |
    *       ----------------------------------------
    * * ****************************************************************************************/
    /*Checks if the driver presses the throttle*/
   status.DriveDecisionMaker[0] = getThrottleStatus() ? 0x00 : 0x01; //Throttle Ready --> 0x00 ; Not Ready: 0x01
    /*Checks if there's any ETU Faults*/
    if ( (*status.ptrMotorFault) != 0 ){
    	status.DriveDecisionMaker[1] = 0x01;
    }else if( (*status.ptrMotorFault) == 0){
    	status.DriveDecisionMaker[1] = 0x00;
    }
    /*Checks if disconnection occurs*/
    if( (*status.ptrConnectFault) != 0){
    	status.DriveDecisionMaker[2] = 0x01;
    }else if( (*status.ptrConnectFault) == 0){
    	status.DriveDecisionMaker[2] = 0x00;
    }

    /*OR Operation --> If one of the issues exits in ETU, driving is forbidden*/
    status.isFault    = status.DriveDecisionMaker[1] | status.DriveDecisionMaker[2];
    status.gearStop = status.DriveDecisionMaker[0]  | status.DriveDecisionMaker[1] | status.DriveDecisionMaker[2];

    return status.gearStop;
}

/*Automatic gear selection*/
void GearTransmitControlPanel()
{
	/*What's the current gear*/
	GearMode_t gearMode = getCurrentGear(&transmissionMode);

	/*Is "Gear" Transmission Ready ? */
	uint8_t gearStop =  gearAvailable();

	motor_speed();
	switch (gearMode)
	{
	    case PARK:
	    {
	         if(getThrottlePercent() != 0) //Hand-Brake
	         {
	        	 if(getRPM() < 80)
	        	 {
		              throttleSignalInput();
		              GearToggle(&transmissionMode, NEUTRAL);
	        	 }
	          }

	    }
	    break;

	    case NEUTRAL:
	    {
	    	if(gearStop == 0){
	    		/*Shift the Gear to D from N to start driving*/
	    		status.systemExit  = 0x00;
	    		GearToggle(&transmissionMode, DRIVE);
	    	}
	    	else if(gearStop == 1){
		    	setIQ(0); //It must not be neglected
		    	throttleSignalInput();
	    	}
	    }
	    break;

	    case DRIVE:
	    {
	    	if(gearStop == 0)
	    	{
	    		ETU_GearTransmitStart(); // IQ != 0
	    	}
	    	else if(gearStop == 1)
	    	{
	    		GearToggle(&transmissionMode, NEUTRAL);
	    	}
	    }
	    break;

	    default:
	      break;
	}
}

static void GeneralTasks(void const * argument)
{
	priority = osThreadGetPriority(NULL);

	ETU_BootRoutine(SKIP_DIAGNOSIS);

	if(ETU_GetState(&ETU_State) == ETU_START)
	{
	   updateConnectionStatus(false,0);
	   timeOutStart();
	   for(;;)
	   {
		   osDelay(GeneralTask_TIME);
		   /****************  Task timing & sleep *******************/
		   /* Task sleep must be positioned at the beginning of the for loop */
		   if( (*status.ptrPower) == true)
		   {
			  if(status.inConnectionLED == 0)
			  {
				  status.inConnectionLED = led_indicator_on();
			  }
			 GearTransmitControlPanel();
		     if((taskSleepCount % N1_ticks) == 0)
		     {
                /*We could put GearTransmitControlPanel() into here */

		     }

		     if((taskSleepCount % N2_ticks) == 0)
		     {
		    	 tailLightStateMachine();
		     }

		     if((taskSleepCount % N3_ticks) == 0)
		     {
		    	/*Tail Light Control*/
		    	lightSensorStateChange();
				checkConnectionStatus();
		     }

		     /*Put line 348 - line 364 into a function which handles system fault*/
		     if((*status.ptrMotorFault) != 0)
		     {
		    	 setIQ(0);
		    	 throttleSignalInput();
		    	 driveStop();
		    	 error_indicator_on();
		     }

		     if((*status.ptrConnectFault) != 0)
		     {
		    	 driving = 0;
		    	 parking = 0x01;
		    	 setIQ(0);
		    	 throttleSignalInput();
		    	 driveStop();
		    	 ETU_connectionFaultHandler();
		    	 *status.ptrPower = false;
		    	 break;
		     }


		 }else if( (*status.ptrPower) == false)
		 {
			ETU_PowerShutDown();
			ETU_SignalIndicatorOFF();
			timeOutStop();
			ETU_NextState(&ETU_State,ETU_OFF);
			break;
		 }
		taskSleepCount++;
		if(taskSleepCount == UINT32_MAX)taskSleepCount = 0;
	  }
	}

	/*Shut Down Process Begins*/
	if(ETU_GetState(&ETU_State) == ETU_OFF || ETU_GetState(&ETU_State) == ETU_OBD || ETU_GetState(&ETU_State) == ETU_FAULT)
	{
		/* A function that implements a task must not exit or attempt to return to
			its caller as there is nothing to return to.  If a task wants to exit it
			should instead call vTaskDelete( NULL ).

			Artificially force an assert() to be triggered if configASSERT() is
			defined, then stop here so application writers can catch the error. static void prvTaskExitError() from port.c*/
		vTaskDelete(NULL); //Kills the task !
	}
}
