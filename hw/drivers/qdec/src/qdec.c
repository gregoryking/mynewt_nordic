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


#include <qdec/qdec.h>
#include <errno.h>
#include <assert.h>

/**
 * Read accumulated transitions on QDEC device.
 *
 * @param dev The device to read.
 * @param p_acc Pointer to store accumulated transitions.
 * @param p_accdbl Pointer to store accumulated double transitions.
 *
 */
void
qdec_accumulators_read(struct qdec_dev *dev, int16_t *p_acc, int16_t *p_accdbl)
{
    assert(dev->qdec_funcs.qdec_accumulators_read != NULL);

    dev->qdec_funcs.qdec_accumulators_read(dev, p_acc, p_accdbl);
}
