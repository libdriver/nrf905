[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.svg" width="400" height="150"/>
</div>

## LibDriver NRF905

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/nrf905/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

NRF905是一款适用于433/868/915MHz ISM频段的单芯片无线电收发芯片。收发器由一个完全集成的频率合成器、带解调器的接收器链、功率放大器、晶体振荡器和调制器组成。ShockBurst™ 能自动处理前导码和CRC。您可以通过SPI轻松配置NRF905。芯片的电流消耗非常低，在输出功率为-10dBm的情况下，传输模式下仅为9mA，接收模式下为12.5mA，芯片内置掉电模式，省电容易实现。NRF905用于无线数据通信、报警安全系统、家庭自动化、远程控制等。

LibDriver NRF905是LibDriver推出的NRF905全功能驱动程序，提供无线发送、无线接收等功能并且它符合MISRA标准。

### 目录

  - [说明](#说明)
  - [安装](#安装)
  - [使用](#使用)
    - [example basic send](#example-basic-send)
    - [example basic receive](#example-basic-receive)
  - [文档](#文档)
  - [贡献](#贡献)
  - [版权](#版权)
  - [联系我们](#联系我们)

### 说明

/src目录包含了LibDriver NRF905的源文件。

/interface目录包含了LibDriver NRF905与平台无关的SPI总线模板。

/test目录包含了LibDriver NRF905驱动测试程序，该程序可以简单的测试芯片必要功能。

/example目录包含了LibDriver NRF905编程范例。

/doc目录包含了LibDriver NRF905离线文档。

/datasheet目录包含了NRF905数据手册。

/project目录包含了常用Linux与单片机开发板的工程样例。所有工程均采用shell脚本作为调试方法，详细内容可参考每个工程里面的README.md。

/misra目录包含了LibDriver MISRA代码扫描结果。

### 安装

参考/interface目录下与平台无关的SPI总线模板，完成指定平台的SPI总线驱动。

将/src目录，您使用平台的接口驱动和您开发的驱动加入工程，如果您想要使用默认的范例驱动，可以将/example目录加入您的工程。

### 使用

您可以参考/example目录下的编程范例完成适合您的驱动，如果您想要使用默认的编程范例，以下是它们的使用方法。

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

```C
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

### 文档

在线文档: [https://www.libdriver.com/docs/nrf905/index.html](https://www.libdriver.com/docs/nrf905/index.html)。

离线文档: /doc/html/index.html。

### 贡献

请参考CONTRIBUTING.md。

### 版权

版权 (c) 2015 - 现在 LibDriver 版权所有

MIT 许可证（MIT）

特此免费授予任何获得本软件副本和相关文档文件（下称“软件”）的人不受限制地处置该软件的权利，包括不受限制地使用、复制、修改、合并、发布、分发、转授许可和/或出售该软件副本，以及再授权被配发了本软件的人如上的权利，须在下列条件下：

上述版权声明和本许可声明应包含在该软件的所有副本或实质成分中。

本软件是“如此”提供的，没有任何形式的明示或暗示的保证，包括但不限于对适销性、特定用途的适用性和不侵权的保证。在任何情况下，作者或版权持有人都不对任何索赔、损害或其他责任负责，无论这些追责来自合同、侵权或其它行为中，还是产生于、源于或有关于本软件以及本软件的使用或其它处置。

### 联系我们

请联系lishifenging@outlook.com。