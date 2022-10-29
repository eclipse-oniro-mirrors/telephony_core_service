/*
 * Copyright (C) 2021-2022 Huawei Device Co., Ltd.
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

#include "sim_manager.h"

#include "telephony_errors.h"
#include "radio_event.h"

namespace OHOS {
namespace Telephony {
SimManager::SimManager(std::shared_ptr<ITelRilManager> telRilManager) : telRilManager_(telRilManager)
{
    TELEPHONY_LOGI("SimManager::SimManager()");
}

SimManager::~SimManager()
{
    if (multiSimMonitor_ != nullptr) {
        for (int32_t slotId = 0; slotId < slotCount_; slotId++) {
            multiSimMonitor_->UnRegisterForIccLoaded(slotId);
            multiSimMonitor_->UnRegisterForSimStateChanged(slotId);
        }
    }
}

bool SimManager::OnInit(int32_t slotCount)
{
    TELEPHONY_LOGI("SimManager OnInit, slotCount = %{public}d", slotCount);
    slotCount_ = slotCount;
    InitMultiSimObject();
    InitSingleSimObject();
    TELEPHONY_LOGI("SimManager OnInit success");
    return true;
}

void SimManager::InitMultiSimObject()
{
    // Program memory
    simStateManager_.resize(slotCount_);
    simFileManager_.resize(slotCount_);
    simSmsManager_.resize(slotCount_);
    simAccountManager_.resize(slotCount_);
    iccDiallingNumbersManager_.resize(slotCount_);
    stkManager_.resize(slotCount_);
    // Many card create
    for (int32_t slotId = 0; slotId < slotCount_; slotId++) {
        simStateManager_[slotId] = std::make_shared<SimStateManager>(telRilManager_);
        if (simStateManager_[slotId] != nullptr) {
            simStateManager_[slotId]->Init(slotId);
        }
        simFileManager_[slotId] = SimFileManager::CreateInstance(telRilManager_, simStateManager_[slotId]);
        if (simFileManager_[slotId] != nullptr) {
            simFileManager_[slotId]->Init(slotId);
        }
        simSmsManager_[slotId] = std::make_shared<SimSmsManager>(
            telRilManager_, simFileManager_[slotId], simStateManager_[slotId]);
        if (simSmsManager_[slotId] != nullptr) {
            simSmsManager_[slotId]->Init(slotId);
        }
        simAccountManager_[slotId] = std::make_shared<SimAccountManager>(
            telRilManager_, simStateManager_[slotId], simFileManager_[slotId]);
        if (simAccountManager_[slotId] != nullptr) {
            simAccountManager_[slotId]->Init(slotId);
        }
        iccDiallingNumbersManager_[slotId] =
            IccDiallingNumbersManager::CreateInstance(simFileManager_[slotId], simStateManager_[slotId]);
        if (iccDiallingNumbersManager_[slotId] != nullptr) {
            iccDiallingNumbersManager_[slotId]->Init();
        }
        stkManager_[slotId] = std::make_shared<StkManager>(telRilManager_, simStateManager_[slotId]);
        if (stkManager_[slotId] != nullptr) {
            stkManager_[slotId]->Init(slotId);
        }
        if (simStateManager_[DEFAULT_SIM_SLOT_ID] != nullptr && slotId == DEFAULT_SIM_SLOT_ID) {
            simStateManager_[DEFAULT_SIM_SLOT_ID]->RefreshSimState(DEFAULT_SIM_SLOT_ID);
        }
    }
}

void SimManager::InitSingleSimObject()
{
    controllerRunner_ = AppExecFwk::EventRunner::Create("MultiSimController");
    if (controllerRunner_.get() == nullptr) {
        TELEPHONY_LOGE("SimManager::InitSingleSimObject get controllerRunner_ failed");
        return;
    }
    multiSimController_ = std::make_shared<MultiSimController>(
        telRilManager_, simStateManager_, simFileManager_, controllerRunner_);
    if (multiSimController_ == nullptr) {
        TELEPHONY_LOGE("SimManager::InitSingleSimObject multiSimController init failed");
        return;
    }
    multiSimController_->Init();

    monitorRunner_ = AppExecFwk::EventRunner::Create("MultiSimMonitor");
    if (monitorRunner_.get() == nullptr) {
        TELEPHONY_LOGE("get monitorRunner_ failed");
        return;
    }
    multiSimMonitor_ = std::make_shared<MultiSimMonitor>(
        monitorRunner_, multiSimController_, simStateManager_, simFileManager_);
    if (multiSimMonitor_ == nullptr) {
        TELEPHONY_LOGE("SimAccountManager:: multiSimMonitor is null");
        return;
    }
    multiSimMonitor_->Init();
    for (int32_t slotId = 0; slotId < slotCount_; slotId++) {
        multiSimMonitor_->RegisterForIccLoaded(slotId);
        multiSimMonitor_->RegisterForSimStateChanged(slotId);
    }
}

void SimManager::SetNetworkSearchManager(std::shared_ptr<INetworkSearch> networkSearchManager)
{
    TELEPHONY_LOGI("SimManager::SetNetworkSearchManager");
    if (multiSimController_ == nullptr) {
        TELEPHONY_LOGE("SimManager::SetNetworkSearchManager multiSimController_ is nullptr");
        return;
    }
    multiSimController_->SetNetworkSearchManager(networkSearchManager);
    return;
}

bool SimManager::HasSimCard(int32_t slotId)
{
    if ((!IsValidSlotId(slotId)) || (simStateManager_[slotId] == nullptr)) {
        TELEPHONY_LOGE("simStateManager is null!");
        return false;
    }
    if (simStateManager_[slotId]->HasSimCard()) {
        TELEPHONY_LOGE("HasSimCard is true!");
        return true;
    }
    if ((!IsValidSlotId(slotId)) || (multiSimController_ == nullptr)) {
        TELEPHONY_LOGE("slotId is invalid or multiSimController_ is nullptr");
        return false;
    }
    return multiSimController_->IsSimActivatable(slotId);
}

int32_t SimManager::GetSimState(int32_t slotId)
{
    if ((!IsValidSlotId(slotId)) || (simStateManager_[slotId] == nullptr)) {
        TELEPHONY_LOGE("simStateManager is null!");
        return TELEPHONY_ERROR;
    }
    return static_cast<int32_t>(simStateManager_[slotId]->GetSimState());
}

int32_t SimManager::GetCardType(int32_t slotId)
{
    if ((!IsValidSlotId(slotId)) || (simStateManager_[slotId] == nullptr)) {
        TELEPHONY_LOGE("simStateManager is null!");
        return TELEPHONY_ERROR;
    }
    return static_cast<int32_t>(simStateManager_[slotId]->GetCardType());
}

bool SimManager::UnlockPin(int32_t slotId, const std::string &pin, LockStatusResponse &response)
{
    if ((!IsValidSlotId(slotId)) || (simStateManager_[slotId] == nullptr)) {
        TELEPHONY_LOGE("simStateManager is null!");
        return false;
    }
    return simStateManager_[slotId]->UnlockPin(slotId, pin, response);
}

bool SimManager::UnlockPuk(
    int32_t slotId, const std::string &newPin, const std::string &puk, LockStatusResponse &response)
{
    if ((!IsValidSlotId(slotId)) || (simStateManager_[slotId] == nullptr)) {
        TELEPHONY_LOGE("simStateManager is null!");
        return false;
    }
    return simStateManager_[slotId]->UnlockPuk(slotId, newPin, puk, response);
}

bool SimManager::AlterPin(
    int32_t slotId, const std::string &newPin, const std::string &oldPin, LockStatusResponse &response)
{
    if ((!IsValidSlotId(slotId)) || (simStateManager_[slotId] == nullptr)) {
        TELEPHONY_LOGE("simStateManager is null!");
        return false;
    }
    return simStateManager_[slotId]->AlterPin(slotId, newPin, oldPin, response);
}

bool SimManager::SetLockState(int32_t slotId, const LockInfo &options, LockStatusResponse &response)
{
    if ((!IsValidSlotId(slotId)) || (simStateManager_[slotId] == nullptr)) {
        TELEPHONY_LOGE("simStateManager is null!");
        return false;
    }
    return simStateManager_[slotId]->SetLockState(slotId, options, response);
}

int32_t SimManager::GetLockState(int32_t slotId, LockType lockType)
{
    if ((!IsValidSlotId(slotId)) || (simStateManager_[slotId] == nullptr)) {
        TELEPHONY_LOGE("simStateManager is null!");
        return TELEPHONY_ERROR;
    }
    return simStateManager_[slotId]->GetLockState(slotId, lockType);
}

int32_t SimManager::RefreshSimState(int32_t slotId)
{
    if ((!IsValidSlotId(slotId)) || (simStateManager_[slotId] == nullptr)) {
        TELEPHONY_LOGE("simStateManager is null!");
        return TELEPHONY_ERROR;
    }
    return simStateManager_[slotId]->RefreshSimState(slotId);
}

bool SimManager::UnlockPin2(int32_t slotId, const std::string &pin2, LockStatusResponse &response)
{
    if ((!IsValidSlotId(slotId)) || (simStateManager_[slotId] == nullptr)) {
        TELEPHONY_LOGE("simStateManager is null!");
        return false;
    }
    return simStateManager_[slotId]->UnlockPin2(slotId, pin2, response);
}

bool SimManager::UnlockPuk2(
    int32_t slotId, const std::string &newPin2, const std::string &puk2, LockStatusResponse &response)
{
    if ((!IsValidSlotId(slotId)) || (simStateManager_[slotId] == nullptr)) {
        TELEPHONY_LOGE("simStateManager is null!");
        return false;
    }
    return simStateManager_[slotId]->UnlockPuk2(slotId, newPin2, puk2, response);
}

bool SimManager::AlterPin2(
    int32_t slotId, const std::string &newPin2, const std::string &oldPin2, LockStatusResponse &response)
{
    if ((!IsValidSlotId(slotId)) || (simStateManager_[slotId] == nullptr)) {
        TELEPHONY_LOGE("simStateManager is null!");
        return false;
    }
    return simStateManager_[slotId]->AlterPin2(slotId, newPin2, oldPin2, response);
}

bool SimManager::UnlockSimLock(int32_t slotId, const PersoLockInfo &lockInfo, LockStatusResponse &response)
{
    if ((!IsValidSlotId(slotId)) || (simStateManager_[slotId] == nullptr)) {
        TELEPHONY_LOGE("simStateManager is null!");
        return false;
    }
    return simStateManager_[slotId]->UnlockSimLock(slotId, lockInfo, response);
}

bool SimManager::IsSimActive(int32_t slotId)
{
    if ((!IsValidSlotId(slotId)) || (multiSimController_ == nullptr)) {
        TELEPHONY_LOGE("slotId is invalid or multiSimController_ is nullptr");
        return false;
    }
    return multiSimController_->IsSimActive(slotId);
}

bool SimManager::SetActiveSim(int32_t slotId, int32_t enable)
{
    if ((!IsValidSlotId(slotId)) || (multiSimController_ == nullptr)) {
        TELEPHONY_LOGE("slotId is invalid or multiSimController_ is nullptr");
        return false;
    }
    return multiSimController_->SetActiveSim(slotId, enable);
}

bool SimManager::GetSimAccountInfo(int32_t slotId, IccAccountInfo &info)
{
    if ((!IsValidSlotId(slotId)) || (multiSimController_ == nullptr)) {
        TELEPHONY_LOGE("slotId is invalid or multiSimController_ is nullptr");
        return false;
    }
    return multiSimController_->GetSimAccountInfo(slotId, info);
}

bool SimManager::SetDefaultVoiceSlotId(int32_t slotId)
{
    if ((!IsValidSlotIdForDefault(slotId)) || (multiSimController_ == nullptr)) {
        TELEPHONY_LOGE("slotId is invalid for default or multiSimController_ is nullptr");
        return false;
    }
    if (slotId != DEFAULT_SIM_SLOT_ID_REMOVE && !IsSimActive(slotId)) {
        TELEPHONY_LOGE("SetDefaultVoiceSlotId slotId is not active!");
        return false;
    }
    return multiSimController_->SetDefaultVoiceSlotId(slotId);
}

bool SimManager::SetDefaultSmsSlotId(int32_t slotId)
{
    if ((!IsValidSlotIdForDefault(slotId)) || (multiSimController_ == nullptr)) {
        TELEPHONY_LOGE("slotId is invalid for default or multiSimController_ is nullptr");
        return false;
    }
    if (slotId != DEFAULT_SIM_SLOT_ID_REMOVE && !IsSimActive(slotId)) {
        TELEPHONY_LOGE("SetDefaultSmsSlotId slotId is not active!");
        return false;
    }
    return multiSimController_->SetDefaultSmsSlotId(slotId);
}

bool SimManager::SetDefaultCellularDataSlotId(int32_t slotId)
{
    if ((!IsValidSlotIdForDefault(slotId)) || (multiSimController_ == nullptr)) {
        TELEPHONY_LOGE("slotId is invalid for default or multiSimController_ is nullptr");
        return false;
    }
    if (slotId != DEFAULT_SIM_SLOT_ID_REMOVE && !IsSimActive(slotId)) {
        TELEPHONY_LOGE("SetDefaultSmsSlotId slotId is not active!");
        return false;
    }
    return multiSimController_->SetDefaultCellularDataSlotId(slotId);
}

bool SimManager::SetPrimarySlotId(int32_t slotId)
{
    if ((!IsValidSlotId(slotId)) || (multiSimController_ == nullptr)) {
        TELEPHONY_LOGE("slotId is invalid or multiSimController_ is nullptr");
        return false;
    }
    if (!IsSimActive(slotId)) {
        TELEPHONY_LOGE("SetPrimarySlotId slotId is not active!");
        return false;
    }
    return multiSimController_->SetPrimarySlotId(slotId);
}

bool SimManager::SetShowNumber(int32_t slotId, const std::u16string &number)
{
    if ((!IsValidSlotId(slotId)) || (multiSimController_ == nullptr)) {
        TELEPHONY_LOGE("slotId is invalid or multiSimController_ is nullptr");
        return false;
    }
    return multiSimController_->SetShowNumber(slotId, number);
}

bool SimManager::SetShowName(int32_t slotId, const std::u16string &name)
{
    if ((!IsValidSlotId(slotId)) || (multiSimController_ == nullptr)) {
        TELEPHONY_LOGE("slotId is invalid or multiSimController_ is nullptr");
        return false;
    }
    return multiSimController_->SetShowName(slotId, name);
}

int32_t SimManager::GetDefaultVoiceSlotId()
{
    if (slotCount_ == std::atoi(DEFAULT_SLOT_COUNT)) {
        TELEPHONY_LOGI("default slotId is 0 for single card version");
        return DEFAULT_SIM_SLOT_ID;
    }
    if (multiSimController_ == nullptr) {
        TELEPHONY_LOGE("multiSimController_ is nullptr");
        return TELEPHONY_ERROR;
    }
    return multiSimController_->GetDefaultVoiceSlotId();
}

int32_t SimManager::GetDefaultSmsSlotId()
{
    if (slotCount_ == std::atoi(DEFAULT_SLOT_COUNT)) {
        TELEPHONY_LOGI("default slotId is 0 for single card version");
        return DEFAULT_SIM_SLOT_ID;
    }
    if (multiSimController_ == nullptr) {
        TELEPHONY_LOGE("multiSimController_ is nullptr");
        return TELEPHONY_ERROR;
    }
    return multiSimController_->GetDefaultSmsSlotId();
}

int32_t SimManager::GetDefaultCellularDataSlotId()
{
    if (slotCount_ == std::atoi(DEFAULT_SLOT_COUNT)) {
        TELEPHONY_LOGI("default slotId is 0 for single card version");
        return DEFAULT_SIM_SLOT_ID;
    }
    if (multiSimController_ == nullptr) {
        TELEPHONY_LOGE("multiSimController_ is nullptr");
        return TELEPHONY_ERROR;
    }
    return multiSimController_->GetDefaultCellularDataSlotId();
}

int32_t SimManager::GetPrimarySlotId()
{
    if (slotCount_ == std::atoi(DEFAULT_SLOT_COUNT)) {
        TELEPHONY_LOGI(" default slotId is 0 for single card version");
        return DEFAULT_SIM_SLOT_ID;
    }
    if (multiSimController_ == nullptr) {
        TELEPHONY_LOGE("multiSimController_ is nullptr");
        return TELEPHONY_ERROR;
    }
    return multiSimController_->GetPrimarySlotId();
}

std::u16string SimManager::GetShowNumber(int32_t slotId)
{
    if ((!IsValidSlotId(slotId)) || (multiSimController_ == nullptr)) {
        TELEPHONY_LOGE("slotId is invalid or multiSimController_ is nullptr");
        return u"";
    }
    return multiSimController_->GetShowNumber(slotId);
}

std::u16string SimManager::GetShowName(int32_t slotId)
{
    if ((!IsValidSlotId(slotId)) || (multiSimController_ == nullptr)) {
        TELEPHONY_LOGE("slotId is invalid or multiSimController_ is nullptr");
        return u"";
    }
    return multiSimController_->GetShowName(slotId);
}

bool SimManager::GetActiveSimAccountInfoList(std::vector<IccAccountInfo> &iccAccountInfoList)
{
    if (multiSimController_ == nullptr) {
        TELEPHONY_LOGE("multiSimController_ is nullptr");
        return false;
    }
    return multiSimController_->GetActiveSimAccountInfoList(iccAccountInfoList);
}

int32_t SimManager::GetSlotId(int32_t simId)
{
    if (multiSimController_ == nullptr) {
        TELEPHONY_LOGE("multiSimController_ is nullptr");
        return TELEPHONY_ERROR;
    }
    return multiSimController_->GetSlotId(simId);
}

int32_t SimManager::GetSimId(int32_t slotId)
{
    IccAccountInfo accountInfo;
    if (GetSimAccountInfo(slotId, accountInfo)) {
        return accountInfo.simId;
    }
    TELEPHONY_LOGE("GetSimAccountInfo fail!");
    return TELEPHONY_ERROR;
}

bool SimManager::GetOperatorConfigs(int slotId, OperatorConfig &poc)
{
    if ((!IsValidSlotId(slotId)) || (simAccountManager_[slotId] == nullptr)) {
        TELEPHONY_LOGE("simAccountManager is null!");
        return false;
    }
    return simAccountManager_[slotId]->GetOperatorConfigs(slotId, poc);
}

bool SimManager::HasOperatorPrivileges(const int32_t slotId)
{
    TELEPHONY_LOGI("SimManager::HasOperatorPrivileges slotId:%{public}d", slotId);
    if ((!IsValidSlotId(slotId)) || (simAccountManager_[slotId] == nullptr)) {
        TELEPHONY_LOGE("simAccountManager_ can not be null!");
        return false;
    }
    return simAccountManager_[slotId]->HasOperatorPrivileges(slotId);
}

int32_t SimManager::SimAuthentication(
    int32_t slotId, const std::string &aid, const std::string &authData, SimAuthenticationResponse &response)
{
    TELEPHONY_LOGI("SimManager::SimAuthentication slotId:%{public}d", slotId);
    if ((!IsValidSlotId(slotId)) || (simStateManager_[slotId] == nullptr)) {
        TELEPHONY_LOGE("simStateManager_ can not be null!");
        return TELEPHONY_ERR_LOCAL_PTR_NULL;
    }
    return simStateManager_[slotId]->SimAuthentication(slotId, aid, authData, response);
}

int32_t SimManager::GetRadioProtocolTech(int32_t slotId)
{
    TELEPHONY_LOGI("SimManager::GetRadioProtocolTech slotId:%{public}d", slotId);
    if ((!IsValidSlotId(slotId)) || (multiSimController_ == nullptr)) {
        TELEPHONY_LOGE("slotId is invalid or multiSimController_ is nullptr");
        return static_cast<int32_t>(RadioProtocolTech::RADIO_PROTOCOL_TECH_UNKNOWN);
    }
    return multiSimController_->GetRadioProtocolTech(slotId);
}

void SimManager::GetRadioProtocol(int32_t slotId)
{
    TELEPHONY_LOGI("SimManager::GetRadioProtocol slotId:%{public}d", slotId);
    if ((!IsValidSlotId(slotId)) || (multiSimController_ == nullptr)) {
        TELEPHONY_LOGE("slotId is invalid or multiSimController_ is nullptr");
        return;
    }
    return multiSimController_->GetRadioProtocol(slotId);
}

bool SimManager::SendEnvelopeCmd(int32_t slotId, const std::string &cmd)
{
    if ((!IsValidSlotId(slotId)) || (stkManager_[slotId] == nullptr)) {
        TELEPHONY_LOGE("stkManager is null!");
        return false;
    }
    if (!HasSimCard(slotId)) {
        TELEPHONY_LOGE("SendEnvelopeCmd has no sim card!");
        return false;
    }
    return stkManager_[slotId]->SendEnvelopeCmd(slotId, cmd);
}

bool SimManager::SendTerminalResponseCmd(int32_t slotId, const std::string &cmd)
{
    if ((!IsValidSlotId(slotId)) || (stkManager_[slotId] == nullptr)) {
        TELEPHONY_LOGE("stkManager is null!");
        return false;
    }
    if (!HasSimCard(slotId)) {
        TELEPHONY_LOGE("SendTerminalResponseCmd has no sim card!");
        return false;
    }
    return stkManager_[slotId]->SendTerminalResponseCmd(slotId, cmd);
}

int32_t SimManager::SendCallSetupRequestResult(int32_t slotId, bool accept)
{
    if (!IsValidSlotId(slotId)) {
        TELEPHONY_LOGE("slotId is invalid!");
        return TELEPHONY_ERR_SLOTID_INVALID;
    }
    if (stkManager_[slotId] == nullptr) {
        TELEPHONY_LOGE("stkManager is null!");
        return TELEPHONY_ERR_LOCAL_PTR_NULL;
    }
    if (!HasSimCard(slotId)) {
        TELEPHONY_LOGE("SendCallSetupRequestResult has no sim card!");
        return TELEPHONY_ERR_NO_SIM_CARD;
    }
    return stkManager_[slotId]->SendCallSetupRequestResult(slotId, accept);
}

std::u16string SimManager::GetSimOperatorNumeric(int32_t slotId)
{
    if ((!IsValidSlotId(slotId)) || (simFileManager_[slotId] == nullptr)) {
        TELEPHONY_LOGE("simFileManager is null!");
        return u"";
    }
    return simFileManager_[slotId]->GetSimOperatorNumeric();
}

std::u16string SimManager::GetISOCountryCodeForSim(int32_t slotId)
{
    if ((!IsValidSlotId(slotId)) || (simFileManager_[slotId] == nullptr)) {
        TELEPHONY_LOGE("simFileManager is null!");
        return u"";
    }
    return simFileManager_[slotId]->GetISOCountryCodeForSim();
}

std::u16string SimManager::GetSimSpn(int32_t slotId)
{
    if ((!IsValidSlotId(slotId)) || (simFileManager_[slotId] == nullptr)) {
        TELEPHONY_LOGE("simFileManager is null");
        return std::u16string();
    }
    return simFileManager_[slotId]->GetSimSpn();
}

std::u16string SimManager::GetSimEons(int32_t slotId, const std::string &plmn, int32_t lac,
    bool longNameRequired)
{
    if ((!IsValidSlotId(slotId)) || (simFileManager_[slotId] == nullptr)) {
        TELEPHONY_LOGE("simFileManager is null");
        return std::u16string();
    }

    return simFileManager_[slotId]->GetSimEons(plmn, lac, longNameRequired);
}

std::u16string SimManager::GetSimIccId(int32_t slotId)
{
    if ((!IsValidSlotId(slotId)) || (simFileManager_[slotId] == nullptr)) {
        TELEPHONY_LOGE("simFileManager is null!");
        return u"";
    }
    return simFileManager_[slotId]->GetSimIccId();
}

std::u16string SimManager::GetIMSI(int32_t slotId)
{
    if ((!IsValidSlotId(slotId)) || (simFileManager_[slotId] == nullptr)) {
        TELEPHONY_LOGE("simFileManager is null!");
        return u"";
    }
    return simFileManager_[slotId]->GetIMSI();
}

std::u16string SimManager::GetLocaleFromDefaultSim(int32_t slotId)
{
    if ((!IsValidSlotId(slotId)) || (simFileManager_[slotId] == nullptr)) {
        TELEPHONY_LOGE("simFileManager is null!");
        return u"";
    }
    return simFileManager_[slotId]->GetLocaleFromDefaultSim();
}

std::u16string SimManager::GetSimGid1(int32_t slotId)
{
    if ((!IsValidSlotId(slotId)) || (simFileManager_[slotId] == nullptr)) {
        TELEPHONY_LOGE("simFileManager is null!");
        return u"";
    }
    return simFileManager_[slotId]->GetSimGid1();
}

std::u16string SimManager::GetSimGid2(int32_t slotId)
{
    if ((!IsValidSlotId(slotId)) || (simFileManager_[slotId] == nullptr)) {
        TELEPHONY_LOGE("simFileManager is null!");
        return u"";
    }
    return simFileManager_[slotId]->GetSimGid2();
}

int32_t SimManager::GetOpName(int32_t slotId, std::u16string &opname)
{
    if (!IsValidSlotId(slotId)) {
        TELEPHONY_LOGE("slotId is invalid! %{public}d", slotId);
        return TELEPHONY_ERR_SLOTID_INVALID;
    }
    if (simFileManager_[slotId] == nullptr) {
        TELEPHONY_LOGE("simFileManager is null! %{public}d", slotId);
        return TELEPHONY_ERR_LOCAL_PTR_NULL;
    }
    opname = simFileManager_[slotId]->GetOpName();
    return TELEPHONY_ERR_SUCCESS;
}

int32_t SimManager::GetOpKey(int32_t slotId, std::u16string &opkey)
{
    if (!IsValidSlotId(slotId)) {
        TELEPHONY_LOGE("slotId is invalid! %{public}d", slotId);
        return TELEPHONY_ERR_SLOTID_INVALID;
    }
    if (simFileManager_[slotId] == nullptr) {
        TELEPHONY_LOGE("simFileManager is null! %{public}d", slotId);
        return TELEPHONY_ERR_LOCAL_PTR_NULL;
    }
    opkey = simFileManager_[slotId]->GetOpKey();
    return TELEPHONY_ERR_SUCCESS;
}

int32_t SimManager::GetOpKeyExt(int32_t slotId, std::u16string &opkeyExt)
{
    if (!IsValidSlotId(slotId)) {
        TELEPHONY_LOGE("slotId is invalid! %{public}d", slotId);
        return TELEPHONY_ERR_SLOTID_INVALID;
    }
    if (simFileManager_[slotId] == nullptr) {
        TELEPHONY_LOGE("simFileManager is null! %{public}d", slotId);
        return TELEPHONY_ERR_LOCAL_PTR_NULL;
    }
    opkeyExt = simFileManager_[slotId]->GetOpKeyExt();
    return TELEPHONY_ERR_SUCCESS;
}

std::u16string SimManager::GetSimTelephoneNumber(int32_t slotId)
{
    if ((!IsValidSlotId(slotId)) || (simFileManager_[slotId] == nullptr)) {
        TELEPHONY_LOGE("simFileManager is null!");
        return u"";
    }
    return simFileManager_[slotId]->GetSimTelephoneNumber();
}

std::u16string SimManager::GetSimTeleNumberIdentifier(const int32_t slotId)
{
    if ((!IsValidSlotId(slotId)) || (simFileManager_[slotId] == nullptr)) {
        TELEPHONY_LOGE("simFileManager is null!");
        return u"";
    }
    return simFileManager_[slotId]->GetSimTeleNumberIdentifier();
}

std::u16string SimManager::GetVoiceMailIdentifier(int32_t slotId)
{
    if ((!IsValidSlotId(slotId)) || (simFileManager_[slotId] == nullptr)) {
        TELEPHONY_LOGE("simFileManager is null!");
        return u"";
    }
    return simFileManager_[slotId]->GetVoiceMailIdentifier();
}

std::u16string SimManager::GetVoiceMailNumber(int32_t slotId)
{
    if ((!IsValidSlotId(slotId)) || (simFileManager_[slotId] == nullptr)) {
        TELEPHONY_LOGE("simFileManager is null!");
        return u"";
    }
    return simFileManager_[slotId]->GetVoiceMailNumber();
}

int32_t SimManager::ObtainSpnCondition(int32_t slotId, bool roaming, std::string operatorNum)
{
    if ((!IsValidSlotId(slotId)) || (simFileManager_[slotId] == nullptr)) {
        TELEPHONY_LOGE("simFileManager is null");
        return TELEPHONY_ERROR;
    }
    return simFileManager_[slotId]->ObtainSpnCondition(roaming, operatorNum);
}

bool SimManager::SetVoiceMailInfo(int32_t slotId, const std::u16string &mailName, const std::u16string &mailNumber)
{
    if ((!IsValidSlotId(slotId)) || (simFileManager_[slotId] == nullptr)) {
        TELEPHONY_LOGE("simFileManager is null");
        return false;
    }
    return simFileManager_[slotId]->SetVoiceMailInfo(mailName, mailNumber);
}

bool SimManager::AddSmsToIcc(int32_t slotId, int status, std::string &pdu, std::string &smsc)
{
    if ((!IsValidSlotId(slotId)) || (simSmsManager_[slotId] == nullptr)) {
        TELEPHONY_LOGE("simSmsManager_ is null!");
        return false;
    }
    return simSmsManager_[slotId]->AddSmsToIcc(status, pdu, smsc);
}

bool SimManager::UpdateSmsIcc(int32_t slotId, int index, int status, std::string &pduData, std::string &smsc)
{
    if ((!IsValidSlotId(slotId)) || (simSmsManager_[slotId] == nullptr)) {
        TELEPHONY_LOGE("simSmsManager_ is null!");
        return false;
    }
    return simSmsManager_[slotId]->UpdateSmsIcc(index, status, pduData, smsc);
}

bool SimManager::DelSmsIcc(int32_t slotId, int index)
{
    if ((!IsValidSlotId(slotId)) || (simSmsManager_[slotId] == nullptr)) {
        TELEPHONY_LOGE("simSmsManager_ is null!");
        return false;
    }
    return simSmsManager_[slotId]->DelSmsIcc(index);
}

std::vector<std::string> SimManager::ObtainAllSmsOfIcc(int32_t slotId)
{
    std::vector<std::string> result;
    if ((!IsValidSlotId(slotId)) || (simSmsManager_[slotId] == nullptr)) {
        TELEPHONY_LOGE("simSmsManager_ is null!");
        return result;
    }
    return simSmsManager_[slotId]->ObtainAllSmsOfIcc();
}

std::vector<std::shared_ptr<DiallingNumbersInfo>> SimManager::QueryIccDiallingNumbers(int slotId, int type)
{
    std::vector<std::shared_ptr<DiallingNumbersInfo>> result;
    if ((!IsValidSlotId(slotId)) || (iccDiallingNumbersManager_[slotId] == nullptr)) {
        TELEPHONY_LOGE("iccDiallingNumbersManager is null!");
        return result;
    }
    return iccDiallingNumbersManager_[slotId]->QueryIccDiallingNumbers(type);
}

bool SimManager::AddIccDiallingNumbers(
    int slotId, int type, const std::shared_ptr<DiallingNumbersInfo> &diallingNumber)
{
    if ((!IsValidSlotId(slotId)) || (iccDiallingNumbersManager_[slotId] == nullptr)) {
        TELEPHONY_LOGE("iccDiallingNumbersManager is null!");
        return false;
    }
    return iccDiallingNumbersManager_[slotId]->AddIccDiallingNumbers(type, diallingNumber);
}

bool SimManager::DelIccDiallingNumbers(
    int slotId, int type, const std::shared_ptr<DiallingNumbersInfo> &diallingNumber)
{
    if ((!IsValidSlotId(slotId)) || (iccDiallingNumbersManager_[slotId] == nullptr)) {
        TELEPHONY_LOGE("iccDiallingNumbersManager is null!");
        return false;
    }
    return iccDiallingNumbersManager_[slotId]->DelIccDiallingNumbers(type, diallingNumber);
}

bool SimManager::UpdateIccDiallingNumbers(
    int slotId, int type, const std::shared_ptr<DiallingNumbersInfo> &diallingNumber)
{
    if ((!IsValidSlotId(slotId)) || (iccDiallingNumbersManager_[slotId] == nullptr)) {
        TELEPHONY_LOGE("iccDiallingNumbersManager is null!");
        return false;
    }
    return iccDiallingNumbersManager_[slotId]->UpdateIccDiallingNumbers(type, diallingNumber);
}

void SimManager::RegisterCoreNotify(int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler, int what)
{
    if ((what >= RadioEvent::RADIO_IMSI_LOADED_READY) && (what <= RadioEvent::RADIO_SIM_RECORDS_LOADED)) {
        if ((!IsValidSlotId(slotId)) || (simFileManager_[slotId] == nullptr)) {
            TELEPHONY_LOGE("slotId is invalid or simFileManager_ is nullptr");
            return;
        }
        simFileManager_[slotId]->RegisterCoreNotify(handler, what);
    } else if ((what >= RadioEvent::RADIO_SIM_STATE_CHANGE) &&
        (what <= RadioEvent::RADIO_SIM_STATE_SIMLOCK)) {
        if ((!IsValidSlotId(slotId)) || (simStateManager_[slotId] == nullptr)) {
            TELEPHONY_LOGE("slotId is invalid or simStateManager_ is nullptr");
            return;
        }
        simStateManager_[slotId]->RegisterCoreNotify(handler, what);
    } else if (what == RadioEvent::RADIO_SIM_ACCOUNT_LOADED) {
        if ((!IsValidSlotId(slotId)) || (multiSimMonitor_ == nullptr)) {
            TELEPHONY_LOGE("slotId is invalid or multiSimMonitor_ is nullptr");
            return;
        }
        multiSimMonitor_->RegisterCoreNotify(handler, what);
    } else {
        TELEPHONY_LOGE("SimManager::RegisterCoreNotify faild");
    }
}

void SimManager::UnRegisterCoreNotify(
    int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &observerCallBack, int what)
{
    if (what >= RadioEvent::RADIO_IMSI_LOADED_READY && what <= RadioEvent::RADIO_SIM_RECORDS_LOADED) {
        if ((!IsValidSlotId(slotId)) || (simFileManager_[slotId] == nullptr)) {
            TELEPHONY_LOGE("simFileManager is null");
            return;
        }
        simFileManager_[slotId]->UnRegisterCoreNotify(observerCallBack, what);
    } else if (what >= RadioEvent::RADIO_SIM_STATE_CHANGE && what <= RadioEvent::RADIO_SIM_STATE_SIMLOCK) {
        if ((!IsValidSlotId(slotId)) || (simStateManager_[slotId] == nullptr)) {
            TELEPHONY_LOGE("simStateManager_ is null");
            return;
        }
        simStateManager_[slotId]->UnRegisterCoreNotify(observerCallBack, what);
    } else {
        TELEPHONY_LOGE("SimManager::UnRegisterCoreNotify faild");
    }
}

bool SimManager::IsValidSlotId(int32_t slotId)
{
    if ((slotId < SLOT_ID_ZERO) || (slotId >= slotCount_)) {
        TELEPHONY_LOGE("slotId is invalid, slotId = %{public}d", slotId);
        return false;
    }
    TELEPHONY_LOGI("slotId is valid, slotId = %{public}d", slotId);
    return true;
}

bool SimManager::IsValidSlotIdForDefault(int32_t slotId)
{
    if ((slotId < DEFAULT_SIM_SLOT_ID_REMOVE) || (slotId >= slotCount_)) {
        TELEPHONY_LOGE("slotId is invalid, slotId = %{public}d", slotId);
        return false;
    }
    TELEPHONY_LOGI("slotId is valid, slotId = %{public}d", slotId);
    return true;
}

std::u16string SimManager::GetSimIst(int32_t slotId)
{
    if ((!IsValidSlotId(slotId)) || (simFileManager_[slotId] == nullptr)) {
        TELEPHONY_LOGE("simFileManager is null!");
        return u"";
    }
    return simFileManager_[slotId]->GetSimIst();
}

int32_t SimManager::SaveImsSwitch(int32_t slotId, int32_t imsSwitchValue)
{
    if ((!IsValidSlotId(slotId)) || (multiSimController_ == nullptr)) {
        TELEPHONY_LOGE("slotId is invalid or multiSimController_ is nullptr");
        return TELEPHONY_ERR_ARGUMENT_INVALID;
    }
    return multiSimController_->SaveImsSwitch(slotId, imsSwitchValue);
}

int32_t SimManager::QueryImsSwitch(int32_t slotId, int32_t &imsSwitchValue)
{
    if ((!IsValidSlotId(slotId)) || (multiSimController_ == nullptr)) {
        TELEPHONY_LOGE("slotId is invalid or multiSimController_ is nullptr");
        return TELEPHONY_ERR_ARGUMENT_INVALID;
    }
    return multiSimController_->QueryImsSwitch(slotId, imsSwitchValue);
}
} // namespace Telephony
} // namespace OHOS
