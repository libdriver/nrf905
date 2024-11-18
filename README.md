[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver NRF905

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/nrf905/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

NRF905 is a single chip radio transceiver for the 433/868/915MHz ISM band. The transceiver consists of a fully integrated frequency synthesizer, receiver chain with demodulator, a power amplifier, a crystal oscillator and, a modulator. The ShockBurst™ feature automatically handles preamble and CRC. You can easily configure the nRF905 through the SPI. Current consumption is very low, in transmit only 9mA at an output power of -10dBm, and in receive mode 12.5mA. Built-in power down modes makes power saving easily realizable.NRF905 is used in wireless data communication,  alarm security system, home automation, remote control and so on.

LibDriver NRF905 is the full function driver of NRF905 launched by LibDriver.It provides functions of wireless senting, wireless receiving, etc. LibDriver is MISRA compliant.

### Table of Contents

  - [Instruction](#Instruction)
  - [Install](#Install)
  - [Usage](#Usage)
    - [example basic send](#example-basic-send)
    - [example basic receive](#example-basic-receive)
  - [Document](#Document)
  - [Contributing](#Contributing)
  - [License](#License)
  - [Contact Us](#Contact-Us)

### Instruction

/src includes LibDriver NRF905 source files.

/interface includes LibDriver NRF905 SPI platform independent template.

/test includes LibDriver NRF905 driver test code and this code can test the chip necessary function simply.

/example includes LibDriver NRF905 sample code.

/doc includes LibDriver NRF905 offline document.

/datasheet includes NRF905 datasheet.

/project includes the common Linux and MCU development board sample code. All projects use the shell script to debug the driver and the detail instruction can be found in each project's README.md.

/misra includes the LibDriver MISRA code scanning results.

### Install

Reference /interface SPI platform independent template and finish your platform SPI driver.

Add the /src directory, the interface driver for your platform, and your own drivers to your project, if you want to use the default example drivers, add the /example directory to your project.

### Usage

You can refer to the examples in the /example directory to complete your own driver. If you want to use the default programming examples, here's how to use them.

#### example basic send

```C
#include "driver_nrf905_basic.h"

uint8_t (*g_gpio_irq)(void) = NULL;
uint8_t res;
uint8_t addr[4] = NRF905_BASIC_DEFAULT_RX_ADDR;
char data[] = "libdriver";

...

static void a_callback(uint8_t type, uint8_t *buf, uint8_t len)
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
            uint8_t i;
            
            nrf905_interface_debug_print("nrf905: irq receive with length %d.\n", len);
            for (i = 0; i < len; i++)
            {
                nrf905_interface_debug_print("%c", buf[i]);
            }
            nrf905_interface_debug_print(".\n");
            
            break;
        }
        default :
        {
            nrf905_interface_debug_print("nrf905: unknown code.\n");
            
            break;
        }
    }
}

...
    
res = gpio_interrupt_init();
if (res != 0)
{
    return 1;
}
g_gpio_irq = nrf905_interrupt_irq_handler;
res = nrf905_basic_init(NRF905_MODE_TX, a_callback);
if (res != 0)
{
    (void)gpio_interrupt_deinit();
    g_gpio_irq = NULL;

    return 1;
}

...

if (nrf905_basic_send((uint8_t *)addr, (uint8_t *)data, (uint8_t)strlen(data)) != 0)
{
    (void)nrf905_basic_deinit();
    (void)gpio_interrupt_deinit();
    g_gpio_irq = NULL;
}

...

if (nrf905_basic_deinit() != 0)
{
    (void)gpio_interrupt_deinit();
    g_gpio_irq = NULL;
}
(void)gpio_interrupt_deinit();
g_gpio_irq = NULL;

...

return 0;
```
#### example basic receive

```c
#include "driver_nrf905_basic.h"

uint8_t (*g_gpio_irq)(void) = NULL;
uint8_t res;
uint32_t timeout;
uint8_t addr[4] = NRF905_BASIC_DEFAULT_RX_ADDR;

...

static void a_callback(uint8_t type, uint8_t *buf, uint8_t len)
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
            uint8_t i;
            
            nrf905_interface_debug_print("nrf905: irq receive with length %d.\n", len);
            for (i = 0; i < len; i++)
            {
                nrf905_interface_debug_print("%c", buf[i]);
            }
            nrf905_interface_debug_print(".\n");
            
            break;
        }
        default :
        {
            nrf905_interface_debug_print("nrf905: unknown code.\n");
            
            break;
        }
    }
}

...
    
timeout = 5000;
res = gpio_interrupt_init();
if (res != 0)
{
    return 1;
}
g_gpio_irq = nrf905_interrupt_irq_handler;
res = nrf905_basic_init(NRF905_MODE_RX, a_callback);
if (res != 0)
{
    (void)gpio_interrupt_deinit();
    g_gpio_irq = NULL;

    return 1;
}

...

nrf905_interface_delay_ms(timeout);

...

if (nrf905_basic_deinit() != 0)
{
    (void)gpio_interrupt_deinit();
    g_gpio_irq = NULL;
}
(void)gpio_interrupt_deinit();
g_gpio_irq = NULL;

...

return 0;
```

### Document

Online documents: [https://www.libdriver.com/docs/nrf905/index.html](https://www.libdriver.com/docs/nrf905/index.html).

Offline documents: /doc/html/index.html.

### Contributing

Please refer to CONTRIBUTING.md.

### License

Copyright (c) 2015 - present LibDriver All rights reserved



The MIT License (MIT) 



Permission is hereby granted, free of charge, to any person obtaining a copy

of this software and associated documentation files (the "Software"), to deal

in the Software without restriction, including without limitation the rights

to use, copy, modify, merge, publish, distribute, sublicense, and/or sell

copies of the Software, and to permit persons to whom the Software is

furnished to do so, subject to the following conditions: 



The above copyright notice and this permission notice shall be included in all

copies or substantial portions of the Software. 



THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR

IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,

FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE

AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER

LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,

OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE

SOFTWARE. 

### Contact Us

Please send an e-mail to lishifenging@outlook.com.