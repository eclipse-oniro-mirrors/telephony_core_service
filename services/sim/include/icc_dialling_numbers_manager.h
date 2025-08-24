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

#ifndef OHOS_ICC_DIALLING_NUMBERS_MANAGER_H
#define OHOS_ICC_DIALLING_NUMBERS_MANAGER_H

#include <cstring>
#include <iostream>
#include <string>

#include "dialling_numbers_info.h"
#include "i_tel_ril_manager.h"
#include "icc_dialling_numbers_cache.h"
#include "sim_file_manager.h"
#include "tel_event_handler.h"

namespace OHOS {
namespace Telephony {
enum DiallingNumbersMessageType {
    MSG_SIM_ADVANCE_LOAD_PBR = 0,
    MSG_SIM_DIALLING_NUMBERS_GET_DONE  = 1,
    MSG_SIM_DIALLING_NUMBERS_UPDATE_DONE,
    MSG_SIM_DIALLING_NUMBERS_WRITE_DONE,
    MSG_SIM_DIALLING_NUMBERS_DELETE_DONE,
};
class IccDiallingNumbersManager : public TelEventHandler {
public:
    explicit IccDiallingNumbersManager(
        std::weak_ptr<SimFileManager> simFileManager, std::shared_ptr<SimStateManager> simState);
    ~IccDiallingNumbersManager();
    void ProcessEvent(const AppExecFwk::InnerEvent::Pointer &event);
    int32_t QueryIccDiallingNumbers(int type, std::vector<std::shared_ptr<DiallingNumbersInfo>> &result);
    int32_t AddIccDiallingNumbers(int type, const std::shared_ptr<DiallingNumbersInfo> &diallingNumber);
    int32_t DelIccDiallingNumbers(int type, const std::shared_ptr<DiallingNumbersInfo> &diallingNumber);
    int32_t UpdateIccDiallingNumbers(int type, const std::shared_ptr<DiallingNumbersInfo> &diallingNumber);
    void Init();
    static std::shared_ptr<IccDiallingNumbersManager> CreateInstance(
        std::weak_ptr<SimFileManager> simFile, std::shared_ptr<SimStateManager> simState);
    enum class HandleRunningState {
        STATE_NOT_START,
        STATE_RUNNING
    };

protected:
    std::shared_ptr<IccDiallingNumbersCache> diallingNumbersCache_ = nullptr;
    std::shared_ptr<AppExecFwk::EventRunner> eventLoopDiallingNumbers_ = nullptr;
    HandleRunningState stateDiallingNumbers_ = HandleRunningState::STATE_NOT_START;

private:
    std::weak_ptr<SimFileManager> simFileManager_;
    std::shared_ptr<Telephony::SimStateManager> simStateManager_ = nullptr;
    std::vector<std::shared_ptr<DiallingNumbersInfo>> diallingNumbersList_;
    std::mutex mtx_;
    std::mutex queryMtx_;
    bool hasEventDone_ = false;
    bool hasQueryEventDone_ = false;
    std::condition_variable processWait_;
    SimState currentSimState_ = SimState::SIM_STATE_NOT_PRESENT;
    void ProcessSimStateChanged();
    void ProcessLoadDone(const AppExecFwk::InnerEvent::Pointer &event);
    void ProcessUpdateDone(const AppExecFwk::InnerEvent::Pointer &event);
    void ProcessWriteDone(const AppExecFwk::InnerEvent::Pointer &event);
    void ProcessDeleteDone(const AppExecFwk::InnerEvent::Pointer &event);
    void ProcessAdvanceLoadPbr();
    AppExecFwk::InnerEvent::Pointer BuildCallerInfo(int eventId);

    void ClearRecords();
    int GetFileIdForType(int fileType);
    void FillResults(const std::shared_ptr<std::vector<std::shared_ptr<DiallingNumbersInfo>>> &listInfo);
    bool IsValidType(int type);
    bool HasSimCard();
    bool IsValidParam(int type, const std::shared_ptr<DiallingNumbersInfo> &info);
};
} // namespace Telephony
} // namespace OHOS
#endif // OHOS_ICC_DIALLING_NUMBERS_MANAGER_H
