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

#include "icc_dialling_numbers_manager.h"

#include "core_service_errors.h"
#include "radio_event.h"
#include "telephony_errors.h"

namespace OHOS {
namespace Telephony {
constexpr static const int32_t WAIT_TIME_SECOND = 1;
constexpr static const int32_t WAIT_QUERY_TIME_SECOND = 60;
constexpr static const int32_t DELAY_LOAD_PBR_MS = 5 * 1000;

IccDiallingNumbersManager::IccDiallingNumbersManager(
    std::weak_ptr<SimFileManager> simFileManager, std::shared_ptr<SimStateManager> simState)
    : TelEventHandler("IccDiallingNumbersManager"), simFileManager_(simFileManager), simStateManager_(simState)
{}

void IccDiallingNumbersManager::Init()
{
    TELEPHONY_LOGI("IccDiallingNumbersManager::Init() started ");
    if (stateDiallingNumbers_ == HandleRunningState::STATE_RUNNING) {
        TELEPHONY_LOGI("IccDiallingNumbersManager::Init eventLoopDiallingNumbers_ started.");
        return;
    }

    auto simFileManager = simFileManager_.lock();
    if (simFileManager == nullptr) {
        TELEPHONY_LOGE("SimFileManager null pointer");
        return;
    }
    diallingNumbersCache_ = std::make_shared<IccDiallingNumbersCache>(simFileManager);

    stateDiallingNumbers_ = HandleRunningState::STATE_RUNNING;

    diallingNumbersCache_->Init();
    simFileManager->RegisterCoreNotify(shared_from_this(), RadioEvent::RADIO_SIM_STATE_CHANGE);
    TELEPHONY_LOGI("Init() end");
    SendEvent(MSG_SIM_ADVANCE_LOAD_PBR, 0, DELAY_LOAD_PBR_MS);
}

void IccDiallingNumbersManager::ProcessEvent(const AppExecFwk::InnerEvent::Pointer &event)
{
    if (event == nullptr) {
        TELEPHONY_LOGE("event is nullptr!");
        return;
    }
    uint32_t id = event->GetInnerEventId();
    TELEPHONY_LOGD("IccDiallingNumbersManager ProcessEvent Id is %{public}d", id);
    switch (id) {
        case MSG_SIM_ADVANCE_LOAD_PBR:
            ProcessAdvanceLoadPbr();
            break;
        case MSG_SIM_DIALLING_NUMBERS_GET_DONE:
            ProcessLoadDone(event);
            break;
        case MSG_SIM_DIALLING_NUMBERS_UPDATE_DONE:
            ProcessUpdateDone(event);
            break;
        case MSG_SIM_DIALLING_NUMBERS_WRITE_DONE:
            ProcessWriteDone(event);
            break;
        case MSG_SIM_DIALLING_NUMBERS_DELETE_DONE:
            ProcessDeleteDone(event);
            break;
        case RadioEvent::RADIO_SIM_STATE_CHANGE:
            ProcessSimStateChanged();
            break;
        default:
            break;
    }
}

void IccDiallingNumbersManager::ProcessAdvanceLoadPbr()
{
    std::thread t([manager = std::static_pointer_cast<IccDiallingNumbersManager>(shared_from_this())] {
        std::vector<std::shared_ptr<DiallingNumbersInfo>> result;
        manager->QueryIccDiallingNumbers(DiallingNumbersInfo::SIM_ADN, result);
    });
    t.detach();
}

void IccDiallingNumbersManager::ProcessSimStateChanged()
{
    if (simStateManager_ == nullptr || diallingNumbersCache_ == nullptr) {
        TELEPHONY_LOGE("IccDiallingNumbersManager::ProcessSimStateChanged simStateManager_ is nullptr");
        return;
    }
    if (simStateManager_->GetSimState() == SimState::SIM_STATE_NOT_PRESENT) {
        TELEPHONY_LOGI("IccDiallingNumbersManager::ProcessSimStateChanged clear data when sim is absent");
        diallingNumbersCache_->ClearDiallingNumberCache();
    } else if (simStateManager_->GetSimState() == SimState::SIM_STATE_READY) {
        TELEPHONY_LOGI("IccDiallingNumbersManager::ProcessSimStateChanged reload when sim is ready");
        ProcessAdvanceLoadPbr();
    }
}

void IccDiallingNumbersManager::ProcessLoadDone(const AppExecFwk::InnerEvent::Pointer &event)
{
    TELEPHONY_LOGI("IccDiallingNumbersManager::ProcessLoadDone: start");
    std::unique_ptr<ResponseResult> object = event->GetUniqueObject<ResponseResult>();
    if (object != nullptr) {
        if (object->exception == nullptr) {
            std::shared_ptr<std::vector<std::shared_ptr<DiallingNumbersInfo>>> diallingNumberList =
                std::static_pointer_cast<std::vector<std::shared_ptr<DiallingNumbersInfo>>>(object->result);
            if (diallingNumberList != nullptr) {
                FillResults(diallingNumberList);
            } else {
                TELEPHONY_LOGE("ProcessDiallingNumberLoadDone: get null vectors!!!");
            }
        } else {
            TELEPHONY_LOGE("ProcessLoadDone: icc diallingnumbers get exception result");
        }
    } else {
        TELEPHONY_LOGE("ProcessDiallingNumberLoadDone: get null pointer!!!");
    }
    TELEPHONY_LOGI("IccDiallingNumbersManager::ProcessLoadDone: end");
    hasQueryEventDone_ = true;
    processWait_.notify_all();
}

void IccDiallingNumbersManager::ProcessUpdateDone(const AppExecFwk::InnerEvent::Pointer &event)
{
    std::unique_ptr<ResponseResult> object = event->GetUniqueObject<ResponseResult>();
    if (object != nullptr && object->exception != nullptr) {
        std::shared_ptr<RadioResponseInfo> responseInfo =
            std::static_pointer_cast<RadioResponseInfo>(object->exception);
        TELEPHONY_LOGE("IccDiallingNumbersManager::ProcessUpdateDone error %{public}d", responseInfo->error);
        hasEventDone_ = (responseInfo->error == ErrType::NONE);
    } else {
        hasEventDone_ = true;
    }
    TELEPHONY_LOGI("IccDiallingNumbersManager::ProcessUpdateDone: end");
    processWait_.notify_all();
}

void IccDiallingNumbersManager::ProcessWriteDone(const AppExecFwk::InnerEvent::Pointer &event)
{
    std::unique_ptr<ResponseResult> object = event->GetUniqueObject<ResponseResult>();
    if (object != nullptr && object->exception != nullptr) {
        std::shared_ptr<RadioResponseInfo> responseInfo =
            std::static_pointer_cast<RadioResponseInfo>(object->exception);
        TELEPHONY_LOGE("IccDiallingNumbersManager::ProcessWriteDone error %{public}d", responseInfo->error);
        hasEventDone_ = (responseInfo->error == ErrType::NONE);
    } else {
        hasEventDone_ = true;
    }
    TELEPHONY_LOGI("IccDiallingNumbersManager::ProcessWriteDone: end");
    processWait_.notify_all();
}

void IccDiallingNumbersManager::ProcessDeleteDone(const AppExecFwk::InnerEvent::Pointer &event)
{
    std::unique_ptr<ResponseResult> object = event->GetUniqueObject<ResponseResult>();
    if (object != nullptr && object->exception != nullptr) {
        std::shared_ptr<RadioResponseInfo> responseInfo =
            std::static_pointer_cast<RadioResponseInfo>(object->exception);
        TELEPHONY_LOGE("IccDiallingNumbersManager::ProcessDeleteDone error %{public}d", responseInfo->error);
        hasEventDone_ = (responseInfo->error == ErrType::NONE);
    } else {
        hasEventDone_ = true;
    }
    TELEPHONY_LOGI("IccDiallingNumbersManager::ProcessDeleteDone: end");
    processWait_.notify_all();
}

int32_t IccDiallingNumbersManager::UpdateIccDiallingNumbers(
    int type, const std::shared_ptr<DiallingNumbersInfo> &diallingNumber)
{
    std::unique_lock<std::mutex> lock(mtx_);
    if (diallingNumber == nullptr) {
        return TELEPHONY_ERR_LOCAL_PTR_NULL;
    }
    if (!HasSimCard()) {
        return TELEPHONY_ERR_NO_SIM_CARD;
    }
    if (!IsValidType(type) || !IsValidParam(type, diallingNumber)) {
        return TELEPHONY_ERR_ARGUMENT_INVALID;
    }
    int index = diallingNumber->GetIndex();
    TELEPHONY_LOGI("UpdateIccDiallingNumbers start: %{public}d %{public}d", type, index);
    int fileId = GetFileIdForType(type);
    AppExecFwk::InnerEvent::Pointer response = BuildCallerInfo(MSG_SIM_DIALLING_NUMBERS_UPDATE_DONE);
    hasEventDone_ = false;
    diallingNumbersCache_->UpdateDiallingNumberToIcc(fileId, diallingNumber, index, false, response);
    while (!hasEventDone_) {
        TELEPHONY_LOGI("UpdateIccDiallingNumbers::wait(), response = false");
        if (processWait_.wait_for(lock, std::chrono::seconds(WAIT_TIME_SECOND)) == std::cv_status::timeout) {
            break;
        }
    }
    TELEPHONY_LOGI("IccDiallingNumbersManager::UpdateIccDiallingNumbers OK return %{public}d", hasEventDone_);
    return hasEventDone_ ? TELEPHONY_SUCCESS : CORE_ERR_SIM_CARD_UPDATE_FAILED;
}

int32_t IccDiallingNumbersManager::DelIccDiallingNumbers(
    int type, const std::shared_ptr<DiallingNumbersInfo> &diallingNumber)
{
    std::unique_lock<std::mutex> lock(mtx_);
    if (diallingNumber == nullptr) {
        return TELEPHONY_ERR_LOCAL_PTR_NULL;
    }
    if (!HasSimCard()) {
        return TELEPHONY_ERR_NO_SIM_CARD;
    }
    if (!IsValidType(type) || !IsValidParam(type, diallingNumber)) {
        return TELEPHONY_ERR_ARGUMENT_INVALID;
    }
    int index = diallingNumber->GetIndex();
    TELEPHONY_LOGI("DelIccDiallingNumbers start: %{public}d %{public}d", type, index);
    int fileId = GetFileIdForType(type);
    AppExecFwk::InnerEvent::Pointer response = BuildCallerInfo(MSG_SIM_DIALLING_NUMBERS_DELETE_DONE);
    hasEventDone_ = false;
    diallingNumbersCache_->UpdateDiallingNumberToIcc(fileId, diallingNumber, index, true, response);
    while (!hasEventDone_) {
        TELEPHONY_LOGI("DelIccDiallingNumbers::wait(), response = false");
        if (processWait_.wait_for(lock, std::chrono::seconds(WAIT_TIME_SECOND)) == std::cv_status::timeout) {
            break;
        }
    }
    TELEPHONY_LOGI("IccDiallingNumbersManager::DelIccDiallingNumbers OK return %{public}d", hasEventDone_);
    return hasEventDone_ ? TELEPHONY_SUCCESS : CORE_ERR_SIM_CARD_UPDATE_FAILED;
}

int32_t IccDiallingNumbersManager::AddIccDiallingNumbers(
    int type, const std::shared_ptr<DiallingNumbersInfo> &diallingNumber)
{
    std::unique_lock<std::mutex> lock(mtx_);
    TELEPHONY_LOGI("AddIccDiallingNumbers start:%{public}d", type);
    if (diallingNumber == nullptr) {
        return TELEPHONY_ERR_LOCAL_PTR_NULL;
    }
    if (!HasSimCard()) {
        return TELEPHONY_ERR_NO_SIM_CARD;
    }
    if (!IsValidType(type) || !IsValidParam(type, diallingNumber)) {
        return TELEPHONY_ERR_ARGUMENT_INVALID;
    }
    AppExecFwk::InnerEvent::Pointer response = BuildCallerInfo(MSG_SIM_DIALLING_NUMBERS_WRITE_DONE);
    int fileId = GetFileIdForType(type);
    hasEventDone_ = false;
    diallingNumbersCache_->UpdateDiallingNumberToIcc(fileId, diallingNumber, ADD_FLAG, false, response);
    while (!hasEventDone_) {
        TELEPHONY_LOGI("AddIccDiallingNumbers::wait(), response = false");
        if (processWait_.wait_for(lock, std::chrono::seconds(WAIT_TIME_SECOND)) == std::cv_status::timeout) {
            break;
        }
    }
    TELEPHONY_LOGI("IccDiallingNumbersManager::AddIccDiallingNumbers OK return %{public}d", hasEventDone_);
    return hasEventDone_ ? TELEPHONY_SUCCESS : CORE_ERR_SIM_CARD_UPDATE_FAILED;
}

int32_t IccDiallingNumbersManager::QueryIccDiallingNumbers(
    int type, std::vector<std::shared_ptr<DiallingNumbersInfo>> &result)
{
    std::unique_lock<std::mutex> queryLock(queryMtx_);
    std::unique_lock<std::mutex> lock(mtx_);
    if (diallingNumbersCache_ == nullptr) {
        return TELEPHONY_ERR_LOCAL_PTR_NULL;
    }
    if (!HasSimCard()) {
        return TELEPHONY_ERR_NO_SIM_CARD;
    }
    if (!IsValidType(type)) {
        return TELEPHONY_ERR_ARGUMENT_INVALID;
    }
    TELEPHONY_LOGI("QueryIccDiallingNumbers start:%{public}d", type);
    ClearRecords();
    auto simFM = simFileManager_.lock();
    if (simFM == nullptr) {
        TELEPHONY_LOGE("QueryIccDiallingNumbers no sim card");
        return TELEPHONY_ERR_LOCAL_PTR_NULL;
    }
    bool isCardUicc = simFM->IsUiccCard();
    int fileId = isCardUicc ? ELEMENTARY_FILE_PBR : GetFileIdForType(type);
    int extensionEf = diallingNumbersCache_->ExtendedElementFile(fileId);
    AppExecFwk::InnerEvent::Pointer event = BuildCallerInfo(MSG_SIM_DIALLING_NUMBERS_GET_DONE);
    hasQueryEventDone_ = false;
    diallingNumbersCache_->ObtainAllDiallingNumberFiles(fileId, extensionEf, event);
    
    bool success = processWait_.wait_for(
        lock, std::chrono::seconds(WAIT_QUERY_TIME_SECOND), [this] { return hasQueryEventDone_ == true; });
    if (success) {
        TELEPHONY_LOGI("QueryIccDiallingNumbers wait success");
    } else {
        TELEPHONY_LOGI("QueryIccDiallingNumbers wait timeout");
    }
    if (!diallingNumbersList_.empty()) {
        result = diallingNumbersList_;
    }
    return success ? TELEPHONY_SUCCESS : CORE_ERR_SIM_CARD_LOAD_FAILED;
}

AppExecFwk::InnerEvent::Pointer IccDiallingNumbersManager::BuildCallerInfo(int eventId)
{
    std::unique_ptr<ResultObtain> object = std::make_unique<ResultObtain>();
    int eventParam = 0;
    AppExecFwk::InnerEvent::Pointer event = AppExecFwk::InnerEvent::Get(eventId, object, eventParam);
    if (event == nullptr) {
        TELEPHONY_LOGE("event is nullptr!");
        return AppExecFwk::InnerEvent::Pointer(nullptr, nullptr);
    }
    event->SetOwner(shared_from_this());
    return event;
}

void IccDiallingNumbersManager::ClearRecords()
{
    std::vector<std::shared_ptr<DiallingNumbersInfo>> nullVector;
    diallingNumbersList_.swap(nullVector);
}

int IccDiallingNumbersManager::GetFileIdForType(int fileType)
{
    int fileId = 0;
    if (fileType == DiallingNumbersInfo::SIM_ADN) {
        fileId = ELEMENTARY_FILE_ADN; //  ELEMENTARY_FILE_PBR  for usim
    } else if (fileType == DiallingNumbersInfo::SIM_FDN) {
        fileId = ELEMENTARY_FILE_FDN;
    }
    return fileId;
}

void IccDiallingNumbersManager::FillResults(
    const std::shared_ptr<std::vector<std::shared_ptr<DiallingNumbersInfo>>> &listInfo)
{
    TELEPHONY_LOGI("IccDiallingNumbersManager::FillResults  %{public}zu", listInfo->size());
    for (auto it = listInfo->begin(); it != listInfo->end(); it++) {
        std::shared_ptr<DiallingNumbersInfo> item = *it;
        if (!item->IsEmpty()) {
            diallingNumbersList_.push_back(item);
        }
    }
    TELEPHONY_LOGI("IccDiallingNumbersManager::FillResults %{public}zu", diallingNumbersList_.size());
}

bool IccDiallingNumbersManager::IsValidType(int type)
{
    switch (type) {
        case DiallingNumbersInfo::SIM_ADN:
        case DiallingNumbersInfo::SIM_FDN:
            return true;
        default:
            return false;
    }
}

std::shared_ptr<IccDiallingNumbersManager> IccDiallingNumbersManager::CreateInstance(
    std::weak_ptr<SimFileManager> simFile, std::shared_ptr<SimStateManager> simState)
{
    if (simFile.lock() == nullptr) {
        TELEPHONY_LOGE("IccDiallingNumbersManager::Init SimFileManager null pointer");
        return nullptr;
    }
    return std::make_shared<IccDiallingNumbersManager>(simFile, simState);
}

bool IccDiallingNumbersManager::HasSimCard()
{
    return (simStateManager_ != nullptr) ? simStateManager_->HasSimCard() : false;
}

bool IccDiallingNumbersManager::IsValidParam(int type, const std::shared_ptr<DiallingNumbersInfo> &info)
{
    if (type == DiallingNumbersInfo::SIM_FDN) {
        return !(info->pin2_.empty());
    } else {
        return true;
    }
}

IccDiallingNumbersManager::~IccDiallingNumbersManager() {}
} // namespace Telephony
} // namespace OHOS
