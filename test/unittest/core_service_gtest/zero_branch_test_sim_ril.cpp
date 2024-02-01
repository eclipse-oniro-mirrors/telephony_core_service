/*
 * Copyright (C) 2023 Huawei Device Co., Ltd.
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
#include <gtest/gtest.h>
#include <string_ex.h>

#include "core_service.h"
#include "icc_dialling_numbers_handler.h"
#include "icc_dialling_numbers_manager.h"
#include "icc_file_controller.h"
#include "icc_operator_privilege_controller.h"
#include "mcc_pool.h"
#include "operator_config_cache.h"
#include "operator_config_loader.h"
#include "parcel.h"
#include "plmn_file.h"
#include "sim_account_manager.h"
#include "sim_data_type.h"
#include "sim_file_controller.h"
#include "sim_manager.h"
#include "sim_rdb_helper.h"
#include "telephony_log_wrapper.h"
#include "usim_dialling_numbers_service.h"
#include "want.h"

namespace OHOS {
namespace Telephony {
using namespace testing::ext;

namespace {
constexpr int32_t SLOT_ID = 0;
constexpr int INVALID_MCC = 100;
const int32_t INVALID_SLOTID = 2;
} // namespace

class DemoHandler : public AppExecFwk::EventHandler {
public:
    explicit DemoHandler(std::shared_ptr<AppExecFwk::EventRunner> &runner) : AppExecFwk::EventHandler(runner) {}
    virtual ~DemoHandler() {}
    void ProcessEvent(const AppExecFwk::InnerEvent::Pointer &event) {}
};

class SimRilBranchTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};
void SimRilBranchTest::SetUpTestCase() {}

void SimRilBranchTest::TearDownTestCase() {}

void SimRilBranchTest::SetUp() {}

void SimRilBranchTest::TearDown() {}

/**
 * @tc.number   Telephony_IccOperatorPrivilegeController_001
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(SimRilBranchTest, Telephony_IccOperatorPrivilegeController_001, Function | MediumTest | Level1)
{
    auto telRilManager = std::make_shared<TelRilManager>();
    auto simStateManager = std::make_shared<SimStateManager>(telRilManager);
    auto iccOperatorPrivilegeController =
        std::make_shared<IccOperatorPrivilegeController>(telRilManager, simStateManager);
    auto iccOperatorPrivilegeController2 = std::make_shared<IccOperatorPrivilegeController>(nullptr, simStateManager);
    auto iccOperatorPrivilegeController3 = std::make_shared<IccOperatorPrivilegeController>(telRilManager, nullptr);
    iccOperatorPrivilegeController->Init(0);
    iccOperatorPrivilegeController2->Init(0);
    iccOperatorPrivilegeController3->Init(0);
    int slotId = 2;
    iccOperatorPrivilegeController->Init(slotId);
    std::shared_ptr<AppExecFwk::EventHandler> handler;
    bool hasOperatorPrivileges = false;
    AppExecFwk::InnerEvent::Pointer event = AppExecFwk::InnerEvent::Get(1, 1);
    event = nullptr;
    iccOperatorPrivilegeController->ProcessEvent(event);
    iccOperatorPrivilegeController->ProcessEvent(AppExecFwk::InnerEvent::Get(-1));
    constexpr std::string_view certHash = " test ";
    constexpr std::string_view packageName = "  ";
    iccOperatorPrivilegeController->HasOperatorPrivileges(certHash, packageName, hasOperatorPrivileges);
    iccOperatorPrivilegeController->ProcessEvent(AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_SIM_STATE_CHANGE));
    iccOperatorPrivilegeController->ProcessEvent(
        AppExecFwk::InnerEvent::Get(IccOperatorPrivilegeController::MSG_OPEN_LOGICAL_CHANNEL_DONE, 1));
    auto resultPtr = std::make_shared<OpenLogicalChannelResponse>();
    resultPtr->channelId = 0;
    iccOperatorPrivilegeController->ProcessEvent(
        AppExecFwk::InnerEvent::Get(IccOperatorPrivilegeController::MSG_OPEN_LOGICAL_CHANNEL_DONE, resultPtr));
    resultPtr->channelId = 1;
    iccOperatorPrivilegeController->ProcessEvent(
        AppExecFwk::InnerEvent::Get(IccOperatorPrivilegeController::MSG_OPEN_LOGICAL_CHANNEL_DONE, resultPtr));
    iccOperatorPrivilegeController->ProcessEvent(
        AppExecFwk::InnerEvent::Get(IccOperatorPrivilegeController::MSG_TRANSMIT_LOGICAL_CHANNEL_DONE, 1));
    auto resultPtr1 = std::make_shared<IccIoResultInfo>();
    iccOperatorPrivilegeController->ProcessEvent(
        AppExecFwk::InnerEvent::Get(IccOperatorPrivilegeController::MSG_TRANSMIT_LOGICAL_CHANNEL_DONE, resultPtr1));
    iccOperatorPrivilegeController->ProcessEvent(
        AppExecFwk::InnerEvent::Get(IccOperatorPrivilegeController::MSG_CLOSE_LOGICAL_CHANNEL_DONE));
    iccOperatorPrivilegeController->OpenChannel();
    EXPECT_GT(iccOperatorPrivilegeController->HasOperatorPrivileges(hasOperatorPrivileges), TELEPHONY_ERR_SUCCESS);
}

/**
 * @tc.number   Telephony_SimAccountManager_001
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(SimRilBranchTest, Telephony_SimAccountManager_001, Function | MediumTest | Level1)
{
    auto telRilManager = std::make_shared<TelRilManager>();
    auto simStateManager = std::make_shared<SimStateManager>(telRilManager);
    EventFwk::MatchingSkills matchingSkills;
    matchingSkills.AddEvent(CommonEventSupport::COMMON_EVENT_OPERATOR_CONFIG_CHANGED);
    EventFwk::CommonEventSubscribeInfo subcribeInfo(matchingSkills);
    auto simFileManager = std::make_shared<SimFileManager>(subcribeInfo, telRilManager, simStateManager);
    auto simAccountManager = std::make_shared<SimAccountManager>(telRilManager, simStateManager, simFileManager);
    simAccountManager->Init(0);
    simAccountManager->Init(-1);
    auto simAccountManager1 = std::make_shared<SimAccountManager>(nullptr, simStateManager, simFileManager);
    simAccountManager1->Init(0);
    OperatorConfig operatorConfig;
    simAccountManager->GetOperatorConfigs(0, operatorConfig);
    simAccountManager->operatorConfigCache_ = nullptr;
    EXPECT_GE(simAccountManager->GetOperatorConfigs(0, operatorConfig), TELEPHONY_ERR_SUCCESS);
    int32_t errorId = -2;
    EXPECT_FALSE(simAccountManager->IsValidSlotIdForDefault(errorId));
    EXPECT_TRUE(simAccountManager->IsValidSlotIdForDefault(0));
    bool hasOperatorPrivileges = false;
    simAccountManager->HasOperatorPrivileges(0, hasOperatorPrivileges);
    simAccountManager->privilegeController_ = nullptr;
    simAccountManager->HasOperatorPrivileges(0, hasOperatorPrivileges);
    simAccountManager->telRilManager_ = nullptr;
    EXPECT_GE(simAccountManager->HasOperatorPrivileges(0, hasOperatorPrivileges), TELEPHONY_ERR_SUCCESS);
    simAccountManager->privilegesRunner_ = nullptr;
    EXPECT_GE(simAccountManager->HasOperatorPrivileges(0, hasOperatorPrivileges), TELEPHONY_ERR_SUCCESS);
}

/**
 * @tc.number   Telephony_OperatorConfigLoader_001
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(SimRilBranchTest, Telephony_OperatorConfigLoader_001, Function | MediumTest | Level1)
{
    auto telRilManager = std::make_shared<TelRilManager>();
    auto simStateManager = std::make_shared<SimStateManager>(telRilManager);
    EventFwk::MatchingSkills matchingSkills;
    matchingSkills.AddEvent(CommonEventSupport::COMMON_EVENT_OPERATOR_CONFIG_CHANGED);
    EventFwk::CommonEventSubscribeInfo subcribeInfo(matchingSkills);
    auto simFileManager = std::make_shared<SimFileManager>(subcribeInfo, telRilManager, simStateManager);
    auto operatorConfigCache = std::make_shared<OperatorConfigCache>(simFileManager, 0);
    auto operatorConfigLoader = std::make_shared<OperatorConfigLoader>(simFileManager, operatorConfigCache);
    operatorConfigLoader->LoadOperatorConfig(0);
    operatorConfigLoader->operatorConfigCache_ = nullptr;
    operatorConfigLoader->LoadOperatorConfig(0);
    operatorConfigLoader->LoadOpKeyOnMccMnc(0);
    simFileManager = nullptr;
    operatorConfigLoader->simFileManager_ = simFileManager;
    EXPECT_EQ(operatorConfigLoader->LoadOpKeyOnMccMnc(0), DEFAULT_OPERATOR_KEY);
    EXPECT_EQ(operatorConfigLoader->GetOpKey(nullptr, 0), DEFAULT_OPERATOR_KEY);
    std::shared_ptr<DataShare::DataShareResultSet> resultSet = nullptr;
    EXPECT_FALSE(operatorConfigLoader->MatchOperatorRule(resultSet, 0));
}

/**
 * @tc.number   Telephony_StkManager_001
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(SimRilBranchTest, Telephony_StkManager_001, Function | MediumTest | Level1)
{
    auto telRilManager = std::make_shared<TelRilManager>();
    auto simStateManager = std::make_shared<SimStateManager>(telRilManager);
    std::string cmd = "";
    auto stkManager = std::make_shared<StkManager>(nullptr, simStateManager);
    stkManager->Init(0);
    stkManager->stkController_ = nullptr;
    EXPECT_EQ(stkManager->SendEnvelopeCmd(0, cmd), TELEPHONY_ERR_LOCAL_PTR_NULL);
    EXPECT_EQ(stkManager->SendTerminalResponseCmd(0, cmd), TELEPHONY_ERR_LOCAL_PTR_NULL);
    EXPECT_EQ(stkManager->SendCallSetupRequestResult(0, false), TELEPHONY_ERR_LOCAL_PTR_NULL);
}

/**
 * @tc.number   Telephony_StkController_002
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(SimRilBranchTest, Telephony_StkController_002, Function | MediumTest | Level1)
{
    std::string name = "StkController_";
    AppExecFwk::InnerEvent::Pointer event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_STK_CALL_SETUP, 1);
    std::shared_ptr<TelRilManager> telRilManager = nullptr;
    std::shared_ptr<Telephony::SimStateManager> simStateManager = std::make_shared<SimStateManager>(telRilManager);
    auto stkController = std::make_shared<StkController>(telRilManager, simStateManager, INVALID_SLOTID);
    std::string bundleNameEmpty = "";
    std::string bundleName = "123";
    EXPECT_FALSE(stkController->CheckIsSystemApp(bundleNameEmpty));
    EXPECT_FALSE(stkController->CheckIsSystemApp(bundleName));
    AAFwk::Want want;
    EXPECT_FALSE(stkController->PublishStkEvent(want));
}

/**
 * @tc.number   Telephony_SimStateTracker_001
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(SimRilBranchTest, Telephony_SimStateTracker_001, Function | MediumTest | Level1)
{
    auto telRilManager = std::make_shared<TelRilManager>();
    auto simStateManager = std::make_shared<SimStateManager>(telRilManager);
    EventFwk::MatchingSkills matchingSkills;
    matchingSkills.AddEvent(CommonEventSupport::COMMON_EVENT_OPERATOR_CONFIG_CHANGED);
    EventFwk::CommonEventSubscribeInfo subcribeInfo(matchingSkills);
    auto simFileManager = std::make_shared<SimFileManager>(subcribeInfo, telRilManager, simStateManager);
    auto operatorConfigCache = std::make_shared<OperatorConfigCache>(simFileManager, 0);
    auto simStateTracker =
        std::make_shared<SimStateTracker>(simFileManager, operatorConfigCache, 0);
    AppExecFwk::InnerEvent::Pointer event = AppExecFwk::InnerEvent::Get(1, 1);
    event = nullptr;
    simStateTracker->ProcessEvent(event);
    simFileManager = nullptr;
    simStateTracker->simFileManager_ = simFileManager;
    std::shared_ptr<OperatorConfigLoader> operatorConfigLoader_ = nullptr;
    auto statusChangeListener_ =
        new (std::nothrow) SimStateTracker::SystemAbilityStatusChangeListener(0, operatorConfigLoader_);
    statusChangeListener_->OnAddSystemAbility(0, "test");
    EXPECT_FALSE(simStateTracker->RegisterForIccLoaded());
    EXPECT_FALSE(simStateTracker->UnRegisterForIccLoaded());
}

/**
 * @tc.number   Telephony_PlmnFile_001
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(SimRilBranchTest, Telephony_PlmnFile_001, Function | MediumTest | Level1)
{
    std::string plmn = "plmntest";
    int32_t accessTechs = 8;
    Parcel parcel;
    auto plmnFile = std::make_shared<PlmnFile>(plmn, accessTechs);
    plmnFile->Marshalling(parcel);
    EXPECT_TRUE(plmnFile->ReadFromParcel(parcel));
}

/**
 * @tc.number   Telephony_IccDiallingNumbersCache_001
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(SimRilBranchTest, Telephony_IccDiallingNumbersCache_001, Function | MediumTest | Level1)
{
    auto telRilManager = std::make_shared<TelRilManager>();
    auto simStateManager = std::make_shared<SimStateManager>(telRilManager);
    EventFwk::MatchingSkills matchingSkills;
    matchingSkills.AddEvent(CommonEventSupport::COMMON_EVENT_OPERATOR_CONFIG_CHANGED);
    
    EventFwk::CommonEventSubscribeInfo subcribeInfo(matchingSkills);
    auto simFileManager = std::make_shared<SimFileManager>(subcribeInfo, telRilManager, simStateManager);
    auto diallingNumbersCache = std::make_shared<IccDiallingNumbersCache>(simFileManager);
    diallingNumbersCache->simFileManager_ = nullptr;
    diallingNumbersCache->Init();
    AppExecFwk::InnerEvent::Pointer event = AppExecFwk::InnerEvent::Get(1, 1);
    event = nullptr;
    diallingNumbersCache->SendExceptionResult(event, 0);
    diallingNumbersCache->ProcessEvent(event);
    diallingNumbersCache->ProcessEvent(AppExecFwk::InnerEvent::Get(MSG_SIM_OBTAIN_ADN_DETAILS_DONE, 1));
    diallingNumbersCache->ProcessEvent(AppExecFwk::InnerEvent::Get(MSG_SIM_CHANGE_DIALLING_NUMBERS_DONE, 1));
    diallingNumbersCache->ProcessEvent(AppExecFwk::InnerEvent::Get(MSG_SIM_OBTAIN_PBR_DETAILS_DONE, 1));
    std::shared_ptr<DiallingNumbersInfo> telNumber = std::make_shared<DiallingNumbersInfo>();
    telNumber = nullptr;
    EXPECT_FALSE(diallingNumbersCache->IsDiallingNumberEqual(telNumber, telNumber));
}

/**
 * @tc.number   Telephony_IccDiallingNumbersHandler_001
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(SimRilBranchTest, Telephony_IccDiallingNumbersHandler_001, Function | MediumTest | Level1)
{
    std::shared_ptr<AppExecFwk::EventRunner> runner = AppExecFwk::EventRunner::Create("test");
    std::shared_ptr<IccFileController> iccFileController = std::make_shared<SimFileController>(1);
    auto diallingNumberHandler = std::make_shared<IccDiallingNumbersHandler>(iccFileController);
    diallingNumberHandler->fileController_ = nullptr;
    AppExecFwk::InnerEvent::Pointer event = AppExecFwk::InnerEvent::Get(1, 1);
    DiallingNumberUpdateInfor infor;
    diallingNumberHandler->GetDiallingNumbers(0, 0, 0, event);
    diallingNumberHandler->GetAllDiallingNumbers(0, 0, event);
    diallingNumberHandler->UpdateDiallingNumbers(infor, event);
    event = nullptr;
    diallingNumberHandler->ProcessEvent(event);
    int id;
    diallingNumberHandler->ProcessLinearSizeDone(event, id);
    diallingNumberHandler->ProcessUpdateRecordDone(event, id);
    diallingNumberHandler->ProcessDiallingNumberAllLoadDone(event, id);
    std::shared_ptr<DiallingNumberLoadRequest> loadRequest = nullptr;
    std::shared_ptr<MultiRecordResult> object = nullptr;
    diallingNumberHandler->ProcessDiallingNumber(loadRequest, object);
    diallingNumberHandler->ProcessDiallingNumberLoadDone(event, id);
    std::shared_ptr<DiallingNumbersInfo> telNumber = std::make_shared<DiallingNumbersInfo>();
    EXPECT_TRUE(diallingNumberHandler->FormatNameAndNumber(telNumber, false));
    EXPECT_TRUE(diallingNumberHandler->FormatNameAndNumber(telNumber, true));
    diallingNumberHandler->MakeExceptionResult(0);
    diallingNumberHandler->UpdateFileController(iccFileController);
    diallingNumberHandler->CreateSavingSequence(telNumber, -1);
    telNumber = nullptr;
    diallingNumberHandler->FormatNameAndNumber(telNumber, true);
    std::shared_ptr<unsigned char> diallingNumberStringPac = nullptr;
    std::string numbe = "test";
    diallingNumberHandler->FillNumberFiledForDiallingNumber(diallingNumberStringPac, numbe, 0);
    diallingNumberHandler->FindLoadRequest(-1);
    std::string recordData = "test";
    diallingNumberHandler->FetchDiallingNumberContent(telNumber, recordData);
    std::u16string name = u"电话联系人";
    int seqLength;
    diallingNumberHandler->CreateNameSequence(name, seqLength);
}

/**
 * @tc.number   Telephony_IccDiallingNumbersManager_001
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(SimRilBranchTest, Telephony_IccDiallingNumbersManager_001, Function | MediumTest | Level1)
{
    auto telRilManager = std::make_shared<TelRilManager>();
    auto simStateManager = std::make_shared<SimStateManager>(telRilManager);
    EventFwk::MatchingSkills matchingSkills;
    matchingSkills.AddEvent(CommonEventSupport::COMMON_EVENT_OPERATOR_CONFIG_CHANGED);
    EventFwk::CommonEventSubscribeInfo subcribeInfo(matchingSkills);
    auto simFileManager1 = std::make_shared<SimFileManager>(subcribeInfo, telRilManager, simStateManager);
    simFileManager1 = nullptr;
    IccDiallingNumbersManager::CreateInstance(simFileManager1, simStateManager);
    auto simFileManager = std::make_shared<SimFileManager>(subcribeInfo, telRilManager, simStateManager);
    auto iccDiallingNumbersManager = IccDiallingNumbersManager::CreateInstance(simFileManager, simStateManager);
    iccDiallingNumbersManager->stateDiallingNumbers_ = IccDiallingNumbersManager::HandleRunningState::STATE_RUNNING;
    iccDiallingNumbersManager->Init();
    iccDiallingNumbersManager->stateDiallingNumbers_ = IccDiallingNumbersManager::HandleRunningState::STATE_NOT_START;
    iccDiallingNumbersManager->Init();
    simFileManager = nullptr;
    iccDiallingNumbersManager->simFileManager_ = simFileManager;
    iccDiallingNumbersManager->stateDiallingNumbers_ = IccDiallingNumbersManager::HandleRunningState::STATE_NOT_START;
    iccDiallingNumbersManager->Init();
    AppExecFwk::InnerEvent::Pointer event = AppExecFwk::InnerEvent::Get(1, 1);
    event = nullptr;
    iccDiallingNumbersManager->ProcessEvent(event);
    iccDiallingNumbersManager->ProcessEvent(AppExecFwk::InnerEvent::Get(0, 1));
    iccDiallingNumbersManager->ProcessEvent(AppExecFwk::InnerEvent::Get(MSG_SIM_DIALLING_NUMBERS_GET_DONE, 1));
    iccDiallingNumbersManager->ProcessEvent(AppExecFwk::InnerEvent::Get(MSG_SIM_DIALLING_NUMBERS_UPDATE_DONE, 1));
    iccDiallingNumbersManager->ProcessEvent(AppExecFwk::InnerEvent::Get(MSG_SIM_DIALLING_NUMBERS_WRITE_DONE, 1));
    iccDiallingNumbersManager->ProcessEvent(AppExecFwk::InnerEvent::Get(MSG_SIM_DIALLING_NUMBERS_DELETE_DONE, 1));
    iccDiallingNumbersManager->BuildCallerInfo(-1);
    std::shared_ptr<DiallingNumbersInfo> diallingNumber = nullptr;
    std::vector<std::shared_ptr<DiallingNumbersInfo>> result = {};
    EXPECT_GE(iccDiallingNumbersManager->UpdateIccDiallingNumbers(0, diallingNumber), TELEPHONY_ERR_SUCCESS);
    EXPECT_GE(iccDiallingNumbersManager->DelIccDiallingNumbers(0, diallingNumber), TELEPHONY_ERR_SUCCESS);
    EXPECT_GE(iccDiallingNumbersManager->AddIccDiallingNumbers(0, diallingNumber), TELEPHONY_ERR_SUCCESS);
    iccDiallingNumbersManager->diallingNumbersCache_ = nullptr;
    EXPECT_GE(iccDiallingNumbersManager->QueryIccDiallingNumbers(0, result), TELEPHONY_ERR_SUCCESS);
    std::shared_ptr<DiallingNumbersInfo> diallingNumbers1 = std::make_shared<DiallingNumbersInfo>();
    EXPECT_GE(iccDiallingNumbersManager->UpdateIccDiallingNumbers(-1, diallingNumber), TELEPHONY_ERR_SUCCESS);
    EXPECT_GE(iccDiallingNumbersManager->DelIccDiallingNumbers(-1, diallingNumber), TELEPHONY_ERR_SUCCESS);
    EXPECT_GE(iccDiallingNumbersManager->AddIccDiallingNumbers(-1, diallingNumber), TELEPHONY_ERR_SUCCESS);
    EXPECT_GE(iccDiallingNumbersManager->QueryIccDiallingNumbers(-1, result), TELEPHONY_ERR_SUCCESS);
}

/**
 * @tc.number   Telephony_UsimDiallingNumbersService_001
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(SimRilBranchTest, Telephony_UsimDiallingNumbersService_001, Function | MediumTest | Level1)
{
    auto usimDiallingNumbersService = std::make_shared<UsimDiallingNumbersService>();
    usimDiallingNumbersService->InitFuncMap();
    AppExecFwk::InnerEvent::Pointer event = AppExecFwk::InnerEvent::Get(1, 1);
    event = nullptr;
    usimDiallingNumbersService->ProcessEvent(event);
    usimDiallingNumbersService->FillDiallingNumbersRecords(nullptr);
    std::string record = "record";
    std::vector<std::string> records;
    records.push_back(record);
    usimDiallingNumbersService->GeneratePbrFile(records);
    usimDiallingNumbersService->BuildCallerInfo(-1);
    usimDiallingNumbersService->CreateHandlerPointer(-1, 1, 0, nullptr);
    usimDiallingNumbersService->BuildNumberFileByRecord(record);
    std::shared_ptr<UsimDiallingNumberFile> file = std::make_shared<UsimDiallingNumberFile>();
    std::vector<uint8_t> datav;
    uint8_t data = 0;
    datav.push_back(data);
    auto tlvEfSfi = std::make_shared<TagService>(datav);
    usimDiallingNumbersService->StorePbrDetailInfo(file, tlvEfSfi, 0);
    std::shared_ptr<IccFileController> iccFileController = nullptr;
    std::shared_ptr<IccDiallingNumbersHandler> iccDiallingNumbersHandler = nullptr;
    usimDiallingNumbersService->SetFileControllerAndDiallingNumberHandler(iccFileController, iccDiallingNumbersHandler);
    usimDiallingNumbersService->LoadPbrFiles();
    EXPECT_TRUE(usimDiallingNumbersService->fileController_ == nullptr);
}

/**
 * @tc.number   Telephony_SimStateManager_002
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(SimRilBranchTest, Telephony_SimStateManager_002, Function | MediumTest | Level1)
{
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    std::shared_ptr<Telephony::SimStateManager> simStateManager = std::make_shared<SimStateManager>(telRilManager);
    simStateManager->simStateRun_ = STATE_RUNNING;
    simStateManager->Init(0);
    simStateManager->simStateRun_ = STATE_NOT_START;
    simStateManager->telRilManager_ = nullptr;
    simStateManager->Init(0);
    simStateManager->simStateHandle_ = nullptr;
    simStateManager->RegisterCoreNotify(nullptr, 1);
    simStateManager->UnRegisterCoreNotify(nullptr, 1);
    simStateManager->RefreshSimState(0);
    std::string password = "1234";
    LockStatusResponse mLockStatusResponse;
    EXPECT_GT(simStateManager->UnlockPin(0, password, mLockStatusResponse), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simStateManager->UnlockPuk(0, password, password, mLockStatusResponse), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simStateManager->AlterPin(0, password, password, mLockStatusResponse), TELEPHONY_ERR_SUCCESS);
    LockInfo mLockInfo;
    mLockInfo.lockType = LockType::PIN_LOCK;
    mLockInfo.lockState = LockState::LOCK_OFF;
    EXPECT_GT(simStateManager->SetLockState(0, mLockInfo, mLockStatusResponse), TELEPHONY_ERR_SUCCESS);
    LockState lockState;
    PersoLockInfo mPersoLockInfo;
    SimAuthenticationResponse mResponse;
    EXPECT_GT(simStateManager->GetLockState(0, LockType::PIN_LOCK, lockState), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simStateManager->UnlockPin2(0, password, mLockStatusResponse), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simStateManager->UnlockPuk2(0, password, password, mLockStatusResponse), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simStateManager->AlterPin2(0, password, password, mLockStatusResponse), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simStateManager->UnlockSimLock(0, mPersoLockInfo, mLockStatusResponse), TELEPHONY_ERR_SUCCESS);
    EXPECT_NE(
        simStateManager->SimAuthentication(0, AuthType::SIM_AUTH_EAP_SIM_TYPE, "", mResponse), TELEPHONY_ERR_SUCCESS);
    EXPECT_NE(simStateManager->SendSimMatchedOperatorInfo(0, 0, "", ""), TELEPHONY_ERR_SUCCESS);
}

/**
 * @tc.number   Telephony_SimStateHandle_002
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(SimRilBranchTest, Telephony_SimStateHandle_002, Function | MediumTest | Level1)
{
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    std::shared_ptr<Telephony::SimStateManager> simStateManager = std::make_shared<SimStateManager>(telRilManager);
    auto simStateHandle = std::make_shared<SimStateHandle>(simStateManager);
    simStateHandle->Init(0);
    std::shared_ptr<TelRilManager> telRilManager1 = std::make_shared<TelRilManager>();
    telRilManager1 = nullptr;
    simStateHandle->SetRilManager(telRilManager1);
    simStateHandle->Init(0);
    simStateHandle->iccState_.simStatus_ = ICC_CARD_ABSENT;
    EXPECT_FALSE(simStateHandle->HasSimCard());
    simStateHandle->iccState_.simStatus_ = ICC_CONTENT_READY;
    EXPECT_TRUE(simStateHandle->HasSimCard());
    simStateHandle->GetSimState();
    simStateHandle->GetCardType();
    PersoLockInfo lockInfo;
    std::string authData = "test";
    simStateHandle->UnlockSimLock(0, lockInfo);
    EXPECT_EQ(simStateHandle->SimAuthentication(0, AuthType::SIM_AUTH_EAP_SIM_TYPE, authData), SIM_AUTH_FAIL);
}

/**
 * @tc.number   Telephony_IccOperatorRule_002
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(SimRilBranchTest, Telephony_IccOperatorRule_002, Function | MediumTest | Level1)
{
    auto iccOperatorRule = std::make_shared<IccOperatorRule>();
    std::string numIt = "E2E2";
    std::string numIt1 = "E1E1";
    std::string numIt2 = "E3E3";
    std::string numIt3 = "FF40FF40";
    IccOperatorRule result;
    std::string::const_iterator hexStrBeg = numIt.begin();
    std::string::const_iterator hexStrEnd = numIt.begin();
    std::string::const_iterator hexStrBeg1 = numIt1.begin();
    std::string::const_iterator hexStrEnd1 = numIt1.begin();
    std::string::const_iterator hexStrBeg2 = numIt2.begin();
    std::string::const_iterator hexStrEnd2 = numIt2.begin();
    std::string::const_iterator hexStrBeg3 = numIt3.begin();
    std::string::const_iterator hexStrEnd3 = numIt3.begin();
    std::vector<IccOperatorRule> list;
    std::string_view certHash = iccOperatorRule->certificate_;
    std::string_view packageName;
    std::string hexStr = "12";
    std::string testStr = "testStr";
    int32_t len = 1;
    iccOperatorRule->GetCertificate(testStr);
    iccOperatorRule->SetCertificate(testStr);
    iccOperatorRule->SetCertificate(std::move(testStr));
    iccOperatorRule->SetAccessLimit(testStr);
    iccOperatorRule->SetAccessLimit(std::move(testStr));
    EXPECT_FALSE(iccOperatorRule->DecodeTLVTagRule(hexStrBeg, hexStrEnd, result, len));
    EXPECT_FALSE(iccOperatorRule->DecodeTLVTagCertPkg(hexStrBeg1, hexStrEnd1, result));
    EXPECT_FALSE(iccOperatorRule->DecodeTLVTagLimits(hexStrBeg2, hexStrEnd2, result));
    EXPECT_FALSE(iccOperatorRule->CreateFromTLV(hexStrBeg3, hexStrEnd3, list));
    EXPECT_TRUE(iccOperatorRule->CreateFromTLV(numIt3, list));
    EXPECT_TRUE(iccOperatorRule->Matche(certHash, packageName));
    EXPECT_TRUE(iccOperatorRule->SetPackageNameByHexStr(hexStr));
}

/**
 * @tc.number   Telephony_tel_ril_manager_001
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(SimRilBranchTest, Telephony_tel_ril_manager_001, Function | MediumTest | Level1)
{
    auto telRilManager = std::make_shared<TelRilManager>();
    telRilManager->DeInit();
    telRilManager->ReduceRunningLock();
    telRilManager->SendAckAndLock();
    int32_t slotFaild = -1;
    telRilManager->GetTelRilSms(slotFaild);
    telRilManager->GetTelRilSim(slotFaild);
    telRilManager->GetTelRilModem(slotFaild);
    telRilManager->GetTelRilData(slotFaild);
    telRilManager->GetTelRilNetwork(slotFaild);
    telRilManager->GetTelRilCall(slotFaild);
    int32_t slot_0 = SLOT_ID;
    telRilManager->GetTelRilSms(slot_0);
    telRilManager->GetTelRilSim(slot_0);
    telRilManager->GetTelRilModem(slot_0);
    telRilManager->GetTelRilData(slot_0);
    telRilManager->GetTelRilNetwork(slot_0);
    telRilManager->GetTelRilCall(slot_0);
    std::shared_ptr<TelRilCall> emptyCall = nullptr;
    telRilManager->telRilCall_.push_back(emptyCall);
    telRilManager->ResetRilInterface();
    std::shared_ptr<ObserverHandler> observerHandler = nullptr;
    telRilManager->observerHandler_.push_back(observerHandler);
    std::shared_ptr<AppExecFwk::EventHandler> observerCallBack = nullptr;
    int32_t what = 0;
    int32_t *obj = 0;
    telRilManager->RegisterCoreNotify(slot_0, observerCallBack, what, obj);
    telRilManager->UnRegisterCoreNotify(slot_0, observerCallBack, what);
    telRilManager->rilInterface_ = nullptr;
    ASSERT_TRUE(telRilManager->DisConnectRilInterface());
}

/**
 * @tc.number   Telephony_IccFileController_001
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(SimRilBranchTest, Telephony_IccFileController_001, Function | MediumTest | Level1)
{
    std::shared_ptr<AppExecFwk::EventRunner> runner = AppExecFwk::EventRunner::Create("test");
    std::shared_ptr<IccFileController> iccFileController = std::make_shared<SimFileController>(1);
    auto event = AppExecFwk::InnerEvent::Get(0);
    iccFileController->ProcessReadBinary(event);
    std::shared_ptr<IccControllerHolder> holder = nullptr;
    std::unique_ptr<Telephony::IccFromRilMsg> object = std::make_unique<Telephony::IccFromRilMsg>(holder);
    auto event1 = AppExecFwk::InnerEvent::Get(0, object);
    auto event01 = AppExecFwk::InnerEvent::Get(0, object);
    auto event2 = AppExecFwk::InnerEvent::Get(0, object);
    iccFileController->ProcessReadBinary(event1);
    int fileId = 0;
    int size = 0;
    std::string path = "";
    int fileNum = 0;
    std::string data = "";
    int dataLength = 0;
    std::string pin2 = "";
    int *val = nullptr;
    iccFileController->ObtainBinaryFile(fileId, event01);
    iccFileController->ObtainBinaryFile(fileId, size, event2);
    iccFileController->ObtainLinearFixedFile(fileId, path, fileNum, event);
    iccFileController->ObtainAllLinearFixedFile(fileId, path, event);
    iccFileController->ObtainLinearFileSize(fileId, path, event);
    iccFileController->UpdateLinearFixedFile(fileId, path, fileNum, data, dataLength, pin2, event);
    iccFileController->UpdateLinearFixedFile(fileId, fileNum, data, dataLength, pin2, event);
    iccFileController->UpdateBinaryFile(fileId, data, dataLength, event);
    auto telRilManager = std::make_shared<TelRilManager>();
    iccFileController->telRilManager_ = telRilManager;
    auto event3 = AppExecFwk::InnerEvent::Get(0, object);
    auto event4 = AppExecFwk::InnerEvent::Get(0, object);
    iccFileController->ObtainBinaryFile(fileId, event3);
    iccFileController->ObtainBinaryFile(fileId, size, event4);
    iccFileController->ObtainLinearFixedFile(fileId, path, fileNum, event);
    iccFileController->UpdateLinearFixedFile(fileId, path, fileNum, data, dataLength, pin2, event);
    iccFileController->UpdateLinearFixedFile(fileId, fileNum, data, dataLength, pin2, event);
    iccFileController->UpdateBinaryFile(fileId, data, dataLength, event);
    std::vector<std::string> strValue;
    IccFileData *fd = nullptr;
    iccFileController->SendResponse(holder, fd);
    auto event5 = AppExecFwk::InnerEvent::Get(0);
    iccFileController->SendEfLinearResult(event5, val, size);
    auto event6 = AppExecFwk::InnerEvent::Get(0);
    iccFileController->SendMultiRecordResult(event6, strValue);
    unsigned char *data1;
    iccFileController->ParseFileSize(val, size, data1);
    iccFileController->IsValidRecordSizeData(data1);
    iccFileController->GetFileAndDataSize(data1, size, size);
    ASSERT_TRUE(iccFileController->BuildCallerInfo(0, 0, 0, holder) != nullptr);
}

/**
 * @tc.number   Telephony_SimManager_001
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(SimRilBranchTest, Telephony_SimManager_001, Function | MediumTest | Level1)
{
    std::shared_ptr<ITelRilManager> telRilManager = nullptr;
    auto simManager = std::make_shared<SimManager>(telRilManager);
    simManager->InitSingleSimObject();
    simManager->slotCount_ = 1;
    int32_t slotId;
    std::u16string testU = u"";
    simManager->SetShowNumber(INVALID_SLOTID, testU);
    simManager->GetShowNumber(INVALID_SLOTID, testU);
    simManager->GetDefaultVoiceSimId(slotId);
    simManager->GetDefaultSmsSlotId();
    simManager->slotCount_ = 1;
    int32_t dsdsMode = 0;
    int32_t slotCount = 1;
    std::string testS = "";
    simManager->GetDsdsMode(dsdsMode);
    simManager->stkManager_.resize(slotCount);
    simManager->simFileManager_.resize(slotCount);
    simManager->SendCallSetupRequestResult(INVALID_SLOTID, true);
    simManager->GetSimGid2(INVALID_SLOTID);
    simManager->GetOpName(INVALID_SLOTID, testU);
    simManager->GetOpKey(INVALID_SLOTID, testU);
    simManager->GetOpKeyExt(INVALID_SLOTID, testU);
    simManager->GetSimTeleNumberIdentifier(INVALID_SLOTID);
    simManager->ObtainSpnCondition(INVALID_SLOTID, false, testS);
    simManager->slotCount_ = 0;
    simManager->GetPrimarySlotId(slotId);
    EXPECT_GT(simManager->GetDefaultSmsSlotId(), TELEPHONY_PERMISSION_ERROR);
}

AppExecFwk::InnerEvent::Pointer GetControllerToFileMsgEvent(int32_t code, bool withException)
{
    auto objectUnique = std::make_unique<ControllerToFileMsg>(nullptr, nullptr);
    objectUnique->resultData = "123456";
    if (withException) {
        objectUnique->exception = std::make_shared<int32_t>(0);
    }
    return AppExecFwk::InnerEvent::Get(code, objectUnique);
}

AppExecFwk::InnerEvent::Pointer GetDiallingNumbersHandlerResultEvent(int32_t code, bool withException)
{
    auto objectUnique = std::make_unique<DiallingNumbersHandlerResult>(nullptr);
    objectUnique->result = std::make_shared<DiallingNumbersInfo>();
    if (withException) {
        objectUnique->exception = std::make_shared<int32_t>(0);
    }
    return AppExecFwk::InnerEvent::Get(code, objectUnique);
}

/**
 * @tc.number   Telephony_SimFile_001
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(SimRilBranchTest, Telephony_SimFile_001, Function | MediumTest | Level1)
{
    std::shared_ptr<AppExecFwk::EventRunner> runner = nullptr;
    std::shared_ptr<SimStateManager> simStateManager = nullptr;
    auto simFile = std::make_shared<SimFile>(simStateManager);
    std::vector<int> testCase1 = { MSG_SIM_OBTAIN_GID1_DONE, MSG_SIM_OBTAIN_GID2_DONE, MSG_SIM_SET_MSISDN_DONE,
        MSG_SIM_OBTAIN_SPDI_DONE, MSG_SIM_OBTAIN_CFIS_DONE, MSG_SIM_OBTAIN_MWIS_DONE,
        MSG_SIM_OBTAIN_VOICE_MAIL_INDICATOR_CPHS_DONE, MSG_SIM_OBTAIN_ICCID_DONE, MSG_SIM_OBTAIN_CFF_DONE,
        MSG_SIM_OBTAIN_CFF_DONE, MSG_SIM_OBTAIN_AD_DONE, MSG_SIM_SMS_ON_SIM, MSG_SIM_OBTAIN_ALL_SMS_DONE,
        MSG_SIM_OBTAIN_SMS_DONE, MSG_SIM_OBTAIN_PLMN_W_ACT_DONE, MSG_SIM_OBTAIN_OPLMN_W_ACT_DONE,
        MSG_SIM_OBTAIN_CSP_CPHS_DONE, MSG_SIM_OBTAIN_INFO_CPHS_DONE, MSG_SIM_OBTAIN_SST_DONE, MSG_SIM_OBTAIN_PNN_DONE,
        MSG_SIM_OBTAIN_OPL_DONE, MSG_SIM_OBTAIN_OPL5G_DONE, MSG_SIM_UPDATE_DONE, MSG_SIM_OBTAIN_HPLMN_W_ACT_DONE,
        MSG_SIM_OBTAIN_EHPLMN_DONE, MSG_SIM_OBTAIN_FPLMN_DONE };
    for (int32_t code : testCase1) {
        auto event1 = GetControllerToFileMsgEvent(code, true);
        simFile->ProcessEvent(event1);
        if (code == MSG_SIM_SMS_ON_SIM) {
            continue;
        }
        auto event2 = GetControllerToFileMsgEvent(code, false);
        simFile->ProcessEvent(event2);
    }
    std::vector<int> testCase2 = { MSG_SIM_OBTAIN_MSISDN_DONE, MSG_SIM_OBTAIN_CPHS_MAILBOX_DONE,
        MSG_SIM_SET_CPHS_MAILBOX_DONE, MSG_SIM_SET_MBDN_DONE };
    simFile->cphsInfo_ = "";
    for (int32_t code : testCase2) {
        auto event1 = GetDiallingNumbersHandlerResultEvent(code, false);
        simFile->ProcessEvent(event1);
        auto event2 = GetDiallingNumbersHandlerResultEvent(code, true);
        simFile->ProcessEvent(event2);
    }
    int32_t efCfisSize = 0;
    std::string efCfisStr = "1234567890ABCDEF1234567890ABCDEF1234567890ABCDEF";
    std::string number = "123456789111111";
    std::shared_ptr<unsigned char> efCfisData = SIMUtils::HexStringConvertToBytes(efCfisStr, efCfisSize);
    ASSERT_TRUE(simFile->FillNumber(efCfisData, efCfisSize, number));
    efCfisStr = "1234";
    efCfisData = SIMUtils::HexStringConvertToBytes(efCfisStr, efCfisSize);
    ASSERT_FALSE(simFile->FillNumber(efCfisData, efCfisSize, number));
}

/**
 * @tc.number   Telephony_SimFile_002
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(SimRilBranchTest, Telephony_SimFile_002, Function | MediumTest | Level1)
{
    std::shared_ptr<AppExecFwk::EventRunner> runner = nullptr;
    std::shared_ptr<SimStateManager> simStateManager = nullptr;
    auto simFile = std::make_shared<SimFile>(simStateManager);
    int32_t voiceMailCount = 2;
    simFile->SetVoiceMailCount(voiceMailCount);
    bool enable = true;
    std::string number = "00000000000";
    simFile->SetVoiceCallForwarding(enable, number);
    std::vector<std::string> records = { "460000", "01AABBCCDD" };
    simFile->ParsePnn(records);
    ASSERT_FALSE(simFile->EfCfisAvailable(0));
}

/**
 * @tc.number   Telephony_MccPool_001
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(SimRilBranchTest, Telephony_MccPool_001, Function | MediumTest | Level1)
{
    EXPECT_EQ(MccPool::MccCountryCode(MCC_CN_A), "cn");
    EXPECT_EQ(MccPool::MccCountryCode(INVALID_MCC), "");
    EXPECT_EQ(MccPool::ShortestMncLengthFromMcc(MCC_CN_A), MCC_SHORT);
    EXPECT_EQ(MccPool::ShortestMncLengthFromMcc(MCC_US_A), MCC_LONG);
    ASSERT_TRUE(MccPool::LengthIsThreeMnc("302370"));
    ASSERT_FALSE(MccPool::LengthIsThreeMnc("46000"));

    auto mccAccess = MccPool::AccessToMcc(MCC_BJ);
    EXPECT_EQ(mccAccess->iso_, "bj");
    std::shared_ptr<MccAccess> mccAccessA = std::make_shared<MccAccess>(MCC_MK, "", 0);
    std::shared_ptr<MccAccess> mccAccessB = std::make_shared<MccAccess>(MCC_CN_A, "", 0);
    std::shared_ptr<MccAccess> mccAccessC = std::make_shared<MccAccess>(MCC_CD, "", 0);
    std::shared_ptr<MccAccess> mccAccessD = nullptr;
    ASSERT_TRUE(MccPool::MccCompare(mccAccessA, mccAccessB));
    ASSERT_FALSE(MccPool::MccCompare(mccAccessC, mccAccessB));
    ASSERT_FALSE(MccPool::MccCompare(mccAccessA, mccAccessD));
}
} // namespace Telephony
} // namespace OHOS
