/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#include <hal/hal_bsp.h>
#include <assert.h>
#include <os/os.h>
#include <errno.h>
#include <hal/hal_gpio.h>
#include <qdec/qdec.h>
#include <string.h>

/* Nordic headers */
#include <bsp.h>
#include <nrf.h>
#include <nrf_qdec.h>
#include <nrf_drv_qdec.h>
#include <nrf_drv_clock.h> /* check required */

#include <app_util_platform.h> /* check required */
#include <nrf_drv_ppi.h> /* check required */
#include <nrf_drv_timer.h> /* check required */

/* Mynewt Nordic driver */
#include "qdec_nrf52/qdec_nrf52.h"

struct nrf52_qdec_dev_global  {
    bool in_use;
};


struct nrf52_qdec_dev_global instance =
{
		.in_use = false,

};

/**
 * Empty qdec_event_handler
 * TO-DO: Complete when have a need to handle qdec reports
 */
static void qdec_event_handler(nrf_drv_qdec_event_t event)
{
    if (event.type == NRF_QDEC_EVENT_REPORTRDY)
    {
    	/* do some stuff */
        nrf_drv_qdec_disable();
    }
}

/**
 * Initialize the driver.
 */
static int
init(nrf_drv_qdec_config_t config)
{
	int stat;

	instance.in_use = true;

    stat = nrf_drv_qdec_init(&config, qdec_event_handler);
    if (stat != NRF_SUCCESS) {
    	return (stat);
    }

    /*
     * TODO: Enable the necessary GPIO pins as INPUTS
     */
//    hal_gpio_init_in(4, HAL_GPIO_PULL_DOWN);
//    hal_gpio_init_in(5, HAL_GPIO_PULL_DOWN);
//    hal_gpio_init_out(6, 0);

    nrf_drv_qdec_enable();

    return (0);
}


/**
 * Open the NRF52 QDEC device
 *
 * This function locks the device for access from other tasks.
 *
 * @param odev The OS device to open
 * @param wait The time in MS to wait.  If 0 specified, returns immediately
 *             if resource unavailable.  If OS_WAIT_FOREVER specified, blocks
 *             until resource is available.
 * @param arg  Argument provided by higher layer to open, in this case
 *             it can be a nrf_drv_qdec_config_t, to override the default
 *             configuration.
 *
 * @return 0 on success, non-zero on failure.
 */
static int
nrf52_qdec_open(struct os_dev *odev, uint32_t wait, void *arg)
{
    struct qdec_dev *dev;
    int stat = 0;
    dev = (struct qdec_dev *) odev;

    if(arg != NULL){
        memcpy(&dev->cfg, arg, sizeof(struct qdec_cfg));
    }

    if (os_started()) {
        stat = os_mutex_pend(&dev->qdec_lock, wait);
        if (stat != OS_OK) {
            return (stat);
        }
    }

    if (odev->od_flags & OS_DEV_F_STATUS_OPEN) {
        os_mutex_release(&dev->qdec_lock);
        stat = OS_EBUSY;
        return (stat);
    }

    nrf_drv_qdec_config_t config = NRF_DRV_QDEC_DEFAULT_CONFIG;
    config.psela = dev->cfg.phase_A_pin;
    config.pselb = dev->cfg.phase_B_pin;
    config.pselled = dev->cfg.LED_pin;
//    config.interrupt_priority = APP_IRQ_PRIORITY_HIGH;

    stat = init(config);
    if (stat) {
        return (stat);
    }

    return (0);
}

/**
 * Close the NRF52 QDEC device.
 *
 * This function unlocks the device.
 *
 * @param odev The device to close.
 */
static int
nrf52_qdec_close(struct os_dev *odev)
{
    struct qdec_dev *dev;

    dev = (struct qdec_dev *) odev;

    nrf_drv_qdec_disable();
    nrf_drv_qdec_uninit();
    instance.in_use = false;

    if (os_started()) {
        os_mutex_release(&dev->qdec_lock);
    }

    return (0);
}



/**
 * Read accumulated transitions on QDEC device.
 *
 * @param dev The device to read.
 * @param p_acc Pointer to store accumulated transitions.
 * @param p_accdbl Pointer to store accumulated double transitions.
 *
 */
static void
nrf52_qdec_accumulators_read(struct qdec_dev *dev, int16_t *p_acc, int16_t *p_accdbl)
{
	nrf_drv_qdec_accumulators_read(p_acc, p_accdbl);
}

/**
 * Callback to initialize an qdec_dev structure from the os device
 * initialization callback.  This sets up a nrf52_dev_device(), so
 * that subsequent lookups to this device allow us to manipulate it.
 */
int
nrf52_qdec_dev_init(struct os_dev *odev, void *arg)
{
    struct qdec_dev *dev;
    struct qdec_driver_funcs *qdec_funcs;

    dev = (struct qdec_dev *) odev;

    struct qdec_cfg *cfg = arg;
    dev->cfg = *cfg;
    /* TO-DO: Instance management, none provided for time being (may also not be appropriate...) */

    os_mutex_init(&dev->qdec_lock);

    OS_DEV_SETHANDLERS(odev, nrf52_qdec_open, nrf52_qdec_close);

    qdec_funcs = &dev->qdec_funcs;
    qdec_funcs->qdec_accumulators_read = nrf52_qdec_accumulators_read;

    return (0);
}
