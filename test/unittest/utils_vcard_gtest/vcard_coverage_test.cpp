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

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "mock_data_share_result_set.h"
#include "mock_datashare_helper.h"
#include "telephony_errors.h"
#include "vcard_email_data.h"
#include "vcard_event_data.h"
#include "vcard_group_data.h"
#include "vcard_im_data.h"
#include "vcard_rdb_helper.h"

namespace OHOS {
namespace Telephony {
using namespace testing::ext;
using namespace testing;

class VCardCoverageTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};

void VCardCoverageTest::SetUpTestCase() {}

void VCardCoverageTest::TearDownTestCase() {}

void VCardCoverageTest::SetUp() {}

void VCardCoverageTest::TearDown()
{
    VCardRdbHelper::SetDataHelper(nullptr);
}

HWTEST_F(VCardCoverageTest, VCardEmailData_BuildData_NullResultSet, Function | MediumTest | Level1)
{
    VCardEmailData emailData;
    EXPECT_EQ(emailData.BuildData(nullptr), TELEPHONY_ERROR);
}

HWTEST_F(VCardCoverageTest, VCardEmailData_BuildData_DetailColumnFail, Function | MediumTest | Level1)
{
    auto mock = std::make_shared<DataShareResultSetMock>();
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::DETAIL_INFO, _))
        .WillRepeatedly(Return(DataShare::E_ERROR));
    VCardEmailData emailData;
    EXPECT_EQ(emailData.BuildData(mock), TELEPHONY_ERROR);
}

HWTEST_F(VCardCoverageTest, VCardEmailData_BuildData_DetailValueFail, Function | MediumTest | Level1)
{
    auto mock = std::make_shared<DataShareResultSetMock>();
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::DETAIL_INFO, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(0), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetString(0, _)).WillRepeatedly(Return(DataShare::E_ERROR));
    VCardEmailData emailData;
    EXPECT_EQ(emailData.BuildData(mock), TELEPHONY_ERROR);
}

HWTEST_F(VCardCoverageTest, VCardEmailData_BuildData_LabelNameColumnFail, Function | MediumTest | Level1)
{
    auto mock = std::make_shared<DataShareResultSetMock>();
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::DETAIL_INFO, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(0), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetString(0, _)).WillRepeatedly(Return(DataShare::E_OK));
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::LABEL_NAME, _))
        .WillRepeatedly(Return(DataShare::E_ERROR));
    VCardEmailData emailData;
    EXPECT_EQ(emailData.BuildData(mock), TELEPHONY_ERROR);
}

HWTEST_F(VCardCoverageTest, VCardEmailData_BuildData_LabelNameValueFail, Function | MediumTest | Level1)
{
    auto mock = std::make_shared<DataShareResultSetMock>();
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::DETAIL_INFO, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(0), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetString(0, _)).WillRepeatedly(Return(DataShare::E_OK));
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::LABEL_NAME, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(1), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetString(1, _)).WillRepeatedly(Return(DataShare::E_ERROR));
    VCardEmailData emailData;
    EXPECT_EQ(emailData.BuildData(mock), TELEPHONY_ERROR);
}

HWTEST_F(VCardCoverageTest, VCardEmailData_BuildData_LabelIdColumnFail, Function | MediumTest | Level1)
{
    auto mock = std::make_shared<DataShareResultSetMock>();
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::DETAIL_INFO, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(0), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetString(0, _)).WillRepeatedly(Return(DataShare::E_OK));
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::LABEL_NAME, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(1), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetString(1, _)).WillRepeatedly(Return(DataShare::E_OK));
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::LABEL_ID, _))
        .WillRepeatedly(Return(DataShare::E_ERROR));
    VCardEmailData emailData;
    EXPECT_EQ(emailData.BuildData(mock), TELEPHONY_ERROR);
}

HWTEST_F(VCardCoverageTest, VCardEmailData_BuildData_LabelIdValueFail, Function | MediumTest | Level1)
{
    auto mock = std::make_shared<DataShareResultSetMock>();
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::DETAIL_INFO, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(0), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetString(0, _)).WillRepeatedly(Return(DataShare::E_OK));
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::LABEL_NAME, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(1), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetString(1, _)).WillRepeatedly(Return(DataShare::E_OK));
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::LABEL_ID, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(2), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetString(2, _)).WillRepeatedly(Return(DataShare::E_ERROR));
    VCardEmailData emailData;
    EXPECT_EQ(emailData.BuildData(mock), TELEPHONY_ERROR);
}

HWTEST_F(VCardCoverageTest, VCardEmailData_BuildData_AliasColumnFail, Function | MediumTest | Level1)
{
    auto mock = std::make_shared<DataShareResultSetMock>();
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::DETAIL_INFO, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(0), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetString(0, _)).WillRepeatedly(Return(DataShare::E_OK));
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::LABEL_NAME, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(1), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetString(1, _)).WillRepeatedly(Return(DataShare::E_OK));
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::LABEL_ID, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(2), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetString(2, _)).WillRepeatedly(Return(DataShare::E_OK));
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::ALIAS_DETAIL_INFO_KEY, _))
        .WillRepeatedly(Return(DataShare::E_ERROR));
    VCardEmailData emailData;
    EXPECT_EQ(emailData.BuildData(mock), TELEPHONY_ERROR);
}

HWTEST_F(VCardCoverageTest, VCardEmailData_BuildData_AliasValueFail, Function | MediumTest | Level1)
{
    auto mock = std::make_shared<DataShareResultSetMock>();
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::DETAIL_INFO, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(0), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetString(0, _)).WillRepeatedly(Return(DataShare::E_OK));
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::LABEL_NAME, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(1), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetString(1, _)).WillRepeatedly(Return(DataShare::E_OK));
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::LABEL_ID, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(2), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetString(2, _)).WillRepeatedly(Return(DataShare::E_OK));
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::ALIAS_DETAIL_INFO_KEY, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(3), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetString(3, _)).WillRepeatedly(Return(DataShare::E_ERROR));
    VCardEmailData emailData;
    EXPECT_EQ(emailData.BuildData(mock), TELEPHONY_ERROR);
}

HWTEST_F(VCardCoverageTest, VCardEmailData_Setters, Function | MediumTest | Level1)
{
    VCardEmailData emailData;
    emailData.SetAddress("test@test.com");
    emailData.SetLabelName("label");
    emailData.SetDisplayName("display");
    EXPECT_EQ(emailData.GetAddress(), "test@test.com");
    EXPECT_EQ(emailData.GetLabelName(), "label");
    EXPECT_EQ(emailData.GetDisplayName(), "display");
}

HWTEST_F(VCardCoverageTest, VCardImData_BuildData_NullResultSet, Function | MediumTest | Level1)
{
    VCardImData imData;
    EXPECT_EQ(imData.BuildData(nullptr), TELEPHONY_ERROR);
}

HWTEST_F(VCardCoverageTest, VCardImData_BuildData_DetailColumnFail, Function | MediumTest | Level1)
{
    auto mock = std::make_shared<DataShareResultSetMock>();
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::DETAIL_INFO, _))
        .WillRepeatedly(Return(DataShare::E_ERROR));
    VCardImData imData;
    EXPECT_EQ(imData.BuildData(mock), TELEPHONY_ERROR);
}

HWTEST_F(VCardCoverageTest, VCardImData_BuildData_DetailValueFail, Function | MediumTest | Level1)
{
    auto mock = std::make_shared<DataShareResultSetMock>();
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::DETAIL_INFO, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(0), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetString(0, _)).WillRepeatedly(Return(DataShare::E_ERROR));
    VCardImData imData;
    EXPECT_EQ(imData.BuildData(mock), TELEPHONY_ERROR);
}

HWTEST_F(VCardCoverageTest, VCardImData_BuildData_LabelIdColumnFail, Function | MediumTest | Level1)
{
    auto mock = std::make_shared<DataShareResultSetMock>();
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::DETAIL_INFO, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(0), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetString(0, _)).WillRepeatedly(Return(DataShare::E_OK));
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::LABEL_ID, _))
        .WillRepeatedly(Return(DataShare::E_ERROR));
    VCardImData imData;
    EXPECT_EQ(imData.BuildData(mock), TELEPHONY_ERROR);
}

HWTEST_F(VCardCoverageTest, VCardImData_BuildData_LabelIdValueFail, Function | MediumTest | Level1)
{
    auto mock = std::make_shared<DataShareResultSetMock>();
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::DETAIL_INFO, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(0), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetString(0, _)).WillRepeatedly(Return(DataShare::E_OK));
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::LABEL_ID, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(1), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetString(1, _)).WillRepeatedly(Return(DataShare::E_ERROR));
    VCardImData imData;
    EXPECT_EQ(imData.BuildData(mock), TELEPHONY_ERROR);
}

HWTEST_F(VCardCoverageTest, VCardEventData_BuildValuesBucket, Function | MediumTest | Level1)
{
    VCardEventData eventData;
    eventData.SetEventDate("2024-01-01");
    eventData.SetLabelId("1");
    eventData.SetLabelName("birthday");
    DataShare::DataShareValuesBucket valuesBucket;
    EXPECT_EQ(eventData.BuildValuesBucket(valuesBucket), TELEPHONY_SUCCESS);
}

HWTEST_F(VCardCoverageTest, VCardEventData_BuildData_NullResultSet, Function | MediumTest | Level1)
{
    VCardEventData eventData;
    EXPECT_EQ(eventData.BuildData(nullptr), TELEPHONY_ERROR);
}

HWTEST_F(VCardCoverageTest, VCardEventData_BuildData_DetailColumnFail, Function | MediumTest | Level1)
{
    auto mock = std::make_shared<DataShareResultSetMock>();
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::DETAIL_INFO, _))
        .WillRepeatedly(Return(DataShare::E_ERROR));
    VCardEventData eventData;
    EXPECT_EQ(eventData.BuildData(mock), TELEPHONY_ERROR);
}

HWTEST_F(VCardCoverageTest, VCardEventData_BuildData_DetailValueFail, Function | MediumTest | Level1)
{
    auto mock = std::make_shared<DataShareResultSetMock>();
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::DETAIL_INFO, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(0), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetString(0, _)).WillRepeatedly(Return(DataShare::E_ERROR));
    VCardEventData eventData;
    EXPECT_EQ(eventData.BuildData(mock), TELEPHONY_ERROR);
}

HWTEST_F(VCardCoverageTest, VCardEventData_BuildData_LabelNameColumnFail, Function | MediumTest | Level1)
{
    auto mock = std::make_shared<DataShareResultSetMock>();
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::DETAIL_INFO, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(0), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetString(0, _)).WillRepeatedly(Return(DataShare::E_OK));
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::LABEL_NAME, _))
        .WillRepeatedly(Return(DataShare::E_ERROR));
    VCardEventData eventData;
    EXPECT_EQ(eventData.BuildData(mock), TELEPHONY_ERROR);
}

HWTEST_F(VCardCoverageTest, VCardEventData_BuildData_LabelNameValueFail, Function | MediumTest | Level1)
{
    auto mock = std::make_shared<DataShareResultSetMock>();
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::DETAIL_INFO, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(0), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetString(0, _)).WillRepeatedly(Return(DataShare::E_OK));
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::LABEL_NAME, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(1), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetString(1, _)).WillRepeatedly(Return(DataShare::E_ERROR));
    VCardEventData eventData;
    EXPECT_EQ(eventData.BuildData(mock), TELEPHONY_ERROR);
}

HWTEST_F(VCardCoverageTest, VCardEventData_BuildData_LabelIdColumnFail, Function | MediumTest | Level1)
{
    auto mock = std::make_shared<DataShareResultSetMock>();
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::DETAIL_INFO, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(0), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetString(0, _)).WillRepeatedly(Return(DataShare::E_OK));
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::LABEL_NAME, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(1), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetString(1, _)).WillRepeatedly(Return(DataShare::E_OK));
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::LABEL_ID, _))
        .WillRepeatedly(Return(DataShare::E_ERROR));
    VCardEventData eventData;
    EXPECT_EQ(eventData.BuildData(mock), TELEPHONY_ERROR);
}

HWTEST_F(VCardCoverageTest, VCardEventData_BuildData_LabelIdValueFail, Function | MediumTest | Level1)
{
    auto mock = std::make_shared<DataShareResultSetMock>();
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::DETAIL_INFO, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(0), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetString(0, _)).WillRepeatedly(Return(DataShare::E_OK));
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::LABEL_NAME, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(1), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetString(1, _)).WillRepeatedly(Return(DataShare::E_OK));
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::LABEL_ID, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(2), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetString(2, _)).WillRepeatedly(Return(DataShare::E_ERROR));
    VCardEventData eventData;
    EXPECT_EQ(eventData.BuildData(mock), TELEPHONY_ERROR);
}

HWTEST_F(VCardCoverageTest, VCardGroupData_BuildData_InvalidGroupId, Function | MediumTest | Level1)
{
    auto mock = std::make_shared<DataShareResultSetMock>();
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::DETAIL_INFO, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(0), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetString(0, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>("not_a_number"), Return(DataShare::E_OK)));
    VCardGroupData groupData;
    EXPECT_EQ(groupData.BuildData(mock), TELEPHONY_ERROR);
}

HWTEST_F(VCardCoverageTest, VCardGroupData_BuildData_NameResultNull, Function | MediumTest | Level1)
{
    auto mock = std::make_shared<DataShareResultSetMock>();
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::DETAIL_INFO, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(0), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetString(0, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>("1"), Return(DataShare::E_OK)));
    auto helper = std::make_shared<MockDataShareHelper>();
    EXPECT_CALL(*helper, Query(_, _, _, _)).WillRepeatedly(Return(nullptr));
    VCardRdbHelper::SetDataHelper(helper);
    VCardGroupData groupData;
    EXPECT_EQ(groupData.BuildData(mock), DB_FAILD);
    VCardRdbHelper::SetDataHelper(nullptr);
}

HWTEST_F(VCardCoverageTest, VCardGroupData_BuildData_GoToFirstRowFail, Function | MediumTest | Level1)
{
    auto mock = std::make_shared<DataShareResultSetMock>();
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::DETAIL_INFO, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(0), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetString(0, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>("2"), Return(DataShare::E_OK)));
    auto nameResultSet = std::make_shared<DataShareResultSetMock>();
    EXPECT_CALL(*nameResultSet, GoToFirstRow()).WillRepeatedly(Return(DataShare::E_ERROR));
    EXPECT_CALL(*nameResultSet, Close()).WillRepeatedly(Return(DataShare::E_OK));
    auto helper = std::make_shared<MockDataShareHelper>();
    EXPECT_CALL(*helper, Query(_, _, _, _)).WillRepeatedly(Return(
        std::static_pointer_cast<DataShare::DataShareResultSet>(nameResultSet)));
    VCardRdbHelper::SetDataHelper(helper);
    VCardGroupData groupData;
    EXPECT_EQ(groupData.BuildData(mock), TELEPHONY_SUCCESS);
    EXPECT_EQ(groupData.GetGroupId(), 2);
    EXPECT_EQ(groupData.GetGroupName(), "");
    VCardRdbHelper::SetDataHelper(nullptr);
}

HWTEST_F(VCardCoverageTest, VCardGroupData_BuildData_GroupNameColumnFail, Function | MediumTest | Level1)
{
    auto mock = std::make_shared<DataShareResultSetMock>();
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::DETAIL_INFO, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(0), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetString(0, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>("3"), Return(DataShare::E_OK)));
    auto nameResultSet = std::make_shared<DataShareResultSetMock>();
    EXPECT_CALL(*nameResultSet, GoToFirstRow()).WillRepeatedly(Return(TELEPHONY_ERR_SUCCESS));
    EXPECT_CALL(*nameResultSet, GetColumnIndex(Group::GROUP_NAME, _))
        .WillRepeatedly(Return(DataShare::E_ERROR));
    EXPECT_CALL(*nameResultSet, Close()).WillRepeatedly(Return(DataShare::E_OK));
    auto helper = std::make_shared<MockDataShareHelper>();
    EXPECT_CALL(*helper, Query(_, _, _, _)).WillRepeatedly(Return(
        std::static_pointer_cast<DataShare::DataShareResultSet>(nameResultSet)));
    VCardRdbHelper::SetDataHelper(helper);
    VCardGroupData groupData;
    EXPECT_EQ(groupData.BuildData(mock), TELEPHONY_SUCCESS);
    EXPECT_EQ(groupData.GetGroupId(), 3);
    VCardRdbHelper::SetDataHelper(nullptr);
}

HWTEST_F(VCardCoverageTest, VCardGroupData_BuildData_Success, Function | MediumTest | Level1)
{
    auto mock = std::make_shared<DataShareResultSetMock>();
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::DETAIL_INFO, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(0), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetString(0, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>("5"), Return(DataShare::E_OK)));
    auto nameResultSet = std::make_shared<DataShareResultSetMock>();
    EXPECT_CALL(*nameResultSet, GoToFirstRow()).WillRepeatedly(Return(TELEPHONY_ERR_SUCCESS));
    EXPECT_CALL(*nameResultSet, GetColumnIndex(Group::GROUP_NAME, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(0), Return(DataShare::E_OK)));
    EXPECT_CALL(*nameResultSet, GetString(0, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>("Friends"), Return(DataShare::E_OK)));
    EXPECT_CALL(*nameResultSet, Close()).WillRepeatedly(Return(DataShare::E_OK));
    auto helper = std::make_shared<MockDataShareHelper>();
    EXPECT_CALL(*helper, Query(_, _, _, _)).WillRepeatedly(Return(
        std::static_pointer_cast<DataShare::DataShareResultSet>(nameResultSet)));
    VCardRdbHelper::SetDataHelper(helper);
    VCardGroupData groupData;
    EXPECT_EQ(groupData.BuildData(mock), TELEPHONY_SUCCESS);
    EXPECT_EQ(groupData.GetGroupId(), 5);
    EXPECT_EQ(groupData.GetGroupName(), "Friends");
    VCardRdbHelper::SetDataHelper(nullptr);
}

HWTEST_F(VCardCoverageTest, VCardRdbHelper_QueryRawContactMaxId_Fail, Function | MediumTest | Level1)
{
    auto helper = std::make_shared<MockDataShareHelper>();
    EXPECT_CALL(*helper, Insert(_, _)).WillRepeatedly(Return(-1));
    VCardRdbHelper::SetDataHelper(helper);
    EXPECT_EQ(VCardRdbHelper::QueryRawContactMaxId(1), DB_FAILD);
    VCardRdbHelper::SetDataHelper(nullptr);
}

HWTEST_F(VCardCoverageTest, VCardRdbHelper_SuccessPaths, Function | MediumTest | Level1)
{
    auto helper = std::make_shared<MockDataShareHelper>();
    auto resultSet = std::make_shared<DataShareResultSetMock>();
    std::vector<DataShare::DataShareValuesBucket> buckets;
    DataShare::DataShareValuesBucket bucket;
    std::vector<std::string> columns;
    DataShare::DataSharePredicates predicates;
    EXPECT_CALL(*helper, Insert(_, _)).WillRepeatedly(Return(5));
    EXPECT_CALL(*helper, BatchInsert(_, _)).WillRepeatedly(Return(3));
    EXPECT_CALL(*helper, Query(_, _, _, _)).WillRepeatedly(Return(
        std::static_pointer_cast<DataShare::DataShareResultSet>(resultSet)));
    VCardRdbHelper::SetDataHelper(helper);
    EXPECT_EQ(VCardRdbHelper::QueryRawContactMaxId(1), 5);
    EXPECT_EQ(VCardRdbHelper::BatchInsertRawContact(buckets), 3);
    EXPECT_EQ(VCardRdbHelper::BatchInsertContactData(buckets), 3);
    EXPECT_EQ(VCardRdbHelper::InsertRawContact(bucket), 5);
    EXPECT_EQ(VCardRdbHelper::InsertContactData(buckets), 3);
    EXPECT_EQ(VCardRdbHelper::InsertGroupData(bucket), 5);
    EXPECT_NE(VCardRdbHelper::QueryGroupData(columns, predicates), nullptr);
    EXPECT_NE(VCardRdbHelper::QueryAccount(columns, predicates), nullptr);
    EXPECT_NE(VCardRdbHelper::VCardRdbHelper::GetInstance().QueryContact(columns, predicates), nullptr);
    EXPECT_NE(VCardRdbHelper::QueryRawContact(columns, predicates), nullptr);
    EXPECT_NE(VCardRdbHelper::QueryContactData(columns, predicates), nullptr);
    VCardRdbHelper::SetDataHelper(nullptr);
}

HWTEST_F(VCardCoverageTest, VCardRdbHelper_QueryGroupId_ResultNull, Function | MediumTest | Level1)
{
    auto helper = std::make_shared<MockDataShareHelper>();
    EXPECT_CALL(*helper, Query(_, _, _, _)).WillRepeatedly(Return(nullptr));
    VCardRdbHelper::SetDataHelper(helper);
    EXPECT_EQ(VCardRdbHelper::QueryGroupId("group"), DB_FAILD);
    VCardRdbHelper::SetDataHelper(nullptr);
}

HWTEST_F(VCardCoverageTest, VCardRdbHelper_QueryGroupId_Success, Function | MediumTest | Level1)
{
    auto helper = std::make_shared<MockDataShareHelper>();
    auto resultSet = std::make_shared<DataShareResultSetMock>();
    EXPECT_CALL(*resultSet, GoToFirstRow()).WillRepeatedly(Return(TELEPHONY_ERR_SUCCESS));
    EXPECT_CALL(*resultSet, GetColumnIndex(Group::GROUP_ID, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(0), Return(DataShare::E_OK)));
    EXPECT_CALL(*resultSet, GetInt(0, _)).WillRepeatedly(DoAll(SetArgReferee<1>(7), Return(DataShare::E_OK)));
    EXPECT_CALL(*resultSet, Close()).WillRepeatedly(Return(DataShare::E_OK));
    EXPECT_CALL(*helper, Query(_, _, _, _)).WillRepeatedly(Return(
        std::static_pointer_cast<DataShare::DataShareResultSet>(resultSet)));
    VCardRdbHelper::SetDataHelper(helper);
    EXPECT_EQ(VCardRdbHelper::QueryGroupId("group"), 7);
    VCardRdbHelper::SetDataHelper(nullptr);
}

HWTEST_F(VCardCoverageTest, VCardRdbHelper_QueryGroupId_ColumnIndexFail, Function | MediumTest | Level1)
{
    auto helper = std::make_shared<MockDataShareHelper>();
    auto resultSet = std::make_shared<DataShareResultSetMock>();
    EXPECT_CALL(*resultSet, GoToFirstRow()).WillRepeatedly(Return(TELEPHONY_ERR_SUCCESS));
    EXPECT_CALL(*resultSet, GetColumnIndex(Group::GROUP_ID, _))
        .WillRepeatedly(Return(DataShare::E_ERROR));
    EXPECT_CALL(*resultSet, Close()).WillRepeatedly(Return(DataShare::E_OK));
    EXPECT_CALL(*helper, Query(_, _, _, _)).WillRepeatedly(Return(
        std::static_pointer_cast<DataShare::DataShareResultSet>(resultSet)));
    VCardRdbHelper::SetDataHelper(helper);
    EXPECT_EQ(VCardRdbHelper::QueryGroupId("group"), 0);
    VCardRdbHelper::SetDataHelper(nullptr);
}

HWTEST_F(VCardCoverageTest, VCardRdbHelper_QueryGroupId_InsertNewGroup, Function | MediumTest | Level1)
{
    auto helper = std::make_shared<MockDataShareHelper>();
    auto resultSet = std::make_shared<DataShareResultSetMock>();
    EXPECT_CALL(*resultSet, GoToFirstRow()).WillRepeatedly(Return(DataShare::E_ERROR));
    EXPECT_CALL(*resultSet, Close()).WillRepeatedly(Return(DataShare::E_OK));
    EXPECT_CALL(*helper, Query(_, _, _, _)).WillRepeatedly(Return(
        std::static_pointer_cast<DataShare::DataShareResultSet>(resultSet)));
    EXPECT_CALL(*helper, Insert(_, _)).WillRepeatedly(Return(9));
    VCardRdbHelper::SetDataHelper(helper);
    EXPECT_EQ(VCardRdbHelper::QueryGroupId("newGroup"), 9);
    VCardRdbHelper::SetDataHelper(nullptr);
}
} // namespace Telephony
} // namespace OHOS