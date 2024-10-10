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

#include "core_service_client.h"
#include "esim_state_type.h"
#include "securec.h"
#include "str_convert.h"
#include "string_ex.h"
#include "telephony_errors.h"
#include "telephony_log_wrapper.h"
#include "gtest/gtest.h"

namespace OHOS {
namespace Telephony {
using namespace testing::ext;
class EsimCoreServiceClientTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};

void EsimCoreServiceClientTest::SetUpTestCase() {}

void EsimCoreServiceClientTest::TearDownTestCase() {}

void EsimCoreServiceClientTest::SetUp() {}

void EsimCoreServiceClientTest::TearDown() {}

HWTEST_F(EsimCoreServiceClientTest, GetEid_0001, Function | MediumTest | Level1)
{
    int32_t slotId = 0;
    std::u16string eId = Str8ToStr16("1A2B3C4D");
    int32_t result = CoreServiceClient::GetInstance().GetEid(slotId, eId);
    EXPECT_NE(result, TELEPHONY_SUCCESS);
}

HWTEST_F(EsimCoreServiceClientTest, GetEuiccProfileInfoList_0001, Function | MediumTest | Level1)
{
    int32_t slotId = 0;
    GetEuiccProfileInfoListResult euiccProfileInfoList;
    int32_t result = CoreServiceClient::GetInstance().GetEuiccProfileInfoList(slotId, euiccProfileInfoList);
    EXPECT_NE(result, TELEPHONY_SUCCESS);
}

HWTEST_F(EsimCoreServiceClientTest, GetEuiccInfo_0001, Function | MediumTest | Level1)
{
    int32_t slotId = 0;
    EuiccInfo euiccInfo;
    euiccInfo.osVersion = Str8ToStr16("BF2003010203");
    int32_t result = CoreServiceClient::GetInstance().GetEuiccInfo(slotId, euiccInfo);
    EXPECT_NE(result, TELEPHONY_SUCCESS);
}

HWTEST_F(EsimCoreServiceClientTest, DisableProfile_0001, Function | MediumTest | Level1)
{
    int32_t slotId = 0;
    int32_t portIndex = 0;
    std::u16string iccId = Str8ToStr16("98760000000000543210");
    bool refresh = true;
    ResultState disableProfileResult;
    int32_t result = CoreServiceClient::GetInstance().DisableProfile(
        slotId, portIndex, iccId, refresh, disableProfileResult);
    EXPECT_NE(result, TELEPHONY_SUCCESS);
}

HWTEST_F(EsimCoreServiceClientTest, GetSmdsAddress_0001, Function | MediumTest | Level1)
{
    int32_t slotId = 0;
    int32_t portIndex = 0;
    std::u16string smdsAddress;
    int32_t result = CoreServiceClient::GetInstance().GetSmdsAddress(slotId, portIndex, smdsAddress);
    EXPECT_NE(result, TELEPHONY_SUCCESS);
}

HWTEST_F(EsimCoreServiceClientTest, GetRulesAuthTable_0001, Function | MediumTest | Level1)
{
    int32_t slotId = 0;
    int32_t portIndex = 0;
    EuiccRulesAuthTable eUiccRulesAuthTable;
    int32_t result = CoreServiceClient::GetInstance().GetRulesAuthTable(slotId, portIndex, eUiccRulesAuthTable);
    EXPECT_NE(result, TELEPHONY_SUCCESS);
}

HWTEST_F(EsimCoreServiceClientTest, GetEuiccChallenge_0001, Function | MediumTest | Level1)
{
    int32_t slotId = 0;
    int32_t portIndex = 0;
    ResponseEsimResult responseResult;
    int32_t result = CoreServiceClient::GetInstance().GetEuiccChallenge(slotId, portIndex, responseResult);
    EXPECT_NE(result, TELEPHONY_SUCCESS);
}
} // namespace Telephony
} // namespace OHOS