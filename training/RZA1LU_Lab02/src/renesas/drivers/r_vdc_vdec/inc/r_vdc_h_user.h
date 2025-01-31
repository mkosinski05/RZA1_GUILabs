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
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
* Copyright (C) 2012 - 2014 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/**************************************************************************//**
* @file         r_vdc_h_user.h
* @version      1.00
* $Rev: 99 $
* $Date:: 2017-04-10 20:56:26 +0900#$
* @brief        VDC driver user-defined header
******************************************************************************/

#ifndef R_VDC_H_USER_H
#define R_VDC_H_USER_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include    "r_typedefs.h"
#include    "iodefine.h"


/******************************************************************************
Macro definitions
******************************************************************************/
/*! Compilation switch to enable parameter check functions<br>
    If this definition is enabled, the parameter check in each API function will be performed. */
#define R_VDC_CHECK_PARAMETERS


/*! GBR to YCbCr conversion <br>
 *  Matrix coefficient (typical value) for SMPTE 293M
 *  - Y  =  0.299 * R + 0.587 * G + 0.114 * B
 *  - Cb = -0.169 * R - 0.331 * G + 0.500 * B
 *  - Cr =  0.500 * R - 0.419 * G - 0.081 * B
 *  @note
 *      Coefficient values are represented in 11-bit two's complement integer
 *      - DEFINITION = int(COEF * 256 + 0.5)
 */
typedef enum
{
    VDC_COLORCONV_Y_R  = (77u),
    VDC_COLORCONV_Y_G  = (150u),
    VDC_COLORCONV_Y_B  = (29u),
    VDC_COLORCONV_CB_R = (2005u),
    VDC_COLORCONV_CB_G = (1963u),
    VDC_COLORCONV_CB_B = (128u),
    VDC_COLORCONV_CR_R = (128u),
    VDC_COLORCONV_CR_G = (1941u),
    VDC_COLORCONV_CR_B = (2027u)
} vdc_colcnv_rgb_ycbcr_t;

/*! YCbCr to GBR conversion <br>
 *  Matrix coefficient (typical value) for SMPTE 293M
 *  - G = 1.0 * Y - 0.344 * Cb - 0.714 * Cr
 *  - B = 1.0 * Y + 1.772 * Cb +   0.0 * Cr
 *  - R = 1.0 * Y +   0.0 * Cb + 1.402 * Cr
 *  @note
 *      Coefficient values are represented in 11-bit two's complement integer
 *      - DEFINITION = int(COEF * 256 + 0.5)
 */
typedef enum
{
    VDC_COLORCONV_G_Y  = (256u),
    VDC_COLORCONV_G_CB = (1960u),
    VDC_COLORCONV_G_CR = (1865u),
    VDC_COLORCONV_B_Y  = (256u),
    VDC_COLORCONV_B_CB = (454u),
    VDC_COLORCONV_B_CR = (0u),
    VDC_COLORCONV_R_Y  = (256u),
    VDC_COLORCONV_R_CB = (0u),
    VDC_COLORCONV_R_CR = (359u)
} vdc_colcnv_ycbcr_rgb_t;

/*! Color matrix DC offset typical value */
#define     VDC_COLORCONV_DC_OFFSET        (128u)
/*! Color matrix coefficient value that represents one times */
#define     VDC_COLORCONV_1TIMES_GAIN      (256u)

/*! 1usec wait cycle for LVDS PLL */
#define     VDC_LVDS_PLL_WAIT_CYCLE        (103u/*38u*/)
/*! 200usec wait counter for LVDS PLL */
#define     VDC_LVDS_PLL_WAIT_200USEC      (10/*13400u*/)


#endif  /* R_VDC_H_USER_H */

