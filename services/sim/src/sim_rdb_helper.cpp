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

#include "sim_rdb_helper.h"

#include "telephony_errors.h"
#include "telephony_types.h"

namespace OHOS {
namespace Telephony {
SimRdbHelper::SimRdbHelper() {}

SimRdbHelper::~SimRdbHelper() {}

std::shared_ptr<DataShare::DataShareHelper> SimRdbHelper::CreateDataAHelper()
{
    TELEPHONY_LOGI("SimRdbHelper::CreateDataAHelper");
    auto saManager = SystemAbilityManagerClient::GetInstance().GetSystemAbilityManager();
    if (saManager == nullptr) {
        TELEPHONY_LOGE("SimRdbHelper Get system ability mgr failed.");
        return nullptr;
    }
    auto remoteObj = saManager->GetSystemAbility(TELEPHONY_CORE_SERVICE_SYS_ABILITY_ID);
    if (remoteObj == nullptr) {
        TELEPHONY_LOGE("SimRdbHelper GetSystemAbility Service Failed.");
        return nullptr;
    }
    if (helper_ == nullptr) {
        helper_ = DataShare::DataShareHelper::Creator(remoteObj, SimRdbInfo::SIM_RDB_URI);
    }
    return helper_;
}

int SimRdbHelper::Insert(const DataShare::DataShareValuesBucket &values)
{
    std::shared_ptr<DataShare::DataShareHelper> dataShareHelper = CreateDataAHelper();
    if (dataShareHelper == nullptr) {
        TELEPHONY_LOGE("SimRdbHelper::Insert failed by nullptr");
        return INVALID_VALUE;
    }
    Uri simUri(SimRdbInfo::SIM_RDB_SELECTION);
    TELEPHONY_LOGI("SimRdbHelper::Insert");
    return dataShareHelper->Insert(simUri, values);
}

std::shared_ptr<DataShare::DataShareResultSet> SimRdbHelper::Query(
    std::vector<std::string> &columns, const DataShare::DataSharePredicates &predicates)
{
    std::shared_ptr<DataShare::DataShareHelper> dataShareHelper = CreateDataAHelper();
    if (dataShareHelper == nullptr) {
        TELEPHONY_LOGE("SimRdbHelper::Query failed by nullptr");
        return nullptr;
    }
    Uri simUri(SimRdbInfo::SIM_RDB_SELECTION);
    TELEPHONY_LOGI("SimRdbHelper::Query");
    std::shared_ptr<DataShare::DataShareResultSet> ret = dataShareHelper->Query(simUri, predicates, columns);
    return ret;
}

int SimRdbHelper::Update(const DataShare::DataShareValuesBucket &value,
    const DataShare::DataSharePredicates &predicates)
{
    std::shared_ptr<DataShare::DataShareHelper> dataShareHelper = CreateDataAHelper();
    if (dataShareHelper == nullptr) {
        TELEPHONY_LOGE("SimRdbHelper::Update failed by nullptr");
        return INVALID_VALUE;
    }
    Uri simUri(SimRdbInfo::SIM_RDB_SELECTION);
    TELEPHONY_LOGI("SimRdbHelper::Update");
    return dataShareHelper->Update(simUri, predicates, value);
}

int SimRdbHelper::Delete(const DataShare::DataSharePredicates &predicates)
{
    std::shared_ptr<DataShare::DataShareHelper> dataShareHelper = CreateDataAHelper();
    if (dataShareHelper == nullptr) {
        TELEPHONY_LOGE("SimRdbHelper::Delete failed by nullptr");
        return INVALID_VALUE;
    }
    Uri simUri(SimRdbInfo::SIM_RDB_SELECTION);
    TELEPHONY_LOGI("SimRdbHelper::Delete");
    return dataShareHelper->Delete(simUri, predicates);
}

int32_t SimRdbHelper::GetDefaultMainCardSlotId()
{
    TELEPHONY_LOGI("SimRdbHelper::GetDefaultMainCardSlotId");
    int32_t mainCardSlotId = 0;
    std::vector<std::string> colume;
    DataShare::DataSharePredicates predicates;
    predicates.EqualTo(SimRdbInfo::IS_MAIN_CARD, std::to_string(static_cast<int32_t>(MAIN_CARD)));
    std::shared_ptr<DataShare::DataShareResultSet> result = Query(colume, predicates);
    if (result == nullptr) {
        TELEPHONY_LOGE("SimRdbHelper::get nothing");
        return mainCardSlotId;
    }
    int resultSetNum = result->GoToFirstRow();
    if (resultSetNum != 0) {
        TELEPHONY_LOGI("SimRdbHelper::GetDefaultMainCardSlotId not found main card");
        return mainCardSlotId;
    }
    int index = 0;
    result->GetColumnIndex(SimRdbInfo::SLOT_INDEX, index);
    result->GetInt(index, mainCardSlotId);
    result->Close();
    return mainCardSlotId;
}

int32_t SimRdbHelper::GetDefaultMessageCardSlotId()
{
    TELEPHONY_LOGI("SimRdbHelper::GetDefaultMessageCardSlotId");
    int32_t messageCardSlotId = 0;
    std::vector<std::string> colume;
    DataShare::DataSharePredicates predicates;
    predicates.EqualTo(SimRdbInfo::IS_MESSAGE_CARD, std::to_string(static_cast<int32_t>(MAIN_CARD)));
    std::shared_ptr<DataShare::DataShareResultSet> result = Query(colume, predicates);
    if (result == nullptr) {
        TELEPHONY_LOGE("SimRdbHelper::get nothing");
        return messageCardSlotId;
    }
    int resultSetNum = result->GoToFirstRow();
    if (resultSetNum != 0) {
        TELEPHONY_LOGI("SimRdbHelper::GetDefaultMessageCardSlotId not found default sms card");
        return messageCardSlotId;
    }
    int index = 0;
    result->GetColumnIndex(SimRdbInfo::SLOT_INDEX, index);
    result->GetInt(index, messageCardSlotId);
    result->Close();
    return messageCardSlotId;
}

int32_t SimRdbHelper::GetDefaultCellularDataCardSlotId()
{
    TELEPHONY_LOGI("SimRdbHelper::GetDefaultCellularDataCardSlotId");
    int32_t cellularDataCardSlotId = 0;
    std::vector<std::string> colume;
    DataShare::DataSharePredicates predicates;
    predicates.EqualTo(SimRdbInfo::IS_CELLULAR_DATA_CARD, std::to_string(static_cast<int32_t>(MAIN_CARD)));
    std::shared_ptr<DataShare::DataShareResultSet> result = Query(colume, predicates);
    if (result == nullptr) {
        TELEPHONY_LOGE("SimRdbHelper::get nothing");
        return cellularDataCardSlotId;
    }
    int resultSetNum = result->GoToFirstRow();
    if (resultSetNum != 0) {
        TELEPHONY_LOGI("SimRdbHelper::GetDefaultCellularDataCardSlotId not found default data card");
        return cellularDataCardSlotId;
    }
    int index = 0;
    result->GetColumnIndex(SimRdbInfo::SLOT_INDEX, index);
    result->GetInt(index, cellularDataCardSlotId);
    result->Close();
    return cellularDataCardSlotId;
}

int32_t SimRdbHelper::GetDefaultVoiceCardSlotId()
{
    int32_t voiceCardSlotId = INVALID_VALUE;
    std::vector<std::string> colume;
    DataShare::DataSharePredicates predicates;
    predicates.EqualTo(SimRdbInfo::IS_VOICE_CARD, std::to_string(static_cast<int32_t>(MAIN_CARD)));
    std::shared_ptr<DataShare::DataShareResultSet> result = Query(colume, predicates);
    if (result == nullptr) {
        TELEPHONY_LOGE("SimRdbHelper::get nothing");
        return voiceCardSlotId;
    }
    int resultSetNum = result->GoToFirstRow();
    if (resultSetNum != 0) {
        TELEPHONY_LOGI("SimRdbHelper::GetDefaultVoiceCardSlotId not found default voice card");
        return voiceCardSlotId;
    }
    int index = 0;
    result->GetColumnIndex(SimRdbInfo::SLOT_INDEX, index);
    result->GetInt(index, voiceCardSlotId);
    result->Close();
    TELEPHONY_LOGI("SimRdbHelper::GetDefaultVoiceCardSlotId = %{public}d", voiceCardSlotId);
    return voiceCardSlotId;
}

int32_t SimRdbHelper::SetDefaultMainCard(int32_t slotId)
{
    TELEPHONY_LOGI("SimRdbHelper::SetDefaultMainCard = %{public}d", slotId);
    DataShare::DataSharePredicates predicates;
    DataShare::DataShareValuesBucket value;
    DataShare::DataShareValueObject slotObj(slotId);
    DataShare::DataShareValueObject valueObj(static_cast<int>(DefaultCardType::MAIN));
    value.Put(SimRdbInfo::SLOT_INDEX, slotObj);
    value.Put(SimRdbInfo::CARD_TYPE, valueObj);
    std::shared_ptr<DataShare::DataShareHelper> dataShareHelper = CreateDataAHelper();
    if (dataShareHelper == nullptr) {
        TELEPHONY_LOGE("SimRdbHelper::SetDefaultMainCard failed by nullptr");
        return INVALID_VALUE;
    }
    Uri defaultUri(SimRdbInfo::SIM_RDB_DEFAULT_SET_URI);
    return dataShareHelper->Update(defaultUri, predicates, value);
}

int32_t SimRdbHelper::SetDefaultVoiceCard(int32_t slotId)
{
    TELEPHONY_LOGI("SimRdbHelper::SetDefaultVoiceCard = %{public}d", slotId);
    DataShare::DataSharePredicates predicates;
    DataShare::DataShareValuesBucket value;
    DataShare::DataShareValueObject slotObj(slotId);
    DataShare::DataShareValueObject valueObj(static_cast<int>(DefaultCardType::VOICE));
    value.Put(SimRdbInfo::SLOT_INDEX, slotObj);
    value.Put(SimRdbInfo::CARD_TYPE, valueObj);
    std::shared_ptr<DataShare::DataShareHelper> dataShareHelper = CreateDataAHelper();
    if (dataShareHelper == nullptr) {
        TELEPHONY_LOGE("SimRdbHelper::SetDefaultVoiceCard failed by nullptr");
        return INVALID_VALUE;
    }
    Uri defaultUri(SimRdbInfo::SIM_RDB_DEFAULT_SET_URI);
    return dataShareHelper->Update(defaultUri, predicates, value);
}

int32_t SimRdbHelper::SetDefaultMessageCard(int32_t slotId)
{
    TELEPHONY_LOGI("SimRdbHelper::SetDefaultMessageCard = %{public}d", slotId);
    DataShare::DataSharePredicates predicates;
    DataShare::DataShareValuesBucket value;
    DataShare::DataShareValueObject slotObj(slotId);
    DataShare::DataShareValueObject valueObj(static_cast<int>(DefaultCardType::SMS));
    value.Put(SimRdbInfo::SLOT_INDEX, slotObj);
    value.Put(SimRdbInfo::CARD_TYPE, valueObj);
    std::shared_ptr<DataShare::DataShareHelper> dataShareHelper = CreateDataAHelper();
    if (dataShareHelper == nullptr) {
        TELEPHONY_LOGE("SimRdbHelper::SetDefaultMessageCard failed by nullptr");
        return INVALID_VALUE;
    }
    Uri defaultUri(SimRdbInfo::SIM_RDB_DEFAULT_SET_URI);
    return dataShareHelper->Update(defaultUri, predicates, value);
}

int32_t SimRdbHelper::SetDefaultCellularData(int32_t slotId)
{
    TELEPHONY_LOGI("SimRdbHelper::SetDefaultCellularData = %{public}d", slotId);
    DataShare::DataSharePredicates predicates;
    DataShare::DataShareValuesBucket value;
    DataShare::DataShareValueObject slotObj(slotId);
    DataShare::DataShareValueObject valueObj(static_cast<int>(DefaultCardType::DATA));
    value.Put(SimRdbInfo::SLOT_INDEX, slotObj);
    value.Put(SimRdbInfo::CARD_TYPE, valueObj);
    std::shared_ptr<DataShare::DataShareHelper> dataShareHelper = CreateDataAHelper();
    if (dataShareHelper == nullptr) {
        TELEPHONY_LOGE("SimRdbHelper::SetDefaultCellularData failed by nullptr");
        return INVALID_VALUE;
    }
    Uri defaultUri(SimRdbInfo::SIM_RDB_DEFAULT_SET_URI);
    return dataShareHelper->Update(defaultUri, predicates, value);
}

int32_t SimRdbHelper::InsertData(int64_t &id, const DataShare::DataShareValuesBucket &values)
{
    TELEPHONY_LOGI("SimRdbHelper::InsertData");
    return Insert(values);
}

void SimRdbHelper::SaveDataToBean(std::shared_ptr<DataShare::DataShareResultSet> result, SimRdbInfo &simBean)
{
    TELEPHONY_LOGI("SimRdbHelper::SaveDataToBean");
    int index = 0;
    result->GetColumnIndex(SimRdbInfo::SIM_ID, index);
    result->GetInt(index, simBean.simId);
    result->GetColumnIndex(SimRdbInfo::ICC_ID, index);
    result->GetString(index, simBean.iccId);
    result->GetColumnIndex(SimRdbInfo::CARD_ID, index);
    result->GetString(index, simBean.cardId);
    result->GetColumnIndex(SimRdbInfo::IMS_SWITCH, index);
    result->GetInt(index, simBean.imsSwitch);
    result->GetColumnIndex(SimRdbInfo::SLOT_INDEX, index);
    result->GetInt(index, simBean.slotIndex);
    result->GetColumnIndex(SimRdbInfo::CARD_TYPE, index);
    result->GetInt(index, simBean.cardType);
    result->GetColumnIndex(SimRdbInfo::SHOW_NAME, index);
    result->GetString(index, simBean.showName);
    result->GetColumnIndex(SimRdbInfo::PHONE_NUMBER, index);
    result->GetString(index, simBean.phoneNumber);
    result->GetColumnIndex(SimRdbInfo::COUNTRY_CODE, index);
    result->GetString(index, simBean.countryCode);
    result->GetColumnIndex(SimRdbInfo::LANGUAGE, index);
    result->GetString(index, simBean.language);
    result->GetColumnIndex(SimRdbInfo::IMSI, index);
    result->GetString(index, simBean.imsi);
    result->GetColumnIndex(SimRdbInfo::IS_MAIN_CARD, index);
    result->GetInt(index, simBean.isMainCard);
    result->GetColumnIndex(SimRdbInfo::IS_VOICE_CARD, index);
    result->GetInt(index, simBean.isVoiceCard);
    result->GetColumnIndex(SimRdbInfo::IS_MESSAGE_CARD, index);
    result->GetInt(index, simBean.isMessageCard);
    result->GetColumnIndex(SimRdbInfo::IS_CELLULAR_DATA_CARD, index);
    result->GetInt(index, simBean.isCellularDataCard);
    result->GetColumnIndex(SimRdbInfo::IS_ACTIVE, index);
    result->GetInt(index, simBean.isActive);
}

int32_t SimRdbHelper::QueryDataBySlotId(int32_t slotId, SimRdbInfo &simBean)
{
    TELEPHONY_LOGI("SimRdbHelper::QueryDataBySlotId = %{public}d", slotId);
    std::string slot = std::to_string(slotId);
    std::vector<std::string> colume;
    DataShare::DataSharePredicates predicates;
    predicates.EqualTo(SimRdbInfo::SLOT_INDEX, slot);
    std::shared_ptr<DataShare::DataShareResultSet> result = Query(colume, predicates);
    if (result == nullptr) {
        TELEPHONY_LOGE("SimRdbHelper::QueryDataBySlotId get nothing");
        return INVALID_VALUE;
    }
    int resultSetNum = result->GoToFirstRow();
    while (resultSetNum == 0) {
        SaveDataToBean(result, simBean);
        resultSetNum = result->GoToNextRow();
    }
    result->Close();
    return TELEPHONY_SUCCESS;
}

int32_t SimRdbHelper::QueryDataByIccId(std::string iccId, SimRdbInfo &simBean)
{
    TELEPHONY_LOGI("SimRdbHelper::QueryDataByIccId");
    std::vector<std::string> colume;
    DataShare::DataSharePredicates predicates;
    predicates.EqualTo(SimRdbInfo::ICC_ID, iccId);
    std::shared_ptr<DataShare::DataShareResultSet> result = Query(colume, predicates);
    if (result == nullptr) {
        TELEPHONY_LOGE("SimRdbHelper::QueryDataByIccId get nothing");
        return INVALID_VALUE;
    }
    int resultSetNum = result->GoToFirstRow();
    while (resultSetNum == 0) {
        SaveDataToBean(result, simBean);
        resultSetNum = result->GoToNextRow();
    }
    result->Close();
    return TELEPHONY_SUCCESS;
}

int32_t SimRdbHelper::QueryAllData(std::vector<SimRdbInfo> &vec)
{
    TELEPHONY_LOGI("SimRdbHelper::QueryAllData");
    std::vector<std::string> colume;
    DataShare::DataSharePredicates predicates;
    std::shared_ptr<DataShare::DataShareResultSet> result = Query(colume, predicates);
    if (result == nullptr) {
        TELEPHONY_LOGE("SimRdbHelper::QueryAllData get nothing");
        return INVALID_VALUE;
    }
    int resultSetNum = result->GoToFirstRow();
    while (resultSetNum == 0) {
        SimRdbInfo simBean;
        SaveDataToBean(result, simBean);
        vec.push_back(simBean);
        resultSetNum = result->GoToNextRow();
    }
    result->Close();
    return TELEPHONY_SUCCESS;
}

int32_t SimRdbHelper::QueryAllValidData(std::vector<SimRdbInfo> &vec)
{
    TELEPHONY_LOGI("SimRdbHelper::QueryAllValidData");
    std::vector<std::string> colume;
    std::string id = std::to_string(DEACTIVE);
    DataShare::DataSharePredicates predicates;
    predicates.GreaterThan(SimRdbInfo::IS_ACTIVE, id);
    std::shared_ptr<DataShare::DataShareResultSet> result = Query(colume, predicates);
    if (result == nullptr) {
        TELEPHONY_LOGE("SimRdbHelper::QueryAllValidData get nothing");
        return INVALID_VALUE;
    }
    int resultSetNum = result->GoToFirstRow();
    while (resultSetNum == 0) {
        SimRdbInfo simBean;
        SaveDataToBean(result, simBean);
        vec.push_back(simBean);
        resultSetNum = result->GoToNextRow();
    }
    result->Close();
    return TELEPHONY_SUCCESS;
}

int32_t SimRdbHelper::UpdateDataBySlotId(int32_t slotId, const DataShare::DataShareValuesBucket &values)
{
    TELEPHONY_LOGI("SimRdbHelper::UpdateDataBySlotId = %{public}d", slotId);
    std::string slot = std::to_string(slotId);
    DataShare::DataSharePredicates predicates;
    predicates.EqualTo(SimRdbInfo::SLOT_INDEX, slot);
    return Update(values, predicates);
}

int32_t SimRdbHelper::UpdateDataByIccId(std::string iccId, const DataShare::DataShareValuesBucket &values)
{
    TELEPHONY_LOGI("SimRdbHelper::UpdateDataByIccId");
    DataShare::DataSharePredicates predicates;
    predicates.EqualTo(SimRdbInfo::ICC_ID, iccId);
    return Update(values, predicates);
}

int32_t SimRdbHelper::ForgetAllData()
{
    TELEPHONY_LOGI("SimRdbHelper::ForgetAllData");
    DataShare::DataSharePredicates predicates;
    DataShare::DataShareValuesBucket value;
    DataShare::DataShareValueObject valueObj(DEACTIVE);
    value.Put(SimRdbInfo::IS_ACTIVE, valueObj);
    return Update(value, predicates);
}

int32_t SimRdbHelper::ForgetAllData(int32_t slotId)
{
    TELEPHONY_LOGI("SimRdbHelper::ForgetAllData slotId = %{public}d", slotId);
    DataShare::DataSharePredicates predicates;
    predicates.EqualTo(SimRdbInfo::SLOT_INDEX, std::to_string(slotId));
    DataShare::DataShareValuesBucket value;
    DataShare::DataShareValueObject valueObj(DEACTIVE);
    value.Put(SimRdbInfo::IS_ACTIVE, valueObj);
    return Update(value, predicates);
}

int32_t SimRdbHelper::ClearData()
{
    std::string id = std::to_string(INVALID_VALUE);
    DataShare::DataSharePredicates predicates;
    predicates.GreaterThan(SimRdbInfo::SIM_ID, id);
    return Delete(predicates);
}
} // namespace Telephony
} // namespace OHOS
