/*
 * Copyright (C) 2026 Huawei Device Co., Ltd.
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
#include <memory>
#include <string>
#include <vector>

#define private public
#define protected public

#include "gtest/gtest.h"

#include "cJSON.h"
#include "network_search_manager.h"
#include "network_type.h"
#include "network_utils.h"
#include "nitz_update.h"
#include "operator_name_utils.h"
#include "radio_event.h"
#include "signal_info.h"
#include "sim_manager.h"
#include "tel_ril_manager.h"
#include "telephony_ext_wrapper.h"

namespace OHOS {
namespace Telephony {
using namespace testing::ext;

namespace {
constexpr int32_t COVERAGE_SLOT_ID = 0;
constexpr int32_t INVALID_RSSI = 5;
constexpr int32_t VALID_RSSI = -5;
} // namespace

class NetworkSearchCoverageTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};

void NetworkSearchCoverageTest::SetUpTestCase() {}

void NetworkSearchCoverageTest::TearDownTestCase() {}

void NetworkSearchCoverageTest::SetUp() {}

void NetworkSearchCoverageTest::TearDown() {}

HWTEST_F(NetworkSearchCoverageTest, Telephony_Coverage_SignalInfoEvent, TestSize.Level0)
{
    SignalInfo signalInfo;
    AppExecFwk::InnerEvent::Pointer nullEvent(nullptr, nullptr);
    signalInfo.ProcessSignalIntensity(COVERAGE_SLOT_ID, nullEvent);

    auto eventWithoutRssi = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_SIGNAL_STRENGTH_UPDATE);
    ASSERT_NE(eventWithoutRssi, nullptr);
    signalInfo.ProcessSignalIntensity(COVERAGE_SLOT_ID, eventWithoutRssi);

    auto rssi = std::make_shared<Rssi>();
    auto eventWithRssi = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_SIGNAL_STRENGTH_UPDATE, rssi);
    ASSERT_NE(eventWithRssi, nullptr);
    TELEPHONY_EXT_WRAPPER.processSignalInfos_ = [](int32_t slotId, Rssi &signalIntensity) {
        return false;
    };
    signalInfo.ProcessSignalIntensity(COVERAGE_SLOT_ID, eventWithRssi);
    TELEPHONY_EXT_WRAPPER.processSignalInfos_ = nullptr;
}

HWTEST_F(NetworkSearchCoverageTest, Telephony_Coverage_SignalInfoProcess, TestSize.Level0)
{
    SignalInfo signalInfo;

    Rssi unchanged {};
    signalInfo.ProcessSignalIntensity(COVERAGE_SLOT_ID, &unchanged);

    Rssi invalidChanged {};
    invalidChanged.gw.rxlev = INVALID_RSSI;
    signalInfo.ProcessSignalIntensity(COVERAGE_SLOT_ID, &invalidChanged);

    Rssi allValid {};
    allValid.gw.rxlev = VALID_RSSI;
    allValid.cdma.absoluteRssi = VALID_RSSI;
    allValid.lte.rsrp = VALID_RSSI;
    allValid.wcdma.rscp = VALID_RSSI;
    allValid.tdScdma.rscp = VALID_RSSI;
    allValid.nr.rsrp = VALID_RSSI;
    signalInfo.ProcessSignalIntensity(COVERAGE_SLOT_ID, &allValid);

    signalInfo.ProcessSignalIntensity(COVERAGE_SLOT_ID, &allValid);

    Rssi changeCdma = allValid;
    changeCdma.cdma.absoluteRssi = VALID_RSSI - 1;
    signalInfo.ProcessSignalIntensity(COVERAGE_SLOT_ID, &changeCdma);

    Rssi changeLte = changeCdma;
    changeLte.lte.rsrp = VALID_RSSI - 1;
    signalInfo.ProcessSignalIntensity(COVERAGE_SLOT_ID, &changeLte);

    Rssi changeWcdma = changeLte;
    changeWcdma.wcdma.rscp = VALID_RSSI - 1;
    signalInfo.ProcessSignalIntensity(COVERAGE_SLOT_ID, &changeWcdma);

    Rssi changeTdScdma = changeWcdma;
    changeTdScdma.tdScdma.rscp = VALID_RSSI - 1;
    signalInfo.ProcessSignalIntensity(COVERAGE_SLOT_ID, &changeTdScdma);

    Rssi changeNr = changeTdScdma;
    changeNr.nr.rsrp = VALID_RSSI - 1;
    signalInfo.ProcessSignalIntensity(COVERAGE_SLOT_ID, &changeNr);
}

HWTEST_F(NetworkSearchCoverageTest, Telephony_Coverage_OperatorNameParse, TestSize.Level0)
{
    OperatorNameUtils &utils = OperatorNameUtils::GetInstance();
    EXPECT_EQ(utils.ParseString(nullptr), "");

    cJSON *numberValue = cJSON_CreateNumber(1);
    ASSERT_NE(numberValue, nullptr);
    EXPECT_EQ(utils.ParseString(numberValue), "");
    cJSON_Delete(numberValue);

    cJSON *stringValue = cJSON_CreateString("coverage_name");
    ASSERT_NE(stringValue, nullptr);
    EXPECT_EQ(utils.ParseString(stringValue), "coverage_name");
    cJSON_Delete(stringValue);

    cJSON *itemRoots = cJSON_CreateArray();
    ASSERT_NE(itemRoots, nullptr);

    cJSON *itemWithoutPlmn = cJSON_CreateObject();
    ASSERT_NE(itemWithoutPlmn, nullptr);
    cJSON_AddItemToArray(itemRoots, itemWithoutPlmn);

    cJSON *validItem = cJSON_CreateObject();
    ASSERT_NE(validItem, nullptr);
    cJSON *plmnArray = cJSON_CreateArray();
    ASSERT_NE(plmnArray, nullptr);
    cJSON_AddItemToArray(plmnArray, cJSON_CreateString("not_a_number"));
    cJSON_AddItemToArray(plmnArray, cJSON_CreateNumber(46000));
    cJSON_AddItemToObject(validItem, "mcc_mnc_array", plmnArray);
    cJSON_AddStringToObject(validItem, "zh_CN", "coverage_zh");
    cJSON_AddItemToArray(itemRoots, validItem);

    auto nameList = utils.nameArray_;
    nameList.clear();
    utils.ParserOperatorNames(nameList, itemRoots);
    ASSERT_EQ(nameList.size(), 1U);
    EXPECT_EQ(nameList[0].zhCN, "coverage_zh");
    ASSERT_EQ(nameList[0].mccMnc.size(), 1U);
    EXPECT_EQ(nameList[0].mccMnc[0], "46000");
    cJSON_Delete(itemRoots);
}

HWTEST_F(NetworkSearchCoverageTest, Telephony_Coverage_OperatorNameInit, TestSize.Level0)
{
    OperatorNameUtils &utils = OperatorNameUtils::GetInstance();
    utils.Init();
    utils.Init();

    auto backupNameArray = utils.nameArray_;
    bool backupIsInit = utils.isInit_;
    utils.nameArray_.clear();
    utils.isInit_ = true;
    EXPECT_EQ(utils.GetCustomName("46000"), "");
    utils.nameArray_ = backupNameArray;
    utils.isInit_ = backupIsInit;
}

HWTEST_F(NetworkSearchCoverageTest, Telephony_Coverage_NitzParse, TestSize.Level0)
{
    auto telRilManager = std::make_shared<TelRilManager>();
    auto simManager = std::make_shared<SimManager>(telRilManager);
    auto networkSearchManager = std::make_shared<NetworkSearchManager>(telRilManager, simManager);
    NitzUpdate nitzUpdate(networkSearchManager, COVERAGE_SLOT_ID);
    NitzUpdate::NetworkTime networkTime;

    std::string twoDigitYear = "23/01/01,12:00:00+08,0;123456789";
    EXPECT_TRUE(nitzUpdate.NitzParse(twoDigitYear, networkTime));
    EXPECT_EQ(networkTime.year, 2023);

    std::string fourDigitYear = "2023/01/01,12:00:00-05,0;123456789";
    EXPECT_TRUE(nitzUpdate.NitzParse(fourDigitYear, networkTime));
    EXPECT_EQ(networkTime.offset, -5);

    std::string shortNitzInfo = "23/01/01,12:00:00;123456789";
    EXPECT_FALSE(nitzUpdate.NitzParse(shortNitzInfo, networkTime));

    std::string badDate = "23/01,12:00:00,0;123456789";
    EXPECT_FALSE(nitzUpdate.NitzParse(badDate, networkTime));

    std::string badYear = "202/01/01,12:00:00,0;123456789";
    EXPECT_FALSE(nitzUpdate.NitzParse(badYear, networkTime));

    std::string badTimeZone = "23/01/01,12:00:00,0;123456789";
    EXPECT_FALSE(nitzUpdate.NitzParse(badTimeZone, networkTime));

    std::string badTime = "12:00+08";
    EXPECT_FALSE(nitzUpdate.NitzTimeParse(badTime, networkTime));
}

HWTEST_F(NetworkSearchCoverageTest, Telephony_Coverage_NitzProcessUpdate, TestSize.Level0)
{
    auto telRilManager = std::make_shared<TelRilManager>();
    auto simManager = std::make_shared<SimManager>(telRilManager);
    auto networkSearchManager = std::make_shared<NetworkSearchManager>(telRilManager, simManager);
    NitzUpdate nitzUpdate(networkSearchManager, COVERAGE_SLOT_ID);

    AppExecFwk::InnerEvent::Pointer nullEvent(nullptr, nullptr);
    nitzUpdate.ProcessNitzUpdate(nullEvent);

    auto emptyString = std::make_shared<std::string>("");
    auto emptyEvent = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_NITZ_UPDATE, emptyString);
    nitzUpdate.ProcessNitzUpdate(emptyEvent);

    int64_t backupLastSystemTime = NitzUpdate::lastSystemTime_;
    NitzUpdate::lastSystemTime_ = 100000000;
    auto nitzString = std::make_shared<std::string>("23/01/01,12:00:00+08,0;123456789");
    auto nitzEvent = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_NITZ_UPDATE, nitzString);
    nitzUpdate.ProcessNitzUpdate(nitzEvent);
    NitzUpdate::lastSystemTime_ = backupLastSystemTime;
}

HWTEST_F(NetworkSearchCoverageTest, Telephony_Coverage_NetworkTypeWrite, TestSize.Level0)
{
    auto telRilManager = std::make_shared<TelRilManager>();
    auto simManager = std::make_shared<SimManager>(telRilManager);
    auto networkSearchManager = std::make_shared<NetworkSearchManager>(telRilManager, simManager);
    std::weak_ptr<NetworkSearchManager> weakManager = networkSearchManager;
    NetworkType networkType(weakManager, COVERAGE_SLOT_ID);

    int64_t unregisteredIndex = NetworkUtils::GetCallbackIndex64bit();
    auto preferredInfo = std::make_shared<PreferredNetworkTypeInfo>();
    preferredInfo->preferredNetworkType = static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_AUTO);
    preferredInfo->flag = unregisteredIndex;
    std::shared_ptr<RadioResponseInfo> responseInfo = nullptr;
    MessageParcel data;
    int64_t index = 0;
    EXPECT_TRUE(networkType.WriteGetPreferredNetworkInfo(preferredInfo, responseInfo, data, index));
    EXPECT_EQ(index, unregisteredIndex);

    int64_t callbackIndex = NetworkUtils::GetCallbackIndex64bit();
    sptr<INetworkSearchCallback> nullCallback = nullptr;
    std::shared_ptr<NetworkSearchCallbackInfo> callbackInfo =
        std::make_shared<NetworkSearchCallbackInfo>(0, nullCallback);
    ASSERT_TRUE(NetworkUtils::AddNetworkSearchCallBack(callbackIndex, callbackInfo));
    preferredInfo->flag = callbackIndex;
    TELEPHONY_EXT_WRAPPER.getPreferredNetworkExt_ = [](int32_t &networkMode) {
        networkMode = static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_AUTO);
    };
    MessageParcel callbackData;
    int64_t callbackResultIndex = 0;
    EXPECT_TRUE(networkType.WriteGetPreferredNetworkInfo(
        preferredInfo, responseInfo, callbackData, callbackResultIndex));
    TELEPHONY_EXT_WRAPPER.getPreferredNetworkExt_ = nullptr;
    NetworkUtils::RemoveCallbackFromMap(callbackIndex);

    std::shared_ptr<PreferredNetworkTypeInfo> nullPreferredInfo = nullptr;
    auto response = std::make_shared<RadioResponseInfo>();
    int64_t responseFlag = NetworkUtils::GetCallbackIndex64bit();
    response->flag = responseFlag;
    MessageParcel responseData;
    int64_t responseIndex = 0;
    EXPECT_TRUE(networkType.WriteGetPreferredNetworkInfo(
        nullPreferredInfo, response, responseData, responseIndex));
    EXPECT_EQ(responseIndex, responseFlag);
}

HWTEST_F(NetworkSearchCoverageTest, Telephony_Coverage_NetworkTypeProcess, TestSize.Level0)
{
    auto telRilManager = std::make_shared<TelRilManager>();
    auto simManager = std::make_shared<SimManager>(telRilManager);
    auto networkSearchManager = std::make_shared<NetworkSearchManager>(telRilManager, simManager);
    std::weak_ptr<NetworkSearchManager> weakManager = networkSearchManager;
    NetworkType networkType(weakManager, COVERAGE_SLOT_ID);

    int64_t getCallbackIndex = NetworkUtils::GetCallbackIndex64bit();
    sptr<INetworkSearchCallback> nullCallback = nullptr;
    std::shared_ptr<NetworkSearchCallbackInfo> getCallbackInfo =
        std::make_shared<NetworkSearchCallbackInfo>(0, nullCallback);
    ASSERT_TRUE(NetworkUtils::AddNetworkSearchCallBack(getCallbackIndex, getCallbackInfo));

    auto preferredInfo = std::make_shared<PreferredNetworkTypeInfo>();
    preferredInfo->preferredNetworkType = static_cast<int32_t>(PreferredNetworkMode::CORE_NETWORK_MODE_AUTO);
    preferredInfo->flag = getCallbackIndex;
    auto getEvent = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_GET_PREFERRED_NETWORK_MODE, preferredInfo);
    networkType.ProcessGetPreferredNetwork(getEvent);
    NetworkUtils::RemoveCallbackFromMap(getCallbackIndex);

    auto setResponse = std::make_shared<RadioResponseInfo>();
    setResponse->error = ErrType::NONE;
    setResponse->flag = NetworkUtils::GetCallbackIndex64bit();
    auto setEvent = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_SET_PREFERRED_NETWORK_MODE, setResponse);
    networkType.ProcessSetPreferredNetwork(setEvent);

    int64_t setCallbackIndex = NetworkUtils::GetCallbackIndex64bit();
    std::shared_ptr<NetworkSearchCallbackInfo> setCallbackInfo =
        std::make_shared<NetworkSearchCallbackInfo>(0, nullCallback);
    ASSERT_TRUE(NetworkUtils::AddNetworkSearchCallBack(setCallbackIndex, setCallbackInfo));
    setResponse->flag = setCallbackIndex;
    auto setEventWithCallback = AppExecFwk::InnerEvent::Get(
        RadioEvent::RADIO_SET_PREFERRED_NETWORK_MODE, setResponse);
    networkType.ProcessSetPreferredNetwork(setEventWithCallback);
    NetworkUtils::RemoveCallbackFromMap(setCallbackIndex);

    setResponse->error = ErrType::ERR_INVALID_PARAMETER;
    setResponse->flag = NetworkUtils::GetCallbackIndex64bit();
    auto failEvent = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_SET_PREFERRED_NETWORK_MODE, setResponse);
    networkType.ProcessSetPreferredNetwork(failEvent);
}
} // namespace Telephony
} // namespace OHOS