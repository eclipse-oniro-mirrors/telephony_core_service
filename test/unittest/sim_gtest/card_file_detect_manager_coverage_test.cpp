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
#define private public
#define protected public

#include <memory>
#include <string>
#include <vector>

#include "card_file_detect_manager.h"
#include "gtest/gtest.h"
#include "mock_multi_sim_controller.h"
#include "mock_sim_rdb_helper.h"
#include "sim_constant.h"
#include "telephony_errors.h"

namespace OHOS {
namespace Telephony {
using namespace testing::ext;
using namespace testing;
constexpr int32_t TEST_MNC_LEN = 2;

class CardFileDetectManagerCoverageTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();

protected:
    std::shared_ptr<MultiSimControllerMock> controller_ = nullptr;
    std::shared_ptr<MockSimRdbHelper> rdbHelper_ = nullptr;
    std::shared_ptr<CardFileDetectManager> manager_ = nullptr;

    CardFileDetectData BuildData(int32_t phyCard)
    {
        CardFileDetectData data;
        data.phyCard = phyCard;
        data.lsi = 1;
        data.mncLen = TEST_MNC_LEN;
        data.iccId = "89860012345678901234";
        data.efust = "FFFFFFFF";
        data.imsi = "460001234567890";
        data.gid1 = "01";
        data.gid2 = "02";
        data.spn = "SPN";
        data.ehplmn = "46000";
        return data;
    }

    SimRdbInfo BuildRdbInfo(const std::string &iccId)
    {
        SimRdbInfo info;
        info.iccId = iccId;
        return info;
    }
};

void CardFileDetectManagerCoverageTest::SetUpTestCase() {}

void CardFileDetectManagerCoverageTest::TearDownTestCase() {}

void CardFileDetectManagerCoverageTest::SetUp()
{
    controller_ = std::make_shared<MultiSimControllerMock>();
    rdbHelper_ = std::make_shared<MockSimRdbHelper>();
    manager_ = std::make_shared<CardFileDetectManager>(controller_);
    manager_->simRdbHelper_ = rdbHelper_;
}

void CardFileDetectManagerCoverageTest::TearDown()
{
    manager_ = nullptr;
    rdbHelper_ = nullptr;
    controller_ = nullptr;
}

/**
 * @tc.number: CardFileDetectManager_IsIccIdExists_Empty
 * @tc.name: IsIccIdExists rejects empty iccId
 * @tc.desc: Verify empty iccId returns false without querying database.
 */
HWTEST_F(CardFileDetectManagerCoverageTest, CardFileDetectManager_IsIccIdExists_Empty,
    Function | MediumTest | Level2)
{
    EXPECT_CALL(*rdbHelper_, QueryDataByIccId(_, _)).Times(0);
    EXPECT_FALSE(manager_->IsIccIdExists(""));
}

/**
 * @tc.number: CardFileDetectManager_IsIccIdExists_QueryFail
 * @tc.name: IsIccIdExists handles database query failure
 * @tc.desc: Verify query failure returns false.
 */
HWTEST_F(CardFileDetectManagerCoverageTest, CardFileDetectManager_IsIccIdExists_QueryFail,
    Function | MediumTest | Level2)
{
    EXPECT_CALL(*rdbHelper_, QueryDataByIccId(_, _)).WillOnce(Return(INVALID_VALUE));
    EXPECT_FALSE(manager_->IsIccIdExists("89860012345678901234"));
}

/**
 * @tc.number: CardFileDetectManager_IsIccIdExists_EmptyResult
 * @tc.name: IsIccIdExists handles empty query result
 * @tc.desc: Verify query success but empty iccId returns false.
 */
HWTEST_F(CardFileDetectManagerCoverageTest, CardFileDetectManager_IsIccIdExists_EmptyResult,
    Function | MediumTest | Level2)
{
    EXPECT_CALL(*rdbHelper_, QueryDataByIccId(_, _))
        .WillOnce(DoAll(SetArgReferee<1>(SimRdbInfo()), Return(TELEPHONY_SUCCESS)));
    EXPECT_FALSE(manager_->IsIccIdExists("89860012345678901234"));
}

/**
 * @tc.number: CardFileDetectManager_IsIccIdExists_Exists
 * @tc.name: IsIccIdExists returns true when record exists
 * @tc.desc: Verify query success with non-empty iccId returns true.
 */
HWTEST_F(CardFileDetectManagerCoverageTest, CardFileDetectManager_IsIccIdExists_Exists,
    Function | MediumTest | Level2)
{
    EXPECT_CALL(*rdbHelper_, QueryDataByIccId(_, _))
        .WillOnce(DoAll(SetArgReferee<1>(BuildRdbInfo("89860012345678901234")), Return(TELEPHONY_SUCCESS)));
    EXPECT_TRUE(manager_->IsIccIdExists("89860012345678901234"));
}

/**
 * @tc.number: CardFileDetectManager_BuildDataShareValues_Physical
 * @tc.name: BuildDataShareValues with physical card
 * @tc.desc: Verify physical card branch fills label index and esim flag.
 */
HWTEST_F(CardFileDetectManagerCoverageTest, CardFileDetectManager_BuildDataShareValues_Physical,
    Function | MediumTest | Level2)
{
    DataShare::DataShareValuesBucket values;
    manager_->BuildDataShareValues(BuildData(0), values, true);
    DataShare::DataShareValuesBucket values1;
    manager_->BuildDataShareValues(BuildData(1), values1, false);
    EXPECT_TRUE(!values.IsEmpty());
    EXPECT_TRUE(!values1.IsEmpty());
}

/**
 * @tc.number: CardFileDetectManager_BuildDataShareValues_Esim
 * @tc.name: BuildDataShareValues with esim card
 * @tc.desc: Verify esim branch handles insert and update and zero mncLen.
 */
HWTEST_F(CardFileDetectManagerCoverageTest, CardFileDetectManager_BuildDataShareValues_Esim,
    Function | MediumTest | Level2)
{
    auto data = BuildData(2);
    DataShare::DataShareValuesBucket values;
    manager_->BuildDataShareValues(data, values, true);
    DataShare::DataShareValuesBucket values1;
    manager_->BuildDataShareValues(data, values1, false);
    EXPECT_TRUE(!values.IsEmpty());

    data.mncLen = 0;
    data.iccId = "";
    data.efust = "";
    data.imsi = "";
    data.gid1 = "";
    data.gid2 = "";
    data.spn = "";
    data.ehplmn = "";
    DataShare::DataShareValuesBucket values2;
    manager_->BuildDataShareValues(data, values2, false);
    EXPECT_TRUE(!values2.IsEmpty());
}

/**
 * @tc.number: CardFileDetectManager_MaskSensitiveData
 * @tc.name: MaskSensitiveData masks after keep section
 * @tc.desc: Verify masking behavior for empty and long strings.
 */
HWTEST_F(CardFileDetectManagerCoverageTest, CardFileDetectManager_MaskSensitiveData,
    Function | MediumTest | Level2)
{
    EXPECT_EQ(manager_->MaskSensitiveData(""), "");
    EXPECT_EQ(manager_->MaskSensitiveData("1234"), "1234");
    EXPECT_EQ(manager_->MaskSensitiveData("12345678"), "1234****");
    EXPECT_EQ(manager_->MaskSensitiveData("1234567890"), "1234****90");
}

/**
 * @tc.number: CardFileDetectManager_Insert_Fail
 * @tc.name: InsertCardFileDetectData handles insert failure
 * @tc.desc: Verify insert failure maps to database write error.
 */
HWTEST_F(CardFileDetectManagerCoverageTest, CardFileDetectManager_Insert_Fail,
    Function | MediumTest | Level2)
{
    EXPECT_CALL(*rdbHelper_, InsertData(_, _)).WillOnce(Return(INVALID_VALUE));
    EXPECT_EQ(manager_->InsertCardFileDetectData(BuildData(0)), TELEPHONY_ERR_DATABASE_WRITE_FAIL);
}

/**
 * @tc.number: CardFileDetectManager_Insert_Success
 * @tc.name: InsertCardFileDetectData succeeds
 * @tc.desc: Verify insert success returns TELEPHONY_SUCCESS.
 */
HWTEST_F(CardFileDetectManagerCoverageTest, CardFileDetectManager_Insert_Success,
    Function | MediumTest | Level2)
{
    EXPECT_CALL(*rdbHelper_, InsertData(_, _)).WillOnce(Return(TELEPHONY_SUCCESS));
    EXPECT_EQ(manager_->InsertCardFileDetectData(BuildData(0)), TELEPHONY_SUCCESS);
}

/**
 * @tc.number: CardFileDetectManager_Update_Fail
 * @tc.name: UpdateCardFileDetectData handles update failure
 * @tc.desc: Verify update failure maps to database write error.
 */
HWTEST_F(CardFileDetectManagerCoverageTest, CardFileDetectManager_Update_Fail,
    Function | MediumTest | Level2)
{
    EXPECT_CALL(*rdbHelper_, UpdateDataByIccId(_, _)).WillOnce(Return(INVALID_VALUE));
    EXPECT_EQ(manager_->UpdateCardFileDetectData(BuildData(0)), TELEPHONY_ERR_DATABASE_WRITE_FAIL);
}

/**
 * @tc.number: CardFileDetectManager_Update_Success
 * @tc.name: UpdateCardFileDetectData succeeds
 * @tc.desc: Verify update success returns TELEPHONY_SUCCESS.
 */
HWTEST_F(CardFileDetectManagerCoverageTest, CardFileDetectManager_Update_Success,
    Function | MediumTest | Level2)
{
    EXPECT_CALL(*rdbHelper_, UpdateDataByIccId(_, _)).WillOnce(Return(TELEPHONY_SUCCESS));
    EXPECT_EQ(manager_->UpdateCardFileDetectData(BuildData(0)), TELEPHONY_SUCCESS);
}

/**
 * @tc.number: CardFileDetectManager_Save_EmptyIccId
 * @tc.name: SaveCardFileDetectData rejects empty iccId
 * @tc.desc: Verify empty iccId returns argument invalid.
 */
HWTEST_F(CardFileDetectManagerCoverageTest, CardFileDetectManager_Save_EmptyIccId,
    Function | MediumTest | Level2)
{
    auto data = BuildData(0);
    data.iccId = "";
    EXPECT_EQ(manager_->SaveCardFileDetectData(data), TELEPHONY_ERR_ARGUMENT_INVALID);
}

/**
 * @tc.number: CardFileDetectManager_Save_UpdatePath
 * @tc.name: SaveCardFileDetectData updates existing record
 * @tc.desc: Verify existing iccId triggers update and reloads list.
 */
HWTEST_F(CardFileDetectManagerCoverageTest, CardFileDetectManager_Save_UpdatePath,
    Function | MediumTest | Level2)
{
    EXPECT_CALL(*rdbHelper_, QueryDataByIccId(_, _))
        .WillOnce(DoAll(SetArgReferee<1>(BuildRdbInfo("89860012345678901234")), Return(TELEPHONY_SUCCESS)));
    EXPECT_CALL(*rdbHelper_, UpdateDataByIccId(_, _)).WillOnce(Return(TELEPHONY_SUCCESS));
    EXPECT_EQ(manager_->SaveCardFileDetectData(BuildData(0)), TELEPHONY_SUCCESS);
}

/**
 * @tc.number: CardFileDetectManager_Save_InsertPath
 * @tc.name: SaveCardFileDetectData inserts new record
 * @tc.desc: Verify missing iccId triggers insert.
 */
HWTEST_F(CardFileDetectManagerCoverageTest, CardFileDetectManager_Save_InsertPath,
    Function | MediumTest | Level2)
{
    EXPECT_CALL(*rdbHelper_, QueryDataByIccId(_, _))
        .WillOnce(DoAll(SetArgReferee<1>(SimRdbInfo()), Return(TELEPHONY_SUCCESS)));
    EXPECT_CALL(*rdbHelper_, InsertData(_, _)).WillOnce(Return(TELEPHONY_SUCCESS));
    EXPECT_EQ(manager_->SaveCardFileDetectData(BuildData(2)), TELEPHONY_SUCCESS);
}

/**
 * @tc.number: CardFileDetectManager_Save_NullController
 * @tc.name: SaveCardFileDetectData with expired controller
 * @tc.desc: Verify success path skips reload when controller is expired.
 */
HWTEST_F(CardFileDetectManagerCoverageTest, CardFileDetectManager_Save_NullController,
    Function | MediumTest | Level2)
{
    controller_ = nullptr;
    EXPECT_CALL(*rdbHelper_, QueryDataByIccId(_, _))
        .WillOnce(DoAll(SetArgReferee<1>(BuildRdbInfo("89860012345678901234")), Return(TELEPHONY_SUCCESS)));
    EXPECT_CALL(*rdbHelper_, UpdateDataByIccId(_, _)).WillOnce(Return(TELEPHONY_SUCCESS));
    EXPECT_EQ(manager_->SaveCardFileDetectData(BuildData(0)), TELEPHONY_SUCCESS);
}

/**
 * @tc.number: CardFileDetectManager_GetAllSimCardInfo_Fail
 * @tc.name: GetAllSimCardInfo handles query failure
 * @tc.desc: Verify query failure returns the error code.
 */
HWTEST_F(CardFileDetectManagerCoverageTest, CardFileDetectManager_GetAllSimCardInfo_Fail,
    Function | MediumTest | Level2)
{
    EXPECT_CALL(*rdbHelper_, QueryAllData(_)).WillOnce(Return(INVALID_VALUE));
    std::vector<SimCardInfo> results;
    EXPECT_EQ(manager_->GetAllSimCardInfo(results), INVALID_VALUE);
    EXPECT_TRUE(results.empty());
}

/**
 * @tc.number: CardFileDetectManager_GetAllSimCardInfo_Success
 * @tc.name: GetAllSimCardInfo converts database records
 * @tc.desc: Verify records are converted to SimCardInfo including esim type.
 */
HWTEST_F(CardFileDetectManagerCoverageTest, CardFileDetectManager_GetAllSimCardInfo_Success,
    Function | MediumTest | Level2)
{
    std::vector<SimRdbInfo> rdbResult(2);
    rdbResult[0].iccId = "icc0";
    rdbResult[0].isEsim = 0;
    rdbResult[0].simLabelIndex = 1;
    rdbResult[1].iccId = "icc1";
    rdbResult[1].isEsim = 1;
    rdbResult[1].simLabelIndex = 2;
    EXPECT_CALL(*rdbHelper_, QueryAllData(_))
        .WillOnce(DoAll(SetArgReferee<0>(rdbResult), Return(TELEPHONY_SUCCESS)));
    std::vector<SimCardInfo> results;
    EXPECT_EQ(manager_->GetAllSimCardInfo(results), TELEPHONY_SUCCESS);
    ASSERT_EQ(results.size(), 2U);
    EXPECT_EQ(results[0].iccId, "icc0");
    EXPECT_EQ(results[0].simlabel.simType, SimType::PSIM);
    EXPECT_EQ(results[1].simlabel.simType, SimType::ESIM);
}
} // namespace Telephony
} // namespace OHOS