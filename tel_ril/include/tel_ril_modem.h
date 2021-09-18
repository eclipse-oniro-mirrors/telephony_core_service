/*
 * Copyright (C) 2021 Huawei Device Co., Ltd.
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

#ifndef TEL_RIL_COMMON_H
#define TEL_RIL_COMMON_H

#include "i_tel_ril_manager.h"
#include "tel_ril_base.h"

namespace OHOS {
namespace Telephony {
class TelRilModem : public TelRilBase {
public:
    TelRilModem(sptr<IRemoteObject> cellularRadio, std::shared_ptr<ObserverHandler> observerHandler);
    ~TelRilModem() = default;

    /**
     * @brief Turn on and off radio response (for flight mode)
     *
     * @param data is HDF service callback message
     */
    void SetRadioStatusResponse(MessageParcel &data);
    void GetRadioStatusResponse(MessageParcel &data);
    void ShutDown(const AppExecFwk::InnerEvent::Pointer &response) {}
    void RadioStateUpdated(MessageParcel &data);

    /**
     * @brief Radio Status Change response
     *
     * @param data is HDF service callback message
     */
    void SetRadioStatus(int fun, int rst, const AppExecFwk::InnerEvent::Pointer &response);
    void GetRadioStatus(const AppExecFwk::InnerEvent::Pointer &response);
    void ProcessCommonRespOrNotify(uint32_t code, MessageParcel &data);
    bool IsCommonRespOrNotify(uint32_t code);
    ModemPowerState radioState_ = ModemPowerState::CORE_SERVICE_POWER_NOT_AVAILABLE;

private:
    void AddHandlerToMap();
    bool IsCommonResponse(uint32_t code);
    bool IsCommonNotification(uint32_t code);

private:
    using Func = void (TelRilModem::*)(MessageParcel &data);
    std::map<uint32_t, Func> memberFuncMap_;
};
} // namespace Telephony
} // namespace OHOS
#endif // TEL_RIL_COMMON_H
