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
#include "tel_ril_test.h"

#include <iostream>

#include "radio_event.h"
#include "securec.h"
#include "sim_constant.h"
#include "sim_data_type.h"
#include "sim_utils.h"
#include "tel_ril_manager.h"
#include "telephony_log_wrapper.h"

namespace OHOS {
namespace Telephony {
TelRilTest::TelRilTest()
{
    memberFuncMap_[DiffInterfaceId::TEST_CURRENT_CALLS] = &TelRilTest::OnRequestCallGetCurrentCallsStatusTest;
}

TelRilTest::~TelRilTest() {}

void TelRilTest::OnInitInterface()
{
    telRilManager_ = std::make_shared<TelRilManager>();
    telRilManager_->OnInit();

    /* --------------------------------- MODEL ----------------------------- */
    memberFuncMap_[DiffInterfaceId::TEST_GET_SIGNAL_STRENGTH] = &TelRilTest::OnRequestNetworkGetRssiTest;
    memberFuncMap_[DiffInterfaceId::TEST_SHUT_DOWN] = &TelRilTest::OnRequestShutDownTest;
    memberFuncMap_[DiffInterfaceId::TEST_SET_POWER_STATE] = &TelRilTest::OnRequestSetRadioStateTest;
    memberFuncMap_[DiffInterfaceId::TEST_GET_POWER_STATE] = &TelRilTest::OnRequestGetRadioStateTest;
    memberFuncMap_[DiffInterfaceId::TEST_GET_CELL_INFO_LIST] = &TelRilTest::OnRequestGetCellInfoListTest;
    memberFuncMap_[DiffInterfaceId::TEST_GET_CURRENT_CELL_INFO] = &TelRilTest::OnRequestGetCurrentCellInfoTest;

    /* --------------------------------- DATA ----------------------------- */
    memberFuncMap_[DiffInterfaceId::TEST_RILCM_SET_INIT_APN_INFO_TEST] = &TelRilTest::OnRequestSetInitApnInfoTest;
    memberFuncMap_[DiffInterfaceId::TEST_RILCM_SETUP_DATA_CALL_TEST] = &TelRilTest::OnRequestDataSetupDataCallTest;
    memberFuncMap_[DiffInterfaceId::TEST_RILCM_DEACTIVATE_DATA_CALL_TEST] =
        &TelRilTest::OnRequestDataDisableDataCallTest;
    memberFuncMap_[DiffInterfaceId::TEST_RILCM_GET_DATA_CALL_LIST_TEST] = &TelRilTest::OnRequestGetDataCallListTest;
    memberFuncMap_[DiffInterfaceId::TEST_RILCM_GET_LINK_BANDWIDTH_INFO] =
        &TelRilTest::OnRequestGetLinkBandwidthInfoTest;
    memberFuncMap_[DiffInterfaceId::TEST_RILCM_SET_LINK_BANDWIDTH_REPORTING_RULE] =
        &TelRilTest::OnRequestSetLinkBandwidthReportingRuleTest;
    memberFuncMap_[DiffInterfaceId::TEST_RILCM_SET_DATA_PERMITTED_TEST] = &TelRilTest::OnRequestSetDataPermittedTest;
    memberFuncMap_[DiffInterfaceId::TEST_RILCM_GET_LINK_CAPABILITY] = &TelRilTest::OnRequestGetLinkCapabilityTest;
    memberFuncMap_[DiffInterfaceId::TEST_RILCM_CLEAN_ALL_DATA_CONNECTIONS_TEST] =
        &TelRilTest::OnRequestCleanAllConnectionsTest;

    OnInitCall();

    OnInitSms();

    OnInitSim();

    OnInitNetwork();
}

void TelRilTest::OnInitCall()
{
    /* --------------------------------- CALL ----------------------------- */
    memberFuncMap_[DiffInterfaceId::TEST_CALL_DIAL] = &TelRilTest::OnRequestCallDialTest;
    memberFuncMap_[DiffInterfaceId::TEST_HANDUP_CONNECT] = &TelRilTest::OnRequestCallHangupTest;
    memberFuncMap_[DiffInterfaceId::TEST_ACCEPT_CALL] = &TelRilTest::OnRequestCallAnswerTest;
    memberFuncMap_[DiffInterfaceId::TEST_HOLD_CALL] = &TelRilTest::OnRequestCallHoldTest;
    memberFuncMap_[DiffInterfaceId::TEST_ACTIVE_CALL] = &TelRilTest::OnRequestCallActiveTest;
    memberFuncMap_[DiffInterfaceId::TEST_SWAP_CALL] = &TelRilTest::OnRequestCallSwapTest;
    memberFuncMap_[DiffInterfaceId::TEST_JOIN_CALL] = &TelRilTest::OnRequestCallJoinTest;
    memberFuncMap_[DiffInterfaceId::TEST_SPLIT_CALL] = &TelRilTest::OnRequestSeparateConferenceTest;
    memberFuncMap_[DiffInterfaceId::TEST_REJECT_CALL] = &TelRilTest::OnRequestRefusedCallTest;
    memberFuncMap_[DiffInterfaceId::TEST_GET_CALL_WAIT] = &TelRilTest::OnRequestGetCallWaitTest;
    memberFuncMap_[DiffInterfaceId::TEST_SET_CALL_WAIT] = &TelRilTest::OnRequestSetCallWaitTest;
    memberFuncMap_[DiffInterfaceId::TEST_GET_CALL_FORWARD] = &TelRilTest::OnRequestGetCallForwardTest;
    memberFuncMap_[DiffInterfaceId::TEST_SET_CALL_FORWARD] = &TelRilTest::OnRequestSetCallForwardTest;
    memberFuncMap_[DiffInterfaceId::TEST_GET_CALL_DEAL_CLIP] = &TelRilTest::OnRequestGetClipTest;
    memberFuncMap_[DiffInterfaceId::TEST_SET_CALL_CLIP] = &TelRilTest::OnRequestSetClipTest;
    memberFuncMap_[DiffInterfaceId::TEST_GET_CALL_DEAL_CLIR] = &TelRilTest::OnRequestGetClirTest;
    memberFuncMap_[DiffInterfaceId::TEST_SET_CALL_CLIR] = &TelRilTest::OnRequestSetClirTest;
    memberFuncMap_[DiffInterfaceId::TEST_GET_CALL_RESTRICTION] = &TelRilTest::OnRequestGetCallRestrictionTest;
    memberFuncMap_[DiffInterfaceId::TEST_SET_CALL_RESTRICTION] = &TelRilTest::OnRequestSetCallRestrictionTest;
    memberFuncMap_[DiffInterfaceId::TEST_SEND_DTMF] = &TelRilTest::OnRequestSendDtmfTest;
    memberFuncMap_[DiffInterfaceId::TEST_START_DTMF] = &TelRilTest::OnRequestStartDtmfTest;
    memberFuncMap_[DiffInterfaceId::TEST_STOP_DTMF] = &TelRilTest::OnRequestStopDtmfTest;
    memberFuncMap_[DiffInterfaceId::TEST_SET_CALL_PREFERENCE_MODE] = &TelRilTest::OnRequestSetCallPreferenceModeTest;
    memberFuncMap_[DiffInterfaceId::TEST_GET_CALL_PREFERENCE_MODE] = &TelRilTest::OnRequestGetCallPreferenceModeTest;
    memberFuncMap_[DiffInterfaceId::TEST_SET_USSD] = &TelRilTest::OnRequestSetUssdTest;
    memberFuncMap_[DiffInterfaceId::TEST_GET_USSD] = &TelRilTest::OnRequestGetUssdTest;
    memberFuncMap_[DiffInterfaceId::TEST_SET_MUTE] = &TelRilTest::OnRequestSetMuteTest;
    memberFuncMap_[DiffInterfaceId::TEST_GET_MUTE] = &TelRilTest::OnRequestGetMuteTest;
    memberFuncMap_[DiffInterfaceId::TEST_GET_XLEMA] = &TelRilTest::OnRequestGetEmergencyCallListTest;
    memberFuncMap_[DiffInterfaceId::TEST_GET_CALL_FAIL] = &TelRilTest::OnRequestGetCallFailReasonTest;
    memberFuncMap_[DiffInterfaceId::TEST_SET_VONR_SWITCH] = &TelRilTest::OnRequestSetVoNRSwitchTest;
}

void TelRilTest::OnInitSms()
{
    /* --------------------------------- SMS ----------------------------- */
    memberFuncMap_[DiffInterfaceId::TEST_SEND_SMS] = &TelRilTest::OnRequestSendRilCmSmsTest;
    memberFuncMap_[DiffInterfaceId::TEST_SEND_CDMA_SMS] = &TelRilTest::OnRequestSendRilCmCdmaSmsTest;
    memberFuncMap_[DiffInterfaceId::TEST_STORAGE_SMS] = &TelRilTest::OnRequestStorageRilCmSmsTest;
    memberFuncMap_[DiffInterfaceId::TEST_DELETE_SMS] = &TelRilTest::OnRequestDeleteRilCmSmsTest;
    memberFuncMap_[DiffInterfaceId::TEST_UPDATE_SMS] = &TelRilTest::OnRequestUpdateRilCmSmsTest;
    memberFuncMap_[DiffInterfaceId::TEST_SET_SMS_CENTER_ADDRESS] = &TelRilTest::OnRequestSetRilCmSmsCenterAddressTest;
    memberFuncMap_[DiffInterfaceId::TEST_GET_SMS_CENTER_ADDRESS] = &TelRilTest::OnRequestGetRilCmSmsCenterAddressTest;
    memberFuncMap_[DiffInterfaceId::TEST_SET_CB_CONFIG] = &TelRilTest::OnRequestSetRilCBConfigTest;
    memberFuncMap_[DiffInterfaceId::TEST_SET_CDMA_CB_CONFIG] = &TelRilTest::OnRequestSetRilCdmaCBConfigTest;
    memberFuncMap_[DiffInterfaceId::TEST_GET_CB_CONFIG] = &TelRilTest::OnRequestGetRilCBConfigTest;
    memberFuncMap_[DiffInterfaceId::TEST_GET_CDMA_CB_CONFIG] = &TelRilTest::OnRequestGetRilCdmaCBConfigTest;
    memberFuncMap_[DiffInterfaceId::TEST_SEND_SMS_EXPECT_MORE] = &TelRilTest::OnRequestSmsSendSmsExpectMoreTest;
    memberFuncMap_[DiffInterfaceId::TEST_ACKNOWLEDGE_RILCM_LAST_INCOMING_GSM_SMS_TEST] =
        &TelRilTest::OnRequestSmsAcknowledgeTest;
    memberFuncMap_[DiffInterfaceId::TEST_ADD_CDMA_SMS] = &TelRilTest::OnRequestAddRilCmCdmaSmsTest;
    memberFuncMap_[DiffInterfaceId::TEST_DEL_CDMA_SMS] = &TelRilTest::OnRequestDelRilCmCdmaSmsTest;
    memberFuncMap_[DiffInterfaceId::TEST_UPDATE_CDMA_SMS] = &TelRilTest::OnRequestUpdateRilCmCdmaSmsTest;
}

void TelRilTest::OnInitSim()
{
    /*-----------------------------------SIM----------------------------------*/
    memberFuncMap_[DiffInterfaceId::TEST_GET_SIM_CARD_STATUS] = &TelRilTest::OnRequestSimGetSimStatusTest;
    memberFuncMap_[DiffInterfaceId::TEST_SIM_IO] = &TelRilTest::OnRequestSimIccIoTest;
    memberFuncMap_[DiffInterfaceId::TEST_GET_SIM_SMS] = &TelRilTest::OnRequestSimSmsTest;
    memberFuncMap_[DiffInterfaceId::TEST_GET_IMSI] = &TelRilTest::OnRequestSimGetImsiTest;
    memberFuncMap_[DiffInterfaceId::TEST_GET_SIM_LOCK_STATUS] = &TelRilTest::OnRequestGetSimLockStatusTest;
    memberFuncMap_[DiffInterfaceId::TEST_SET_SIM_LOCK] = &TelRilTest::OnRequestSetSimLockTest;
    memberFuncMap_[DiffInterfaceId::TEST_GET_CHANGE_SIM_PASSWD] = &TelRilTest::OnRequestChangeSimPasswordTest;
    memberFuncMap_[DiffInterfaceId::TEST_ENTER_SIM_PIN] = &TelRilTest::OnRequestEnterSimPinTest;
    memberFuncMap_[DiffInterfaceId::TEST_UNLOCK_SIM_PIN] = &TelRilTest::OnRequestUnlockSimPinTest;
    memberFuncMap_[DiffInterfaceId::TEST_ENTER_SIM_PIN2] = &TelRilTest::OnRequestEnterSimPin2Test;
    memberFuncMap_[DiffInterfaceId::TEST_UNLOCK_SIM_PIN2] = &TelRilTest::OnRequestUnlockSimPin2Test;
    memberFuncMap_[DiffInterfaceId::TEST_ENABLE_SIM_CARD] = &TelRilTest::OnRequestSetActiveSimTest;
    memberFuncMap_[DiffInterfaceId::TEST_OPEN_LG_SIMIO] = &TelRilTest::OnRequestOpenLGSimIOTest;
    memberFuncMap_[DiffInterfaceId::TEST_TRANSMIT_APDU_LOGICAL_CHANNEL] =
        &TelRilTest::OnRequestTransmitApduLogicalChannelTest;
    memberFuncMap_[DiffInterfaceId::TEST_TRANSMIT_APDU_BASIC_CHANNEL] =
        &TelRilTest::OnRequestTransmitApduBasicChannelTest;
    memberFuncMap_[DiffInterfaceId::TEST_SIM_AUTH] = &TelRilTest::OnRequestSimAuthenticationTest;
    memberFuncMap_[DiffInterfaceId::TEST_SIM_SEND_NCFG_OPER_INFO] =
        &TelRilTest::OnRequestSendSimMatchedOperatorInfoTest;
    memberFuncMap_[DiffInterfaceId::TEST_CLOSE_LG_SIMIO] = &TelRilTest::OnRequestCloseLGSimIOTest;
    memberFuncMap_[DiffInterfaceId::TEST_STK_SEND_TERMINAL_RESPONSE] =
        &TelRilTest::OnRequestSendTerminalResponseCmdTest;
    memberFuncMap_[DiffInterfaceId::TEST_STK_SEND_ENVELOPE] = &TelRilTest::OnRequestSendEnvelopeCmdTest;
    memberFuncMap_[DiffInterfaceId::TEST_STK_SEND_CALL_SETUP_REQUEST_RESULT] =
        &TelRilTest::OnRequestSendCallSetupRequestResultTest;
    memberFuncMap_[DiffInterfaceId::TEST_GET_RADIO_PROTOCOL] = &TelRilTest::OnRequestGetRadioProtocolTest;
    memberFuncMap_[DiffInterfaceId::TEST_SET_RADIO_PROTOCOL] = &TelRilTest::OnRequestSetRadioProtocolTest;
}

void TelRilTest::OnInitNetwork()
{
    /* --------------------------------- NETWORK ----------------------------- */
    memberFuncMap_[DiffInterfaceId::TEST_OPERATOR] = &TelRilTest::OnRequestNetworkOperatorTest;
    memberFuncMap_[DiffInterfaceId::TEST_GET_RILCM_VOICE_REGISTRATION_STATE_TEST] =
        &TelRilTest::OnRequestNetworkVoiceRegistrationStateTest;
    memberFuncMap_[DiffInterfaceId::TEST_GET_RILCM_DATA_REGISTRATION_STATE_TEST] =
        &TelRilTest::OnRequestNetworkDataRegistrationStateTest;
    memberFuncMap_[DiffInterfaceId::TEST_GET_NETWORKS_TO_USE] = &TelRilTest::OnRequestGetNetworkSearchInformationTest;
    memberFuncMap_[DiffInterfaceId::TEST_GET_SELECTION_MOD_FOR_NETWORKS] =
        &TelRilTest::OnRequestGetNetworkSelectionModeTest;
    memberFuncMap_[DiffInterfaceId::TEST_SET_MODE_AUTOMATIC_NETWORKS] =
        &TelRilTest::OnRequestSetNetworkSelectionModeTest;
    memberFuncMap_[DiffInterfaceId::TEST_GET_PREFERRED_NETWORK_TYPE] = &TelRilTest::OnRequestGetPreferredNetworkTest;
    memberFuncMap_[DiffInterfaceId::TEST_SET_PREFERRED_NETWORK_TYPE] = &TelRilTest::OnRequestSetPreferredNetworkTest;
    memberFuncMap_[DiffInterfaceId::TEST_GET_IMEI] = &TelRilTest::OnRequestGetImeiTest;
    memberFuncMap_[DiffInterfaceId::TEST_GET_IMEISV] = &TelRilTest::OnRequestGetImeiSvTest;
    memberFuncMap_[DiffInterfaceId::TEST_GET_BASEBAND_VERSION] = &TelRilTest::OnRequestGetBasebandVersionTest;
    memberFuncMap_[DiffInterfaceId::TEST_GET_MEID] = &TelRilTest::OnRequestGetMeidTest;
    memberFuncMap_[DiffInterfaceId::TEST_GET_CS_REG_STATUS] = &TelRilTest::OnRequestGetCsRegStatusTest;
    memberFuncMap_[DiffInterfaceId::TEST_GET_PS_REG_STATUS] = &TelRilTest::OnRequestGetPsRegStatusTest;
    memberFuncMap_[DiffInterfaceId::TEST_GET_VOICE_RADIO_INFO] = &TelRilTest::OnRequestGetVoiceRadioTechnology;
    memberFuncMap_[DiffInterfaceId::TEST_GET_PHYSICAL_CHANNEL_CONFIG] = &TelRilTest::OnRequestGetPhysicalChannelConfig;
    memberFuncMap_[DiffInterfaceId::TEST_SET_LOCATE_UPDATES] = &TelRilTest::OnRequestSetLocateUpdatesTest;
    memberFuncMap_[DiffInterfaceId::TEST_SET_NOTIFICATION_FILTER] = &TelRilTest::OnRequestSetNotificationFilterTest;
    memberFuncMap_[DiffInterfaceId::TEST_SET_DEVICE_STATE] = &TelRilTest::OnRequestSetDeviceStateTest;
    memberFuncMap_[DiffInterfaceId::TEST_GET_RRC_CONNECTION_STATE] = &TelRilTest::OnRequestGetRrcConnectionStateTest;
    memberFuncMap_[DiffInterfaceId::TEST_GET_NR_OPTION_MODE] = &TelRilTest::OnRequestGetNrOptionModeTest;
    memberFuncMap_[DiffInterfaceId::TEST_SET_NR_OPTION_MODE] = &TelRilTest::OnRequestSetNrOptionModeTest;
    memberFuncMap_[DiffInterfaceId::TEST_GET_NR_SSBID_INFO] = &TelRilTest::OnRequestGetNrSsbIdTest;
}

void TelRilTest::OnInitForRegister(int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    TELEPHONY_LOGI("TelRilTest::OnInitForRegister -->");
    // Register all APIs
    telRilManager_->RegisterCoreNotify(slotId, handler, RadioEvent::RADIO_STATE_CHANGED, nullptr);
    telRilManager_->RegisterCoreNotify(slotId, handler, RadioEvent::RADIO_DSDS_MODE_CHANGED, nullptr);
    telRilManager_->RegisterCoreNotify(slotId, handler, RadioEvent::RADIO_LINK_CAPABILITY_CHANGED, nullptr);
    telRilManager_->RegisterCoreNotify(slotId, handler, RadioEvent::RADIO_IMS_NETWORK_STATE_CHANGED, nullptr);
    telRilManager_->RegisterCoreNotify(slotId, handler, RadioEvent::RADIO_ON, nullptr);
    telRilManager_->RegisterCoreNotify(slotId, handler, RadioEvent::RADIO_NOT_AVAIL, nullptr);
    telRilManager_->RegisterCoreNotify(slotId, handler, RadioEvent::RADIO_CALL_STATUS_INFO, nullptr);
    telRilManager_->RegisterCoreNotify(slotId, handler, RadioEvent::RADIO_NETWORK_STATE, nullptr);
    telRilManager_->RegisterCoreNotify(slotId, handler, RadioEvent::RADIO_DATA_CALL_LIST_CHANGED, nullptr);
    telRilManager_->RegisterCoreNotify(slotId, handler, RadioEvent::RADIO_ICC_STATUS_CHANGED, nullptr);
    telRilManager_->RegisterCoreNotify(slotId, handler, RadioEvent::RADIO_GSM_SMS, nullptr);
    telRilManager_->RegisterCoreNotify(slotId, handler, RadioEvent::RADIO_CDMA_SMS, nullptr);
    telRilManager_->RegisterCoreNotify(slotId, handler, RadioEvent::RADIO_SMS_ON_SIM, nullptr);
    telRilManager_->RegisterCoreNotify(slotId, handler, RadioEvent::RADIO_SMS_STATUS, nullptr);
    telRilManager_->RegisterCoreNotify(slotId, handler, RadioEvent::RADIO_SIGNAL_STRENGTH_UPDATE, nullptr);
    telRilManager_->RegisterCoreNotify(slotId, handler, RadioEvent::RADIO_ICC_REFRESH, nullptr);
    telRilManager_->RegisterCoreNotify(slotId, handler, RadioEvent::RADIO_CONNECTED, nullptr);
    telRilManager_->RegisterCoreNotify(slotId, handler, RadioEvent::RADIO_PCODATA, nullptr);
    telRilManager_->RegisterCoreNotify(slotId, handler, RadioEvent::RADIO_JOIN_CALL, nullptr);
    telRilManager_->RegisterCoreNotify(slotId, handler, RadioEvent::RADIO_SPLIT_CALL, nullptr);
    telRilManager_->RegisterCoreNotify(slotId, handler, RadioEvent::RADIO_GET_CALL_WAIT, nullptr);
    telRilManager_->RegisterCoreNotify(slotId, handler, RadioEvent::RADIO_SET_CALL_WAIT, nullptr);
    telRilManager_->RegisterCoreNotify(slotId, handler, RadioEvent::RADIO_GET_CALL_FORWARD, nullptr);
    telRilManager_->RegisterCoreNotify(slotId, handler, RadioEvent::RADIO_SET_CALL_FORWARD, nullptr);
    telRilManager_->RegisterCoreNotify(slotId, handler, RadioEvent::RADIO_GET_CALL_CLIP, nullptr);
    telRilManager_->RegisterCoreNotify(slotId, handler, RadioEvent::RADIO_SET_CALL_CLIP, nullptr);
    telRilManager_->RegisterCoreNotify(slotId, handler, RadioEvent::RADIO_GET_CALL_RESTRICTION, nullptr);
    telRilManager_->RegisterCoreNotify(slotId, handler, RadioEvent::RADIO_SET_CALL_RESTRICTION, nullptr);
    telRilManager_->RegisterCoreNotify(slotId, handler, RadioEvent::RADIO_SEND_DTMF, nullptr);
    telRilManager_->RegisterCoreNotify(slotId, handler, RadioEvent::RADIO_START_DTMF, nullptr);
    telRilManager_->RegisterCoreNotify(slotId, handler, RadioEvent::RADIO_STOP_DTMF, nullptr);
    telRilManager_->RegisterCoreNotify(slotId, handler, RadioEvent::RADIO_REJECT_CALL, nullptr);
    telRilManager_->RegisterCoreNotify(slotId, handler, RadioEvent::RADIO_SET_CALL_PREFERENCE_MODE, nullptr);
    telRilManager_->RegisterCoreNotify(slotId, handler, RadioEvent::RADIO_GET_CALL_PREFERENCE_MODE, nullptr);
    telRilManager_->RegisterCoreNotify(slotId, handler, RadioEvent::RADIO_SET_USSD, nullptr);
    telRilManager_->RegisterCoreNotify(slotId, handler, RadioEvent::RADIO_GET_USSD, nullptr);
}

void TelRilTest::OnProcessInput(int32_t slotId, int32_t what, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto itFunc = memberFuncMap_.find((DiffInterfaceId)what);
    if (itFunc != memberFuncMap_.end()) {
        auto memberFunc = itFunc->second;
        if (memberFunc != nullptr) {
            (this->*memberFunc)(slotId, handler);
        }
    }
}

void TelRilTest::OnRequestCallGetCurrentCallsStatusTest(
    int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_CURRENT_CALLS);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestCallGetCurrentCallsStatusTest -->");
        telRilManager_->GetCallList(slotId, event);
        TELEPHONY_LOGI("OnRequestCallGetCurrentCallsStatusTest finished");
    }
}
/************************************** SIM test func *******************************************/
void TelRilTest::OnRequestSimGetSimStatusTest(int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_SIM_GET_STATUS);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestSimGetSimStatusTest -->");
        telRilManager_->GetSimStatus(slotId, event);
        TELEPHONY_LOGI("TelRilTest::OnRequestSimGetSimStatusTest --> OnRequestSimGetSimStatusTest finished");
    }
}

void TelRilTest::OnRequestSimIccIoTest(int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_SIM_IO);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestSimIccIoTest -->");
        SimIoRequestInfo simIoRequestInfo;
        simIoRequestInfo.command = COMMAND;
        simIoRequestInfo.fileId = FILEID;
        simIoRequestInfo.p1 = 0;
        simIoRequestInfo.p2 = 0;
        simIoRequestInfo.p3 = P3;
        simIoRequestInfo.data = "";
        simIoRequestInfo.path = "3F007F105F3A";
        simIoRequestInfo.pin2 = "";
        telRilManager_->GetSimIO(slotId, simIoRequestInfo, event);
        TELEPHONY_LOGI("TelRilTest::OnRequestSimIccIoTest --> OnRequestSimIccIoTest finished");
    }
}

void TelRilTest::OnRequestSimSmsTest(int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    int32_t cardType = 0;
    auto event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_SIM_IO);
    if (event == nullptr) {
        TELEPHONY_LOGE("event is nullptr");
        return;
    }
    event->SetOwner(handler);
    std::cout << "input 0 : GSM or 1 : CDMA" << std::endl;
    std::cin >> cardType;
    TELEPHONY_LOGI("TelRilTest::cardType is %{public}d", cardType);
    std::string path = cardType == 0 ? "3F007F10" : "3F007F25";
    std::shared_ptr<IccControllerHolder> ctrlHolder = std::make_shared<IccControllerHolder>(FILEID_SMS, path);
    ctrlHolder->fileLoaded = std::move(const_cast<AppExecFwk::InnerEvent::Pointer &>(event));

    std::unique_ptr<IccToRilMsg> msg = std::make_unique<IccToRilMsg>(ctrlHolder);
    if (msg == nullptr) {
        TELEPHONY_LOGE("msg is nullptr");
        return;
    }
    int64_t param = 0;
    AppExecFwk::InnerEvent::Pointer process = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_SIM_IO, msg, param);
    if (process == nullptr) {
        TELEPHONY_LOGE("event is nullptr!");
        return;
    }
    process->SetOwner(handler);

    if (telRilManager_ != nullptr) {
        TELEPHONY_LOGI("TelRilTest::OnRequestSimSmsTest -->");
        SimIoRequestInfo simIoRequestInfo;
        simIoRequestInfo.command = COMMAND;
        simIoRequestInfo.fileId = FILEID_SMS;
        simIoRequestInfo.p1 = 0;
        simIoRequestInfo.p2 = 0;
        simIoRequestInfo.p3 = P3;
        simIoRequestInfo.data = "";
        simIoRequestInfo.path = path;
        simIoRequestInfo.pin2 = "";
        telRilManager_->GetSimIO(slotId, simIoRequestInfo, process);
        TELEPHONY_LOGI("TelRilTest::OnRequestSimSmsTest --> OnRequestSimSmsTest finished");
    }
}

void TelRilTest::OnRequestOpenLGSimIOTest(int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    std::cout << "TelRilTest::OnRequestOpenLGSimIOTest begin:" << std::endl;
    auto event = AppExecFwk::InnerEvent::Get(uint32_t(CustomMessageID::MSG_OPEN_LOGICAL_CHANNEL_DONE));
    if (event == nullptr) {
        std::cerr << "event is nullptr" << std::endl;
        return;
    }
    event->SetOwner(handler);
    if (telRilManager_ == nullptr) {
        std::cerr << "telRilManager is nullptr" << std::endl;
        return;
    }
    telRilManager_->SimOpenLogicalChannel(slotId, "apdu", 0, event);
    std::cout << "TelRilTest::OnRequestOpenLGSimIOTest end" << std::endl;
}

void TelRilTest::OnRequestTransmitApduLogicalChannelTest(int32_t slotId,
    const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    std::cout << "TelRilTest::OnRequestTransmitApduLogicalChannelTest begin:" << std::endl;
    auto event = AppExecFwk::InnerEvent::Get(uint32_t(CustomMessageID::MSG_TRANSMIT_LOGICAL_CHANNEL_DONE));
    if (event == nullptr) {
        std::cerr << "event is nullptr" << std::endl;
        return;
    }
    event->SetOwner(handler);
    if (telRilManager_ == nullptr) {
        std::cerr << "telRilManager is nullptr" << std::endl;
        return;
    }
    ApduSimIORequestInfo reqInfo;
    telRilManager_->SimTransmitApduLogicalChannel(slotId, reqInfo, event);
    std::cout << "TelRilTest::OnRequestTransmitApduLogicalChannelTest end" << std::endl;
}

void TelRilTest::OnRequestTransmitApduBasicChannelTest(int32_t slotId,
    const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    std::cout << "TelRilTest::OnRequestTransmitApduBasicChannelTest begin:" << std::endl;
    auto event = AppExecFwk::InnerEvent::Get(uint32_t(CustomMessageID::MSG_TRANSMIT_BASIC_CHANNEL_DONE));
    if (event == nullptr) {
        std::cerr << "event is nullptr" << std::endl;
        return;
    }
    event->SetOwner(handler);
    if (telRilManager_ == nullptr) {
        std::cerr << "telRilManager is nullptr" << std::endl;
        return;
    }
    ApduSimIORequestInfo reqInfo;
    telRilManager_->SimTransmitApduBasicChannel(slotId, reqInfo, event);
    std::cout << "TelRilTest::OnRequestTransmitApduBasicChannelTest end" << std::endl;
}

void TelRilTest::OnRequestSimAuthenticationTest(int32_t slotId,
    const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    std::cout << "TelRilTest::OnRequestSimAuthenticationTest begin:" << std::endl;
    auto event = AppExecFwk::InnerEvent::Get(uint32_t(CustomMessageID::MSG_SIM_AUTHENTICATION_DONE));
    if (event == nullptr) {
        std::cerr << "event is nullptr" << std::endl;
        return;
    }
    event->SetOwner(handler);
    if (telRilManager_ == nullptr) {
        std::cerr << "telRilManager is nullptr" << std::endl;
        return;
    }
    int32_t authType;
    std::string aid;
    std::string authData;

    std::cout << "please enter the authType:" << std::endl;
    std::cout << "SIM_AUTH_EAP_SIM_TYPE is 128" << std::endl;
    std::cout << "SIM_AUTH_EAP_AKA_TYPE is 129" << std::endl;
    std::cin >> authType;
    std::cout << "please enter the aid:" << std::endl;
    std::cout << "USIM_AID" << std::endl;
    std::cout << "CDMA_FAKE_AID" << std::endl;
    std::cout << "GSM_FAKE_AID" << std::endl;
    std::cin >> aid;
    std::cout << "please enter the authData base64 encoded:" << std::endl;
    std::cin >> authData;
    SimAuthenticationRequestInfo reqInfo;
    reqInfo.serial = authType;
    reqInfo.aid = aid;
    reqInfo.authData = authData;
    int ret = telRilManager_->SimAuthentication(slotId, reqInfo, event);
    TELEPHONY_LOGI("OnRequestSimAuthenticationTest ret = %{public}d", ret);
    std::cout << "TelRilTest::OnRequestSimAuthenticationTest end" << std::endl;
}

void TelRilTest::OnRequestSendSimMatchedOperatorInfoTest(int32_t slotId,
    const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    std::cout << "TelRilTest::OnRequestSendSimMatchedOperatorInfoTest begin:" << std::endl;
    auto event = AppExecFwk::InnerEvent::Get(uint32_t(CustomMessageID::MSG_SIM_SEND_NCFG_OPER_INFO_DONE));
    if (event == nullptr) {
        std::cerr << "event is nullptr" << std::endl;
        return;
    }
    event->SetOwner(handler);
    if (telRilManager_ == nullptr) {
        std::cerr << "telRilManager is nullptr" << std::endl;
        return;
    }

    NcfgOperatorInfo reqInfo = NcfgOperatorInfo();
    cout << "input operName:" << std::endl;
    cin >> reqInfo.operName;
    cout << "input operKey:" << std::endl;
    cin >> reqInfo.operKey;
    cout << "input state:" << std::endl;
    cin >> reqInfo.state;
    cout << "input reserve:" << std::endl;
    cin >> reqInfo.reserve;
    int32_t ret = telRilManager_->SendSimMatchedOperatorInfo(slotId, reqInfo, event);
    TELEPHONY_LOGI("OnRequestSendSimMatchedOperatorInfoTest ret = %{public}d", ret);
    std::cout << "TelRilTest::OnRequestSendSimMatchedOperatorInfoTest end" << std::endl;
}

void TelRilTest::OnRequestCloseLGSimIOTest(int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    std::cout << "TelRilTest::OnRequestCloseLGSimIOTest begin:" << std::endl;
    auto event = AppExecFwk::InnerEvent::Get(uint32_t(CustomMessageID::MSG_CLOSE_LOGICAL_CHANNEL_DONE));
    if (event == nullptr) {
        std::cerr << "event is nullptr" << std::endl;
        return;
    }
    event->SetOwner(handler);
    if (telRilManager_ == nullptr) {
        std::cerr << "telRilManager is nullptr" << std::endl;
        return;
    }
    telRilManager_->SimCloseLogicalChannel(slotId, 0, event);
    std::cout << "TelRilTest::OnRequestCloseLGSimIOTest end" << std::endl;
}

void TelRilTest::OnRequestSimGetImsiTest(int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_SIM_GET_IMSI);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestSimGetImsiTest -->");
        telRilManager_->GetImsi(slotId, event);
        TELEPHONY_LOGI("OnRequestSimGetImsiTest finished");
    }
}

void TelRilTest::OnRequestGetSimLockStatusTest(
    int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_SIM_GET_LOCK_STATUS);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        std::string fac;

        std::cout << "please enter the fac:";
        std::cin >> fac;

        TELEPHONY_LOGI("TelRilTest::%{public}s -->", __func__);
        telRilManager_->GetSimLockStatus(slotId, fac, event);
        TELEPHONY_LOGI("TelRilTest::%{public}s --> finished", __func__);
    }
}

void TelRilTest::OnRequestSetSimLockTest(int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_SIM_SET_LOCK);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        std::string fac;
        int32_t mode;
        std::string code;

        std::cout << "please enter the fac:";
        std::cin >> fac;
        std::cout << "please enter the mode:";
        std::cin >> mode;
        std::cout << "please enter the pwd:";
        std::cin >> code;

        TELEPHONY_LOGI("TelRilTest::%{public}s -->", __func__);
        SimLockParam simLock;
        simLock.fac = fac;
        simLock.mode = mode;
        simLock.passwd = code;
        telRilManager_->SetSimLock(slotId, simLock, event);
        TELEPHONY_LOGI("TelRilTest::%{public}s --> finished", __func__);
    }
}

void TelRilTest::OnRequestChangeSimPasswordTest(
    int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_SIM_CHANGE_PASSWD);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        std::string fac;
        std::string oldPassword;
        std::string newPassword;
        int32_t passwordLength = 4;

        std::cout << "please enter the fac:";
        std::cin >> fac;
        std::cout << "please enter the oldPassword:";
        std::cin >> oldPassword;
        std::cout << "please enter the newPassword:";
        std::cin >> newPassword;

        TELEPHONY_LOGI("TelRilTest::%{public}s -->", __func__);
        SimPasswordParam simPass;
        simPass.fac = fac;
        simPass.oldPassword = oldPassword;
        simPass.newPassword = newPassword;
        simPass.passwordLength = passwordLength;
        telRilManager_->ChangeSimPassword(slotId, simPass, event);
        TELEPHONY_LOGI("TelRilTest::%{public}s --> finished", __func__);
    }
}

void TelRilTest::OnRequestEnterSimPinTest(int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_SIM_ENTER_PIN);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);

        std::string pin;
        std::cout << "please enter the SIM PIN:";
        std::cin >> pin;

        TELEPHONY_LOGI("TelRilTest::%{public}s -->", __func__);
        telRilManager_->UnlockPin(slotId, pin, event);
        TELEPHONY_LOGI("TelRilTest::%{public}s --> finished", __func__);
    }
}

void TelRilTest::OnRequestUnlockSimPinTest(int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_SIM_UNLOCK_PIN);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        std::string puk;
        std::string pin;

        std::cout << "please enter the SIM PUK:";
        std::cin >> puk;
        std::cout << "please enter the SIM PIN:";
        std::cin >> pin;

        TELEPHONY_LOGI("TelRilTest::%{public}s -->", __func__);
        telRilManager_->UnlockPuk(slotId, puk, pin, event);
        TELEPHONY_LOGI("TelRilTest::%{public}s --> finished", __func__);
    }
}

void TelRilTest::OnRequestEnterSimPin2Test(int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_SIM_ENTER_PIN2);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);

        std::string pin2;
        std::cout << "please enter the SIM PIN2:";
        std::cin >> pin2;

        TELEPHONY_LOGI("TelRilTest::%{public}s -->", __func__);
        telRilManager_->UnlockPin2(slotId, pin2, event);
        TELEPHONY_LOGI("TelRilTest::%{public}s --> finished", __func__);
    }
}

void TelRilTest::OnRequestUnlockSimPin2Test(int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_SIM_UNLOCK_PIN2);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        std::string puk2;
        std::string pin2;

        std::cout << "please enter the SIM PUK:";
        std::cin >> puk2;
        std::cout << "please enter the SIM PIN:";
        std::cin >> pin2;

        TELEPHONY_LOGI("TelRilTest::%{public}s -->", __func__);
        telRilManager_->UnlockPuk2(slotId, puk2, pin2, event);
        TELEPHONY_LOGI("TelRilTest::%{public}s --> finished", __func__);
    }
}

void TelRilTest::OnRequestSetActiveSimTest(int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_SIM_CARD_ENABLED);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);

        int32_t index;
        int32_t enable;
        std::cout << "please enter the SIM index:";
        std::cin >> index;

        std::cout << "please enter the SIM enable:";
        std::cin >> enable;

        TELEPHONY_LOGI("TelRilTest::%{public}s -->", __func__);
        telRilManager_->SetActiveSim(slotId, index, enable, event);
        TELEPHONY_LOGI("TelRilTest::%{public}s --> finished", __func__);
    }
}

void TelRilTest::OnRequestGetRadioProtocolTest(
    int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(TYPESBITMAP);
    if (event == nullptr || telRilManager_ == nullptr) {
        TELEPHONY_LOGE("TelRilTest::OnRequestGetRadioProtocolTest telRilManager_ or event is nullptr");
        return;
    }
    event->SetOwner(handler);
    TELEPHONY_LOGI("TelRilTest::OnRequestGetRadioProtocolTest -->");
    telRilManager_->GetRadioProtocol(slotId, event);
    TELEPHONY_LOGI("TelRilTest::OnRequestGetRadioProtocolTest --> finished");
}

void TelRilTest::OnRequestSetRadioProtocolTest(
    int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(TYPESBITMAP);
    if (event == nullptr || telRilManager_ == nullptr) {
        TELEPHONY_LOGE("TelRilTest::OnRequestSetRadioProtocolTest telRilManager_ or event is nullptr");
        return;
    }
    event->SetOwner(handler);

    int32_t sessionId;
    int32_t phase;
    int32_t technology;
    int32_t modemId;
    int32_t status;
    std::cout << "please enter the Radio Protocol sessionId:";
    std::cin >> sessionId;
    std::cout << "please enter the Radio Protocol phase(0-4):";
    std::cin >> phase;
    std::cout << "please enter the Radio Protocol technology(0-12):";
    std::cin >> technology;
    std::cout << "please enter the Radio Protocol modemId:";
    std::cin >> modemId;
    std::cout << "please enter the Radio Protocol status(0-2):";
    std::cin >> status;

    RadioProtocol protocol;
    protocol.sessionId = sessionId;
    protocol.phase = static_cast<RadioProtocolPhase>(phase);
    protocol.technology = technology;
    protocol.modemId = modemId;
    protocol.status = static_cast<RadioProtocolStatus>(status);

    TELEPHONY_LOGI("TelRilTest::OnRequestSetRadioProtocolTest -->");
    telRilManager_->SetRadioProtocol(slotId, protocol, event);
    TELEPHONY_LOGI("TelRilTest::OnRequestSetRadioProtocolTest --> finished");
}

void TelRilTest::OnRequestSendTerminalResponseCmdTest(
    int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_STK_SEND_TERMINAL_RESPONSE);
    if (event == nullptr || telRilManager_ == nullptr) {
        TELEPHONY_LOGE("TelRilTest::OnRequestSendTerminalResponseCmdTest failed!!!!");
        return;
    }
    event->SetOwner(handler);
    std::string strCmd;
    std::cout << "please enter the command string:";
    std::cin >> strCmd;
    TELEPHONY_LOGI("TelRilTest::OnRequestSendTerminalResponseCmdTest -->");
    telRilManager_->SendTerminalResponseCmd(slotId, strCmd, event);
    TELEPHONY_LOGI("TelRilTest::OnRequestSendTerminalResponseCmdTest --> finished");
}

void TelRilTest::OnRequestSendEnvelopeCmdTest(int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_STK_SEND_ENVELOPE);
    if (event == nullptr || telRilManager_ == nullptr) {
        TELEPHONY_LOGE("TelRilTest::OnRequestSendEnvelopeCmdTest failed!!!!");
        return;
    }
    event->SetOwner(handler);
    std::string strCmd;
    std::cout << "please enter the command string:";
    std::cin >> strCmd;
    TELEPHONY_LOGI("TelRilTest::OnRequestSendEnvelopeCmdTest -->");
    telRilManager_->SendEnvelopeCmd(slotId, strCmd, event);
    TELEPHONY_LOGI("TelRilTest::OnRequestSendEnvelopeCmdTest --> finished");
}

void TelRilTest::OnRequestSendCallSetupRequestResultTest(
    int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_STK_SEND_CALL_SETUP_REQUEST_RESULT);
    if (event == nullptr || telRilManager_ == nullptr) {
        TELEPHONY_LOGE("TelRilTest::OnRequestSendCallSetupRequestResultTest failed!!!!");
        return;
    }
    event->SetOwner(handler);
    int32_t accept;
    std::cout << "please enter the call setup request result:";
    std::cin >> accept;
    TELEPHONY_LOGI("TelRilTest::OnRequestSendCallSetupRequestResultTest -->");
    telRilManager_->SendCallSetupRequestResult(slotId, accept, event);
    TELEPHONY_LOGI("TelRilTest::OnRequestSendCallSetupRequestResultTest --> finished");
}

/************************************** SIM test func *******************************************/

void TelRilTest::OnRequestNetworkGetRssiTest(int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_GET_SIGNAL_STRENGTH);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestNetworkGetRssiTest -->");
        telRilManager_->GetSignalStrength(slotId, event);
        TELEPHONY_LOGI("OnRequestNetworkGetRssiTest finished");
    }
}

void TelRilTest::OnRequestCallDialTest(int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_DIAL);
    if (event == nullptr || telRilManager_ == nullptr) {
        TELEPHONY_LOGI("TelRilTest::OnRequestCallDialTest failed!!!!");
        return;
    }

    std::string phoneNum;
    int32_t clirMode; /* Calling Line Identification Restriction . From TS 27.007 V3.4.0 (2000-03) */
    event->SetOwner(handler);
    std::cout << "please enter the phone number:";
    std::cin >> phoneNum;
    clirMode = 0; // use subscription default value
    TELEPHONY_LOGI("TelRilTest::OnRequestCallDialTest -->");
    telRilManager_->Dial(slotId, phoneNum, clirMode, event);
    TELEPHONY_LOGI("OnRequestCallDialTest finished");
}

void TelRilTest::OnRequestRefusedCallTest(int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_REJECT_CALL);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestRefusedCallTest -->");
        telRilManager_->Reject(slotId, event);
        TELEPHONY_LOGI("OnRequestRefusedCallTest finished");
    }
}

void TelRilTest::OnRequestGetCallWaitTest(int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_GET_CALL_WAIT);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestGetCallWaitTest -->");
        telRilManager_->GetCallWaiting(slotId, event);
        TELEPHONY_LOGI("OnRequestGetCallWaitTest finished");
    }
}

void TelRilTest::OnRequestSetCallWaitTest(int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_SET_CALL_WAIT);
    if (event == nullptr || telRilManager_ == nullptr) {
        return;
    }

    event->SetOwner(handler);
    int32_t operating;
    TELEPHONY_LOGI("TelRilTest::OnRequestSetCallWaitTest -->");
    std::cout << "Please input set value[0:disable 1:enable]: " << endl;
    std::cin >> operating;

    telRilManager_->SetCallWaiting(slotId, operating, event);
    TELEPHONY_LOGI("OnRequestSetCallWaitTest finished");
}

void TelRilTest::OnRequestCallHangupTest(int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_HANGUP_CONNECT);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestCallHangupTest -->");
        telRilManager_->Hangup(slotId, static_cast<int32_t>(event->GetInnerEventId()), event);
        TELEPHONY_LOGI("OnRequestCallHangupTest finished");
    }
}

void TelRilTest::OnRequestCallAnswerTest(int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_ACCEPT_CALL);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestCallAnswerTest -->");
        telRilManager_->Answer(slotId, event);
        TELEPHONY_LOGI("OnRequestCallAnswerTest finished");
    }
}

void TelRilTest::OnRequestCallHoldTest(int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_HOLD_CALL);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestCallHoldTest -->");
        telRilManager_->HoldCall(slotId, event);
        TELEPHONY_LOGI("OnRequestCallHoldTest finished");
    }
}

void TelRilTest::OnRequestCallActiveTest(int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_ACTIVE_CALL);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestCallActiveTest -->");
        telRilManager_->UnHoldCall(slotId, event);
        TELEPHONY_LOGI("OnRequestCallActiveTest finished");
    }
}

void TelRilTest::OnRequestCallSwapTest(int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_SWAP_CALL);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestCallSwapTest -->");
        telRilManager_->SwitchCall(slotId, event);
        TELEPHONY_LOGI("OnRequestCallSwapTest finished");
    }
}

void TelRilTest::OnRequestNetworkVoiceRegistrationStateTest(
    int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(TYPE);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestNetworkVoiceRegistrationStateTest -->");
        telRilManager_->GetCsRegStatus(slotId, event);
        TELEPHONY_LOGI("OnRequestNetworkVoiceRegistrationStateTest finished");
    }
}

void TelRilTest::OnRequestNetworkDataRegistrationStateTest(
    int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(MAXCONNSTIME);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestNetworkDataRegistrationStateTest -->");
        telRilManager_->GetPsRegStatus(slotId, event);
        TELEPHONY_LOGI("OnRequestNetworkDataRegistrationStateTest finished");
    }
}

void TelRilTest::OnRequestNetworkOperatorTest(int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_OPERATOR);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestNetworkOperatorTest -->");
        telRilManager_->GetOperatorInfo(slotId, event);
        TELEPHONY_LOGI("TelRilTest::OnRequestNetworkOperatorTest --> OnRequestNetworkOperatorTest finished");
    }
}

void TelRilTest::OnRequestSendRilCmSmsTest(int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_SEND_SMS);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestSendRilCmSmsTest -->");
        telRilManager_->SendGsmSms(slotId, "smscPdu", "pdu", event);
        TELEPHONY_LOGI("TelRilTest::OnRequestSendRilCmSmsTest --> OnRequestSendRilCmSmsTest finished");
    }
}

void TelRilTest::OnRequestSendRilCmCdmaSmsTest(
    int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_SEND_CDMA_SMS);
    CdmaSmsMessageInfo msg = {};
    msg.serviceId = 1;
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestSendRilCmCdmaSmsTest -->");
        telRilManager_->SendCdmaSms(slotId, "pdu", event);
        TELEPHONY_LOGI("TelRilTest::OnRequestSendRilCmCdmaSmsTest --> OnRequestSendRilCmCdmaSmsTest finished");
    }
}

void TelRilTest::OnRequestStorageRilCmSmsTest(int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_STORAGE_SMS);
    int32_t status;
    std::cout << "Please input status:" << std::endl;
    std::cin >> status;
    std::cout << "Please input smsc:" << std::endl;
    string smsc;
    std::cin >> smsc;
    std::cout << "Please input pdu:" << std::endl;
    string pdu;
    std::cin >> pdu;
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestStorageRilCmSmsTest -->");
        SimMessageParam simMsg;
        simMsg.status = status;
        simMsg.smscPdu = smsc;
        simMsg.pdu = pdu;
        telRilManager_->AddSimMessage(slotId, simMsg, event);
        TELEPHONY_LOGI("TelRilTest::OnRequestStorageRilCmSmsTest --> OnRequestStorageRilCmSmsTest finished");
    }
}

void TelRilTest::OnRequestDeleteRilCmSmsTest(int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_DELETE_SMS);
    int32_t gsmIndex;

    std::cout << "Please input gsmIndex: " << endl;
    std::cin >> gsmIndex;
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestDeleteRilCmSmsTest -->");
        telRilManager_->DelSimMessage(slotId, gsmIndex, event);
        TELEPHONY_LOGI("TelRilTest::OnRequestDeleteRilCmSmsTest --> OnRequestDeleteRilCmSmsTest finished");
    }
}

void TelRilTest::OnRequestUpdateRilCmSmsTest(int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_UPDATE_SMS);
    int32_t gsmIndex;
    std::string pdu;

    std::cout << "Please input gsmIndex: " << endl;
    std::cin >> gsmIndex;
    std::cout << "Please input pdu: " << endl;
    std::cin >> pdu;
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        SimMessageParam simMsg = {};
        simMsg.gsmIndex = gsmIndex;
        simMsg.status = 0;
        simMsg.smscPdu = "00";
        simMsg.pdu = pdu;
        TELEPHONY_LOGI("TelRilTest::OnRequestUpdateRilCmSmsTest -->");
        telRilManager_->UpdateSimMessage(slotId, simMsg, event);
        TELEPHONY_LOGI("TelRilTest::OnRequestUpdateRilCmSmsTest --> OnRequestUpdateRilCmSmsTest finished");
    }
}

void TelRilTest::OnRequestSetRilCmSmsCenterAddressTest(
    int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_SET_SMS_CENTER_ADDRESS);
    int32_t tosca;
    std::string address;

    std::cout << "Please input tosca: " << endl;
    std::cin >> tosca;
    std::cout << "Please input address: " << endl;
    std::cin >> address;
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestSetRilCmSmsCenterAddressTest -->");
        telRilManager_->SetSmscAddr(slotId, tosca, address, event);
        TELEPHONY_LOGI("OnRequestSetRilCmSmsCenterAddressTest finished");
    }
}

void TelRilTest::OnRequestGetRilCmSmsCenterAddressTest(
    int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_GET_SMS_CENTER_ADDRESS);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestGetRilCmSmsCenterAddressTest -->");
        telRilManager_->GetSmscAddr(slotId, event);
        TELEPHONY_LOGI("OnRequestGetRilCmSmsCenterAddressTest finished");
    }
}

void TelRilTest::OnRequestSetRilCBConfigTest(int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_SET_CELL_BROADCAST);
    int32_t mode;
    std::string idList;
    std::string dcsList;

    std::cout << "Please input mode: " << endl;
    std::cin >> mode;
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestSetRilCBConfigTest -->");
        CBConfigParam cBConfig = {};
        cBConfig.mode = mode;
        cBConfig.idList = "0,1,5,320-478,922";
        cBConfig.dcsList = "0-3,5";
        telRilManager_->SetCBConfig(slotId, cBConfig, event);
        TELEPHONY_LOGI("TelRilTest::OnRequestSetRilCBConfigTest --> OnRequestSetRilCBConfigTest finished");
    }
}

void TelRilTest::OnRequestSetRilCdmaCBConfigTest(
    int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_SET_CDMA_CELL_BROADCAST);
    CdmaCBConfigInfoList cdmaCBConfigInfoList = {};
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestSetRilCdmaCBConfigTest -->");
        telRilManager_->SetCdmaCBConfig(slotId, cdmaCBConfigInfoList, event);
        TELEPHONY_LOGI("TelRilTest::OnRequestSetRilCdmaCBConfigTest --> OnRequestSetRilCdmaCBConfigTest finished");
    }
}

void TelRilTest::OnRequestGetRilCBConfigTest(int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_GET_CELL_BROADCAST);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestGetRilCBConfigTest -->");
        telRilManager_->GetCBConfig(slotId, event);
        TELEPHONY_LOGI("TelRilTest::OnRequestGetRilCBConfigTest--> OnRequestGetRilCBConfigTest finished");
    }
}

void TelRilTest::OnRequestGetRilCdmaCBConfigTest(
    int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_GET_CDMA_CELL_BROADCAST);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestGetRilCdmaCBConfigTest -->");
        telRilManager_->GetCdmaCBConfig(slotId, event);
        TELEPHONY_LOGI("TelRilTest::OnRequestGetRilCdmaCBConfigTest--> OnRequestGetRilCdmaCBConfigTest finished");
    }
}

void TelRilTest::OnRequestSmsSendSmsExpectMoreTest(
    int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_SEND_SMS_EXPECT_MORE);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestSmsSendSmsExpectMoreTest -->");
        telRilManager_->SendSmsMoreMode(slotId, "smscPdu", "pdu", event);
        TELEPHONY_LOGI("OnRequestSmsSendSmsExpectMoreTest finished");
    }
}

void TelRilTest::OnRequestAddRilCmCdmaSmsTest(int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_ADD_CDMA_SMS);
    int32_t status;
    std::cout << "Please input status:" << std::endl;
    std::cin >> status;
    std::cout << "Please input pdu:" << std::endl;
    string pdu;
    std::cin >> pdu;
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestAddRilCmCdmaSmsTest -->");
        telRilManager_->AddCdmaSimMessage(slotId, status, pdu, event);
        TELEPHONY_LOGI("TelRilTest::OnRequestAddRilCmCdmaSmsTest --> OnRequestAddRilCmCdmaSmsTest finished");
    }
}

void TelRilTest::OnRequestDelRilCmCdmaSmsTest(int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_DEL_CDMA_SMS);
    int32_t cdmaIndex;

    std::cout << "Please input gsmIndex: " << endl;
    std::cin >> cdmaIndex;
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestDelRilCmCdmaSmsTest -->");
        telRilManager_->DelCdmaSimMessage(slotId, cdmaIndex, event);
        TELEPHONY_LOGI("TelRilTest::OnRequestDelRilCmCdmaSmsTest --> OnRequestDelRilCmCdmaSmsTest finished");
    }
}

void TelRilTest::OnRequestUpdateRilCmCdmaSmsTest(
    int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_UPDATE_CDMA_SMS);
    int32_t cdmaIndex;
    int32_t state;
    std::string pdu;

    std::cout << "Please input gsmIndex: " << endl;
    std::cin >> cdmaIndex;
    std::cout << "Please input state: " << endl;
    std::cin >> state;
    std::cout << "Please input pdu: " << endl;
    std::cin >> pdu;
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        CdmaSimMessageParam cdmaSimMsg = {};
        cdmaSimMsg.cdmaIndex = cdmaIndex;
        cdmaSimMsg.status = state;
        cdmaSimMsg.pdu = pdu;
        TELEPHONY_LOGI("TelRilTest::OnRequestUpdateRilCmCdmaSmsTest -->");
        telRilManager_->UpdateCdmaSimMessage(slotId, cdmaSimMsg, event);
        TELEPHONY_LOGI("TelRilTest::OnRequestUpdateRilCmCdmaSmsTest --> OnRequestUpdateRilCmCdmaSmsTest finished");
    }
}

void TelRilTest::OnRequestShutDownTest(int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    int32_t eventId = static_cast<int32_t>(DiffInterfaceId::TEST_SHUT_DOWN);
    auto event = AppExecFwk::InnerEvent::Get(eventId);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestShutDownTest -->");
        telRilManager_->ShutDown(slotId, event);
        TELEPHONY_LOGI("TelRilTest::OnRequestShutDownTest --> OnRequestShutDownTest finished");
    }
}

void TelRilTest::OnRequestSetRadioStateTest(int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_SET_STATUS);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestSetRadioStateTest -->");
        int32_t radioState = -1;

        std::cout << "please enter the new radioState:";
        std::cin >> radioState;

        telRilManager_->SetRadioState(slotId, radioState, 0, event);
        TELEPHONY_LOGI("TelRilTest::OnRequestSetRadioStateTest --> OnRequestSetRadioStateTest finished");
    }
}

void TelRilTest::OnRequestGetRadioStateTest(int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_GET_STATUS);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestGetRadioStateTest -->");
        telRilManager_->GetRadioState(slotId, event);
        TELEPHONY_LOGI("TelRilTest::OnRequestGetRadioStateTest --> OnRequestGetRadioStateTest finished");
    }
}

void TelRilTest::OnRequestGetCellInfoListTest(int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_POWER);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestGetCellInfoListTest -->");
        telRilManager_->GetCellInfoList(slotId, event);
        TELEPHONY_LOGI("TelRilTest::OnRequestGetCellInfoListTest --> OnRequestGetCellInfoListTest finished");
    }
}

void TelRilTest::OnRequestGetCurrentCellInfoTest(
    int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_POWER);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestGetCurrentCellInfoTest -->");
        telRilManager_->GetCurrentCellInfo(slotId, event);
        TELEPHONY_LOGI("TelRilTest::OnRequestGetCurrentCellInfoTest --> OnRequestGetCurrentCellInfoTest finished");
    }
}

void TelRilTest::OnRequestSmsAcknowledgeTest(int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(MAXCONNS);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestSmsAcknowledgeTest -->");
        telRilManager_->SendSmsAck(slotId, true, REASON, event);
        TELEPHONY_LOGI("OnRequestSmsAcknowledgeTest finished");
    }
}

void TelRilTest::OnRequestSetInitApnInfoTest(int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    int32_t eventId = static_cast<int32_t>(DiffInterfaceId::TEST_RILCM_SET_INIT_APN_INFO_TEST);
    auto event = AppExecFwk::InnerEvent::Get(eventId);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestSetInitApnInfoTest -->");
        DataProfile dataProfile = {0, "cmnet", "IPV4V6", AUTHTYPE_1, "", "", "IPV4V6"};
        telRilManager_->SetInitApnInfo(slotId, dataProfile, event);
        TELEPHONY_LOGI("OnRequestSetInitApnInfoTest finished");
    }
}

void TelRilTest::OnRequestDataSetupDataCallTest(
    int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    int32_t eventId = static_cast<int32_t>(DiffInterfaceId::TEST_RILCM_SETUP_DATA_CALL_TEST);
    auto event = AppExecFwk::InnerEvent::Get(eventId);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestDataSetupDataCallTest -->");
        DataProfile dataProfile = {0, "cmnet", "IPV4V6", AUTHTYPE_1, "", "", "IPV4V6"};
        ActivateDataParam activateData = {0, REASON, dataProfile, false, true};
        telRilManager_->ActivatePdpContext(slotId, activateData, event);
        TELEPHONY_LOGI("OnRequestDataSetupDataCallTest finished");
    }
}

void TelRilTest::OnRequestDataDisableDataCallTest(
    int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    int32_t eventId = static_cast<int32_t>(DiffInterfaceId::TEST_RILCM_DEACTIVATE_DATA_CALL_TEST);
    auto event = AppExecFwk::InnerEvent::Get(eventId);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestCallDeactivateDataCallTest -->");
        telRilManager_->DeactivatePdpContext(slotId, CID, REASON, event);
        TELEPHONY_LOGI("OnRequestDataDisableDataCallTest finished");
    }
}

void TelRilTest::OnRequestGetDataCallListTest(int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    int32_t eventId = static_cast<int32_t>(DiffInterfaceId::TEST_RILCM_GET_DATA_CALL_LIST_TEST);
    auto event = AppExecFwk::InnerEvent::Get(eventId);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestGetDataCallListTest -->");
        telRilManager_->GetPdpContextList(slotId, event);
        TELEPHONY_LOGI("OnRequestGetDataCallListTest finished");
    }
}

void TelRilTest::OnRequestGetLinkBandwidthInfoTest(
    int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    int32_t eventId = static_cast<int32_t>(DiffInterfaceId::TEST_RILCM_GET_LINK_BANDWIDTH_INFO);
    auto event = AppExecFwk::InnerEvent::Get(eventId);
    if (event != nullptr && telRilManager_ != nullptr) {
        int32_t cid;
        cout << "Please enter the specified cid:";
        cin >> cid;
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestGetLinkBandwidthInfoTest -->");
        telRilManager_->GetLinkBandwidthInfo(slotId, cid, event);
        TELEPHONY_LOGI("OnRequestGetLinkBandwidthInfoTest finished");
    }
}

void TelRilTest::OnRequestGetLinkCapabilityTest(
    int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    int32_t eventId = static_cast<int32_t>(DiffInterfaceId::TEST_RILCM_GET_LINK_CAPABILITY);
    auto event = AppExecFwk::InnerEvent::Get(eventId);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestGetLinkCapabilityTest -->");
        telRilManager_->GetLinkCapability(slotId, event);
        TELEPHONY_LOGI("OnRequestGetLinkCapabilityTest finished");
    }
}

void TelRilTest::OnRequestCleanAllConnectionsTest(
    int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    int32_t eventId = static_cast<int32_t>(DiffInterfaceId::TEST_RILCM_CLEAN_ALL_DATA_CONNECTIONS_TEST);
    auto event = AppExecFwk::InnerEvent::Get(eventId);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestCleanAllConnectionsTest -->");
        telRilManager_->CleanAllConnections(slotId, event);
        TELEPHONY_LOGI("OnRequestCleanAllConnectionsTest finished");
    }
}

void TelRilTest::OnRequestSetLinkBandwidthReportingRuleTest(
    int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    int32_t eventId = static_cast<int32_t>(DiffInterfaceId::TEST_RILCM_SET_LINK_BANDWIDTH_REPORTING_RULE);
    auto event = AppExecFwk::InnerEvent::Get(eventId);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestSetLinkBandwidthReportingRuleTest -->");
        LinkBandwidthRule rule;
        rule.delayMs = BANDWIDTH_HYSTERESIS_MS;
        rule.rat = NETWORK_TYPE_LTE;
        rule.delayUplinkKbps = BANDWIDTH_HYSTERESIS_KBPS;
        rule.delayDownlinkKbps = BANDWIDTH_HYSTERESIS_KBPS;
        for (uint32_t i = 0; i < sizeof(MAX_UPLINK_LINK_BANDWIDTH) / sizeof(int32_t); i++) {
            rule.maximumUplinkKbps.push_back(MAX_UPLINK_LINK_BANDWIDTH[i]);
        }
        for (uint32_t i = 0; i < sizeof(MAX_DOWNLINK_LINK_BANDWIDTH) / sizeof(int32_t); i++) {
            rule.maximumDownlinkKbps.push_back(MAX_DOWNLINK_LINK_BANDWIDTH[i]);
        }
        telRilManager_->SetLinkBandwidthReportingRule(slotId, rule, event);
        TELEPHONY_LOGI("OnRequestSetLinkBandwidthReportingRuleTest finished");
    }
}

void TelRilTest::OnRequestSetDataPermittedTest(int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    int32_t eventId = static_cast<int32_t>(DiffInterfaceId::TEST_RILCM_SET_DATA_PERMITTED_TEST);
    auto event = AppExecFwk::InnerEvent::Get(eventId);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestSetDataPermittedTest -->");
        telRilManager_->SetDataPermitted(slotId, true, event);
        TELEPHONY_LOGI("TelRilTest::OnRequestSetDataPermittedTest --> finished");
    }
}

void TelRilTest::OnRequestGetNetworkSearchInformationTest(
    int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(TYPESBITMAP);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestGetNetworkSearchInformationTest -->");
        telRilManager_->GetNetworkSearchInformation(slotId, event);
        TELEPHONY_LOGI("TelRilTest::OnRequestGetNetworkSearchInformationTest -->finished");
    }
}

void TelRilTest::OnRequestGetNetworkSelectionModeTest(
    int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(TYPESBITMAP);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestGetNetworkSelectionModeTest -->");
        telRilManager_->GetNetworkSelectionMode(slotId, event);
        TELEPHONY_LOGI("OnRequestGetNetworkSelectionModeTest finished");
    }
}

void TelRilTest::OnRequestSetNetworkSelectionModeTest(
    int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(TYPESBITMAP);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestSetNetworkSelectionModeTest -->");
        telRilManager_->SetNetworkSelectionMode(slotId, 1, "46001", event);
        TELEPHONY_LOGI("OnRequestSetNetworkSelectionModeTest finished");
    }
}

void TelRilTest::OnRequestSetPreferredNetworkTest(
    int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(TYPESBITMAP);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        int32_t netType = 0;
        std::cout << "Please input netType: " << endl;
        std::cin >> netType;
        TELEPHONY_LOGI("TelRilTest::OnRequestSetPreferredNetworkTest -->");
        telRilManager_->SetPreferredNetwork(slotId, netType, event);
        TELEPHONY_LOGI("OnRequestSetPreferredNetworkTest finished");
    }
}

void TelRilTest::OnRequestGetPreferredNetworkTest(
    int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(TYPESBITMAP);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestGetPreferredNetworkTest -->");
        telRilManager_->GetPreferredNetwork(slotId, event);
        TELEPHONY_LOGI("OnRequestGetPreferredNetworkTest finished");
    }
}

void TelRilTest::OnRequestGetImeiTest(int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(TYPESBITMAP);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestGetImeiTest -->");
        telRilManager_->GetImei(slotId, event);
        TELEPHONY_LOGI("OnRequestGetImeiTest finished");
    }
}

void TelRilTest::OnRequestGetImeiSvTest(int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(TYPESBITMAP);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestGetImeiSvTest -->");
        telRilManager_->GetImeiSv(slotId, event);
        TELEPHONY_LOGI("OnRequestGetImeiSvTest finished");
    }
}

void TelRilTest::OnRequestGetBasebandVersionTest(int32_t slotId,
    const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    int32_t eventId = static_cast<int32_t>(DiffInterfaceId::TEST_GET_BASEBAND_VERSION);
    auto event = AppExecFwk::InnerEvent::Get(eventId);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestGetBasebandVersionTest -->");
        telRilManager_->GetBasebandVersion(slotId, event);
        TELEPHONY_LOGI("OnRequestGetBasebandVersionTest finished");
    }
}

void TelRilTest::OnRequestGetMeidTest(int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(TYPESBITMAP);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestGetMeidTest -->");
        telRilManager_->GetMeid(slotId, event);
        TELEPHONY_LOGI("OnRequestGetMeidTest finished");
    }
}

void TelRilTest::OnRequestGetCsRegStatusTest(int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(TYPESBITMAP);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestGetCsRegStatusTest -->");
        telRilManager_->GetCsRegStatus(slotId, event);
        TELEPHONY_LOGI("TelRilTest::OnRequestGetCsRegStatusTest -->finished");
    }
}

void TelRilTest::OnRequestGetPsRegStatusTest(int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(TYPESBITMAP);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestGetPsRegStatusTest -->");
        telRilManager_->GetPsRegStatus(slotId, event);
        TELEPHONY_LOGI("TelRilTest::OnRequestGetPsRegStatusTest -->finished");
    }
}

void TelRilTest::OnRequestGetVoiceRadioTechnology(
    int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(TYPESBITMAP);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestGetVoiceRadioTechnology -->");
        telRilManager_->GetVoiceRadioTechnology(slotId, event);
        TELEPHONY_LOGI("TelRilTest::OnRequestGetVoiceRadioTechnology -->finished");
    }
}

void TelRilTest::OnRequestGetPhysicalChannelConfig(
    int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(TYPESBITMAP);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestGetPhysicalChannelConfig -->");
        telRilManager_->GetPhysicalChannelConfig(slotId, event);
        TELEPHONY_LOGI("TelRilTest::OnRequestGetPhysicalChannelConfig -->finished");
    }
}

void TelRilTest::OnRequestSetLocateUpdatesTest(
    int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(TYPESBITMAP);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        const int32_t NOTIFY_STAT_ONLY = 1;
        const int32_t NOTIFY_STAT_LAC_CELLID = 2;
        int32_t notifyModeValue = REG_NOTIFY_STAT_LAC_CELLID;
        RegNotifyMode mode;
        std::cout << "Please input notifyMode(NOTIFY_STAT_ONLY: 1 / NOTIFY_STAT_LAC_CELLID: 2): " << endl;
        std::cin >> notifyModeValue;
        if (notifyModeValue == NOTIFY_STAT_ONLY) {
            mode = REG_NOTIFY_STAT_ONLY;
        } else if (notifyModeValue == NOTIFY_STAT_LAC_CELLID) {
            mode = REG_NOTIFY_STAT_LAC_CELLID;
        } else {
            std::cout << "Please input notifyMode invalid: " << endl;
            return;
        }
        TELEPHONY_LOGI("TelRilTest::OnRequestSetLocateUpdatesTest -->");
        telRilManager_->SetLocateUpdates(slotId, mode, event);
        TELEPHONY_LOGI("TelRilTest::OnRequestSetLocateUpdatesTest -->finished");
    }
}

void TelRilTest::OnRequestSetNotificationFilterTest(
    int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(TYPESBITMAP);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        int32_t filter = 0;
        std::cout << "Please input filter: " << endl;
        std::cin >> filter;
        TELEPHONY_LOGI("TelRilTest::OnRequestSetNotificationFilterTest -->");
        telRilManager_->SetNotificationFilter(slotId, filter, event);
        TELEPHONY_LOGI("TelRilTest::OnRequestSetNotificationFilterTest --> finished");
    }
}

void TelRilTest::OnRequestSetDeviceStateTest(
    int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(TYPESBITMAP);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        int32_t deviceStateType = 0;
        std::cout << "Please input deviceStateType: " << endl;
        std::cin >> deviceStateType;
        bool deviceStateOn = true;
        std::cout << "Please input deviceStateOn: " << endl;
        std::cin >> deviceStateOn;
        TELEPHONY_LOGI("TelRilTest::OnRequestSetDeviceStateTest -->");
        telRilManager_->SetDeviceState(slotId, deviceStateType, deviceStateOn, event);
        TELEPHONY_LOGI("TelRilTest::OnRequestSetDeviceStateTest --> finished");
    }
}

void TelRilTest::OnRequestGetRrcConnectionStateTest(
    int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    int32_t eventId = static_cast<int32_t>(DiffInterfaceId::TEST_GET_RRC_CONNECTION_STATE);
    auto event = AppExecFwk::InnerEvent::Get(eventId);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestGetRrcConnectionStateTest -->");
        telRilManager_->GetRrcConnectionState(slotId, event);
        TELEPHONY_LOGI("TelRilTest::OnRequestGetRrcConnectionStateTest --> finished");
    }
}

void TelRilTest::OnRequestGetNrOptionModeTest(int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    int32_t eventId = static_cast<int32_t>(DiffInterfaceId::TEST_GET_NR_OPTION_MODE);
    auto event = AppExecFwk::InnerEvent::Get(eventId);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestGetNrOptionModeTest -->");
        telRilManager_->GetNrOptionMode(slotId, event);
        TELEPHONY_LOGI("TelRilTest::OnRequestGetNrOptionModeTest --> finished");
    }
}

void TelRilTest::OnRequestSetNrOptionModeTest(int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    int32_t eventId = static_cast<int32_t>(DiffInterfaceId::TEST_SET_NR_OPTION_MODE);
    auto event = AppExecFwk::InnerEvent::Get(eventId);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestSetNrOptionModeTest -->");
        int32_t mode = 0;
        std::cout << "Please input nr mode: " << endl;
        std::cin >> mode;
        telRilManager_->SetNrOptionMode(slotId, mode, event);
        TELEPHONY_LOGI("TelRilTest::OnRequestSetNrOptionModeTest --> finished");
    }
}

void TelRilTest::OnRequestGetNrSsbIdTest(
    int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    int32_t eventId = static_cast<int32_t>(DiffInterfaceId::TEST_GET_NR_SSBID_INFO);
    auto event = AppExecFwk::InnerEvent::Get(eventId);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestGetNrSsbIdTest -->");
        telRilManager_->GetNrSsbId(slotId, event);
        TELEPHONY_LOGI("TelRilTest::OnRequestGetNrSsbIdTest --> finished");
    }
}

void TelRilTest::OnRequestCallJoinTest(int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    int32_t callType = 0; /* call type
                           * 0: Voice call
                           * 1: Video call: send one-way video, two-way voice
                           * 2: Video call: one-way receiving video, two-way voice
                           * 3: Video call: two-way video, two-way voice
                           */

    TELEPHONY_LOGI("RilUnitTest::OnRequestCallJoinTest -->");
    std::cout << "please enter the call type:";
    std::cin >> callType;

    auto event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_JOIN_CALL);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestCallJoinTest -->");
        telRilManager_->CombineConference(slotId, callType, event);
        TELEPHONY_LOGI("OnRequestCallJoinTest finished");
    }
}

void TelRilTest::OnRequestSeparateConferenceTest(
    int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    int32_t callIndex = 0;
    int32_t callType = 0; /* call type
                           * 0: Voice call
                           * 1: Video call: send one-way video, two-way voice
                           * 2: Video call: one-way receiving video, two-way voice
                           * 3: Video call: two-way video, two-way voice
                           */

    TELEPHONY_LOGI("RilUnitTest::OnRequestSeparateConferenceTest -->");

    std::cout << "please enter the call split number:";
    std::cin >> callIndex;

    std::cout << "please enter the call type:";
    std::cin >> callType;

    auto event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_SPLIT_CALL);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestSeparateConferenceTest -->");
        telRilManager_->SeparateConference(slotId, callIndex, callType, event);
        TELEPHONY_LOGI("OnRequestSeparateConferenceTest finished");
    }
}

void TelRilTest::OnRequestGetCallForwardTest(int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    int32_t reason = 0;
    std::cout << "please enter Get Call Forward reason<0-5>:";
    std::cin >> reason;

    auto event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_GET_CALL_FORWARD);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestGetCallForwardTest -->");
        telRilManager_->GetCallTransferInfo(slotId, reason, event);
        TELEPHONY_LOGI("OnRequestGetCallForwardTest ");
    }
}

void DiffParamType(int32_t loopFlag, int32_t reasonType)
{
    const int32_t MIN_TYPE = 0;
    const int32_t MAX_TYPE = 4;

    while (loopFlag) {
        cout << "0: unconditional" << endl;
        cout << "1: mobile busy" << endl;
        cout << "2: no reply" << endl;
        cout << "3: not reachable" << endl;
        cout << "4: all call forwarding" << endl;
        cout << "5: all conditional call forwarding" << endl;
        std::cout << "please select call forward reason type: ";
        std::cin >> reasonType;
        if (reasonType < MIN_TYPE || reasonType > MAX_TYPE) {
            std::cout << "select error, please retry!" << endl;
            continue;
        }
        break;
    }
}

void DiffParamMode(int32_t loopFlag, int32_t mode)
{
    const int32_t MIN_MODE = 0;
    const int32_t MAX_MODE = 4;
    const int32_t QUERY_STATUS = 2;

    while (loopFlag) {
        cout << "0: disable" << endl;
        cout << "1: enable" << endl; //  "2: query status"
        cout << "3: registration" << endl;
        cout << "4: erasure" << endl;
        std::cout << "please select call forward mode type: ";
        std::cin >> mode;
        if (mode < MIN_MODE || mode > MAX_MODE) {
            std::cout << "select error, please retry!" << endl;
            continue;
        }
        if (mode == QUERY_STATUS) {
            std::cout << "select error, mode 2 is query status please retry!" << endl;
            continue;
        }
        break;
    }
}

void DiffParamClassx(int32_t loopFlag, int32_t classx)
{
    const int32_t MIN_CLASSX = 1;
    const int32_t MAX_CLASSX = 16;

    while (loopFlag) {
        cout << "1: voice" << endl;
        cout << "2: data" << endl;
        cout << "4: fax" << endl;
        cout << "8: short message service" << endl;
        cout << "16: data circuit sync" << endl;
        std::cout << "please select call forward class type: ";
        std::cin >> classx;
        if (classx < MIN_CLASSX || classx > MAX_CLASSX) {
            std::cout << "select error, please retry!" << endl;
            continue;
        }
        break;
    }
}

void TelRilTest::OnRequestSetCallForwardTest(int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    int32_t mode = 0;
    int32_t reasonType = 0;
    int32_t classx = 0; /* call type
                         * 0: Voice call
                         * 1: Video call: send one-way video, two-way voice
                         * 2: Video call: one-way receiving video, two-way voice
                         * 3: Video call: two-way video, two-way voice
                         */
    std::string phoneNum;
    int32_t loopFlag = true;

    TELEPHONY_LOGI("RilUnitTest::OnRequestSetCallForwardTest -->");
    DiffParamType(loopFlag, reasonType);

    DiffParamMode(loopFlag, mode);

    std::cout << "please enter the phone number:";
    std::cin >> phoneNum;

    DiffParamClassx(loopFlag, classx);

    auto event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_SPLIT_CALL);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestSetCallForwardTest -->");
        CallTransferParam callTransfer;
        callTransfer.reason = reasonType;
        callTransfer.mode = mode;
        callTransfer.number = phoneNum;
        callTransfer.classx = classx;
        telRilManager_->SetCallTransferInfo(slotId, callTransfer, event);
        TELEPHONY_LOGI("OnRequestSetCallForwardTest finished");
    }
}

void TelRilTest::OnRequestGetClipTest(int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_GET_CALL_CLIP);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestGetClipTest -->");
        telRilManager_->GetClip(slotId, event);
        TELEPHONY_LOGI("OnRequestGetClipTest finished");
    }
}

void TelRilTest::OnRequestSetClipTest(int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_SET_CALL_CLIP);
    if (event != nullptr && telRilManager_ != nullptr) {
        int32_t action;
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestSetClipTest -->");
        std::cout << "please input call set clip action: ";
        std::cin >> action;
        telRilManager_->SetClip(slotId, action, event);
        TELEPHONY_LOGI("OnRequestSetClipTest finished");
    }
}

void TelRilTest::OnRequestGetClirTest(int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_GET_CALL_CLIR);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestGetClirTest -->");
        telRilManager_->GetClir(slotId, event);
        TELEPHONY_LOGI("TelRilTest::OnRequestGetClirTest --> finished");
    }
}

void TelRilTest::OnRequestSetClirTest(int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_SET_CALL_CLIR);
    if (event != nullptr && telRilManager_ != nullptr) {
        int32_t action;
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestSetClirTest -->");
        std::cout << "please input call set clir action: ";
        std::cin >> action;
        telRilManager_->SetClip(slotId, action, event);
        TELEPHONY_LOGI("TelRilTest::OnRequestSetClirTest --> finished");
    }
}

void TelRilTest::OnRequestGetCallRestrictionTest(
    int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_GET_CALL_RESTRICTION);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestGetCallRestrictionTest -->");
        telRilManager_->GetCallRestriction(slotId, "AI", event);
        TELEPHONY_LOGI("OnRequestGetCallRestrictionTest finished");
    }
}

void TelRilTest::OnRequestSetCallRestrictionTest(
    int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_SET_CALL_RESTRICTION);
    if (event != nullptr && telRilManager_ != nullptr) {
        CallRestrictionParam restriction = {};

        std::string fac;
        int32_t mode;
        std::string code;

        std::cout << "please enter the fac:";
        std::cin >> fac;
        std::cout << "please enter the mode:";
        std::cin >> mode;
        std::cout << "please enter the pwd:";
        std::cin >> code;

        TELEPHONY_LOGI("TelRilTest::%{public}s -->", __func__);
        restriction.fac = fac;
        restriction.mode = mode;
        size_t cpyLen = strlen(code.c_str()) + 1;
        size_t maxCpyLen = sizeof(restriction.password);
        if (strcpy_s(restriction.password, cpyLen > maxCpyLen ? maxCpyLen : cpyLen, code.c_str()) != EOK) {
            TELEPHONY_LOGE("Test stop by strcpy_s fail.");
            return;
        }
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestSetCallRestrictionTest -->");
        telRilManager_->SetCallRestriction(slotId, restriction, event);
        TELEPHONY_LOGI("OnRequestSetCallRestrictionTest finished");
    }
}

void TelRilTest::OnRequestSendDtmfTest(int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_SEND_DTMF);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("OnRequestSendDtmfTest finished");
    }
}

void TelRilTest::OnRequestStartDtmfTest(int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_START_DTMF);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("OnRequestStartDtmfTest finished");
    }
}

void TelRilTest::OnRequestStopDtmfTest(int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_STOP_DTMF);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestStopDtmfTest -->");
        TELEPHONY_LOGI("OnRequestStopDtmfTest finished");
    }
}

void TelRilTest::DemoHandler::ProcessEvent(const AppExecFwk::InnerEvent::Pointer &event)
{
    if (event == nullptr) {
        TELEPHONY_LOGE("TelRilTest::DemoHandler::ProcessEvent input param event is nullptr");
        return;
    }
    const uint32_t eventId = event->GetInnerEventId();
    TELEPHONY_LOGI("eventId:%{public}d, slotId:%{public}d", eventId, this->slotId_);
    switch (eventId) {
        case uint32_t(CustomMessageID::MSG_OPEN_LOGICAL_CHANNEL_DONE): {
            auto result = event->GetUniqueObject<OpenLogicalChannelResponse>();
            if (result == nullptr) {
                TELEPHONY_LOGE("Open logical response nullptr");
                break;
            }
            TELEPHONY_LOGI("Open logical response channel:'%{public}d'", result->channelId);
            break;
        }
        case uint32_t(CustomMessageID::MSG_TRANSMIT_LOGICAL_CHANNEL_DONE):
        case uint32_t(CustomMessageID::MSG_TRANSMIT_BASIC_CHANNEL_DONE): {
            auto result = event->GetUniqueObject<IccIoResultInfo>();
            if (result == nullptr) {
                TELEPHONY_LOGE("TransmitSimIO response nullptr");
                break;
            }
            TELEPHONY_LOGI("TransmitSimIO response:'%{public}s'", result->response.c_str());
            break;
        }
        case uint32_t(CustomMessageID::MSG_CLOSE_LOGICAL_CHANNEL_DONE): {
            TELEPHONY_LOGI("close logical channel done");
            break;
        }
        case uint32_t(CustomMessageID::MSG_SIM_AUTHENTICATION_DONE): {
            break;
        }
        case uint32_t(CustomMessageID::MSG_SIM_SEND_NCFG_OPER_INFO_DONE): {
            break;
        }
        case uint32_t(DiffInterfaceId::TEST_GET_BASEBAND_VERSION): {
            OnRequestGetBasebandVersionTestResponse(event);
            break;
        }
        case uint32_t(DiffInterfaceId::TEST_SHUT_DOWN): {
            OnRequestShutDownTestResponse(event);
            break;
        }
        default:
            ProcessRecordSize(event);
            break;
    }
}

void TelRilTest::DemoHandler::ProcessRecordSize(const AppExecFwk::InnerEvent::Pointer &event)
{
    int size = 0;
    std::unique_ptr<IccFromRilMsg> receiveMsg = event->GetUniqueObject<IccFromRilMsg>();
    if (receiveMsg == nullptr) {
        TELEPHONY_LOGE("receiveMsg is nullptr");
        return;
    }
    IccFileData *result = &(receiveMsg->fileData);
    std::shared_ptr<IccControllerHolder> &holder = receiveMsg->controlHolder;
    if (result == nullptr || holder == nullptr) {
        return;
    }
    TELEPHONY_LOGI(" resultData: %{public}s", result->resultData.c_str());
    int recordLen = 0;
    std::shared_ptr<unsigned char> rawData = SIMUtils::HexStringConvertToBytes(result->resultData, recordLen);
    if (rawData == nullptr) {
        return;
    }
    unsigned char *fileData = rawData.get();
    if (recordLen > LENGTH_OF_RECORD) {
        GetFileAndDataSize(fileData, holder->fileSize, size);
        if (holder->fileSize != 0) {
            holder->countFiles = size / holder->fileSize;
        }
    }
    TELEPHONY_LOGI("ProcessRecordSize %{public}d %{public}d %{public}d", size, holder->fileSize, holder->countFiles);
}

void TelRilTest::DemoHandler::GetFileAndDataSize(const unsigned char *data, int &fileSize, int &dataSize)
{
    uint8_t byteNum = 0xFF;
    uint32_t offset = 8;
    if (data == nullptr) {
        TELEPHONY_LOGE("GetFileAndDataSize null data");
        return;
    }
    fileSize = data[LENGTH_OF_RECORD] & byteNum;
    dataSize = ((data[SIZE_ONE_OF_FILE] & byteNum) << offset) + (data[SIZE_TWO_OF_FILE] & byteNum);
}

void TelRilTest::DemoHandler::OnRequestShutDownTestResponse(const AppExecFwk::InnerEvent::Pointer &event)
{
    std::shared_ptr<RadioResponseInfo> responseInfo = event->GetSharedObject<RadioResponseInfo>();
    if (responseInfo == nullptr) {
        TELEPHONY_LOGI("OnRequestShutDownTestResponse success");
    }
    TELEPHONY_LOGI("test shut down done");
}

void TelRilTest::DemoHandler::OnRequestGetBasebandVersionTestResponse(const AppExecFwk::InnerEvent::Pointer &event)
{
    std::shared_ptr<StringParcel> basebandVersion = event->GetSharedObject<StringParcel>();
    if (basebandVersion != nullptr) {
        TELEPHONY_LOGI("test get baseband version:%{public}s", basebandVersion->data.c_str());
    }
    TELEPHONY_LOGI("test get baseband version done");
}

void TelRilTest::OnRequestSetCallPreferenceModeTest(
    int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_SET_CALL_PREFERENCE_MODE);
    if (event != nullptr && telRilManager_ != nullptr) {
        int32_t mode = 0;
        event->SetOwner(handler);
        std::cout << "Please enter to set the voice call mode operation,[1-4]: ";
        std::cin >> mode;
        TELEPHONY_LOGI("TelRilTest::OnRequestSetCallPreferenceModeTest --> mode = [%{public}d]", mode);
        telRilManager_->SetCallPreferenceMode(slotId, mode, event);
        TELEPHONY_LOGI("OnRequestSetCallPreferenceModeTest finished");
    }
}

void TelRilTest::OnRequestGetCallPreferenceModeTest(
    int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_GET_CALL_PREFERENCE_MODE);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestGetCallPreferenceModeTest -->");
        telRilManager_->GetCallPreferenceMode(slotId, event);
        TELEPHONY_LOGI("OnRequestGetCallPreferenceModeTest finished");
    }
}

void TelRilTest::OnRequestSetUssdTest(int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_SET_USSD);
    if (event != nullptr && telRilManager_ != nullptr) {
        std::string str;
        event->SetOwner(handler);
        std::cout << "USSD string, the maximum length is 160 characters: ";
        std::cin >> str;
        TELEPHONY_LOGI("TelRilTest::OnRequestSetUssdTest --> str = [%{public}s]", str.c_str());
        telRilManager_->SetUssd(slotId, str, event);
        TELEPHONY_LOGI("OnRequestSetUssdTest finished");
    }
}

void TelRilTest::OnRequestGetUssdTest(int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_GET_USSD);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestGetUssdTest -->");
        telRilManager_->GetUssd(slotId, event);
        TELEPHONY_LOGI("OnRequestGetUssdTest finished");
    }
}

void TelRilTest::OnRequestGetCallFailReasonTest(
    int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_GET_CALL_FAIL_REASON);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestGetCallFailReasonTest -->");
        telRilManager_->GetCallFailReason(slotId, event);
        TELEPHONY_LOGI("OnRequestGetCallFailReasonTest finished");
    }
}

void TelRilTest::OnRequestSetMuteTest(int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_SET_CMUT);
    if (event != nullptr && telRilManager_ != nullptr) {
        int32_t mute;
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestSetMuteTest -->");
        std::cout << "CMUT set, the maximum length is 0 or 1: ";
        std::cin >> mute;
        TELEPHONY_LOGI("TelRilTest::OnRequestSetMuteTest --> mute = [%{public}d]", mute);
        telRilManager_->SetMute(slotId, mute, event);
        TELEPHONY_LOGI("OnRequestSetMuteTest finished");
    }
}

void TelRilTest::OnRequestGetMuteTest(int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_GET_CMUT);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestGetMuteTest -->");
        telRilManager_->GetMute(slotId, event);
        TELEPHONY_LOGI("OnRequestGetMuteTest finished");
    }
}

void TelRilTest::OnRequestGetEmergencyCallListTest(
    int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_GET_EMERGENCY_CALL_LIST);
    if (event != nullptr && telRilManager_ != nullptr) {
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestGetEmergencyCallListTest -->");
        telRilManager_->GetEmergencyCallList(slotId, event);
        TELEPHONY_LOGI("OnRequestGetEmergencyCallListTest finished");
    }
}

void TelRilTest::OnRequestSetVoNRSwitchTest(int32_t slotId, const std::shared_ptr<AppExecFwk::EventHandler> &handler)
{
    auto event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_SET_VONR_SWITCH_STATUS);
    if (event != nullptr && telRilManager_ != nullptr) {
        int32_t state;
        event->SetOwner(handler);
        TELEPHONY_LOGI("TelRilTest::OnRequestSetVoNRSwitchTest -->");
        std::cout << "Please input set value[0:disable 1:enable]:  ";
        std::cin >> state;
        TELEPHONY_LOGI("TelRilTest::OnRequestSetVoNRSwitchTest --> state = [%{public}d]", state);
        telRilManager_->SetVoNRSwitch(slotId, state, event);
        TELEPHONY_LOGI("OnRequestSetVoNRSwitchTest finished");
    }
}

void Promote()
{
    cout << "########################### TEL RIL TEST ######################" << endl;
    cout << "usage:" << endl;

    cout << static_cast<int32_t>(DiffInterfaceId::TEST_SHUT_DOWN) << " --> OnRequestShutDownTest" << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_SET_POWER_STATE) << " --> OnRequestSetRadioStateTest" << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_GET_POWER_STATE) << " --> OnRequestGetRadioStateTest" << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_GET_IMEI) << "--> OnRequestGetImeiTest" << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_GET_IMEISV) << "--> OnRequestGetImeiSvTest" << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_GET_BASEBAND_VERSION) << "--> OnRequestGetBasebandVersionTest"
         << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_GET_MEID) << "--> OnRequestGetMeidTest" << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_GET_VOICE_RADIO_INFO) << "--> OnRequestGetVoiceRadioTechnology"
         << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_EXIT) << "--> Exit" << endl << endl; // exit
}

void SimTest()
{
    /*-----------------------------------------------SIM-------------------------------------*/
    cout << "please input a cmd num: " << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_GET_SIM_CARD_STATUS) << "--> OnRequestSimGetSimStatusTest"
         << endl; // pass
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_SIM_IO) << "--> OnRequestSimIccIoTest" << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_GET_SIM_SMS) << "--> OnRequestSimSmsTest" << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_OPEN_LG_SIMIO) << "--> OnRequestOpenLGSimIOTest" << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_TRANSMIT_APDU_LOGICAL_CHANNEL)
         << "--> OnRequestTransmitApduLogicalChannelTest" << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_TRANSMIT_APDU_BASIC_CHANNEL)
         << "--> OnRequestTransmitApduBasicChannelTest" << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_CLOSE_LG_SIMIO) << "--> OnRequestCloseLGSimIOTest" << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_SIM_AUTH) << "--> OnRequestSimAuthenticationTest" << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_SIM_SEND_NCFG_OPER_INFO)
         << "--> OnRequestSendSimMatchedOperatorInfoTest" << endl;

    cout << static_cast<int32_t>(DiffInterfaceId::TEST_GET_IMSI) << "--> OnRequestSimGetImsiTest" << endl; // pass
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_GET_ICCID) << "--> OnRequestSimGetIccIDTest" << endl; // pass
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_GET_SIM_LOCK_STATUS) << "--> OnRequestGetSimLockStatusTest"
         << endl; // pass
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_SET_SIM_LOCK) << "--> OnRequestSetSimLockTest" << endl; // pass
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_GET_CHANGE_SIM_PASSWD) << "--> OnRequestChangeSimPasswordTest"
         << endl; // pass
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_ENTER_SIM_PIN) << "--> OnRequestEnterSimPinTest" << endl; // pass
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_UNLOCK_SIM_PIN) << "--> OnRequestUnlockSimPinTest"
         << endl; // pass
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_ENTER_SIM_PIN2) << "--> OnRequestEnterSimPin2Test"
         << endl; // pass
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_UNLOCK_SIM_PIN2) << "--> OnRequestUnlockSimPin2Test"
         << endl; // pass
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_ENABLE_SIM_CARD) << "--> OnRequestSetActiveSimTest"
         << endl; // pass

    cout << static_cast<int32_t>(DiffInterfaceId::TEST_RILCM_GET_DATA_CALL_LIST_TEST)
         << "--> OnRequestGetDataCallListTest" << endl; // pass
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_ACKNOWLEDGE_RILCM_LAST_INCOMING_GSM_SMS_TEST)
         << "--> OnRequestSmsAcknowledgeTest" << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_STK_SEND_TERMINAL_RESPONSE)
         << "--> OnRequestSendTerminalResponseCmdTest" << endl; // pass
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_STK_SEND_ENVELOPE)
         << "--> OnRequestSendEnvelopeCmdTest" << endl; // pass
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_STK_SEND_CALL_SETUP_REQUEST_RESULT)
         << "--> OnRequestSendCallSetupRequestResultTest" << endl; // pass
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_GET_RADIO_PROTOCOL)
         << "--> OnRequestGetRadioProtocolTest" << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_SET_RADIO_PROTOCOL)
         << "--> OnRequestSetRadioProtocolTest" << endl;
}

void DataTest()
{
    cout << "=========== Cellular Data Start =============" << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_RILCM_SET_INIT_APN_INFO_TEST)
         << "--> OnRequestSetInitApnInfoTest" << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_RILCM_SETUP_DATA_CALL_TEST)
         << "--> OnRequestDataSetupDataCallTest" << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_RILCM_DEACTIVATE_DATA_CALL_TEST)
         << "--> OnRequestDataDisableDataCallTest" << endl; // pass
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_RILCM_GET_LINK_BANDWIDTH_INFO)
         << "--> OnRequestGetLinkBandwidthInfoTest" << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_RILCM_SET_LINK_BANDWIDTH_REPORTING_RULE)
         << "--> OnRequestSetLinkBandwidthReportingRuleTest" << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_RILCM_SET_DATA_PERMITTED_TEST)
         << "--> OnRequestSetDataPermittedTest" << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_RILCM_GET_LINK_CAPABILITY)
         << "--> OnRequestGetLinkCapabilityTest" << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_RILCM_CLEAN_ALL_DATA_CONNECTIONS_TEST)
         << "--> OnRequestCleanAllConnectionsTest" << endl;
    cout << "=========== Cellular Data End =============" << endl;
}

void CallTest()
{
    /* --------------------------------- CALL -------------------------- */
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_CALL_DIAL) << " --> OnRequestCallDialTest" << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_HANDUP_CONNECT) << " --> OnRequestCallHangupTest" << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_ACCEPT_CALL) << "--> OnRequestCallAnswerTest" << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_CURRENT_CALLS) << "--> OnRequestCallGetCurrentCallsStatusTest"
         << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_REJECT_CALL) << "--> OnRequestRefusedCallTest" << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_JOIN_CALL) << "--> OnRequestCallJoinTest" << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_SPLIT_CALL) << "--> OnRequestSeparateConferenceTest" << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_GET_CALL_WAIT) << " --> OnRequestGetCallWaitTest" << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_SET_CALL_WAIT) << " --> OnRequestSetCallWaitTest" << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_GET_CALL_FORWARD) << " --> OnRequestGetCallForwardTest" << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_SET_CALL_FORWARD) << " --> OnRequestSetCallForwardTest" << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_GET_CALL_DEAL_CLIP) << " --> OnRequestGetClipTest" << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_SET_CALL_CLIP) << " --> OnRequestSetClipTest" << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_GET_CALL_DEAL_CLIR) << " --> OnRequestGetClirTest" << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_SET_CALL_CLIR) << " --> OnRequestSetClirTest" << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_GET_CALL_RESTRICTION) << " --> OnRequestGetCallRestrictionTest"
         << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_SET_CALL_RESTRICTION) << " --> OnRequestSetCallRestrictionTest"
         << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_SEND_DTMF) << " --> OnRequestSendDtmfTest" << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_START_DTMF) << " --> OnRequestStartDtmfTest" << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_STOP_DTMF) << " --> OnRequestStopDtmfTest" << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_SET_CALL_PREFERENCE_MODE)
         << "--> OnRequestSetCallPreferenceModeTest" << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_GET_CALL_PREFERENCE_MODE)
         << "--> OnRequestGetCallPreferenceModeTest" << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_SET_USSD) << " --> OnRequestSetUssdTest" << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_GET_USSD) << " --> OnRequestGetUssdTest" << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_SET_MUTE) << " --> OnRequestSetMuteTest" << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_GET_MUTE) << " --> OnRequestGetMuteTest" << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_GET_XLEMA) << " --> OnRequestGetEmergencyCallListTest" << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_GET_CALL_FAIL) << " --> OnRequestGetCallFailReasonTest" << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_SET_VONR_SWITCH) << " --> OnRequestSetVoNRSwitchTest" << endl;
}

void SmsTest()
{
    /* --------------------------------- SMS -------------------------- */
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_SEND_SMS) << "--> OnRequestSendRilCmSmsTest"
         << endl; // failed, Sim not inserted, radioResponseInfo->error : 2
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_SEND_CDMA_SMS) << "--> OnRequestSendRilCmCdmaSmsTest" << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_STORAGE_SMS) << "--> OnRequestStorageRilCmSmsTest" << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_DELETE_SMS) << "--> OnRequestDeleteRilCmSmsTest" << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_UPDATE_SMS) << "--> OnRequestUpdateRilCmSmsTest" << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_SET_SMS_CENTER_ADDRESS)
         << "--> OnRequestSetRilCmSmsCenterAddressTest" << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_GET_SMS_CENTER_ADDRESS)
         << "--> OnRequestGetRilCmSmsCenterAddressTest" << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_SET_CB_CONFIG) << "--> OnRequestSetRilCBConfigTest" << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_GET_CB_CONFIG) << "--> OnRequestGetRilCBConfigTest" << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_GET_CDMA_CB_CONFIG) << "--> OnRequestGetRilCdmaCBConfigTest"
         << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_SEND_SMS_EXPECT_MORE) << " --> OnRequestSmsSendSmsExpectMoreTest"
         << endl; // failed, Sim not inserted, radioResponseInfo->error : 2
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_GET_RILCM_VOICE_REGISTRATION_STATE_TEST)
         << "--> OnRequestNetworkVoiceRegistrationStateTest" << endl; // pass
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_GET_RILCM_DATA_REGISTRATION_STATE_TEST)
         << "--> OnRequestNetworkDataRegistrationStateTest" << endl; // pass
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_ADD_CDMA_SMS) << "--> OnRequestAddRilCmCdmaSmsTest" << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_DEL_CDMA_SMS) << "--> OnRequestDelRilCmCdmaSmsTest" << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_UPDATE_CDMA_SMS) << "--> OnRequestUpdateRilCmCdmaSmsTest"
         << endl;
}

void NetworkTest()
{
    /* --------------------------------- Network -------------------------- */
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_SET_PREFERRED_NETWORK_TYPE)
         << "--> OnRequestSetPreferredNetworkTest" << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_GET_PREFERRED_NETWORK_TYPE)
         << "--> OnRequestGetPreferredNetworkTest" << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_GET_CURRENT_CELL_INFO) << " --> OnRequestGetCurrentCellInfoTest"
         << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_GET_CELL_INFO_LIST) << " --> OnRequestGetCellInfoListTest"
         << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_OPERATOR) << " --> OnRequestNetworkOperatorTest"
         << endl; // failed, Invalid response: nullptr, radioResponseInfo->error : 2
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_GET_NETWORKS_TO_USE)
         << "--> OnRequestGetNetworkSearchInformationTest"
         << endl; // failed, Invalid response: nullptr, radioResponseInfo->error : 2
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_GET_SELECTION_MOD_FOR_NETWORKS)
         << "--> OnRequestGetNetworkSelectionModeTest"
         << endl; // failed, Invalid response: nullptr, radioResponseInfo->error : 2
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_SET_MODE_AUTOMATIC_NETWORKS)
         << "--> OnRequestSetNetworkSelectionModeTest"
         << endl; // failed, Invalid response: nullptr, radioResponseInfo->error : 2
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_GET_SIGNAL_STRENGTH) << "--> OnRequestNetworkGetRssiTest"
         << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_GET_CS_REG_STATUS) << "--> OnRequestGetCsRegStatusTest" << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_GET_PS_REG_STATUS) << "--> OnRequestGetPsRegStatusTest" << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_GET_PHYSICAL_CHANNEL_CONFIG)
         << "--> OnRequestGetPhysicalChannelConfig" << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_SET_LOCATE_UPDATES) << "--> OnRequestSetLocateUpdatesTest"
         << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_SET_NOTIFICATION_FILTER)
         << "--> OnRequestSetNotificationFilterTest" << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_SET_DEVICE_STATE) << "--> OnRequestSetDeviceStateTest" << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_GET_RRC_CONNECTION_STATE)
         << "--> OnRequestGetRrcConnectionStateTest" << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_GET_NR_OPTION_MODE) << "--> OnRequestGetNrOptionModeTest"
         << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_SET_NR_OPTION_MODE) << "--> OnRequestSetNrOptionModeTest"
         << endl;
    cout << static_cast<int32_t>(DiffInterfaceId::TEST_GET_NR_SSBID_INFO) << "--> OnRequestGetNrSsbIdTest" << endl;
}
} // namespace Telephony
} // namespace OHOS

using namespace OHOS;
using namespace OHOS::Telephony;
int main()
{
    int32_t slotId;
    std::vector<std::shared_ptr<TelRilTest::DemoHandler>> demohandler;
    std::unique_ptr<TelRilTest> rilManagerAndResponseTest = std::make_unique<TelRilTest>();
    if (rilManagerAndResponseTest == nullptr) {
        TELEPHONY_LOGE("rilManagerAndResponseTest is nullptr.");
        return TELEPHONY_ERROR;
    }
    rilManagerAndResponseTest->OnInitInterface();
    std::shared_ptr<AppExecFwk::EventRunner> eventRunner = AppExecFwk::EventRunner::Create("DemoHandler");
    if (eventRunner == nullptr) {
        TELEPHONY_LOGE("ERROR : AppExecFwk::EventRunner::Create(\"DemoHandler\") --> nullptr !!!");
        return TELEPHONY_ERROR;
    }
    for (slotId = 0; slotId < SIM_SLOT_COUNT; slotId++) {
        demohandler.push_back(make_shared<TelRilTest::DemoHandler>(slotId, eventRunner));
        rilManagerAndResponseTest->OnInitForRegister(slotId, demohandler[slotId]);
        TELEPHONY_LOGI("OnInitForRegister(g_handler) finished -->");
    }
    eventRunner->Run();
    int32_t what = 1;
    while (what) {
        SimTest();
        DataTest();
        CallTest();
        SmsTest();
        NetworkTest();
        Promote();
        cout << "Please select SIM card, 0 or 1." << endl;
        cin >> slotId;
        if ((slotId != 0) && (slotId != 1)) {
            cout << "Invalid SIM card selected." << endl;
            cout << "Please select SIM card, 0 or 1." << endl;
            continue;
        }
        cout << "Please select a test item." << endl;
        cin >> what;
        cout << "" << endl;
        if (what == static_cast<int32_t>(DiffInterfaceId::TEST_EXIT)) {
            break;
        }
        if ((what >= static_cast<int32_t>(DiffInterfaceId::TEST_GET_SIM_CARD_STATUS)) &&
            (what < static_cast<int32_t>(DiffInterfaceId::TEST_EXIT))) {
            rilManagerAndResponseTest->OnProcessInput(slotId, what, demohandler[slotId]);
        }
    }
    return 0;
}
