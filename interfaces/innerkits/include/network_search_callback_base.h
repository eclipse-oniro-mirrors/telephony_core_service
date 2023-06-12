/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef NETWORK_SEARCH_CALLBACK_BASE_H
#define NETWORK_SEARCH_CALLBACK_BASE_H

#include <string>

#include "refbase.h"

namespace OHOS {
namespace Telephony {
class NetworkSearchCallBackBase : public virtual RefBase {
public:
    virtual ~NetworkSearchCallBackBase() = default;

    /**
     * @brief Whether cellular data has internet capability.
     *
     * @param slotId[in], sim slot id
     * @param cid[in], Context identification
     * @return Return true if has, false if hasn't.
     */
    virtual bool HasInternetCapability(int32_t slotId, int32_t cId)
    {
        return false;
    }
    /**
     * @brief Clear cellular data connections.
     *
     * @param slotId[in], sim slot id
     */
    virtual void ClearCellularDataConnections(int32_t slotId) {}
    /**
     * @brief Clear the call corresponding to slotId
     *
     * @param slotId[in], sim slot id
     */
    virtual void ClearCellularCallList(int32_t slotId) {}
    /**
     * @brief Set whether the device can make calls
     * @param slotId[in], sim slot id
     * @param callType[in], indicate the call type is cs or ims. 0 is cs, 1 is ims
     * @param isReadyToCall[in], indicate whether the device can make calls
     */
    virtual void SetReadyToCall(int32_t slotId, int32_t callType, bool isReadyToCall) {}
};
} // namespace Telephony
} // namespace OHOS
#endif // NET_SUPPLIER_CALLBACK_BASE_H