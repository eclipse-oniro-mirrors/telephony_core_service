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

#include "icc_file.h"

using namespace std;
using namespace OHOS::AppExecFwk;
using namespace OHOS::EventFwk;

namespace OHOS {
namespace Telephony {
IccFile::IccFile(
    const std::shared_ptr<AppExecFwk::EventRunner> &runner, std::shared_ptr<ISimStateManager> simStateManager)
    : AppExecFwk::EventHandler(runner), stateManager_(simStateManager)
{
    if (stateManager_ == nullptr) {
        TELEPHONY_LOGE("IccFile::IccFile set NULL SIMStateManager!!");
    }

    filesFetchedObser_ = std::make_unique<ObserverHandler>();
    if (filesFetchedObser_ == nullptr) {
        TELEPHONY_LOGE("IccFile::IccFile filesFetchedObser_ create nullptr.");
        return;
    }

    lockedFilesFetchedObser_ = std::make_unique<ObserverHandler>();
    if (lockedFilesFetchedObser_ == nullptr) {
        TELEPHONY_LOGE("IccFile::IccFile lockedFilesFetchedObser_ create nullptr.");
        return;
    }
    networkLockedFilesFetchedObser_ = std::make_unique<ObserverHandler>();
    if (networkLockedFilesFetchedObser_ == nullptr) {
        TELEPHONY_LOGE("IccFile::IccFile networkLockedFilesFetchedObser_ create nullptr.");
        return;
    }
    imsiReadyObser_ = std::make_unique<ObserverHandler>();
    if (imsiReadyObser_ == nullptr) {
        TELEPHONY_LOGE("IccFile::IccFile imsiReadyObser_ create nullptr.");
        return;
    }
    recordsEventsObser_ = std::make_unique<ObserverHandler>();
    if (recordsEventsObser_ == nullptr) {
        TELEPHONY_LOGE("IccFile::IccFile recordsEventsObser_ create nullptr.");
        return;
    }
    networkSelectionModeAutomaticObser_ = std::make_unique<ObserverHandler>();
    if (networkSelectionModeAutomaticObser_ == nullptr) {
        TELEPHONY_LOGE("IccFile::IccFile networkSelectionModeAutomaticObser_ create nullptr.");
        return;
    }
    spnUpdatedObser_ = std::make_unique<ObserverHandler>();
    if (spnUpdatedObser_ == nullptr) {
        TELEPHONY_LOGE("IccFile::IccFile spnUpdatedObser_ create nullptr.");
        return;
    }
    recordsOverrideObser_ = std::make_unique<ObserverHandler>();
    if (recordsOverrideObser_ == nullptr) {
        TELEPHONY_LOGE("IccFile::IccFile recordsOverrideObser_ create nullptr.");
        return;
    }
    TELEPHONY_LOGD("simmgr IccFile::IccFile finish");
}

void IccFile::Init() {}

void IccFile::StartLoad()
{
    TELEPHONY_LOGD("simmgr IccFile::StarLoad() start");
}

std::string IccFile::ObtainIMSI()
{
    if (imsi_.empty()) {
        TELEPHONY_LOGI("IccFile::ObtainIMSI  is null:");
    }
    return imsi_;
}

void IccFile::UpdateImsi(std::string imsi)
{
    imsi_ = imsi;
}

std::string IccFile::ObtainFullIccId()
{
    return iccIdComplete_;
}

std::string IccFile::ObtainIccId()
{
    return iccId_;
}

std::string IccFile::ObtainGid1()
{
    return gid1_;
}

std::string IccFile::ObtainGid2()
{
    return gid2_;
}

std::string IccFile::ObtainMsisdnNumber()
{
    return msisdn_;
}

bool IccFile::LoadedOrNot()
{
    return loaded_;
}

void IccFile::UpdateLoaded(bool loaded)
{
    loaded_ = loaded;
}

std::string IccFile::ObtainSimOperator()
{
    return operatorNumeric_;
}

std::string IccFile::ObtainIsoCountryCode()
{
    return "";
}

int IccFile::ObtainCallForwardStatus()
{
    return ICC_CALL_FORWARD_TYPE_UNKNOWN;
}

void IccFile::UpdateMsisdnNumber(
    const std::string &alphaTag, const std::string &number, const AppExecFwk::InnerEvent::Pointer &onComplete)
{}

bool IccFile::ObtainFilesFetched()
{
    return (fileToGet_ == 0) && fileQueried_;
}

bool IccFile::LockQueriedOrNot()
{
    return (fileToGet_ == 0) && lockQueried_;
}

std::string IccFile::ObtainAdnInfo()
{
    return "";
}

std::string IccFile::ObtainNAI()
{
    return "";
}

std::string IccFile::ObtainHomeNameOfPnn()
{
    return pnnHomeName_;
}

std::string IccFile::ObtainMsisdnAlphaStatus()
{
    return msisdnTag_;
}

std::string IccFile::ObtainVoiceMailNumber()
{
    return voiceMailNum_;
}

std::string IccFile::ObtainSPN()
{
    return spn_;
}

std::string IccFile::ObtainVoiceMailInfo()
{
    return voiceMailTag_;
}

std::string IccFile::ObtainIccLanguage()
{
    return iccLanguage_;
}

std::shared_ptr<UsimFunctionHandle> IccFile::ObtainUsimFunctionHandle()
{
    return std::make_shared<UsimFunctionHandle>(nullptr, 0);
}

std::string IccFile::ObtainSpNameFromEfSpn()
{
    return "";
}

int IccFile::ObtainLengthOfMnc()
{
    return lengthOfMnc_;
}

void IccFile::ProcessEvent(const AppExecFwk::InnerEvent::Pointer &event)
{
    auto id = event->GetInnerEventId();
    bool result = false;
    TELEPHONY_LOGI("IccFile::ProcessEvent id %{public}d", id);
    switch (id) {
        case MSG_SIM_OBTAIN_ICC_FILE_DONE:
            result = ProcessIccFileObtained(event);
            ProcessFileLoaded(result);
            break;
        case MSG_ICC_REFRESH:
            ProcessIccRefresh(MSG_ID_DEFAULT);
            break;
        default:
            break;
    }
}

void IccFile::RegisterImsiLoaded(std::shared_ptr<AppExecFwk::EventHandler> eventHandler)
{
    int eventCode = ObserverHandler::RADIO_IMSI_LOADED_READY;
    imsiReadyObser_->RegObserver(eventCode, eventHandler);
    if (!ObtainIMSI().empty()) {
        imsiReadyObser_->NotifyObserver(ObserverHandler::RADIO_IMSI_LOADED_READY);
        PublishSimFileEvent(SIM_STATE_ACTION, ICC_STATE_IMSI, ObtainIMSI());
    }
}

void IccFile::UnregisterImsiLoaded(const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    imsiReadyObser_->Remove(ObserverHandler::RADIO_IMSI_LOADED_READY, handler);
}

void IccFile::RegisterAllFilesLoaded(std::shared_ptr<AppExecFwk::EventHandler> eventHandler)
{
    int eventCode = ObserverHandler::RADIO_SIM_RECORDS_LOADED;
    filesFetchedObser_->RegObserver(eventCode, eventHandler);
    TELEPHONY_LOGD("IccFile::RegisterAllFilesLoaded: registerd");
    if (ObtainFilesFetched()) {
        TELEPHONY_LOGD("IccFile::RegisterAllFilesLoaded: notify");
        filesFetchedObser_->NotifyObserver(ObserverHandler::RADIO_SIM_RECORDS_LOADED);
        PublishSimFileEvent(SIM_STATE_ACTION, ICC_STATE_LOADED, "");
    }
}

void IccFile::UnregisterAllFilesLoaded(const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    filesFetchedObser_->Remove(ObserverHandler::RADIO_SIM_RECORDS_LOADED, handler);
}

void IccFile::UpdateSPN(const std::string &spn)
{
    if (spn_ != spn) {
        spnUpdatedObser_->NotifyObserver(MSG_SIM_SPN_UPDATED);
        spn_ = spn;
    }
}

AppExecFwk::InnerEvent::Pointer IccFile::CreatePointer(int eventId)
{
    std::unique_ptr<FileToControllerMsg> object = std::make_unique<FileToControllerMsg>();
    int eventParam = 0;
    AppExecFwk::InnerEvent::Pointer event = AppExecFwk::InnerEvent::Get(eventId, object, eventParam);
    event->SetOwner(shared_from_this());
    return event;
}

AppExecFwk::InnerEvent::Pointer IccFile::CreatePointer(int eventId, int arg1, int arg2)
{
    std::unique_ptr<FileToControllerMsg> object = std::make_unique<FileToControllerMsg>();
    object->arg1 = arg1;
    object->arg2 = arg2;
    int eventParam = 0;
    AppExecFwk::InnerEvent::Pointer event = AppExecFwk::InnerEvent::Get(eventId, object, eventParam);
    event->SetOwner(shared_from_this());
    return event;
}

AppExecFwk::InnerEvent::Pointer IccFile::CreatePointer(int eventId, std::shared_ptr<void> loader)
{
    std::unique_ptr<FileToControllerMsg> object = std::make_unique<FileToControllerMsg>();
    object->iccLoader = loader;
    int eventParam = 0;
    AppExecFwk::InnerEvent::Pointer event = AppExecFwk::InnerEvent::Get(eventId, object, eventParam);
    event->SetOwner(shared_from_this());
    return event;
}

bool IccFile::PublishSimFileEvent(const std::string &event, int eventCode, const std::string &eventData)
{
    Want want;
    want.SetAction(event);
    CommonEventData data;
    data.SetWant(want);
    data.SetCode(eventCode);
    data.SetData(eventData);
    CommonEventPublishInfo publishInfo;
    publishInfo.SetOrdered(true);
    bool publishResult = CommonEventManager::PublishCommonEvent(data, publishInfo, nullptr);
    TELEPHONY_LOGI("IccFile::PublishSimEvent result : %{public}d", publishResult);
    return publishResult;
}

bool IccFile::ProcessIccFileObtained(const AppExecFwk::InnerEvent::Pointer &event)
{
    bool isFileProcessResponse = true;
    std::shared_ptr<ControllerToFileMsg> fd = event->GetSharedObject<ControllerToFileMsg>();
    std::shared_ptr<void> baseLoad = fd->iccLoader;
    if (baseLoad != nullptr) {
        std::shared_ptr<IccFileLoaded> destLoad = std::static_pointer_cast<IccFileLoaded>(baseLoad);
        destLoad->ProcessParseFile(event);
        TELEPHONY_LOGD("ProcessIccFileObtained item %{public}s", destLoad->ObtainElementaryFileName().c_str());
    } else {
        isFileProcessResponse = false;
        TELEPHONY_LOGE("IccFile::ProcessIccFileObtained null base ponter");
    }
    return isFileProcessResponse;
}

void IccFile::UpdateIccLanguage(const std::string &langLi, const std::string &langPl)
{
    iccLanguage_ = ObtainValidLanguage(langLi);
    if (iccLanguage_.empty()) {
        iccLanguage_ = ObtainValidLanguage(langPl);
    }
    TELEPHONY_LOGI("IccFile::UpdateIccLanguage end is %{public}s", iccLanguage_.c_str());
}

std::string IccFile::ObtainValidLanguage(const std::string &langData)
{
    if (langData.empty()) {
        TELEPHONY_LOGE("langData null data!!");
        return "";
    }
    int langDataLen = 0;
    std::shared_ptr<unsigned char> ucc = SIMUtils::HexStringConvertToBytes(langData, langDataLen);
    unsigned char *data = ucc.get();

    std::string spnName((char *)data);
    TELEPHONY_LOGI("ObtainValidLanguage all is %{public}s---%{public}d", spnName.c_str(), langDataLen);
    std::string result = "";
    for (int i = 0; (i + 1) < langDataLen; i += DATA_STEP) {
        std::string langName((char *)data, i, DATA_STEP);
        TELEPHONY_LOGI("ObtainValidLanguage item is %{public}d--%{public}s", i, langName.c_str());
        if (!langName.empty()) {
            result = langName;
        }
    }

    return result;
}

IccFile::~IccFile() {}

void IccFile::SetRilAndFileController(
    std::shared_ptr<Telephony::IRilManager> ril, std::shared_ptr<IccFileController> file)
{
    rilManager_ = ril;
    if (rilManager_ == nullptr) {
        TELEPHONY_LOGE("IccFile set NULL rilManager!!");
    }

    fileController_ = file;
    if (fileController_ == nullptr) {
        TELEPHONY_LOGE("IccFile set NULL File Controller!!");
    }
}
} // namespace Telephony
} // namespace OHOS
