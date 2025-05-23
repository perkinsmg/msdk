/**
 * @file    main.c
 * @brief   SPI Master Demo
 * @details Shows Master loopback demo for QSPI1
 *          Read the printf() for instructions
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

/***** Includes *****/
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "mxc_device.h"
#include "mxc_delay.h"
#include "mxc_pins.h"
#include "nvic_table.h"
#include "uart.h"
#include "spi.h"
#include "dma.h"
#include "led.h"

/***** Preprocessors *****/
#define MASTERSYNC 1
#define MASTERASYNC 0
#define MASTERDMA 0

#if (!(MASTERSYNC || MASTERASYNC || MASTERDMA))
#error "You must set either MASTERSYNC or MASTERASYNC or MASTERDMA to 1."
#endif
#if ((MASTERSYNC && MASTERASYNC) || (MASTERASYNC && MASTERDMA) || (MASTERDMA && MASTERSYNC))
#error "You must select either MASTERSYNC or MASTERASYNC or MASTERDMA, not all 3."
#endif

/***** Definitions *****/
#define DATA_LEN 100 // Words
#define DATA_VALUE 0xA5A5 // This is for master mode only...
#define VALUE 0xFFFF
#define SPI_SPEED 100000 // Bit Rate

#define SPI MXC_SPI1
#define SPI_IRQ SPI1_IRQn

/***** Globals *****/
uint16_t rx_data[DATA_LEN];
uint16_t tx_data[DATA_LEN];
volatile int SPI_FLAG;
volatile uint8_t DMA_FLAG = 0;

/***** Functions *****/
void SPI0_IRQHandler(void)
{
    MXC_SPI_AsyncHandler(SPI);
}

void DMA0_IRQHandler(void)
{
    MXC_DMA_Handler(MXC_DMA0);
}

void DMA1_IRQHandler(void)
{
    MXC_DMA_Handler(MXC_DMA0);
    DMA_FLAG = 1;
}

void SPI_Callback(mxc_spi_req_t *req, int error)
{
    SPI_FLAG = error;
}

int main(void)
{
    int i, j, retVal, fail = 0;
    uint16_t temp;
    mxc_spi_req_t req;

    printf("\n************** SPI Loopback Demo ****************\n");
    printf("This example configures the SPI to send data between the MISO (AIN2/P0.18) and\n");
    printf("MOSI (AIN1/P0.17) pins.  Connect these two pins together.  This demo shows SPI\n");
    printf("sending different bit sizes each run through.  If successful, the LED1(green or blue) "
           "will\n");
    printf("illuminate.  If fails, the LED0(red) will illuminate.\n");
    printf("\nNote: some board versions of the MAX32665 won't print out the transaction\n");
    printf("status because the SPI and Console UART pins are shared.  Check the LEDs, as\n");
    printf("stated above, for the example status.\n");

    printf("\nThis demo shows Asynchronous, Synchronous and DMA transaction for SPI1\n");

    for (i = 1; i < 17; i++) {
        if (i == 1) { // Sending out 2 to 16 bits
            continue;
        }

        for (j = 0; j < DATA_LEN; j++) {
            tx_data[j] = DATA_VALUE;
        }

        // Configure the peripheral
        retVal = MXC_SPI_Init(SPI, 1, 0, 1, 0, SPI_SPEED, MAP_A);
        if (retVal != E_NO_ERROR) {
            printf("\nSPI INITIALIZATION ERROR\n");
            return retVal;
        }

        memset(rx_data, 0x0, DATA_LEN * sizeof(uint16_t));

        //SPI Request
        req.spi = SPI;
        req.txData = (uint8_t *)tx_data;
        req.rxData = (uint8_t *)rx_data;
        req.txLen = DATA_LEN;
        req.rxLen = DATA_LEN;
        req.ssIdx = 0;
        req.ssDeassert = 1;
        req.txCnt = 0;
        req.rxCnt = 0;
        req.completeCB = (spi_complete_cb_t)SPI_Callback;
        SPI_FLAG = 1;

        retVal = MXC_SPI_SetDataSize(SPI, i);
        if (retVal != E_NO_ERROR) {
            printf("\nSPI SET DATASIZE ERROR: %d\n", retVal);
            return retVal;
        }

        retVal = MXC_SPI_SetWidth(SPI, SPI_WIDTH_STANDARD);
        if (retVal != E_NO_ERROR) {
            printf("\nSPI SET WIDTH ERROR: %d\n", retVal);
            return retVal;
        }

#if MASTERSYNC
        MXC_SPI_MasterTransaction(&req);
#endif

#if MASTERASYNC
        MXC_NVIC_SetVector(SPI_IRQ, SPI0_IRQHandler);
        NVIC_EnableIRQ(SPI_IRQ);
        MXC_SPI_MasterTransactionAsync(&req);

        while (SPI_FLAG == 1) {}
#endif

#if MASTERDMA
        MXC_DMA_ReleaseChannel(0);
        MXC_DMA_ReleaseChannel(1);

        NVIC_EnableIRQ(DMA0_IRQn);
        NVIC_EnableIRQ(DMA1_IRQn);
        MXC_SPI_MasterTransactionDMA(&req, MXC_DMA0);

        while (DMA_FLAG == 0) {}
        DMA_FLAG = 0;
#endif

        uint8_t bits = MXC_SPI_GetDataSize(SPI);

        for (j = 0; j < DATA_LEN; j++) {
            if (bits <= 8) {
                if (j < (DATA_LEN / 2)) {
                    temp = VALUE >> (16 - bits);
                    temp = (temp << 8) | temp;
                    temp &= DATA_VALUE;
                    tx_data[j] = temp;
                } else if (j == (DATA_LEN / 2) && DATA_LEN % 2 == 1) {
                    temp = VALUE >> (16 - bits);
                    temp &= DATA_VALUE;
                    tx_data[j] = temp;
                } else {
                    tx_data[j] = 0x0000;
                }
            } else {
                temp = VALUE >> (16 - bits);
                temp &= DATA_VALUE;
                tx_data[j] = temp;
            }
        }

        // Compare Sent data vs Received data
        // Printf needs the Uart turned on since they share the same pins
        if (memcmp(rx_data, tx_data, sizeof(tx_data)) != 0) {
            printf("\n-->%2d Bits Transaction Failed\n", i);
            fail++;
            break;
        } else {
            printf("\n-->%2d Bits Transaction Successful\n", i);
        }

        retVal = MXC_SPI_Shutdown(SPI);
        if (retVal != E_NO_ERROR) {
            printf("\n-->SPI SHUTDOWN ERROR: %d\n", retVal);
            return retVal;
        }
    }

    if (fail != 0) {
        LED_On(0); // indicates FAIL
        printf("\nExample Failed\n");
        return E_FAIL;
    }

    LED_On(1); // indicates SUCCESS
    printf("\nExample Succeeded\n");
    return E_NO_ERROR;
}
