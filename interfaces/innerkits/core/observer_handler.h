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

#ifndef OBSERVER_HANDLER_H
#define OBSERVER_HANDLER_H

#include <unordered_map>
#include "event_handler.h"
#include "telephony_log_wrapper.h"

namespace OHOS {
namespace Telephony {
class ObserverHandler {
public:
    ObserverHandler();

    virtual ~ObserverHandler();

    void RegObserver(int what, const std::shared_ptr<AppExecFwk::EventHandler> handler);

    void RegUniqueObserver(int what, const std::shared_ptr<AppExecFwk::EventHandler> handler);

    void Remove(int what, const std::shared_ptr<AppExecFwk::EventHandler> handler);

    void RemoveAll();

    void NotifyObserver(int what);

    template<typename T>
    void NotifyObserver(int what, T *object)
    {
        auto iter = observerHandlerMap_.find(what);
        if (iter == observerHandlerMap_.end()) {
            TELEPHONY_LOGE("ObserverHandler NotifyObserver %{public}d not register", what);
            return;
        }
        std::shared_ptr<T> msg(object);

        for (auto handlers : iter->second) {
            handlers->SendEvent(what, msg);
        }
    }

    template<typename T>
    void NotifyObserver(int what, std::shared_ptr<T> object)
    {
        auto iter = observerHandlerMap_.find(what);
        if (iter == observerHandlerMap_.end()) {
            TELEPHONY_LOGE("ObserverHandler NotifyObserver %{public}d not register", what);
            return;
        }
        for (auto handlers : iter->second) {
            handlers->SendEvent(what, object);
        }
    }

    enum ObserverHandlerId {
        // modem
        RADIO_STATE_CHANGED = 0,
        RADIO_GET_STATUS,
        RADIO_SET_STATUS,
        RADIO_GET_VOICE_TECH,
        RADIO_VOICE_TECH_CHANGED,

        RADIO_POWER,
        RADIO_ON,
        RADIO_AVAIL,
        RADIO_OFF,
        RADIO_NOT_AVAIL,
        RADIO_CALL_STATE,
        RADIO_DATA_CALL_LIST_CHANGED,
        RADIO_IMS_NETWORK_STATE_CHANGED,
        RADIO_ICC_STATUS_CHANGED,
        RADIO_CONNECTED,
        RADIO_ICC_REFRESH,
        RADIO_PCODATA,
        RADIO_GSM_SMS,
        RADIO_CDMA_SMS,
        RADIO_CALL_CRING,
        RADIO_CALL_WAITING,
        RADIO_CALL_CONNECT,
        RADIO_CALL_END,
        RADIO_CALL_STATUS_INFO,
        RADIO_CALL_IMS_SERVICE_STATUS,
        RADIO_CALL_USSD_CUSD_NOTICE,
        RADIO_HOT_PLUG,
        RADIO_CALL_RINGBACK_VOICE,
        RADIO_CALL_SRVCC_STATUS,
        RADIO_CALL_EMERGENCY_NUMBER_REPORT,

        RADIO_SMS_ON_SIM,
        RADIO_SMS_STATUS,
        RADIO_RESTRICTED_STATE,
        RADIO_CELL_BROADCAST,
        RADIO_CDMA_CELL_BROADCAST,
        RADIO_RIL_SETUP_DATA_CALL,
        RADIO_RIL_IMS_REGISTRATION_STATE,
        RADIO_RIL_DEACTIVATE_DATA_CALL,

        // cellular call
        RADIO_DIAL = 101,
        RADIO_REJECT_CALL,
        RADIO_HANGUP_CONNECT,
        RADIO_ACCEPT_CALL,
        RADIO_CURRENT_CALLS,
        RADIO_HOLD_CALL,
        RADIO_ACTIVE_CALL,
        RADIO_SWAP_CALL,
        RADIO_JOIN_CALL,
        RADIO_SPLIT_CALL,
        RADIO_CALL_SUPPLEMENT,
        RADIO_GET_CALL_WAIT,
        RADIO_SET_CALL_WAIT,
        RADIO_GET_CALL_FORWARD,
        RADIO_SET_CALL_FORWARD,
        RADIO_GET_CALL_CLIP,
        RADIO_SET_CALL_CLIP,
        RADIO_GET_CALL_CLIR,
        RADIO_SET_CALL_CLIR,
        RADIO_GET_CALL_RESTRICTION,
        RADIO_SET_CALL_RESTRICTION,
        RADIO_SEND_DTMF,
        RADIO_START_DTMF,
        RADIO_STOP_DTMF,
        RADIO_GET_IMS_CALL_LIST,
        RADIO_SET_CALL_PREFERENCE_MODE,
        RADIO_GET_CALL_PREFERENCE_MODE,
        RADIO_SET_LTE_IMS_SWITCH_STATUS,
        RADIO_GET_LTE_IMS_SWITCH_STATUS,
        RADIO_SET_USSD_CUSD,
        RADIO_GET_USSD_CUSD,
        RADIO_SET_CMUT,
        RADIO_GET_CMUT,
        RADIO_GET_EMERGENCY_CALL_LIST,
        RADIO_GET_CALL_FAIL_REASON,

        // Ims sms
        RADIO_SEND_IMS_GSM_SMS = 201,
        RADIO_SEND_SMS,
        RADIO_SEND_CDMA_SMS,
        RADIO_STORAGE_SMS,
        RADIO_DELETE_SMS,
        RADIO_SEND_SMS_EXPECT_MORE,
        RADIO_ADD_CDMA_SMS,
        RADIO_DEL_CDMA_SMS,
        RADIO_UPDATE_CDMA_SMS,

        // Network Search
        RADIO_NETWORK_STATE = 301,
        RADIO_VOICE_REG_STATE,
        RADIO_DATA_REG_STATE,
        RADIO_OPERATOR,
        RADIO_GET_SIGNAL_STRENGTH,
        RADIO_SIGNAL_STRENGTH_UPDATE,
        RADIO_NETWORK_SEARCH_RESULT,
        RADIO_GET_NETWORK_SELECTION_MODE,
        RADIO_SET_NETWORK_SELECTION_MODE,
        RADIO_GET_NEIGHBORING_CELL_INFO,
        RADIO_GET_CURRENT_CELL_INFO,
        RADIO_NETWORK_TIME_UPDATE,
        RADIO_NETWORK_TIME_ZONE_UPDATE,
        RADIO_SET_PREFERRED_NETWORK_MODE,
        RADIO_GET_PREFERRED_NETWORK_MODE,
        RADIO_NITZ_UPDATE,
        RADIO_GET_IMS_REG_STATUS,
        RADIO_GET_IMEI,
        RADIO_SET_PS_ATTACH_STATUS,
        RADIO_GET_MEID,
        RADIO_GET_RADIO_CAPABILITY,
        RADIO_SET_RADIO_CAPABILITY,
        RADIO_CHANNEL_CONFIG_UPDATE,
        RADIO_SET_LOCATION_UPDATE,

        // module internal events
        RADIO_PS_CONNECTION_ATTACHED = 401,
        RADIO_PS_CONNECTION_DETACHED,
        RADIO_IMS_REG_STATUS_UPDATE,
        RADIO_NR_STATE_CHANGED,
        RADIO_NR_FREQUENCY_CHANGED,
        RADIO_PS_RAT_CHANGED,
        RADIO_PS_ROAMING_OPEN,
        RADIO_PS_ROAMING_CLOSE,
        RADIO_EMERGENCY_STATE_OPEN,
        RADIO_EMERGENCY_STATE_CLOSE,

        // SIM
        RADIO_SIM_GET_STATUS = 501,
        RADIO_SIM_IO,
        RADIO_SIM_GET_IMSI,
        RADIO_SIM_GET_ICCID,
        RADIO_SIM_GET_LOCK_STATUS,
        RADIO_SIM_SET_LOCK,
        RADIO_SIM_CHANGE_PASSWD,
        RADIO_SIM_ENTER_PIN,
        RADIO_SIM_UNLOCK_PIN,
        RADIO_SIM_PIN_INPUT_TIMES,
        RADIO_SIM_ENTER_PIN2,
        RADIO_SIM_UNLOCK_PIN2,
        RADIO_SIM_PIN2_INPUT_TIMES,
        RADIO_SIM_CARD_ENABLED,
        RADIO_SIM_STATE_CHANGE,
        RADIO_SIM_STATE_READY,
        RADIO_SIM_STATE_LOCKED,
        RADIO_SIM_STATE_SIMLOCK,
        RADIO_IMSI_LOADED_READY,
        RADIO_SIM_RECORDS_LOADED,

        // STK
        RADIO_STK_SESSION_END,
        RADIO_STK_PROACTIVE_COMMAND,
        RADIO_STK_ALPHA_NOTIFY,

        RADIO_UPDATE_SMS,
        RADIO_SET_SMS_CENTER_ADDRESS,
        RADIO_GET_SMS_CENTER_ADDRESS,
        RADIO_SET_CELL_BROADCAST,
        RADIO_SET_CDMA_CELL_BROADCAST,
        RADIO_GET_CELL_BROADCAST,
        RADIO_GET_CDMA_CELL_BROADCAST,
        RADIO_ACTIVE_CDMA_CELL_BROADCAST,

        // Card
        RADIO_CARD_TYPE_CHANGE,
    };

private:
    std::unordered_map<int32_t, std::list<std::shared_ptr<AppExecFwk::EventHandler>>> observerHandlerMap_;
};
} // namespace Telephony
} // namespace OHOS
#endif // OBSERVER_HANDLER_H
