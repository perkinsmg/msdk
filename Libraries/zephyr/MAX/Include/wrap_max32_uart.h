/******************************************************************************
 *
 * Copyright (C) 2023-2025 Analog Devices, Inc.
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

#ifndef LIBRARIES_ZEPHYR_MAX_INCLUDE_WRAP_MAX32_UART_H_
#define LIBRARIES_ZEPHYR_MAX_INCLUDE_WRAP_MAX32_UART_H_

/***** Includes *****/
#include <uart.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(CONFIG_SOC_MAX32665) || defined(CONFIG_SOC_MAX32666) || \
    defined(CONFIG_SOC_MAX32650) || defined(CONFIG_SOC_MAX32660)
#if !defined(CONFIG_SOC_MAX32650) && !defined(CONFIG_SOC_MAX32660)
// status flags
#define ADI_MAX32_UART_RX_EMPTY MXC_F_UART_STATUS_RX_EMPTY
#define ADI_MAX32_UART_TX_EMPTY MXC_F_UART_STATUS_TX_EMPTY
#define ADI_MAX32_UART_STATUS_TX_FULL MXC_F_UART_STATUS_TX_FULL
// error flags
#define ADI_MAX32_UART_ERROR_OVERRUN MXC_F_UART_INT_FL_RX_OVERRUN
#define ADI_MAX32_UART_ERROR_PARITY MXC_F_UART_INT_FL_RX_PARITY_ERROR
#define ADI_MAX32_UART_ERROR_FRAMING MXC_F_UART_INT_FL_RX_FRAME_ERROR
#else
// status flags
#define ADI_MAX32_UART_RX_EMPTY MXC_F_UART_STAT_RX_EMPTY
#define ADI_MAX32_UART_TX_EMPTY MXC_F_UART_STAT_TX_EMPTY
#define ADI_MAX32_UART_STATUS_TX_FULL MXC_F_UART_STAT_TX_FULL
// error flags
#define ADI_MAX32_UART_ERROR_OVERRUN MXC_F_UART_INT_FL_RX_OVR
#define ADI_MAX32_UART_ERROR_PARITY MXC_F_UART_INT_FL_PARITY
#define ADI_MAX32_UART_ERROR_FRAMING MXC_F_UART_INT_FL_FRAME
#endif
// interrupt flag
#define ADI_MAX32_UART_INT_EOT MXC_F_UART_INT_EN_LAST_BREAK // End Of Transmission Interrupt
#define ADI_MAX32_UART_INT_OE MXC_F_UART_INT_EN_RX_OVERRUN // Overrun Error Interrupt
#define ADI_MAX32_UART_INT_BE MXC_F_UART_INT_EN_BREAK // Break Error Interrupt
#define ADI_MAX32_UART_INT_PE MXC_F_UART_INT_EN_RX_PARITY_ERROR // Parity Error Interrupt
#define ADI_MAX32_UART_INT_FE MXC_F_UART_INT_EN_RX_FRAME_ERROR // Framing Error Interrupt
#if !defined(CONFIG_SOC_MAX32650) && !defined(CONFIG_SOC_MAX32660)
#define ADI_MAX32_UART_INT_RT MXC_F_UART_INT_EN_RX_TIMEOUT // Receive Timeout Interrupt
#define ADI_MAX32_UART_INT_TX MXC_F_UART_INT_EN_TX_FIFO_THRESH // Transmit Interrupt
#define ADI_MAX32_UART_INT_RX MXC_F_UART_INT_EN_RX_FIFO_THRESH // Receive Interrupt
#define ADI_MAX32_UART_INT_CTS MXC_F_UART_INT_EN_CTS_CHANGE // CTS Modem Interrupt
#define ADI_MAX32_UART_INT_TX_OEM \
    MXC_F_UART_INT_EN_TX_FIFO_ALMOST_EMPTY // TX FIFO Almost Empty Interrupt
#else
#define ADI_MAX32_UART_INT_RT MXC_F_UART_INT_EN_RX_TO // Receive Timeout Interrupt
#define ADI_MAX32_UART_INT_TX MXC_F_UART_INT_EN_TX_FIFO_LVL // Transmit Interrupt
#define ADI_MAX32_UART_INT_RX MXC_F_UART_INT_EN_RX_FIFO_LVL // Receive Interrupt
#define ADI_MAX32_UART_INT_CTS MXC_F_UART_INT_EN_CTS // CTS Modem Interrupt
#define ADI_MAX32_UART_INT_TX_OEM MXC_F_UART_INT_EN_TX_FIFO_AE // TX FIFO Almost Empty Interrupt
#endif
// parity
#define ADI_MAX32_UART_CFG_PARITY_NONE MXC_UART_PARITY_DISABLE
#define ADI_MAX32_UART_CFG_PARITY_ODD MXC_UART_PARITY_ODD
#define ADI_MAX32_UART_CFG_PARITY_EVEN MXC_UART_PARITY_EVEN
#define ADI_MAX32_UART_CFG_PARITY_MARK MXC_UART_PARITY_MARK
#define ADI_MAX32_UART_CFG_PARITY_SPACE MXC_UART_PARITY_SPACE

/* Error interrupts */
#define ADI_MAX32_UART_ERROR_INTERRUPTS \
    (ADI_MAX32_UART_INT_OE | ADI_MAX32_UART_INT_PE | ADI_MAX32_UART_INT_FE)

static inline int Wrap_MXC_UART_Init(mxc_uart_regs_t *uart)
{
    int ret;

    ret = MXC_UART_SetRXThreshold(uart, 1);
    if (ret) {
        return ret;
    }

    // default values as per of MSDK
    ret = MXC_UART_SetTXThreshold(uart, 2);
    if (ret) {
        return ret;
    }

#if defined(CONFIG_SOC_MAX32650) || defined(CONFIG_SOC_MAX32660)
    uart->ctrl0 |= MXC_F_UART_CTRL0_ENABLE;
#else
    uart->ctrl |= MXC_F_UART_CTRL_ENABLE;
#endif

    return ret;
}

static inline int Wrap_MXC_UART_SetFrequency(mxc_uart_regs_t *uart, unsigned int baud,
                                             int clock_source)
{
    (void)clock_source;
    return MXC_UART_SetFrequency(uart, baud);
}

static inline int Wrap_MXC_UART_SetClockSource(mxc_uart_regs_t *uart, int clock_source)
{
    (void)uart;
    if (clock_source == ADI_MAX32_PRPH_CLK_SRC_PCLK) {
        return E_NO_ERROR;
    } else {
        return E_BAD_PARAM;
    }
}

static inline void Wrap_MXC_UART_SetTxDMALevel(mxc_uart_regs_t *uart, uint8_t bytes)
{
    uart->dma |= ((bytes & 0x1F) << MXC_F_UART_DMA_TXDMA_LEVEL_POS);
}

static inline void Wrap_MXC_UART_SetRxDMALevel(mxc_uart_regs_t *uart, uint8_t bytes)
{
    uart->dma |= ((bytes & 0x1F) << MXC_F_UART_DMA_RXDMA_LEVEL_POS);
}

static inline void Wrap_MXC_UART_EnableTxDMA(mxc_uart_regs_t *uart)
{
    uart->dma |= MXC_F_UART_DMA_TXDMA_EN;
}

static inline void Wrap_MXC_UART_EnableRxDMA(mxc_uart_regs_t *uart)
{
    uart->dma |= MXC_F_UART_DMA_RXDMA_EN;
}

static inline void Wrap_MXC_UART_DisableTxDMA(mxc_uart_regs_t *uart)
{
    uart->dma &= ~MXC_F_UART_DMA_TXDMA_EN;
}

static inline void Wrap_MXC_UART_DisableRxDMA(mxc_uart_regs_t *uart)
{
    uart->dma &= ~MXC_F_UART_DMA_RXDMA_EN;
}

/*
 *  MAX32690, MAX32655 related mapping
 */
#elif defined(CONFIG_SOC_MAX32690) || defined(CONFIG_SOC_MAX32655) || \
    defined(CONFIG_SOC_MAX32670) || defined(CONFIG_SOC_MAX32672) ||   \
    defined(CONFIG_SOC_MAX32662) || defined(CONFIG_SOC_MAX32675) ||   \
    defined(CONFIG_SOC_MAX32680) || defined(CONFIG_SOC_MAX32657) ||   \
    defined(CONFIG_SOC_MAX78002) || defined(CONFIG_SOC_MAX78000)

// status flags
#define ADI_MAX32_UART_RX_EMPTY MXC_F_UART_STATUS_RX_EM
#define ADI_MAX32_UART_TX_EMPTY MXC_F_UART_STATUS_TX_EM
#define ADI_MAX32_UART_STATUS_TX_FULL MXC_F_UART_STATUS_TX_FULL

#if defined(CONFIG_SOC_MAX32662) || defined(CONFIG_SOC_MAX32657)
// error flags
#define ADI_MAX32_UART_ERROR_OVERRUN MXC_F_UART_INTFL_RX_OV
#define ADI_MAX32_UART_ERROR_PARITY MXC_F_UART_INTFL_RX_PAR
#define ADI_MAX32_UART_ERROR_FRAMING MXC_F_UART_INTFL_RX_FERR
// interrupt flag
#define ADI_MAX32_UART_INT_OE MXC_F_UART_INTEN_RX_OV // Overrun Error Interrupt
#define ADI_MAX32_UART_INT_PE MXC_F_UART_INTEN_RX_PAR // Parity Error Interrupt
#define ADI_MAX32_UART_INT_FE MXC_F_UART_INTEN_RX_FERR // Framing Error Interrupt
#if defined(CONFIG_SOC_MAX32657)
#define ADI_MAX32_UART_INT_TX MXC_F_UART_INTEN_TX_THD // Transmit Interrupt
#else
#define ADI_MAX32_UART_INT_TX MXC_F_UART_INTEN_TX_HE // Transmit Interrupt
#endif
#define ADI_MAX32_UART_INT_RX MXC_F_UART_INTEN_RX_THD // Receive Interrupt
#define ADI_MAX32_UART_INT_CTS MXC_F_UART_INTEN_CTS_EV // CTS Modem Interrupt
#define ADI_MAX32_UART_INT_TX_OEM MXC_F_UART_INTEN_TX_OB // TX FIFO Almost Empty Interrupt
#else
// error flags
#define ADI_MAX32_UART_ERROR_OVERRUN MXC_F_UART_INT_FL_RX_OV
#define ADI_MAX32_UART_ERROR_PARITY MXC_F_UART_INT_FL_RX_PAR
#define ADI_MAX32_UART_ERROR_FRAMING MXC_F_UART_INT_FL_RX_FERR
// interrupt flag
#define ADI_MAX32_UART_INT_OE MXC_F_UART_INT_EN_RX_OV // Overrun Error Interrupt
#define ADI_MAX32_UART_INT_PE MXC_F_UART_INT_EN_RX_PAR // Parity Error Interrupt
#define ADI_MAX32_UART_INT_FE MXC_F_UART_INT_EN_RX_FERR // Framing Error Interrupt
#define ADI_MAX32_UART_INT_TX MXC_F_UART_INT_EN_TX_HE // Transmit Interrupt
#define ADI_MAX32_UART_INT_RX MXC_F_UART_INT_EN_RX_THD // Receive Interrupt
#define ADI_MAX32_UART_INT_CTS MXC_F_UART_INT_EN_CTS_EV // CTS Modem Interrupt
#define ADI_MAX32_UART_INT_TX_OEM MXC_F_UART_INT_EN_TX_OB // TX FIFO Almost Empty Interrupt
#endif
//#define ADI_MAX32_UART_INT_RT   // Receive Timeout Interrupt
//#define ADI_MAX32_UART_INT_BE   // Break Error Interrupt
//#define ADI_MAX32_UART_INT_EOT  // End Of Transmission Interrupt

// parity
#define ADI_MAX32_UART_CFG_PARITY_NONE MXC_UART_PARITY_DISABLE
#define ADI_MAX32_UART_CFG_PARITY_ODD MXC_UART_PARITY_ODD_0
#define ADI_MAX32_UART_CFG_PARITY_EVEN MXC_UART_PARITY_EVEN_0
//#define ADI_MAX32_UART_CFG_PARITY_MARK
//#define ADI_MAX32_UART_CFG_PARITY_SPACE

/* Error interrupts */
#define ADI_MAX32_UART_ERROR_INTERRUPTS \
    (ADI_MAX32_UART_INT_OE | ADI_MAX32_UART_INT_PE | ADI_MAX32_UART_INT_FE)

static inline int Wrap_MXC_UART_Init(mxc_uart_regs_t *uart)
{
    int ret;

    ret = MXC_UART_SetRXThreshold(uart, 1);

    return ret;
}

static inline mxc_uart_clock_t wrap_get_clock_source_instance(int clock_source)
{
    mxc_uart_clock_t clk_src;

    switch (clock_source) {
    case 0: // ADI_MAX32_PRPH_CLK_SRC_PCLK
        clk_src = MXC_UART_APB_CLK;
        break;
#if defined(CONFIG_SOC_MAX32662) || defined(CONFIG_SOC_MAX32670) || \
    defined(CONFIG_SOC_MAX32672) || defined(CONFIG_SOC_MAX32675)
    case 1: // ADI_MAX32_PRPH_CLK_SRC_EXTCLK
        clk_src = MXC_UART_EXT_CLK;
        break;
#endif
    case 2: // ADI_MAX32_PRPH_CLK_SRC_IBRO
        clk_src = MXC_UART_IBRO_CLK;
        break;
#if defined(CONFIG_SOC_MAX32662) || defined(CONFIG_SOC_MAX32670) || \
    defined(CONFIG_SOC_MAX32675) || defined(CONFIG_SOC_MAX32690)
    case 3: // ADI_MAX32_PRPH_CLK_SRC_ERFO
        clk_src = MXC_UART_ERFO_CLK;
        break;
#endif
#if defined(CONFIG_SOC_MAX32655) || defined(CONFIG_SOC_MAX32670) || \
    defined(CONFIG_SOC_MAX32672) || defined(CONFIG_SOC_MAX32690) || \
    defined(CONFIG_SOC_MAX78002) || defined(CONFIG_SOC_MAX78000)
    case 4: // ADI_MAX32_PRPH_CLK_SRC_ERTCO
        clk_src = MXC_UART_ERTCO_CLK;
        break;
#endif
#if defined(CONFIG_SOC_MAX32670) || defined(CONFIG_SOC_MAX32672)
    case 5: // ADI_MAX32_PRPH_CLK_SRC_INRO
        clk_src = MXC_UART_INRO_CLK;
        break;
#endif
    default:
        return -1;
    }

    return clk_src;
}
static inline int Wrap_MXC_UART_SetFrequency(mxc_uart_regs_t *uart, unsigned int baud,
                                             int clock_source)
{
    mxc_uart_clock_t clk_src;

    clk_src = wrap_get_clock_source_instance(clock_source);

    return MXC_UART_SetFrequency(uart, baud, clk_src);
}

static inline int Wrap_MXC_UART_SetClockSource(mxc_uart_regs_t *uart, int clock_source)
{
    mxc_uart_clock_t clk_src;

    clk_src = wrap_get_clock_source_instance(clock_source);

    return MXC_UART_SetClockSource(uart, clk_src);
}

static inline void Wrap_MXC_UART_SetTxDMALevel(mxc_uart_regs_t *uart, uint8_t bytes)
{
    uart->dma |= ((bytes & 0xF) << MXC_F_UART_DMA_TX_THD_VAL_POS);
}

static inline void Wrap_MXC_UART_SetRxDMALevel(mxc_uart_regs_t *uart, uint8_t bytes)
{
    uart->dma |= ((bytes & 0xF) << MXC_F_UART_DMA_RX_THD_VAL_POS);
}

static inline void Wrap_MXC_UART_EnableTxDMA(mxc_uart_regs_t *uart)
{
    uart->dma |= MXC_F_UART_DMA_TX_EN;
}

static inline void Wrap_MXC_UART_EnableRxDMA(mxc_uart_regs_t *uart)
{
    uart->dma |= MXC_F_UART_DMA_RX_EN;
}

static inline void Wrap_MXC_UART_DisableTxDMA(mxc_uart_regs_t *uart)
{
    uart->dma &= ~MXC_F_UART_DMA_TX_EN;
}

static inline void Wrap_MXC_UART_DisableRxDMA(mxc_uart_regs_t *uart)
{
    uart->dma &= ~MXC_F_UART_DMA_RX_EN;
}

#endif // defined(CONFIG_SOC_MAX32690) || defined(CONFIG_SOC_MAX32655)

static inline unsigned int Wrap_MXC_UART_GetRegINTEN(mxc_uart_regs_t *uart)
{
#if defined(CONFIG_SOC_MAX32662) || defined(CONFIG_SOC_MAX32657)
    return uart->inten;
#else
    return uart->int_en;
#endif
}

#ifdef __cplusplus
}
#endif

#endif // LIBRARIES_ZEPHYR_MAX_INCLUDE_WRAP_MAX32_UART_H_
