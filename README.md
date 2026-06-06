# lcd1602_aip31068l_pca9633dp2_lib

Portable C library for the **DFRobot Gravity LCD1602** family of I2C display modules.
Provides a clean application API (`lcd1602`) built on top of the AiP31068L LCD controller
driver and PCA9633DP2 RGB backlight driver. Platform-specific code is isolated to
`examples/<platform>/` so the library compiles unchanged on any target with an I2C bus.

---

## Directory layout

```
inc/
  lcd1602.h          High-level application API — start here
  aip31068l.h        AiP31068L I2C-native LCD controller driver header
  pca9633dp2.h       PCA9633DP2 RGB LED backlight driver header
  implement/
    lcd_intfc.h      Platform-agnostic serial-bus write interface (implement this to port)
    delay.h          Platform-agnostic blocking delay interface (implement this to port)

src/
  lcd1602.c          Application API implementation (no platform code)
  aip31068l.c        AiP31068L driver (calls lcd_intfc.h and delay.h only)
  pca9633dp2.c       PCA9633DP2 driver (calls lcd_intfc.h only)

examples/
  silabs/
    gecko_sdk/
      lcd_intfc.c    Silicon Labs I2C implementation (Gecko SDK 4.4.5, i2cspm: lcd1602)
      delay.c        Silicon Labs delay implementation (sl_udelay)
```

---

## Supported hardware

| Chip | Role | I2C address |
|---|---|---|
| AiP31068L | HD44780-compatible I2C LCD controller | 0x3E |
| PCA9633DP2 | RGB LED backlight (V1.0 / RGB V1.0) | 0x60 |
| V1.1 controller | RGB LED backlight (V1.1, blue or RGB) | 0x6B |

The `pca9633dp2` driver handles both backlight variants automatically — pass
`PCA9633DP2_ADDR_V10` or `PCA9633DP2_ADDR_V11` to match your board revision.

---

## Integrating into a project

### Include paths (add both)

```
inc/
inc/implement/
```

### Source files to compile

```
src/lcd1602.c
src/aip31068l.c
src/pca9633dp2.c
examples/<platform>/lcd_intfc.c   ← pick the one that matches your SDK
examples/<platform>/delay.c
```

For Silicon Labs Gecko SDK 4.4.5 projects, use `examples/silabs/gecko_sdk/`.

### Minimal usage

```c
#include "lcd1602.h"
#include "pca9633dp2.h"

// in app_init():
lcd_intfc_init();          // no-op on Gecko SDK (bus already up)
delay_init();              // no-op on Gecko SDK
lcd1602_init(2, false);    // 2-line, 5×8 font
pca9633dp2_init(PCA9633DP2_DEFAULT_ADDR);  // white backlight

lcd1602_write_string("Hello, world!");
lcd1602_set_cursor(0, 1);
lcd1602_write_string("LCD1602 lib v1");
```

---

## Porting to a new platform

Implement the two interfaces in `inc/implement/` for your target:

### `lcd_intfc.h` — bus communication

```c
// lcd_intfc.c for your platform
#include "lcd_intfc.h"

void lcd_intfc_init(void) { /* initialise your I2C/SPI peripheral */ }

lcd_intfc_status_t lcd_intfc_write(uint16_t addr, const uint8_t *data, size_t len)
{
    // write len bytes to 7-bit address addr
    // return LCD_INTFC_OK or LCD_INTFC_ERROR
}
```

### `delay.h` — blocking delay

```c
// delay.c for your platform
#include "delay.h"

void  delay_init(void)          { /* set up SysTick or timer if needed */ }
void  delay_us(uint32_t us)     { /* busy-wait at least us microseconds */ }
void  delay_ms(uint32_t ms)     { delay_us(ms * 1000UL); }
```

Add your two files to the build in place of the `examples/silabs/gecko_sdk/` files. No
changes to `src/` are required.

---

## License

MIT — see [LICENSE](LICENSE).
