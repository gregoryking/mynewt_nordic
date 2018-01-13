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

#ifndef __QDEC_H__
#define __QDEC_H__

#include <os/os_dev.h>
#include "nrf_drv_qdec.h"

#ifdef __cplusplus
extern "C" {
#endif

struct qdec_dev;

/**
 * Read accumulated transitions on QDEC device.
 *
 * @param dev The device to read.
 * @param p_acc Pointer to store accumulated transitions.
 * @param p_accdbl Pointer to store accumulated double transitions.
 *
 */
typedef void (*qdec_accumulators_read_func_t)(struct qdec_dev *, int16_t *, int16_t *);


struct qdec_driver_funcs {
	qdec_accumulators_read_func_t qdec_accumulators_read;
};

struct qdec_cfg  {
    int phase_A_pin;
    int phase_B_pin;
    int LED_pin;
};

struct qdec_dev {
    struct os_dev qdec_os_dev;
    struct os_mutex qdec_lock;
    struct qdec_driver_funcs qdec_funcs;
    struct qdec_cfg cfg;
};

void qdec_accumulators_read(struct qdec_dev *dev, int16_t *p_acc, int16_t *p_accdbl);

#ifdef __cplusplus
}
#endif

#endif /* __QDEC_H__ */
