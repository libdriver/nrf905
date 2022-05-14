[English](/README.md) | [ 简体中文](/README_zh-Hans.md) | [繁體中文](/README_zh-Hant.md) | [日本語](/README_ja.md) | [Deutsch](/README_de.md) | [한국어](/README_ko.md)

<div align=center>
<img src="/doc/image/logo.png"/>
</div>

## LibDriver NRF905

[![MISRA](https://img.shields.io/badge/misra-compliant-brightgreen.svg)](/misra/README.md) [![API](https://img.shields.io/badge/api-reference-blue.svg)](https://www.libdriver.com/docs/nrf905/index.html) [![License](https://img.shields.io/badge/license-MIT-brightgreen.svg)](/LICENSE)

NRF905는 433/868/915MHz ISM 대역용 단일 칩 무선 송수신기입니다. 송수신기는 완전히 통합된 주파수 합성기, 복조기가 있는 수신기 체인, 전력 증폭기, 수정 발진기 및 변조기로 구성됩니다. ShockBurst™ 기능은 프리앰블과 CRC를 자동으로 처리합니다. SPI를 통해 nRF905를 쉽게 구성할 수 있습니다. 소비 전류는 -10dBm의 출력 전력에서 송신에서 9mA, 수신 모드에서 12.5mA로 매우 낮습니다. 내장된 절전 모드로 절전을 쉽게 실현할 수 있습니다. NRF905는 무선 데이터 통신, 경보 보안 시스템, 홈 오토메이션, 원격 제어 등에 사용됩니다.

LibDriver NRF905는 LibDriver에서 출시한 NRF905의 전체 기능 드라이버입니다. 무선 전송, 무선 수신 등의 기능을 제공합니다. LibDriver는 MISRA를 준수합니다.

### 目次

  - [説明](#説明)
  - [インストール](#インストール)
  - [使用](#使用)
    - [example basic sent](#example-basic-sent)
    - [example basic receive](#example-basic-receive)
  - [ドキュメント](#ドキュメント)
  - [貢献](#貢献)
  - [著作権](#著作権)
  - [連絡して](#連絡して)

### 説明

/ srcディレクトリには、LibDriver NRF905のソースファイルが含まれています。

/ interfaceディレクトリには、LibDriver NRF905用のプラットフォームに依存しないSPIバステンプレートが含まれています。

/ testディレクトリには、チップの必要な機能を簡単にテストできるLibDriver NRF905ドライバーテストプログラムが含まれています。

/ exampleディレクトリには、LibDriver NRF905プログラミング例が含まれています。

/ docディレクトリには、LibDriver NRF905オフラインドキュメントが含まれています。

/ datasheetディレクトリには、NRF905データシートが含まれています。

/ projectディレクトリには、一般的に使用されるLinuxおよびマイクロコントローラー開発ボードのプロジェクトサンプルが含まれています。 すべてのプロジェクトは、デバッグ方法としてシェルスクリプトを使用しています。詳細については、各プロジェクトのREADME.mdを参照してください。

### インストール

/ interfaceディレクトリにあるプラットフォームに依存しないSPIバステンプレートを参照して、指定したプラットフォームのSPIバスドライバを完成させます。

/ srcディレクトリ、/ interfaceディレクトリ、および/exampleディレクトリをプロジェクトに追加します。

### 使用

#### example basic sent

```C
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

if (nrf905_basic_sent((uint8_t *)addr, (uint8_t *)data, (uint8_t)strlen(data)) != 0)
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

### ドキュメント

オンラインドキュメント: https://www.libdriver.com/docs/nrf905/index.html

オフラインドキュメント: /doc/html/index.html

### 貢献

お問い合わせくださいlishifenging@outlook.com

### 著作権

著作権（c）2015-今 LibDriver 全著作権所有

MITライセンス（MIT）

このソフトウェアおよび関連するドキュメントファイル（「ソフトウェア」）のコピーを取得した人は、無制限の使用、複製、変更、組み込み、公開、配布、サブライセンスを含む、ソフトウェアを処分する権利を制限なく付与されます。ソフトウェアのライセンスおよび/またはコピーの販売、および上記のようにソフトウェアが配布された人の権利のサブライセンスは、次の条件に従うものとします。

上記の著作権表示およびこの許可通知は、このソフトウェアのすべてのコピーまたは実体に含まれるものとします。

このソフトウェアは「現状有姿」で提供され、商品性、特定目的への適合性、および非侵害の保証を含むがこれらに限定されない、明示または黙示を問わず、いかなる種類の保証もありません。 いかなる場合も、作者または著作権所有者は、契約、不法行為、またはその他の方法で、本ソフトウェアおよび本ソフトウェアの使用またはその他の廃棄に起因または関連して、請求、損害、またはその他の責任を負わないものとします。

### 連絡して

お問い合わせくださいlishifenging@outlook.com