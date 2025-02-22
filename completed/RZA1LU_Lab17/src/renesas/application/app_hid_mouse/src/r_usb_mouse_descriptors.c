/******************************************************************************
* DISCLAIMER                                                                      
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized.
* This software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES
* REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY,
* INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
* PARTICULAR PURPOSE AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY
* DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES
* FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS
* AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this
* software and to discontinue the availability of this software.  
* By using this software, you agree to the additional terms and
* conditions found by accessing the following link:
* http://www.renesas.com/disclaimer
******************************************************************************/
/* Copyright (C) 2010 Renesas Electronics Corporation. All rights reserved.*/

/******************************************************************************
* File Name       : usbdescriptors.c
* Version         : 1.00
* Device          : RZA1(H)
* Tool Chain      : HEW, Renesas SuperH Standard Tool chain v9.3
* H/W Platform    : RSK2+SH7269
* Description     : Descriptors required to enumerate a device as a USB HID Class.
*                   This sets up a Report IN and a Report OUT.
* 
*                   NOTE: This will need to be modified for a particular
*                   product as it includes company/product specific data including
*                   string descriptors specifying
*                   Manufacturer, Product and Serial Number.
******************************************************************************/

/******************************************************************************
* History         : 12.11.2009 Ver. 1.00 First Release
******************************************************************************/

/***********************************************************************************
User Includes
***********************************************************************************/
/*    Following header file provides definition for Low level driver. */
#include "r_usb_hal.h"
/*    Following header file provides definition for usb_core.c. */
#include "r_usbf_core.h"
/*    Following header file provides USB descriptor information. */
#include "r_usb_mouse_descriptors.h"

/***********************************************************************************
Macro Defines
***********************************************************************************/
/*Vendor and Product ID*/
/*NOTE Please use your company Vendor ID when developing a new product.*/
#define VID (0x0000)
#define PID (0x0023)

/* Descxriptor sizes */
#define DEVICE_DESCRIPTOR_SIZE    (18)
#define CONFIG_DESCRIPTOR_SIZE_TOTAL    (34)
#define STRING_MANUFACTURER_SIZE (16)
#define STRING_PRODUCT_SIZE (32)

#define STRING_SERIAL_NUM_SIZE (10)
#define DEVICE_QUALIFIER_DESCRIPTOR_SIZE    (10)
#define OTHER_SPEED_CONFIG_DESCRIPTOR_SIZE_TOTAL    (34)

/***********************************************************************************
Global Variables
***********************************************************************************/
/**    Device Descriptor    */
static const uint8_t m_device_descriptor_data[DEVICE_DESCRIPTOR_SIZE + (DEVICE_DESCRIPTOR_SIZE % 2)] =
{
    /** Size of this descriptor*/
    DEVICE_DESCRIPTOR_SIZE,

    /** Device Descriptor*/
    0x01,

    /** USB Version 2.0*/
    0x00,0x02,

    /** Class Code - None as HID is defined in the Interface Descriptor*/
    0x00,

    /** Subclass Code*/
    0x00,

    /** Protocol Code*/
    0x00,

    /** Max Packet Size for endpoint 0*/
    CONTROL_IN_PACKET_SIZE,

    /** Vendor ID LSB*/
    (uint8_t)(VID & 0xFF),

    /** Vendor ID MSB*/
    (uint8_t)((VID>>8)& 0xFF),

    /** Product ID LSB*/
    (uint8_t)(PID & 0xFF),

    /** Product ID MSB*/
    (uint8_t)((PID>>8)& 0xFF),

    /** Device Release Number*/
    0x00,0x01,

    /** Manufacturer String Descriptor*/
    STRING_iMANUFACTURER,

    /** Product String Descriptor*/
    STRING_iPRODUCT,

    /** Serial Number String Descriptor*/
    STRING_iSERIAL,

    /** Number of Configurations supported*/
    0x01
};

/*
Configuration Descriptor
For HID this includes Interface Descriptors, HID descriptor and endpoint descriptor.
Ensure START_INDEX_OF_HID_WITHIN_CONFIG_DESC and HID_DESCRIPTOR_SIZE
are defined according to this.  
*/
/** This includes Interfaces and Endpoints*/
static const uint8_t m_configuration_descriptor_data[CONFIG_DESCRIPTOR_SIZE_TOTAL + (CONFIG_DESCRIPTOR_SIZE_TOTAL % 2)] =
{
    /** Size of this descriptor (Just the configuration part)*/
    0x09,

    /** Configuration Descriptor*/
    0x02,

    /** Combined length of all descriptors (little endian)*/
    CONFIG_DESCRIPTOR_SIZE_TOTAL,0x00,

    /** Number of interfaces*/
    0x01,

    /** This Interface Value*/
    0x01,

    /** String Descriptor for this configuration */
    0x06,

    /** bmAttributes - Self Powered(USB bus powered), Remote Wakeup*/
    0xA0,

    /** bMaxPower (2mA units) 100mA (A unit load is defined as 100mA)*/
    50,


    /*INTERACE*/

    /** Size of this descriptor*/
    0x09,

    /** INTERFACE Descriptor*/
    0x04,

    /** Index of Interface*/
    0x00,

    /** bAlternateSetting*/
    0x00,

    /** Number of Endpoints*/
    0x01,

    /** HID Class code*/
    0x03,

    /** No Subclass*/
    0x00,

    /** No Protocol*/
    0x02,

    /** No String Descriptor for this interface*/
    0x00,

    /** HID Descriptor
     * Size of this descriptor*/
    0x09,

    /** HID Descriptor*/
    0x21,

    /** HID Class Specification Release Number 1.11*/
    0x00,0x01,

    /** No Target Country*/
    0x00,

    /** Number of HID Descriptors*/
    0x01,

    /** Type of HID Descriptor = "Report"*/
    0x22,

    /** Length Of HID Report Descriptor*/
    HID_MOUSE_REPORT_DESCRIPTOR_SIZE,0x00,

    /** Endpoint Descriptor 0
     * Size of this descriptor*/
    0x07,

    /** ENDPOINT Descriptor*/
    0x05,

    /** bEndpointAddress - IN endpoint, endpoint number = 6 */
    0x86,

    /** Endpoint Type is Interrupt*/
    0x03,

    /** Max Packet Size*/
    HID_IN_REPORT_SIZE,    0x00,

    /** Polling Interval in mS*/
    0x0A
    };

/**
Report Descriptor
NOTE The size of this must be HID_MOUSE_REPORT_DESCRIPTOR_SIZE
*/
static const uint8_t m_hid_report_descriptor_data[HID_MOUSE_REPORT_DESCRIPTOR_SIZE + (HID_MOUSE_REPORT_DESCRIPTOR_SIZE % 2)] =
{
    /** Usage Page (Generic Desktop Control) */
        0x05, 0x01,

    /** Usage (Mouse) */
        0x09, 0x02,

    /** Collection (Application) */
        0xA1, 0x01,

    /** Usage (Pointer) */
        0x09, 0x01,

    /** Collection (Physical) */
        0xA1, 0x00,

    /** Usage Page (Button) */
        0x05, 0x09,

    /** Usage Minimum (1) */
        0x19, 0x01,

    /** Usage Maximum (3) */
        0x29, 0x03,

    /** Logical Minimum (0) */
        0x15, 0x00,

    /** Logical Maximum (1) */
        0x25, 0x01,

    /** Report Count (3) */
        0x95, HID_IN_REPORT_SIZE,

    /** Report Size (1) */
        0x75, 0x01,

    /** Input (Data, Variable, Relative, Bitfield) */
        0x81, 0x02,

    /** Report Count */
        0x95, 0x01,

    /** Report Size */
        0x75, 0x05,

    /** Input (constant, Array, Absolute, Bitfield) */
        0x81, 0x01,

    /** Usage Page (Generic Desktop Controls */
        0x05, 0x01,

    /** Usage X */
        0x09, 0x30,

    /** Usage Y */
        0x09, 0x31,

    /** Logical Minimum */
        0x15, 0x81,

    /** Logical Maximum */
        0x25, 0x7F,

    /** Report Size */
        0x75, 0x08,

    /** Report Count */
        0x95, 0x02,

    /** Input (Data, Variable, Relative, Bitfield */
        0x81, 0x06,

    /** End collection */
        0xC0, 0xC0
};

/*String Descriptors*/
    /*Note Language ID is in USB Core */

/** Manufacturer string
 * "Renesas" */
static const uint8_t m_str_desc_manufacturer_data[STRING_MANUFACTURER_SIZE + (STRING_MANUFACTURER_SIZE % 2)] =
{
    /** Length of this descriptor*/
    STRING_MANUFACTURER_SIZE,

    /** Descriptor Type = STRING */
    0x03,

    /** Descriptor Text (unicode) */
    'R', 0x00, 'E', 0x00, 'N', 0x00, 'E', 0x00,
    'S', 0x00, 'A', 0x00, 'S', 0x00
};

/*Product string*/
/** "HID USB Demonstration" */
static const uint8_t m_string_desc_prod_data[STRING_PRODUCT_SIZE + (STRING_PRODUCT_SIZE % 2)] =
{
    /** Length of this descriptor*/
        STRING_PRODUCT_SIZE,

    /** Descriptor Type = STRING */
    0x03,

    /** Descriptor Text (unicode) */
    'U', 0x00, 'S', 0x00, 'B', 0x00, ' ', 0x00,
    'P', 0x00, 'e', 0x00, 'r', 0x00, 'i', 0x00,
    ' ', 0x00, 'H', 0x00, 'I', 0x00, 'D', 0x00,
    ' ', 0x00, 'F', 0x00, 'W', 0x00
};

/** Serial number string*/
static const uint8_t m_str_desc_serial_num_data[STRING_SERIAL_NUM_SIZE + (STRING_SERIAL_NUM_SIZE % 2)] = /* "1.1" */
{
    /** Length of this descriptor*/
    STRING_SERIAL_NUM_SIZE,

    /** Descriptor Type = STRING */
    0x03,

    /** Descriptor Text (unicode) */
    'P', 0x00,
    '0', 0x00,
    '0', 0x00,
    '1', 0x00
};

/**    Device Qualifier Descriptor    */
static const uint8_t m_device_qualifier_desc_data[DEVICE_QUALIFIER_DESCRIPTOR_SIZE + (DEVICE_QUALIFIER_DESCRIPTOR_SIZE % 2)] =
{
    /** Size of this descriptor*/
    DEVICE_QUALIFIER_DESCRIPTOR_SIZE,

    /** Device Qualifier Type*/
    0x06,

    /** USB Version 2.0*/
    0x00,0x02,

    /** Class Code - None as HID is defined in the Interface Descriptor*/
    0x00,

    /** Subclass Code*/
    0x00,

    /** Protocol Code*/
    0x00,

    /** Max Packet Size for Other Speed*/
    CONTROL_IN_PACKET_SIZE,

    /** Number of Configurations supported*/
    0x01,

    /** Reserved*/
    0x00

};

/*
Other Speed Configuration Descriptor
For HID this includes Interface Descriptors, HID descriptor and endpoint descriptor.
Ensure START_INDEX_OF_HID_WITHIN_CONFIG_DESC and HID_DESCRIPTOR_SIZE
are defined according to this.  
*/

/** This includes Interfaces and Endpoints*/
static const uint8_t m_other_speed_config_desc_data[OTHER_SPEED_CONFIG_DESCRIPTOR_SIZE_TOTAL + (OTHER_SPEED_CONFIG_DESCRIPTOR_SIZE_TOTAL % 2)] =
{
    /** Size of this descriptor (Just the configuration part)*/
    0x09,

    /** Other_speed_Configuration Type*/
    0x07,

    /** Combined length of all descriptors (little endian)
     * Total length of data returned*/
    OTHER_SPEED_CONFIG_DESCRIPTOR_SIZE_TOTAL,0x00,

    /** Number of interfaces*/
    0x01,

    /** This configuration Value*/
    0x02,

    /** Index of String Descriptor for this configuration */
    0x00,

    /** bmAttributes - Self Powered(USB bus powered), No Remote Wakeup*/
    0x80,

    /** bMaxPower (2mA units) 100mA (A unit load is defined as 100mA)*/
    50,

    /** INTERACE*/

    /** Size of this descriptor*/
    0x09,

    /** INTERFACE Descriptor*/
    0x04,

    /** Index of Interface*/
    0x00,

    /** bAlternateSetting*/
    0x00,

    /** Number of Endpoints*/
    0x01,

    /** HID Class code*/
    0x03,

    /** No Subclass*/
    0x00,

    /** No Protocol*/
    0x00,

    /** No String Descriptor for this interface*/
    0x00,

    /** Size of this descriptor*/
    0x09,

    /** HID Descriptor*/
    0x21,

    /** HID Class Specification Release Number 1.11*/
    0x11,0x01,

    /** No Target Country*/
    0x00,

    /** Number of HID Descriptors*/
    0x01,

    /** Type of HID Descriptor = "Report"*/
    0x22,

    /** Length Of HID Report Descriptor*/
    HID_MOUSE_REPORT_DESCRIPTOR_SIZE,0x00,
    
    /** Endpoint Descriptor 0
     * Size of this descriptor*/
    0x07,

    /** ENDPOINT Descriptor*/
    0x05,

    /** bEndpointAddress - IN endpoint, endpoint number = 6 */
    0x86,

    /** Endpoint Type is Interrupt*/
    0x03,

    /** Max Packet Size*/
    0x40,0x00,

    /** Polling Interval in mS*/
    0x0A
    };

static const uint8_t m_usbf_md_inreport[HID_MOUSE_IN_REPORT_SIZE + (HID_MOUSE_IN_REPORT_SIZE % 2)] = {};
static const uint8_t m_usbf_md_outreport[HID_MOUSE_OUTPUT_REPORT_SIZE + (HID_MOUSE_OUTPUT_REPORT_SIZE % 2)] = {};

const descriptor_t g_usbf_md_inreport =
{
    HID_MOUSE_IN_REPORT_SIZE,
    (uint8_t *)m_usbf_md_inreport
};

const descriptor_t g_usbf_md_outreport =
{
    HID_MOUSE_OUTPUT_REPORT_SIZE,
    (uint8_t *)m_usbf_md_outreport
};

const descriptor_t g_usbf_md_device_desc =
{
    DEVICE_DESCRIPTOR_SIZE,
    (uint8_t *)m_device_descriptor_data
};

const descriptor_t g_usbf_md_config_desc =
{
    CONFIG_DESCRIPTOR_SIZE_TOTAL,
    (uint8_t *)m_configuration_descriptor_data
};

const descriptor_t g_usbf_md_hid_report_descriptor =
{
    /* Must match value specified in HID descriptor! */
    HID_MOUSE_REPORT_DESCRIPTOR_SIZE,
    (uint8_t *)m_hid_report_descriptor_data
};

const descriptor_t g_usbf_md_str_desc_manufacturer =
{
    STRING_MANUFACTURER_SIZE,
    (uint8_t *)m_str_desc_manufacturer_data
};

const descriptor_t g_usbf_md_str_desc_product =
{
    STRING_PRODUCT_SIZE,
    (uint8_t *)m_string_desc_prod_data
};

const descriptor_t g_usbf_md_str_desc_serial_num =
{
    STRING_SERIAL_NUM_SIZE,
    m_str_desc_serial_num_data
};

const descriptor_t g_usbf_md_device_qualifier_desc =
{
    DEVICE_QUALIFIER_DESCRIPTOR_SIZE,
    (uint8_t *)m_device_qualifier_desc_data
};

const descriptor_t g_usbf_md_other_speed_cfg_desc =
{
    OTHER_SPEED_CONFIG_DESCRIPTOR_SIZE_TOTAL,
    (uint8_t *)m_other_speed_config_desc_data
};

