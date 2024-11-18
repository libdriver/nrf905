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
 * @file      driver_nrf905.h
 * @brief     driver nrf905 header file
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

#ifndef DRIVER_NRF905_H
#define DRIVER_NRF905_H

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#ifdef __cplusplus
extern "C"{
#endif

/**
 * @defgroup nrf905_driver nrf905 driver function
 * @brief    nrf905 driver modules
 * @{
 */

/**
 * @addtogroup nrf905_basic_driver
 * @{
 */

/**
 * @brief nrf905 bool enumeration definition
 */
typedef enum
{
    NRF905_BOOL_FALSE = 0x00,        /**< disable function */
    NRF905_BOOL_TRUE  = 0x01,        /**< enable function */
} nrf905_bool_t;

/**
 * @brief nrf905 pll mode enumeration definition
 */
typedef enum
{
    NRF905_PLL_MODE_433_MHZ     = 0x00,        /**< chip operating in 433MHz band */
    NRF905_PLL_MODE_868_915_MHZ = 0x01,        /**< chip operating in 868 or 915 MHz band */
} nrf905_pll_mode_t;

/**
 * @brief nrf905 output power enumeration definition
 */
typedef enum
{
    NRF905_OUTPUT_POWER_NEGATIVE_10_DBM = 0x00,        /**< -10dBm */
    NRF905_OUTPUT_POWER_NEGATIVE_2_DBM  = 0x01,        /**< -2dBm */
    NRF905_OUTPUT_POWER_6_DBM          = 0x02,         /**< +6dBm */
    NRF905_OUTPUT_POWER_10_DBM         = 0x03,         /**< +10dBm */
} nrf905_output_power_t;

/**
 * @brief nrf905 rx mode enumeration definition
 */
typedef enum
{
    NRF905_RX_MODE_NORMAL        = 0x00,        /**< normal operation */
    NRF905_RX_MODE_REDUCED_POWER = 0x01,        /**< reduced power */
} nrf905_rx_mode_t;

/**
 * @brief nrf905 address width enumeration definition
 */
typedef enum
{
    NRF905_ADDRESS_WIDTH_1_BYTE = 0x01,        /**< 1 byte tx address field width */
    NRF905_ADDRESS_WIDTH_4_BYTE = 0x04,        /**< 4 byte tx address field width */
} nrf905_address_width_t;

/**
 * @brief nrf905 output clock frequency enumeration definition
 */
typedef enum
{
    NRF905_OUTPUT_CLOCK_FREQUENCY_4MHZ   = 0x00,        /**< 4MHz */
    NRF905_OUTPUT_CLOCK_FREQUENCY_2MHZ   = 0x01,        /**< 2MHz */
    NRF905_OUTPUT_CLOCK_FREQUENCY_1MHZ   = 0x02,        /**< 1MHz */
    NRF905_OUTPUT_CLOCK_FREQUENCY_500KHZ = 0x03,        /**< 500kHZ */
} nrf905_output_clock_frequency_t;

/**
 * @brief nrf905 crystal oscillator frequency enumeration definition
 */
typedef enum
{
    NRF905_CRYSTAL_OSCILLATOR_FREQUENCY_4MHZ  = 0x00,        /**< 4MHz */
    NRF905_CRYSTAL_OSCILLATOR_FREQUENCY_8MHZ  = 0x01,        /**< 8MHz */
    NRF905_CRYSTAL_OSCILLATOR_FREQUENCY_12MHZ = 0x02,        /**< 12MHz */
    NRF905_CRYSTAL_OSCILLATOR_FREQUENCY_16MHZ = 0x03,        /**< 16MHz */
    NRF905_CRYSTAL_OSCILLATOR_FREQUENCY_20MHZ = 0x04,        /**< 20MHz */
} nrf905_crystal_oscillator_frequency_t;

/**
 * @brief nrf905 crc mode enumeration definition
 */
typedef enum
{
    NRF905_CRC_MODE_8  = 0x00,        /**< 8 crc check bit */
    NRF905_CRC_MODE_16 = 0x01,        /**< 16 crc check bit */
} nrf905_crc_mode_t;

/**
 * @brief nrf905 mode enumeration definition
 */
typedef enum
{
    NRF905_MODE_RX = 0x00,        /**< rx mode */
    NRF905_MODE_TX = 0x01,        /**< tx mode */
} nrf905_mode_t;

/**
 * @brief nrf905 status enumeration definition
 */
typedef enum
{
    NRF905_STATUS_AM      = 0x00,        /**< address match */
    NRF905_STATUS_TX_DONE = 0x01,        /**< tx done */
    NRF905_STATUS_RX_DONE = 0x02,        /**< rx done */
} nrf905_status_t;

/**
 * @brief nrf905 handle structure definition
 */
typedef struct nrf905_handle_s
{
    uint8_t (*ce_gpio_init)(void);                                            /**< point to a ce_gpio_init function address */
    uint8_t (*ce_gpio_deinit)(void);                                          /**< point to a ce_gpio_deinit function address */
    uint8_t (*ce_gpio_write)(uint8_t value);                                  /**< point to a ce_gpio_write function address */
    uint8_t (*tx_en_gpio_init)(void);                                         /**< point to a tx_en_gpio_init function address */
    uint8_t (*tx_en_gpio_deinit)(void);                                       /**< point to a tx_en_gpio_deinit function address */
    uint8_t (*tx_en_gpio_write)(uint8_t value);                               /**< point to a tx_en_gpio_write function address */
    uint8_t (*pwr_up_gpio_init)(void);                                        /**< point to a pwr_up_gpio_init function address */
    uint8_t (*pwr_up_gpio_deinit)(void);                                      /**< point to a pwr_up_gpio_deinit function address */
    uint8_t (*pwr_up_gpio_write)(uint8_t value);                              /**< point to a pwr_up_gpio_write function address */
    uint8_t (*spi_init)(void);                                                /**< point to a spi_init function address */
    uint8_t (*spi_deinit)(void);                                              /**< point to a spi_deinit function address */
    uint8_t (*spi_read)(uint8_t reg, uint8_t *buf, uint16_t len);             /**< point to a spi_read function address */
    uint8_t (*spi_write)(uint8_t reg, uint8_t *buf, uint16_t len);            /**< point to a spi_write function address */
    uint8_t (*spi_transmit)(uint8_t *tx, uint8_t *rx, uint16_t len);          /**< point to a spi_transmit function address */
    void (*delay_ms)(uint32_t ms);                                            /**< point to a delay_ms function address */
    void (*debug_print)(const char *const fmt, ...);                          /**< point to a debug_print function address */
    void (*receive_callback)(uint8_t type, uint8_t *buf, uint8_t len);        /**< point to a receive_callback function address */
    uint8_t inited;                                                           /**< inited flag */
    uint8_t conf[10];                                                         /**< conf buffer */
    uint8_t finished;                                                         /**< finished flag */
    uint8_t mode;                                                             /**< chip mode */
} nrf905_handle_t;

/**
 * @brief nrf905 information structure definition
 */
typedef struct nrf905_info_s
{
    char chip_name[32];                /**< chip name */
    char manufacturer_name[32];        /**< manufacturer name */
    char interface[8];                 /**< chip interface name */
    float supply_voltage_min_v;        /**< chip min supply voltage */
    float supply_voltage_max_v;        /**< chip max supply voltage */
    float max_current_ma;              /**< chip max current */
    float temperature_min;             /**< chip min operating temperature */
    float temperature_max;             /**< chip max operating temperature */
    uint32_t driver_version;           /**< driver version */
} nrf905_info_t;

/**
 * @}
 */

/**
 * @defgroup nrf905_link_driver nrf905 link driver function
 * @brief    nrf905 link driver modules
 * @ingroup  nrf905_driver
 * @{
 */

/**
 * @brief     initialize nrf905_handle_t structure
 * @param[in] HANDLE points to an nrf905 handle structure
 * @param[in] STRUCTURE is nrf905_handle_t
 * @note      none
 */
#define DRIVER_NRF905_LINK_INIT(HANDLE, STRUCTURE)               memset(HANDLE, 0, sizeof(STRUCTURE))

/**
 * @brief     link spi_init function
 * @param[in] HANDLE points to an nrf905 handle structure
 * @param[in] FUC points to a spi_init function address
 * @note      none
 */
#define DRIVER_NRF905_LINK_SPI_INIT(HANDLE, FUC)                 (HANDLE)->spi_init = FUC

/**
 * @brief     link spi_deinit function
 * @param[in] HANDLE points to an nrf905 handle structure
 * @param[in] FUC points to a spi_deinit function address
 * @note      none
 */
#define DRIVER_NRF905_LINK_SPI_DEINIT(HANDLE, FUC)               (HANDLE)->spi_deinit = FUC

/**
 * @brief     link spi_read function
 * @param[in] HANDLE points to an nrf905 handle structure
 * @param[in] FUC points to a spi_read function address
 * @note      none
 */
#define DRIVER_NRF905_LINK_SPI_READ(HANDLE, FUC)                 (HANDLE)->spi_read = FUC

/**
 * @brief     link spi_write function
 * @param[in] HANDLE points to an nrf905 handle structure
 * @param[in] FUC points to a spi_write function address
 * @note      none
 */
#define DRIVER_NRF905_LINK_SPI_WRITE(HANDLE, FUC)                (HANDLE)->spi_write = FUC

/**
 * @brief     link spi_transmit function
 * @param[in] HANDLE points to an nrf905 handle structure
 * @param[in] FUC points to a spi_transmit function address
 * @note      none
 */
#define DRIVER_NRF905_LINK_SPI_TRANSMIT(HANDLE, FUC)             (HANDLE)->spi_transmit = FUC

/**
 * @brief     link ce_gpio_init function
 * @param[in] HANDLE points to an nrf905 handle structure
 * @param[in] FUC points to a ce_gpio_init function address
 * @note      none
 */
#define DRIVER_NRF905_LINK_CE_GPIO_INIT(HANDLE, FUC)             (HANDLE)->ce_gpio_init = FUC

/**
 * @brief     link ce_gpio_deinit function
 * @param[in] HANDLE points to an nrf905 handle structure
 * @param[in] FUC points to a ce_gpio_deinit function address
 * @note      none
 */
#define DRIVER_NRF905_LINK_CE_GPIO_DEINIT(HANDLE, FUC)           (HANDLE)->ce_gpio_deinit = FUC

/**
 * @brief     link ce_gpio_write function
 * @param[in] HANDLE points to an nrf905 handle structure
 * @param[in] FUC points to a ce_gpio_write function address
 * @note      none
 */
#define DRIVER_NRF905_LINK_CE_GPIO_WRITE(HANDLE, FUC)            (HANDLE)->ce_gpio_write = FUC

/**
 * @brief     link tx_en_gpio_init function
 * @param[in] HANDLE points to an nrf905 handle structure
 * @param[in] FUC points to a tx_en_gpio_init function address
 * @note      none
 */
#define DRIVER_NRF905_LINK_TX_EN_GPIO_INIT(HANDLE, FUC)          (HANDLE)->tx_en_gpio_init = FUC

/**
 * @brief     link tx_en_gpio_deinit function
 * @param[in] HANDLE points to an nrf905 handle structure
 * @param[in] FUC points to a tx_en_gpio_deinit function address
 * @note      none
 */
#define DRIVER_NRF905_LINK_TX_EN_GPIO_DEINIT(HANDLE, FUC)        (HANDLE)->tx_en_gpio_deinit = FUC

/**
 * @brief     link tx_en_gpio_write function
 * @param[in] HANDLE points to an nrf905 handle structure
 * @param[in] FUC points to a tx_en_gpio_write function address
 * @note      none
 */
#define DRIVER_NRF905_LINK_TX_EN_GPIO_WRITE(HANDLE, FUC)         (HANDLE)->tx_en_gpio_write = FUC

/**
 * @brief     link pwr_up_gpio_init function
 * @param[in] HANDLE points to an nrf905 handle structure
 * @param[in] FUC points to a pwr_up_gpio_init function address
 * @note      none
 */
#define DRIVER_NRF905_LINK_PWR_UP_GPIO_INIT(HANDLE, FUC)         (HANDLE)->pwr_up_gpio_init = FUC

/**
 * @brief     link pwr_up_gpio_deinit function
 * @param[in] HANDLE points to an nrf905 handle structure
 * @param[in] FUC points to a pwr_up_gpio_deinit function address
 * @note      none
 */
#define DRIVER_NRF905_LINK_PWR_UP_GPIO_DEINIT(HANDLE, FUC)       (HANDLE)->pwr_up_gpio_deinit = FUC

/**
 * @brief     link pwr_up_gpio_write function
 * @param[in] HANDLE points to an nrf905 handle structure
 * @param[in] FUC points to a pwr_up_gpio_write function address
 * @note      none
 */
#define DRIVER_NRF905_LINK_PWR_UP_GPIO_WRITE(HANDLE, FUC)        (HANDLE)->pwr_up_gpio_write = FUC

/**
 * @brief     link delay_ms function
 * @param[in] HANDLE points to an nrf905 handle structure
 * @param[in] FUC points to a delay_ms function address
 * @note      none
 */
#define DRIVER_NRF905_LINK_DELAY_MS(HANDLE, FUC)                 (HANDLE)->delay_ms = FUC

/**
 * @brief     link debug_print function
 * @param[in] HANDLE points to an nrf905 handle structure
 * @param[in] FUC points to a debug_print function address
 * @note      none
 */
#define DRIVER_NRF905_LINK_DEBUG_PEINT(HANDLE, FUC)              (HANDLE)->debug_print = FUC

/**
 * @brief     link receive_callback function
 * @param[in] HANDLE points to an nrf905 handle structure
 * @param[in] FUC points to a receive_callback function address
 * @note      none
 */
#define DRIVER_NRF905_LINK_RECEIVE_CALLBACK(HANDLE, FUC)         (HANDLE)->receive_callback = FUC

/**
 * @}
 */

/**
 * @defgroup nrf905_basic_driver nrf905 basic driver function
 * @brief    nrf905 basic driver modules
 * @ingroup  nrf905_driver
 * @{
 */

/**
 * @brief      get chip's information
 * @param[out] *info points to an nrf905 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t nrf905_info(nrf905_info_t *info);

/**
 * @brief     irq handler
 * @param[in] *handle points to an nrf905 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 run failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t nrf905_irq_handler(nrf905_handle_t *handle);

/**
 * @brief     initialize the chip
 * @param[in] *handle points to an nrf905 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 spi initialization failed
 *            - 2 handle is NULL
 *            - 3 linked functions is NULL
 *            - 4 pwr up gpio init failed
 *            - 5 ce gpio init failed
 *            - 6 tx en gpio init failed
 * @note      none
 */
uint8_t nrf905_init(nrf905_handle_t *handle);

/**
 * @brief     close the chip
 * @param[in] *handle points to an nrf905 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 power down failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 spi deinit failed
 *            - 5 pwr up gpio deinit failed
 *            - 6 ce gpio deinit failed
 *            - 7 tx en gpio deinit failed
 * @note      none
 */
uint8_t nrf905_deinit(nrf905_handle_t *handle);

/**
 * @brief     set the power up
 * @param[in] *handle points to an nrf905 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set power up failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t nrf905_set_power_up(nrf905_handle_t *handle, nrf905_bool_t enable);

/**
 * @brief     enable or disable the chip
 * @param[in] *handle points to an nrf905 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set enable failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t nrf905_set_enable(nrf905_handle_t *handle, nrf905_bool_t enable);

/**
 * @brief     set the mode
 * @param[in] *handle points to an nrf905 handle structure
 * @param[in] mode is the chip mode
 * @return    status code
 *            - 0 success
 *            - 1 set mode failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t nrf905_set_mode(nrf905_handle_t *handle, nrf905_mode_t mode);

/**
 * @brief     send data
 * @param[in] *handle points to an nrf905 handle structure
 * @param[in] *buf points to a data buffer
 * @param[in] len is the buffer length
 * @return    status code
 *            - 0 success
 *            - 1 send failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 len is over conf
 *            - 5 send timeout
 * @note      none
 */
uint8_t nrf905_send(nrf905_handle_t *handle, uint8_t *buf, uint8_t len);

/**
 * @brief     write the conf
 * @param[in] *handle points to an nrf905 handle structure
 * @return    status code
 *            - 0 success
 *            - 1 write conf failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t nrf905_write_conf(nrf905_handle_t *handle);

/**
 * @brief      read the conf
 * @param[in]  *handle points to an nrf905 handle structure
 * @param[out] *conf points to a conf buffer
 * @return     status code
 *             - 0 success
 *             - 1 read conf failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t nrf905_read_conf(nrf905_handle_t *handle, uint8_t conf[10]);

/**
 * @brief     update the conf
 * @param[in] *handle points to an nrf905 handle structure
 * @param[in] *conf points to a conf buffer
 * @return    status code
 *            - 0 success
 *            - 1 update conf failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t nrf905_update_conf(nrf905_handle_t *handle, uint8_t conf[10]);

/**
 * @brief     set the tx payload
 * @param[in] *handle points to an nrf905 handle structure
 * @param[in] *buf points to a data buffer
 * @param[in] len is the buffer length
 * @return    status code
 *            - 0 success
 *            - 1 set tx payload failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 len is over 32
 * @note      1<= len <=32
 */
uint8_t nrf905_set_tx_payload(nrf905_handle_t *handle, uint8_t *buf, uint8_t len);

/**
 * @brief      get the tx payload
 * @param[in]  *handle points to an nrf905 handle structure
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the buffer length
 * @return     status code
 *             - 0 success
 *             - 1 get tx payload failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 len is over 32
 * @note       1<= len <=32
 */
uint8_t nrf905_get_tx_payload(nrf905_handle_t *handle, uint8_t *buf, uint8_t len);

/**
 * @brief      get the rx payload
 * @param[in]  *handle points to an nrf905 handle structure
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the buffer length
 * @return     status code
 *             - 0 success
 *             - 1 get rx payload failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 len is over 32
 * @note       1<= len <=32
 */
uint8_t nrf905_get_rx_payload(nrf905_handle_t *handle, uint8_t *buf, uint8_t len);

/**
 * @brief     set the tx address
 * @param[in] *handle points to an nrf905 handle structure
 * @param[in] *addr points to an address buffer
 * @param[in] len is the address length
 * @return    status code
 *            - 0 success
 *            - 1 set tx address failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 len is over 4
 * @note      1<= len <=4
 */
uint8_t nrf905_set_tx_address(nrf905_handle_t *handle, uint8_t *addr, uint8_t len);

/**
 * @brief      get the tx address
 * @param[in]  *handle points to an nrf905 handle structure
 * @param[out] *addr points to an address buffer
 * @param[in]  len is the address length
 * @return     status code
 *             - 0 success
 *             - 1 get tx address failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 *             - 4 len is over 4
 * @note       1<= len <=4
 */
uint8_t nrf905_get_tx_address(nrf905_handle_t *handle, uint8_t *addr, uint8_t len);

/**
 * @brief     config the channel
 * @param[in] *handle points to an nrf905 handle structure
 * @param[in] freq is the chip frequency config
 * @param[in] mode is the pll mode
 * @param[in] power is the output power
 * @return    status code
 *            - 0 success
 *            - 1 channel config failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 freq is over 0x1FF
 * @note      0<= freq <= 0x1FF
 */
uint8_t nrf905_channel_config(nrf905_handle_t *handle, uint16_t freq, nrf905_pll_mode_t mode, nrf905_output_power_t power);

/**
 * @brief      get the status
 * @param[in]  *handle points to an nrf905 handle structure
 * @param[out] *status points to a status buffer
 * @return     status code
 *             - 0 success
 *             - 1 get status failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t nrf905_get_status(nrf905_handle_t *handle, uint8_t *status);

/**
 * @brief     set the frequency
 * @param[in] *handle points to an nrf905 handle structure
 * @param[in] freq is the chip frequency config
 * @return    status code
 *            - 0 success
 *            - 1 set frequency failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 freq is over 0x1FF
 * @note      0<= freq <= 0x1FF
 */
uint8_t nrf905_set_frequency(nrf905_handle_t *handle, uint16_t freq);

/**
 * @brief      get the frequency
 * @param[in]  *handle points to an nrf905 handle structure
 * @param[out] *freq points to a chip frequency config buffer
 * @return     status code
 *             - 0 success
 *             - 1 get frequency failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t nrf905_get_frequency(nrf905_handle_t *handle, uint16_t *freq);

/**
 * @brief      convert the frequency to the register raw data
 * @param[in]  *handle points to an nrf905 handle structure
 * @param[in]  mhz is the frequency
 * @param[out] *reg points to a register raw buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t nrf905_frequency_convert_to_register(nrf905_handle_t *handle, float mhz, uint16_t *reg);

/**
 * @brief      convert the register raw data to the frequency
 * @param[in]  *handle points to an nrf905 handle structure
 * @param[in]  reg is the register raw data
 * @param[out] *mhz points to a frequency buffer
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t nrf905_frequency_convert_to_data(nrf905_handle_t *handle, uint16_t reg, float *mhz);

/**
 * @brief     set the pll mode
 * @param[in] *handle points to an nrf905 handle structure
 * @param[in] mode is the pll mode
 * @return    status code
 *            - 0 success
 *            - 1 set pll mode failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t nrf905_set_pll_mode(nrf905_handle_t *handle, nrf905_pll_mode_t mode);

/**
 * @brief      get the pll mode
 * @param[in]  *handle points to an nrf905 handle structure
 * @param[out] *mode points to a pll mode buffer
 * @return     status code
 *             - 0 success
 *             - 1 get pll mode failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t nrf905_get_pll_mode(nrf905_handle_t *handle, nrf905_pll_mode_t *mode);

/**
 * @brief     set the output power
 * @param[in] *handle points to an nrf905 handle structure
 * @param[in] output_power is the output power
 * @return    status code
 *            - 0 success
 *            - 1 set output power failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t nrf905_set_output_power(nrf905_handle_t *handle, nrf905_output_power_t output_power);

/**
 * @brief      get the output power
 * @param[in]  *handle points to an nrf905 handle structure
 * @param[out] *output_power points to an output power buffer
 * @return     status code
 *             - 0 success
 *             - 1 get output power failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t nrf905_get_output_power(nrf905_handle_t *handle, nrf905_output_power_t *output_power);

/**
 * @brief     set the rx mode
 * @param[in] *handle points to an nrf905 handle structure
 * @param[in] mode is the rx mode
 * @return    status code
 *            - 0 success
 *            - 1 set rx mode failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t nrf905_set_rx_mode(nrf905_handle_t *handle, nrf905_rx_mode_t mode);

/**
 * @brief      get the rx mode
 * @param[in]  *handle points to an nrf905 handle structure
 * @param[out] *mode points to a rx mode buffer
 * @return     status code
 *             - 0 success
 *             - 1 get rx mode failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t nrf905_get_rx_mode(nrf905_handle_t *handle, nrf905_rx_mode_t *mode);

/**
 * @brief     enable or disable the auto retransmit
 * @param[in] *handle points to an nrf905 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set auto retransmit failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t nrf905_set_auto_retransmit(nrf905_handle_t *handle, nrf905_bool_t enable);

/**
 * @brief      get the auto retransmit status
 * @param[in]  *handle points to an nrf905 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get auto retransmit failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t nrf905_get_auto_retransmit(nrf905_handle_t *handle, nrf905_bool_t *enable);

/**
 * @brief     set the rx address width
 * @param[in] *handle points to an nrf905 handle structure
 * @param[in] width is the address width
 * @return    status code
 *            - 0 success
 *            - 1 set rx address width failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t nrf905_set_rx_address_width(nrf905_handle_t *handle, nrf905_address_width_t width);

/**
 * @brief      get the rx address width
 * @param[in]  *handle points to an nrf905 handle structure
 * @param[out] *width points to an address width buffer
 * @return     status code
 *             - 0 success
 *             - 1 get rx address width failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t nrf905_get_rx_address_width(nrf905_handle_t *handle, nrf905_address_width_t *width);

/**
 * @brief     set the tx address width
 * @param[in] *handle points to an nrf905 handle structure
 * @param[in] width is the address width
 * @return    status code
 *            - 0 success
 *            - 1 set tx address width failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t nrf905_set_tx_address_width(nrf905_handle_t *handle, nrf905_address_width_t width);

/**
 * @brief      get the tx address width
 * @param[in]  *handle points to an nrf905 handle structure
 * @param[out] *width points to an address width buffer
 * @return     status code
 *             - 0 success
 *             - 1 get tx address width failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t nrf905_get_tx_address_width(nrf905_handle_t *handle, nrf905_address_width_t *width);

/**
 * @brief     set the rx payload width
 * @param[in] *handle points to an nrf905 handle structure
 * @param[in] width is the rx payload width
 * @return    status code
 *            - 0 success
 *            - 1 set rx payload width failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 width is over 0x20
 * @note      1<= width <= 0x20
 */
uint8_t nrf905_set_rx_payload_width(nrf905_handle_t *handle, uint8_t width);

/**
 * @brief      get the rx payload width
 * @param[in]  *handle points to an nrf905 handle structure
 * @param[out] *width points to a payload width buffer
 * @return     status code
 *             - 0 success
 *             - 1 get rx payload width failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t nrf905_get_rx_payload_width(nrf905_handle_t *handle, uint8_t *width);

/**
 * @brief     set the tx payload width
 * @param[in] *handle points to an nrf905 handle structure
 * @param[in] width is the tx payload width
 * @return    status code
 *            - 0 success
 *            - 1 set tx payload width failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 *            - 4 width is over 0x20
 * @note      1<= width <= 0x20
 */
uint8_t nrf905_set_tx_payload_width(nrf905_handle_t *handle, uint8_t width);

/**
 * @brief      get the tx payload width
 * @param[in]  *handle points to an nrf905 handle structure
 * @param[out] *width points to a payload width buffer
 * @return     status code
 *             - 0 success
 *             - 1 get tx payload width failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t nrf905_get_tx_payload_width(nrf905_handle_t *handle, uint8_t *width);

/**
 * @brief     set the rx address
 * @param[in] *handle points to an nrf905 handle structure
 * @param[in] *addr points to a rx address
 * @return    status code
 *            - 0 success
 *            - 1 set rx address failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t nrf905_set_rx_address(nrf905_handle_t *handle, uint8_t addr[4]);

/**
 * @brief      get the rx address
 * @param[in]  *handle points to an nrf905 handle structure
 * @param[out] *addr points to a rx address
 * @return     status code
 *             - 0 success
 *             - 1 get rx address failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t nrf905_get_rx_address(nrf905_handle_t *handle, uint8_t addr[4]);

/**
 * @brief     set the output clock frequency
 * @param[in] *handle points to an nrf905 handle structure
 * @param[in] freq is the output clock frequency
 * @return    status code
 *            - 0 success
 *            - 1 set output clock frequency failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t nrf905_set_output_clock_frequency(nrf905_handle_t *handle, nrf905_output_clock_frequency_t freq);

/**
 * @brief      get the output clock frequency
 * @param[in]  *handle points to an nrf905 handle structure
 * @param[out] *freq points to an output clock frequency buffer
 * @return     status code
 *             - 0 success
 *             - 1 get output clock frequency failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t nrf905_get_output_clock_frequency(nrf905_handle_t *handle, nrf905_output_clock_frequency_t *freq);

/**
 * @brief     enable or disable the output clock
 * @param[in] *handle points to an nrf905 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set output clock failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t nrf905_set_output_clock(nrf905_handle_t *handle, nrf905_bool_t enable);

/**
 * @brief      get the output clock status
 * @param[in]  *handle points to an nrf905 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get output clock failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t nrf905_get_output_clock(nrf905_handle_t *handle, nrf905_bool_t *enable);

/**
 * @brief     set the crystal oscillator frequency
 * @param[in] *handle points to an nrf905 handle structure
 * @param[in] freq is the crystal oscillator frequency
 * @return    status code
 *            - 0 success
 *            - 1 set crystal oscillator frequency failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t nrf905_set_crystal_oscillator_frequency(nrf905_handle_t *handle, nrf905_crystal_oscillator_frequency_t freq);

/**
 * @brief      get the crystal oscillator frequency
 * @param[in]  *handle points to an nrf905 handle structure
 * @param[out] *freq points to a crystal oscillator frequency buffer
 * @return     status code
 *             - 0 success
 *             - 1 get crystal oscillator frequency failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t nrf905_get_crystal_oscillator_frequency(nrf905_handle_t *handle, nrf905_crystal_oscillator_frequency_t *freq);

/**
 * @brief     enable or disable the crc
 * @param[in] *handle points to an nrf905 handle structure
 * @param[in] enable is a bool value
 * @return    status code
 *            - 0 success
 *            - 1 set crc failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t nrf905_set_crc(nrf905_handle_t *handle, nrf905_bool_t enable);

/**
 * @brief      get the crc status
 * @param[in]  *handle points to an nrf905 handle structure
 * @param[out] *enable points to a bool value buffer
 * @return     status code
 *             - 0 success
 *             - 1 get crc failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t nrf905_get_crc(nrf905_handle_t *handle, nrf905_bool_t *enable);

/**
 * @brief     set the crc mode
 * @param[in] *handle points to an nrf905 handle structure
 * @param[in] mode is the crc mode
 * @return    status code
 *            - 0 success
 *            - 1 set crc mode failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t nrf905_set_crc_mode(nrf905_handle_t *handle, nrf905_crc_mode_t mode);

/**
 * @brief      get the crc mode
 * @param[in]  *handle points to an nrf905 handle structure
 * @param[out] *mode points to a crc mode buffer
 * @return     status code
 *             - 0 success
 *             - 1 get crc mode failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t nrf905_get_crc_mode(nrf905_handle_t *handle, nrf905_crc_mode_t *mode);

/**
 * @}
 */

/**
 * @defgroup nrf905_extern_driver nrf905 extern driver function
 * @brief    nrf905 extern driver modules
 * @ingroup  nrf905_driver
 * @{
 */

/**
 * @brief     set the chip register
 * @param[in] *handle points to an nrf905 handle structure
 * @param[in] reg is the register address
 * @param[in] *buf points to a data buffer
 * @param[in] len is the data buffer length
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 *            - 2 handle is NULL
 *            - 3 handle is not initialized
 * @note      none
 */
uint8_t nrf905_set_reg(nrf905_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len);

/**
 * @brief      get the chip register
 * @param[in]  *handle points to an nrf905 handle structure
 * @param[in]  reg is the register address
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the data buffer length
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 *             - 2 handle is NULL
 *             - 3 handle is not initialized
 * @note       none
 */
uint8_t nrf905_get_reg(nrf905_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len);

/**
 * @}
 */

/**
 * @}
 */

#ifdef __cplusplus
}
#endif

#endif
