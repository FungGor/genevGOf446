/**
  ******************************************************************************
  * @file    motor_control_protocol.c
  * @author  Motor Control SDK Team, ST Microelectronics
  * @brief   This file provides firmware functions that implement the features
  *          of the motor_control_protocol component of the Motor Control SDK.
  *
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "user_interface.h"
#include "motor_control_protocol.h"
#include "SleepAndWake.h"
#include "brake_and_throttle.h"
#include "tail_light.h"
#include "lightSensor.h"
#include "ERROR_REPORT.h"
#include "PROTOCOL_HANDLER.h"
#include "motor_param.h"
#include "ETU_OBD.h"
#include "flash_internal.h"
#include "backgroundTask.h"
/**
 * @addtogroup MCSDK
 * @{
 */

/**
 * @addtogroup MCUI
 * @{
 */

/**
 * @defgroup motor_control_protocol Motor Control Protocol
 *
 * @brief Transmission protocol designed to report Motor Control subsystem status and to control motors.
 *
 * The Motor Control Protocol defines a transmission mechanism that allows a Motor Control
 * Application to send in real time the values of a defined set of internal variables to an
 * external recipient and to receive Motor Control commands from this recipient.
 *
 * The Commands that can be sent with the Motor Control Protocol are defined in **TBD**.
 *
 * The variables which values can be exchanged through the Motor Control Protocol are listed in
 * @ref MC_Protocol_REG.
 *
 * An example of external recipient is the Motor Control Monitor tool that is part of the Motor
 * Control Workbench.
 *
 * The Motor Control Protocol defines frames that contain either commands or variables values.
 * These frames are the exchanged between the Application and the external recipient. To that end,
 * the Motor Control Protocol relies on a lower level transport protocol to actually send them.
 *
 * @todo Complete documentation
 * @{
 */

/* Private define ------------------------------------------------------------*/

#define ACK_NOERROR 0xF0
#define ACK_ERROR   0xFF
#define ATR_FRAME_START 0xE0

#define MC_PROTOCOL_CODE_NONE        0x00

/* List of error codes */
typedef enum ERROR_CODE_e
{
	ERROR_NONE = 0,             /**<  0x00 - No error */
	ERROR_BAD_FRAME_ID,         /**<  0x01 - BAD Frame ID. The Frame ID has not been recognized by the firmware. */
	ERROR_CODE_SET_READ_ONLY,   /**<  0x02 - Write on read-only. The master wants to write on a read-only register. */
	ERROR_CODE_GET_WRITE_ONLY,  /**<  0x03 - Read not allowed. The value cannot be read. */
	ERROR_CODE_NO_TARGET_DRIVE, /**<  0x04 - Bad target drive. The target motor is not supported by the firmware. */
	ERROR_CODE_WRONG_SET,       /**<  0x05 - Value used in the frame is out of range expected by the FW. */
	ERROR_CODE_CMD_ID,          /**<  0x06 - NOT USED */
	ERROR_CODE_WRONG_CMD,       /**<  0x07 - Bad command ID. The command ID has not been recognized. */
	ERROR_CODE_OVERRUN,         /**<  0x08 - Overrun error. Transmission speed too fast, frame not received correctly */
	ERROR_CODE_TIMEOUT,         /**<  0x09 - Timeout error. Received frame corrupted or unrecognized by the FW. */
	ERROR_CODE_BAD_CRC,         /**<  0x0A - The computed CRC is not equal to the received CRC byte. */
	ERROR_BAD_MOTOR_SELECTED,   /**<  0x0B - Bad target drive. The target motor is not supported by the firmware. */
	ERROR_MP_NOT_ENABLED        /**<  0x0C - Motor Profiler not enabled. */
} ERROR_CODE;

MPInfo_t MPInfo = {0, 0};

/**
* @brief  Initializes  MCP component parameters
*
* @param  pHandle Pointer on the handle of the component to initialize.
* @param  pFCP Pointer on Frame communication protocol component's handle to use.
* @param  fFcpSend Pointer on FCP's send message function
* @param  fFcpReceive Pointer on FCP's receive message function
* @param  fFcpAbortReceive Pointer on FCP's abort receive message function
* @param  s_fwVer Pointer on string containing FW release version.
*/
__weak void MCP_Init( MCP_Handle_t *pHandle,
               FCP_Handle_t * pFCP,
               FCP_SendFct_t fFcpSend,
               FCP_ReceiveFct_t fFcpReceive,
               FCP_AbortReceiveFct_t fFcpAbortReceive,
               const char* s_fwVer )
{
  pHandle->pFCP = pFCP;
  pHandle->s_fwVer = s_fwVer;
  FCP_SetClient( pFCP, pHandle,
                 (FCP_SentFrameCallback_t) & MCP_SentFrame,
                 (FCP_ReceivedFrameCallback_t) & MCP_ReceivedFrame,
                 (FCP_RxTimeoutCallback_t) & MCP_OnTimeOut );
  pHandle->fFcpSend = fFcpSend;
  pHandle->fFcpReceive = fFcpReceive;
  pHandle->fFcpAbortReceive = fFcpAbortReceive;

  MCP_WaitNextFrame(pHandle);
}

/**
* @brief  Function used to set and report the Time Out.
*
* @param  pHandle Pointer on the handle of the component.
*/
__weak void MCP_OnTimeOut(MCP_Handle_t *pHandle)
{
     MCP_WaitNextFrame(pHandle);
}

/**
* @brief  Function used to check next reception frame
*
* @param  pHandle Pointer on the handle of the component.
*/
__weak void MCP_WaitNextFrame(MCP_Handle_t *pHandle)
{
  pHandle->fFcpAbortReceive(pHandle->pFCP);
  pHandle->BufferSize = FCP_MAX_PAYLOAD_SIZE;
  pHandle->fFcpReceive(pHandle->pFCP);
}

/**
* @brief  Function used to transmit the data
*
* @param  pHandle Pointer on the handle of the component.
* @param  Code code value of frame to send.
* @param  buffer frame data buffer.
* @param  Size size of data frame.
*/
__weak void MCP_SentFrame(MCP_Handle_t *pHandle, uint8_t Code, uint8_t *buffer, uint8_t Size)
{
    MCP_WaitNextFrame(pHandle);
}

/**
* @brief  Function used to decode received data
*
* @param  pHandle Pointer on the handle of the component.
* @param  Code code value of frame to send.
* @param  buffer frame data buffer.
* @param  Size size of data frame.
*/
uint8_t uart = 0;
__weak void MCP_ReceivedFrame(MCP_Handle_t *pHandle, uint8_t Code, uint8_t *buffer, uint8_t Size)
{
  bool RequireAck = true;
  bool bNoError = false; // Default is error
  uint8_t bErrorCode;

  /* Protocol version >3.3 motor selection inside Frame ID */
  uint8_t bMotorSelection = (Code & 0xE0) >> 5; /* Mask: 1110|0000 */
  if (bMotorSelection != 0)
  {
    if (UI_SetReg(&pHandle->_Super, MC_PROTOCOL_REG_TARGET_MOTOR, bMotorSelection - 1))
    {
      Code &= 0x1F; /* Mask: 0001|1111 */

    }
    else
    {
      Code = MC_PROTOCOL_CODE_NONE; /* Error */
      bErrorCode = ERROR_BAD_MOTOR_SELECTED;
    }
  }

  switch (Code)
  {
  case MC_PROTOCOL_CODE_SET_REG:
    {
      MC_Protocol_REG_t bRegID = (MC_Protocol_REG_t)buffer[0];
      bErrorCode = ERROR_CODE_WRONG_SET;

      switch (bRegID)
      {
      case MC_PROTOCOL_REG_TARGET_MOTOR:
        {
          /* Deprecated */
          int32_t wValue = (int32_t)(buffer[1]);

          bNoError = UI_SetReg(&pHandle->_Super, bRegID, wValue);
        }
        break;
      case MC_PROTOCOL_REG_CONTROL_MODE:
      case MC_PROTOCOL_REG_SC_PP:
        {
          /* 8bit variables */
          bNoError = UI_SetReg(&pHandle->_Super, bRegID, (int32_t)(buffer[1]));
        }
        break;

      case MC_PROTOCOL_REG_DAC_OUT1:
        {
          bNoError = true; /* No check inside class return always true*/
        }
        break;

      case MC_PROTOCOL_REG_DAC_OUT2:
        {
          bNoError = true; /* No check inside class return always true*/
        }
        break;

      case MC_PROTOCOL_REG_TORQUE_REF:
      case MC_PROTOCOL_REG_FLUX_REF:
      case MC_PROTOCOL_REG_SPEED_KP:
      case MC_PROTOCOL_REG_SPEED_KI:
      case MC_PROTOCOL_REG_SPEED_KD:
      case MC_PROTOCOL_REG_TORQUE_KP:
      case MC_PROTOCOL_REG_TORQUE_KI:
      case MC_PROTOCOL_REG_TORQUE_KD:
      case MC_PROTOCOL_REG_FLUX_KP:
      case MC_PROTOCOL_REG_FLUX_KI:
      case MC_PROTOCOL_REG_FLUX_KD:
      case MC_PROTOCOL_REG_PLL_KI:
      case MC_PROTOCOL_REG_PLL_KP:
      case MC_PROTOCOL_REG_FLUXWK_KP:
      case MC_PROTOCOL_REG_FLUXWK_KI:
      case MC_PROTOCOL_REG_FLUXWK_BUS:
      case MC_PROTOCOL_REG_IQ_SPEEDMODE:
      case MC_PROTOCOL_REG_PFC_DCBUS_REF:
      case MC_PROTOCOL_REG_PFC_I_KP:
      case MC_PROTOCOL_REG_PFC_I_KI:
      case MC_PROTOCOL_REG_PFC_I_KD:
      case MC_PROTOCOL_REG_PFC_V_KP:
      case MC_PROTOCOL_REG_PFC_V_KI:
      case MC_PROTOCOL_REG_PFC_V_KD:
      case MC_PROTOCOL_REG_PFC_STARTUP_DURATION:
      case MC_PROTOCOL_REG_HFI_INIT_ANG_SAT_DIFF:
      case MC_PROTOCOL_REG_HFI_PI_TRACK_KP:
      case MC_PROTOCOL_REG_HFI_PI_TRACK_KI:
      case MC_PROTOCOL_REG_POSITION_KP:
      case MC_PROTOCOL_REG_POSITION_KI:
      case MC_PROTOCOL_REG_POSITION_KD:
        {
          /* 16bit variables */
          int32_t wValue = buffer[1] + (buffer[2] << 8);
          bNoError = UI_SetReg(&pHandle->_Super, bRegID, wValue);
        }
        break;

      case MC_PROTOCOL_REG_OBSERVER_C1:
      case MC_PROTOCOL_REG_OBSERVER_C2:
      case MC_PROTOCOL_REG_FF_1Q:
      case MC_PROTOCOL_REG_FF_1D:
      case MC_PROTOCOL_REG_FF_2:
      case MC_PROTOCOL_REG_RAMP_FINAL_SPEED:
      case MC_PROTOCOL_REG_SC_CURRENT:
      case MC_PROTOCOL_REG_SC_SPDBANDWIDTH:
      case MC_PROTOCOL_REG_SC_LDLQRATIO:
      case MC_PROTOCOL_REG_SC_NOMINAL_SPEED:
      case MC_PROTOCOL_REG_SC_CURRBANDWIDTH:
      case MC_PROTOCOL_REG_SC_STARTUP_SPEED:
      case MC_PROTOCOL_REG_SC_STARTUP_ACC:
        {
          /* 32bit variables */
          int32_t wValue = buffer[1] + (buffer[2] << 8) + (buffer[3] << 16) + (buffer[4] << 24);
          bNoError = UI_SetReg(&pHandle->_Super, bRegID, wValue);
        }
        break;

      default:
        {
          bErrorCode = ERROR_CODE_SET_READ_ONLY;
        }
        break;
      }
    }
    break;

  case MC_PROTOCOL_CODE_GET_REG:
    {
      MC_Protocol_REG_t bRegID = (MC_Protocol_REG_t)buffer[0];
      bErrorCode = ERROR_CODE_GET_WRITE_ONLY;

      switch (bRegID)
      {
      case MC_PROTOCOL_REG_TARGET_MOTOR:
      case MC_PROTOCOL_REG_STATUS:
      case MC_PROTOCOL_REG_CONTROL_MODE:
      case MC_PROTOCOL_REG_RUC_STAGE_NBR:
      case MC_PROTOCOL_REG_PFC_STATUS:
      case MC_PROTOCOL_REG_PFC_ENABLED:
      case MC_PROTOCOL_REG_SC_CHECK:
      case MC_PROTOCOL_REG_SC_STATE:
      case MC_PROTOCOL_REG_SC_STEPS:
      case MC_PROTOCOL_REG_SC_PP:
      case MC_PROTOCOL_REG_SC_FOC_REP_RATE:
      case MC_PROTOCOL_REG_SC_COMPLETED:
        {
          /* 8bit variables */
          int32_t value = UI_GetReg( &pHandle->_Super, bRegID, &bNoError );
           if ( bNoError == true )
          {
            pHandle->fFcpSend(pHandle->pFCP, ACK_NOERROR, (uint8_t*)(&value), 1);
            RequireAck = false;
          }
        }
        break;

      case MC_PROTOCOL_REG_DAC_OUT1:
        {
        }
        break;

      case MC_PROTOCOL_REG_DAC_OUT2:
        {
        }
        break;

      case MC_PROTOCOL_REG_SPEED_KP:
      case MC_PROTOCOL_REG_SPEED_KP_DIV:
      case MC_PROTOCOL_REG_SPEED_KI:
      case MC_PROTOCOL_REG_SPEED_KI_DIV:
      case MC_PROTOCOL_REG_SPEED_KD:
      case MC_PROTOCOL_REG_TORQUE_REF:
      case MC_PROTOCOL_REG_TORQUE_KP:
      case MC_PROTOCOL_REG_TORQUE_KI:
      case MC_PROTOCOL_REG_TORQUE_KD:
      case MC_PROTOCOL_REG_FLUX_REF:
      case MC_PROTOCOL_REG_FLUX_KP:
      case MC_PROTOCOL_REG_FLUX_KI:
      case MC_PROTOCOL_REG_FLUX_KD:
      case MC_PROTOCOL_REG_OBSERVER_C1:
      case MC_PROTOCOL_REG_OBSERVER_C2:
      case MC_PROTOCOL_REG_OBSERVER_CR_C1:
      case MC_PROTOCOL_REG_OBSERVER_CR_C2:
      case MC_PROTOCOL_REG_PLL_KP:
      case MC_PROTOCOL_REG_PLL_KI:
      case MC_PROTOCOL_REG_FLUXWK_KP:
      case MC_PROTOCOL_REG_FLUXWK_KI:
      case MC_PROTOCOL_REG_FLUXWK_BUS:
      case MC_PROTOCOL_REG_BUS_VOLTAGE:
      case MC_PROTOCOL_REG_HEATS_TEMP:
      case MC_PROTOCOL_REG_MOTOR_POWER:
      case MC_PROTOCOL_REG_TORQUE_MEAS:
      case MC_PROTOCOL_REG_FLUX_MEAS:
      case MC_PROTOCOL_REG_FLUXWK_BUS_MEAS:
      case MC_PROTOCOL_REG_IQ_SPEEDMODE:
      case MC_PROTOCOL_REG_FF_VQ:
      case MC_PROTOCOL_REG_FF_VD:
      case MC_PROTOCOL_REG_FF_VQ_PIOUT:
      case MC_PROTOCOL_REG_FF_VD_PIOUT:
      case MC_PROTOCOL_REG_PFC_DCBUS_REF:
      case MC_PROTOCOL_REG_PFC_DCBUS_MEAS:
      case MC_PROTOCOL_REG_PFC_ACBUS_FREQ:
      case MC_PROTOCOL_REG_PFC_ACBUS_RMS:
      case MC_PROTOCOL_REG_PFC_I_KP:
      case MC_PROTOCOL_REG_PFC_I_KI:
      case MC_PROTOCOL_REG_PFC_I_KD:
      case MC_PROTOCOL_REG_PFC_V_KP:
      case MC_PROTOCOL_REG_PFC_V_KI:
      case MC_PROTOCOL_REG_PFC_V_KD:
      case MC_PROTOCOL_REG_PFC_STARTUP_DURATION:
      case MC_PROTOCOL_REG_HFI_EL_ANGLE:
      case MC_PROTOCOL_REG_HFI_ROT_SPEED:
      case MC_PROTOCOL_REG_HFI_CURRENT:
      case MC_PROTOCOL_REG_HFI_INIT_ANG_PLL:
      case MC_PROTOCOL_REG_HFI_INIT_ANG_SAT_DIFF:
      case MC_PROTOCOL_REG_HFI_PI_TRACK_KP:
      case MC_PROTOCOL_REG_HFI_PI_TRACK_KI:
      case MC_PROTOCOL_REG_CTRBDID:
      case MC_PROTOCOL_REG_PWBDID:
      case MC_PROTOCOL_REG_POSITION_KP:
      case MC_PROTOCOL_REG_POSITION_KI:
      case MC_PROTOCOL_REG_POSITION_KD:
        {
          int32_t value = UI_GetReg( &pHandle->_Super, bRegID, &bNoError );
          if ( bNoError == true )
          {
            /* 16bit variables */
            pHandle->fFcpSend(pHandle->pFCP, ACK_NOERROR, (uint8_t*)(&value), 2);
            RequireAck = false;
          }
        }
        break;

      case MC_PROTOCOL_REG_FLAGS:
      case MC_PROTOCOL_REG_SPEED_REF:
      case MC_PROTOCOL_REG_SPEED_MEAS:
      case MC_PROTOCOL_REG_FF_1Q:
      case MC_PROTOCOL_REG_FF_1D:
      case MC_PROTOCOL_REG_FF_2:
      case MC_PROTOCOL_REG_PFC_FAULTS:
      case MC_PROTOCOL_REG_RAMP_FINAL_SPEED:
      case MC_PROTOCOL_REG_SC_RS:
      case MC_PROTOCOL_REG_SC_LS:
      case MC_PROTOCOL_REG_SC_KE:
      case MC_PROTOCOL_REG_SC_VBUS:
      case MC_PROTOCOL_REG_SC_MEAS_NOMINALSPEED:
      case MC_PROTOCOL_REG_SC_CURRENT:
      case MC_PROTOCOL_REG_SC_SPDBANDWIDTH:
      case MC_PROTOCOL_REG_SC_LDLQRATIO:
      case MC_PROTOCOL_REG_SC_NOMINAL_SPEED:
      case MC_PROTOCOL_REG_SC_CURRBANDWIDTH:
      case MC_PROTOCOL_REG_SC_J:
      case MC_PROTOCOL_REG_SC_F:
      case MC_PROTOCOL_REG_SC_MAX_CURRENT:
      case MC_PROTOCOL_REG_SC_STARTUP_SPEED:
      case MC_PROTOCOL_REG_SC_STARTUP_ACC:
      case MC_PROTOCOL_REG_SC_PWM_FREQUENCY:
      case MC_PROTOCOL_REG_UID:
        {
          int32_t value = UI_GetReg( &pHandle->_Super, bRegID, &bNoError);
          if ( bNoError == true )
          {
            /* 32bit variables */
            pHandle->fFcpSend(pHandle->pFCP, ACK_NOERROR, (uint8_t*)(&value), 4);
            RequireAck = false;
          }
        }
        break;

      case MC_PROTOCOL_REG_CURRENT_POSITION:
      case MC_PROTOCOL_REG_TARGET_POSITION:
      case MC_PROTOCOL_REG_MOVE_DURATION:
        {
          int32_t value = UI_GetReg( &pHandle->_Super, bRegID, &bNoError);
          if ( bNoError == true )
          {
            /* 32bit variables */
            pHandle->fFcpSend(pHandle->pFCP, ACK_NOERROR, (uint8_t*)(&value), 4);
            RequireAck = false;
          }
        }
        break;

      default:
        bErrorCode = ERROR_CODE_GET_WRITE_ONLY;
        break;
      }
    }
    break;

  case MC_PROTOCOL_CODE_EXECUTE_CMD:
    {
      uint8_t bCmdID = buffer[0];
      bErrorCode = ERROR_CODE_WRONG_CMD;
      bNoError = UI_ExecCmd(&pHandle->_Super,bCmdID);
    }
    break;

  case MC_PROTOCOL_CODE_GET_BOARD_INFO:
    {
      /* GetBoardInfo */
      unsigned char i;
      uint8_t outBuff[32];
      for (i = 0; i < 32; i++)
      {
        outBuff[i] = 0;
      }
      for (i = 0; (i<29) && (pHandle->s_fwVer[i]!='\t'); i++)
      {
        outBuff[3+i] = pHandle->s_fwVer[i];
      }
      outBuff[0] = pHandle->s_fwVer[i+5];
      outBuff[1] = pHandle->s_fwVer[i+7];
      outBuff[2] = pHandle->s_fwVer[i+9];
      pHandle->fFcpSend(pHandle->pFCP, ACK_NOERROR, outBuff, 32 );
      bNoError = true;
    }
    break;

  case MC_PROTOCOL_CODE_SET_SPEED_RAMP:
    {
      uint16_t duration = buffer[4] + (buffer[5] << 8);
      int32_t rpm = buffer[0] + (buffer[1] << 8) + (buffer[2] << 16) + (buffer[3] << 24);
      bNoError = UI_ExecSpeedRamp(&pHandle->_Super, rpm,duration);
    }
    break;
  case MC_PROTOCOL_CODE_SET_POSITION_CMD:
    {
    }
    break;

  case MC_PROTOCOL_CODE_SET_TORQUE_RAMP:
    {
      uint16_t duration = buffer[4] + (buffer[5] << 8);
      int32_t torque = buffer[0] + (buffer[1] << 8) + (buffer[2] << 16) + (buffer[3] << 24);
      bNoError = UI_ExecTorqueRamp(&pHandle->_Super, torque,duration);
    }
    break;

  case MC_PROTOCOL_CODE_GET_REVUP_DATA:
    {
      uint8_t outBuff[8];
      uint16_t Durationms;
      int16_t FinalMecSpeedUnit;
      int16_t FinalTorque;
      int32_t rpm;
      UI_GetRevupData(&pHandle->_Super, buffer[0], &Durationms, &FinalMecSpeedUnit, &FinalTorque);
      rpm = (FinalMecSpeedUnit * _RPM) / SPEED_UNIT;
      outBuff[0] = (uint8_t)(rpm);
      outBuff[1] = (uint8_t)(rpm >> 8);
      outBuff[2] = (uint8_t)(rpm >> 16);
      outBuff[3] = (uint8_t)(rpm >> 24);
      outBuff[4] = (uint8_t)(FinalTorque);
      outBuff[5] = (uint8_t)(FinalTorque >> 8);
      outBuff[6] = (uint8_t)(Durationms);
      outBuff[7] = (uint8_t)(Durationms >> 8);
      pHandle->fFcpSend(pHandle->pFCP, ACK_NOERROR, outBuff, 8 );
    }
    break;

  case MC_PROTOCOL_CODE_SET_REVUP_DATA:
    {
      uint8_t bStage;
      uint16_t hDurationms;
      int16_t hFinalMecSpeedUnit;
      int16_t hFinalTorque;
      int32_t rpm;
      bStage = buffer[0];
      hDurationms = buffer[7] + (buffer[8] << 8);
      rpm = buffer[1] + (buffer[2] << 8) + (buffer[3] << 16) + (buffer[4] << 24);
      hFinalMecSpeedUnit = (rpm * SPEED_UNIT ) / _RPM ;
      hFinalTorque = buffer[5] + (buffer[6] << 8);
      bNoError = UI_SetRevupData( &pHandle->_Super, bStage, hDurationms, hFinalMecSpeedUnit, hFinalTorque );
    }
    break;

  case MC_PROTOCOL_CODE_SET_CURRENT_REF:
    {
      int16_t hIqRef;
      int16_t hIdRef;
      hIqRef = buffer[0] + (buffer[1] << 8);
      hIdRef = buffer[2] + (buffer[3] << 8);
      UI_SetCurrentReferences(&pHandle->_Super, hIqRef, hIdRef);
      bNoError = true;
    }
    break;

  case MC_PROTOCOL_CODE_GET_MP_INFO:
    {
      MPInfo_t stepList;
      stepList.data = buffer;
      stepList.len = Size;
      bErrorCode = ERROR_MP_NOT_ENABLED;
      bNoError = UI_GetMPInfo(&stepList, &MPInfo);

      if (bNoError)
      {
        pHandle->fFcpSend(pHandle->pFCP, ACK_NOERROR, MPInfo.data, MPInfo.len);
        RequireAck = false;
      }
    }
    break;

    case MC_PROTOCOL_CODE_GET_FW_VERSION:
    {
      /* Get Firmware Version */
      unsigned char i;
      uint8_t outBuff[32];
      for (i = 0; (i<32) && (pHandle->s_fwVer[i]!=0); i++)
      {
        outBuff[i] = pHandle->s_fwVer[i];
      }

      for (; i < 32; i++)
      {
        outBuff[i] = 0;
      }

      pHandle->fFcpSend(pHandle->pFCP, ACK_NOERROR, outBuff, 32 );
      bNoError = true;
    }
    break;

    case DEFINE_ESCOOTER_BEHAVIOR:
    {
    	EScooter_Behavior_t behaviorID = (EScooter_Behavior_t) buffer[0];
    	switch(behaviorID)
    	{
    	   case BOOT_ACK:
    	   {
    		   /*$2E$01$00$2F*/
    		   RequireAck = false;
    		   bNoError = true;
    		   uint8_t bootDone = 0x01;
    		   pHandle -> fFcpSend(pHandle->pFCP, ACK_NOERROR,&bootDone,1);
    	   }
    	   break;

    	   case ERROR_REPORT:
    	   {
    		   /*$2E$01$01$30*/
    		   RequireAck = false;
    		   bNoError = true;
    		   uint8_t report = 0;
    		   report++;
    		   uint8_t errorReport = GET_MOTOR_ERROR_REPORT();
    		   updateConnectionStatus(true,report);
    		   pHandle -> fFcpSend(pHandle->pFCP, ACK_NOERROR, &errorReport, 1);
    	   }
    	   break;

    	   case THORTTLE_TRIGGER:
    	   {
    		   /*$2E$01$02$31*/
    		   RequireAck = false;
    		   bNoError = true;
    		   uint8_t twistedThorttle = 0x03;
    		   pHandle -> fFcpSend(pHandle->pFCP, ACK_NOERROR,&twistedThorttle,1);
    	   }
    	   break;

    	   case BRAKE_PRESS:
    	   {
    		   /*$2E$01$03$32*/
    		   RequireAck = false;
    		   bNoError = true;
    		   uint8_t pressBrake = 0x04;
    		   updateBrakeStatus(true);
    		   pHandle -> fFcpSend(pHandle->pFCP, ACK_NOERROR,&pressBrake,1);
    	   }
    	   break;

    	   case BRAKE_RELEASE:
    	   {
    		   /*$2E$01$04$33*/
    		   RequireAck = false;
    		   bNoError = true;
    		   uint8_t releaseBrake = 0x05;
    		   updateBrakeStatus(false);
    		   pHandle -> fFcpSend(pHandle->pFCP, ACK_NOERROR,&releaseBrake,1);
    	   }
    	   break;

    	   case TAIL_LIGHT_TOGGLE:
    	   {
    		   /*$2E$01$05$34*/
    		   RequireAck = false;
    		   bNoError = true;
    		   uint8_t toggle = 0x06;
    		   /*Toggles the Light*/
    		   pHandle -> fFcpSend(pHandle->pFCP, ACK_NOERROR,&toggle,1);
    	   }
    	   break;

    	   case TAIL_LIGHT_OFF:
    	   {
    		   /*$2E$01$06$35*/
    		   RequireAck = false;
    		   bNoError = true;
    		   uint8_t lightOff = 0x07;
    		   /*Off the tail light*/
    		   set_tail_light_status(0x00);
    		   pHandle -> fFcpSend(pHandle->pFCP, ACK_NOERROR,&lightOff,1);
    	   }
    	   break;

    	   case SHUT_DOWN:
    	   {
    		   /*$2E$01$07$36*/
    		   RequireAck = false;
    		   bNoError = true;
    		   changePowerMode();
    	   }
    	   break;

    	   case TAIL_LIGHT_ON:
    	   {
    		   /*$2E$01$08$37*/
    		   RequireAck = false;
    		   bNoError = true;
    		   uint8_t lightOn = 0x09;
    		   /*On the tail light*/
    		   set_tail_light_status(0x01);
    		   pHandle -> fFcpSend(pHandle->pFCP, ACK_NOERROR,&lightOn,1);
    	   }
    	   break;

    	   case TIMEOUT_CHECKING:
    	   {
    		   /*$2E$01$09$38*/
    		   RequireAck = false;
    		   bNoError = true;
    		   uint8_t timeout = 0x0A;
    		   uart++;
    		   updateConnectionStatus(true,uart);
    		   pHandle -> fFcpSend(pHandle->pFCP, ACK_NOERROR,&timeout,1);
    	   }
    	   break;

    	   /*Get Motor Driver Temperature*/
    	   case MOTOR_DRIVER_TEMP:
    	   {
    		   /*$2E$01$0A$39*/
    		   RequireAck = false;
    		   bNoError = true;
    		   int32_t motorDriverTemp = getDriverTemperature(); /*int32_t format*/
    		   pHandle -> fFcpSend(pHandle->pFCP, ACK_NOERROR,(uint8_t*)&motorDriverTemp,4);
    	   }
    	   break;

    	   /*Get Motor Temperature*/
    	   case MOTOR_TEMP:
    	   {
    		   /*$2E$01$0B$3A*/
    		   RequireAck = false;
    		   bNoError = true;
    		   //int32_t motor_temp = getMotorTemperature(); /*int32_t format*/
    		   uint8_t fake = 0x0A;
    		   pHandle -> fFcpSend(pHandle->pFCP, ACK_NOERROR,&fake,1); //32-bit variable for motor temperature
    	   }
    	   break;

    	   case CURRENT_CHECKING:
    	   {
    		   /*$2E$01$0C$3B*/
    		   RequireAck = false;
    		   bNoError = true;
    		   int32_t current = getDC(); /*int32_t format*/
    		   pHandle -> fFcpSend(pHandle->pFCP, ACK_NOERROR,(uint8_t*)(&current),4); //32-bit variable for current
    	   }
    	   break;

    	   case BATTERY_VOLTAGE:
    	   {
    		   /*$2E$01$0D$3C*/
    		   RequireAck = false;
    		   bNoError = true;
    		   int32_t BatteryVoltage = getBatteryVoltage(); /*int32_t format*/
    		   pHandle -> fFcpSend(pHandle->pFCP, ACK_NOERROR,(uint8_t*)(&BatteryVoltage),4);
    	   }
    	   break;

    	   case OBD_MODE:
    	   {
    		   /*$2E$01$0E$3D*/
    		   RequireAck = false;
    		   bNoError = true;
    		   enterOBD();
    	   }
    	   break;

    	   default:
    	   {
    		   uint8_t invalidCMD = 0xFF;
    		   pHandle -> fFcpSend(pHandle->pFCP, ACK_NOERROR,&invalidCMD,1);
    	   }
    	   break;

    	}
    }
    break;

    case TOGGLE_SPEED_MODE:
     {
         /*Decode the input dummy data, pass them to the API or UART2 fcpSend*/
     	  /*Example :
     	   * CMD LIST --> Configure different driving modes! !:
     	   * Sports  : $33$0A$86$3D$00$00$97$02$00$00$D0$07$72
     	   * Leisure : $33$0A$BA$2C$00$00$E0$01$00$00$B8$0B$C9
     	   * Amble   : $33$0A$64$19$00$00$0E$01$00$00$A0$0F$79
     	   * CMD is sent by the Dash-board
     	   * Those commands are defined and encoded through STM32MCP the protocol in CC2640 / Master Devices
     	   * */
  	   bNoError = true;
  	   RequireAck = false;
  	   uint8_t changeDone = 0x13;
  	   int16_t  speed_mode_IQmax = buffer[0] + (buffer[1] << 8) + (buffer[2] << 16) + (buffer[3] << 24);
  	   int16_t  allowable_rpm    = buffer[4] + (buffer[5] << 8) + (buffer[6] << 16) + (buffer[7] << 24);
  	   uint16_t ramp             = buffer[8] + (buffer[9] << 8);
  	   changeSpeedMode(speed_mode_IQmax,allowable_rpm,ramp);
  	   pHandle -> fFcpSend(pHandle->pFCP, ACK_NOERROR,&changeDone,1);
     }
     break;

    case APPLY_THROTTLE_IQ:
     {
  	   bNoError = true;
  	   RequireAck = false;
  	   uint8_t IQReceived = 0x14;
  	   uint16_t throttlePercentage = buffer[0] + (buffer[1] << 8) + (buffer[2] << 16) + (buffer[3] << 24);
  	   int16_t IQ_value            = buffer[4] + (buffer[5] << 8) + (buffer[6] << 16) + (buffer[7] << 24);
  	   /*Iq Value to the motor controller*/
  	   setIQ(IQ_value);
  	   /*Percentage of throttle twisted*/
  	   set_ThrottlePercent(throttlePercentage);
  	   pHandle -> fFcpSend(pHandle->pFCP, ACK_NOERROR,&IQReceived,1);
     }
     break;

    case ON_BOARD_DIAGNOSTIC_MODE:
    {
    	ETU_OBD_t behaviorID = (ETU_OBD_t)buffer[0];
    	switch(behaviorID)
    	{
    	   case OBD_DRIVER_CHECK:
    		   break;

    	   case OBD_MOTOR_CHECK:
    		   break;

    	   case OBD_FAULT_CHECK:
    		   break;

    	   case OBD_THROTTLE_BRAKE_CHECK:
    		   break;

    	   case OBD_TIMEOUT:
    		   break;

    	   default:
    		   break;
    	}
    }
    break;

  case MC_PROTOCOL_CODE_NONE:
    {
    }
    break;

  default:
    {
      bErrorCode = ERROR_BAD_FRAME_ID;
    }
    break;
  }

  if (RequireAck)
  {
    if (bNoError)
    {
      pHandle->fFcpSend(pHandle->pFCP, ACK_NOERROR, MC_NULL, 0);
    }
    else
    {
      pHandle->fFcpSend(pHandle->pFCP, ACK_ERROR, &bErrorCode, 1);
    }
  }
}

/**
* @brief  Allow to report the overrun error message.
*
* Called when received frame has not been received correctly due to the
* transmission speed too fast.
*
* @param  pHandle Pointer on the handle of the component.
*/
__weak void MCP_SendOverrunMessage(MCP_Handle_t *pHandle)
{
  uint8_t bErrorCode = ERROR_CODE_OVERRUN;
  pHandle->fFcpSend(pHandle->pFCP, ACK_ERROR, &bErrorCode, 1);
}

/**
* @brief  Allow to report the time out error message.
*
* @param  pHandle Pointer on the handle of the component.
*/
__weak void MCP_SendTimeoutMessage(MCP_Handle_t *pHandle)
{
  uint8_t bErrorCode = ERROR_CODE_TIMEOUT;
  pHandle->fFcpSend(pHandle->pFCP, ACK_ERROR, &bErrorCode, 1);
}

/**
* @brief  Allow to send an ATR message.
*
* @param  pHandle Pointer on the handle of the component.
*/
__weak void MCP_SendATRMessage(MCP_Handle_t *pHandle)
{
  uint32_t wUID = UI_GetReg(&pHandle->_Super, MC_PROTOCOL_REG_UID, MC_NULL);
  unsigned char i;
  uint8_t bFWX;
  uint8_t bFWY;
  uint8_t bFWZ;
  uint16_t hCBDID = UI_GetReg( &pHandle->_Super, MC_PROTOCOL_REG_CTRBDID, MC_NULL );
  uint16_t hPBDID = UI_GetReg( &pHandle->_Super, MC_PROTOCOL_REG_PWBDID, MC_NULL );
  uint16_t hPBDID2 = UI_GetReg( &pHandle->_Super, MC_PROTOCOL_REG_PWBDID2, MC_NULL );
  uint8_t buff[13]={0,0,0,0,0,0,0,0,0,0,0,0,0};

  for (i = 0; (i<29) && (pHandle->s_fwVer[i]!='\t'); i++);

  bFWX = pHandle->s_fwVer[i+5];
  bFWY = pHandle->s_fwVer[i+7];
  bFWZ = pHandle->s_fwVer[i+9];

  //*(uint32_t*)(&buff[0]) = wUID;
  buff[0] = (uint8_t)(wUID & 0xff);
  buff[1] = (uint8_t)((wUID >> 8)  & 0xff);
  buff[2] = (uint8_t)((wUID >> 16) & 0xff);
  buff[3] = (uint8_t)((wUID >> 24) & 0xff);
  buff[4] = bFWX;
  buff[5] = bFWY;
  buff[6] = bFWZ;
  buff[7] = (uint8_t)(hCBDID);
  buff[8] = (uint8_t)(hCBDID>>8);
  buff[9] = (uint8_t)(hPBDID);
  buff[10]= (uint8_t)(hPBDID>>8);
  buff[11] = (uint8_t)(hPBDID2);
  buff[12]= (uint8_t)(hPBDID2>>8);

  pHandle->fFcpSend(pHandle->pFCP, ATR_FRAME_START, buff, 13);
}

/**
* @brief  Allow to send back a BAD CRC message.
*
* @param  pHandle Pointer on the handle of the component.
*/
__weak void MCP_SendBadCRCMessage(MCP_Handle_t *pHandle)
{
  uint8_t bErrorCode = ERROR_CODE_BAD_CRC;
  pHandle->fFcpSend(pHandle->pFCP, ACK_ERROR, &bErrorCode, 1);
}

/**
  * @}
  */

/**
  * @}
  */

/**
 * @}
 */

/**
 * @}
 */

/************************ (C) COPYRIGHT 2019 STMicroelectronics *****END OF FILE****/
