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
 * @file      stm32f407_driver_nrf905_interface.c
 * @brief     stm32f407 driver nrf905 interface source file
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

#include "driver_nrf905_interface.h"
#include "delay.h"
#include "spi.h"
#include "uart.h"
#include "wire.h"
#include <stdarg.h>

/**
 * @brief  interface spi bus init
 * @return status code
 *         - 0 success
 *         - 1 spi init failed
 * @note   none
 */
uint8_t nrf905_interface_spi_init(void)
{
    return spi_init(SPI_MODE_0);
}

/**
 * @brief  interface spi bus deinit
 * @return status code
 *         - 0 success
 *         - 1 spi deinit failed
 * @note   none
 */
uint8_t nrf905_interface_spi_deinit(void)
{
    return spi_deinit();
}

/**
 * @brief      interface spi bus read
 * @param[in]  reg is the register address
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the length of data buffer
 * @return     status code
 *             - 0 success
 *             - 1 read failed
 * @note       none
 */
uint8_t nrf905_interface_spi_read(uint8_t reg, uint8_t *buf, uint16_t len)
{
    return spi_read(reg, buf, len);
}

/**
 * @brief     interface spi bus write
 * @param[in] reg is the register address
 * @param[in] *buf points to a data buffer
 * @param[in] len is the length of data buffer
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t nrf905_interface_spi_write(uint8_t reg, uint8_t *buf, uint16_t len)
{
    return spi_write(reg, buf, len);
}

/**
 * @brief      interface spi bus transmit
 * @param[in]  *tx points to a tx data buffer
 * @param[out] *rx points to a rx data buffer
 * @param[in]  len is the length of the data buffer
 * @return     status code
 *             - 0 success
 *             - 1 transmit failed
 * @note       none
 */
uint8_t nrf905_interface_spi_transmit(uint8_t *tx, uint8_t *rx, uint16_t len)
{
    return spi_transmit(tx, rx, len);
}

/**
 * @brief  interface ce gpio init
 * @return status code
 *         - 0 success
 *         - 1 init failed
 * @note   none
 */
uint8_t nrf905_interface_ce_gpio_init(void)
{
    return wire_ce_init();
}

/**
 * @brief  interface ce gpio deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t nrf905_interface_ce_gpio_deinit(void)
{
    return wire_ce_deinit();
}

/**
 * @brief     interface ce gpio write
 * @param[in] data is the written data
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t nrf905_interface_ce_gpio_write(uint8_t data)
{
    return wire_ce_write(data);
}

/**
 * @brief  interface tx en gpio init
 * @return status code
 *         - 0 success
 *         - 1 init failed
 * @note   none
 */
uint8_t nrf905_interface_tx_en_gpio_init(void)
{
    return wire_en_init();
}

/**
 * @brief  interface tx en gpio deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t nrf905_interface_tx_en_gpio_deinit(void)
{
    return wire_en_deinit();
}

/**
 * @brief     interface tx en gpio write
 * @param[in] data is the written data
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t nrf905_interface_tx_en_gpio_write(uint8_t data)
{
    return wire_en_write(data);
}

/**
 * @brief  interface pwr up gpio init
 * @return status code
 *         - 0 success
 *         - 1 init failed
 * @note   none
 */
uint8_t nrf905_interface_pwr_up_gpio_init(void)
{
    return wire_init();
}

/**
 * @brief  interface pwr up gpio deinit
 * @return status code
 *         - 0 success
 *         - 1 deinit failed
 * @note   none
 */
uint8_t nrf905_interface_pwr_up_gpio_deinit(void)
{
    return wire_deinit();
}

/**
 * @brief     interface pwr up gpio write
 * @param[in] data is the written data
 * @return    status code
 *            - 0 success
 *            - 1 write failed
 * @note      none
 */
uint8_t nrf905_interface_pwr_up_gpio_write(uint8_t data)
{
    return wire_write(data);
}

/**
 * @brief     interface delay ms
 * @param[in] ms
 * @note      none
 */
void nrf905_interface_delay_ms(uint32_t ms)
{
    delay_ms(ms);
}

/**
 * @brief     interface print format data
 * @param[in] fmt is the format data
 * @note      none
 */
void nrf905_interface_debug_print(const char *const fmt, ...)
{
    char str[256];
    uint16_t len;
    va_list args;
    
    memset((char *)str, 0, sizeof(char) * 256); 
    va_start(args, fmt);
    vsnprintf((char *)str, 255, (char const *)fmt, args);
    va_end(args);
    
    len = strlen((char *)str);
    (void)uart_write((uint8_t *)str, len);
}

/**
 * @brief     interface receive callback
 * @param[in] type is the receive callback type
 * @param[in] *buf points to a data buffer
 * @param[in] len is the buffer length
 * @note      none
 */
void nrf905_interface_receive_callback(uint8_t type, uint8_t *buf, uint8_t len)
{
    switch (type)
    {
        case NRF905_STATUS_AM :
        {
            nrf905_interface_debug_print("nrf905: address match.\n");
            
            break;
        }
        case NRF905_STATUS_TX_DONE :
        {
            nrf905_interface_debug_print("nrf905: tx done.\n");
            
            break;
        }
        case NRF905_STATUS_RX_DONE :
        {
            nrf905_interface_debug_print("nrf905: rx done.\n");
            
            break;
        }
        default :
        {
            nrf905_interface_debug_print("nrf905: unknown code.\n");
            
            break;
        }
    }
}
