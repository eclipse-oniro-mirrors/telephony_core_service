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

#ifndef OHOS_SIM_STATE_HANDLE_H
#define OHOS_SIM_STATE_HANDLE_H

#include <chrono>
#include <condition_variable>
#include <list>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include "i_sim_manager.h"
#include "i_tel_ril_manager.h"
#include "icc_state.h"
#include "observer_handler.h"
#include "satellite_core_callback.h"
#include "sim_state_type.h"
#include "tel_event_handler.h"
#include "telephony_errors.h"
#include "want.h"

namespace OHOS {
namespace Telephony {
class SimStateManager;

enum UnlockType {
    PIN_TYPE,
    PUK_TYPE,
};

enum UnlockCmd {
    REQUEST_UNLOCK_PIN,
    REQUEST_UNLOCK_PUK,
    REQUEST_UNLOCK_REMAIN,
};

// pin/puk password incorect
const int UNLOCK_PIN_PUK_INCORRECT = 16; // incorrect password
// Phone number
const int SIM_CARD_NUM = 2;
// The events for handleMessage
const int MSG_SIM_GET_ICC_STATUS_DONE = 3;
// Unlock pin
const int MSG_SIM_UNLOCK_PIN_DONE = 4;
// Unlock puk
const int MSG_SIM_UNLOCK_PUK_DONE = 5;
// Change pin
const int MSG_SIM_CHANGE_PIN_DONE = 6;
// Check pin state
const int MSG_SIM_CHECK_PIN_DONE = 7;
// Set pin state[0:close_lock_state], [1:open_lock_state]
const int MSG_SIM_ENABLE_PIN_DONE = 8;
// Get sim unlock pin remain
const int MSG_SIM_UNLOCK_REMAIN_DONE = 10;
// Get sim realtime icc state
const int MSG_SIM_GET_REALTIME_ICC_STATUS_DONE = 21;
// Unlock pin2
const int MSG_SIM_UNLOCK_PIN2_DONE = 31;
// Unlock puk2
const int MSG_SIM_UNLOCK_PUK2_DONE = 32;
// Change pin2
const int MSG_SIM_CHANGE_PIN2_DONE = 33;
// Get sim unlock pin2 remain
const int MSG_SIM_UNLOCK_PIN2_REMAIN_DONE = 34;
// Unlock simlock
const int MSG_SIM_UNLOCK_SIMLOCK_DONE = 51;

const int MSG_SIM_AUTHENTICATION_DONE = 61;

const int MSG_SIM_SEND_NCFG_OPER_INFO_DONE = 62;

// pin lock type
constexpr const char *FAC_PIN_LOCK = "SC";
// change pin2 type
constexpr const char *FDN_PIN_LOCK = "P2";
// FDN lock type
constexpr const char *FDN_PIN2_LOCK = "FD";

constexpr const char *USIM_AID = "USIM_AID";
constexpr const char *CDMA_FAKE_AID = "CDMA_FAKE_AID";
constexpr const char *GSM_FAKE_AID = "GSM_FAKE_AID";

struct UnlockData {
    int32_t result = 0;
    int32_t remain = 0;
    int32_t lockState = 0;
};

class SimStateHandle : public TelEventHandler {
public:
    using Func = void (SimStateHandle::*)(int32_t, const AppExecFwk::InnerEvent::Pointer &);
    explicit SimStateHandle(const std::weak_ptr<SimStateManager> &simStateManager);
    ~SimStateHandle() = default;
    void Init(int32_t slotId);
    void UnInit();
    SimState GetSimState();
    CardType GetCardType();
    bool HasSimCard();
    void ObtainRealtimeIccStatus(int32_t slotId);
    void UnlockPin(int32_t slotId, const std::string &pin);
    void UnlockPuk(int32_t slotId, const std::string &newPin, const std::string &puk);
    void AlterPin(int32_t slotId, const std::string &newPin, const std::string &oldPin);
    void SetLockState(int32_t slotId, const LockInfo &options);
    void GetLockState(int32_t slotId, LockType lockType);
    UnlockData GetUnlockData();
    LockStatusResponse GetSimlockResponse();
    bool ConnectService();
    void UnlockPin2(int32_t slotId, const std::string &pin2);
    void UnlockPuk2(int32_t slotId, const std::string &newPin2, const std::string &puk2);
    void AlterPin2(int32_t slotId, const std::string &newPin2, const std::string &oldPin2);
    void UnlockSimLock(int32_t slotId, const PersoLockInfo &lockInfo);
    void SetRilManager(std::weak_ptr<Telephony::ITelRilManager> telRilManager);
    bool IsIccReady();
    void RegisterCoreNotify(const std::shared_ptr<AppExecFwk::EventHandler> &handler, int what);
    void UnRegisterCoreNotify(const std::shared_ptr<AppExecFwk::EventHandler> &observerCallBack, int what);
    void RegisterSatelliteCallback();
    void UnregisterSatelliteCallback();
    int32_t SimAuthentication(int32_t slotId, AuthType authType, const std::string &authData);
    SimAuthenticationResponse GetSimAuthenticationResponse();
    void SendSimMatchedOperatorInfo(
        int32_t slotId, int32_t state, const std::string &operName, const std::string &operKey);
    int32_t GetSendSimMatchedOperatorInfoResponse();

public:
    bool modemInitDone_ = false;

private:
    void SyncCmdResponse();
    void ObtainIccStatus(int32_t slotId);
    void GetSimCardData(int32_t slotId, const AppExecFwk::InnerEvent::Pointer &event);
    void GetSimLockState(int32_t slotId, const AppExecFwk::InnerEvent::Pointer &event);
    void GetSetLockResult(int32_t slotId, const AppExecFwk::InnerEvent::Pointer &event);
    void GetUnlockResult(int32_t slotId, const AppExecFwk::InnerEvent::Pointer &event);
    void ProcessEvent(const AppExecFwk::InnerEvent::Pointer &event);
    void ProcessIccCardState(IccState &ar, int32_t slotId);
    void UpdateAppInfo(IccState &ar, int32_t slotId);
    bool PublishSimStateEvent(std::string event, int32_t eventCode, std::string eventData);
    void SimStateEscape(int32_t simState, int32_t slotId, LockReason &reason);
    void CardTypeEscape(int32_t simType, int32_t slotId);
    void SimLockStateEscape(int32_t simState, int32_t slotId, LockReason &reason);
    void NotifySimLock(int slotId);
    void GetUnlockSimLockResult(int32_t slotId, const AppExecFwk::InnerEvent::Pointer &event);
    void GetSimAuthenticationResult(int32_t slotId, const AppExecFwk::InnerEvent::Pointer &event);
    void GetSendSimMatchedOperatorInfoResult(int32_t slotId, const AppExecFwk::InnerEvent::Pointer &event);
    std::string GetAidByCardType(CardType type);
    bool IsRadioStateUnavailable(const AppExecFwk::InnerEvent::Pointer &event);
    int32_t IsSatelliteSupported();

private:
    static const std::map<uint32_t, Func> memberFuncMap_;
    int32_t oldSimType_ = ICC_UNKNOWN_TYPE;
    int32_t oldSimStatus_ = ICC_CONTENT_UNKNOWN;
    int32_t slotId_ = DEFAULT_SIM_SLOT_ID;
    UnlockData unlockRespon_ = { UNLOCK_FAIL, TELEPHONY_ERROR, static_cast<int32_t>(LockState::LOCK_ERROR) };
    SimAuthenticationResponse simAuthRespon_ = { 0 };
    int32_t sendSimMatchedOperatorInfoResult_ = static_cast<int32_t>(ErrType::NONE);
    LockStatusResponse simlockRespon_ = { UNLOCK_FAIL, TELEPHONY_ERROR };
    IccState iccState_; // icc card states
    SimState externalState_; // need to broadcast sim state;
    CardType externalType_ = CardType::UNKNOWN_CARD; // need to broadcast card type;
    std::weak_ptr<SimStateManager> simStateManager_;
    std::weak_ptr<Telephony::ITelRilManager> telRilManager_; // ril manager
    std::unique_ptr<ObserverHandler> observerHandler_ = nullptr;
    sptr<ISatelliteCoreCallback> satelliteCallback_ = nullptr;
};
} // namespace Telephony
} // namespace OHOS

#endif // OHOS_SIM_STATE_HANDLE_H
