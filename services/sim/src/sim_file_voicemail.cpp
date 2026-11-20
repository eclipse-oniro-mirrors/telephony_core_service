/*
 * Copyright (C) 2025 Huawei Device Co., Ltd.
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

#include "sim_file_voicemail.h"

#include <unistd.h>

#include <chrono>
#include <map>
#include <vector>

#include "app_mgr_client.h"
#include "common_event_manager.h"
#include "common_event_support.h"
#include "configuration.h"
#include "core_manager_inner.h"
#include "radio_event.h"
#include "sim_number_decode.h"
#include "telephony_common_utils.h"
#include "telephony_ext_wrapper.h"
#include "telephony_state_registry_client.h"

namespace OHOS {
namespace Telephony {
constexpr static const int32_t WAIT_TIME_SECOND = 1;
const int CFIS_BCD_NUMBER_LENGTH_OFFSET = 2;
const int CFIS_TON_NPI_OFFSET = 3;
const int CFIS_ADN_CAPABILITY_ID_OFFSET = 14;
const int CFIS_ADN_EXTENSION_ID_OFFSET = 15;

std::string SimFileVoiceMail::GetVoiceMailNumber(SimFile &simFile)
{
    std::shared_lock<ffrt::shared_mutex> lock(simFile.voiceMailMutex_);
    return simFile.voiceMailNum_;
}

void SimFileVoiceMail::SetVoiceMailNumber(const std::string mailNumber, SimFile &simFile)
{
    std::unique_lock<ffrt::shared_mutex> lock(simFile.voiceMailMutex_);
    simFile.voiceMailNum_ = mailNumber;
}

bool SimFileVoiceMail::VoiceMailNotEditToSim(SimFile &simFile)
{
    OperatorConfig operatorConfig;
    CoreManagerInner::GetInstance().GetOperatorConfigs(simFile.slotId_, operatorConfig);
    bool custVmNotToSim = false;
    std::map<std::string, bool>::iterator it = operatorConfig.boolValue.find(KEY_VOICE_MAIL_EDIT_NOT_TO_SIM_BOOL);
    if (it != operatorConfig.boolValue.end()) {
        custVmNotToSim = it->second;
    }
    return custVmNotToSim;
}

bool SimFileVoiceMail::UpdateVoiceMail(const std::string &mailName, const std::string &mailNumber, SimFile &simFile)
{
    if (VoiceMailNotEditToSim(simFile)) {
        TELEPHONY_LOGI("SimFile::UpdateVoiceMail, no need to edit voice mail info to sim");
        return true;
    }
    simFile.waitResult_ = false;
    std::shared_ptr<DiallingNumbersInfo> diallingNumber = std::make_shared<DiallingNumbersInfo>();
    diallingNumber->name_ = Str8ToStr16(mailName);
    diallingNumber->number_ = Str8ToStr16(mailNumber);

    if ((simFile.indexOfMailbox_) && (simFile.indexOfMailbox_ != simFile.BYTE_NUM)) {
        std::unique_lock<ffrt::mutex> lock(simFile.mtx_);
        TELEPHONY_LOGI("UpdateVoiceMail start MBDN");
        AppExecFwk::InnerEvent::Pointer event =
            simFile.CreateDiallingNumberPointer(MSG_SIM_SET_MBDN_DONE, 0, 0, nullptr);
        DiallingNumberUpdateInfor infor;
        infor.diallingNumber = diallingNumber;
        infor.fileId = ELEMENTARY_FILE_MBDN;
        infor.extFile = ELEMENTARY_FILE_EXT6;
        infor.index = simFile.indexOfMailbox_;
        simFile.diallingNumberHandler_->UpdateDiallingNumbers(infor, event);
        while (!simFile.waitResult_) {
            TELEPHONY_LOGI("update voicemail wait, response = false");
            if (simFile.processWait_.wait_for(lock, std::chrono::seconds(WAIT_TIME_SECOND)) ==
                ffrt::cv_status::timeout) {
                break;
            }
        }
    } else if (CphsVoiceMailAvailable(simFile)) {
        std::unique_lock<ffrt::mutex> lock(simFile.mtx_);
        AppExecFwk::InnerEvent::Pointer event =
            simFile.CreateDiallingNumberPointer(MSG_SIM_SET_CPHS_MAILBOX_DONE, 0, 0, nullptr);
        DiallingNumberUpdateInfor infor;
        infor.diallingNumber = diallingNumber;
        infor.fileId = ELEMENTARY_FILE_MAILBOX_CPHS;
        infor.extFile = ELEMENTARY_FILE_EXT1;
        infor.index = 1;
        simFile.diallingNumberHandler_->UpdateDiallingNumbers(infor, event);
        while (!simFile.waitResult_) {
            TELEPHONY_LOGI("update voicemail wait, response = false");
            if (simFile.processWait_.wait_for(lock, std::chrono::seconds(WAIT_TIME_SECOND)) ==
                ffrt::cv_status::timeout) {
                break;
            }
        }
    } else {
        TELEPHONY_LOGE("UpdateVoiceMail indexOfMailbox_ %{public}d is invalid!!", simFile.indexOfMailbox_);
    }
    TELEPHONY_LOGI("UpdateVoiceMail finished %{public}d", simFile.waitResult_);
    return simFile.waitResult_;
}

bool SimFileVoiceMail::SetVoiceMailCount(int32_t voiceMailCount, SimFile &simFile)
{
    bool setDone = false;
    AppExecFwk::InnerEvent::Pointer eventUpdate = simFile.BuildCallerInfo(MSG_SIM_UPDATE_DONE);
    std::shared_ptr<unsigned char> efMWISData =
        SIMUtils::HexStringConvertToBytes(simFile.efMWISStr_, simFile.efMWISSize_);
    simFile.efMWIS_ = efMWISData != nullptr ? efMWISData.get() : nullptr;
    if (simFile.efMWIS_ != nullptr && simFile.efMWISSize_ > 1) {
        simFile.efMWIS_[0] = static_cast<unsigned char>(
            (simFile.efMWIS_[0] & simFile.BYTE_NUM6) | (voiceMailCount == 0 ? 0 : simFile.BYTE_NUM2));
        simFile.efMWIS_[1] = voiceMailCount < 0 ? 0 : static_cast<unsigned char>(voiceMailCount);
        simFile.fileController_->UpdateLinearFixedFile(
            ELEMENTARY_FILE_MWIS, 1, simFile.efMWISStr_, simFile.efMWISSize_, "", eventUpdate);
        setDone = true;
    }
    std::shared_ptr<unsigned char> efCphsMwiData =
        SIMUtils::HexStringConvertToBytes(simFile.efCphsMwisStr_, simFile.efCphsMwiSize_);
    simFile.efCphsMwi_ = efCphsMwiData != nullptr ? efCphsMwiData.get() : nullptr;
    if (simFile.efCphsMwi_ != nullptr && simFile.efCphsMwiSize_ > 1) {
        simFile.efCphsMwi_[0] = static_cast<unsigned char>(
            (simFile.efCphsMwi_[0] & simFile.BYTE_NUM7) |
            (voiceMailCount == 0 ? simFile.BYTE_NUM5 : simFile.BYTE_NUM4));
        simFile.fileController_->UpdateBinaryFile(
            ELEMENTARY_FILE_VOICE_MAIL_INDICATOR_CPHS, simFile.efCphsMwisStr_, simFile.efCphsMwiSize_, eventUpdate);
        setDone = true;
    }
    if (setDone) {
        simFile.voiceMailCount_ = voiceMailCount;
        DelayedRefSingleton<TelephonyStateRegistryClient>::GetInstance().UpdateVoiceMailMsgIndicator(
            simFile.slotId_, simFile.voiceMailCount_ > 0);
        return true;
    }
    TELEPHONY_LOGE("SetVoiceMailCount efMWIS_ and efCphsMwi_ is nullptr");
    return false;
}

bool SimFileVoiceMail::SetVoiceCallForwarding(bool enable, const std::string &number, SimFile &simFile)
{
    bool setDone = false;
    AppExecFwk::InnerEvent::Pointer eventUpdate = simFile.BuildCallerInfo(MSG_SIM_UPDATE_DONE);
    std::shared_ptr<unsigned char> efCfisData =
        SIMUtils::HexStringConvertToBytes(simFile.efCfisStr_, simFile.efCfisSize_);
    simFile.efCfis_ = efCfisData != nullptr ? efCfisData.get() : nullptr;
    if (EfCfisAvailable(simFile, simFile.efCfisSize_)) {
        if (enable) {
            simFile.efCfis_[1] |= simFile.BYTE_NUM2;
        } else {
            simFile.efCfis_[1] &= simFile.BYTE_NUM6;
        }
        if (enable && !number.empty()) {
            if (!FillNumber(efCfisData, simFile.efCfisSize_, number, simFile)) {
                TELEPHONY_LOGE("fill number failed");
                return false;
            }
        }
        simFile.fileController_->UpdateLinearFixedFile(
            ELEMENTARY_FILE_CFIS, 1, simFile.efCfisStr_, simFile.efCfisSize_, "", eventUpdate);
        setDone = true;
    }
    std::shared_ptr<unsigned char> efCffData =
        SIMUtils::HexStringConvertToBytes(simFile.efCffStr_, simFile.efCffSize_);
    simFile.efCff_ = efCffData != nullptr ? efCffData.get() : nullptr;
    if (simFile.efCff_ != nullptr && simFile.efCffSize_ > 0) {
        if (enable) {
            simFile.efCff_[0] = static_cast<unsigned char>((simFile.efCff_[0] & simFile.BYTE_NUM7) | simFile.BYTE_NUM4);
        } else {
            simFile.efCff_[0] = static_cast<unsigned char>((simFile.efCff_[0] & simFile.BYTE_NUM7) | simFile.BYTE_NUM5);
        }
        simFile.fileController_->UpdateBinaryFile(
            ELEMENTARY_FILE_CFF_CPHS, simFile.efCffStr_, simFile.efCffSize_, eventUpdate);
        setDone = true;
    }
    if (setDone) {
        simFile.callForwardingStatus = enable ? CALL_FORWARDING_STATUS_ENABLED : CALL_FORWARDING_STATUS_DISABLED;
        DelayedRefSingleton<TelephonyStateRegistryClient>::GetInstance().UpdateCfuIndicator(
            simFile.slotId_, simFile.callForwardingStatus == CALL_FORWARDING_STATUS_ENABLED);
        return true;
    }
    TELEPHONY_LOGE("SetVoiceCallForwarding efCfis_ and efCff_ is nullptr");
    return false;
}

bool SimFileVoiceMail::FillNumber(std::shared_ptr<unsigned char> efCfisData, int32_t efCfisSize,
    const std::string &number, SimFile &simFile)
{
    std::vector<uint8_t> bcdCodes;
    SimNumberDecode::NumberConvertToBCD(number, bcdCodes, false, SimNumberDecode::BCD_TYPE_ADN);
    unsigned int dataLength = bcdCodes.size();
    unsigned char numberData[dataLength];
    for (unsigned int i = 0; i < dataLength; ++i) {
        numberData[i] = bcdCodes.at(i);
    }
    if (CFIS_TON_NPI_OFFSET + static_cast<int32_t>(dataLength) >= efCfisSize) {
        TELEPHONY_LOGE("data is invalid");
        return false;
    }
    auto efCfis = efCfisData.get();
    SIMUtils::ArrayCopy(numberData, 0, efCfis, CFIS_TON_NPI_OFFSET, dataLength);
    if (CFIS_ADN_EXTENSION_ID_OFFSET >= efCfisSize) {
        TELEPHONY_LOGE("data is invalid");
        return false;
    }
    efCfis[CFIS_BCD_NUMBER_LENGTH_OFFSET] = static_cast<unsigned char>(dataLength);
    efCfis[CFIS_ADN_CAPABILITY_ID_OFFSET] = static_cast<unsigned char>(simFile.BYTE_NUM);
    efCfis[CFIS_ADN_EXTENSION_ID_OFFSET] = static_cast<unsigned char>(simFile.BYTE_NUM);
    return true;
}

bool SimFileVoiceMail::CphsVoiceMailAvailable(SimFile &simFile)
{
    bool available = false;
    if (!simFile.cphsInfo_.empty()) {
        int dataLen = 0;
        std::shared_ptr<unsigned char> dataByte = SIMUtils::HexStringConvertToBytes(simFile.cphsInfo_, dataLen);
        available = (dataByte != nullptr)
            ? (dataByte.get()[1] & simFile.CPHS_VOICE_MAIL_MASK) == simFile.CPHS_VOICE_MAIL_EXSIT
            : false;
    }
    return available;
}

bool SimFileVoiceMail::EfCfisAvailable(SimFile &simFile, int32_t size)
{
    if (simFile.efCfis_ != nullptr && size > 1) {
        for (int32_t i = 0; i < size; ++i) {
            if (simFile.efCfis_[i] != simFile.BYTE_NUM) {
                return true;
            }
        }
    }
    return false;
}

void SimFileVoiceMail::GetCphsMailBox(SimFile &simFile)
{
    simFile.fileToGet_ += simFile.LOAD_STEP;
    AppExecFwk::InnerEvent::Pointer cphsEvent =
        simFile.CreateDiallingNumberPointer(MSG_SIM_OBTAIN_CPHS_MAILBOX_DONE, 0, 0, nullptr);
    simFile.diallingNumberHandler_->GetDiallingNumbers(
        ELEMENTARY_FILE_MAILBOX_CPHS, ELEMENTARY_FILE_EXT1, 1, cphsEvent);
}
} // namespace Telephony
} // namespace OHOS