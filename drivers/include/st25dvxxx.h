/*
 * Copyright (C) 2024 Max Tacker
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @defgroup    drivers_st25dvxxx ST25DVXXX Dynamic NFC/RFID Tag
 * @ingroup     drivers_st25dvxxx
 * @brief       Device driver interface for the ST25DVXXX Dynamic NFC/RFID Tags
 *
//  * @section overview Overview
//  * Various manufacturers such as Atmel/Microchip or ST offer small I2C EEPROMs which usually
//  * come in 8-pin packages and are used for persistent data storage of settings, counters, etc.
//  * This driver adds support for these devices with direct read and write functions.
//  *
//  * The high level wrapper for RIOTs MTD interface to utilize the I2C EEPROMs as MTD storage
//  * is described in drivers_mtd_st25dvxxx.
//  *
//  * A list of supported devices can be found in the st25dvxxx_defines.h file.
//  *
//  * @section usage Usage
//  *
//  * The preconfigured devices in the st25dvxxx_defines.h file devices are easily
//  * accessible as pseudomodules and can be added to the Makefile of your project:
//  *
//  *      USEMODULE += at24c02
//  *
//  * When using one of the pseudomodules, the configuration of the device is already
//  * predefined in the ST25DVXXX_PARAMS macro and can be used for the
//  * initialization:
//  *
//  *      st25dvxxx_t eeprom_dev;
//  *      st25dvxxx_params_t eeprom_params = ST25DVXXX_PARAMS;
//  *
//  *      st25dvxxx_init(&eeprom_dev, &eeprom_params);
//  *
//  * \n
//  * For other devices that are not yet part of the library, the generic module
//  * has to be added:
//  *
//  *      USEMODULE += st25dvxxx
//  *
//  * The predefined macro can not be used in this case, so the parameters of the
//  * device have to be added to the st25dvxxx_params_t structure manually with
//  * the values from the corresponding datasheet:
//  *
//  *      st25dvxxx_t eeprom_dev;
//  *      st25dvxxx_params_t eeprom_params = {
//  *          .i2c = I2C_DEV(0), \
//  *          ...
//  *      };
//  *
//  *      st25dvxxx_init(&eeprom_dev, &eeprom_params);
//  *
//  *
//  * @{
 *
 * @file
 * @brief       Device driver interface for the ST25DVXXX Dynamic NFC/RFID Tags.
 *
 * @author      Maxwell Tacker <max@tacker.co>
 *
 */

#ifndef ST25DVXXX_H
#define ST25DVXXX_H

#include <stdint.h>

#include "periph/gpio.h"
#include "periph/i2c.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief   Return values
 */
enum {
    ST25DVXXX_OK,
    ST25DVXXX_I2C_ERROR
};

/**
 * @brief   Struct that holds initialization parameters
 */
typedef struct st25dvxxx_params {
    i2c_t i2c;                      /**< I2C bus number */
    gpio_t pin_wp;                  /**< write protect pin */
    uint32_t eeprom_size;           /**< EEPROM memory capacity */
    uint8_t dev_addr;               /**< I2C device address */
    uint8_t page_size;              /**< page size */
    uint8_t max_polls;              /**< number of ACK poll attempts */
} st25dvxxx_params_t;

/**
 * @brief   Struct that represents an ST25DVXXX device
 */
typedef struct {
    st25dvxxx_params_t params;     /**< parameters */
} st25dvxxx_t;

/**
 * @brief   Initialize an ST25DVXXX device handle with ST25DVXXX parameters
 *
 * @param[in, out]  dev     ST25DVXXX device handle
 * @param[in]       params  ST25DVXXX parameters
 *
 * @return          ST25DVXXX_OK on success
 * @return          -ST25DVXXX_I2C_ERROR if i2c could not be acquired
 * @return          -EINVAL if input parameters are NULL
 */
int st25dvxxx_init(st25dvxxx_t *dev, const st25dvxxx_params_t *params);

/**
 * @brief   Read a byte at a given position @p pos
 *
 * @param[in]       dev      ST25DVXXX device handle
 * @param[in]       pos      Position in EEPROM memory
 * @param[out]      dest     Read byte
 *
 * @return          ST25DVXXX_OK on success
 * @return          -ERANGE if @p pos is out of bounds
 * @return          @see i2c_read_regs
 */
int st25dvxxx_read_byte(const st25dvxxx_t *dev, uint32_t pos, void *dest);

/**
 * @brief Sequentially read @p len bytes from a given position @p pos
 *
 * @param[in]       dev     ST25DVXXX device handle
 * @param[in]       pos     Position in EEPROM memory
 * @param[out]      data    Read buffer
 * @param[in]       len     Requested length to be read
 *
 * @return          ST25DVXXX_OK on success
 * @return          -ERANGE if @p pos + @p len is out of bounds
 * @return          @see i2c_read_regs
 */
int st25dvxxx_read(const st25dvxxx_t *dev, uint32_t pos, void *data, size_t len);

/**
 * @brief   Write a byte at a given position @p pos
 *
 * @param[in]       dev      ST25DVXXX device handle
 * @param[in]       pos      Position in EEPROM memory
 * @param[in]       data     Value to be written
 *
 * @return          ST25DVXXX_OK on success
 * @return          -ERANGE if @p pos is out of bounds
 * @return          @see i2c_write_regs
 */
int st25dvxxx_write_byte(const st25dvxxx_t *dev, uint32_t pos, uint8_t data);

/**
 * @brief   Sequentially write @p len bytes from a given position @p pos
 *
 * Writing is performed in chunks of size ST25DVXXX_PAGE_SIZE.
 *
 * @param[in]       dev       ST25DVXXX device handle
 * @param[in]       pos       Position in EEPROM memory
 * @param[in]       data      Write buffer
 * @param[in]       len       Requested length to be written
 *
 * @return          ST25DVXXX_OK on success
 * @return          -ERANGE if @p pos + @p len is out of bounds
 * @return          @see i2c_write_regs
 */
int st25dvxxx_write(const st25dvxxx_t *dev, uint32_t pos, const void *data,
                   size_t len);

/**
 * @brief   Set @p len bytes from a given position @p pos to the
 * value @p val
 *
 * Writing is performed in chunks of size ST25DVXXX_SET_BUFFER_SIZE.
 *
 * @param[in]       dev       ST25DVXXX device handle
 * @param[in]       pos       Position in EEPROM memory
 * @param[in]       val       Value to be set
 * @param[in]       len       Requested length to be written
 *
 * @return          ST25DVXXX_OK on success
 * @return          -ERANGE if @p pos + @p len is out of bounds
 * @return          @see i2c_write_byte
 */
int st25dvxxx_set(const st25dvxxx_t *dev, uint32_t pos, uint8_t val,
                 size_t len);

/**
 * @brief Set @p len bytes from position @p pos to
 * ST25DVXXX_CLEAR_BYTE
 *
 * This is a wrapper around @see st25dvxxx_set.
 *
 * @param[in]       dev       ST25DVXXX device handle
 * @param[in]       pos       Position in EEPROM memory
 * @param[in]       len       Requested length to be written
 *
 * @return          @see st25dvxxx_set
 */
int st25dvxxx_clear(const st25dvxxx_t *dev, uint32_t pos, size_t len);

/**
 * @brief Set the entire EEPROM memory to ST25DVXXX_CLEAR_BYTE
 *
 * This is a wrapper around @see st25dvxxx_clear.
 *
 * @param[in]       dev       ST25DVXXX device handle
 *
 * @return          @see st25dvxxx_set
 */
int st25dvxxx_erase(const st25dvxxx_t *dev);

/**
 * @brief Enable write protection
 *
 * @param[in]       dev       ST25DVXXX device handle
 *
 * @return          ST25DVXXX_OK on success
 * @return          -ENOTSUP if pin_wp was initialized with GPIO_UNDEF
 */
int st25dvxxx_enable_write_protect(const st25dvxxx_t *dev);

/**
 * @brief Disable write protection
 *
 * @param[in]       dev       ST25DVXXX device handle
 *
 * @return          ST25DVXXX_OK on success
 * @return          -ENOTSUP if pin_wp was initialized with GPIO_UNDEF
 */
int st25dvxxx_disable_write_protect(const st25dvxxx_t *dev);

#ifdef __cplusplus
}
#endif

#endif /* ST25DVXXX_H */
/** @} */
