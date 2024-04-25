/*
 * Copyright (C) 2024 Maxwell Tacker
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     drivers_st25dvxxx
 * @{
 *
 * @file
 * @brief       Default configuration for the ST25DVXXX driver
 *
 * @author      Maxwell Tacker <max@tacker.co>
 */

#ifndef ST25DVXXX_PARAMS_H
#define ST25DVXXX_PARAMS_H

#include "board.h"
#include "periph/gpio.h"
#include "st25dvxxx_defines.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @name    Set default configuration parameters for the ST25DVXXX driver
 * @{
 */
#ifndef ST25DVXXX_PARAM_I2C
/**
 * @brief  I2C bus the EEPROM is connected to
 */
#define ST25DVXXX_PARAM_I2C              (I2C_DEV(0))
#endif
#ifndef ST25DVXXX_PARAM_ADDR
/**
 * @brief   I2C address of the EEPROM device
 */
#define ST25DVXXX_PARAM_ADDR             (ST25DVXXX_DEF_DEV_ADDR)
#endif
#ifndef ST25DVXXX_PARAM_PIN_WP
/**
 * @brief   EEPROM write protect pin
 */
#define ST25DVXXX_PARAM_PIN_WP           (GPIO_UNDEF)
#endif
#ifndef ST25DVXXX_PARAM_EEPROM_SIZE
/**
 * @brief   EEPROM size
 */
#define ST25DVXXX_PARAM_EEPROM_SIZE      (ST25DVXXX_EEPROM_SIZE)
#endif
#ifndef ST25DVXXX_PARAM_PAGE_SIZE
/**
 * @brief   Page size
 */
#define ST25DVXXX_PARAM_PAGE_SIZE        (ST25DVXXX_PAGE_SIZE)
#endif
#ifndef ST25DVXXX_PARAM_MAX_POLLS
/**
 * @brief   Maximum poll poll
 */
#define ST25DVXXX_PARAM_MAX_POLLS        (ST25DVXXX_MAX_POLLS)
#endif
#ifndef ST25DVXXX_PARAMS
/**
 * @brief   Default device configuration parameters
 */
#define ST25DVXXX_PARAMS                {            \
    .i2c = ST25DVXXX_PARAM_I2C,                      \
    .pin_wp = ST25DVXXX_PARAM_PIN_WP,                \
    .eeprom_size = ST25DVXXX_PARAM_EEPROM_SIZE,      \
    .dev_addr = ST25DVXXX_PARAM_ADDR,                \
    .page_size = ST25DVXXX_PARAM_PAGE_SIZE,          \
    .max_polls = ST25DVXXX_PARAM_MAX_POLLS           \
}
#endif
/** @} */

/**
 * @brief Number of configured ST25DVXXX EEPROM devices
 */
#define ST25DVXXX_NUMOF ARRAY_SIZE(st25dvxxx_params)

/**
 * @brief   ST25DVXXX configuration
 */
static const st25dvxxx_params_t st25dvxxx_params[] =
{
    ST25DVXXX_PARAMS
};

#ifdef __cplusplus
}
#endif

#endif /* ST25DVXXX_PARAMS_H */
/** @} */
