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

#ifndef OHOS_MULTI_SIM_CONTROLLER_H
#define OHOS_MULTI_SIM_CONTROLLER_H

#include <list>

#include "if_system_ability_manager.h"
#include "radio_protocol_controller.h"
#include "sim_constant.h"
#include "sim_file_manager.h"
#include "sim_rdb_helper.h"
#include "sim_state_manager.h"
#include "telephony_errors.h"
#include "want.h"

namespace OHOS {
namespace Telephony {
class MultiSimController {
public:
    MultiSimController(std::shared_ptr<Telephony::ITelRilManager> telRilManager,
        std::vector<std::shared_ptr<Telephony::SimStateManager>> simStateManager,
        std::vector<std::shared_ptr<Telephony::SimFileManager>> simFileManager);
    virtual ~MultiSimController();

    void Init();
    bool InitData(int32_t slotId);
    int32_t GetDefaultVoiceSlotId();
    int32_t SetDefaultVoiceSlotId(int32_t slotId);
    int32_t GetDefaultSmsSlotId();
    int32_t SetDefaultSmsSlotId(int32_t slotId);
    int32_t GetSimAccountInfo(int32_t slotId, bool denied, IccAccountInfo &info);
    int32_t GetDefaultCellularDataSlotId();
    int32_t SetDefaultCellularDataSlotId(int32_t slotId);
    int32_t GetPrimarySlotId();
    int32_t SetPrimarySlotId(int32_t slotId);
    int32_t GetShowNumber(int32_t slotId, std::u16string &showNumber);
    int32_t SetShowNumber(int32_t slotId, std::u16string Number, bool force = false);
    int32_t GetShowName(int32_t slotId, std::u16string &showName);
    int32_t SetShowName(int32_t slotId, std::u16string name, bool force = false);
    int32_t GetSimTelephoneNumber(int32_t slotId, std::u16string &telephoneNumber);
    bool IsSimActive(int32_t slotId);
    int32_t SetActiveSim(int32_t slotId, int32_t enable, bool force = false);
    bool SetActiveSimToRil(int32_t slotId, int32_t type, int32_t enable);
    bool ForgetAllData();
    bool ForgetAllData(int32_t slotId);
    int32_t GetSlotId(int32_t simId);
    int32_t SaveImsSwitch(int32_t slotId, int32_t imsSwitchValue);
    int32_t QueryImsSwitch(int32_t slotId, int32_t &imsSwitchValue);
    bool GetListFromDataBase();
    int32_t GetActiveSimAccountInfoList(bool denied, std::vector<IccAccountInfo> &iccAccountInfoList);
    int32_t GetRadioProtocolTech(int32_t slotId);
    void GetRadioProtocol(int32_t slotId);
    bool InitShowName(int slotId);
    void ReCheckPrimary();

public:
    int32_t unInitModemSlotId_ = INVALID_VALUE;

private:
    bool IsValidData(int32_t slotId);
    int32_t GetFirstActivedSlotId();
    bool InitShowNumber(int slotId);
    bool InitActive(int slotId);
    bool InitIccId(int slotId);
    int32_t UpdateDataByIccId(int slotId, const std::string &newIccId);
    int32_t InsertData(int slotId, const std::string &newIccId);
    void SortCache();
    void SavePrimarySlotIdInfo(int32_t slotId);
    void SaveDefaultCellularDataSlotIdInfo(int32_t slotId);
    bool AnnouncePrimarySimIdChanged(int32_t simId);
    bool AnnounceDefaultVoiceSimIdChanged(int32_t simId);
    bool AnnounceDefaultSmsSimIdChanged(int32_t simId);
    bool AnnounceDefaultCellularDataSimIdChanged(int32_t simId);
    bool PublishSimFileEvent(const AAFwk::Want &want, int eventCode, const std::string &eventData);
    bool RefreshActiveIccAccountInfoList();
    std::string EncryptIccId(const std::string iccid);
    void CheckIfNeedSwitchMainSlotId();
    bool IsValidSlotId(int32_t slotId);
    bool InitPrimary();
    bool IsAllCardsReady();
    bool IsAllCardsLoaded();
    void SendMainCardBroadCast(int32_t slotId);
    void SendDefaultCellularDataBroadCast(int32_t slotId);
    void InitMainCardSlotId();
    void PublishSetPrimaryEvent(bool setDone);
    int32_t GetTargetDefaultSimId(int32_t slotId, int &simId);
    size_t GetLocalCacheSize();
    int32_t GetTargetSimId(int32_t slotId, int &simId);
    int32_t GetTargetIccId(int32_t slotId, std::string &iccId);
    bool IsAllModemInitDone();

private:
    int32_t maxCount_ = 0;
    int32_t primarySimId_ = 0;
    int32_t defaultSmsSimId_ = 0;
    int32_t defaultCellularSimId_ = 0;
    int32_t defaultVoiceSimId_ = 0;
    int32_t lastPrimarySlotId_ = 0;
    int32_t lastCellularDataSlotId_ = 0;
    std::vector<std::shared_ptr<Telephony::SimStateManager>> simStateManager_;
    std::vector<std::shared_ptr<Telephony::SimFileManager>> simFileManager_;
    std::unique_ptr<SimRdbHelper> simDbHelper_ = nullptr;
    IccAccountInfo iccAccountInfo_;
    std::vector<IccAccountInfo> iccAccountInfoList_;
    std::vector<SimRdbInfo> localCacheInfo_;
    std::mutex mutex_;
    std::shared_ptr<RadioProtocolController> radioProtocolController_ = nullptr;
};
} // namespace Telephony
} // namespace OHOS
#endif // OHOS_MULTI_SIM_CONTROLLER_H
