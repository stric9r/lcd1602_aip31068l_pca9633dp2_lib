/**
  ******************************************************************************
  * @file           : lcd_intfc.h
  * @brief          : Platform-agnostic serial-bus write interface
  ******************************************************************************
  * SPDX-License-Identifier: MIT
  * Copyright (C) 2026 Stric Roberts.
  *
  * Permission is hereby granted, free of charge, to any person obtaining a
  * copy of this software and associated documentation files (the "Software"),
  * to deal in the Software without restriction, including without limitation
  * the rights to use, copy, modify, merge, publish, distribute, sublicense,
  * and/or sell copies of the Software, and to permit persons to whom the
  * Software is furnished to do so, subject to the following conditions:
  *
  * The above copyright notice and this permission notice shall be included in
  * all copies or substantial portions of the Software.
  *
  * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
  * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
  * IN THE SOFTWARE.
  ******************************************************************************
  */

#ifndef LCD_INTFC_H
#define LCD_INTFC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>

/**
 * @defgroup lcd_intfc LCD Bus Interface
 * @brief Platform-agnostic communication interface for LCD peripheral drivers.
 *
 * Provide one translation unit that implements both functions for the target
 * platform. The Silicon Labs Gecko SDK 4.4.5 implementation is at
 * examples/silabs/gecko_sdk/lcd_intfc.c.
 *
 * To port to a new platform (SPI, different I2C SDK, bare-metal register
 * access), implement lcd_intfc_init() and lcd_intfc_write() in a new file
 * and add it to the build instead of the Gecko SDK version.
 * @{
 */

/** @brief Return type for lcd_intfc operations. */
typedef int lcd_intfc_status_t;

/** @brief Transaction completed without error. */
#define LCD_INTFC_OK     0

/** @brief Transaction failed (NACK, bus error, or timeout). */
#define LCD_INTFC_ERROR  (-1)

/**
 * @brief One-time bus interface initialisation.
 *
 * Call once at startup before any peripheral driver (aip31068l, pca9633dp2).
 */
void lcd_intfc_init(void);

/**
 * @brief Re-initialise the bus after a low-power sleep cycle.
 *
 * Call before any lcd_intfc_write() following lcd_intfc_teardown().
 */
void lcd_intfc_bringup(void);

/**
 * @brief De-initialise the bus before entering a low-power sleep cycle.
 *
 * Resets the I2C peripheral and disables its clock. Pair with
 * lcd_intfc_bringup() on wake.
 */
void lcd_intfc_teardown(void);

/**
 * @brief Write @p len bytes to the device at 7-bit address @p addr.
 *
 * Blocks until the transaction completes or fails. The buffer pointed to by
 * @p data must remain valid for the duration of the call.
 *
 * Callers may silently ignore LCD_INTFC_ERROR for optional peripherals
 * (e.g. backlight controller not fitted on a given board revision).
 *
 * @param addr   7-bit device address (not shifted).
 * @param data   Bytes to transmit; must not be NULL.
 * @param len    Number of bytes to transmit.
 * @return       LCD_INTFC_OK or LCD_INTFC_ERROR.
 */
lcd_intfc_status_t lcd_intfc_write(uint16_t addr, const uint8_t *data, size_t len);

/** @} */

#ifdef __cplusplus
}
#endif

#endif /* LCD_INTFC_H */
