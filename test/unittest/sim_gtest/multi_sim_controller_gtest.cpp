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
#define private public
#define protected public

#include <string>
#include <unistd.h>
#include "gtest/gtest.h"
#include "sim_constant.h"
#include "sim_manager.h"
#include "core_manager_inner.h"
#include "core_service.h"
#include "core_service_client.h"
#include "enum_convert.h"

#include "multi_sim_controller.h"
#include "mock_sim_fil_manage.h"
#include "mock_multi_sim_controller.h"
#include "mock_sim_rdb_helper.h"
#include "tel_ril_manager.h"

namespace OHOS {
namespace Telephony {
using namespace testing::ext;
using namespace testing;
const int SLOT_COUNT = 2;
class MultiSimControllerTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};


void MultiSimControllerTest::SetUpTestCase() {}

void MultiSimControllerTest::TearDownTestCase() {}

void MultiSimControllerTest::SetUp() {}

void MultiSimControllerTest::TearDown() {}

HWTEST_F(MultiSimControllerTest, MultiSimControllerTest_Init_001, Function | MediumTest | Level1)
{
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    std::vector<std::shared_ptr<Telephony::SimStateManager>> simStateManager = { nullptr, nullptr };
    std::vector<std::shared_ptr<Telephony::SimFileManager>> simFileManager = { nullptr, nullptr };
    std::shared_ptr<Telephony::MultiSimController> multiSimController =
        std::make_shared<MultiSimController>(telRilManager, simStateManager, simFileManager);
    auto radioProtocolController =
        std::make_shared<RadioProtocolController>(std::weak_ptr<TelRilManager>(telRilManager));
    multiSimController->simDbHelper_ = std::make_unique<SimRdbHelper>();
    multiSimController->ResetDataShareError();
    multiSimController->radioProtocolController_ = nullptr;
    multiSimController->Init();
    multiSimController->radioProtocolController_ = radioProtocolController;
    multiSimController->GetRadioProtocol(1);
    EXPECT_EQ(multiSimController->isSetActiveSimInProgress_.size(), SIM_SLOT_COUNT);
}

HWTEST_F(MultiSimControllerTest, MultiSimControllerTest_ForgetAllData_001, Function | MediumTest | Level1)
{
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    std::vector<std::shared_ptr<Telephony::SimStateManager>> simStateManager = { nullptr, nullptr };
    std::vector<std::shared_ptr<Telephony::SimFileManager>> simFileManager = { nullptr, nullptr };
    std::shared_ptr<Telephony::MultiSimController> multiSimController =
        std::make_shared<MultiSimController>(telRilManager, simStateManager, simFileManager);
    multiSimController->simDbHelper_ = nullptr;
    bool ret = multiSimController->ForgetAllData();
    multiSimController->ResetDataShareError();
    EXPECT_FALSE(ret);
}

HWTEST_F(MultiSimControllerTest, MultiSimControllerTest_ForgetAllDataWithSlotId_001, Function | MediumTest | Level1)
{
    int slotId = -1;
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    std::vector<std::shared_ptr<Telephony::SimStateManager>> simStateManager = { nullptr, nullptr };
    std::vector<std::shared_ptr<Telephony::SimFileManager>> simFileManager = { nullptr, nullptr };
    std::shared_ptr<Telephony::MultiSimController> multiSimController =
        std::make_shared<MultiSimController>(telRilManager, simStateManager, simFileManager);
    multiSimController->simDbHelper_ = nullptr;
    bool ret = multiSimController->ForgetAllData(slotId);
    multiSimController->simDbHelper_ = std::make_unique<SimRdbHelper>();
    multiSimController->ForgetAllData(slotId);
    EXPECT_FALSE(ret);
}

HWTEST_F(MultiSimControllerTest, MultiSimControllerTest_UpdateOpKeyInfo_001, Function | MediumTest | Level1)
{
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    std::vector<std::shared_ptr<Telephony::SimStateManager>> simStateManager = { nullptr, nullptr };
    std::vector<std::shared_ptr<Telephony::SimFileManager>> simFileManager = { nullptr, nullptr };
    std::shared_ptr<Telephony::MultiSimController> multiSimController =
        std::make_shared<MultiSimController>(telRilManager, simStateManager, simFileManager);
    multiSimController->simDbHelper_ = nullptr;
    bool ret = multiSimController->UpdateOpKeyInfo();
    multiSimController->simDbHelper_ = std::make_unique<SimRdbHelper>();
    multiSimController->UpdateOpKeyInfo();
    EXPECT_TRUE(ret);
}

HWTEST_F(MultiSimControllerTest, MultiSimControllerTest_InitData_001, Function | MediumTest | Level1)
{
    int32_t slotId = DEFAULT_SIM_SLOT_ID - 1;
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    std::vector<std::shared_ptr<Telephony::SimStateManager>> simStateManager = { nullptr, nullptr };
    std::vector<std::shared_ptr<Telephony::SimFileManager>> simFileManager = { nullptr, nullptr };
    std::shared_ptr<Telephony::MultiSimController> multiSimController =
        std::make_shared<MultiSimController>(telRilManager, simStateManager, simFileManager);
    bool ret = multiSimController->InitData(slotId);
    EXPECT_FALSE(ret);
}

HWTEST_F(MultiSimControllerTest, MultiSimControllerTest_InitData_002, Function | MediumTest | Level1)
{
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    auto simStateManager0 = std::make_shared<Telephony::SimStateManager>(telRilManager);
    EventFwk::MatchingSkills matchingSkills;
    EventFwk::CommonEventSubscribeInfo subcribeInfo(matchingSkills);
    
    std::vector<std::shared_ptr<Telephony::SimStateManager>> simStateManager;
    simStateManager.push_back(simStateManager0);
    
    auto simFileManager = std::make_shared<SimFileManager>(subcribeInfo, telRilManager, simStateManager0);
    simFileManager->simFile_ = std::make_shared<SimFile>(simStateManager0);
    
    std::vector<std::shared_ptr<Telephony::SimFileManager>> simFileManager_;
    simFileManager_.push_back(simFileManager);
    
    std::shared_ptr<Telephony::MultiSimController> multiSimController =
        std::make_shared<MultiSimController>(telRilManager, simStateManager, simFileManager_);
    
    std::vector<SimRdbInfo> newCache;
    newCache.resize(2);
    newCache[0].iccId = "2164181618486135";
    newCache[1].iccId.clear();
    multiSimController->localCacheInfo_ = newCache;

    bool ret = multiSimController->InitData(0);
    EXPECT_FALSE(ret);
}

HWTEST_F(MultiSimControllerTest, MultiSimControllerTest_InitData_003, Function | MediumTest | Level1)
{
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    auto simStateManager0 = std::make_shared<Telephony::SimStateManager>(telRilManager);
    EventFwk::MatchingSkills matchingSkills;
    EventFwk::CommonEventSubscribeInfo subcribeInfo(matchingSkills);
    
    std::vector<std::shared_ptr<Telephony::SimStateManager>> simStateManager;
    simStateManager.push_back(simStateManager0);
    
    auto simFileManager = std::make_shared<SimFileManager>(subcribeInfo, telRilManager, simStateManager0);
    simFileManager->simFile_ = std::make_shared<SimFile>(simStateManager0);
    
    std::vector<std::shared_ptr<Telephony::SimFileManager>> simFileManager_;
    simFileManager_.push_back(simFileManager);
    
    std::shared_ptr<Telephony::MultiSimController> multiSimController =
        std::make_shared<MultiSimController>(telRilManager, simStateManager, simFileManager_);
    
    std::vector<SimRdbInfo> newCache;
    newCache.resize(2);
    newCache[0].iccId = "2164181618486135";
    newCache[1].iccId.clear();
    multiSimController->localCacheInfo_ = newCache;

    auto multiSimControllerMock = std::make_shared<MultiSimControllerMock>(telRilManager,
        simStateManager, simFileManager_);
    EXPECT_CALL(*multiSimControllerMock, InitIccId(0)).WillRepeatedly(Return(true));
    EXPECT_CALL(*multiSimControllerMock, IsValidData(0)).WillRepeatedly(Return(true));

    bool ret = multiSimController->InitData(0);
    EXPECT_FALSE(ret);
}

HWTEST_F(MultiSimControllerTest, MultiSimControllerTest_InitActive_001, Function | MediumTest | Level1)
{
    int32_t slotId = 0;
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    auto simStateManager0 = std::make_shared<Telephony::SimStateManager>(telRilManager);
    std::vector<std::shared_ptr<Telephony::SimStateManager>> simStateManager = { simStateManager0, nullptr };
    std::vector<std::shared_ptr<Telephony::SimFileManager>> simFileManager = { nullptr, nullptr };
    std::shared_ptr<Telephony::MultiSimController> multiSimController =
        std::make_shared<MultiSimController>(telRilManager, simStateManager, simFileManager);
    bool ret = multiSimController->InitActive(slotId);
    EXPECT_TRUE(ret);
}

HWTEST_F(MultiSimControllerTest, MultiSimControllerTest_ReCheckPrimary_001, Function | MediumTest | Level1)
{
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    auto simStateManager0 = std::make_shared<Telephony::SimStateManager>(telRilManager);
    std::vector<std::shared_ptr<Telephony::SimStateManager>> simStateManager = { simStateManager0, nullptr };
    std::vector<std::shared_ptr<Telephony::SimFileManager>> simFileManager = { nullptr, nullptr };
    std::shared_ptr<Telephony::MultiSimController> multiSimController =
        std::make_shared<MultiSimController>(telRilManager, simStateManager, simFileManager);
    multiSimController->maxCount_ = 1;
    multiSimController->ReCheckPrimary();
    EXPECT_NE(multiSimController->simStateManager_[0], nullptr);
}

HWTEST_F(MultiSimControllerTest, MultiSimControllerTest_IsAllCardsLoaded_001, Function | MediumTest | Level1)
{
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    auto simStateManager0 = std::make_shared<Telephony::SimStateManager>(telRilManager);
    std::vector<std::shared_ptr<Telephony::SimStateManager>> simStateManager = { simStateManager0, nullptr };
    std::vector<std::shared_ptr<Telephony::SimFileManager>> simFileManager = { nullptr, nullptr };
    std::shared_ptr<Telephony::MultiSimController> multiSimController =
        std::make_shared<MultiSimController>(telRilManager, simStateManager, simFileManager);
    std::vector<SimRdbInfo> newCache;
    newCache.resize(2);
    newCache[0].iccId = "2164181618486135";
    newCache[1].iccId.clear();
    multiSimController->localCacheInfo_ = newCache;
    bool ret = multiSimController->IsAllCardsLoaded();
    EXPECT_FALSE(ret);
}

HWTEST_F(MultiSimControllerTest, MultiSimControllerTest_IsAllCardsLoaded_002, Function | MediumTest | Level1)
{
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    auto simStateManager0 = std::make_shared<Telephony::SimStateManager>(telRilManager);
    std::vector<std::shared_ptr<Telephony::SimStateManager>> simStateManager = { simStateManager0, nullptr };
    std::vector<std::shared_ptr<Telephony::SimFileManager>> simFileManager = { nullptr, nullptr };
    std::shared_ptr<Telephony::MultiSimController> multiSimController =
        std::make_shared<MultiSimController>(telRilManager, simStateManager, simFileManager);
    std::vector<SimRdbInfo> newCache;
    newCache.resize(2);
    newCache[0].iccId = "2164181618486135";
    newCache[1].iccId.clear();
    multiSimController->localCacheInfo_ = newCache;
    bool ret = multiSimController->IsAllCardsLoaded();
    EXPECT_FALSE(ret);
}

HWTEST_F(MultiSimControllerTest, MultiSimControllerTest_InitIccId_001, Function | MediumTest | Level1)
{
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    auto simStateManager0 = std::make_shared<Telephony::SimStateManager>(telRilManager);
    EventFwk::MatchingSkills matchingSkills;
    EventFwk::CommonEventSubscribeInfo subcribeInfo(matchingSkills);
    
    std::vector<std::shared_ptr<Telephony::SimStateManager>> simStateManager;
    simStateManager.push_back(simStateManager0);
    
    auto simFileManager = std::make_shared<SimFileManager>(subcribeInfo, telRilManager, simStateManager0);
    simFileManager->simFile_ = std::make_shared<SimFile>(simStateManager0);
    
    std::vector<std::shared_ptr<Telephony::SimFileManager>> simFileManager_;
    simFileManager_.push_back(simFileManager);
    
    std::shared_ptr<Telephony::MultiSimController> multiSimController =
        std::make_shared<MultiSimController>(telRilManager, simStateManager, simFileManager_);
    
    std::vector<SimRdbInfo> newCache;
    newCache.resize(2);
    newCache[0].iccId = "2164181618486135";
    newCache[1].iccId.clear();
    multiSimController->localCacheInfo_ = newCache;
    
    bool ret = multiSimController->InitIccId(0);
    EXPECT_FALSE(ret);
}

HWTEST_F(MultiSimControllerTest, MultiSimControllerTest_InitIccId_002, Function | MediumTest | Level1)
{
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    auto simStateManager0 = std::make_shared<Telephony::SimStateManager>(telRilManager);
    EventFwk::MatchingSkills matchingSkills;
    EventFwk::CommonEventSubscribeInfo subcribeInfo(matchingSkills);
    
    std::vector<std::shared_ptr<Telephony::SimStateManager>> simStateManager;
    simStateManager.push_back(simStateManager0);
    
    auto simFileManager = std::make_shared<SimFileManager>(subcribeInfo, telRilManager, simStateManager0);
    simFileManager->simFile_ = std::make_shared<SimFile>(simStateManager0);
    
    std::vector<std::shared_ptr<Telephony::SimFileManager>> simFileManager_;
    simFileManager_.push_back(simFileManager);
    
    std::shared_ptr<Telephony::MultiSimController> multiSimController =
        std::make_shared<MultiSimController>(telRilManager, simStateManager, simFileManager_);
    
    std::vector<SimRdbInfo> newCache;
    newCache.resize(2);
    newCache[0].iccId = "2164181618486135";
    newCache[1].iccId.clear();
    multiSimController->localCacheInfo_ = newCache;

    auto mockSimRdbHelper = std::make_shared<MockSimRdbHelper>();
    EXPECT_CALL(*mockSimRdbHelper, QueryDataByIccId(_, _)).WillRepeatedly(Return(INVALID_VALUE));
    multiSimController->simDbHelper_ = std::make_unique<SimRdbHelper>();
    bool ret = multiSimController->InitIccId(0);
    EXPECT_FALSE(ret);
}

HWTEST_F(MultiSimControllerTest, MultiSimControllerTest_InitIccId_003, Function | MediumTest | Level1)
{
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    auto simStateManager0 = std::make_shared<Telephony::SimStateManager>(telRilManager);
    EventFwk::MatchingSkills matchingSkills;
    EventFwk::CommonEventSubscribeInfo subcribeInfo(matchingSkills);
    
    std::vector<std::shared_ptr<Telephony::SimStateManager>> simStateManager;
    simStateManager.push_back(simStateManager0);
    
    auto simFileManager = std::make_shared<SimFileManager>(subcribeInfo, telRilManager, simStateManager0);
    simFileManager->simFile_ = std::make_shared<SimFile>(simStateManager0);
    
    std::vector<std::shared_ptr<Telephony::SimFileManager>> simFileManager_;
    simFileManager_.push_back(simFileManager);
    
    std::shared_ptr<Telephony::MultiSimController> multiSimController =
        std::make_shared<MultiSimController>(telRilManager, simStateManager, simFileManager_);
    
    std::vector<SimRdbInfo> newCache;
    newCache.resize(2);
    newCache[0].iccId = "2164181618486135";
    newCache[1].iccId.clear();
    multiSimController->localCacheInfo_ = newCache;

    auto mockSimRdbHelper = std::make_shared<MockSimRdbHelper>();
    EXPECT_CALL(*mockSimRdbHelper, QueryDataByIccId(_, _)).WillRepeatedly(Return(TELEPHONY_SUCCESS));
    EXPECT_CALL(*mockSimRdbHelper, UpdateDataByIccId(_, _)).WillRepeatedly(Return(INVALID_VALUE));
    multiSimController->simDbHelper_ = std::make_unique<SimRdbHelper>();
    bool ret = multiSimController->InitIccId(0);
    EXPECT_FALSE(ret);
}

HWTEST_F(MultiSimControllerTest, MultiSimControllerTest_GetShowNumber_001, Function | MediumTest | Level1)
{
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    auto simStateManager0 = std::make_shared<Telephony::SimStateManager>(telRilManager);
    simStateManager0->Init(0);
    simStateManager0->simStateHandle_->iccState_.simStatus_ = ICC_CONTENT_READY;
 
    auto simStateManager1 = std::make_shared<Telephony::SimStateManager>(telRilManager);
    simStateManager1->Init(1);
    simStateManager1->simStateHandle_->iccState_.simStatus_ = ICC_CONTENT_READY;
 
    std::vector<std::shared_ptr<Telephony::SimStateManager>> simStateManager_ = { simStateManager0, simStateManager1 };
    EventFwk::MatchingSkills matchingSkills;
    matchingSkills.AddEvent(CommonEventSupport::COMMON_EVENT_OPERATOR_CONFIG_CHANGED);
    EventFwk::CommonEventSubscribeInfo subcribeInfo(matchingSkills);
    auto simFileManager = std::make_shared<SimFileManager>(subcribeInfo, telRilManager, simStateManager0);
    simFileManager->simFile_ = std::make_shared<SimFile>(simStateManager0);
    simFileManager->simFile_->msisdn_ = "2164181618486135";
    std::vector<std::shared_ptr<Telephony::SimFileManager>> simFileManager_ = { simFileManager, nullptr };
    std::shared_ptr<Telephony::MultiSimController> multiSimController =
        std::make_shared<MultiSimController>(telRilManager, simStateManager_, simFileManager_);
    multiSimController->maxCount_ = 2;
    multiSimController->Init();

    std::vector<SimRdbInfo> newCache;
    newCache.resize(1);
    std::u16string showNumber = u"";
    newCache[0].phoneNumber = "2164181618486135";
    multiSimController->simStateManager_ = { simStateManager0, simStateManager1 };
    multiSimController->localCacheInfo_ = newCache;
    int32_t result = multiSimController->GetShowNumber(0, showNumber);
    EXPECT_EQ(result, TELEPHONY_ERR_SUCCESS);
    result = multiSimController->GetShowNumber(1, showNumber);
 
    newCache[0].phoneNumber = "2164181618486139";
    multiSimController->localCacheInfo_ = newCache;
    result = multiSimController->GetShowNumber(0, showNumber);
    EXPECT_EQ(result, TELEPHONY_ERR_SUCCESS);
}

HWTEST_F(MultiSimControllerTest, MultiSimControllerTest_UpdateDataByIccId_001, Function | MediumTest | Level1)
{
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    std::vector<std::shared_ptr<Telephony::SimStateManager>> simStateManager = { nullptr, nullptr };
    std::vector<std::shared_ptr<Telephony::SimFileManager>> simFileManager = { nullptr, nullptr };
    std::shared_ptr<Telephony::MultiSimController> multiSimController =
        std::make_shared<MultiSimController>(telRilManager, simStateManager, simFileManager);
    auto radioProtocolController =
        std::make_shared<RadioProtocolController>(std::weak_ptr<TelRilManager>(telRilManager));
    radioProtocolController->Init();
    multiSimController->simDbHelper_ = nullptr;

    multiSimController->maxCount_ = 2;
    multiSimController->Init();
    auto ret = multiSimController->UpdateDataByIccId(0, "000011111");

    multiSimController->simDbHelper_ = std::make_unique<SimRdbHelper>();
    multiSimController->UpdateDataByIccId(1, "000011111");

    EXPECT_EQ(ret, INVALID_VALUE);
}

HWTEST_F(MultiSimControllerTest, MultiSimControllerTest_InsertData_001, Function | MediumTest | Level1)
{
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    std::vector<std::shared_ptr<Telephony::SimStateManager>> simStateManager = { nullptr, nullptr };
    std::vector<std::shared_ptr<Telephony::SimFileManager>> simFileManager = { nullptr, nullptr };
    std::shared_ptr<Telephony::MultiSimController> multiSimController =
        std::make_shared<MultiSimController>(telRilManager, simStateManager, simFileManager);
    auto radioProtocolController =
        std::make_shared<RadioProtocolController>(std::weak_ptr<TelRilManager>(telRilManager));
    multiSimController->simDbHelper_ = nullptr;

    multiSimController->maxCount_ = 2;
    multiSimController->Init();
    auto ret = multiSimController->InsertData(0, "000011111");

    multiSimController->simDbHelper_ = std::make_unique<SimRdbHelper>();
    multiSimController->InsertData(1, "000011111");

    multiSimController->UpdateSubState(0, 1);

    EXPECT_EQ(ret, INVALID_VALUE);
}

HWTEST_F(MultiSimControllerTest, MultiSimControllerTest_SetPrimarySlotId_001, Function | MediumTest | Level1)
{
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    std::vector<std::shared_ptr<Telephony::SimStateManager>> simStateManager = { nullptr, nullptr };
    std::vector<std::shared_ptr<Telephony::SimFileManager>> simFileManager = { nullptr, nullptr };
    std::shared_ptr<Telephony::MultiSimController> multiSimController =
        std::make_shared<MultiSimController>(telRilManager, simStateManager, simFileManager);
    auto radioProtocolController =
        std::make_shared<RadioProtocolController>(std::weak_ptr<TelRilManager>(telRilManager));
    multiSimController->simDbHelper_ = nullptr;

    multiSimController->maxCount_ = 2;
    multiSimController->Init();
    auto ret = multiSimController->SetPrimarySlotId(0, false);

    multiSimController->simDbHelper_ = std::make_unique<SimRdbHelper>();
    multiSimController->SetPrimarySlotId(1, false);

    EXPECT_EQ(ret, TELEPHONY_ERR_NO_SIM_CARD);

    multiSimController->isRilSetPrimarySlotSupport_ = true;
    multiSimController->SetPrimarySlotId(1, true);
    EXPECT_EQ(ret, TELEPHONY_ERR_NO_SIM_CARD);
}

HWTEST_F(MultiSimControllerTest, MultiSimControllerTest_SetPrimarySlotId_002, Function | MediumTest | Level1)
{
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    std::vector<std::shared_ptr<Telephony::SimStateManager>> simStateManager = { nullptr, nullptr };
    std::vector<std::shared_ptr<Telephony::SimFileManager>> simFileManager = { nullptr, nullptr };
    std::shared_ptr<Telephony::MultiSimController> multiSimController =
        std::make_shared<MultiSimController>(telRilManager, simStateManager, simFileManager);
    auto radioProtocolController =
        std::make_shared<RadioProtocolController>(std::weak_ptr<TelRilManager>(telRilManager));
    multiSimController->simDbHelper_ = nullptr;
    multiSimController->maxCount_ = 2;
    multiSimController->Init();
    multiSimController->isRilSetPrimarySlotSupport_ = true;
    multiSimController->isSettingPrimarySlotToRil_ = true;
    EXPECT_FALSE(multiSimController->SetPrimarySlotToRil(0));

    multiSimController->isSettingPrimarySlotToRil_ = false;
    telRilManager = nullptr;
    multiSimController->SetPrimarySlotToRil(0);
    multiSimController->SendSetPrimarySlotEvent(0);
}

HWTEST_F(MultiSimControllerTest, MultiSimControllerTest_ProcessEvent_001, Function | MediumTest | Level1)
{
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    std::vector<std::shared_ptr<Telephony::SimStateManager>> simStateManager = { nullptr, nullptr };
    std::vector<std::shared_ptr<Telephony::SimFileManager>> simFileManager = { nullptr, nullptr };
    std::shared_ptr<Telephony::MultiSimController> multiSimController =
        std::make_shared<MultiSimController>(telRilManager, simStateManager, simFileManager);
    auto radioProtocolController =
        std::make_shared<RadioProtocolController>(std::weak_ptr<TelRilManager>(telRilManager));
    multiSimController->simDbHelper_ = nullptr;

    multiSimController->maxCount_ = 2;
    multiSimController->Init();
    multiSimController->isRilSetPrimarySlotSupport_ = true;
    AppExecFwk::InnerEvent::Pointer event1 = AppExecFwk::InnerEvent::
        Get(RADIO_SIM_SET_PRIMARY_SLOT, 0);
    multiSimController->ProcessEvent(event1);
    AppExecFwk::InnerEvent::Pointer event2 = AppExecFwk::InnerEvent::
        Get(RADIO_SIM_SET_PRIMARY_SLOT, 0);
    multiSimController->ProcessEvent(event2);
    auto nullEvent = AppExecFwk::InnerEvent::Pointer(nullptr, nullptr);
    multiSimController->OnRilSetPrimarySlotDone(nullEvent);
    multiSimController->OnRilSetPrimarySlotTimeout(nullEvent);
    EXPECT_FALSE(multiSimController->setPrimarySlotResponseResult_);
}

HWTEST_F(MultiSimControllerTest, MultiSimControllerTest_SendMainCardBroadCast_001, Function | MediumTest | Level1)
{
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    std::vector<std::shared_ptr<Telephony::SimStateManager>> simStateManager = { nullptr, nullptr };
    std::vector<std::shared_ptr<Telephony::SimFileManager>> simFileManager = { nullptr, nullptr };
    std::shared_ptr<Telephony::MultiSimController> multiSimController =
        std::make_shared<MultiSimController>(telRilManager, simStateManager, simFileManager);
    auto radioProtocolController =
        std::make_shared<RadioProtocolController>(std::weak_ptr<TelRilManager>(telRilManager));
    multiSimController->simDbHelper_ = nullptr;

    multiSimController->maxCount_ = 2;
    multiSimController->Init();
    multiSimController->SendMainCardBroadCast(1);

    std::vector<SimRdbInfo> newCache;
    newCache.resize(2);
    newCache[0].iccId = "2164181618486135";
    newCache[1].iccId.clear();
    multiSimController->localCacheInfo_ = newCache;
    multiSimController->ResetSetPrimarySlotRemain(0);
    multiSimController->ResetSetPrimarySlotRemain(-1);
    multiSimController->ResetSetPrimarySlotRemain(5);
    multiSimController->SendMainCardBroadCast(0);
    multiSimController->SendDefaultCellularDataBroadCast(0);
    EXPECT_EQ(multiSimController->localCacheInfo_.size(), 2);
}

HWTEST_F(MultiSimControllerTest, MultiSimControllerTest_GetSimTelephoneNumber_001, Function | MediumTest | Level1)
{
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    auto simStateManager0 = std::make_shared<Telephony::SimStateManager>(telRilManager);
    simStateManager0->Init(0);
    simStateManager0->simStateHandle_->iccState_.simStatus_ = ICC_CONTENT_READY;
 
    auto simStateManager1 = std::make_shared<Telephony::SimStateManager>(telRilManager);
    simStateManager1->Init(1);
    simStateManager1->simStateHandle_->iccState_.simStatus_ = ICC_CONTENT_READY;
 
    std::vector<std::shared_ptr<Telephony::SimStateManager>> simStateManager_ = { simStateManager0, simStateManager1 };
    EventFwk::MatchingSkills matchingSkills;
    matchingSkills.AddEvent(CommonEventSupport::COMMON_EVENT_OPERATOR_CONFIG_CHANGED);
    EventFwk::CommonEventSubscribeInfo subcribeInfo(matchingSkills);
    auto simFileManager = std::make_shared<SimFileManager>(subcribeInfo, telRilManager, simStateManager0);
    simFileManager->simFile_ = std::make_shared<SimFile>(simStateManager0);
    simFileManager->simFile_->msisdn_ = "2164181618486135";
    std::vector<std::shared_ptr<Telephony::SimFileManager>> simFileManager_ = { simFileManager, nullptr };
    std::shared_ptr<Telephony::MultiSimController> multiSimController =
        std::make_shared<MultiSimController>(telRilManager, simStateManager_, simFileManager_);
    multiSimController->maxCount_ = 2;
    multiSimController->Init();

    std::vector<SimRdbInfo> newCache;
    newCache.resize(1);
    std::u16string showNumber = u"";
    newCache[0].phoneNumber = "2164181618486135";
    multiSimController->simStateManager_ = { simStateManager0, simStateManager1 };
    multiSimController->localCacheInfo_ = newCache;
    int32_t result = multiSimController->GetSimTelephoneNumber(0, showNumber);
    EXPECT_EQ(result, TELEPHONY_ERR_SUCCESS);
    result = multiSimController->GetSimTelephoneNumber(1, showNumber);
 
    newCache[0].phoneNumber = "2164181618486139";
    multiSimController->localCacheInfo_ = newCache;
    result = multiSimController->GetSimTelephoneNumber(0, showNumber);
    EXPECT_EQ(result, TELEPHONY_ERR_SUCCESS);
}

HWTEST_F(MultiSimControllerTest, BuildRadioProtocolForCommunication_001, Function | MediumTest | Level1)
{
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    std::vector<std::shared_ptr<Telephony::SimStateManager>> simStateManager = { nullptr, nullptr };
    std::vector<std::shared_ptr<Telephony::SimFileManager>> simFileManager = { nullptr, nullptr };
    std::shared_ptr<Telephony::MultiSimController> multiSimController =
        std::make_shared<MultiSimController>(telRilManager, simStateManager, simFileManager);
    auto radioProtocolController =
        std::make_shared<RadioProtocolController>(std::weak_ptr<TelRilManager>(telRilManager));
    radioProtocolController->slotCount_ = 2;
    radioProtocolController->Init();
    radioProtocolController->BuildRadioProtocolForCommunication(
        RadioProtocolPhase::RADIO_PROTOCOL_PHASE_CHECK, RadioProtocolStatus::RADIO_PROTOCOL_STATUS_FAIL);
    EXPECT_EQ(multiSimController->localCacheInfo_.size(), 0);
}

HWTEST_F(MultiSimControllerTest, WhenForgetAllDataReturnsValidValue, Function | MediumTest | Level1)
{   
    MockSimRdbHelper mockSimRdbHelper;
    EXPECT_CALL(mockSimRdbHelper, ForgetAllData())
        .WillRepeatedly(Return(1));
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    std::vector<std::shared_ptr<Telephony::SimStateManager>> simStateManager = { nullptr, nullptr };
    std::vector<std::shared_ptr<Telephony::SimFileManager>> simFileManager = { nullptr, nullptr };
    std::shared_ptr<Telephony::MultiSimController> multiSimController =
        std::make_shared<MultiSimController>(telRilManager, simStateManager, simFileManager);
    EXPECT_FALSE(multiSimController->ForgetAllData());
}

HWTEST_F(MultiSimControllerTest, WhenForgetAllDataReturnsInvalidValue, Function | MediumTest | Level1)
{      
    MockSimRdbHelper mockSimRdbHelper;
    EXPECT_CALL(mockSimRdbHelper, ForgetAllData())
        .WillRepeatedly(Return(0));
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    std::vector<std::shared_ptr<Telephony::SimStateManager>> simStateManager = { nullptr, nullptr };
    std::vector<std::shared_ptr<Telephony::SimFileManager>> simFileManager = { nullptr, nullptr };
    std::shared_ptr<Telephony::MultiSimController> multiSimController =
        std::make_shared<MultiSimController>(telRilManager, simStateManager, simFileManager);
    EXPECT_FALSE(multiSimController->ForgetAllData());
}

HWTEST_F(MultiSimControllerTest, MultiSimControllerTest_SetPrimarySlotId_003, Function | MediumTest | Level1)
{
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    telRilManager->OnInit();
    std::vector<std::shared_ptr<Telephony::SimStateManager>> simStateManager;
    std::vector<std::shared_ptr<Telephony::SimFileManager>> simFileManager;
    simStateManager.resize(SLOT_COUNT);
    simFileManager.resize(SLOT_COUNT);
    for (int32_t slotId = 0; slotId < SLOT_COUNT; slotId++) {
        simStateManager[slotId] = std::make_shared<SimStateManager>(telRilManager);
        simStateManager[slotId]->Init(slotId);
        simStateManager[slotId]->simStateHandle_->externalState_ = SimState::SIM_STATE_READY;
        simStateManager[slotId]->simStateHandle_->externalType_ = CardType::SINGLE_MODE_USIM_CARD;
        simFileManager[slotId] = SimFileManager::CreateInstance(
            std::weak_ptr<ITelRilManager>(telRilManager), std::weak_ptr<SimStateManager>(simStateManager[slotId]));
        simFileManager[slotId]->Init(slotId);
    }
    std::shared_ptr<MultiSimController> multiSimController =
        std::make_shared<MultiSimController>(telRilManager, simStateManager, simFileManager);
    multiSimController->Init();
    auto ret = multiSimController->SetPrimarySlotId(2, false);
    EXPECT_EQ(ret, TELEPHONY_ERR_NO_SIM_CARD);
    IccState iccState;
    iccState.iccid_ = "012345678901234";
    iccState.simStatus_ = ICC_CONTENT_READY;
    iccState.simType_ = ICC_USIM_TYPE;
    multiSimController->simStateManager_[0]->simStateHandle_->iccState_ = iccState;
    multiSimController->simStateManager_[0]->simStateHandle_->iccid_ = "012345678901234";
    ret = multiSimController->SetPrimarySlotId(0, false);
    EXPECT_EQ(ret, TELEPHONY_ERR_SUCCESS);
    multiSimController->radioProtocolController_->radioProtocol_[1].modemId = 1;
    multiSimController->radioProtocolController_->isCommunicating_ = true;
    multiSimController->simStateManager_[1]->simStateHandle_->iccState_ = iccState;
    multiSimController->simStateManager_[1]->simStateHandle_->iccid_ = "012345678901234";
    ret = multiSimController->SetPrimarySlotId(1, false);
    multiSimController->setPrimarySlotRemainCount_[1] = 0;
    EXPECT_EQ(ret, TELEPHONY_ERR_LOCAL_PTR_NULL);
    multiSimController->radioProtocolController_->isCommunicating_ = false;
    ret = multiSimController->SetPrimarySlotId(1, false);
    EXPECT_EQ(ret, TELEPHONY_ERR_SUCCESS);
    multiSimController->radioProtocolController_->radioProtocol_[0].modemId = 1;
    ret = multiSimController->SetPrimarySlotId(0, false);
}
}
}
