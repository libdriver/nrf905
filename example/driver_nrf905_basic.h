/**
 * Copyright (c) 2015 - present LibDriver All rights reserved
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 *
 * @file      driver_nrf905_basic.h
 * @brief     driver nrf905 basic header file
 * @version   1.0.0
 * @author    Shifeng Li
 * @date      2022-03-31
 *
 * <h3>history</h3>
 * <table>
 * <tr><th>Date        <th>Version  <th>Author      <th>Description
 * <tr><td>2022/03/31  <td>1.0      <td>Shifeng Li  <td>first upload
 * </table>
 */

#ifndef DRIVER_NRF905_BASIC_H
#define DRIVER_NRF905_BASIC_H

#include "driver_nrf905_interface.h"

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup nrf905_example_driver nrf905 example driver function
 * @brief    nrf905 example driver modules
 * @ingroup  nrf905_driver
 * @{
 */

/**
 * @brief nrf905 basic example default definition
 */
#define NRF905_BASIC_DEFAULT_PLL_MODE                            NRF905_PLL_MODE_433_MHZ                          /**< 433 MHz */
#define NRF905_BASIC_DEFAULT_OUTPUT_POWER                        NRF905_OUTPUT_POWER_10_DBM                       /**< +10dBm */
#define NRF905_BASIC_DEFAULT_RX_MODE                             NRF905_RX_MODE_NORMAL                            /**< normal mode */
#define NRF905_BASIC_DEFAULT_AUTO_RETRANSMIT                     NRF905_BOOL_FALSE                                /**< disable the auto retransmit */
#define NRF905_BASIC_DEFAULT_RX_ADDRESS_WIDTH                    NRF905_ADDRESS_WIDTH_4_BYTE                      /**< 4 bytes width */
#define NRF905_BASIC_DEFAULT_TX_ADDRESS_WIDTH                    NRF905_ADDRESS_WIDTH_4_BYTE                      /**< 4 bytes width */
#define NRF905_BASIC_DEFAULT_RX_PAYLOAD_WIDTH                    32                                               /**< 32 bytes */
#define NRF905_BASIC_DEFAULT_TX_PAYLOAD_WIDTH                    32                                               /**< 32 bytes */
#define NRF905_BASIC_DEFAULT_RX_ADDR                             {0xE7, 0xE7, 0xE7, 0xE7}                         /**< rx address */
#define NRF905_BASIC_DEFAULT_OUTPUT_CLOCK_FREQUENCY              NRF905_OUTPUT_CLOCK_FREQUENCY_500KHZ             /**< 500 KHz */
#define NRF905_BASIC_DEFAULT_OUTPUT_CLOCK                        NRF905_BOOL_FALSE                                /**< disable the output clock */
#define NRF905_BASIC_DEFAULT_CRYSTAL_OSCILLATOR_FREQUENCY        NRF905_CRYSTAL_OSCILLATOR_FREQUENCY_16MHZ        /**< 16 MHz */
#define NRF905_BASIC_DEFAULT_CRC                                 NRF905_BOOL_TRUE                                 /**< enable the crc */
#define NRF905_BASIC_DEFAULT_CRC_MODE                            NRF905_CRC_MODE_8                                /**< crc 8 */
#define NRF905_BASIC_DEFAULT_FREQUENCY                           433.2f                                           /**< 433.2 MHz */

/**
 * @brief  nrf905 irq
 * @return status code
 *         - 0 success
 *         - 1 run failed
 * @note   none
 */
uint8_t nrf905_interrupt_irq_handler(void);

/**
 * @brief     basic example init
 * @param[in] mode is the chip working mode
 * @param[in] *callback points to a callback function
 * @return    status code
 *            - 0 success
 *            - 1 init failed
 * @note      none
 */
uint8_t nrf905_basic_init(nrf905_mode_t mode, void (*callback)(uint8_t type, uint8_t *buf, uint8_t len));

/**
 * @brief  basic example deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t nrf905_basic_deinit(void);

/**
 * @brief     basic example send
 * @param[in] *addr points to a address buffer
 * @param[in] *buf points to a data buffer
 * @param[in] len is the buffer length
 * @return    status code
 *            - 0 success
 *            - 1 send failed
 * @note      none
 */
uint8_t nrf905_basic_send(uint8_t *addr, uint8_t *buf, uint8_t len);

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
