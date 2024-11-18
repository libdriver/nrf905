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
 * @file      driver_nrf905.c
 * @brief     driver nrf905 source file
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

#include "driver_nrf905.h"

/**
 * @brief chip information definition
 */
#define CHIP_NAME                 "Nordic nRF905"        /**< chip name */
#define MANUFACTURER_NAME         "Nordic"               /**< manufacturer name */
#define SUPPLY_VOLTAGE_MIN        1.9f                   /**< chip min supply voltage */
#define SUPPLY_VOLTAGE_MAX        3.6f                   /**< chip max supply voltage */
#define MAX_CURRENT               30.0f                  /**< chip max current */
#define TEMPERATURE_MIN           -40.0f                 /**< chip min operating temperature */
#define TEMPERATURE_MAX           85.0f                  /**< chip max operating temperature */
#define DRIVER_VERSION            1000                   /**< driver version */

/**
 * @brief chip command definition
 */
#define NRF905_COMMAND_W_CONFIG              0x00        /**< write configuration register command */
#define NRF905_COMMAND_R_CONFIG              0x10        /**< read configuration register command */
#define NRF905_COMMAND_W_TX_PAYLOAD          0x20        /**< write tx payload command */
#define NRF905_COMMAND_R_TX_PAYLOAD          0x21        /**< read tx payload command */
#define NRF905_COMMAND_W_TX_ADDRESS          0x22        /**< write tx address command */
#define NRF905_COMMAND_R_TX_ADDRESS          0x23        /**< read tx address command */
#define NRF905_COMMAND_R_RX_PAYLOAD          0x24        /**< read rx address command */
#define NRF905_COMMAND_CHANNEL_CONFIG        0x80        /**< special command for fast setting command */

/**
 * @brief chip byte definition
 */
#define NRF905_REG_CH_NO             0x00        /**< ch_no byte */
#define NRF905_REG_CONF1             0x01        /**< conf1 byte */
#define NRF905_REG_CONF2             0x02        /**< conf2 byte */
#define NRF905_REG_RX_PW             0x03        /**< rx pw byte */
#define NRF905_REG_TX_PW             0x04        /**< tx pw byte */
#define NRF905_REG_RX_ADDRESS_0      0x05        /**< rx address 0 byte */
#define NRF905_REG_RX_ADDRESS_1      0x06        /**< rx address 1 byte */
#define NRF905_REG_RX_ADDRESS_2      0x07        /**< rx address 2 byte */
#define NRF905_REG_RX_ADDRESS_3      0x08        /**< rx address 3 byte */
#define NRF905_REG_CONF3             0x09        /**< conf3 byte */

/**
 * @brief      write and read bytes
 * @param[in]  *handle points to an nrf905 handle structure
 * @param[out] *tx points to a tx data buffer
 * @param[out] *rx points to a rx data buffer
 * @param[in]  len is the length of the data buffer
 * @return     status code
 *             - 0 success
 *             - 1 spi write and read failed
 * @note       none
 */
static uint8_t a_nrf905_spi_transmit(nrf905_handle_t *handle, uint8_t *tx, uint8_t *rx, uint16_t len)
{
    if (handle->spi_transmit(tx, rx, len) != 0)       /* spi transmit */
    {
        return 1;                                     /* return error */
    }
    else
    {
        return 0;                                     /* success return 0 */
    }
}

/**
 * @brief      read bytes
 * @param[in]  *handle points to an nrf905 handle structure
 * @param[in]  reg is the register address
 * @param[out] *buf points to a data buffer
 * @param[in]  len is the data length
 * @return     status code
 *             - 0 success
 *             - 1 spi read failed
 * @note       none
 */
static uint8_t a_nrf905_spi_read(nrf905_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle->spi_read(reg, buf, len) != 0)        /* spi read */
    {
        return 1;                                    /* return error */
    }
    else
    {
        return 0;                                    /* success return 0 */
    }
}

/**
 * @brief     write bytes
 * @param[in] *handle points to an nrf905 handle structure
 * @param[in] reg is the register address
 * @param[in] *buf points to a data buffer
 * @param[in] len is the data length
 * @return    status code
 *            - 0 success
 *            - 1 spi write failed
 * @note      none
 */
static uint8_t a_nrf905_spi_write(nrf905_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle->spi_write(reg, buf, len) != 0)        /* spi write */
    {
        return 1;                                     /* return error */
    }
    else
    {
        return 0;                                     /* success return 0 */
    }
}

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
uint8_t nrf905_init(nrf905_handle_t *handle)
{
    if (handle == NULL)                                                      /* check handle */
    {
        return 2;                                                            /* return error */
    }
    if (handle->debug_print == NULL)                                         /* check debug_print */
    {
        return 3;                                                            /* return error */
    }
    if (handle->spi_init == NULL)                                            /* check spi_init */
    {
        handle->debug_print("nrf905: spi_init is null.\n");                  /* spi_init is null */
       
        return 3;                                                            /* return error */
    }
    if (handle->spi_deinit == NULL)                                          /* check spi_deinit */
    {
        handle->debug_print("nrf905: spi_deinit is null.\n");                /* spi_deinit is null */
       
        return 3;                                                            /* return error */
    }
    if (handle->spi_read == NULL)                                            /* check spi_read */
    {
        handle->debug_print("nrf905: spi_read is null.\n");                  /* spi_read is null */
       
        return 3;                                                            /* return error */
    }
    if (handle->spi_write == NULL)                                           /* check spi_write */
    {
        handle->debug_print("nrf905: spi_write is null.\n");                 /* spi_write is null */
       
        return 3;                                                            /* return error */
    }
    if (handle->spi_transmit == NULL)                                        /* check spi_transmit */
    {
        handle->debug_print("nrf905: spi_transmit is null.\n");              /* spi_transmit is null */
       
        return 3;                                                            /* return error */
    }
    if (handle->ce_gpio_init == NULL)                                        /* check ce_gpio_init */
    {
        handle->debug_print("nrf905: ce_gpio_init is null.\n");              /* ce_gpio_init is null */
       
        return 3;                                                            /* return error */
    }
    if (handle->ce_gpio_deinit == NULL)                                      /* check ce_gpio_deinit */
    {
        handle->debug_print("nrf905: ce_gpio_deinit is null.\n");            /* ce_gpio_deinit is null */
       
        return 3;                                                            /* return error */
    }
    if (handle->ce_gpio_write == NULL)                                       /* check ce_gpio_write */
    {
        handle->debug_print("nrf905: ce_gpio_write is null.\n");             /* ce_gpio_write is null */
       
        return 3;                                                            /* return error */
    }
    if (handle->tx_en_gpio_init == NULL)                                     /* check tx_en_gpio_init */
    {
        handle->debug_print("nrf905: tx_en_gpio_init is null.\n");           /* tx_en_gpio_init is null */
       
        return 3;                                                            /* return error */
    }
    if (handle->tx_en_gpio_deinit == NULL)                                   /* check tx_en_gpio_deinit */
    {
        handle->debug_print("nrf905: tx_en_gpio_deinit is null.\n");         /* tx_en_gpio_deinit is null */
       
        return 3;                                                            /* return error */
    }
    if (handle->tx_en_gpio_write == NULL)                                    /* check tx_en_gpio_write */
    {
        handle->debug_print("nrf905: tx_en_gpio_write is null.\n");          /* tx_en_gpio_write is null */
       
        return 3;                                                            /* return error */
    }
    if (handle->pwr_up_gpio_init == NULL)                                    /* check pwr_up_gpio_init */
    {
        handle->debug_print("nrf905: pwr_up_gpio_init is null.\n");          /* pwr_up_gpio_init is null */
       
        return 3;                                                            /* return error */
    }
    if (handle->pwr_up_gpio_deinit == NULL)                                  /* check pwr_up_gpio_deinit */
    {
        handle->debug_print("nrf905: pwr_up_gpio_deinit is null.\n");        /* pwr_up_gpio_deinit is null */
       
        return 3;                                                            /* return error */
    }
    if (handle->pwr_up_gpio_write == NULL)                                   /* check pwr_up_gpio_write */
    {
        handle->debug_print("nrf905: pwr_up_gpio_write is null.\n");         /* pwr_up_gpio_write is null */
       
        return 3;                                                            /* return error */
    }
    if (handle->delay_ms == NULL)                                            /* check delay_ms */
    {
        handle->debug_print("nrf905: delay_ms is null.\n");                  /* delay_ms is null */
       
        return 3;                                                            /* return error */
    }
    if (handle->receive_callback == NULL)                                    /* check receive_callback */
    {
        handle->debug_print("nrf905: receive_callback is null.\n");          /* receive_callback is null */
       
        return 3;                                                            /* return error */
    }
    
    if (handle->spi_init() != 0)                                             /* spi init */
    {
        handle->debug_print("nrf905: spi init failed.\n");                   /* spi init failed */
       
        return 1;                                                            /* return error */
    }
    if (handle->pwr_up_gpio_init() != 0)                                     /* pwr up gpio init */
    {
        handle->debug_print("nrf905: pwr up gpio init failed.\n");           /* pwr up gpio init failed */
        (void)handle->spi_deinit();                                          /* spi deinit */
        
        return 4;                                                            /* return error */
    }
    if (handle->ce_gpio_init() != 0)                                         /* ce gpio init */
    {
        handle->debug_print("nrf905: ce gpio init failed.\n");               /* ce gpio init failed */
        (void)handle->spi_deinit();                                          /* spi deinit */
        (void)handle->pwr_up_gpio_deinit();                                  /* pwr up gpio deinit */
        
        return 5;                                                            /* return error */
    }
    if (handle->tx_en_gpio_init() != 0)                                      /* tx en gpio init */
    {
        handle->debug_print("nrf905: tx en gpio init failed.\n");            /* tx en gpio init failed */
        (void)handle->spi_deinit();                                          /* spi deinit */
        (void)handle->pwr_up_gpio_deinit();                                  /* pwr up gpio deinit */
        (void)handle->ce_gpio_deinit();                                      /* ce gpio deinit */
        
        return 6;                                                            /* return error */
    }
    handle->conf[0] = 0x6C;                                                  /* set default conf */
    handle->conf[1] = 0x00;                                                  /* set default conf */
    handle->conf[2] = 0x44;                                                  /* set default conf */
    handle->conf[3] = 0x20;                                                  /* set default conf */
    handle->conf[4] = 0x20;                                                  /* set default conf */
    handle->conf[5] = 0xE7;                                                  /* set default conf */
    handle->conf[6] = 0xE7;                                                  /* set default conf */
    handle->conf[7] = 0xE7;                                                  /* set default conf */
    handle->conf[8] = 0xE7;                                                  /* set default conf */
    handle->conf[9] = 0xE7;                                                  /* set default conf */
    
    handle->inited = 1;                                                      /* flag finish initialization */
    
    return 0;                                                                /* success return 0 */
}

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
uint8_t nrf905_deinit(nrf905_handle_t *handle)
{
    if (handle == NULL)                                                     /* check handle */
    {
        return 2;                                                           /* return error */
    }
    if (handle->inited != 1)                                                /* check handle initialization */
    {
        return 3;                                                           /* return error */
    }
    
    if (handle->pwr_up_gpio_write(0) != 0)                                  /* power down */
    {
        handle->debug_print("nrf905: power down failed.\n");                /* power down failed */
        
        return 1;                                                           /* return error */
    }
    if (handle->spi_deinit() != 0)                                          /* spi deinit */
    {
        handle->debug_print("nrf905: spi deinit failed.\n");                /* spi deinit failed */
       
        return 4;                                                           /* return error */
    }
    if (handle->pwr_up_gpio_deinit() != 0)                                  /* pwr up gpio deinit */
    {
        handle->debug_print("nrf905: pwr up gpio deinit failed.\n");        /* pwr up gpio deinit failed */
        
        return 5;                                                           /* return error */
    }
    if (handle->ce_gpio_deinit() != 0)                                      /* ce gpio deinit */
    {
        handle->debug_print("nrf905: ce gpio deinit failed.\n");            /* ce gpio deinit failed */
        
        return 6;                                                           /* return error */
    }
    if (handle->tx_en_gpio_deinit() != 0)                                   /* tx en gpio deinit */
    {
        handle->debug_print("nrf905: tx en gpio deinit failed.\n");         /* tx en gpio deinit failed */
        
        return 7;                                                           /* return error */
    }
    
    handle->inited = 0;                                                     /* flag closed */
    
    return 0;                                                               /* success return 0 */
}

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
uint8_t nrf905_irq_handler(nrf905_handle_t *handle)
{
    uint8_t res;
    uint8_t status;
    uint8_t reg;
    
    if (handle == NULL)                                                                            /* check handle */
    {
        return 2;                                                                                  /* return error */
    }
    if (handle->inited != 1)                                                                       /* check handle initialization */
    {
        return 3;                                                                                  /* return error */
    }
    
    if (handle->ce_gpio_write(0) != 0)                                                             /* disable the chip */
    {
        handle->debug_print("nrf905: ce gpio write failed.\n");                                    /* ce gpio write failed */
        
        return 1;                                                                                  /* return error */
    }
    status = 0;                                                                                    /* set 0 */
    reg = 0xFF;                                                                                    /* set 0xFF */
    res = a_nrf905_spi_transmit(handle, &reg, (uint8_t *)&status, 1);                              /* read status */
    if (res != 0)                                                                                  /* check result */
    {
        handle->debug_print("nrf905: get status failed.\n");                                       /* get status failed */
        (void)handle->ce_gpio_write(1);                                                            /* enable the chip */
       
        return 1;                                                                                  /* return error */
    }
    if ((status & (1 << 5)) != 0)                                                                  /* if dr */
    {
        if (handle->mode == 1)                                                                     /* if tx mode */
        {
            handle->finished = 1;                                                                  /* flag finished */
            if (handle->receive_callback != NULL)                                                  /* if receive callback */
            {
                handle->receive_callback(NRF905_STATUS_TX_DONE, NULL, 0);                          /* run receive callback */
            }
        }
        else                                                                                       /* if rx mode */
        {
            uint8_t buf[32];
            uint8_t l;
            
            l = handle->conf[NRF905_REG_RX_PW] > 32 ? 32 : handle->conf[NRF905_REG_RX_PW];         /* get the read length */
            res = a_nrf905_spi_read(handle, NRF905_COMMAND_R_RX_PAYLOAD, (uint8_t *)buf, l);       /* read rx payload */
            if (res != 0)                                                                          /* check result */
            {
                handle->debug_print("nrf905: get rx payload failed.\n");                           /* get rx payload failed */
                (void)handle->ce_gpio_write(1);                                                    /* enable the chip */
                
                return 1;                                                                          /* return error */
            }
            if (handle->receive_callback != NULL)                                                  /* if receive callback */
            {
                handle->receive_callback(NRF905_STATUS_RX_DONE, (uint8_t *)buf, l);                /* run receive callback */
            }
        }
    }
    if ((status & (1 << 7)) != 0)                                                                  /* if am */
    {
        if (handle->receive_callback != NULL)                                                      /* if receive callback */
        {
            handle->receive_callback(NRF905_STATUS_AM, NULL, 0);                                   /* run receive callback */
        }
    }
    if (handle->ce_gpio_write(1) != 0)                                                             /* enable the chip */
    {
        handle->debug_print("nrf905: ce gpio write failed.\n");                                    /* ce gpio write failed */
        
        return 1;                                                                                  /* return error */
    }
    
    return 0;                                                                                      /* success return 0 */
}

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
uint8_t nrf905_set_power_up(nrf905_handle_t *handle, nrf905_bool_t enable)
{
    if (handle == NULL)                                               /* check handle */
    {
        return 2;                                                     /* return error */
    }
    if (handle->inited != 1)                                          /* check handle initialization */
    {
        return 3;                                                     /* return error */
    }
    
    if (handle->pwr_up_gpio_write(enable) != 0)                       /* set power up */
    {
        handle->debug_print("nrf905: set power up failed.\n");        /* set power up failed */
       
        return 1;                                                     /* return error */
    }
    else
    {
        return 0;                                                     /* success return 0 */
    }
}

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
uint8_t nrf905_set_enable(nrf905_handle_t *handle, nrf905_bool_t enable)
{
    if (handle == NULL)                                             /* check handle */
    {
        return 2;                                                   /* return error */
    }
    if (handle->inited != 1)                                        /* check handle initialization */
    {
        return 3;                                                   /* return error */
    }
    
    if (handle->ce_gpio_write(enable) != 0)                         /* set enable */
    {
        handle->debug_print("nrf905: set enable failed.\n");        /* set enable failed */
       
        return 1;                                                   /* return error */
    }
    else
    {
        return 0;                                                   /* success return 0 */
    }
}

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
uint8_t nrf905_set_mode(nrf905_handle_t *handle, nrf905_mode_t mode)
{
    if (handle == NULL)                                           /* check handle */
    {
        return 2;                                                 /* return error */
    }
    if (handle->inited != 1)                                      /* check handle initialization */
    {
        return 3;                                                 /* return error */
    }
    
    if (handle->tx_en_gpio_write(mode) != 0)                      /* set mode */
    {
        handle->debug_print("nrf905: set mode failed.\n");        /* set enable failed */
       
        return 1;                                                 /* return error */
    }
    else
    {
        handle->mode = (uint8_t)mode;                             /* set the chip mode */
        
        return 0;                                                 /* success return 0 */
    }
}

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
uint8_t nrf905_send(nrf905_handle_t *handle, uint8_t *buf, uint8_t len)
{
    uint8_t res;
    uint32_t timeout;
    
    if (handle == NULL)                                                                          /* check handle */
    {
        return 2;                                                                                /* return error */
    }
    if (handle->inited != 1)                                                                     /* check handle initialization */
    {
        return 3;                                                                                /* return error */
    }
    if (len > handle->conf[NRF905_REG_TX_PW])                                                    /* check the result */
    {
        handle->debug_print("nrf905: len is over %d.\n", handle->conf[NRF905_REG_TX_PW]);        /* len is over conf */
       
        return 4;                                                                                /* return error */
    }
    
    if (handle->ce_gpio_write(0) != 0)                                                           /* disable the chip */
    {
        handle->debug_print("nrf905: ce gpio write failed.\n");                                  /* ce gpio write failed */
        
        return 1;                                                                                /* return error */
    }
    if (handle->tx_en_gpio_write(NRF905_MODE_TX) != 0)                                           /* set tx mode */
    {
        handle->debug_print("nrf905: tx en gpio write failed.\n");                               /* tx en gpio write failed */
        
        return 1;                                                                                /* return error */
    }
    handle->delay_ms(1);                                                                         /* wait 1 ms */
    res = a_nrf905_spi_write(handle, NRF905_COMMAND_W_TX_PAYLOAD, buf, len);                     /* write tx payload */
    if (res != 0)                                                                                /* check result */
    {
        handle->debug_print("nrf905: set tx payload failed.\n");                                 /* set tx payload failed */
       
        return 1;                                                                                /* return error */
    }
    handle->finished = 0;                                                                        /* clear finished */
    handle->mode = NRF905_MODE_TX;                                                               /* set the tx mode */
    if (handle->ce_gpio_write(1) != 0)                                                           /* enable the chip */
    {
        handle->debug_print("nrf905: ce gpio write failed.\n");                                  /* ce gpio write failed */
        
        return 1;                                                                                /* return error */
    }
    timeout = 5000;                                                                              /* set timeout */
    while ((timeout != 0) && (handle->finished == 0))                                            /* wait time */
    {
        handle->delay_ms(1);                                                                     /* delay 1 ms */
        timeout--;                                                                               /* timeout-- */
    }
    if (handle->ce_gpio_write(0) != 0)                                                           /* disable the chip */
    {
        handle->debug_print("nrf905: ce gpio write failed.\n");                                  /* ce gpio write failed */
        
        return 1;                                                                                /* return error */
    }
    if (timeout == 0)                                                                            /* check timeout */
    {
        handle->debug_print("nrf905: send timeout.\n");                                          /* send timeout failed */
       
        return 5;                                                                                /* return error */
    }
    if (handle->finished == 1)                                                                   /* check finished */
    {
        return 0;                                                                                /* success return 0 */
    }
    else
    {
        handle->debug_print("nrf905: send failed.\n");                                           /* send failed */
       
        return 1;                                                                                /* return error */
    }
}

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
uint8_t nrf905_write_conf(nrf905_handle_t *handle)
{
    uint8_t res;
    
    if (handle == NULL)                                                                /* check handle */
    {
        return 2;                                                                      /* return error */
    }
    if (handle->inited != 1)                                                           /* check handle initialization */
    {
        return 3;                                                                      /* return error */
    }
    
    res = a_nrf905_spi_write(handle, NRF905_COMMAND_W_CONFIG, handle->conf, 10);       /* write conf */
    if (res != 0)                                                                      /* check result */
    {
        handle->debug_print("nrf905: write conf failed.\n");                           /* write conf failed */
       
        return 1;                                                                      /* return error */
    }
    
    return 0;                                                                          /* success return 0 */
}

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
uint8_t nrf905_read_conf(nrf905_handle_t *handle, uint8_t conf[10])
{
    uint8_t res;
    
    if (handle == NULL)                                                       /* check handle */
    {
        return 2;                                                             /* return error */
    }
    if (handle->inited != 1)                                                  /* check handle initialization */
    {
        return 3;                                                             /* return error */
    }
    
    res = a_nrf905_spi_read(handle, NRF905_COMMAND_R_CONFIG, conf, 10);       /* read conf */
    if (res != 0)                                                             /* check result */
    {
        handle->debug_print("nrf905: read conf failed.\n");                   /* read conf failed */
       
        return 1;                                                             /* return error */
    }
    
    return 0;                                                                 /* success return 0 */
}

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
uint8_t nrf905_update_conf(nrf905_handle_t *handle, uint8_t conf[10])
{
    if (handle == NULL)                    /* check handle */
    {
        return 2;                          /* return error */
    }
    if (handle->inited != 1)               /* check handle initialization */
    {
        return 3;                          /* return error */
    }
    
    memcpy(handle->conf, conf, 10);        /* copy the conf */
    
    return 0;                              /* success return 0 */
}

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
uint8_t nrf905_set_tx_payload(nrf905_handle_t *handle, uint8_t *buf, uint8_t len)
{
    uint8_t res;
    
    if (handle == NULL)                                                            /* check handle */
    {
        return 2;                                                                  /* return error */
    }
    if (handle->inited != 1)                                                       /* check handle initialization */
    {
        return 3;                                                                  /* return error */
    }
    if (len > 32)                                                                  /* check the result */
    {
        handle->debug_print("nrf905: len is over 32.\n");                          /* len is over 32 */
       
        return 4;                                                                  /* return error */
    }
    
    res = a_nrf905_spi_write(handle, NRF905_COMMAND_W_TX_PAYLOAD, buf, len);       /* write tx payload */
    if (res != 0)                                                                  /* check result */
    {
        handle->debug_print("nrf905: set tx payload failed.\n");                   /* set tx payload failed */
       
        return 1;                                                                  /* return error */
    }
    
    return 0;                                                                      /* success return 0 */
}

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
uint8_t nrf905_get_tx_payload(nrf905_handle_t *handle, uint8_t *buf, uint8_t len)
{
    uint8_t res;
    
    if (handle == NULL)                                                           /* check handle */
    {
        return 2;                                                                 /* return error */
    }
    if (handle->inited != 1)                                                      /* check handle initialization */
    {
        return 3;                                                                 /* return error */
    }
    if (len > 32)                                                                 /* check the result */
    {
        handle->debug_print("nrf905: len is over 32.\n");                         /* len is over 32 */
       
        return 4;                                                                 /* return error */
    }
    
    res = a_nrf905_spi_read(handle, NRF905_COMMAND_R_TX_PAYLOAD, buf, len);       /* read tx payload */
    if (res != 0)                                                                 /* check result */
    {
        handle->debug_print("nrf905: get tx payload failed.\n");                  /* get tx payload failed */
       
        return 1;                                                                 /* return error */
    }
    
    return 0;                                                                     /* success return 0 */
}

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
uint8_t nrf905_get_rx_payload(nrf905_handle_t *handle, uint8_t *buf, uint8_t len)
{
    uint8_t res;
    
    if (handle == NULL)                                                           /* check handle */
    {
        return 2;                                                                 /* return error */
    }
    if (handle->inited != 1)                                                      /* check handle initialization */
    {
        return 3;                                                                 /* return error */
    }
    if (len > 32)                                                                 /* check the result */
    {
        handle->debug_print("nrf905: len is over 32.\n");                         /* len is over 32 */
       
        return 4;                                                                 /* return error */
    }
    
    res = a_nrf905_spi_read(handle, NRF905_COMMAND_R_RX_PAYLOAD, buf, len);       /* read rx payload */
    if (res != 0)                                                                 /* check result */
    {
        handle->debug_print("nrf905: get rx payload failed.\n");                  /* get rx payload failed */
       
        return 1;                                                                 /* return error */
    }
    
    return 0;                                                                     /* success return 0 */
}

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
uint8_t nrf905_set_tx_address(nrf905_handle_t *handle, uint8_t *addr, uint8_t len)
{
    uint8_t res;
    
    if (handle == NULL)                                                             /* check handle */
    {
        return 2;                                                                   /* return error */
    }
    if (handle->inited != 1)                                                        /* check handle initialization */
    {
        return 3;                                                                   /* return error */
    }
    if (len > 4)                                                                    /* check the result */
    {
        handle->debug_print("nrf905: len is over 4.\n");                            /* len is over 4 */
       
        return 4;                                                                   /* return error */
    }
    
    res = a_nrf905_spi_write(handle, NRF905_COMMAND_W_TX_ADDRESS, addr, len);       /* write tx address */
    if (res != 0)                                                                   /* check result */
    {
        handle->debug_print("nrf905: set tx address failed.\n");                    /* set tx address failed */
       
        return 1;                                                                   /* return error */
    }
    
    return 0;                                                                       /* success return 0 */
}

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
uint8_t nrf905_get_tx_address(nrf905_handle_t *handle, uint8_t *addr, uint8_t len)
{
    uint8_t res;
    
    if (handle == NULL)                                                            /* check handle */
    {
        return 2;                                                                  /* return error */
    }
    if (handle->inited != 1)                                                       /* check handle initialization */
    {
        return 3;                                                                  /* return error */
    }
    if (len > 4)                                                                   /* check the result */
    {
        handle->debug_print("nrf905: len is over 4.\n");                           /* len is over 4 */
       
        return 4;                                                                  /* return error */
    }
    
    res = a_nrf905_spi_read(handle, NRF905_COMMAND_R_TX_ADDRESS, addr, len);       /* read tx address */
    if (res != 0)                                                                  /* check result */
    {
        handle->debug_print("nrf905: get tx address failed.\n");                   /* get tx address failed */
       
        return 1;                                                                  /* return error */
    }
    
    return 0;                                                                      /* success return 0 */
}

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
uint8_t nrf905_channel_config(nrf905_handle_t *handle, uint16_t freq, nrf905_pll_mode_t mode, nrf905_output_power_t power)
{
    uint8_t res;
    uint8_t buf[2];
    
    if (handle == NULL)                                                      /* check handle */
    {
        return 2;                                                            /* return error */
    }
    if (handle->inited != 1)                                                 /* check handle initialization */
    {
        return 3;                                                            /* return error */
    }
    if (freq > 0x1FF)                                                        /* check the result */
    {
        handle->debug_print("nrf905: freq is over 0x1FF.\n");                /* freq is over 0x1FF */
       
        return 4;                                                            /* return error */
    }
    
    buf[0] = (uint8_t)(NRF905_COMMAND_CHANNEL_CONFIG |
                       power << 2 | mode << 1 | ((freq >> 8) & 0x1));        /* set buf0 */
    buf[1] = (uint8_t)(freq & 0xFF);                                         /* set buf1 */
    res = a_nrf905_spi_write(handle, buf[0], &buf[1], 1);                    /* channel config */
    if (res != 0)                                                            /* check result */
    {
        handle->debug_print("nrf905: channel config failed.\n");             /* channel config failed */
       
        return 1;                                                            /* return error */
    }
    
    return 0;                                                                /* success return 0 */
}

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
uint8_t nrf905_get_status(nrf905_handle_t *handle, uint8_t *status)
{
    uint8_t res;
    uint8_t reg;
    
    if (handle == NULL)                                                     /* check handle */
    {
        return 2;                                                           /* return error */
    }
    if (handle->inited != 1)                                                /* check handle initialization */
    {
        return 3;                                                           /* return error */
    }
    
    reg = 0xFF;                                                             /* set 0xFF */
    res = a_nrf905_spi_transmit(handle, &reg, (uint8_t *)status, 1);        /* read status */
    if (res != 0)                                                           /* check result */
    {
        handle->debug_print("nrf905: get status failed.\n");                /* get status failed */
       
        return 1;                                                           /* return error */
    }
    
    return 0;                                                               /* success return 0 */
}

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
uint8_t nrf905_set_frequency(nrf905_handle_t *handle, uint16_t freq)
{
    uint8_t prev;
    
    if (handle == NULL)                                              /* check handle */
    {
        return 2;                                                    /* return error */
    }
    if (handle->inited != 1)                                         /* check handle initialization */
    {
        return 3;                                                    /* return error */
    }
    if (freq > 0x1FF)                                                /* check the result */
    {
        handle->debug_print("nrf905: freq is over 0x1FF.\n");        /* freq is over 0x1FF */
       
        return 4;                                                    /* return error */
    }
    
    handle->conf[NRF905_REG_CH_NO] = freq & 0xFF;                    /* set the frequency */
    prev = handle->conf[NRF905_REG_CONF1];                           /* get the conf1 */
    prev &= ~(1 << 0);                                               /* clear the frequency */
    prev |= (freq >> 8) & 0x01;                                      /* set the frequency */
    handle->conf[NRF905_REG_CONF1] = prev;                           /* set the conf */
    
    return 0;                                                        /* success return 0 */
}

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
uint8_t nrf905_get_frequency(nrf905_handle_t *handle, uint16_t *freq)
{
    uint8_t prev;
    
    if (handle == NULL)                           /* check handle */
    {
        return 2;                                 /* return error */
    }
    if (handle->inited != 1)                      /* check handle initialization */
    {
        return 3;                                 /* return error */
    }
    
    *freq = 0;                                    /* set freq 0 */
    prev = handle->conf[NRF905_REG_CH_NO];        /* set the frequency */
    *freq |= prev;                                /* set the freq */
    prev = handle->conf[NRF905_REG_CONF1];        /* get the conf1 */
    *freq |= ((uint16_t)prev & 0x01) << 8;        /* set the frequency */
    
    return 0;                                     /* success return 0 */
}

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
uint8_t nrf905_frequency_convert_to_register(nrf905_handle_t *handle, float mhz, uint16_t *reg)
{
    uint8_t bw;
    
    if (handle == NULL)                                          /* check handle */
    {
        return 2;                                                /* return error */
    }
    if (handle->inited != 1)                                     /* check handle initialization */
    {
        return 3;                                                /* return error */
    }
    
    bw = (handle->conf[NRF905_REG_CONF1] >> 1) & 0x01;           /* set bw */
    *reg = (uint16_t)(((mhz / (1 + bw)) - 422.4f) * 10.0f);      /* convert real data to register data */
    
    return 0;                                                    /* success return 0 */
}

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
uint8_t nrf905_frequency_convert_to_data(nrf905_handle_t *handle, uint16_t reg, float *mhz)
{
    uint8_t bw;
    
    if (handle == NULL)                                            /* check handle */
    {
        return 2;                                                  /* return error */
    }
    if (handle->inited != 1)                                       /* check handle initialization */
    {
        return 3;                                                  /* return error */
    }
    
    bw = (handle->conf[NRF905_REG_CONF1] >> 1) & 0x01;             /* set bw */
    *mhz = (float)(422.4f + (float)reg / 10.0f) * (1 + bw);        /* convert real data to register data */
    
    return 0;                                                      /* success return 0 */
}

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
uint8_t nrf905_set_pll_mode(nrf905_handle_t *handle, nrf905_pll_mode_t mode)
{
    uint8_t prev;
    
    if (handle == NULL)                           /* check handle */
    {
        return 2;                                 /* return error */
    }
    if (handle->inited != 1)                      /* check handle initialization */
    {
        return 3;                                 /* return error */
    }

    prev = handle->conf[NRF905_REG_CONF1];        /* get the conf1 */
    prev &= ~(1 << 1);                            /* clear the config */
    prev |= mode << 1;                            /* set the config */
    handle->conf[NRF905_REG_CONF1] = prev;        /* set the conf */
    
    return 0;                                     /* success return 0 */
}

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
uint8_t nrf905_get_pll_mode(nrf905_handle_t *handle, nrf905_pll_mode_t *mode)
{
    uint8_t prev;
    
    if (handle == NULL)                                     /* check handle */
    {
        return 2;                                           /* return error */
    }
    if (handle->inited != 1)                                /* check handle initialization */
    {
        return 3;                                           /* return error */
    }

    prev = handle->conf[NRF905_REG_CONF1];                  /* get the conf1 */
    *mode = (nrf905_pll_mode_t)((prev >> 1) & 0x01);        /* get the mode */
    
    return 0;                                               /* success return 0 */
}

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
uint8_t nrf905_set_output_power(nrf905_handle_t *handle, nrf905_output_power_t output_power)
{
    uint8_t prev;
    
    if (handle == NULL)                           /* check handle */
    {
        return 2;                                 /* return error */
    }
    if (handle->inited != 1)                      /* check handle initialization */
    {
        return 3;                                 /* return error */
    }

    prev = handle->conf[NRF905_REG_CONF1];        /* get the conf1 */
    prev &= ~(3 << 2);                            /* clear the config */
    prev |= output_power << 2;                    /* set the config */
    handle->conf[NRF905_REG_CONF1] = prev;        /* set the conf */
    
    return 0;                                     /* success return 0 */
}

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
uint8_t nrf905_get_output_power(nrf905_handle_t *handle, nrf905_output_power_t *output_power)
{
    uint8_t prev;
    
    if (handle == NULL)                                                 /* check handle */
    {
        return 2;                                                       /* return error */
    }
    if (handle->inited != 1)                                            /* check handle initialization */
    {
        return 3;                                                       /* return error */
    }

    prev = handle->conf[NRF905_REG_CONF1];                              /* get the conf1 */
    *output_power = (nrf905_output_power_t)((prev >> 2) & 0x03);        /* get the output power */
    
    return 0;                                                           /* success return 0 */
}

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
uint8_t nrf905_set_rx_mode(nrf905_handle_t *handle, nrf905_rx_mode_t mode)
{
    uint8_t prev;
    
    if (handle == NULL)                           /* check handle */
    {
        return 2;                                 /* return error */
    }
    if (handle->inited != 1)                      /* check handle initialization */
    {
        return 3;                                 /* return error */
    }

    prev = handle->conf[NRF905_REG_CONF1];        /* get the conf1 */
    prev &= ~(1 << 4);                            /* clear the config */
    prev |= mode << 4;                            /* set the config */
    handle->conf[NRF905_REG_CONF1] = prev;        /* set the conf */
    
    return 0;                                     /* success return 0 */
}

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
uint8_t nrf905_get_rx_mode(nrf905_handle_t *handle, nrf905_rx_mode_t *mode)
{
    uint8_t prev;
    
    if (handle == NULL)                                    /* check handle */
    {
        return 2;                                          /* return error */
    }
    if (handle->inited != 1)                               /* check handle initialization */
    {
        return 3;                                          /* return error */
    }

    prev = handle->conf[NRF905_REG_CONF1];                 /* get the conf1 */
    *mode = (nrf905_rx_mode_t)((prev >> 4) & 0x01);        /* get the mode */
    
    return 0;                                              /* success return 0 */
}

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
uint8_t nrf905_set_auto_retransmit(nrf905_handle_t *handle, nrf905_bool_t enable)
{
    uint8_t prev;
    
    if (handle == NULL)                           /* check handle */
    {
        return 2;                                 /* return error */
    }
    if (handle->inited != 1)                      /* check handle initialization */
    {
        return 3;                                 /* return error */
    }

    prev = handle->conf[NRF905_REG_CONF1];        /* get the conf1 */
    prev &= ~(1 << 5);                            /* clear the config */
    prev |= enable << 5;                          /* set the config */
    handle->conf[NRF905_REG_CONF1] = prev;        /* set the conf */
    
    return 0;                                     /* success return 0 */
}

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
uint8_t nrf905_get_auto_retransmit(nrf905_handle_t *handle, nrf905_bool_t *enable)
{
    uint8_t prev;
    
    if (handle == NULL)                                   /* check handle */
    {
        return 2;                                         /* return error */
    }
    if (handle->inited != 1)                              /* check handle initialization */
    {
        return 3;                                         /* return error */
    }

    prev = handle->conf[NRF905_REG_CONF1];                /* get the conf1 */
    *enable = (nrf905_bool_t)((prev >> 5) & 0x01);        /* get enable */
    
    return 0;                                             /* success return 0 */
}

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
uint8_t nrf905_set_rx_address_width(nrf905_handle_t *handle, nrf905_address_width_t width)
{
    uint8_t prev;
    
    if (handle == NULL)                           /* check handle */
    {
        return 2;                                 /* return error */
    }
    if (handle->inited != 1)                      /* check handle initialization */
    {
        return 3;                                 /* return error */
    }

    prev = handle->conf[NRF905_REG_CONF2];        /* get the conf2 */
    prev &= ~(7 << 0);                            /* clear the config */
    prev |= width << 0;                           /* set the config */
    handle->conf[NRF905_REG_CONF2] = prev;        /* set the conf */
    
    return 0;                                     /* success return 0 */
}

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
uint8_t nrf905_get_rx_address_width(nrf905_handle_t *handle, nrf905_address_width_t *width)
{
    uint8_t prev;
    
    if (handle == NULL)                                           /* check handle */
    {
        return 2;                                                 /* return error */
    }
    if (handle->inited != 1)                                      /* check handle initialization */
    {
        return 3;                                                 /* return error */
    }

    prev = handle->conf[NRF905_REG_CONF2];                        /* get the conf2 */
    *width = (nrf905_address_width_t)((prev >> 0) & 0x07);        /* get the address width */
    
    return 0;                                                     /* success return 0 */
}

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
uint8_t nrf905_set_tx_address_width(nrf905_handle_t *handle, nrf905_address_width_t width)
{
    uint8_t prev;
    
    if (handle == NULL)                           /* check handle */
    {
        return 2;                                 /* return error */
    }
    if (handle->inited != 1)                      /* check handle initialization */
    {
        return 3;                                 /* return error */
    }

    prev = handle->conf[NRF905_REG_CONF2];        /* get the conf2 */
    prev &= ~(7 << 4);                            /* clear the config */
    prev |= width << 4;                           /* set the config */
    handle->conf[NRF905_REG_CONF2] = prev;        /* set the conf */
    
    return 0;                                     /* success return 0 */
}

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
uint8_t nrf905_get_tx_address_width(nrf905_handle_t *handle, nrf905_address_width_t *width)
{
    uint8_t prev;
    
    if (handle == NULL)                                           /* check handle */
    {
        return 2;                                                 /* return error */
    }
    if (handle->inited != 1)                                      /* check handle initialization */
    {
        return 3;                                                 /* return error */
    }

    prev = handle->conf[NRF905_REG_CONF2];                        /* get the conf2 */
    *width = (nrf905_address_width_t)((prev >> 4) & 0x07);        /* get the address width */
    
    return 0;                                                     /* success return 0 */
}

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
uint8_t nrf905_set_rx_payload_width(nrf905_handle_t *handle, uint8_t width)
{
    if (handle == NULL)                                              /* check handle */
    {
        return 2;                                                    /* return error */
    }
    if (handle->inited != 1)                                         /* check handle initialization */
    {
        return 3;                                                    /* return error */
    }
    if (width > 0x20)                                                /* check the result */
    {
        handle->debug_print("nrf905: width is over 0x20.\n");        /* width is over 0x20 */
       
        return 4;                                                    /* return error */
    }
    
    handle->conf[NRF905_REG_RX_PW] = width;                          /* set the rx pw */
    
    return 0;                                                        /* success return 0 */
}

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
uint8_t nrf905_get_rx_payload_width(nrf905_handle_t *handle, uint8_t *width)
{
    if (handle == NULL)                             /* check handle */
    {
        return 2;                                   /* return error */
    }
    if (handle->inited != 1)                        /* check handle initialization */
    {
        return 3;                                   /* return error */
    }
    
    *width = handle->conf[NRF905_REG_RX_PW];        /* get the rx pw */
    
    return 0;                                       /* success return 0 */
}

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
uint8_t nrf905_set_tx_payload_width(nrf905_handle_t *handle, uint8_t width)
{
    if (handle == NULL)                                              /* check handle */
    {
        return 2;                                                    /* return error */
    }
    if (handle->inited != 1)                                         /* check handle initialization */
    {
        return 3;                                                    /* return error */
    }
    if (width > 0x20)                                                /* check the result */
    {
        handle->debug_print("nrf905: width is over 0x20.\n");        /* width is over 0x20 */
       
        return 4;                                                    /* return error */
    }
    
    handle->conf[NRF905_REG_TX_PW] = width;                          /* set the tx pw */
    
    return 0;                                                        /* success return 0 */
}

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
uint8_t nrf905_get_tx_payload_width(nrf905_handle_t *handle, uint8_t *width)
{
    if (handle == NULL)                             /* check handle */
    {
        return 2;                                   /* return error */
    }
    if (handle->inited != 1)                        /* check handle initialization */
    {
        return 3;                                   /* return error */
    }
    
    *width = handle->conf[NRF905_REG_TX_PW];        /* get the tx pw */
    
    return 0;                                       /* success return 0 */
}

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
uint8_t nrf905_set_rx_address(nrf905_handle_t *handle, uint8_t addr[4])
{
    if (handle == NULL)                                             /* check handle */
    {
        return 2;                                                   /* return error */
    }
    if (handle->inited != 1)                                        /* check handle initialization */
    {
        return 3;                                                   /* return error */
    }
    
    memcpy(&handle->conf[NRF905_REG_RX_ADDRESS_0], addr, 4);        /* copy the rx address */
    
    return 0;                                                       /* success return 0 */
}

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
uint8_t nrf905_get_rx_address(nrf905_handle_t *handle, uint8_t addr[4])
{
    if (handle == NULL)                                             /* check handle */
    {
        return 2;                                                   /* return error */
    }
    if (handle->inited != 1)                                        /* check handle initialization */
    {
        return 3;                                                   /* return error */
    }
    
    memcpy(addr, &handle->conf[NRF905_REG_RX_ADDRESS_0], 4);        /* copy the rx address */
    
    return 0;                                                       /* success return 0 */
}

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
uint8_t nrf905_set_output_clock_frequency(nrf905_handle_t *handle, nrf905_output_clock_frequency_t freq)
{
    uint8_t prev;
    
    if (handle == NULL)                           /* check handle */
    {
        return 2;                                 /* return error */
    }
    if (handle->inited != 1)                      /* check handle initialization */
    {
        return 3;                                 /* return error */
    }

    prev = handle->conf[NRF905_REG_CONF3];        /* get the conf3 */
    prev &= ~(3 << 0);                            /* clear the config */
    prev |= freq << 0;                            /* set the config */
    handle->conf[NRF905_REG_CONF3] = prev;        /* set the conf */
    
    return 0;                                     /* success return 0 */
}

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
uint8_t nrf905_get_output_clock_frequency(nrf905_handle_t *handle, nrf905_output_clock_frequency_t *freq)
{
    uint8_t prev;
    
    if (handle == NULL)                                                  /* check handle */
    {
        return 2;                                                        /* return error */
    }
    if (handle->inited != 1)                                             /* check handle initialization */
    {
        return 3;                                                        /* return error */
    }

    prev = handle->conf[NRF905_REG_CONF3];                               /* get the conf3 */
    *freq = (nrf905_output_clock_frequency_t)((prev >> 0) & 0x3);        /* get the freq */
    
    return 0;                                                            /* success return 0 */
}

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
uint8_t nrf905_set_output_clock(nrf905_handle_t *handle, nrf905_bool_t enable)
{
    uint8_t prev;
    
    if (handle == NULL)                           /* check handle */
    {
        return 2;                                 /* return error */
    }
    if (handle->inited != 1)                      /* check handle initialization */
    {
        return 3;                                 /* return error */
    }

    prev = handle->conf[NRF905_REG_CONF3];        /* get the conf3 */
    prev &= ~(1 << 2);                            /* clear the config */
    prev |= enable << 2;                          /* set the config */
    handle->conf[NRF905_REG_CONF3] = prev;        /* set the conf */
    
    return 0;                                     /* success return 0 */
}

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
uint8_t nrf905_get_output_clock(nrf905_handle_t *handle, nrf905_bool_t *enable)
{
    uint8_t prev;
    
    if (handle == NULL)                                  /* check handle */
    {
        return 2;                                        /* return error */
    }
    if (handle->inited != 1)                             /* check handle initialization */
    {
        return 3;                                        /* return error */
    }

    prev = handle->conf[NRF905_REG_CONF3];               /* get the conf3 */
    *enable = (nrf905_bool_t)((prev >> 2) & 0x1);        /* get the bool value */
    
    return 0;                                            /* success return 0 */
}

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
uint8_t nrf905_set_crystal_oscillator_frequency(nrf905_handle_t *handle, nrf905_crystal_oscillator_frequency_t freq)
{
    uint8_t prev;
    
    if (handle == NULL)                           /* check handle */
    {
        return 2;                                 /* return error */
    }
    if (handle->inited != 1)                      /* check handle initialization */
    {
        return 3;                                 /* return error */
    }

    prev = handle->conf[NRF905_REG_CONF3];        /* get the conf3 */
    prev &= ~(7 << 3);                            /* clear the config */
    prev |= freq << 3;                            /* set the config */
    handle->conf[NRF905_REG_CONF3] = prev;        /* set the conf */
    
    return 0;                                     /* success return 0 */
}

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
uint8_t nrf905_get_crystal_oscillator_frequency(nrf905_handle_t *handle, nrf905_crystal_oscillator_frequency_t *freq)
{
    uint8_t prev;
    
    if (handle == NULL)                                                        /* check handle */
    {
        return 2;                                                              /* return error */
    }
    if (handle->inited != 1)                                                   /* check handle initialization */
    {
        return 3;                                                              /* return error */
    }
    
    prev = handle->conf[NRF905_REG_CONF3];                                     /* get the conf3 */
    *freq = (nrf905_crystal_oscillator_frequency_t)((prev >> 3) & 0x7);        /* get the frequency */
    
    return 0;                                                                  /* success return 0 */
}

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
uint8_t nrf905_set_crc(nrf905_handle_t *handle, nrf905_bool_t enable)
{
    uint8_t prev;
    
    if (handle == NULL)                           /* check handle */
    {
        return 2;                                 /* return error */
    }
    if (handle->inited != 1)                      /* check handle initialization */
    {
        return 3;                                 /* return error */
    }

    prev = handle->conf[NRF905_REG_CONF3];        /* get the conf3 */
    prev &= ~(1 << 6);                            /* clear the config */
    prev |= enable << 6;                          /* set the config */
    handle->conf[NRF905_REG_CONF3] = prev;        /* set the conf */
    
    return 0;                                     /* success return 0 */
}

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
uint8_t nrf905_get_crc(nrf905_handle_t *handle, nrf905_bool_t *enable)
{
    uint8_t prev;
    
    if (handle == NULL)                                  /* check handle */
    {
        return 2;                                        /* return error */
    }
    if (handle->inited != 1)                             /* check handle initialization */
    {
        return 3;                                        /* return error */
    }

    prev = handle->conf[NRF905_REG_CONF3];               /* get the conf3 */
    *enable = (nrf905_bool_t)((prev >> 6) & 0x1);        /* get the bool value */
    
    return 0;                                            /* success return 0 */
}

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
uint8_t nrf905_set_crc_mode(nrf905_handle_t *handle, nrf905_crc_mode_t mode)
{
    uint8_t prev;
    
    if (handle == NULL)                           /* check handle */
    {
        return 2;                                 /* return error */
    }
    if (handle->inited != 1)                      /* check handle initialization */
    {
        return 3;                                 /* return error */
    }

    prev = handle->conf[NRF905_REG_CONF3];        /* get the conf3 */
    prev &= ~(1 << 7);                            /* clear the config */
    prev |= mode << 7;                            /* set the config */
    handle->conf[NRF905_REG_CONF3] = prev;        /* set the conf */
    
    return 0;                                     /* success return 0 */
}

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
uint8_t nrf905_get_crc_mode(nrf905_handle_t *handle, nrf905_crc_mode_t *mode)
{
    uint8_t prev;
    
    if (handle == NULL)                                    /* check handle */
    {
        return 2;                                          /* return error */
    }
    if (handle->inited != 1)                               /* check handle initialization */
    {
        return 3;                                          /* return error */
    }

    prev = handle->conf[NRF905_REG_CONF3];                 /* get the conf3 */
    *mode = (nrf905_crc_mode_t)((prev >> 7) & 0x1);        /* get the mode */
    
    return 0;                                              /* success return 0 */
}

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
uint8_t nrf905_set_reg(nrf905_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle == NULL)                                     /* check handle */
    {
        return 2;                                           /* return error */
    }
    if (handle->inited != 1)                                /* check handle initialization */
    {
        return 3;                                           /* return error */
    }
    
    return a_nrf905_spi_write(handle, reg, buf, len);       /* write data */
}

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
uint8_t nrf905_get_reg(nrf905_handle_t *handle, uint8_t reg, uint8_t *buf, uint16_t len)
{
    if (handle == NULL)                                    /* check handle */
    {
        return 2;                                          /* return error */
    }
    if (handle->inited != 1)                               /* check handle initialization */
    {
        return 3;                                          /* return error */
    }
    
    return a_nrf905_spi_read(handle, reg, buf, len);       /* read data */
}

/**
 * @brief      get chip's information
 * @param[out] *info points to an nrf905 info structure
 * @return     status code
 *             - 0 success
 *             - 2 handle is NULL
 * @note       none
 */
uint8_t nrf905_info(nrf905_info_t *info)
{
    if (info == NULL)                                               /* check handle */
    {
        return 2;                                                   /* return error */
    }
    
    memset(info, 0, sizeof(nrf905_info_t));                         /* initialize nrf905 info structure */
    strncpy(info->chip_name, CHIP_NAME, 32);                        /* copy chip name */
    strncpy(info->manufacturer_name, MANUFACTURER_NAME, 32);        /* copy manufacturer name */
    strncpy(info->interface, "SPI", 8);                             /* copy interface name */
    info->supply_voltage_min_v = SUPPLY_VOLTAGE_MIN;                /* set minimal supply voltage */
    info->supply_voltage_max_v = SUPPLY_VOLTAGE_MAX;                /* set maximum supply voltage */
    info->max_current_ma = MAX_CURRENT;                             /* set maximum current */
    info->temperature_max = TEMPERATURE_MAX;                        /* set minimal temperature */
    info->temperature_min = TEMPERATURE_MIN;                        /* set maximum temperature */
    info->driver_version = DRIVER_VERSION;                          /* set driver version */
    
    return 0;                                                       /* success return 0 */
}
