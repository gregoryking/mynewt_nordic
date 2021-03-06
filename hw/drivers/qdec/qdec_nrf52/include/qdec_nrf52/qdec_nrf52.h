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

#ifndef __QDEC_NRF52_H__
#define __QDEC_NRF52_H__

#include <qdec/qdec.h>

#include <nrf.h>
#include <nrf_qdec.h>
#ifdef __cplusplus
extern "C" {
#endif

struct qdecParams {
	nrf_drv_qdec_config_t* init_conf;
	qdec_event_handler_t* eventHandler;
};

int nrf52_qdec_dev_init(struct os_dev *, void *);

#ifdef __cplusplus
}
#endif

#endif /* __QDEC_NRF52_H__ */
