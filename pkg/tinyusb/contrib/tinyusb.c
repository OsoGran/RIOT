/*
 * Copyright (C) 2022 Gunar Schorcht
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#include "kernel_defines.h"
#include "thread.h"

#include "tusb.h"
#include "device/usbd.h"
#include "host/usbh.h"

#include "tinyusb.h"
#include "tinyusb_hw.h"

#define ENABLE_DEBUG    0
#include "debug.h"

static void *_tinyusb_thread_impl(void *arg)
{
    (void)arg;

    if (IS_USED(MODULE_TINYUSB_DEVICE)) {
        if (!tud_init(TINYUSB_TUD_RHPORT)) {
            DEBUG("tinyUSB device stack couldn't be initialized\n");
            assert(0);
        }
        DEBUG("tinyUSB device stack initialized\n");
    }

    if (IS_USED(MODULE_TINYUSB_HOST)) {
        if (!tuh_init(TINYUSB_TUH_RHPORT)) {
            DEBUG("tinyUSB host stack couldn't be initialized\n");
            assert(0);
        }
        DEBUG("tinyUSB host stack initialized\n");
    }

    while (1) {
        if (IS_USED(MODULE_TINYUSB_DEVICE)) {
            /* call tinyUSB device task blocking */
            tud_task();
            DEBUG("tinyUSB device task executed\n");
        }
        if (IS_USED(MODULE_TINYUSB_HOST)) {
            /* call tinyUSB device task blocking */
            tuh_task();
            DEBUG("tinyUSB host task executed\n");
        }
    }

    return NULL;
}

static char _tinyusb_thread_stack[TINYUSB_THREAD_STACKSIZE];

int tinyusb_setup(void)
{
    int res;

    if ((res = tinyusb_hw_init()) != 0) {
        DEBUG("tinyUSB peripherals couldn't be initialized\n");
        return res;
    }
    DEBUG("tinyUSB peripherals initialized\n");

    if ((res = thread_create(_tinyusb_thread_stack,
                             sizeof(_tinyusb_thread_stack),
                             TINYUSB_PRIORITY,
                             THREAD_CREATE_WOUT_YIELD | THREAD_CREATE_STACKTEST,
                             _tinyusb_thread_impl, NULL, "tinyusb")) < 0) {
        DEBUG("tinyUSB thread couldn't be created, reason %d\n", res);
        return res;
    }
    DEBUG("tinyUSB thread created\n");

    return 0;
}
