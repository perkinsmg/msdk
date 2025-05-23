/**
 * @file    descriptors.h
 * @brief   descriptors listed here
 */

/******************************************************************************
 *
 * Copyright (C) 2022-2023 Maxim Integrated Products, Inc. (now owned by
 * Analog Devices, Inc.),
 * Copyright (C) 2023-2024 Analog Devices, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/

/* Define to prevent redundant inclusion */
#ifndef EXAMPLES_MAX78002_USB_MAXUSB_USB_CDCACM_DESCRIPTORS_H_
#define EXAMPLES_MAX78002_USB_MAXUSB_USB_CDCACM_DESCRIPTORS_H_

/* **** Includes **** */
#include <stdint.h>
#include "usb.h"
#include "hid_kbd.h"

#ifdef __cplusplus
extern "C" {
#endif

/* **** Definitions **** */

MXC_USB_device_descriptor_t __attribute__((aligned(4))) device_descriptor = {
    0x12, /* bLength = 18                     */
    0x01, /* bDescriptorType = Device         */
    0x0200, /* bcdUSB USB spec rev (BCD)        */
    0x02, /* bDeviceClass = comm class (2)    */
    0x00, /* bDeviceSubClass                  */
    0x00, /* bDeviceProtocol                  */
    0x40, /* bMaxPacketSize0 is 64 bytes      */
    0x0B6A, /* idVendor (Maxim Integrated)      */
    0x003C, /* idProduct                        */
    0x0100, /* bcdDevice                        */
    0x01, /* iManufacturer Descriptor ID      */
    0x02, /* iProduct Descriptor ID           */
    0x00, /* iSerialNumber = (0) No string    */
    0x01 /* bNumConfigurations               */
};

/* Device qualifier needed for high-speed operation */
MXC_USB_device_qualifier_descriptor_t __attribute__((aligned(4))) device_qualifier_descriptor = {
    0x0A, /* bLength = 10                       */
    0x06, /* bDescriptorType = Device Qualifier */
    0x0200, /* bcdUSB USB spec rev (BCD)          */
    0x02, /* bDeviceClass = Unspecified         */
    0x00, /* bDeviceSubClass                    */
    0x00, /* bDeviceProtocol                    */
    0x40, /* bMaxPacketSize0 is 64 bytes        */
    0x01, /* bNumConfigurations                 */
    0x00 /* Reserved, must be 0                */
};

__attribute__((aligned(4))) struct __attribute__((packed)) {
    MXC_USB_configuration_descriptor_t config_descriptor;
    /* Interface #1 CDCACM Device */
    MXC_USB_interface_descriptor_t comm_interface_descriptor;
    uint8_t header_functional_descriptor[5];
    uint8_t call_management_descriptor[5];
    uint8_t acm_functional_descriptor[4];
    uint8_t union_functional_descriptor[5];
    /* Interface #2 CDC Data*/
    MXC_USB_endpoint_descriptor_t endpoint_descriptor_3;
    MXC_USB_interface_descriptor_t data_interface_descriptor;
    MXC_USB_endpoint_descriptor_t endpoint_descriptor_4;
    MXC_USB_endpoint_descriptor_t endpoint_descriptor_5;
}

config_descriptor = {
    {
        0x09, /*  bLength = 9                     */
        0x02, /*  bDescriptorType = Config (2)    */
        0x0043, /*  wTotalLength(L/H)               */
        0x02, /*  bNumInterfaces                  */
        0x01, /*  bConfigValue                    */
        0x02, /*  iConfiguration                  */
        0xE0, /*  bmAttributes (self-powered, remote wakeup) */
        0x01, /*  MaxPower is 2ma (units are 2ma/bit) */
    },
    /********** Interface #1 : COMM Interface **********/
    {
        /*  First Interface Descriptor For Comm Class Interface */
        0x09, /*  bLength = 9                     */
        0x04, /*  bDescriptorType = Interface (4) */
        0x00, /*  bInterfaceNumber                */
        0x00, /*  bAlternateSetting               */
        0x01, /*  bNumEndpoints (one for OUT)     */
        0x02, /*  bInterfaceClass = Communications Interface Class (2) */
        0x02, /*  bInterfaceSubClass = Abstract Control Model (2) */
        0x01, /*  bInterfaceProtocol = Common "AT" commands (1), no class specific protocol (0) */
        0x04, /*  iInterface                      */
    },
    {
        /*  Header Functional Descriptor */
        0x05, /*  bFunctionalLength = 5           */
        0x24, /*  bDescriptorType                 */
        0x00, /*  bDescriptorSubtype              */
        0x10, 0x01, /*  bcdCDC                          */
    },
    {
        /*  Call Management Descriptor */
        0x05, /*  bFunctionalLength = 5           */
        0x24, /*  bDescriptorType                 */
        0x01, /*  bDescriptorSubtype              */
        0x03, /*  bmCapabilities = Device handles call management itself (0x01), management over data class (0x02) */
        0x01, /*  bmDataInterface                 */
    },
    {
        /*  Abstract Control Management Functional Descriptor */
        0x04, /*  bFunctionalLength = 4           */
        0x24, /*  bDescriptorType                 */
        0x02, /*  bDescriptorSubtype              */
        0x02, /*  bmCapabilities                  */
    },
    {
        /*  Union Functional Descriptor */
        0x05, /*  bFunctionalLength = 5           */
        0x24, /*  bDescriptorType                 */
        0x06, /*  bDescriptorSubtype              */
        0x00, /*  bmMasterInterface               */
        0x01, /*  bmSlaveInterface0               */
    },
    {
        /*  IN Endpoint 3 (Descriptor #1) */
        0x07, /*  bLength                          */
        0x05, /*  bDescriptorType (Endpoint)       */
        0x84, /*  bEndpointAddress (EP3-IN)        */
        0x03, /*  bmAttributes (interrupt)         */
        0x0040, /*  wMaxPacketSize                   */
        0xff, /*  bInterval (milliseconds)         */
    },
    {
        /*  Second Interface Descriptor For Data Interface */
        0x09, /*  bLength                          */
        0x04, /*  bDescriptorType (Interface)      */
        0x01, /*  bInterfaceNumber                 */
        0x00, /*  bAlternateSetting                */
        0x02, /*  bNumEndpoints                    */
        0x0a, /*  bInterfaceClass = Data Interface (10) */
        0x00, /*  bInterfaceSubClass = none (0)    */
        0x00, /*  bInterfaceProtocol = No class specific protocol (0) */
        0x04, /*  biInterface = No Text String (0) */
    },
    {
        /*  OUT Endpoint 1 (Descriptor #2) */
        0x07, /*  bLength                          */
        0x05, /*  bDescriptorType (Endpoint)       */
        0x05, /*  bEndpointAddress (EP1-OUT)       */
        0x02, /*  bmAttributes (bulk)              */
        0x0040, /*  wMaxPacketSize                   */
        0x00, /*  bInterval (N/A)                  */
    },
    {
        /*  IN Endpoint 2 (Descriptor #3) */
        0x07, /*  bLength                          */
        0x05, /*  bDescriptorType (Endpoint)       */
        0x83, /*  bEndpointAddress (EP2-IN)        */
        0x02, /*  bmAttributes (bulk)              */
        0x0040, /*  wMaxPacketSize                   */
        0x00 /*  bInterval (N/A)                  */
    }
};

__attribute__((aligned(4))) struct __attribute__((packed)) {
    MXC_USB_configuration_descriptor_t config_descriptor;
    /* Interface #1 CDCACM Device */
    MXC_USB_interface_descriptor_t comm_interface_descriptor;
    uint8_t header_functional_descriptor[5];
    uint8_t call_management_descriptor[5];
    uint8_t acm_functional_descriptor[4];
    uint8_t union_functional_descriptor[5];
    MXC_USB_endpoint_descriptor_t endpoint_descriptor_3;
    /* Interface #2 CDCACM Data */
    MXC_USB_interface_descriptor_t data_interface_descriptor;
    MXC_USB_endpoint_descriptor_t endpoint_descriptor_4;
    MXC_USB_endpoint_descriptor_t endpoint_descriptor_5;
}

config_descriptor_hs = {
    {
        0x09, /*  bLength                          */
        0x02, /*  bDescriptorType = Config         */
        0x0043, /*  wTotalLength(L/H)                */
        0x02, /*  bNumInterfaces                   */
        0x01, /*  bConfigurationValue              */
        0x02, /*  iConfiguration                   */
        0xE0, /*  bmAttributes (bus-powered, remote wakeup) */
        0x32, /*  MaxPower is 100ma (units are 2ma/bit) */
    },
    /********** Interface #1 : COMM Interface **********/
    {
        /*  First Interface Descriptor For Comm Class Interface */
        0x09, /*  bLength = 9                     */
        0x04, /*  bDescriptorType = Interface (4) */
        0x00, /*  bInterfaceNumber                */
        0x00, /*  bAlternateSetting               */
        0x01, /*  bNumEndpoints (one for OUT)     */
        0x02, /*  bInterfaceClass = Communications Interface Class (2) */
        0x02, /*  bInterfaceSubClass = Abstract Control Model (2) */
        0x01, /*  bInterfaceProtocol = Common "AT" commands (1), no class specific protocol (0) */
        0x04, /*  iInterface                      */
    },
    {
        /*  Header Functional Descriptor */
        0x05, /*  bFunctionalLength = 5           */
        0x24, /*  bDescriptorType                 */
        0x00, /*  bDescriptorSubtype              */
        0x10, 0x01, /*  bcdCDC                          */
    },
    {
        /*  Call Management Descriptor */
        0x05, /*  bFunctionalLength = 5           */
        0x24, /*  bDescriptorType                 */
        0x01, /*  bDescriptorSubtype              */
        0x03, /*  bmCapabilities = Device handles call management itself (0x01), management over data class (0x02) */
        0x01, /*  bmDataInterface                 */
    },
    {
        /*  Abstract Control Management Functional Descriptor */
        0x04, /*  bFunctionalLength = 4           */
        0x24, /*  bDescriptorType                 */
        0x02, /*  bDescriptorSubtype              */
        0x02, /*  bmCapabilities                  */
    },
    {
        /*  Union Functional Descriptor */
        0x05, /*  bFunctionalLength = 5           */
        0x24, /*  bDescriptorType                 */
        0x06, /*  bDescriptorSubtype              */
        0x00, /*  bmMasterInterface               */
        0x01, /*  bmSlaveInterface0               */
    },
    {
        /*  IN Endpoint 3 (Descriptor #1) */
        0x07, /*  bLength                          */
        0x05, /*  bDescriptorType (Endpoint)       */
        0x84, /*  bEndpointAddress (EP3-IN)        */
        0x03, /*  bmAttributes (interrupt)         */
        0x0200, /*  wMaxPacketSize                   */
        0xff, /*  bInterval (milliseconds)         */
    },
    {
        /*  Second Interface Descriptor For Data Interface */
        0x09, /*  bLength                          */
        0x04, /*  bDescriptorType (Interface)      */
        0x01, /*  bInterfaceNumber                 */
        0x00, /*  bAlternateSetting                */
        0x02, /*  bNumEndpoints                    */
        0x0a, /*  bInterfaceClass = Data Interface (10) */
        0x00, /*  bInterfaceSubClass = none (0)    */
        0x00, /*  bInterfaceProtocol = No class specific protocol (0) */
        0x04, /*  biInterface = No Text String (0) */
    },
    {
        /*  OUT Endpoint 1 (Descriptor #2) */
        0x07, /*  bLength                          */
        0x05, /*  bDescriptorType (Endpoint)       */
        0x05, /*  bEndpointAddress (EP1-OUT)       */
        0x02, /*  bmAttributes (bulk)              */
        0x0200, /*  wMaxPacketSize                   */
        0x00, /*  bInterval (N/A)                  */
    },
    {
        /*  IN Endpoint 2 (Descriptor #3) */
        0x07, /*  bLength                          */
        0x05, /*  bDescriptorType (Endpoint)       */
        0x83, /*  bEndpointAddress (EP2-IN)        */
        0x02, /*  bmAttributes (bulk)              */
        0x0200, /*  wMaxPacketSize                   */
        0x00, /*  bInterval (N/A)                  */
    },
};

__attribute__((aligned(4))) uint8_t lang_id_desc[] = {
    0x04, /* bLength */
    0x03, /* bDescriptorType */
    0x09, 0x04 /* bString = wLANGID (see usb_20.pdf 9.6.7 String) */
};

__attribute__((aligned(4))) uint8_t mfg_id_desc[] = {
    0x22, /* bLength */
    0x03, /* bDescriptorType */
    'M',  0, 'a', 0, 'x', 0, 'i', 0, 'm', 0, ' ', 0, 'I', 0, 'n', 0,
    't',  0, 'e', 0, 'g', 0, 'r', 0, 'a', 0, 't', 0, 'e', 0, 'd', 0,
};

__attribute__((aligned(4))) uint8_t prod_id_desc[] = {
    0x22, /* bLength */
    0x03, /* bDescriptorType */
    'M',  0, 'A', 0, 'X', 0, '3', 0, '2', 0, '6', 0, '5', 0, '0', 0,
    ' ',  0, 'C', 0, 'D', 0, 'C', 0, '-', 0, 'A', 0, 'C', 0, 'M', 0,
};

/* Not currently used (see device descriptor), but could be enabled if desired */
__attribute__((aligned(4))) uint8_t serial_id_desc[] = { 0x14, /* bLength */
                                                         0x03, /* bDescriptorType */
                                                         '0',  0, '0', 0, '0', 0, '0', 0, '0', 0,
                                                         '0',  0, '0', 0, '0', 0, '1', 0 };

__attribute__((aligned(4))) uint8_t cdcacm_func_desc[] = {
    0x20, /* bLength */
    0x03, /* bDescriptorType */
    'M',  0,   'A', 0,   'X', 0,   '3', 0,   '2', 0,   '6', 0,   '5', 0,   '0',
    0,    ' ', 0,   'C', 0,   'D', 0,   'C', 0,   'A', 0,   'C', 0,   'M', 0,
};

#ifdef __cplusplus
}
#endif

#endif // EXAMPLES_MAX78002_USB_MAXUSB_USB_CDCACM_DESCRIPTORS_H_
