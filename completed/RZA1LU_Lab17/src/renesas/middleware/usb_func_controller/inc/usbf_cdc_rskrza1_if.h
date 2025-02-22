/*******************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only
 * intended for use with Renesas products. No other uses are authorized. This
 * software is owned by Renesas Electronics Corporation and is protected under
 * all applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
 * LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
 * TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
 * ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
 * FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
 * ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
 * BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software
 * and to discontinue the availability of this software. By using this
 * software, you agree to the additional terms and conditions found by
 * accessing the following link:
 * http://www.renesas.com/disclaimer
*******************************************************************************
* Copyright (C) 2018 Renesas Electronics Corporation. All rights reserved.
 *****************************************************************************/
/******************************************************************************
 * @headerfile     r_usb_cdc.h
 * @brief          USB Port 1 CDC driver hardware interface
 * @warning        Not in Use
 * @version        1.00
 * @date           27.06.2018
 * H/W Platform    RZ/A1LU
 *****************************************************************************/
 /*****************************************************************************
 * History      : DD.MM.YYYY Ver. Description
 *              : 01.02.2015 1.00 First Release
 *****************************************************************************/
/* Multiple inclusion prevention macro */
#ifndef R_USBF_CDC_RZA1_DRV_IF_H_INCLUDED
#define R_USBF_CDC_RZA1_DRV_IF_H_INCLUDED

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "devlink.h"
#include "r_usb_cdc.h"

/**************************************************************************//**
 * @ingroup R_SW_PKG_93_USB_CDC 
 * @{
 *****************************************************************************/
/******************************************************************************
Structures
******************************************************************************/

/******************************************************************************
Driver Interface
******************************************************************************/
/******************************************************************************
 Refer to the appropriate c file

const DEVICE g_usbf0_hid_driver =
{ "USB Func HID Port 0 Device Driver",
   usbf_hid_open,
   usbf_hid_close,
   usbf_hid_read,
   usbf_hid_write,
   usbf_hid_control
};

******************************************************************************
 Control commands supported in this driver
 CTLCODE                     p_ctl_struct required
 =======                     =====================

 CTL_USBF_IS_CONNECTED       NONE
 CTL_USBF_SET_RW_MODE        st_usbf_asyn_config_t
 CTL_USBF_SET_CONFIGURATION  st_usbf_user_configuration_t
 CTL_USBF_GET_CONFIGURATION  st_usbf_user_configuration_t
 CTL_USBF_START              NONE
 CTL_USBF_STOP               NONE 
 *****************************************************************************/

#endif /* R_USBF_CDC_RZA1_DRV_IF_H_INCLUDED */
/**************************************************************************//**
 * @} (end addtogroup)
 *****************************************************************************/
/******************************************************************************
End  Of File
******************************************************************************/
