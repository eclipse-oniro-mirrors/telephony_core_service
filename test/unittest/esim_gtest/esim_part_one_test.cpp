/*
 * Copyright (C) 2024 Huawei Device Co., Ltd.
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

#include "asn1_node.h"
#include "common_event_manager.h"
#include "common_event_support.h"
#include "esim_file.h"
#include "icc_file.h"
#include "sim_file_manager.h"
#include "sim_constant.h"
#include "sim_file_manager.h"
#include "tel_ril_manager.h"
#include "telephony_tag_def.h"
#include "gtest/gtest.h"

namespace OHOS {
namespace Telephony {
using namespace testing::ext;
class EsimPartOneTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};

void EsimPartOneTest::TearDownTestCase() {}

void EsimPartOneTest::SetUp() {}

void EsimPartOneTest::TearDown() {}

void EsimPartOneTest::SetUpTestCase() {}

HWTEST_F(EsimPartOneTest, IsSameAid_001, Function | MediumTest | Level2)
{
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    std::shared_ptr<Telephony::SimStateManager> simStateManager = std::make_shared<SimStateManager>(telRilManager);
    std::shared_ptr<Telephony::EsimFile> esimFile = std::make_shared<EsimFile>(telRilManager);
    const std::u16string aid = u"112223334444555";
    EXPECT_FALSE(esimFile->IsSameAid(aid));
    esimFile->aidStr_ = u"112223334444556";
    EXPECT_FALSE(esimFile->IsValidAidForAllowSameAidReuseChannel(aid));
}

HWTEST_F(EsimPartOneTest, ResetEuiccNotification_001, Function | MediumTest | Level2)
{
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    std::shared_ptr<Telephony::SimStateManager> simStateManager = std::make_shared<SimStateManager>(telRilManager);
    std::shared_ptr<Telephony::EsimFile> esimFile = std::make_shared<EsimFile>(telRilManager);
    std::shared_ptr<Asn1Builder> devCapsBuilder = nullptr;
    esimFile->ResetEuiccNotification();
    const std::u16string aid = u"112223334444555";
    EXPECT_FALSE(esimFile->IsSameAid(aid));
}

HWTEST_F(EsimPartOneTest, ProcessObtainEuiccInfo2Done_001, Function | MediumTest | Level2)
{
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    std::shared_ptr<Telephony::SimStateManager> simStateManager = std::make_shared<SimStateManager>(telRilManager);
    std::shared_ptr<Telephony::EsimFile> esimFile = std::make_shared<EsimFile>(telRilManager);
    auto event = AppExecFwk::InnerEvent::Get(0);
    EXPECT_FALSE(esimFile->ProcessObtainEuiccInfo2Done(event));
    std::shared_ptr<IccControllerHolder> holder = nullptr;
    std::unique_ptr<Telephony::IccFromRilMsg> rcvMsg = std::make_unique<Telephony::IccFromRilMsg>(holder);
    rcvMsg->fileData.resultData = "BF2B0101";
    event = AppExecFwk::InnerEvent::Get(0, rcvMsg);
    EXPECT_FALSE(esimFile->ProcessObtainEuiccInfo2Done(event));
    rcvMsg = nullptr;
    event = AppExecFwk::InnerEvent::Get(0, rcvMsg);
    EXPECT_FALSE(esimFile->ProcessObtainEuiccInfo2Done(event));
}

HWTEST_F(EsimPartOneTest, RetrieveNotificatioParseTagCtxComp0_001, Function | MediumTest | Level2)
{
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    std::shared_ptr<Telephony::SimStateManager> simStateManager = std::make_shared<SimStateManager>(telRilManager);
    std::shared_ptr<Telephony::EsimFile> esimFile = std::make_shared<EsimFile>(telRilManager);
    std::vector<uint8_t> src;
    std::shared_ptr<Asn1Node> root = std::make_shared<Asn1Node>(0, src, 0, 0);
    EXPECT_FALSE(esimFile->RetrieveNotificatioParseTagCtxComp0(root));
}

HWTEST_F(EsimPartOneTest, ProcessRetrieveNotificationDone_001, Function | MediumTest | Level2)
{
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    std::shared_ptr<Telephony::SimStateManager> simStateManager = std::make_shared<SimStateManager>(telRilManager);
    std::shared_ptr<Telephony::EsimFile> esimFile = std::make_shared<EsimFile>(telRilManager);
    std::shared_ptr<IccControllerHolder> holder = nullptr;
    std::unique_ptr<Telephony::IccFromRilMsg> rcvMsg = std::make_unique<Telephony::IccFromRilMsg>(holder);
    rcvMsg->fileData.resultData = "BF2B0101";
    auto event = AppExecFwk::InnerEvent::Get(0, rcvMsg);
    EXPECT_FALSE(esimFile->ProcessRetrieveNotificationDone(event));
    event = nullptr;
    EXPECT_FALSE(esimFile->ProcessRetrieveNotificationDone(event));
    rcvMsg = nullptr;
    event = AppExecFwk::InnerEvent::Get(0, rcvMsg);
    EXPECT_FALSE(esimFile->ProcessRetrieveNotificationDone(event));
}

HWTEST_F(EsimPartOneTest, RealProcessLoadBoundProfilePackageDone_003, Function | MediumTest | Level2)
{
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    std::shared_ptr<Telephony::SimStateManager> simStateManager = std::make_shared<SimStateManager>(telRilManager);
    std::shared_ptr<Telephony::EsimFile> esimFile = std::make_shared<EsimFile>(telRilManager);
    ResponseEsimInnerResult dst;
    AuthServerResponse src;
    esimFile->CovertAuthToApiStruct(dst, src);
    
    EXPECT_FALSE(esimFile->RealProcessLoadBoundProfilePackageDone());
}
HWTEST_F(EsimPartOneTest, RealProcessPrepareDownloadDone_001, Function | MediumTest | Level2)
{
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    std::shared_ptr<Telephony::SimStateManager> simStateManager = std::make_shared<SimStateManager>(telRilManager);
    std::shared_ptr<Telephony::EsimFile> esimFile = std::make_shared<EsimFile>(telRilManager);
    EXPECT_FALSE(esimFile->RealProcessPrepareDownloadDone());
}

HWTEST_F(EsimPartOneTest, AddCtxParams1_001, Function | MediumTest | Level2)
{
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    std::shared_ptr<Telephony::SimStateManager> simStateManager = std::make_shared<SimStateManager>(telRilManager);
    std::shared_ptr<Telephony::EsimFile> esimFile = std::make_shared<EsimFile>(telRilManager);
    std::shared_ptr<Asn1Builder> ctxParams1Builder = nullptr;
    Es9PlusInitAuthResp authRespData;
    esimFile->AddCtxParams1(ctxParams1Builder, authRespData);
    EXPECT_EQ(ctxParams1Builder, nullptr);
}

HWTEST_F(EsimPartOneTest, LoadBoundProfilePackageParseNotificationMetadata_001, Function | MediumTest | Level2)
{
    std::shared_ptr<TelRilManager> telRilManager = std::make_shared<TelRilManager>();
    std::shared_ptr<Telephony::SimStateManager> simStateManager = std::make_shared<SimStateManager>(telRilManager);
    std::shared_ptr<Telephony::EsimFile> esimFile = std::make_shared<EsimFile>(telRilManager);
    std::shared_ptr<Asn1Node> notificationMetadata = nullptr;
    EXPECT_FALSE(esimFile->LoadBoundProfilePackageParseNotificationMetadata(notificationMetadata));
}
}
// namespace Telephony
} // namespace OHOS