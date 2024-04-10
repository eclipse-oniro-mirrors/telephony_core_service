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

#ifndef OHOS_MULTI_SIM_MONITOR_H
#define OHOS_MULTI_SIM_MONITOR_H

#include <list>

#include "common_event_subscriber.h"
#include "iservice_registry.h"
#include "multi_sim_controller.h"
#include "sim_constant.h"
#include "sim_file_manager.h"
#include "system_ability_definition.h"
#include "system_ability_status_change_stub.h"
#include "tel_event_handler.h"
#include "telephony_log_wrapper.h"
#include "telephony_state_registry_client.h"

namespace OHOS {
namespace Telephony {
using namespace OHOS::EventFwk;
using CommonEventSubscribeInfo = OHOS::EventFwk::CommonEventSubscribeInfo;
using CommonEventSubscriber = OHOS::EventFwk::CommonEventSubscriber;
class MultiSimMonitor : public TelEventHandler {
public:
    explicit MultiSimMonitor(const std::shared_ptr<MultiSimController> &controller,
        std::vector<std::shared_ptr<Telephony::SimStateManager>> simStateManager,
        std::vector<std::weak_ptr<Telephony::SimFileManager>> simFileManager);
    ~MultiSimMonitor();

    void Init();
    void RegisterCoreNotify(int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler, int what);
    int32_t RegisterSimAccountCallback(const int32_t tokenId, const sptr<SimAccountCallback> &callback);
    int32_t UnregisterSimAccountCallback(const int32_t tokenId);
    void NotifySimAccountChanged();
    void RegisterSimNotify();
    void UnRegisterSimNotify();
    bool IsVSimSlotId(int32_t slotId);

public:
    enum {
        REGISTER_SIM_NOTIFY_EVENT = 0,
    };

private:
    struct SimAccountCallbackRecord {
        int32_t tokenId = 0;
        sptr<SimAccountCallback> simAccountCallback = nullptr;
    };

private:
    void ProcessEvent(const AppExecFwk::InnerEvent::Pointer &event);
    void RefreshData(int32_t slotId);
    void InitData(int32_t slotId);
    bool IsValidSlotId(int32_t slotId);
    std::list<SimAccountCallbackRecord> GetSimAccountCallbackRecords();

private:
    std::shared_ptr<MultiSimController> controller_ = nullptr;
    std::vector<std::shared_ptr<Telephony::SimStateManager>> simStateManager_;
    std::vector<std::weak_ptr<Telephony::SimFileManager>> simFileManager_;
    std::unique_ptr<ObserverHandler> observerHandler_ = nullptr;
    std::list<SimAccountCallbackRecord> listSimAccountCallbackRecord_;
    std::mutex mutexInner_;
    std::atomic<int32_t> remainCount_ = 30;
    int32_t maxSlotCount_ = 0;
};
} // namespace Telephony
} // namespace OHOS
#endif // OHOS_MULTI_SIM_MONITOR_H
