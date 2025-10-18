/*
 * Copyright (C) 2022-2024 Huawei Device Co., Ltd.
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
#include "cell_info.h"
#include "cell_location.h"
#include "common_event_manager.h"
#include "common_event_support.h"
#include "core_manager_inner.h"
#include "core_service_client.h"
#include "csim_file_controller.h"
#include "gtest/gtest.h"
#include <gmock/gmock.h>
#include "tel_ril_base_parcel.h"
#include "icc_file.h"
#include "icc_file_controller.h"
#include "icc_operator_rule.h"
#include "ims_core_service_callback_proxy.h"
#include "ims_core_service_callback_stub.h"
#include "ims_core_service_proxy.h"
#include "ims_reg_info_callback_proxy.h"
#include "isim_file_controller.h"
#include "multi_sim_controller.h"
#include "multi_sim_monitor.h"
#include "network_register.h"
#include "network_search_manager.h"
#include "network_search_state.h"
#include "operator_matching_rule.h"
#include "operator_name.h"
#include "radio_protocol_controller.h"
#include "ruim_file_controller.h"
#include "sim_file_controller.h"
#include "sim_file_init.h"
#include "sim_file_manager.h"
#include "sim_manager.h"
#include "sim_number_decode.h"
#include "sim_rdb_helper.h"
#include "sim_sms_controller.h"
#include "sim_state_manager.h"
#include "sim_utils.h"
#include "stk_controller.h"
#include "stk_manager.h"
#include "tag_service.h"
#include "tel_ril_manager.h"
#include "telephony_errors.h"
#include "telephony_hisysevent.h"
#include "telephony_log_wrapper.h"
#include "usim_file_controller.h"
#include "telephony_data_helper.h"
#include "sim_data.h"
#include "accesstoken_kit.h"
#include "token_setproc.h"
#include "nativetoken_kit.h"
#include "telephony_ext_wrapper.h"
#include "sim_file_parse.h"
#include "network_utils.h"
#include "mock_sim_manager.h"

namespace OHOS {
namespace Telephony {
using namespace testing::ext;

namespace {
const int32_t SLOT_ID_0 = 0;
const int32_t INVALID_SLOTID = -1;
const int DATA_STORAGE_ERR_PERMISSION_ERR = -3;
const int32_t OBTAIN_SPN_NONE = 0;
const int32_t OBTAIN_SPN_START = 1;
const int32_t OBTAIN_SPN_GENERAL = 2;
const int32_t OBTAIN_OPERATOR_NAMESTRING = 3;
const int32_t OBTAIN_OPERATOR_NAME_SHORTFORM = 4;
const int32_t BYTES_LENGTH = 3;
const int32_t LO_FOUR_LENGTH = 15;
const int32_t VALUE_LENGTH = 128;
static const int32_t SLEEP_TIME = 3;
const int SLOT_COUNT = 2;
} // namespace

class BranchTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};

void BranchTest::TearDownTestCase()
{
    sleep(SLEEP_TIME);
}

void BranchTest::SetUp() {}

void BranchTest::TearDown() {}

void BranchTest::SetUpTestCase()
{
    constexpr int permissionNum = 2;
    const char *perms[permissionNum] = {"ohos.permission.GET_TELEPHONY_STATE",
        "ohos.permission.SET_TELEPHONY_STATE"};
    NativeTokenInfoParams infoInstance = {.dcapsNum = 0, .permsNum = permissionNum, .aclsNum = 0, .dcaps = nullptr,
        .perms = perms, .acls = nullptr, .processName = "BranchTest", .aplStr = "system_basic",
    };
    uint64_t tokenId = GetAccessTokenId(&infoInstance);
    SetSelfTokenID(tokenId);
    auto result = Security::AccessToken::AccessTokenKit::ReloadNativeTokenInfo();
    EXPECT_EQ(result, Security::AccessToken::RET_SUCCESS);
}

/**
 * @tc.number   Telephony_ImsRegInfoCallbackProxy_001
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_ImsRegInfoCallbackProxy_001, Function | MediumTest | Level1)
{
    sptr<ISystemAbilityManager> systemAbilityMgr = SystemAbilityManagerClient::GetInstance().GetSystemAbilityManager();
    if (systemAbilityMgr == nullptr) {
        TELEPHONY_LOGE("Telephony_ImsRegInfoCallbackProxy systemAbilityMgr is nullptr");
    }
    ASSERT_NE(systemAbilityMgr, nullptr);
    sptr<IRemoteObject> remote = systemAbilityMgr->CheckSystemAbility(TELEPHONY_CORE_SERVICE_SYS_ABILITY_ID);
    if (remote == nullptr) {
        TELEPHONY_LOGE("Telephony_ImsRegInfoCallbackProxy remote is nullptr");
    }
    ASSERT_NE(remote, nullptr);
    auto imsRegInfoCallbackProxy = std::make_shared<ImsRegInfoCallbackProxy>(remote);
    EXPECT_NE(imsRegInfoCallbackProxy, nullptr);
    ImsRegInfo info;
    imsRegInfoCallbackProxy->OnImsRegInfoChanged(0, ImsServiceType::TYPE_VOICE, info);
}

/**
 * @tc.number   Telephony_SimFile_001
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_SimFile_001, Function | MediumTest | Level1)
{
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    std::shared_ptr<Telephony::SimStateManager> simStateManager = std::make_shared<SimStateManager>(telRilManager);
    std::shared_ptr<SimFile> simFile = std::make_shared<SimFile>(simStateManager);
    std::shared_ptr<SimFileInit> simFileInit = std::make_shared<SimFileInit>();
    auto event = AppExecFwk::InnerEvent::Get(0);
    event = nullptr;
    simFile->ProcessEvent(event);
    simFile->ObtainSpnPhase(false, event);
    simFile->ProcessSpnGeneral(event);
    simFile->ProcessSpnCphs(event);
    simFile->ProcessSpnShortCphs(event);
    simFileInit->InitMemberFunc(*simFile.get());
    simFile->ProcessFileLoaded(false);
    simFile->ProcessFileLoaded(true);
    simFile->ProcessIccRefresh(ELEMENTARY_FILE_MBDN);
    simFile->ProcessIccRefresh(ELEMENTARY_FILE_MAILBOX_CPHS);
    simFile->ProcessIccRefresh(ELEMENTARY_FILE_CSP_CPHS);
    simFile->ProcessIccRefresh(ELEMENTARY_FILE_FDN);
    simFile->ProcessIccRefresh(ELEMENTARY_FILE_MSISDN);
    simFile->ProcessIccRefresh(ELEMENTARY_FILE_CFIS);
    simFile->ProcessIccRefresh(ELEMENTARY_FILE_CFF_CPHS);
    EXPECT_EQ(simFile->ObtainIsoCountryCode(), "");
    simFile->imsi_ = "1234567890";
    EXPECT_EQ(simFile->ObtainIsoCountryCode(), "");
    simFile->lengthOfMnc_ = OBTAIN_SPN_GENERAL;
    simFile->CheckMncLengthForAdDone();
    simFile->lengthOfMnc_ = UNKNOWN_MNC;
    simFile->CheckMncLengthForAdDone();
    simFile->lengthOfMnc_ = UNINITIALIZED_MNC;
    simFile->CheckMncLengthForAdDone();
    EXPECT_FALSE(simFile->CphsVoiceMailAvailable());
    EXPECT_FALSE(simFile->ProcessIccReady(event));
    EXPECT_TRUE(simFile->ProcessGetAdDone(event));
    EXPECT_TRUE(simFile->ProcessVoiceMailCphs(event));
    EXPECT_TRUE(simFile->ProcessGetMwisDone(event));
    EXPECT_TRUE(simFile->ProcessGetMbdnDone(event));
}

/**
 * @tc.number   Telephony_SimFile_002
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_SimFile_002, Function | MediumTest | Level1)
{
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    std::shared_ptr<Telephony::SimStateManager> simStateManager = std::make_shared<SimStateManager>(telRilManager);
    std::shared_ptr<SimFile> simFile = std::make_shared<SimFile>(simStateManager);
    auto event = AppExecFwk::InnerEvent::Get(0);
    event = nullptr;
    std::string testStr = "";
    EXPECT_TRUE(simFile->ProcessGetCphsMailBoxDone(event));
    EXPECT_TRUE(simFile->ProcessGetMbiDone(event));
    EXPECT_TRUE(simFile->ProcessGetCfisDone(event));
    EXPECT_TRUE(simFile->ProcessGetCffDone(event));
    EXPECT_TRUE(simFile->ProcessObtainIMSIDone(event));
    EXPECT_TRUE(simFile->ProcessGetIccIdDone(event));
    EXPECT_TRUE(simFile->ProcessGetPlmnActDone(event));
    EXPECT_TRUE(simFile->ProcessGetOplmnActDone(event));
    EXPECT_TRUE(simFile->ProcessGetSpdiDone(event));
    EXPECT_TRUE(simFile->ProcessGetMsisdnDone(event));
    simFile->ProcessSetMsisdnDone(event);
    EXPECT_TRUE(simFile->ProcessObtainGid1Done(event));
    EXPECT_TRUE(simFile->ProcessObtainGid2Done(event));
    EXPECT_FALSE(simFile->ProcessSmsOnSim(event));
    EXPECT_TRUE(simFile->ProcessGetCspCphs(event));
    EXPECT_TRUE(simFile->ProcessGetInfoCphs(event));
    EXPECT_TRUE(simFile->ProcessGetSstDone(event));
    EXPECT_FALSE(simFile->ProcessGetSmsDone(event));
    EXPECT_TRUE(simFile->ProcessGetAllSmsDone(event));
    EXPECT_TRUE(simFile->ProcessGetHplmActDone(event));
    EXPECT_TRUE(simFile->ProcessGetEhplmnDone(event));
    EXPECT_TRUE(simFile->ProcessGetPnnDone(event));
    EXPECT_TRUE(simFile->ProcessGetOplDone(event));
    EXPECT_TRUE(simFile->ProcessGetOpl5gDone(event));
    EXPECT_FALSE(simFile->ProcessUpdateDone(event));
    EXPECT_TRUE(simFile->ProcessSetCphsMailbox(event));
    EXPECT_TRUE(simFile->ProcessGetFplmnDone(event));
    EXPECT_TRUE(simFile->ProcessSetMbdn(event));
    EXPECT_FALSE(simFile->ProcessMarkSms(event));
    EXPECT_TRUE(simFile->ProcessObtainSpnPhase(event));
    EXPECT_TRUE(simFile->ProcessObtainLiLanguage(event));
    EXPECT_TRUE(simFile->ProcessObtainPlLanguage(event));
}

/**
 * @tc.number   Telephony_SimFile_003
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_SimFile_003, Function | MediumTest | Level1)
{
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    std::shared_ptr<Telephony::SimStateManager> simStateManager = std::make_shared<SimStateManager>(telRilManager);
    std::shared_ptr<SimFile> simFile = std::make_shared<SimFile>(simStateManager);
    auto event = AppExecFwk::InnerEvent::Get(0);
    std::string mailName = "";
    std::string mailNumber = "1234";
    simFile->indexOfMailbox_ = 0;
    simFile->UpdateVoiceMail("", "");
    EXPECT_FALSE(simFile->UpdateVoiceMail(mailName, mailNumber));
    simFile->efCfis_ = (unsigned char *)mailNumber.c_str();
    simFile->imsi_ = "123";
    simFile->lengthOfMnc_ = UNKNOWN_MNC;
    EXPECT_EQ(simFile->ObtainSimOperator(), "");
    simFile->lengthOfMnc_ = UNINITIALIZED_MNC;
    EXPECT_EQ(simFile->ObtainSimOperator(), "");
    EXPECT_TRUE(simFile->ProcessGetCphsMailBoxDone(event));
    EXPECT_TRUE(simFile->ProcessGetMbiDone(event));
    EXPECT_TRUE(simFile->ProcessGetCfisDone(event));
    EXPECT_TRUE(simFile->ProcessGetCffDone(event));
    EXPECT_TRUE(simFile->ProcessObtainIMSIDone(event));
    EXPECT_TRUE(simFile->ProcessGetIccIdDone(event));
    EXPECT_TRUE(simFile->ProcessGetPlmnActDone(event));
    EXPECT_TRUE(simFile->ProcessGetOplmnActDone(event));
    EXPECT_TRUE(simFile->ProcessGetSpdiDone(event));
    EXPECT_TRUE(simFile->ProcessGetMsisdnDone(event));
    simFile->ProcessSetMsisdnDone(event);
    EXPECT_TRUE(simFile->ProcessObtainGid1Done(event));
    EXPECT_TRUE(simFile->ProcessObtainGid2Done(event));
    EXPECT_FALSE(simFile->ProcessSmsOnSim(event));
    EXPECT_TRUE(simFile->ProcessGetCspCphs(event));
    EXPECT_TRUE(simFile->ProcessGetInfoCphs(event));
    EXPECT_TRUE(simFile->ProcessGetSstDone(event));
    EXPECT_FALSE(simFile->ProcessGetSmsDone(event));
    EXPECT_TRUE(simFile->ProcessGetAllSmsDone(event));
    EXPECT_TRUE(simFile->ProcessGetHplmActDone(event));
    EXPECT_TRUE(simFile->ProcessGetEhplmnDone(event));
    EXPECT_TRUE(simFile->ProcessGetPnnDone(event));
    EXPECT_TRUE(simFile->ProcessGetOplDone(event));
    EXPECT_FALSE(simFile->ProcessUpdateDone(event));
    EXPECT_TRUE(simFile->ProcessGetFplmnDone(event));
}

/**
 * @tc.number   Telephony_SimFile_004
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_SimFile_004, Function | MediumTest | Level1)
{
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    std::shared_ptr<Telephony::SimStateManager> simStateManager = std::make_shared<SimStateManager>(telRilManager);
    std::shared_ptr<SimFile> simFile = std::make_shared<SimFile>(simStateManager);
    std::string operatorNum = "";
    EXPECT_EQ(simFile->ObtainSpnCondition(true, operatorNum), 0);
    simFile->displayConditionOfSpn_ = OBTAIN_SPN_GENERAL;
    EXPECT_GT(simFile->ObtainSpnCondition(false, operatorNum), 0);
    simFile->displayConditionOfSpn_ = -1;
    EXPECT_EQ(simFile->ObtainSpnCondition(false, operatorNum), 0);
    EXPECT_EQ(simFile->ObtainCallForwardStatus(), 0);
    EXPECT_EQ(simFile->ObtainUsimFunctionHandle(), nullptr);
    EXPECT_EQ(simFile->AnalysisBcdPlmn(operatorNum, operatorNum), "");
    simFile->ProcessElementaryFileCsp(operatorNum);
    simFile->simFileParse_->ParseSpdi(operatorNum, *simFile);
    simFile->ProcessSmses(operatorNum);
    simFile->ProcessSms(operatorNum);
    std::vector<std::string> emptyRecords = {};
    std::vector<std::string> records = { "46000", "46002", "46004", "46007", "46008" };
    simFile->simFileParse_->ParsePnn(emptyRecords, *simFile);
    simFile->simFileParse_->ParseOpl(emptyRecords, *simFile);
    simFile->simFileParse_->ParsePnn(records, *simFile);
    simFile->simFileParse_->ParseOpl(records, *simFile);
    std::vector<std::string> invalidRecords = { "64F0100000GGGG02", "64F0000000GGGG01" };
    simFile->simFileParse_->ParseOpl(invalidRecords, *simFile);
    std::vector<std::string> invalidPlmnRecords = { "F640100000FFFE02", "F640000000FFFE01" };
    simFile->simFileParse_->ParseOpl(invalidPlmnRecords, *simFile);
    std::vector<std::string> records5g = { "64F0100000FFFE02", "64F0000000FFFE01" };
    simFile->simFileParse_->ParseOpl(records5g, *simFile);
    SimFile::SpnStatus newStatus;
    EXPECT_TRUE(simFile->IsContinueGetSpn(false, SimFile::SpnStatus::OBTAIN_SPN_NONE, newStatus));
    EXPECT_TRUE(simFile->IsContinueGetSpn(true, SimFile::SpnStatus::OBTAIN_SPN_NONE, newStatus));
    EXPECT_FALSE(simFile->IsContinueGetSpn(true, SimFile::SpnStatus::OBTAIN_SPN_START, newStatus));
    EXPECT_FALSE(simFile->IsContinueGetSpn(true, SimFile::SpnStatus::OBTAIN_SPN_GENERAL, newStatus));
    EXPECT_FALSE(simFile->IsContinueGetSpn(true, SimFile::SpnStatus::OBTAIN_OPERATOR_NAMESTRING, newStatus));
    EXPECT_FALSE(simFile->IsContinueGetSpn(true, SimFile::SpnStatus::OBTAIN_OPERATOR_NAME_SHORTFORM, newStatus));
    EXPECT_EQ(simFile->ObtainExtensionElementaryFile(ELEMENTARY_FILE_MSISDN), ELEMENTARY_FILE_EXT5);
    EXPECT_EQ(simFile->ObtainExtensionElementaryFile(ELEMENTARY_FILE_SPN), ELEMENTARY_FILE_EXT1);
    EXPECT_EQ(simFile->simFileParse_->ParseSpn(operatorNum, 0, *simFile), "");
    EXPECT_EQ(simFile->simFileParse_->ParseSpn("CMCC", OBTAIN_SPN_NONE, *simFile), "");
    EXPECT_EQ(simFile->simFileParse_->ParseSpn("CMCC", OBTAIN_SPN_START, *simFile), "");
    EXPECT_EQ(simFile->simFileParse_->ParseSpn("CMCC", OBTAIN_SPN_GENERAL, *simFile), "");
    EXPECT_EQ(simFile->simFileParse_->ParseSpn("CMCC", OBTAIN_OPERATOR_NAMESTRING, *simFile), "");
    EXPECT_EQ(simFile->simFileParse_->ParseSpn("CMCC", OBTAIN_OPERATOR_NAME_SHORTFORM, *simFile), "");
    EXPECT_EQ(simFile->simFileParse_->ParseSpn("", 0, *simFile), "");
}

/**
 * @tc.number   Telephony_SimFile_005
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_SimFile_005, Function | MediumTest | Level1)
{
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    std::shared_ptr<Telephony::SimStateManager> simStateManager = std::make_shared<SimStateManager>(telRilManager);
    std::shared_ptr<SimFile> simFile = std::make_shared<SimFile>(simStateManager);
    std::vector<std::string> emptyRecords = {};
    simFile->simFileParse_->ParseOpl5g(emptyRecords, *simFile);
    EXPECT_TRUE(simFile->opl5gFiles_.empty());
    std::vector<std::string> records = { "46000", "46002", "46004", "46007", "46008" };
    simFile->simFileParse_->ParseOpl5g(records, *simFile);
    EXPECT_TRUE(simFile->opl5gFiles_.empty());
    std::vector<std::string> invalidRecords = { "64F010000000GGGGGG02", "64F000000000GGGGGG01" };
    simFile->simFileParse_->ParseOpl5g(invalidRecords, *simFile);
    EXPECT_TRUE(simFile->opl5gFiles_.empty());
    std::vector<std::string> invalidPlmnRecords = { "F64010000000FFFFFE02", "F64000000000FFFFFE01" };
    simFile->simFileParse_->ParseOpl5g(invalidPlmnRecords, *simFile);
    EXPECT_TRUE(simFile->opl5gFiles_.empty());
    std::vector<std::string> records5g = { "64F010000000FFFFFE02", "64F000000000FFFFFE01" };
    simFile->simFileParse_->ParseOpl5g(records5g, *simFile);
    EXPECT_FALSE(simFile->opl5gFiles_.empty());
}

/**
 * @tc.number   Telephony_SimFile_006
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_SimFile_006, Function | MediumTest | Level1)
{
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    std::shared_ptr<Telephony::SimStateManager> simStateManager = std::make_shared<SimStateManager>(telRilManager);
    std::shared_ptr<SimFile> simFile = std::make_shared<SimFile>(simStateManager);
    EventFwk::MatchingSkills matchingSkills;
    matchingSkills.AddEvent(EventFwk::CommonEventSupport::COMMON_EVENT_OPERATOR_CONFIG_CHANGED);
    EventFwk::CommonEventSubscribeInfo subcribeInfo(matchingSkills);
    auto simFileManager = std::make_shared<SimFileManager>(subcribeInfo, std::weak_ptr<ITelRilManager>(telRilManager),
        std::weak_ptr<SimStateManager>(simStateManager));
    simFileManager->Init(0);
    auto event = AppExecFwk::InnerEvent::Get(0);
    event = nullptr;
    EXPECT_FALSE(simFile->ProcessIccLocked(event));
}

HWTEST_F(BranchTest, Telephony_SimFile_007, Function | MediumTest | Level1)
{
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    std::shared_ptr<Telephony::SimStateManager> simStateManager = std::make_shared<SimStateManager>(telRilManager);
    std::shared_ptr<SimFile> simFile = std::make_shared<SimFile>(simStateManager);
    std::string hplmn = "";
    simFile->simFileParse_->ParseEhplmn(hplmn, *simFile);
    std::string spdi = "";
    simFile->simFileParse_->ParseSpdi(spdi, *simFile);
    auto event = AppExecFwk::InnerEvent::Get(0);
    EXPECT_TRUE(simFile->ProcessGetSpnCphsDone(event));
    EXPECT_TRUE(simFile->ProcessGetSpnShortCphsDone(event));
}

HWTEST_F(BranchTest, Telephony_SimFile_008, Function | MediumTest | Level1)
{
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    std::shared_ptr<Telephony::SimStateManager> simStateManager = std::make_shared<SimStateManager>(telRilManager);
    std::shared_ptr<SimFile> simFile = std::make_shared<SimFile>(simStateManager);
    std::string hplmn1 = "42F01042F050";
    simFile->simFileParse_->ParseEhplmn(hplmn1, *simFile);
    std::string spdi1 = "A305800342F050";
    simFile->simFileParse_->ParseSpdi(spdi1, *simFile);
    auto event = AppExecFwk::InnerEvent::Get(0);
    EXPECT_TRUE(simFile->ProcessGetSpnCphsDone(event));
    EXPECT_TRUE(simFile->ProcessGetSpnShortCphsDone(event));
}

/**
 * @tc.number   Telephony_ISimFile_001
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_ISimFile_001, Function | MediumTest | Level1)
{
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    std::shared_ptr<Telephony::SimStateManager> simStateManager = std::make_shared<SimStateManager>(telRilManager);
    std::shared_ptr<IsimFile> iSimFile = std::make_shared<IsimFile>(simStateManager);
    auto event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_SIM_STATE_READY, 1);
    iSimFile->InitMemberFunc();
    iSimFile->ProcessEvent(event);
    event = AppExecFwk::InnerEvent::Get(0, 1);
    iSimFile->ProcessEvent(event);
    EXPECT_TRUE(iSimFile->ProcessGetIccidDone(event));
    EXPECT_TRUE(iSimFile->ProcessGetImsiDone(event));
    EXPECT_TRUE(iSimFile->ProcessGetImpiDone(event));
    EXPECT_TRUE(iSimFile->ProcessGetIstDone(event));
    event = nullptr;
    iSimFile->ProcessEvent(event);
    EXPECT_FALSE(iSimFile->ProcessIccReady(event));
    EXPECT_EQ(iSimFile->ObtainIsimImpi(), "");
    EXPECT_EQ(iSimFile->ObtainIsimDomain(), "");
    EXPECT_EQ(iSimFile->ObtainIsimImpu(), nullptr);
    EXPECT_EQ(iSimFile->ObtainIsimIst(), "");
    EXPECT_EQ(iSimFile->ObtainIsimPcscf(), nullptr);
    EXPECT_FALSE(iSimFile->UpdateVoiceMail("", ""));
    EXPECT_EQ(iSimFile->ObtainSpnCondition(true, ""), 0);
    EXPECT_EQ(iSimFile->ObtainIsoCountryCode(), "");
    iSimFile->ProcessFileLoaded(false);
    iSimFile->ProcessLockedAllFilesFetched();
    EXPECT_FALSE(iSimFile->ProcessIsimRefresh(event));
    EXPECT_TRUE(iSimFile->ProcessGetImsiDone(event));
    EXPECT_TRUE(iSimFile->ProcessGetIccidDone(event));
    EXPECT_TRUE(iSimFile->ProcessGetImpiDone(event));
    EXPECT_TRUE(iSimFile->ProcessGetIstDone(event));
}

/**
 * @tc.number   Telephony_RuimFile_001
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_RuimFile_001, Function | MediumTest | Level1)
{
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    std::shared_ptr<Telephony::SimStateManager> simStateManager = std::make_shared<SimStateManager>(telRilManager);
    std::shared_ptr<RuimFile> rUimFile = std::make_shared<RuimFile>(simStateManager);
    auto event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_SIM_STATE_READY, 1);
    rUimFile->InitMemberFunc();
    rUimFile->ProcessEvent(event);
    event = AppExecFwk::InnerEvent::Get(0, 1);
    rUimFile->ProcessEvent(event);
    rUimFile->ProcessFileLoaded(false);
    unsigned char spnData[10] = { 0 };
    rUimFile->ParseSpnName(IccDiallingNumberConstant::CSIM_SPN_OCTET, spnData, 0);
    rUimFile->ParseSpnName(IccDiallingNumberConstant::CSIM_SPN_LATIN, spnData, 0);
    rUimFile->ParseSpnName(IccDiallingNumberConstant::CSIM_SPN_IA5, spnData, 0);
    rUimFile->ParseSpnName(IccDiallingNumberConstant::CSIM_SPN_7BIT_ALPHABET, spnData, 0);
    rUimFile->ParseSpnName(IccDiallingNumberConstant::NAME_CHAR_POS, spnData, 0);
    EXPECT_TRUE(rUimFile->ProcessGetIccidDone(event));
    EXPECT_TRUE(rUimFile->ProcessGetImsiDone(event));
    EXPECT_TRUE(rUimFile->ProcessGetSpnDone(event));
    event = nullptr;
    rUimFile->ProcessEvent(event);
    EXPECT_EQ(rUimFile->ObtainSimOperator(), "");
    EXPECT_EQ(rUimFile->ObtainIsoCountryCode(), "");
    rUimFile->imsi_ = "12345678";
    EXPECT_EQ(rUimFile->ObtainSimOperator(), "12345");
    EXPECT_EQ(rUimFile->ObtainIsoCountryCode(), "");
    EXPECT_EQ(rUimFile->ObtainMdnNumber(), "");
    EXPECT_EQ(rUimFile->ObtainCdmaMin(), "");
    EXPECT_EQ(rUimFile->ObtainPrlVersion(), "");
    EXPECT_EQ(rUimFile->ObtainNAI(), "");
    EXPECT_EQ(rUimFile->ObtainMdn(), "");
    EXPECT_EQ(rUimFile->ObtainMin(), "");
    EXPECT_EQ(rUimFile->ObtainSid(), "");
    EXPECT_EQ(rUimFile->ObtainNid(), "");
    EXPECT_FALSE(rUimFile->ObtainCsimSpnDisplayCondition());
    EXPECT_EQ(rUimFile->ObtainSpnCondition(true, ""), 0);
    EXPECT_FALSE(rUimFile->UpdateVoiceMail("", ""));
    EXPECT_TRUE(rUimFile->ProcessGetImsiDone(event));
    EXPECT_TRUE(rUimFile->ProcessGetIccidDone(event));
    EXPECT_TRUE(rUimFile->ProcessGetSubscriptionDone(event));
    EXPECT_TRUE(rUimFile->ProcessGetSpnDone(event));
}

/**
 * @tc.number   Telephony_RuimFile_002
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_RuimFile_002, Function | MediumTest | Level1)
{
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    std::shared_ptr<Telephony::SimStateManager> simStateManager = std::make_shared<SimStateManager>(telRilManager);
    std::shared_ptr<RuimFile> rUimFile = std::make_shared<RuimFile>(simStateManager);
    EventFwk::MatchingSkills matchingSkills;
    matchingSkills.AddEvent(EventFwk::CommonEventSupport::COMMON_EVENT_OPERATOR_CONFIG_CHANGED);
    EventFwk::CommonEventSubscribeInfo subcribeInfo(matchingSkills);
    auto simFileManager = std::make_shared<SimFileManager>(subcribeInfo, std::weak_ptr<ITelRilManager>(telRilManager),
        std::weak_ptr<SimStateManager>(simStateManager));
    simFileManager->Init(0);
    auto event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_SIM_STATE_READY, 1);
    event = nullptr;
    EXPECT_FALSE(rUimFile->ProcessIccLocked(event));
}

/**
 * @tc.number   Telephony_IccFileController_001
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_IccFileController_001, Function | MediumTest | Level1)
{
    std::shared_ptr<IccFileController> iccFileController = std::make_shared<SimFileController>(1);
    auto event = AppExecFwk::InnerEvent::Get(0);
    iccFileController->ProcessEvent(event);
    event = nullptr;
    iccFileController->ProcessEvent(event);
    iccFileController->telRilManager_ = nullptr;
    iccFileController->ObtainElementFileForPublic(ELEMENTARY_FILE_ICCID);
    iccFileController->ObtainElementFileForPublic(ELEMENTARY_FILE_PL);
    iccFileController->ObtainElementFileForPublic(ELEMENTARY_FILE_ADN);
    iccFileController->ObtainElementFileForPublic(ELEMENTARY_FILE_FDN);
    iccFileController->ObtainElementFileForPublic(ELEMENTARY_FILE_MSISDN);
    iccFileController->ObtainElementFileForPublic(ELEMENTARY_FILE_SDN);
    iccFileController->ObtainElementFileForPublic(ELEMENTARY_FILE_EXT1);
    iccFileController->ObtainElementFileForPublic(ELEMENTARY_FILE_EXT2);
    iccFileController->ObtainElementFileForPublic(ELEMENTARY_FILE_EXT3);
    iccFileController->ObtainElementFileForPublic(ELEMENTARY_FILE_PBR);
    iccFileController->ObtainElementFileForPublic(ELEMENTARY_FILE_IMG);
    iccFileController->ObtainElementFileForPublic(0);
    EXPECT_TRUE(iccFileController->IsFixedNumberType(ELEMENTARY_FILE_ADN));
    EXPECT_TRUE(iccFileController->IsFixedNumberType(ELEMENTARY_FILE_FDN));
    EXPECT_TRUE(iccFileController->IsFixedNumberType(ELEMENTARY_FILE_USIM_ADN));
    iccFileController->SetRilManager(iccFileController->telRilManager_);
    EXPECT_TRUE(iccFileController->IsFixedNumberType(ELEMENTARY_FILE_USIM_IAP));
    EXPECT_FALSE(iccFileController->IsFixedNumberType(0));
}

/**
 * @tc.number   Telephony_CoreManagerInner_001
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_CoreManagerInner_001, Function | MediumTest | Level1)
{
    CoreManagerInner mInner;
    auto event = AppExecFwk::InnerEvent::Get(0);
    event = nullptr;
    mInner.OnInit(nullptr, nullptr, nullptr);
    mInner.SetTelRilMangerObj(nullptr);
    EXPECT_TRUE(mInner.IsInitFinished());
    EXPECT_FALSE(mInner.IsInitFinishedForTelRil());
    EXPECT_EQ(mInner.GetDefaultSlotId(), 0);
    EXPECT_GT(mInner.GetMaxSimCount(), 0);
    EXPECT_GT(mInner.SetRadioState(0, 0, 0, nullptr), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.GetRadioState(0, 0, nullptr), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.ShutDown(0, 0, nullptr), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.Dial(0, 0, "", 0, nullptr), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.Reject(0, 0, nullptr), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.Hangup(0, 0, 0, nullptr), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.Answer(0, 0, nullptr), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.GetCallList(0, 0, nullptr), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.HoldCall(0, 0, nullptr), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.UnHoldCall(0, 0, nullptr), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.SwitchCall(0, 0, nullptr), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.CombineConference(0, 0, 0, nullptr), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.SeparateConference(0, 0, 0, 0, nullptr), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.CallSupplement(0, 0, 0, nullptr), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.SetDataPermitted(0, 0, 0, nullptr), TELEPHONY_ERR_SUCCESS);
    DataProfile mDataProfile;
    EXPECT_GT(mInner.SetInitApnInfo(0, 0, mDataProfile, nullptr), TELEPHONY_ERR_SUCCESS);
    ActivateDataParam mActivateDataParam;
    EXPECT_GT(mInner.ActivatePdpContext(0, 0, mActivateDataParam, nullptr), TELEPHONY_ERR_SUCCESS);
    DeactivateDataParam mDeactivateDataParam;
    EXPECT_GT(mInner.DeactivatePdpContext(0, 0, mDeactivateDataParam, nullptr), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.GetPdpContextList(0, 0, nullptr), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.GetLinkBandwidthInfo(0, 0, 0, nullptr), TELEPHONY_ERR_SUCCESS);
    LinkBandwidthRule mLinkBandwidthRule;
    EXPECT_GT(mInner.SetLinkBandwidthReportingRule(0, 0, mLinkBandwidthRule, nullptr), TELEPHONY_ERR_SUCCESS);
    bool airplaneMode = false;
    EXPECT_GT(mInner.GetAirplaneMode(airplaneMode), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.UpdateRadioOn(INVALID_SLOTID), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.GetLinkCapability(0, 0, nullptr), TELEPHONY_ERR_SUCCESS);
    std::vector<uint8_t> buffer = {};
    EXPECT_GT(mInner.SendUrspDecodeResult(0, buffer, 0), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.SendUePolicySectionIdentifier(0, buffer, 0), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.SendImsRsdList(0, buffer, 0), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.GetNetworkSliceAllowedNssai(0, buffer, 0), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.GetNetworkSliceEhplmn(0, 0), TELEPHONY_ERR_SUCCESS);
}

/**
 * @tc.number   Telephony_CoreManagerInner_002
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_CoreManagerInner_002, Function | MediumTest | Level1)
{
    CoreManagerInner mInner;
    EXPECT_GT(mInner.StopDTMF(0, 0, 0, nullptr), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.StartDTMF(0, 0, 'A', 0, nullptr), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.SendDTMF(0, 0, 'A', 0, nullptr), TELEPHONY_ERR_SUCCESS);
    DtmfParam mDtmfParam;
    EXPECT_GT(mInner.SendDTMF(0, 0, mDtmfParam, nullptr), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.GetSignalStrength(0, 0, nullptr), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.GetCsRegStatus(0, 0, nullptr), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.GetPsRegStatus(0, 0, nullptr), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.SetCallPreferenceMode(0, 0, 0, nullptr), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.GetCallPreferenceMode(0, 0, nullptr), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.SetUssd(0, 0, "", nullptr), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.GetUssd(0, 0, nullptr), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.GetMute(0, 0, nullptr), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.SetMute(0, 0, 0, nullptr), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.GetEmergencyCallList(0, 0, nullptr), TELEPHONY_ERR_SUCCESS);
    std::vector<EmergencyCall> eccVec;
    EXPECT_GT(mInner.SetEmergencyCallList(0, 0, eccVec, nullptr), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.GetCallFailReason(0, 0, nullptr), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.GetOperatorInfo(0, 0, nullptr), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.GetNeighboringCellInfoList(0, 0, nullptr), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.GetCurrentCellInfo(0, 0, nullptr), TELEPHONY_ERR_SUCCESS);
    IccSimStatus iccStatus = IccSimStatus::ICC_CARD_ABSENT;
    EXPECT_GT(mInner.ResetSimLoadAccount(0), TELEPHONY_ERR_SUCCESS);
    auto telRilManager = std::make_shared<TelRilManager>();
    mInner.simManager_ = std::make_shared<SimManager>(telRilManager);
    EXPECT_EQ(mInner.GetSimIccStatus(0, iccStatus), TELEPHONY_ERR_SUCCESS);
    mInner.simManager_ = nullptr;
    EXPECT_GT(mInner.GetSimIccStatus(0, iccStatus), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.UpdateOperatorName(SLOT_ID_0), TELEPHONY_ERR_SUCCESS);
}

/**
 * @tc.number   Telephony_CoreManagerInner_003
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_CoreManagerInner_003, Function | MediumTest | Level1)
{
    CoreManagerInner mInner;
    mInner.DcPhysicalLinkActiveUpdate(0, true);
    int32_t radioTech;
    EXPECT_NE(mInner.GetPsRadioTech(0, radioTech), TELEPHONY_ERR_SUCCESS);
    EXPECT_NE(mInner.GetCsRadioTech(0, radioTech), TELEPHONY_ERR_SUCCESS);
    EXPECT_EQ(mInner.GetPsRegState(0), -1);
    EXPECT_EQ(mInner.GetRadioState(0), -1);
    EXPECT_EQ(mInner.GetCsRegState(0), -1);
    EXPECT_EQ(mInner.GetPsRoamingState(0), -1);
    ImsRegInfo info;
    EXPECT_EQ(mInner.GetImsRegStatus(0, ImsServiceType::TYPE_VOICE, info), TELEPHONY_ERR_LOCAL_PTR_NULL);
    sptr<NetworkInformation> networkInfo = new (std::nothrow) NetworkInformation();
    sptr<INetworkSearchCallback> callback = nullptr;
    EXPECT_FALSE(mInner.SetNetworkSelectionMode(0, 1, networkInfo, true, callback));
    EXPECT_NE(mInner.SetRadioState(0, true, 0, callback), TELEPHONY_ERR_SUCCESS);
    EXPECT_NE(mInner.GetRadioState(0, callback), TELEPHONY_ERR_SUCCESS);
    EXPECT_NE(mInner.GetNetworkSearchInformation(0, callback), TELEPHONY_ERR_SUCCESS);
    EXPECT_NE(mInner.GetNetworkSelectionMode(0, callback), TELEPHONY_ERR_SUCCESS);
    EXPECT_NE(mInner.GetPreferredNetwork(0, callback), TELEPHONY_ERR_SUCCESS);
    EXPECT_NE(mInner.SetPreferredNetwork(0, 0, callback), TELEPHONY_ERR_SUCCESS);
    std::vector<sptr<SignalInformation>> signals;
    mInner.GetSignalInfoList(0, signals);
    EXPECT_EQ(signals, std::vector<sptr<SignalInformation>>());
    std::vector<sptr<CellInformation>> cellInfo;
    EXPECT_EQ(mInner.GetCellInfoList(0, cellInfo), TELEPHONY_ERR_LOCAL_PTR_NULL);
    ASSERT_TRUE(cellInfo.empty());
    EXPECT_EQ(mInner.GetOperatorNumeric(0), std::u16string());
    std::u16string operatorName = u"";
    EXPECT_NE(mInner.GetOperatorName(0, operatorName), TELEPHONY_ERR_SUCCESS);
    EXPECT_EQ(operatorName, std::u16string());
    std::u16string countryCode = u"";
    EXPECT_NE(mInner.GetIsoCountryCodeForNetwork(0, countryCode), TELEPHONY_ERR_SUCCESS);
    EXPECT_EQ(countryCode, std::u16string());
    std::u16string result = u"";
    EXPECT_NE(mInner.GetImei(0, result), TELEPHONY_ERR_SUCCESS);
    EXPECT_EQ(result, std::u16string());
    EXPECT_NE(mInner.GetImeiSv(0, result), TELEPHONY_ERR_SUCCESS);
    EXPECT_EQ(result, std::u16string());
    EXPECT_NE(mInner.GetMeid(0, result), TELEPHONY_ERR_SUCCESS);
    EXPECT_EQ(result, std::u16string());
    EXPECT_NE(mInner.GetUniqueDeviceId(0, result), TELEPHONY_ERR_SUCCESS);
    EXPECT_EQ(result, std::u16string());
    sptr<NetworkState> networkState = nullptr;
    mInner.GetNetworkStatus(0, networkState);
    EXPECT_EQ(networkState, nullptr);
}

/**
 * @tc.number   Telephony_CoreManagerInner_004
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_CoreManagerInner_004, Function | MediumTest | Level1)
{
    CoreManagerInner mInner;
    EXPECT_EQ(mInner.ObtainSpnCondition(0, false, ""), 0);
    std::u16string result;
    EXPECT_GT(mInner.GetSimSpn(0, result), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.GetShowNumber(0, result), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.GetShowName(0, result), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.GetSimOperatorNumeric(0, result), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.GetISOCountryCodeForSim(0, result), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.GetSimIccId(0, result), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.GetIMSI(0, result), TELEPHONY_ERR_SUCCESS);
    EXPECT_EQ(mInner.GetLocaleFromDefaultSim(0), std::u16string());
    EXPECT_GT(mInner.GetSimGid1(0, result), TELEPHONY_ERR_SUCCESS);
    EXPECT_EQ(mInner.GetSimGid2(0), std::u16string());
    EXPECT_GT(mInner.GetSimTelephoneNumber(0, result), TELEPHONY_ERR_SUCCESS);
    EXPECT_EQ(mInner.GetSimTeleNumberIdentifier(0), std::u16string());
    EXPECT_GT(mInner.GetVoiceMailIdentifier(0, result), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.GetVoiceMailNumber(0, result), TELEPHONY_ERR_SUCCESS);
    EXPECT_EQ(mInner.GetSimIst(0), std::u16string());
    EXPECT_GT(mInner.SetVoiceMailInfo(0, u"", u""), TELEPHONY_ERR_SUCCESS);
    bool boolResult = false;
    EXPECT_GT(mInner.HasOperatorPrivileges(0, boolResult), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.HasSimCard(0, boolResult), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.SendEnvelopeCmd(0, ""), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.SendTerminalResponseCmd(0, ""), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.SendTerminalResponseCmd(0, ""), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.DelSmsIcc(0, 0), TELEPHONY_ERR_SUCCESS);
    EXPECT_FALSE(mInner.IsSimActive(0));
    EXPECT_GT(mInner.SetShowName(0, u""), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.IsCTSimCard(0, boolResult), TELEPHONY_ERR_SUCCESS);
    LockStatusResponse response;
    PersoLockInfo lockInfo;
    EXPECT_GT(mInner.UnlockSimLock(0, lockInfo, response), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.UnlockPin(0, "", response), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.UnlockPuk(0, "", "", response), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.AlterPin(0, "", "", response), TELEPHONY_ERR_SUCCESS);
    LockInfo options;
    EXPECT_GT(mInner.SetLockState(0, options, response), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.UnlockPin2(0, "", response), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.UnlockPuk2(0, "", "", response), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.AlterPin2(0, "", "", response), TELEPHONY_ERR_SUCCESS);
    std::shared_ptr<DiallingNumbersInfo> diallingNumbers = std::make_shared<DiallingNumbersInfo>();
    EXPECT_GT(mInner.AddIccDiallingNumbers(0, 0, diallingNumbers), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.DelIccDiallingNumbers(0, 0, diallingNumbers), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.UpdateIccDiallingNumbers(0, 0, diallingNumbers), TELEPHONY_ERR_SUCCESS);
    EXPECT_EQ(mInner.GetNrState(0), NrState::NR_STATE_NOT_SUPPORT);
    EXPECT_NE(mInner.SendUpdateCellLocationRequest(0), TELEPHONY_ERR_SUCCESS);
    EXPECT_FALSE(mInner.IsNrSupported(0));
    EXPECT_NE(mInner.NotifySimSlotsMapping(0), TELEPHONY_ERR_SUCCESS);
}

/**
 * @tc.number   Telephony_CoreManagerInner_005
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_CoreManagerInner_005, Function | MediumTest | Level1)
{
    CoreManagerInner mInner;
    OperatorConfig mOperatorConfig;
    EXPECT_GT(mInner.GetOperatorConfigs(0, mOperatorConfig), TELEPHONY_ERR_SUCCESS);
    IccAccountInfo mIccAccountInfo;
    EXPECT_GT(mInner.GetSimAccountInfo(0, mIccAccountInfo), TELEPHONY_ERR_SUCCESS);
    std::vector<IccAccountInfo> iccAccountInfoList;
    EXPECT_GT(mInner.GetActiveSimAccountInfoList(iccAccountInfoList), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.GetAllSimAccountInfoList(iccAccountInfoList), TELEPHONY_ERR_SUCCESS);
    LockState lockState;
    EXPECT_NE(mInner.GetLockState(0, LockType::PIN_LOCK, lockState), TELEPHONY_ERR_SUCCESS);
    EXPECT_NE(mInner.SendCallSetupRequestResult(0, true), TELEPHONY_ERR_SUCCESS);
    SimState simState = SimState::SIM_STATE_UNKNOWN;
    EXPECT_NE(mInner.GetSimState(0, simState), TELEPHONY_ERR_SUCCESS);
    CardType cardType = CardType::UNKNOWN_CARD;
    EXPECT_NE(mInner.GetCardType(0, cardType), TELEPHONY_ERR_SUCCESS);
    EXPECT_EQ(mInner.RefreshSimState(0), TELEPHONY_ERR_SUCCESS);
    EXPECT_NE(mInner.GetSlotId(0), TELEPHONY_ERR_SUCCESS);
    EXPECT_NE(mInner.GetSimId(0), TELEPHONY_ERR_SUCCESS);
    SimLabel simLabel;
    EXPECT_NE(mInner.InsertEsimData("01234567890123456789", 1, "中国联通"), TELEPHONY_ERR_SUCCESS);
    EXPECT_NE(mInner.SetSimLabelIndex("01234567890123456789", 0), TELEPHONY_ERR_SUCCESS);
    EXPECT_NE(mInner.GetSimLabel(0, simLabel), TELEPHONY_ERR_SUCCESS);
    mInner.SetDefaultCellularDataSlotId(0);
    mInner.SetDefaultSmsSlotId(0);
    mInner.SetDefaultVoiceSlotId(0);
    std::u16string test = u"";
    EXPECT_NE(mInner.GetOpName(0, test), TELEPHONY_ERR_SUCCESS);
    EXPECT_NE(mInner.GetOpKeyExt(0, test), TELEPHONY_ERR_SUCCESS);
    EXPECT_NE(mInner.GetOpKey(test), TELEPHONY_ERR_SUCCESS);
    EXPECT_NE(mInner.GetOpKey(0, test), TELEPHONY_ERR_SUCCESS);
    mInner.GetDefaultVoiceSlotId();
    mInner.GetDefaultSmsSlotId();
    mInner.GetDefaultCellularDataSlotId();
    int32_t slotId = INVALID_VALUE;
    mInner.GetPrimarySlotId(slotId);
    int32_t dsdsMode = INVALID_VALUE;
    mInner.GetDsdsMode(dsdsMode);
    mInner.SetDsdsMode(0);
    EXPECT_NE(mInner.SaveImsSwitch(0, 0), TELEPHONY_ERR_SUCCESS);
    int32_t imsSwitchValue = 0;
    EXPECT_NE(mInner.SaveImsSwitch(0, imsSwitchValue), TELEPHONY_ERR_SUCCESS);
    std::vector<std::string> result;
    EXPECT_EQ(mInner.ObtainAllSmsOfIcc(0), result);
    std::string pduData = "test";
    std::string smsc = "test";
    EXPECT_GT(mInner.UpdateSmsIcc(0, 0, 0, pduData, smsc), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.AddSmsToIcc(0, 0, pduData, smsc), TELEPHONY_ERR_SUCCESS);
}

/**
 * @tc.number   Telephony_CoreManagerInner_006
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_CoreManagerInner_006, Function | MediumTest | Level1)
{
    CoreManagerInner mInner;
    auto event = AppExecFwk::InnerEvent::Get(0);
    event = nullptr;
    EXPECT_GT(mInner.GetClip(0, event), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.SetClip(0, 0, event), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.GetClir(0, event), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.SetClir(0, 0, event), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.SetCallWaiting(0, 0, event), TELEPHONY_ERR_SUCCESS);
    CallTransferParam mCallTransferParam;
    EXPECT_GT(mInner.SetCallTransferInfo(0, mCallTransferParam, event), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.GetCallTransferInfo(0, 0, event), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.GetCallWaiting(0, event), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.GetCallRestriction(0, "", event), TELEPHONY_ERR_SUCCESS);
    CallRestrictionParam mCallRestrictionParam;
    EXPECT_GT(mInner.SetCallRestriction(0, mCallRestrictionParam, event), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.SetBarringPassword(0, "", "", "", event), TELEPHONY_ERR_SUCCESS);
    std::shared_ptr<AppExecFwk::EventHandler> handler;
    sptr<NetworkSearchCallBackBase> callback = nullptr;
    mInner.RegisterCellularDataObject(callback);
    mInner.UnRegisterCellularDataObject(callback);
    mInner.RegisterCellularCallObject(callback);
    mInner.UnRegisterCellularCallObject(callback);
    EXPECT_EQ(mInner.RegisterCoreNotify(INVALID_SLOTID, handler, RadioEvent::RADIO_NR_STATE_CHANGED, nullptr),
        TELEPHONY_ERR_LOCAL_PTR_NULL);
    EXPECT_EQ(mInner.UnRegisterCoreNotify(INVALID_SLOTID, handler, RadioEvent::RADIO_NR_STATE_CHANGED),
        TELEPHONY_ERR_LOCAL_PTR_NULL);
    EXPECT_EQ(mInner.RegisterCoreNotify(INVALID_SLOTID, handler, RadioEvent::RADIO_SIM_STATE_READY, nullptr),
        TELEPHONY_ERR_LOCAL_PTR_NULL);
    EXPECT_EQ(mInner.UnRegisterCoreNotify(INVALID_SLOTID, handler, RadioEvent::RADIO_SIM_GET_RADIO_PROTOCOL),
        TELEPHONY_ERR_LOCAL_PTR_NULL);
    EXPECT_EQ(mInner.RegisterCoreNotify(INVALID_SLOTID, handler, RadioEvent::RADIO_SIM_STATE_READY, nullptr),
        TELEPHONY_ERR_LOCAL_PTR_NULL);
    EXPECT_EQ(mInner.UnRegisterCoreNotify(INVALID_SLOTID, handler, RadioEvent::RADIO_SIM_GET_RADIO_PROTOCOL),
        TELEPHONY_ERR_LOCAL_PTR_NULL);
    int32_t tokenId = 123456789;
    sptr<SimAccountCallback> simAccountCallback;
    int32_t imsSwitchValue = 1;
    EXPECT_EQ(mInner.RegisterSimAccountCallback(tokenId, simAccountCallback), TELEPHONY_ERR_LOCAL_PTR_NULL);
    EXPECT_EQ(mInner.UnregisterSimAccountCallback(simAccountCallback), TELEPHONY_ERR_LOCAL_PTR_NULL);
    EXPECT_EQ(mInner.GetSmscAddr(INVALID_SLOTID, 1, handler), TELEPHONY_ERR_LOCAL_PTR_NULL);
    EXPECT_EQ(mInner.QueryImsSwitch(INVALID_SLOTID, imsSwitchValue), TELEPHONY_ERROR);
    EXPECT_GT(mInner.SetVoNRSwitch(0, 0, 0, nullptr), TELEPHONY_ERR_SUCCESS);
    EXPECT_NE(mInner.SetIccCardState(0, 0), TELEPHONY_ERR_SUCCESS);
}

/**
 * @tc.number   Telephony_CoreManagerInner_007
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_CoreManagerInner_007, Function | MediumTest | Level1)
{
    CoreManagerInner mInner;
    GsmSimMessageParam mGsmSimMessageParam;
    EXPECT_GT(mInner.SendGsmSms(0, 0, mGsmSimMessageParam, nullptr), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.SendSmsMoreMode(0, 0, mGsmSimMessageParam, nullptr), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.SendCdmaSms(0, 0, "", 0L, nullptr), TELEPHONY_ERR_SUCCESS);
    SimMessageParam mSimMessageParam;
    EXPECT_GT(mInner.AddSimMessage(0, 0, mSimMessageParam, nullptr), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.GetCdmaCBConfig(0, 0, nullptr), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.SetSmscAddr(0, 0, 0, "", nullptr), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.DelSimMessage(0, 0, 0, nullptr), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.SendSmsAck(0, 0, true, 0, nullptr), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.GetCBConfig(0, 0, nullptr), TELEPHONY_ERR_SUCCESS);
    CBConfigParam mCBConfigParam;
    EXPECT_GT(mInner.SetCBConfig(0, 0, mCBConfigParam, nullptr), TELEPHONY_ERR_SUCCESS);
    CdmaCBConfigInfoList mCdmaCBConfigInfoList;
    EXPECT_GT(mInner.SetCdmaCBConfig(0, 0, mCdmaCBConfigInfoList, nullptr), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.GetNetworkSearchInformation(0, 0, nullptr), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.GetNetworkSelectionMode(0, 0, nullptr), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.AddCdmaSimMessage(0, 0, 0, "", nullptr), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.DelCdmaSimMessage(0, 0, 0, nullptr), TELEPHONY_ERR_SUCCESS);
    CdmaSimMessageParam mCdmaSimMessageParam;
    EXPECT_GT(mInner.UpdateCdmaSimMessage(0, 0, mCdmaSimMessageParam, nullptr), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.SetPreferredNetworkPara(0, 0, 0, nullptr), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.GetPreferredNetworkPara(0, 0, nullptr), TELEPHONY_ERR_SUCCESS);
    std::vector<std::shared_ptr<DiallingNumbersInfo>> result;
    EXPECT_GT(mInner.QueryIccDiallingNumbers(0, 0, result), TELEPHONY_ERR_SUCCESS);
    EXPECT_EQ(mInner.GetCellLocation(0), nullptr);
    EXPECT_EQ(mInner.GetPhoneType(0), PhoneType::PHONE_TYPE_IS_NONE);
    EXPECT_NE(mInner.NotifyCallStatusToNetworkSearch(0, 0), TELEPHONY_ERR_SUCCESS);
    NrMode mode = NrMode::NR_MODE_UNKNOWN;
    EXPECT_NE(mInner.GetNrOptionMode(0, mode), TELEPHONY_ERR_SUCCESS);
    EXPECT_EQ(mode, NrMode::NR_MODE_UNKNOWN);
    EXPECT_EQ(mInner.GetFrequencyType(0), FrequencyType::FREQ_TYPE_UNKNOWN);
    EXPECT_NE(mInner.SetModemInit(-1, 0), TELEPHONY_ERR_SUCCESS);
}

/**
 * @tc.number   Telephony_CoreManagerInner_008
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_CoreManagerInner_008, Function | MediumTest | Level1)
{
    CoreManagerInner mInner;
    mInner.OnInit(nullptr, nullptr, nullptr);
    auto telRilManager = std::make_shared<TelRilManager>();
    mInner.SetTelRilMangerObj(telRilManager);
    std::vector<uint8_t> buffer = {1};
    EXPECT_GT(mInner.SendUrspDecodeResult(0, buffer, -1), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.SendUePolicySectionIdentifier(0, buffer, -1), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.SendImsRsdList(0, buffer, -1), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.GetNetworkSliceAllowedNssai(0, buffer, -1), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.GetNetworkSliceEhplmn(0, -1), TELEPHONY_ERR_SUCCESS);
    int32_t BASE = 0x00040000;
    int32_t event1 = BASE + 48;
    int32_t event2 = BASE + 49;
    int32_t event3 = BASE + 50;
    int32_t event4 = BASE + 51;
    int32_t event5 = BASE + 52;
    EXPECT_GT(mInner.SendUrspDecodeResult(0, buffer, event1), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.SendUePolicySectionIdentifier(0, buffer, event2), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.SendImsRsdList(0, buffer, event3), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.GetNetworkSliceAllowedNssai(0, buffer, event4), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(mInner.GetNetworkSliceEhplmn(0, event5), TELEPHONY_ERR_SUCCESS);
}

/**
 * @tc.number   Telephony_CoreManagerInner_009
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_CoreManagerInner_009, Function | MediumTest | Level1)
{
    CoreManagerInner mInner;
    mInner.OnInit(nullptr, nullptr, nullptr);
    auto telRilManager = std::make_shared<TelRilManager>();
    auto simManager = std::make_shared<SimManager>(telRilManager);
    auto networkSearchManager = std::make_shared<NetworkSearchManager>(telRilManager, simManager);

    mInner.UpdateDeviceState(SLOT_ID_0, true, true);
    EXPECT_FALSE(mInner.SetForcePreferredNetwork(SLOT_ID_0, 0));
    mInner.networkSearchManager_ = networkSearchManager;
    mInner.UpdateDeviceState(SLOT_ID_0, true, true);
    mInner.GetResidentNetworkNumeric(SLOT_ID_0);
    EXPECT_FALSE(mInner.SetForcePreferredNetwork(SLOT_ID_0, 1));
}

/**
 * @tc.number   Telephony_TagService_001
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_TagService_001, Function | MediumTest | Level1)
{
    std::vector<uint8_t> parameter = {};
    std::string testEmptyStr = "";
    std::string testStr = "12345";
    std::string testCnStr = "123456";
    std::shared_ptr<TagService> tagService = std::make_shared<TagService>(testEmptyStr);
    std::shared_ptr<TagService> tagService1 = std::make_shared<TagService>(testStr);
    std::shared_ptr<TagService> tagService2 = std::make_shared<TagService>(testCnStr);
    EXPECT_NE(tagService->GetTagCode(), 1);
    EXPECT_NE(tagService1->GetTagCode(), 1);
    EXPECT_NE(tagService2->GetTagCode(), 1);
    EXPECT_FALSE(tagService->Next());
    EXPECT_FALSE(tagService1->Next());
    EXPECT_FALSE(tagService2->Next());
}

/**
 * @tc.number   Telephony_SimSmsController_001
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_SimSmsController_001, Function | MediumTest | Level1)
{
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    std::shared_ptr<Telephony::SimStateManager> simStateManager = std::make_shared<SimStateManager>(telRilManager);
    std::shared_ptr<Telephony::SimSmsController> simSmsController = std::make_shared<SimSmsController>(simStateManager);
    auto event = AppExecFwk::InnerEvent::Get(0);
    auto eventGet = simSmsController->BuildCallerInfo(SIM_SMS_GET_COMPLETED);
    auto eventUpdate = simSmsController->BuildCallerInfo(SIM_SMS_UPDATE_COMPLETED);
    auto eventWrite = simSmsController->BuildCallerInfo(SIM_SMS_WRITE_COMPLETED);
    auto eventDelete = simSmsController->BuildCallerInfo(SIM_SMS_DELETE_COMPLETED);
    EXPECT_NO_THROW(simSmsController->ProcessLoadDone(event));
    EXPECT_NO_THROW(simSmsController->ProcessUpdateDone(event));
    EXPECT_NO_THROW(simSmsController->ProcessWriteDone(event));
    EXPECT_NO_THROW(simSmsController->ProcessDeleteDone(event));
    EXPECT_NO_THROW(simSmsController->ProcessDeleteDone(event));
    EXPECT_NO_THROW(simSmsController->ProcessEvent(event));
    EXPECT_NO_THROW(simSmsController->ProcessEvent(eventGet));
    EXPECT_NO_THROW(simSmsController->ProcessEvent(eventUpdate));
    EXPECT_NO_THROW(simSmsController->ProcessEvent(eventWrite));
    EXPECT_NO_THROW(simSmsController->ProcessEvent(eventDelete));
    EventFwk::MatchingSkills matchingSkills;
    matchingSkills.AddEvent(CommonEventSupport::COMMON_EVENT_OPERATOR_CONFIG_CHANGED);
    EventFwk::CommonEventSubscribeInfo subcribeInfo(matchingSkills);
    std::shared_ptr<Telephony::SimFileManager> simFileManager =
        std::make_shared<SimFileManager>(subcribeInfo, telRilManager, simStateManager);
    simSmsController->SetRilAndFileManager(telRilManager, simFileManager);
    simFileManager = nullptr;
    simSmsController->SetRilAndFileManager(telRilManager, simFileManager);
    telRilManager = nullptr;
    simSmsController->SetRilAndFileManager(telRilManager, simFileManager);
    std::string smsc = "";
    std::string pdu = "";
    EXPECT_FALSE(simSmsController->IsCdmaCardType());
}

/**
 * @tc.number   Telephony_MultiSimController_001
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_MultiSimController_001, Function | MediumTest | Level1)
{
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    std::vector<std::shared_ptr<Telephony::SimStateManager>> simStateManager = { nullptr, nullptr };
    std::vector<std::shared_ptr<Telephony::SimFileManager>> simFileManager = { nullptr, nullptr };
    std::shared_ptr<Telephony::MultiSimController> multiSimController =
        std::make_shared<MultiSimController>(telRilManager, simStateManager, simFileManager);
    multiSimController->SortCache();
    std::shared_ptr<RadioProtocolController> radioProtocolController = nullptr;
    EXPECT_FALSE(multiSimController->InitData(0));
    EXPECT_GT(multiSimController->SetDefaultSmsSlotId(0), TELEPHONY_ERR_SUCCESS);
    EXPECT_FALSE(multiSimController->UpdateIccAccountInfoList(
        multiSimController->activeIccAccountInfoList_, multiSimController->localCacheInfo_, false));
    EXPECT_FALSE(multiSimController->IsSimActive(0));
    EXPECT_FALSE(multiSimController->ForgetAllData());
    EXPECT_FALSE(multiSimController->ForgetAllData(0));
    EXPECT_FALSE(multiSimController->IsValidData(0));
    EXPECT_TRUE(multiSimController->AnnouncePrimarySimIdChanged(0));
    EXPECT_TRUE(multiSimController->AnnounceDefaultVoiceSimIdChanged(0));
    EXPECT_TRUE(multiSimController->AnnounceDefaultSmsSimIdChanged(0));
    EXPECT_TRUE(multiSimController->AnnounceDefaultCellularDataSimIdChanged(0));
    AAFwk::Want want;
    EXPECT_FALSE(multiSimController->PublishSimFileEvent(want, 0, ""));
    EXPECT_FALSE(multiSimController->InitShowNumber(0));
    EXPECT_FALSE(multiSimController->InitIccId(0));
    EXPECT_FALSE(multiSimController->GetListFromDataBase());
    EXPECT_FALSE(multiSimController->GetAllListFromDataBase());
    multiSimController->SortAllCache();
    EXPECT_FALSE(multiSimController->UpdateIccAccountInfoList(
        multiSimController->allIccAccountInfoList_, multiSimController->allLocalCacheInfo_, true));
}

/**
 * @tc.number   Telephony_MultiSimController_002
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_MultiSimController_002, Function | MediumTest | Level1)
{
    std::u16string testU16Str = u"";
    std::string testStr = "";
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    std::vector<std::shared_ptr<Telephony::SimStateManager>> simStateManager = { nullptr, nullptr };
    std::vector<std::shared_ptr<Telephony::SimFileManager>> simFileManager = { nullptr, nullptr };
    std::shared_ptr<Telephony::MultiSimController> multiSimController =
        std::make_shared<MultiSimController>(telRilManager, simStateManager, simFileManager);
    multiSimController->GetDefaultVoiceSlotId();
    EXPECT_NE(multiSimController->SetDefaultVoiceSlotId(0), TELEPHONY_ERR_SUCCESS);
    multiSimController->GetDefaultSmsSlotId();
    IccAccountInfo mIccAccountInfo;
    EXPECT_NE(multiSimController->GetSimAccountInfo(0, false, mIccAccountInfo), TELEPHONY_ERR_SUCCESS);
    multiSimController->GetDefaultCellularDataSlotId();
    EXPECT_EQ(multiSimController->SetDefaultCellularDataSlotId(0), TELEPHONY_ERR_SUCCESS);
    multiSimController->GetPrimarySlotId();
    EXPECT_NE(multiSimController->GetShowNumber(0, testU16Str), TELEPHONY_ERR_SUCCESS);
    EXPECT_NE(multiSimController->SetShowNumber(0, testU16Str, false), TELEPHONY_ERR_SUCCESS);
    multiSimController->SetShowNumberToDB(0, testU16Str);
    EXPECT_NE(multiSimController->GetShowName(0, testU16Str), TELEPHONY_ERR_SUCCESS);
    EXPECT_NE(multiSimController->SetShowName(0, testU16Str, false), TELEPHONY_ERR_SUCCESS);
    EXPECT_NE(multiSimController->SetActiveSimSatellite(0, 1, false), TELEPHONY_ERR_SUCCESS);
    EXPECT_NE(multiSimController->GetSlotId(1), TELEPHONY_ERR_SUCCESS);
    EXPECT_NE(multiSimController->SaveImsSwitch(0, 1), TELEPHONY_ERR_SUCCESS);
    int32_t imsSwitchValue;
    EXPECT_NE(multiSimController->QueryImsSwitch(0, imsSwitchValue), TELEPHONY_ERR_SUCCESS);
    std::vector<IccAccountInfo> iccAccountInfoList = {};
    EXPECT_NE(multiSimController->GetActiveSimAccountInfoList(false, iccAccountInfoList), TELEPHONY_ERR_SUCCESS);
    multiSimController->radioProtocolController_ = nullptr;
    EXPECT_NE(multiSimController->GetRadioProtocolTech(0), TELEPHONY_ERR_SUCCESS);
    EXPECT_NE(multiSimController->GetFirstActivedSlotId(), TELEPHONY_ERR_SUCCESS);
    EXPECT_NE(multiSimController->UpdateDataByIccId(0, testStr), TELEPHONY_ERR_SUCCESS);
    EXPECT_NE(multiSimController->InsertData(0, testStr), TELEPHONY_ERR_SUCCESS);
}

/**
 * @tc.number   Telephony_MultiSimController_003
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_MultiSimController_003, Function | MediumTest | Level1)
{
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    std::vector<std::shared_ptr<Telephony::SimStateManager>> simStateManager = { nullptr, nullptr };
    std::vector<std::shared_ptr<Telephony::SimFileManager>> simFileManager = { nullptr, nullptr };
    std::shared_ptr<Telephony::MultiSimController> multiSimController =
        std::make_shared<MultiSimController>(telRilManager, simStateManager, simFileManager);
    multiSimController->UpdateOpKeyInfo();
    EXPECT_FALSE(multiSimController->IsValidData(-1));
}

/**
 * @tc.number   Telephony_MultiSimController_004
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_MultiSimController_004, Function | MediumTest | Level1)
{
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    std::vector<std::shared_ptr<Telephony::SimStateManager>> simStateManager = { nullptr, nullptr };
    std::vector<std::shared_ptr<Telephony::SimFileManager>> simFileManager = { nullptr, nullptr };
    std::shared_ptr<Telephony::MultiSimController> multiSimController =
        std::make_shared<MultiSimController>(telRilManager, simStateManager, simFileManager);
    multiSimController->isSetActiveSimInProgress_.resize(2, 0);
    multiSimController->isSetPrimarySlotIdInProgress_ = false;
    multiSimController->radioProtocolController_ = nullptr;
    multiSimController->SetActiveCommonSim(0, 1, true, 0);
    multiSimController->isSetPrimarySlotIdInProgress_ = true;
    int32_t result = multiSimController-> SetActiveCommonSim(0, 1, true, 0);
    EXPECT_EQ(result, TELEPHONY_ERR_RIL_CMD_FAIL);
}

/**
 * @tc.number   Telephony_SimManager_001
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_SimManager_001, Function | MediumTest | Level1)
{
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    std::shared_ptr<Telephony::SimManager> simManager = std::make_shared<SimManager>(telRilManager);
    std::u16string testStr = u"";
    EXPECT_GT(simManager->SetShowNumber(0, testStr), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simManager->SetShowNumber(INVALID_SLOTID, testStr), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simManager->GetSimOperatorNumeric(0, testStr), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simManager->GetSimOperatorNumeric(INVALID_SLOTID, testStr), TELEPHONY_ERR_SUCCESS);
    bool boolResult = false;
    EXPECT_GT(simManager->HasSimCard(0, boolResult), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simManager->HasSimCard(INVALID_SLOTID, boolResult), TELEPHONY_ERR_SUCCESS);
    SimState simState = SimState::SIM_STATE_UNKNOWN;
    EXPECT_GE(simManager->GetSimState(0, simState), TELEPHONY_ERR_SUCCESS);
    EXPECT_GE(simManager->GetSimState(INVALID_SLOTID, simState), TELEPHONY_ERR_SUCCESS);
    CardType cardType = CardType::UNKNOWN_CARD;
    EXPECT_NE(simManager->GetCardType(0, cardType), TELEPHONY_ERR_SUCCESS);
    EXPECT_NE(simManager->GetCardType(INVALID_SLOTID, cardType), TELEPHONY_ERR_SUCCESS);
    EXPECT_NE(simManager->SetModemInit(0, 0), TELEPHONY_ERR_SUCCESS);
    EXPECT_NE(simManager->SetModemInit(INVALID_SLOTID, 0), TELEPHONY_ERR_SUCCESS);
    LockType mLockType = LockType::PIN_LOCK;
    LockState lockState;
    EXPECT_NE(simManager->GetLockState(0, mLockType, lockState), TELEPHONY_ERR_SUCCESS);
    EXPECT_NE(simManager->GetLockState(INVALID_SLOTID, mLockType, lockState), TELEPHONY_ERR_SUCCESS);
    EXPECT_NE(simManager->RefreshSimState(0), TELEPHONY_ERR_SUCCESS);
    EXPECT_NE(simManager->RefreshSimState(INVALID_SLOTID), TELEPHONY_ERR_SUCCESS);
    IccSimStatus iccStatus = IccSimStatus::ICC_CARD_ABSENT;
    EXPECT_EQ(simManager->GetSimIccStatus(3, iccStatus), TELEPHONY_ERR_SUCCESS);
    EXPECT_EQ(simManager->GetSimIccStatus(0, iccStatus), TELEPHONY_ERR_SUCCESS);
}

/**
 * @tc.number   Telephony_SimManager_002
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_SimManager_002, Function | MediumTest | Level1)
{
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    std::shared_ptr<Telephony::SimManager> simManager = std::make_shared<SimManager>(telRilManager);
    std::u16string testStr = u"";
    EXPECT_GT(simManager->GetISOCountryCodeForSim(0, testStr), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simManager->GetISOCountryCodeForSim(INVALID_SLOTID, testStr), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simManager->SetShowName(0, testStr), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simManager->SetShowName(INVALID_SLOTID, testStr), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simManager->GetSimSpn(0, testStr), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simManager->GetSimSpn(INVALID_SLOTID, testStr), TELEPHONY_ERR_SUCCESS);
    std::u16string result = u"";
    EXPECT_GT(simManager->GetSimIccId(0, result), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simManager->GetSimIccId(INVALID_SLOTID, result), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simManager->GetIMSI(0, result), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simManager->GetIMSI(INVALID_SLOTID, result), TELEPHONY_ERR_SUCCESS);
    EXPECT_EQ(simManager->GetLocaleFromDefaultSim(0), testStr);
    EXPECT_EQ(simManager->GetLocaleFromDefaultSim(INVALID_SLOTID), testStr);
    EXPECT_GT(simManager->GetSimGid1(0, result), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simManager->GetSimGid1(INVALID_SLOTID, result), TELEPHONY_ERR_SUCCESS);
    EXPECT_EQ(simManager->GetSimGid2(0), testStr);
    EXPECT_EQ(simManager->GetSimGid2(INVALID_SLOTID), testStr);
    EXPECT_EQ(simManager->GetSimEons(0, "46001", 1, true), testStr);
    EXPECT_EQ(simManager->GetSimEons(INVALID_SLOTID, "46001", 1, true), testStr);
    EXPECT_GT(simManager->GetShowNumber(0, result), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simManager->GetShowNumber(INVALID_SLOTID, result), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simManager->GetShowName(0, result), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simManager->GetShowName(INVALID_SLOTID, result), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simManager->GetSimTelephoneNumber(0, result), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simManager->GetSimTelephoneNumber(INVALID_SLOTID, result), TELEPHONY_ERR_SUCCESS);
    EXPECT_EQ(simManager->GetSimTeleNumberIdentifier(0), testStr);
    EXPECT_EQ(simManager->GetSimTeleNumberIdentifier(INVALID_SLOTID), testStr);
    EXPECT_GT(simManager->GetVoiceMailIdentifier(0, result), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simManager->GetVoiceMailIdentifier(INVALID_SLOTID, result), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simManager->GetVoiceMailNumber(0, result), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simManager->GetVoiceMailNumber(INVALID_SLOTID, result), TELEPHONY_ERR_SUCCESS);
    EXPECT_EQ(simManager->GetSimIst(0), testStr);
    EXPECT_EQ(simManager->GetSimIst(INVALID_SLOTID), testStr);
    EXPECT_NE(simManager->GetOpName(0, testStr), TELEPHONY_ERR_SUCCESS);
    EXPECT_NE(simManager->GetOpName(INVALID_SLOTID, testStr), TELEPHONY_ERR_SUCCESS);
    EXPECT_NE(simManager->GetOpKey(0, testStr), TELEPHONY_ERR_SUCCESS);
    EXPECT_NE(simManager->GetOpKey(INVALID_SLOTID, testStr), TELEPHONY_ERR_SUCCESS);
    EXPECT_NE(simManager->GetOpKeyExt(0, testStr), TELEPHONY_ERR_SUCCESS);
    EXPECT_NE(simManager->GetOpKeyExt(INVALID_SLOTID, testStr), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simManager->SetVoiceMailInfo(0, testStr, testStr), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simManager->SetVoiceMailInfo(INVALID_SLOTID, testStr, testStr), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simManager->SetActiveSim(0, 1), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simManager->SetActiveSim(INVALID_SLOTID, 1), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simManager->SetActiveSimSatellite(0, 1), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simManager->SetActiveSimSatellite(INVALID_SLOTID, 1), TELEPHONY_ERR_SUCCESS);
}

/**
 * @tc.number   Telephony_SimManager_003
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_SimManager_003, Function | MediumTest | Level1)
{
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    std::shared_ptr<Telephony::SimManager> simManager = std::make_shared<SimManager>(telRilManager);
    simManager->SetDefaultVoiceSlotId(0);
    simManager->SetDefaultVoiceSlotId(INVALID_SLOTID);
    simManager->SetDefaultSmsSlotId(0);
    simManager->SetDefaultSmsSlotId(INVALID_SLOTID);
    simManager->SetDefaultCellularDataSlotId(0);
    simManager->SetDefaultCellularDataSlotId(INVALID_SLOTID);
    simManager->SetPrimarySlotId(INVALID_SLOTID);
    simManager->GetDefaultVoiceSlotId();
    simManager->GetDefaultSmsSlotId();
    simManager->GetDefaultCellularDataSlotId();
    int32_t slotId = INVALID_VALUE;
    simManager->GetPrimarySlotId(slotId);
    EXPECT_NE(simManager->GetSlotId(1), TELEPHONY_ERR_SUCCESS);
    EXPECT_NE(simManager->GetSimId(0), TELEPHONY_ERR_SUCCESS);
    SimLabel simLabel;
    EXPECT_NE(simManager->InsertEsimData("01234567890123456789", 1, "中国联通"), TELEPHONY_ERR_SUCCESS);
    EXPECT_NE(simManager->SetSimLabelIndex("01234567890123456789", 0), TELEPHONY_ERR_SUCCESS);
    EXPECT_NE(simManager->GetSimLabel(0, simLabel), TELEPHONY_ERR_SUCCESS);
    std::vector<IccAccountInfo> iccAccountInfoList;
    EXPECT_GT(simManager->GetActiveSimAccountInfoList(false, iccAccountInfoList), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simManager->GetAllSimAccountInfoList(false, iccAccountInfoList), TELEPHONY_ERR_SUCCESS);
    OperatorConfig mOperatorConfig;
    EXPECT_GT(simManager->GetOperatorConfigs(0, mOperatorConfig), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simManager->GetOperatorConfigs(INVALID_SLOTID, mOperatorConfig), TELEPHONY_ERR_SUCCESS);
    bool boolResult = false;
    EXPECT_GT(simManager->HasOperatorPrivileges(0, boolResult), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simManager->HasOperatorPrivileges(INVALID_SLOTID, boolResult), TELEPHONY_ERR_SUCCESS);
    int32_t testRadioProtocolTech = static_cast<int32_t>(RadioProtocolTech::RADIO_PROTOCOL_TECH_UNKNOWN);
    EXPECT_EQ(simManager->GetRadioProtocolTech(0), testRadioProtocolTech);
    EXPECT_EQ(simManager->GetRadioProtocolTech(INVALID_SLOTID), testRadioProtocolTech);
    EXPECT_GT(simManager->SendEnvelopeCmd(0, ""), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simManager->SendEnvelopeCmd(INVALID_SLOTID, ""), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simManager->SendTerminalResponseCmd(0, ""), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simManager->SendTerminalResponseCmd(INVALID_SLOTID, ""), TELEPHONY_ERR_SUCCESS);
    EXPECT_NE(simManager->SendCallSetupRequestResult(0, true), TELEPHONY_ERR_SUCCESS);
    EXPECT_NE(simManager->SendCallSetupRequestResult(INVALID_SLOTID, true), TELEPHONY_ERR_SUCCESS);
    std::string testAddress = "";
    EXPECT_GT(simManager->AddSmsToIcc(0, 1, testAddress, testAddress), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simManager->AddSmsToIcc(INVALID_SLOTID, 1, testAddress, testAddress), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simManager->UpdateSmsIcc(0, 1, 1, testAddress, testAddress), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simManager->UpdateSmsIcc(INVALID_SLOTID, 1, 1, testAddress, testAddress), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simManager->DelSmsIcc(0, 1), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simManager->DelSmsIcc(INVALID_SLOTID, 1), TELEPHONY_ERR_SUCCESS);
    std::vector<std::string> result;
    EXPECT_EQ(simManager->ObtainAllSmsOfIcc(0), result);
    EXPECT_EQ(simManager->ObtainAllSmsOfIcc(INVALID_SLOTID), result);
}

/**
 * @tc.number   Telephony_SimManager_004
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_SimManager_004, Function | MediumTest | Level1)
{
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    std::shared_ptr<Telephony::SimManager> simManager = std::make_shared<SimManager>(telRilManager);
    std::vector<std::shared_ptr<DiallingNumbersInfo>> list;
    EXPECT_GT(simManager->QueryIccDiallingNumbers(0, 1, list), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simManager->QueryIccDiallingNumbers(INVALID_SLOTID, 1, list), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simManager->AddIccDiallingNumbers(0, 1, nullptr), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simManager->AddIccDiallingNumbers(INVALID_SLOTID, 1, nullptr), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simManager->DelIccDiallingNumbers(0, 1, nullptr), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simManager->DelIccDiallingNumbers(INVALID_SLOTID, 1, nullptr), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simManager->UpdateIccDiallingNumbers(0, 1, nullptr), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simManager->UpdateIccDiallingNumbers(INVALID_SLOTID, 1, nullptr), TELEPHONY_ERR_SUCCESS);
    simManager->RegisterCoreNotify(0, nullptr, 1);
    simManager->RegisterCoreNotify(INVALID_SLOTID, nullptr, 1);
    simManager->UnRegisterCoreNotify(0, nullptr, 1);
    simManager->UnRegisterCoreNotify(INVALID_SLOTID, nullptr, 1);
    EXPECT_NE(simManager->SaveImsSwitch(0, 1), TELEPHONY_ERR_SUCCESS);
    EXPECT_NE(simManager->SaveImsSwitch(INVALID_SLOTID, 1), TELEPHONY_ERR_SUCCESS);
    int32_t imsSwitchValue;
    int32_t tokenId = -1;
    EXPECT_NE(simManager->QueryImsSwitch(0, imsSwitchValue), TELEPHONY_ERR_SUCCESS);
    EXPECT_NE(simManager->QueryImsSwitch(INVALID_SLOTID, imsSwitchValue), TELEPHONY_ERR_SUCCESS);
    EXPECT_NE(simManager->RegisterSimAccountCallback(tokenId, nullptr), TELEPHONY_ERR_SUCCESS);
    EXPECT_NE(simManager->UnregisterSimAccountCallback(nullptr), TELEPHONY_ERR_SUCCESS);
    int32_t dsdsMode = INVALID_VALUE;
    simManager->GetDsdsMode(dsdsMode);
    simManager->SetDsdsMode(0);
    std::string testString = "";
    EXPECT_NE(simManager->ObtainSpnCondition(0, true, testString), TELEPHONY_ERR_SUCCESS);
    EXPECT_NE(simManager->ObtainSpnCondition(INVALID_SLOTID, true, testString), TELEPHONY_ERR_SUCCESS);
    SimAuthenticationResponse mResponse;
    EXPECT_NE(simManager->SimAuthentication(0, static_cast<AuthType>(0), "", mResponse), TELEPHONY_ERR_SUCCESS);
    EXPECT_NE(
        simManager->SimAuthentication(INVALID_SLOTID, static_cast<AuthType>(0), "", mResponse), TELEPHONY_ERR_SUCCESS);
    EXPECT_NE(simManager->SimAuthentication(0, AuthType::SIM_AUTH_EAP_SIM_TYPE, "", mResponse), TELEPHONY_ERR_SUCCESS);
    EXPECT_NE(simManager->SimAuthentication(INVALID_SLOTID, AuthType::SIM_AUTH_EAP_SIM_TYPE, "", mResponse),
        TELEPHONY_ERR_SUCCESS);
    EXPECT_NE(simManager->SendSimMatchedOperatorInfo(0, 0, "NULL", ""), TELEPHONY_ERR_SUCCESS);
}

/**
 * @tc.number   Telephony_SimManager_005
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_SimManager_005, Function | MediumTest | Level1)
{
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    std::shared_ptr<Telephony::SimManager> simManager = std::make_shared<SimManager>(telRilManager);
    std::string password = "1234";
    LockStatusResponse mLockStatusResponse;
    EXPECT_GT(simManager->UnlockPin(0, password, mLockStatusResponse), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simManager->UnlockPin(INVALID_SLOTID, password, mLockStatusResponse), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simManager->UnlockPuk(0, password, password, mLockStatusResponse), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simManager->UnlockPuk(INVALID_SLOTID, password, password, mLockStatusResponse), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simManager->AlterPin(0, password, password, mLockStatusResponse), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simManager->AlterPin(INVALID_SLOTID, password, password, mLockStatusResponse), TELEPHONY_ERR_SUCCESS);
    LockInfo mLockInfo;
    EXPECT_GT(simManager->SetLockState(0, mLockInfo, mLockStatusResponse), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simManager->SetLockState(INVALID_SLOTID, mLockInfo, mLockStatusResponse), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simManager->UnlockPin2(0, password, mLockStatusResponse), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simManager->UnlockPin2(INVALID_SLOTID, password, mLockStatusResponse), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simManager->UnlockPuk2(0, password, password, mLockStatusResponse), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simManager->UnlockPuk2(INVALID_SLOTID, password, password, mLockStatusResponse), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simManager->AlterPin2(0, password, password, mLockStatusResponse), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simManager->AlterPin2(INVALID_SLOTID, password, password, mLockStatusResponse), TELEPHONY_ERR_SUCCESS);
    PersoLockInfo mPersoLockInfo;
    EXPECT_GT(simManager->UnlockSimLock(0, mPersoLockInfo, mLockStatusResponse), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simManager->UnlockSimLock(INVALID_SLOTID, mPersoLockInfo, mLockStatusResponse), TELEPHONY_ERR_SUCCESS);
    EXPECT_FALSE(simManager->IsSimActive(0));
    EXPECT_FALSE(simManager->IsSimActive(INVALID_SLOTID));
    IccAccountInfo mIccAccountInfo;
    EXPECT_GT(simManager->GetSimAccountInfo(0, false, mIccAccountInfo), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simManager->GetSimAccountInfo(INVALID_SLOTID, false, mIccAccountInfo), TELEPHONY_ERR_SUCCESS);
    bool isCTSimCard = false;
    EXPECT_GT(simManager->IsCTSimCard(0, isCTSimCard), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simManager->IsCTSimCard(INVALID_SLOTID, isCTSimCard), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simManager->NotifySimSlotsMapping(INVALID_SLOTID), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simManager->SetIccCardState(INVALID_SLOTID, 0), TELEPHONY_ERR_SUCCESS);
}

/**
 * @tc.number   Telephony_SimStateManager_001
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_SimStateManager_001, Function | MediumTest | Level1)
{
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    std::shared_ptr<Telephony::SimStateManager> simStateManager = std::make_shared<SimStateManager>(telRilManager);
    simStateManager->RegisterCoreNotify(nullptr, 1);
    simStateManager->UnRegisterCoreNotify(nullptr, 1);
    EXPECT_FALSE(simStateManager->HasSimCard());
    std::string password = "1234";
    LockStatusResponse mLockStatusResponse;
    EXPECT_GT(simStateManager->UnlockPin(0, password, mLockStatusResponse), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simStateManager->UnlockPuk(0, password, password, mLockStatusResponse), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simStateManager->AlterPin(0, password, password, mLockStatusResponse), TELEPHONY_ERR_SUCCESS);
    LockInfo mLockInfo;
    EXPECT_GT(simStateManager->SetLockState(0, mLockInfo, mLockStatusResponse), TELEPHONY_ERR_SUCCESS);
    mLockInfo.lockType = LockType::PIN_LOCK;
    mLockInfo.lockState = LockState::LOCK_ERROR;
    EXPECT_GT(simStateManager->SetLockState(0, mLockInfo, mLockStatusResponse), TELEPHONY_ERR_SUCCESS);
    mLockInfo.lockState = LockState::LOCK_ON;
    EXPECT_GT(simStateManager->SetLockState(0, mLockInfo, mLockStatusResponse), TELEPHONY_ERR_SUCCESS);
    LockInfo testLockInfo;
    LockState lockState;
    EXPECT_GT(simStateManager->GetLockState(0, testLockInfo.lockType, lockState), TELEPHONY_ERR_SUCCESS);
    LockType lockType = LockType::PIN_LOCK;
    EXPECT_GT(simStateManager->GetLockState(0, lockType, lockState), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simStateManager->UnlockPin2(0, password, mLockStatusResponse), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simStateManager->UnlockPuk2(0, password, password, mLockStatusResponse), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(simStateManager->AlterPin2(0, password, password, mLockStatusResponse), TELEPHONY_ERR_SUCCESS);
    PersoLockInfo mPersoLockInfo;
    EXPECT_GT(simStateManager->UnlockSimLock(0, mPersoLockInfo, mLockStatusResponse), TELEPHONY_ERR_SUCCESS);
    SimAuthenticationResponse mResponse;
    EXPECT_NE(
        simStateManager->SimAuthentication(0, AuthType::SIM_AUTH_EAP_SIM_TYPE, "", mResponse), TELEPHONY_ERR_SUCCESS);
    auto simStateManagerTwo = std::make_shared<SimStateManager>(telRilManager);
    simStateManager->simStateHandle_ = std::make_shared<SimStateHandle>(simStateManagerTwo);
    EXPECT_GE(simStateManager->GetCardType(), CardType::UNKNOWN_CARD);
    EXPECT_GT(simStateManager->UnlockSimLock(0, mPersoLockInfo, mLockStatusResponse), TELEPHONY_ERR_SUCCESS);
    simStateManager->simStateHandle_->GetSimIccStatus();
    auto ret = simStateManager->GetSimIccStatus();
    EXPECT_EQ(static_cast<int>(ret), -1);
    simStateManager->simStateHandle_ = nullptr;
    ret = simStateManager->GetSimIccStatus();
    EXPECT_EQ(static_cast<int>(ret), -1);
}

/**
 * @tc.number   Telephony_SimStateHandle_001
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_SimStateHandle_001, Function | MediumTest | Level1)
{
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    std::shared_ptr<Telephony::SimStateManager> simStateManager = std::make_shared<SimStateManager>(telRilManager);
    auto simStateHandle = std::make_shared<SimStateHandle>(simStateManager);
    simStateHandle->iccState_.simStatus_ = 1;
    simStateHandle->slotId_ = INVALID_SLOTID;
    AppExecFwk::InnerEvent::Pointer event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_STATE_CHANGED);
    simStateHandle->ProcessEvent(event);
    event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_SIM_STATE_CHANGE);
    simStateHandle->ProcessEvent(event);
    event = AppExecFwk::InnerEvent::Get(MSG_SIM_GET_ICC_STATUS_DONE);
    simStateHandle->ProcessEvent(event);
    event = AppExecFwk::InnerEvent::Get(MSG_SIM_UNLOCK_PIN_DONE);
    simStateHandle->ProcessEvent(event);
    event = AppExecFwk::InnerEvent::Get(MSG_SIM_UNLOCK_PUK_DONE);
    simStateHandle->ProcessEvent(event);
    event = AppExecFwk::InnerEvent::Get(MSG_SIM_CHANGE_PIN_DONE);
    simStateHandle->ProcessEvent(event);
    event = AppExecFwk::InnerEvent::Get(MSG_SIM_UNLOCK_PIN2_DONE);
    simStateHandle->ProcessEvent(event);
    event = AppExecFwk::InnerEvent::Get(MSG_SIM_UNLOCK_PUK2_DONE);
    simStateHandle->ProcessEvent(event);
    event = AppExecFwk::InnerEvent::Get(MSG_SIM_CHANGE_PIN2_DONE);
    simStateHandle->ProcessEvent(event);
    event = AppExecFwk::InnerEvent::Get(MSG_SIM_UNLOCK_SIMLOCK_DONE);
    simStateHandle->ProcessEvent(event);
    event = AppExecFwk::InnerEvent::Get(MSG_SIM_ENABLE_PIN_DONE);
    simStateHandle->ProcessEvent(event);
    event = AppExecFwk::InnerEvent::Get(MSG_SIM_CHECK_PIN_DONE);
    simStateHandle->ProcessEvent(event);
    event = AppExecFwk::InnerEvent::Get(MSG_SIM_GET_REALTIME_ICC_STATUS_DONE);
    simStateHandle->ProcessEvent(event);
    event = AppExecFwk::InnerEvent::Get(MSG_SIM_AUTHENTICATION_DONE);
    simStateHandle->ProcessEvent(event);
    event = AppExecFwk::InnerEvent::Get(MSG_SIM_SEND_NCFG_OPER_INFO_DONE);
    simStateHandle->ProcessEvent(event);
    EXPECT_EQ(simStateHandle->GetAidByCardType(CardType::SINGLE_MODE_RUIM_CARD), CDMA_FAKE_AID);
    EXPECT_EQ(simStateHandle->GetAidByCardType(CardType::SINGLE_MODE_SIM_CARD), GSM_FAKE_AID);
    EXPECT_EQ(simStateHandle->GetAidByCardType(CardType::DUAL_MODE_CG_CARD), GSM_FAKE_AID);
    EXPECT_EQ(simStateHandle->GetAidByCardType(CardType::CT_NATIONAL_ROAMING_CARD), GSM_FAKE_AID);
    EXPECT_EQ(simStateHandle->GetAidByCardType(CardType::CU_DUAL_MODE_CARD), GSM_FAKE_AID);
    EXPECT_EQ(simStateHandle->GetAidByCardType(CardType::DUAL_MODE_TELECOM_LTE_CARD), GSM_FAKE_AID);
    EXPECT_EQ(simStateHandle->GetAidByCardType(CardType::DUAL_MODE_UG_CARD), GSM_FAKE_AID);
    EXPECT_EQ(simStateHandle->GetAidByCardType(CardType::UNKNOWN_CARD), USIM_AID);
}

/**
 * @tc.number   Telephony_SimStateHandle_002
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_SimStateHandle_002, Function | MediumTest | Level1)
{
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    std::shared_ptr<Telephony::SimStateManager> simStateManager = std::make_shared<SimStateManager>(telRilManager);
    auto simStateHandle = std::make_shared<SimStateHandle>(simStateManager);
    simStateHandle->iccState_.simStatus_ = 1;
    simStateHandle->slotId_ = INVALID_SLOTID;
    AppExecFwk::InnerEvent::Pointer event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_STATE_CHANGED);
    simStateHandle->IsRadioStateUnavailable(event);
    auto radioState = std::make_shared<Int32Parcel>(ModemPowerState::CORE_SERVICE_POWER_NOT_AVAILABLE);
    event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_STATE_CHANGED, radioState);
    EXPECT_TRUE(simStateHandle->IsRadioStateUnavailable(event));
}

/**
 * @tc.number   Telephony_TelRilModem_001
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_TelRilModem_001, Function | MediumTest | Level1)
{
    std::shared_ptr<ObserverHandler> observerHandler = std::make_shared<ObserverHandler>();

    std::vector<std::shared_ptr<ObserverHandler>> observerHandlers;
    observerHandlers.push_back(observerHandler);
    std::shared_ptr<TelRilModem> telRilModem =
        std::make_shared<TelRilModem>(SLOT_ID_0, nullptr, observerHandlers[SLOT_ID_0], nullptr);
    if (telRilModem != nullptr) {
        telRilModem->RadioStateUpdated(ModemPowerState::CORE_SERVICE_POWER_ON);
        EXPECT_EQ(telRilModem->OnRilAdapterHostDied(), TELEPHONY_ERR_SUCCESS);
    }
}

/**
 * @tc.number   Telephony_TelRilModem_002
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_TelRilModem_002, Function | MediumTest | Level1)
{
    std::shared_ptr<ObserverHandler> observerHandler = std::make_shared<ObserverHandler>();
    std::vector<std::shared_ptr<ObserverHandler>> observerHandlers;
    observerHandlers.push_back(observerHandler);
    std::shared_ptr<TelRilModem> telRilModem =
        std::make_shared<TelRilModem>(SLOT_ID_0, nullptr, observerHandlers[SLOT_ID_0], nullptr);
    if (telRilModem != nullptr) {
        int32_t state = 1;
        EXPECT_EQ(telRilModem->NcfgFinishedResult(state), TELEPHONY_ERR_SUCCESS);
    }
}

/**
 * @tc.number   Telephony_SimFileController_001
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_SimFileController_001, Function | MediumTest | Level1)
{
    auto simFileController = std::make_shared<SimFileController>(INVALID_SLOTID);
    EXPECT_EQ(simFileController->ObtainElementFilePath(0), "");
    EXPECT_NE(simFileController->ObtainElementFilePath(ELEMENTARY_FILE_SMS), "");
    EXPECT_NE(simFileController->ObtainElementFilePath(ELEMENTARY_FILE_VOICE_MAIL_INDICATOR_CPHS), "");
    EXPECT_NE(simFileController->ObtainElementFilePath(ELEMENTARY_FILE_CFF_CPHS), "");
    EXPECT_NE(simFileController->ObtainElementFilePath(ELEMENTARY_FILE_SPN_CPHS), "");
    EXPECT_NE(simFileController->ObtainElementFilePath(ELEMENTARY_FILE_INFO_CPHS), "");
    EXPECT_NE(simFileController->ObtainElementFilePath(ELEMENTARY_FILE_MAILBOX_CPHS), "");
    EXPECT_NE(simFileController->ObtainElementFilePath(ELEMENTARY_FILE_SPN_SHORT_CPHS), "");
    EXPECT_NE(simFileController->ObtainElementFilePath(ELEMENTARY_FILE_SST), "");
    EXPECT_NE(simFileController->ObtainElementFilePath(ELEMENTARY_FILE_GID1), "");
    EXPECT_NE(simFileController->ObtainElementFilePath(ELEMENTARY_FILE_GID2), "");
    EXPECT_NE(simFileController->ObtainElementFilePath(ELEMENTARY_FILE_SPN), "");
    EXPECT_NE(simFileController->ObtainElementFilePath(ELEMENTARY_FILE_AD), "");
    EXPECT_NE(simFileController->ObtainElementFilePath(ELEMENTARY_FILE_PNN), "");
    EXPECT_NE(simFileController->ObtainElementFilePath(ELEMENTARY_FILE_OPL), "");
    EXPECT_NE(simFileController->ObtainElementFilePath(ELEMENTARY_FILE_MBDN), "");
    EXPECT_NE(simFileController->ObtainElementFilePath(ELEMENTARY_FILE_EXT6), "");
    EXPECT_NE(simFileController->ObtainElementFilePath(ELEMENTARY_FILE_MBI), "");
    EXPECT_NE(simFileController->ObtainElementFilePath(ELEMENTARY_FILE_MWIS), "");
    EXPECT_NE(simFileController->ObtainElementFilePath(ELEMENTARY_FILE_CFIS), "");
    EXPECT_NE(simFileController->ObtainElementFilePath(ELEMENTARY_FILE_CSP_CPHS), "");
}

/**
 * @tc.number   Telephony_CsimFileController_001
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_CsimFileController_001, Function | MediumTest | Level1)
{
    auto csimFileController = std::make_shared<CsimFileController>(INVALID_SLOTID);
    EXPECT_NE(csimFileController->ObtainElementFilePath(0), "");
    EXPECT_NE(csimFileController->ObtainElementFilePath(ELEMENTARY_FILE_SMS), "");
    EXPECT_NE(csimFileController->ObtainElementFilePath(ELEMENTARY_FILE_CST), "");
    EXPECT_NE(csimFileController->ObtainElementFilePath(ELEMENTARY_FILE_FDN), "");
    EXPECT_NE(csimFileController->ObtainElementFilePath(ELEMENTARY_FILE_MSISDN), "");
    EXPECT_NE(csimFileController->ObtainElementFilePath(ELEMENTARY_FILE_RUIM_SPN), "");
    EXPECT_NE(csimFileController->ObtainElementFilePath(ELEMENTARY_FILE_CSIM_LI), "");
    EXPECT_NE(csimFileController->ObtainElementFilePath(ELEMENTARY_FILE_CSIM_MDN), "");
    EXPECT_NE(csimFileController->ObtainElementFilePath(ELEMENTARY_FILE_CSIM_IMSIM), "");
    EXPECT_NE(csimFileController->ObtainElementFilePath(ELEMENTARY_FILE_CSIM_CDMAHOME), "");
    EXPECT_NE(csimFileController->ObtainElementFilePath(ELEMENTARY_FILE_CSIM_EPRL), "");
    EXPECT_NE(csimFileController->ObtainElementFilePath(ELEMENTARY_FILE_CSIM_MIPUPP), "");
    EXPECT_NE(csimFileController->ObtainElementFilePath(ELEMENTARY_FILE_PCSCF), "");
}

/**
 * @tc.number   Telephony_RuimFileController_001
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_RuimFileController_001, Function | MediumTest | Level1)
{
    auto rUimFileController = std::make_shared<RuimFileController>(INVALID_SLOTID);
    EXPECT_EQ(rUimFileController->ObtainElementFilePath(0), "");
    EXPECT_NE(rUimFileController->ObtainElementFilePath(ELEMENTARY_FILE_SMS), "");
    EXPECT_NE(rUimFileController->ObtainElementFilePath(ELEMENTARY_FILE_CST), "");
    EXPECT_NE(rUimFileController->ObtainElementFilePath(ELEMENTARY_FILE_RUIM_SPN), "");
    EXPECT_NE(rUimFileController->ObtainElementFilePath(ELEMENTARY_FILE_CSIM_LI), "");
    EXPECT_NE(rUimFileController->ObtainElementFilePath(ELEMENTARY_FILE_CSIM_MDN), "");
    EXPECT_NE(rUimFileController->ObtainElementFilePath(ELEMENTARY_FILE_CSIM_IMSIM), "");
    EXPECT_NE(rUimFileController->ObtainElementFilePath(ELEMENTARY_FILE_CSIM_CDMAHOME), "");
    EXPECT_NE(rUimFileController->ObtainElementFilePath(ELEMENTARY_FILE_CSIM_EPRL), "");
}

/**
 * @tc.number   Telephony_IsimFileController_001
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_IsimFileController_001, Function | MediumTest | Level1)
{
    auto iSimFileController = std::make_shared<IsimFileController>(INVALID_SLOTID);
    EXPECT_EQ(iSimFileController->ObtainElementFilePath(0), "");
    EXPECT_NE(iSimFileController->ObtainElementFilePath(ELEMENTARY_FILE_IMPI), "");
    EXPECT_NE(iSimFileController->ObtainElementFilePath(ELEMENTARY_FILE_IMPU), "");
    EXPECT_NE(iSimFileController->ObtainElementFilePath(ELEMENTARY_FILE_DOMAIN), "");
    EXPECT_NE(iSimFileController->ObtainElementFilePath(ELEMENTARY_FILE_IST), "");
    EXPECT_NE(iSimFileController->ObtainElementFilePath(ELEMENTARY_FILE_PCSCF), "");
}

/**
 * @tc.number   Telephony_UsimFileController_001
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_UsimFileController_001, Function | MediumTest | Level1)
{
    auto uSimFileController = std::make_shared<UsimFileController>(INVALID_SLOTID);
    EXPECT_NE(uSimFileController->ObtainElementFilePath(0), "");
    EXPECT_NE(uSimFileController->ObtainElementFilePath(ELEMENTARY_FILE_IMPI), "");
    EXPECT_NE(uSimFileController->ObtainElementFilePath(ELEMENTARY_FILE_SMS), "");
    EXPECT_NE(uSimFileController->ObtainElementFilePath(ELEMENTARY_FILE_EXT5), "");
    EXPECT_NE(uSimFileController->ObtainElementFilePath(ELEMENTARY_FILE_EXT6), "");
    EXPECT_NE(uSimFileController->ObtainElementFilePath(ELEMENTARY_FILE_MWIS), "");
    EXPECT_NE(uSimFileController->ObtainElementFilePath(ELEMENTARY_FILE_MBI), "");
    EXPECT_NE(uSimFileController->ObtainElementFilePath(ELEMENTARY_FILE_SPN), "");
    EXPECT_NE(uSimFileController->ObtainElementFilePath(ELEMENTARY_FILE_AD), "");
    EXPECT_NE(uSimFileController->ObtainElementFilePath(ELEMENTARY_FILE_MBDN), "");
    EXPECT_NE(uSimFileController->ObtainElementFilePath(ELEMENTARY_FILE_PNN), "");
    EXPECT_NE(uSimFileController->ObtainElementFilePath(ELEMENTARY_FILE_OPL), "");
    EXPECT_NE(uSimFileController->ObtainElementFilePath(ELEMENTARY_FILE_OPL5G), "");
    EXPECT_NE(uSimFileController->ObtainElementFilePath(ELEMENTARY_FILE_SPDI), "");
    EXPECT_NE(uSimFileController->ObtainElementFilePath(ELEMENTARY_FILE_SST), "");
    EXPECT_NE(uSimFileController->ObtainElementFilePath(ELEMENTARY_FILE_CFIS), "");
    EXPECT_NE(uSimFileController->ObtainElementFilePath(ELEMENTARY_FILE_MAILBOX_CPHS), "");
    EXPECT_NE(uSimFileController->ObtainElementFilePath(ELEMENTARY_FILE_VOICE_MAIL_INDICATOR_CPHS), "");
    EXPECT_NE(uSimFileController->ObtainElementFilePath(ELEMENTARY_FILE_CFF_CPHS), "");
    EXPECT_NE(uSimFileController->ObtainElementFilePath(ELEMENTARY_FILE_SPN_CPHS), "");
    EXPECT_NE(uSimFileController->ObtainElementFilePath(ELEMENTARY_FILE_SPN_SHORT_CPHS), "");
    EXPECT_NE(uSimFileController->ObtainElementFilePath(ELEMENTARY_FILE_FDN), "");
    EXPECT_NE(uSimFileController->ObtainElementFilePath(ELEMENTARY_FILE_SDN), "");
    EXPECT_NE(uSimFileController->ObtainElementFilePath(ELEMENTARY_FILE_EXT3), "");
    EXPECT_NE(uSimFileController->ObtainElementFilePath(ELEMENTARY_FILE_MSISDN), "");
    EXPECT_NE(uSimFileController->ObtainElementFilePath(ELEMENTARY_FILE_EXT2), "");
    EXPECT_NE(uSimFileController->ObtainElementFilePath(ELEMENTARY_FILE_INFO_CPHS), "");
    EXPECT_NE(uSimFileController->ObtainElementFilePath(ELEMENTARY_FILE_CSP_CPHS), "");
    EXPECT_NE(uSimFileController->ObtainElementFilePath(ELEMENTARY_FILE_GID1), "");
    EXPECT_NE(uSimFileController->ObtainElementFilePath(ELEMENTARY_FILE_GID2), "");
    EXPECT_NE(uSimFileController->ObtainElementFilePath(ELEMENTARY_FILE_LI), "");
    EXPECT_NE(uSimFileController->ObtainElementFilePath(ELEMENTARY_FILE_PLMN_W_ACT), "");
    EXPECT_NE(uSimFileController->ObtainElementFilePath(ELEMENTARY_FILE_OPLMN_W_ACT), "");
    EXPECT_NE(uSimFileController->ObtainElementFilePath(ELEMENTARY_FILE_HPLMN_W_ACT), "");
    EXPECT_NE(uSimFileController->ObtainElementFilePath(ELEMENTARY_FILE_EHPLMN), "");
    EXPECT_NE(uSimFileController->ObtainElementFilePath(ELEMENTARY_FILE_FPLMN), "");
    EXPECT_NE(uSimFileController->ObtainElementFilePath(ELEMENTARY_FILE_LRPLMNSI), "");
    EXPECT_NE(uSimFileController->ObtainElementFilePath(ELEMENTARY_FILE_HPPLMN), "");
    EXPECT_NE(uSimFileController->ObtainElementFilePath(ELEMENTARY_FILE_PBR), "");
}

/**
 * @tc.number   Telephony_RadioProtocolController_001
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_RadioProtocolController_001, Function | MediumTest | Level1)
{
    auto telRilManager = std::make_shared<TelRilManager>();
    AppExecFwk::InnerEvent::Pointer event = AppExecFwk::InnerEvent::Get(INVALID_SLOTID, 1);
    auto radioProtocolController =
        std::make_shared<RadioProtocolController>(std::weak_ptr<TelRilManager>(telRilManager));
    radioProtocolController->UnRegisterEvents();
    radioProtocolController->ProcessGetRadioProtocol(event);
    radioProtocolController->ProcessCheckRadioProtocol(event);
    radioProtocolController->ProcessSetRadioProtocolComplete(event);
    radioProtocolController->communicatingSlotCount_ = 1;
    radioProtocolController->ProcessCheckRadioProtocol(event);
    radioProtocolController->communicatingSlotCount_ = true;
    radioProtocolController->ProcessCheckRadioProtocol(event);
    radioProtocolController->ProcessRadioProtocolNotify(event);
    radioProtocolController->ProcessSetRadioProtocolTimeout(event);
    radioProtocolController->ProcessActiveSimToRilResponse(event);
    event = nullptr;
    radioProtocolController->ProcessEvent(event);
    radioProtocolController->ProcessGetRadioProtocol(event);
    radioProtocolController->ProcessCheckRadioProtocol(event);
    radioProtocolController->ProcessUpdateRadioProtocol(event);
    radioProtocolController->ProcessRadioProtocolNotify(event);
    radioProtocolController->ProcessSetRadioProtocolComplete(event);
    radioProtocolController->ProcessSetRadioProtocolTimeout(event);
    radioProtocolController->ProcessActiveSimToRilResponse(event);
    radioProtocolController->BuildRadioProtocolForCommunication(
        RadioProtocolPhase::RADIO_PROTOCOL_PHASE_CHECK, RadioProtocolStatus::RADIO_PROTOCOL_STATUS_FAIL);
    radioProtocolController->BuildRadioProtocolForCommunication(
        RadioProtocolPhase::RADIO_PROTOCOL_PHASE_COMPLETE, RadioProtocolStatus::RADIO_PROTOCOL_STATUS_FAIL);
    radioProtocolController->BuildRadioProtocolForCommunication(
        RadioProtocolPhase::RADIO_PROTOCOL_PHASE_INITIAL, RadioProtocolStatus::RADIO_PROTOCOL_STATUS_FAIL);
    radioProtocolController->BuildRadioProtocolForCommunication(
        RadioProtocolPhase::RADIO_PROTOCOL_PHASE_NOTIFY, RadioProtocolStatus::RADIO_PROTOCOL_STATUS_FAIL);
    radioProtocolController->BuildRadioProtocolForCommunication(
        RadioProtocolPhase::RADIO_PROTOCOL_PHASE_UPDATE, RadioProtocolStatus::RADIO_PROTOCOL_STATUS_FAIL);
    EXPECT_TRUE(radioProtocolController->RadioProtocolControllerPoll());
    EXPECT_FALSE(radioProtocolController->SetRadioProtocol(INVALID_SLOTID));
    radioProtocolController->isCommunicating_ = true;
    EXPECT_FALSE(radioProtocolController->SetRadioProtocol(INVALID_SLOTID));
    telRilManager = nullptr;
    std::vector<RadioProtocol> radioProtocol;
    radioProtocolController->SendRadioProtocolEvent(radioProtocol, 1);
    radioProtocolController->UnRegisterEvents();
    radioProtocolController->GetRadioProtocol(INVALID_SLOTID);
    EXPECT_FALSE(radioProtocolController->SetActiveSimToRil(INVALID_SLOTID, 1, 1));
}

/**
 * @tc.number   Telephony_RadioProtocolController_002
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_RadioProtocolController_002, Function | MediumTest | Level1)
{
    auto telRilManager = std::make_shared<TelRilManager>();
    AppExecFwk::InnerEvent::Pointer event = AppExecFwk::InnerEvent::Get(INVALID_SLOTID, 1);
    auto radioProtocolController =
        std::make_shared<RadioProtocolController>(std::weak_ptr<TelRilManager>(telRilManager));
    RadioProtocol radioProtocol1;
    radioProtocol1.slotId = SLOT_ID_0;
    radioProtocol1.sessionId = 0;
    radioProtocol1.phase = RadioProtocolPhase::RADIO_PROTOCOL_PHASE_INITIAL;
    radioProtocol1.technology = 0;
    radioProtocol1.modemId = 0;
    radioProtocol1.status = RadioProtocolStatus::RADIO_PROTOCOL_STATUS_NONE;
    radioProtocolController->radioProtocol_.push_back(radioProtocol1);
    radioProtocolController->GetRadioProtocolModemId(-1);
    radioProtocolController->GetRadioProtocolModemId(3);
    radioProtocolController->GetRadioProtocolTech(-1);
    radioProtocolController->GetRadioProtocolTech(3);
    EXPECT_EQ(radioProtocolController->GetRadioProtocolModemId(SLOT_ID_0), 0);
}

/**
 * @tc.number   Telephony_StkController_001
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_StkController_001, Function | MediumTest | Level1)
{
    std::string name = "StkController_";
    AppExecFwk::InnerEvent::Pointer event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_STK_CALL_SETUP, 1);
    std::shared_ptr<TelRilManager> telRilManager = nullptr;
    std::shared_ptr<Telephony::SimStateManager> simStateManager = std::make_shared<SimStateManager>(telRilManager);
    auto stkController = std::make_shared<StkController>(telRilManager, simStateManager, INVALID_SLOTID);
    std::string strCmd = "";
    stkController->UnRegisterEvents();
    stkController->ProcessEvent(event);
    event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_STK_IS_READY, 1);
    stkController->ProcessEvent(event);
    event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_SIM_STATE_CHANGE, 1);
    stkController->ProcessEvent(event);
    event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_STK_SESSION_END, 1);
    stkController->ProcessEvent(event);
    event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_STK_PROACTIVE_COMMAND, 1);
    stkController->ProcessEvent(event);
    event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_STK_ALPHA_NOTIFY, 1);
    stkController->ProcessEvent(event);
    event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_STK_EVENT_NOTIFY, 1);
    stkController->ProcessEvent(event);
    event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_ICC_REFRESH, 1);
    stkController->ProcessEvent(event);
    event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_STK_SEND_TERMINAL_RESPONSE, 1);
    stkController->ProcessEvent(event);
    event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_STK_SEND_ENVELOPE, 1);
    stkController->ProcessEvent(event);
    event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_STK_SEND_CALL_SETUP_REQUEST_RESULT, 1);
    stkController->ProcessEvent(event);
    event = nullptr;
    stkController->ProcessEvent(event);
    EXPECT_EQ(stkController->SendCallSetupRequestResult(true), TELEPHONY_ERR_FAIL);
    EXPECT_GT(stkController->SendTerminalResponseCmd(strCmd), TELEPHONY_ERR_SUCCESS);
    EXPECT_GT(stkController->SendEnvelopeCmd(strCmd), TELEPHONY_ERR_SUCCESS);
}

/**
 * @tc.number   Telephony_IccOperatorRule_001
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_IccOperatorRule_001, Function | MediumTest | Level1)
{
    auto iccOperatorRule = std::make_shared<IccOperatorRule>();
    auto operatorMatchingRule = std::make_shared<OperatorMatchingRule>();
    std::string numIt = "123";
    IccOperatorRule result;
    std::string::const_iterator hexStrBeg = numIt.begin();
    std::string::const_iterator hexStrEnd = numIt.begin();
    std::vector<IccOperatorRule> list;
    std::string_view certHash;
    std::string_view packageName;
    std::string hexStr = "12";
    std::string iccidFromSim = "";
    std::string iccidRegex = "";
    int32_t len = 1;
    EXPECT_FALSE(iccOperatorRule->DecodeTLVTagRule(hexStrBeg, hexStrEnd, result, len));
    EXPECT_FALSE(iccOperatorRule->DecodeTLVTagCertPkg(hexStrBeg, hexStrEnd, result));
    EXPECT_FALSE(iccOperatorRule->DecodeTLVTagLimits(hexStrBeg, hexStrEnd, result));
    EXPECT_FALSE(iccOperatorRule->CreateFromTLV(hexStrBeg, hexStrEnd, list));
    EXPECT_FALSE(iccOperatorRule->CreateFromTLV(numIt, list));
    EXPECT_TRUE(iccOperatorRule->Matche(certHash, packageName));
    EXPECT_TRUE(iccOperatorRule->SetPackageNameByHexStr(hexStr));
    EXPECT_FALSE(operatorMatchingRule->IccidRegexMatch(iccidFromSim, iccidRegex));
    EXPECT_FALSE(operatorMatchingRule->ImsiRegexMatch(iccidFromSim, iccidRegex));
    EXPECT_FALSE(operatorMatchingRule->SpnRegexMatch(iccidFromSim, iccidRegex));
    EXPECT_TRUE(operatorMatchingRule->SpnRegexMatch(iccidFromSim, "null"));
    EXPECT_FALSE(operatorMatchingRule->PrefixMatch(iccidFromSim, iccidRegex));
}

/**
 * @tc.number   Telephony_SIMUtils_001
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_SIMUtils_001, Function | MediumTest | Level1)
{
    auto simUtils = std::make_shared<SIMUtils>();
    char spnData = '0';
    std::string str = "";
    int byteslen = 1;
    EXPECT_EQ(simUtils->HexCharConvertToInt(spnData), 0);
    spnData = 'B';
    EXPECT_GT(simUtils->HexCharConvertToInt(spnData), 0);
    spnData = 'b';
    EXPECT_GT(simUtils->HexCharConvertToInt(spnData), 0);
    EXPECT_TRUE(simUtils->HexStringConvertToBytes(str, byteslen) == nullptr);
    str = "123";
    unsigned char *bytes = (unsigned char *)str.c_str();
    std::shared_ptr<unsigned char> bytesTwo = std::make_shared<unsigned char>(1);
    simUtils->ArrayCopy(bytes, 1, bytes, 1, 1);
    EXPECT_TRUE(simUtils->HexStringConvertToBytes(str, byteslen) == nullptr);
    str = "1";
    EXPECT_TRUE(simUtils->IsShowableAsciiOnly(str));
    EXPECT_TRUE(simUtils->HexStringConvertToBytes(str, byteslen) == nullptr);
    byteslen = 2;
    str = "12";
    EXPECT_TRUE(simUtils->HexStringConvertToBytes(str, byteslen) != nullptr);
    EXPECT_EQ(simUtils->BytesConvertToHexString(nullptr, 1), "");
    EXPECT_NE(simUtils->BytesConvertToHexString(bytes, 1), "");
    int outChar16Len = 1;
    EXPECT_TRUE(simUtils->CharsConvertToChar16(nullptr, 1, outChar16Len, true) == nullptr);
    EXPECT_TRUE(simUtils->CharsConvertToChar16(bytes, 0, outChar16Len, true) == nullptr);
    EXPECT_TRUE(simUtils->CharsConvertToChar16(bytes, BYTES_LENGTH, outChar16Len, true) == nullptr);
    EXPECT_TRUE(simUtils->CharsConvertToChar16(bytes, 1, outChar16Len, true) == nullptr);
    EXPECT_FALSE(simUtils->CharsConvertToChar16(bytes, OBTAIN_SPN_GENERAL, outChar16Len, true) == nullptr);
    EXPECT_FALSE(simUtils->CharsConvertToChar16(bytes, OBTAIN_SPN_GENERAL, outChar16Len, false) == nullptr);
    EXPECT_FALSE(simUtils->CharsConvertToChar16(bytes, MAX_ENGLISH_NAME * OBTAIN_SPN_GENERAL, outChar16Len, false)
        == nullptr);
    EXPECT_NE(simUtils->Gsm7bitConvertToString(bytes, 1), "");
    EXPECT_EQ(simUtils->DiallingNumberStringFieldConvertToString(bytesTwo, 0, 0, 1), "");
    EXPECT_EQ(simUtils->DiallingNumberStringFieldConvertToString(nullptr, 0, 1, 1), "");
}

/**
 * @tc.number   Telephony_SIMUtils_002
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_SIMUtils_002, Function | MediumTest | Level1)
{
    auto simUtils = std::make_shared<SIMUtils>();
    unsigned char *data(new unsigned char[5] { 0x81, 0x02, 0xA9, 0xC8, 0xC8 });
    EXPECT_EQ(
        simUtils->DiallingNumberStringFieldConvertToString(std::shared_ptr<unsigned char>(data), 0, 5, 0), "哈哈");
    unsigned char *data2(new unsigned char[4] { 0x81, 0x01, 0xAA, 0xCA });
    EXPECT_EQ(simUtils->DiallingNumberStringFieldConvertToString(std::shared_ptr<unsigned char>(data2), 0, 4, 0), "啊");
    unsigned char *data3(new unsigned char[6] { 0x81, 0x03, 0xCE, 0xDC, 0xDC, 0xDC });
    EXPECT_EQ(
        simUtils->DiallingNumberStringFieldConvertToString(std::shared_ptr<unsigned char>(data3), 0, 6, 0), "杜杜杜");
    unsigned char *data4(new unsigned char[6] { 0x82, 0x02, 0x4E, 0x2A, 0xE2, 0x80 });
    EXPECT_EQ(
        simUtils->DiallingNumberStringFieldConvertToString(std::shared_ptr<unsigned char>(data4), 0, 6, 0), "二个");
    unsigned char *data5(new unsigned char[11] { 0x82, 0x07, 0x82, 0x80, 0x38, 0x30, 0x32, 0x35, 0x45, 0x46, 0xB3 });
    EXPECT_EQ(simUtils->DiallingNumberStringFieldConvertToString(std::shared_ptr<unsigned char>(data5), 0, 11, 0),
        "8025EF芳");
    unsigned char *data6(new unsigned char[5] { 0x80, 0x4E, 0x2D, 0x56, 0xFD });
    EXPECT_EQ(
        simUtils->DiallingNumberStringFieldConvertToString(std::shared_ptr<unsigned char>(data6), 0, 5, 0), "中国");
    unsigned char *data7(new unsigned char[9] { 0x80, 0x67, 0x5C, 0x00, 0x31, 0x00, 0x30, 0x5A, 0x18 });
    EXPECT_EQ(
        simUtils->DiallingNumberStringFieldConvertToString(std::shared_ptr<unsigned char>(data7), 0, 9, 0), "杜10娘");
    unsigned char *data8(new unsigned char[39] { 0x81, 0x0E, 0x08, 0x9B, 0xB8, 0xC7, 0xBD,
        0xCB, 0xB9, 0x20, 0xBA, 0xB0, 0xB1, 0xB8, 0xBD, 0xB5, 0xC2, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x04, 0x81, 0x1A, 0x50,
        0xFB, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF});
    EXPECT_EQ(simUtils->DiallingNumberStringFieldConvertToString(std::shared_ptr<unsigned char>(data8), 0, 39, 0),
        "Личный кабинет");
}

HWTEST_F(BranchTest, Telephony_SIMUtils_003, Function | MediumTest | Level1)
{
    auto simUtils = std::make_shared<SIMUtils>();
    std::string str = "";
    EXPECT_EQ(simUtils->Cphs7bitConvertToString(str), "");
}

HWTEST_F(BranchTest, Telephony_SIMUtils_004, Function | MediumTest | Level1)
{
    auto simUtils = std::make_shared<SIMUtils>();
    std::string str1 = "544553541B141B281B291B2F1B3C1B3D1B3E1B4054455354";
    EXPECT_EQ(simUtils->Cphs7bitConvertToString(str1), "TEST^{}\\[~]|TEST");
}

/**
 * @tc.number   Telephony_SimNumberDecode_001
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_SimNumberDecode_001, Function | MediumTest | Level1)
{
    auto simNumberDecode = std::make_shared<SimNumberDecode>();
    std::string number = "a";
    EXPECT_FALSE(simNumberDecode->IsValidNumberString(number));
    EXPECT_TRUE(simNumberDecode->chooseExtendedByType(1) != nullptr);
    EXPECT_TRUE(simNumberDecode->chooseExtendedByType(OBTAIN_SPN_GENERAL) != nullptr);
    char spnData = '0';
    uint8_t result = 1;
    EXPECT_TRUE(simNumberDecode->CharToBCD(spnData, result, 1));
    spnData = 'a';
    EXPECT_FALSE(simNumberDecode->CharToBCD(spnData, result, BYTES_LENGTH));
    EXPECT_TRUE(simNumberDecode->BcdToChar(1, spnData, 1));
    EXPECT_FALSE(simNumberDecode->BcdToChar(VALUE_LENGTH, spnData, BYTES_LENGTH));
    EXPECT_FALSE(simNumberDecode->BcdToChar(VALUE_LENGTH, spnData, 1));
    number = "+-";
    std::vector<uint8_t> bcdCodes;
    std::shared_ptr<unsigned char> bytesData = nullptr;
    EXPECT_FALSE(simNumberDecode->NumberConvertToBCD(number, bcdCodes, true, BYTES_LENGTH));
    number = "0+";
    EXPECT_TRUE(simNumberDecode->NumberConvertToBCD(number, bcdCodes, true, 1));
    EXPECT_EQ(simNumberDecode->BCDConvertToString(bytesData, 1, 1, 1), "");
    bytesData = std::make_shared<unsigned char>(1);
    EXPECT_EQ(simNumberDecode->BCDConvertToString(bytesData, 1, 1, 1), "");
    bcdCodes.push_back(1);
    bcdCodes.push_back(LO_FOUR_LENGTH);
    std::vector<uint8_t>::const_iterator codeBeg = bcdCodes.begin();
    std::vector<uint8_t>::const_iterator codeEnd = bcdCodes.end();
    EXPECT_FALSE(simNumberDecode->BCDSectionConvertToString(codeBeg, codeEnd, number, BYTES_LENGTH));
    EXPECT_FALSE(simNumberDecode->BCDSectionConvertToString(codeBeg, codeEnd, number, 1));
    EXPECT_TRUE(simNumberDecode->BCDConvertToString(codeBeg, codeEnd, number, BYTES_LENGTH));
    EXPECT_TRUE(simNumberDecode->BCDConvertToString(codeBeg, codeEnd, number, 1));
}

/**
 * @tc.number   Telephony_IccFile_001
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_IccFile_001, Function | MediumTest | Level1)
{
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    auto simStateManager = std::make_shared<SimStateManager>(telRilManager);
    std::shared_ptr<IccFile> iccFile = std::make_shared<IsimFile>(simStateManager);
    AppExecFwk::InnerEvent::Pointer event = AppExecFwk::InnerEvent::Get(StateMessage::MSG_ICC_REFRESH, 1);
    iccFile->ProcessEvent(event);
    event = nullptr;
    iccFile->ProcessEvent(event);
    std::shared_ptr<AppExecFwk::EventHandler> handler = nullptr;
    iccFile->RegisterCoreNotify(handler, RadioEvent::RADIO_SIM_RECORDS_LOADED);
    iccFile->RegisterCoreNotify(handler, RadioEvent::RADIO_SIM_RECORDS_LOADED);
    iccFile->UnRegisterCoreNotify(handler, RadioEvent::RADIO_SIM_RECORDS_LOADED);
    iccFile->UnRegisterCoreNotify(handler, RadioEvent::RADIO_SIM_GET_RADIO_PROTOCOL);
    iccFile->imsi_ = "123";
    iccFile->RegisterCoreNotify(handler, RadioEvent::RADIO_IMSI_LOADED_READY);
    iccFile->UnRegisterCoreNotify(handler, RadioEvent::RADIO_IMSI_LOADED_READY);
    iccFile->RegisterCoreNotify(handler, RadioEvent::RADIO_OPERATOR_CONFIG_UPDATE);
    iccFile->UnRegisterCoreNotify(handler, RadioEvent::RADIO_OPERATOR_CONFIG_UPDATE);
    std::string plmn = "";
    EXPECT_EQ(iccFile->ObtainEons(plmn, 1, true), "");
    plmn = "123";
    iccFile->UpdateSPN(plmn);
    EXPECT_EQ(iccFile->ObtainEons(plmn, 1, true), "");
    auto plmnNetworkName = std::make_shared<PlmnNetworkName>();
    iccFile->pnnFiles_.push_back(plmnNetworkName);
    EXPECT_EQ(iccFile->ObtainEons(plmn, 1, true), "");
    auto opl = std::make_shared<OperatorPlmnInfo>();
    opl->plmnNumeric = "123";
    opl->lacStart = 0;
    opl->lacEnd = 0xfffe;
    opl->pnnRecordId = 1;
    iccFile->oplFiles_.push_back(opl);
    iccFile->oplFiles_.push_back(nullptr);
    EXPECT_EQ(iccFile->ObtainEons(plmn, 0, true), "");
    EXPECT_EQ(iccFile->ObtainEons(plmn, 0, false), "");
}

/**
 * @tc.number   Telephony_IccFile_002
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_IccFile_002, Function | MediumTest | Level1) {
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    auto simStateManager = std::make_shared<SimStateManager>(telRilManager);
    std::shared_ptr<IccFile> iccFile = std::make_shared<IsimFile>(simStateManager);
    std::string iccId = "123456";
    iccFile->SwapPairsForIccId(iccId);
    EXPECT_EQ(iccId, "214365");
    iccId = "12341A56";
    iccFile->SwapPairsForIccId(iccId);
    EXPECT_EQ(iccId, "2143");
    iccId = "1234F156";
    iccFile->SwapPairsForIccId(iccId);
    EXPECT_EQ(iccId, "2143165");
    iccId = "1234A156";
    iccFile->SwapPairsForIccId(iccId);
    EXPECT_EQ(iccId, "21431");
    iccId = "1234AB56TF";
    iccFile->GetFullIccid(iccId);
    EXPECT_EQ(iccId, "2143BA65FT");
    std::string langData = "";
    EXPECT_EQ(iccFile->ObtainValidLanguage(langData), "");
    langData = "000011286F050400000000010203FF";
    EXPECT_EQ(iccFile->ObtainValidLanguage(langData), "");
}

/**
 * @tc.number   Telephony_SimRdbHelper_001
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_SimRdbHelper_001, Function | MediumTest | Level1)
{
    TELEPHONY_LOGI("Telephony_SimRdbHelper_001");
    auto simRdbHelper = std::make_shared<SimRdbHelper>();
    SimRdbInfo simBean;
    std::string iccId = "";
    std::vector<SimRdbInfo> vec;
    EXPECT_GE(simRdbHelper->GetDefaultMainCardSlotId(), DATA_STORAGE_ERR_PERMISSION_ERR);
    EXPECT_GE(simRdbHelper->GetDefaultMessageCardSlotId(), DATA_STORAGE_ERR_PERMISSION_ERR);
    EXPECT_GE(simRdbHelper->GetDefaultCellularDataCardSlotId(), DATA_STORAGE_ERR_PERMISSION_ERR);
    EXPECT_GE(simRdbHelper->GetDefaultVoiceCardSlotId(), DATA_STORAGE_ERR_PERMISSION_ERR);
    EXPECT_GE(simRdbHelper->SetDefaultMainCard(INVALID_SLOTID), DATA_STORAGE_ERR_PERMISSION_ERR);
    EXPECT_GE(simRdbHelper->SetDefaultVoiceCard(INVALID_SLOTID), DATA_STORAGE_ERR_PERMISSION_ERR);
    EXPECT_GE(simRdbHelper->SetDefaultMessageCard(INVALID_SLOTID), DATA_STORAGE_ERR_PERMISSION_ERR);
    EXPECT_GE(simRdbHelper->SetDefaultCellularData(INVALID_SLOTID), DATA_STORAGE_ERR_PERMISSION_ERR);
    EXPECT_GE(simRdbHelper->QueryDataByIccId(iccId, simBean), DATA_STORAGE_ERR_PERMISSION_ERR);
    EXPECT_GE(simRdbHelper->QueryAllData(vec), DATA_STORAGE_ERR_PERMISSION_ERR);
    EXPECT_GE(simRdbHelper->QueryAllValidData(vec), DATA_STORAGE_ERR_PERMISSION_ERR);
}

/**
 * @tc.number   Telephony_SimRdbHelper_002
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_SimRdbHelper_002, Function | MediumTest | Level1)
{
    TELEPHONY_LOGI("Telephony_SimRdbHelper_002");
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    std::vector<std::shared_ptr<Telephony::SimStateManager>> simStateManager = { nullptr, nullptr };
    std::vector<std::shared_ptr<Telephony::SimFileManager>> simFileManager = { nullptr, nullptr };
    std::shared_ptr<Telephony::MultiSimController> multiSimController =
            std::make_shared<MultiSimController>(telRilManager, simStateManager, simFileManager);
    multiSimController->InitData(0);
    multiSimController->ForgetAllData();
    auto simRdbHelper = std::make_shared<SimRdbHelper>();
    SimRdbInfo simBean;
    std::string iccId = "empty_slot0";
    std::vector<SimRdbInfo> vec;
    EXPECT_GE(simRdbHelper->UpdateOpKeyInfo(), DATA_STORAGE_ERR_PERMISSION_ERR);
    EXPECT_GE(simRdbHelper->ClearData(), DATA_STORAGE_ERR_PERMISSION_ERR);
    DataShare::DataShareValuesBucket values;
    DataShare::DataShareValueObject slotObj(0);
    DataShare::DataShareValueObject iccidObj(iccId);
    DataShare::DataShareValueObject valueObj(ACTIVE);
    values.Put(SimData::SLOT_INDEX, slotObj);
    values.Put(SimData::ICC_ID, iccidObj);
    values.Put(SimData::CARD_ID, iccidObj); // iccId == cardId by now
    values.Put(SimData::IS_ACTIVE, valueObj);
    int64_t id;
    EXPECT_GE(simRdbHelper->InsertData(id, values), DATA_STORAGE_ERR_PERMISSION_ERR);
    DataShare::DataShareValuesBucket valuesExt;
    DataShare::DataShareValueObject mccObj("460");
    DataShare::DataShareValueObject mncObj("09");
    valuesExt.Put(SimData::MCC, mccObj);
    valuesExt.Put(SimData::MNC, mncObj);
    DataShare::DataSharePredicates predicates;
    predicates.EqualTo(SimData::ICC_ID, iccId);
    EXPECT_GE(simRdbHelper->UpdateDataByIccId(iccId, valuesExt), DATA_STORAGE_ERR_PERMISSION_ERR);
    EXPECT_GE(simRdbHelper->QueryDataByIccId(iccId, simBean), DATA_STORAGE_ERR_PERMISSION_ERR);
    EXPECT_GE(simRdbHelper->QueryAllData(vec), DATA_STORAGE_ERR_PERMISSION_ERR);
    EXPECT_GE(simRdbHelper->QueryAllValidData(vec), DATA_STORAGE_ERR_PERMISSION_ERR);
    int curSimId = multiSimController->GetSimId(0);
    DataShare::DataShareValuesBucket valuesNumber;
    DataShare::DataShareValueObject numberObj("123569877456");
    valuesNumber.Put(SimData::PHONE_NUMBER, numberObj);
    EXPECT_GE(simRdbHelper->UpdateDataBySimId(curSimId, valuesNumber), DATA_STORAGE_ERR_PERMISSION_ERR);
    multiSimController->ForgetAllData(0);
}

/**
 * @tc.number   Telephony_MultiSimMonitor_001
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_MultiSimMonitor_001, Function | MediumTest | Level1)
{
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    auto simStateManagerPtr = std::make_shared<SimStateManager>(telRilManager);
    auto telRilManagerWeak = std::weak_ptr<TelRilManager>(telRilManager);
    EventFwk::MatchingSkills matchingSkills;
    matchingSkills.AddEvent(CommonEventSupport::COMMON_EVENT_OPERATOR_CONFIG_CHANGED);
    EventFwk::CommonEventSubscribeInfo subcribeInfo(matchingSkills);
    auto simFileManagerPtr = std::make_shared<Telephony::SimFileManager>(
        subcribeInfo, telRilManagerWeak, std::weak_ptr<Telephony::SimStateManager>(simStateManagerPtr));
    std::vector<std::shared_ptr<Telephony::SimStateManager>> simStateManager = { simStateManagerPtr,
        simStateManagerPtr };
    std::vector<std::shared_ptr<Telephony::SimFileManager>> simFileManager = { simFileManagerPtr, simFileManagerPtr };
    std::shared_ptr<Telephony::MultiSimController> multiSimController =
        std::make_shared<MultiSimController>(telRilManager, simStateManager, simFileManager);
    std::vector<std::weak_ptr<Telephony::SimFileManager>> simFileManagerWeak = {
        std::weak_ptr<Telephony::SimFileManager>(simFileManagerPtr),
        std::weak_ptr<Telephony::SimFileManager>(simFileManagerPtr)
    };
    auto multiSimMonitor = std::make_shared<MultiSimMonitor>(multiSimController, simStateManager, simFileManagerWeak);
    AppExecFwk::InnerEvent::Pointer event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_SIM_RECORDS_LOADED, 1);
    multiSimMonitor->isSimAccountLoaded_.resize(SIM_SLOT_COUNT, 0);
    multiSimMonitor->initDataRemainCount_.resize(SIM_SLOT_COUNT, 5);
    multiSimController->setPrimarySlotRemainCount_.resize(2, 5);
    multiSimMonitor->ProcessEvent(event);
    event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_SIM_STATE_CHANGE, 1);
    multiSimMonitor->ProcessEvent(event);
    event = AppExecFwk::InnerEvent::Get(RadioProtocolEvent::MSG_SIM_FORGET_ALLDATA, 1);
    multiSimMonitor->ProcessEvent(event);
    event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_SIM_STATE_SIMLOCK, 1);
    multiSimMonitor->ProcessEvent(event);
    event = nullptr;
    multiSimMonitor->ProcessEvent(event);
    multiSimMonitor->InitData(INVALID_SLOTID);
    multiSimMonitor->InitData(0);
    multiSimMonitor->RefreshData(INVALID_SLOTID);
    multiSimMonitor->RefreshData(0);
    multiSimMonitor->NotifySimAccountChanged();
    int32_t tokenId = 123456789;
    sptr<SimAccountCallback> callback = nullptr;
    EXPECT_GT(multiSimMonitor->RegisterSimAccountCallback(tokenId, callback), TELEPHONY_ERROR);
    EXPECT_GT(multiSimMonitor->UnregisterSimAccountCallback(callback), TELEPHONY_ERROR);
}

/**
 * @tc.number   Telephony_MultiSimMonitor_002
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_MultiSimMonitor_002, Function | MediumTest | Level1)
{
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    auto simStateManagerPtr = std::make_shared<SimStateManager>(telRilManager);
    auto telRilManagerWeak = std::weak_ptr<TelRilManager>(telRilManager);
    EventFwk::MatchingSkills matchingSkills;
    matchingSkills.AddEvent(CommonEventSupport::COMMON_EVENT_OPERATOR_CONFIG_CHANGED);
    EventFwk::CommonEventSubscribeInfo subcribeInfo(matchingSkills);
    auto simFileManagerPtr = std::make_shared<Telephony::SimFileManager>(
        subcribeInfo, telRilManagerWeak, std::weak_ptr<Telephony::SimStateManager>(simStateManagerPtr));
    std::vector<std::shared_ptr<Telephony::SimStateManager>> simStateManager = { simStateManagerPtr,
        simStateManagerPtr };
    std::vector<std::shared_ptr<Telephony::SimFileManager>> simFileManager = { simFileManagerPtr, simFileManagerPtr };
    std::shared_ptr<Telephony::MultiSimController> multiSimController =
        std::make_shared<MultiSimController>(telRilManager, simStateManager, simFileManager);
    std::vector<std::weak_ptr<Telephony::SimFileManager>> simFileManagerWeak = {
        std::weak_ptr<Telephony::SimFileManager>(simFileManagerPtr),
        std::weak_ptr<Telephony::SimFileManager>(simFileManagerPtr)
    };
    auto multiSimMonitor = std::make_shared<MultiSimMonitor>(multiSimController, simStateManager, simFileManagerWeak);
    multiSimMonitor->AddExtraManagers(simStateManagerPtr, simFileManagerPtr);
    auto simStateHandle = std::make_shared<SimStateHandle>(simStateManagerPtr);
    AppExecFwk::InnerEvent::Pointer event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_SIM_STATE_READY, 0);
    multiSimMonitor->ProcessEvent(event);
    multiSimMonitor->RegisterCoreNotify(0, simStateHandle, RadioEvent::RADIO_SIM_ACCOUNT_LOADED);
    multiSimMonitor->IsVSimSlotId(0);
    multiSimMonitor->ResetSimLoadAccount(0);
    multiSimMonitor->RegisterSimNotify(0);
    multiSimMonitor->UnRegisterSimNotify();
    ASSERT_TRUE(matchingSkills.CountEvent() == 1);
}

/**
 * @tc.number   Telephony_MultiSimMonitor_003
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_MultiSimMonitor_003, Function | MediumTest | Level1)
{
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    auto simStateManagerPtr = std::make_shared<SimStateManager>(telRilManager);
    auto telRilManagerWeak = std::weak_ptr<TelRilManager>(telRilManager);
    EventFwk::MatchingSkills matchingSkills;
    matchingSkills.AddEvent(CommonEventSupport::COMMON_EVENT_OPERATOR_CONFIG_CHANGED);
    EventFwk::CommonEventSubscribeInfo subcribeInfo(matchingSkills);
    auto simFileManagerPtr = std::make_shared<Telephony::SimFileManager>(
        subcribeInfo, telRilManagerWeak, std::weak_ptr<Telephony::SimStateManager>(simStateManagerPtr));
    std::vector<std::shared_ptr<Telephony::SimStateManager>> simStateManager = { simStateManagerPtr,
        simStateManagerPtr };
    std::vector<std::shared_ptr<Telephony::SimFileManager>> simFileManager = { simFileManagerPtr, simFileManagerPtr };
    std::shared_ptr<Telephony::MultiSimController> multiSimController =
        std::make_shared<MultiSimController>(telRilManager, simStateManager, simFileManager);
    std::vector<std::weak_ptr<Telephony::SimFileManager>> simFileManagerWeak = {
        std::weak_ptr<Telephony::SimFileManager>(simFileManagerPtr),
        std::weak_ptr<Telephony::SimFileManager>(simFileManagerPtr)
    };
    std::shared_ptr<MultiSimMonitor> multiSimMonitor =
        std::make_shared<MultiSimMonitor>(multiSimController, simStateManager, simFileManagerWeak);
    multiSimMonitor->AddExtraManagers(simStateManagerPtr, simFileManagerPtr);
    EventFwk::MatchingSkills matchSkills;
    matchingSkills.AddEvent(DATASHARE_READY_EVENT);
    EventFwk::CommonEventSubscribeInfo subscriberInfo(matchSkills);
    subscriberInfo.SetThreadMode(CommonEventSubscribeInfo::COMMON);
    ASSERT_TRUE(matchingSkills.CountEvent() == 2);
}

/**
 * @tc.number   Telephony_MultiSimMonitor_004
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_MultiSimMonitor_004, Function | MediumTest | Level1)
{
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    auto simStateManagerPtr = std::make_shared<SimStateManager>(telRilManager);
    auto telRilManagerWeak = std::weak_ptr<TelRilManager>(telRilManager);
    EventFwk::MatchingSkills matchingSkills;
    matchingSkills.AddEvent(CommonEventSupport::COMMON_EVENT_OPERATOR_CONFIG_CHANGED);
    EventFwk::CommonEventSubscribeInfo subcribeInfo(matchingSkills);
    auto simFileManagerPtr = std::make_shared<Telephony::SimFileManager>(
        subcribeInfo, telRilManagerWeak, std::weak_ptr<Telephony::SimStateManager>(simStateManagerPtr));
    std::vector<std::shared_ptr<Telephony::SimStateManager>> simStateManager = { simStateManagerPtr,
        simStateManagerPtr };
    std::vector<std::shared_ptr<Telephony::SimFileManager>> simFileManager = { simFileManagerPtr, simFileManagerPtr };
    std::shared_ptr<Telephony::MultiSimController> multiSimController =
        std::make_shared<MultiSimController>(telRilManager, simStateManager, simFileManager);
    auto simFileManagerWeak = std::weak_ptr<Telephony::SimFileManager>(simFileManagerPtr);
    std::vector<std::weak_ptr<Telephony::SimFileManager>> simFileManagerWeaks = {simFileManagerWeak,
        simFileManagerWeak};
    std::shared_ptr<MultiSimMonitor> multiSimMonitor =
        std::make_shared<MultiSimMonitor>(nullptr, simStateManager, simFileManagerWeaks);
    EXPECT_FALSE(multiSimMonitor->IsValidSlotId(-1));
}

/**
 * @tc.number   Telephony_MultiSimMonitor_005
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_MultiSimMonitor_005, Function | MediumTest | Level1)
{
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    auto simStateManagerPtr = std::make_shared<SimStateManager>(telRilManager);
    auto telRilManagerWeak = std::weak_ptr<TelRilManager>(telRilManager);
    EventFwk::MatchingSkills matchingSkills;
    matchingSkills.AddEvent(CommonEventSupport::COMMON_EVENT_OPERATOR_CONFIG_CHANGED);
    EventFwk::CommonEventSubscribeInfo subcribeInfo(matchingSkills);
    auto simFileManagerPtr = std::make_shared<Telephony::SimFileManager>(
        subcribeInfo, telRilManagerWeak, std::weak_ptr<Telephony::SimStateManager>(simStateManagerPtr));
    std::vector<std::shared_ptr<Telephony::SimStateManager>> simStateManager = { simStateManagerPtr,
        simStateManagerPtr };
    std::vector<std::shared_ptr<Telephony::SimFileManager>> simFileManager = { simFileManagerPtr, simFileManagerPtr };
    std::shared_ptr<Telephony::MultiSimController> multiSimController =
        std::make_shared<MultiSimController>(telRilManager, simStateManager, simFileManager);
    std::vector<std::weak_ptr<Telephony::SimFileManager>> simFileManagerWeak = {
        std::weak_ptr<Telephony::SimFileManager>(simFileManagerPtr),
        std::weak_ptr<Telephony::SimFileManager>(simFileManagerPtr)
    };
    auto multiSimMonitor = std::make_shared<MultiSimMonitor>(multiSimController, simStateManager, simFileManagerWeak);
    multiSimMonitor->isSimAccountLoaded_.resize(SIM_SLOT_COUNT, 0);
    multiSimMonitor->initDataRemainCount_.resize(SIM_SLOT_COUNT, 5);
    multiSimMonitor->simStateManager_[0]->SetSimState(SimState::SIM_STATE_UNKNOWN);
    multiSimMonitor->isSimAccountLoaded_[0] = 0;
    multiSimMonitor->InitData(0);
    EXPECT_EQ(multiSimMonitor->isSimAccountLoaded_[0], 0);
}

/**
 * @tc.number   Telephony_ImsCoreServiceCallbackProxy_001
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_ImsCoreServiceCallbackProxy_001, Function | MediumTest | Level1)
{
    sptr<ISystemAbilityManager> systemAbilityMgr = SystemAbilityManagerClient::GetInstance().GetSystemAbilityManager();
    if (systemAbilityMgr == nullptr) {
        TELEPHONY_LOGE("Telephony_ImsCoreServiceCallbackProxy systemAbilityMgr is nullptr");
    }
    ASSERT_NE(systemAbilityMgr, nullptr);
    sptr<IRemoteObject> remote = systemAbilityMgr->CheckSystemAbility(TELEPHONY_CORE_SERVICE_SYS_ABILITY_ID);
    if (remote == nullptr) {
        TELEPHONY_LOGE("Telephony_ImsCoreServiceCallbackProxy remote is nullptr");
    }
    ASSERT_NE(remote, nullptr);
    auto imsCoreServiceCallbackProxy = std::make_shared<ImsCoreServiceCallbackProxy>(remote);
    ImsServiceStatus imsServiceStatus;
    EXPECT_GE(imsCoreServiceCallbackProxy->UpdateImsServiceStatusChanged(INVALID_SLOTID, imsServiceStatus), 0);
    ImsRegistrationStatus imsRegStatus;
    EXPECT_GE(imsCoreServiceCallbackProxy->GetImsRegistrationStatusResponse(INVALID_SLOTID, imsRegStatus), 0);
    auto imsCoreServiceCallbackStub = std::make_shared<ImsCoreServiceCallbackStub>();
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    EXPECT_GE(imsCoreServiceCallbackStub->OnRemoteRequest(0, data, reply, option), 0);
}

/**
 * @tc.number   Telephony_ImsCoreServiceProxy_001
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_ImsCoreServiceProxy_001, Function | MediumTest | Level1)
{
    sptr<ISystemAbilityManager> systemAbilityMgr = SystemAbilityManagerClient::GetInstance().GetSystemAbilityManager();
    if (systemAbilityMgr == nullptr) {
        TELEPHONY_LOGE("Telephony_ImsCoreServiceProxy systemAbilityMgr is nullptr");
    }
    ASSERT_NE(systemAbilityMgr, nullptr);
    sptr<IRemoteObject> remote = systemAbilityMgr->CheckSystemAbility(TELEPHONY_IMS_SYS_ABILITY_ID);
    if (remote == nullptr) {
        TELEPHONY_LOGE("Telephony_ImsCoreServiceProxy remote is nullptr");
    }
    ASSERT_NE(remote, nullptr);
    auto imsCoreServiceProxy = std::make_shared<ImsCoreServiceProxy>(remote);
    EXPECT_GE(imsCoreServiceProxy->GetImsRegistrationStatus(0), 0);
    EXPECT_GE(imsCoreServiceProxy->RegisterImsCoreServiceCallback(nullptr), 0);
}

/**
 * @tc.number   Telephony_CoreServiceClient_001
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_CoreServiceClient_001, Function | MediumTest | Level1)
{
    auto coreServiceClient = std::make_shared<CoreServiceClient>();
    coreServiceClient->OnRemoteDied(nullptr);
    auto recipient = std::make_shared<CoreServiceClient::CoreServiceDeathRecipient>(CoreServiceClient::GetInstance());
    recipient->OnRemoteDied(nullptr);
    EXPECT_GE(coreServiceClient->GetNetworkSelectionMode(INVALID_SLOTID, nullptr), 0);
    EXPECT_NE(coreServiceClient->RefreshSimState(INVALID_SLOTID), 0);
    EXPECT_GE(coreServiceClient->GetPreferredNetwork(INVALID_SLOTID, nullptr), 0);
    EXPECT_GE(coreServiceClient->SetPreferredNetwork(INVALID_SLOTID, 0, nullptr), 0);
}

/**
 * @tc.number   Telephony_SignalInformation_001
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_SignalInformation_001, Function | MediumTest | Level1)
{
    std::shared_ptr<SignalInformation> wCdmaSignalInformation = std::make_shared<WcdmaSignalInformation>();
    std::shared_ptr<SignalInformation> nrSignalInformation = std::make_shared<NrSignalInformation>();
    std::shared_ptr<SignalInformation> cdmaSignalInformation = std::make_shared<CdmaSignalInformation>();
    std::shared_ptr<SignalInformation> tdScdmaSignalInformation = std::make_shared<TdScdmaSignalInformation>();
    std::shared_ptr<SignalInformation> gsmSignalInformation = std::make_shared<GsmSignalInformation>();
    Parcel parcel;
    gsmSignalInformation->SignalInformation::Unmarshalling(parcel);
    EXPECT_GE(wCdmaSignalInformation->GetSignalIntensity(), 0);
    EXPECT_GE(nrSignalInformation->GetSignalIntensity(), 0);
    EXPECT_GE(cdmaSignalInformation->GetSignalIntensity(), 0);
    EXPECT_GE(tdScdmaSignalInformation->GetSignalIntensity(), 0);
    EXPECT_GE(gsmSignalInformation->GetSignalIntensity(), 0);
    std::shared_ptr<CellLocation> cellLocation = std::make_shared<GsmCellLocation>();
    cellLocation->CellLocation::Unmarshalling(parcel);
    std::shared_ptr<GsmCellLocation> gsmCellLocation = std::make_shared<GsmCellLocation>();
    EXPECT_GE(gsmCellLocation->GetCellId(), 0);
}

/**
 * @tc.number   Telephony_Network_InitTelephonyExtService_001
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_Network_InitTelephonyExtService_001, Function | MediumTest | Level1)
{
    TELEPHONY_EXT_WRAPPER.InitTelephonyExtWrapper();
    if (TELEPHONY_EXT_WRAPPER.telephonyExtWrapperHandle_ == nullptr) {
        TELEPHONY_LOGI("telephonyExtWrapperHandle_ null");
    } else {
        TELEPHONY_LOGI("telephonyExtWrapperHandle_ not null");
        EXPECT_TRUE(TELEPHONY_EXT_WRAPPER.updateNetworkStateExt_ != nullptr);
        EXPECT_TRUE(TELEPHONY_EXT_WRAPPER.updateOperatorNameParamsExt_ != nullptr);
    }
}

HWTEST_F(BranchTest, Telephony_SimStateHandle_003, Function | MediumTest | Level1)
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
        simFileManager[slotId] = SimFileManager::CreateInstance(
            std::weak_ptr<ITelRilManager>(telRilManager), std::weak_ptr<SimStateManager>(simStateManager[slotId]));
        simFileManager[slotId]->Init(slotId);
    }
    IccState iccState;
    iccState.simType_ = ICC_UNKNOWN_TYPE;
    iccState.simStatus_ = ICC_CONTENT_READY;
    iccState.iccid_ = "123456789012345";
    simStateManager[0]->simStateHandle_->oldSimType_ = ICC_USIM_TYPE;
    simStateManager[0]->simStateHandle_->oldSimStatus_ = ICC_CONTENT_UNKNOWN;
    simStateManager[0]->simStateHandle_->ProcessIccCardState(iccState, 0);
    EXPECT_EQ(simStateManager[0]->simStateHandle_->externalState_, SimState::SIM_STATE_READY);
}

/**
 * @tc.number   Telephony_IccFile_003
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_IccFile_003, Function | MediumTest | Level1)
{
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    auto simStateManager = std::make_shared<SimStateManager>(telRilManager);
    std::shared_ptr<IccFile> iccFile = std::make_shared<IsimFile>(simStateManager);
    iccFile->ehplmns_ = { "460-01", "460-02", "470-03" };
    iccFile->ObtainEhPlmns();
    iccFile->ehplmns_.clear();
    EXPECT_TRUE(iccFile->ObtainEhPlmns().empty());
}
 
/**
 * @tc.number   Telephony_IccFile_004
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_IccFile_004, Function | MediumTest | Level1)
{
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    auto simStateManager = std::make_shared<SimStateManager>(telRilManager);
    std::shared_ptr<IccFile> iccFile = std::make_shared<IsimFile>(simStateManager);
    iccFile->spdiPlmns_ = { "460-01", "460-02", "470-03" };
    iccFile->imsi_ = "1234567890";
    iccFile->ObtainMCC();
    iccFile->ObtainSpdiPlmns();
    iccFile->ObtainMNC();
    iccFile->spdiPlmns_.clear();
    iccFile->imsi_ = "";
    iccFile->ObtainMNC();
    EXPECT_TRUE(iccFile->ObtainSpdiPlmns().empty());
}

/**
 * @tc.number   Telephony_RadioProtocolController_003
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_RadioProtocolController_003, Function | MediumTest | Level1)
{
    auto telRilManager = std::make_shared<TelRilManager>();
    auto radioProtocolController =
        std::make_shared<RadioProtocolController>(std::weak_ptr<TelRilManager>(telRilManager));
    AppExecFwk::InnerEvent::Pointer event = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_SIM_GET_RADIO_PROTOCOL);
    AppExecFwk::InnerEvent::Pointer event1 = AppExecFwk::InnerEvent::Get(RadioEvent::RADIO_SIM_RADIO_PROTOCOL_NOTIFY);
    std::shared_ptr<RadioProtocol> radioProtocol = event1->GetSharedObject<RadioProtocol>();
    radioProtocolController->GetRadioProtocol(SLOT_ID_0);
    RadioProtocol protocol;
    protocol.sessionId = radioProtocolController->sessionId_;
    radioProtocolController->ProcessRadioProtocolNotify(event1);
    EXPECT_FALSE(radioProtocolController->communicationFailed_);
    radioProtocolController->ProcessRadioProtocolNotify(event);
    event = nullptr;
    radioProtocolController->slotCount_ = -1;
    radioProtocolController->GetRadioProtocol(SLOT_ID_0);
    radioProtocolController->SendRadioProtocolEvent(radioProtocolController->oldRadioProtocol_, 1);
    protocol.slotId = -1;
    radioProtocolController->SendRadioProtocolEvent(radioProtocolController->oldRadioProtocol_, 1);
 
    radioProtocolController->telRilManager_.reset();
    radioProtocolController->Init();
    EXPECT_FALSE(radioProtocolController->SetRadioProtocol(SLOT_ID_0));
    radioProtocolController->isCommunicating_ = true;
    EXPECT_FALSE(radioProtocolController->SetRadioProtocol(SLOT_ID_0));
    radioProtocolController->oldRadioProtocol_.clear();
    radioProtocolController->SendRadioProtocolEvent(radioProtocolController->oldRadioProtocol_, 1);
    radioProtocolController->BuildRadioProtocolForCommunication(
        RadioProtocolPhase::RADIO_PROTOCOL_PHASE_CHECK, RadioProtocolStatus::RADIO_PROTOCOL_STATUS_FAIL);
    radioProtocolController->BuildRadioProtocolForCommunication(
        RadioProtocolPhase::RADIO_PROTOCOL_PHASE_COMPLETE, RadioProtocolStatus::RADIO_PROTOCOL_STATUS_FAIL);
}
 
/**
 * @tc.number   Telephony_MultiSimController_005
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_MultiSimController_005, Function | MediumTest | Level1)
{
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    std::vector<std::shared_ptr<Telephony::SimStateManager>> simStateManager = { nullptr, nullptr };
    std::vector<std::shared_ptr<Telephony::SimFileManager>> simFileManager = { nullptr, nullptr };
    std::shared_ptr<Telephony::MultiSimController> multiSimController =
        std::make_shared<MultiSimController>(telRilManager, simStateManager, simFileManager);
    TELEPHONY_EXT_WRAPPER.InitTelephonyExtWrapper();
    TELEPHONY_EXT_WRAPPER.isHandleVSim_ = []() { return true; };
    EXPECT_TRUE(TELEPHONY_EXT_WRAPPER.isHandleVSim_);
    EXPECT_TRUE(TELEPHONY_EXT_WRAPPER.isHandleVSim_());
    multiSimController->isRilSetPrimarySlotSupport_ = true;
    EXPECT_EQ(multiSimController->SetPrimarySlotId(0, true), TELEPHONY_ERR_FAIL);
    multiSimController->isRilSetPrimarySlotSupport_ = false;
    EXPECT_EQ(multiSimController->SetPrimarySlotId(0, false), TELEPHONY_ERR_FAIL);
}

/**
 * @tc.number   Telephony_MultiSimController_007
 * @tc.name     test error branch
 * @tc.desc     Function test
 */
HWTEST_F(BranchTest, Telephony_MultiSimController_007, Function | MediumTest | Level1)
{
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    std::vector<std::shared_ptr<Telephony::SimStateManager>> simStateManager = { nullptr, nullptr };
    std::vector<std::shared_ptr<Telephony::SimFileManager>> simFileManager = { nullptr, nullptr };
    std::shared_ptr<Telephony::MultiSimController> multiSimController =
        std::make_shared<MultiSimController>(telRilManager, simStateManager, simFileManager);
    multiSimController->ObtainDualSimCardStatus();
    bool flag = true;
    multiSimController->SetInSenseSwitchPhase(flag);
    EXPECT_EQ(multiSimController->simStateManager_[0], nullptr);
 
    for (int32_t slotId = 0; slotId < SLOT_COUNT; slotId++) {
        multiSimController->simStateManager_[slotId] = std::make_shared<SimStateManager>(telRilManager);
    }
    multiSimController->ObtainDualSimCardStatus();
    multiSimController->SetInSenseSwitchPhase(flag);
    EXPECT_NE(multiSimController->simStateManager_[0], nullptr);
}
} // namespace Telephony
} // namespace OHOS
