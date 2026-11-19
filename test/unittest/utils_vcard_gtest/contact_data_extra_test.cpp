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
#include "vcard_anniversary_data.h"
#include "vcard_birthday_data.h"
#include "vcard_email_data.h"
#include "vcard_event_data.h"
#include "vcard_group_data.h"
#include "vcard_im_data.h"
#include "vcard_name_data.h"
#include "vcard_note_data.h"
#include "vcard_organization_data.h"
#include "vcard_phone_data.h"
#include "vcard_photo_data.h"
#include "vcard_postal_data.h"
#include "vcard_rdb_helper.h"
#include "vcard_relation_data.h"
#include "vcard_sip_data.h"
#include "vcard_uid_data.h"
#include "vcard_website_data.h"

namespace OHOS {
namespace Telephony {
using namespace testing::ext;
using namespace testing;

namespace {
constexpr int32_t INDEX_ZERO = 0;
constexpr int32_t INDEX_ONE = 1;
constexpr int32_t INDEX_TWO = 2;
constexpr int32_t INDEX_THREE = 3;
constexpr int32_t INDEX_FOUR = 4;
constexpr int32_t INDEX_FIVE = 5;
constexpr int32_t INDEX_SIX = 6;
constexpr int32_t INDEX_SEVEN = 7;
constexpr int32_t INDEX_EIGHT = 8;
constexpr size_t MAX_UID_LENGTH = 500;
constexpr size_t MAX_ADDRESS_LENGTH = 300;
constexpr int32_t POSTAL_VALUE_COUNT = 8;
} // namespace

class ContactDataExtraTest : public testing::Test {
public:
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
};

void ContactDataExtraTest::SetUpTestCase() {}

void ContactDataExtraTest::TearDownTestCase() {}

void ContactDataExtraTest::SetUp() {}

void ContactDataExtraTest::TearDown()
{
    VCardRdbHelper::SetDataHelper(nullptr);
}

HWTEST_F(ContactDataExtraTest, ExtraEmailData_BuildData_Success, Function | MediumTest | Level1)
{
    auto mock = std::make_shared<DataShareResultSetMock>();
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::DETAIL_INFO, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(INDEX_ZERO), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::LABEL_NAME, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(INDEX_ONE), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::LABEL_ID, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(INDEX_TWO), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::ALIAS_DETAIL_INFO_KEY, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(INDEX_THREE), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetString(INDEX_ZERO, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>("address"), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetString(INDEX_ONE, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>("labelName"), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetString(INDEX_TWO, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>("labelId"), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetString(INDEX_THREE, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>("display"), Return(DataShare::E_OK)));
    VCardEmailData emailData;
    EXPECT_EQ(emailData.BuildData(mock), TELEPHONY_SUCCESS);
    EXPECT_EQ(emailData.GetAddress(), "address");
    EXPECT_EQ(emailData.GetLabelName(), "labelName");
    EXPECT_EQ(emailData.GetLabelId(), "labelId");
    EXPECT_EQ(emailData.GetDisplayName(), "display");
}

HWTEST_F(ContactDataExtraTest, ExtraEmailData_BuildData_Null, Function | MediumTest | Level1)
{
    VCardEmailData emailData;
    EXPECT_EQ(emailData.BuildData(nullptr), TELEPHONY_ERROR);
}

HWTEST_F(ContactDataExtraTest, ExtraEmailData_Setters, Function | MediumTest | Level1)
{
    VCardEmailData emailData;
    emailData.SetAddress("address");
    emailData.SetLabelId("labelId");
    emailData.SetLabelName("labelName");
    emailData.SetDisplayName("display");
    EXPECT_EQ(emailData.GetAddress(), "address");
    EXPECT_EQ(emailData.GetLabelId(), "labelId");
    EXPECT_EQ(emailData.GetLabelName(), "labelName");
    EXPECT_EQ(emailData.GetDisplayName(), "display");
}

HWTEST_F(ContactDataExtraTest, ExtraEmailData_BuildValuesBucket, Function | MediumTest | Level1)
{
    VCardEmailData emailData;
    emailData.SetAddress("address");
    emailData.SetLabelId("labelId");
    emailData.SetLabelName("labelName");
    emailData.SetDisplayName("display");
    DataShare::DataShareValuesBucket valuesBucket;
    EXPECT_EQ(emailData.BuildValuesBucket(valuesBucket), TELEPHONY_SUCCESS);
}

HWTEST_F(ContactDataExtraTest, ExtraImData_BuildData_Success, Function | MediumTest | Level1)
{
    auto mock = std::make_shared<DataShareResultSetMock>();
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::DETAIL_INFO, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(INDEX_ZERO), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::LABEL_ID, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(INDEX_ONE), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetString(INDEX_ZERO, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>("address"), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetString(INDEX_ONE, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>("labelId"), Return(DataShare::E_OK)));
    VCardImData imData;
    EXPECT_EQ(imData.BuildData(mock), TELEPHONY_SUCCESS);
    EXPECT_EQ(imData.GetAddress(), "address");
    EXPECT_EQ(imData.GetLabelId(), "labelId");
}

HWTEST_F(ContactDataExtraTest, ExtraImData_BuildData_Null, Function | MediumTest | Level1)
{
    VCardImData imData;
    EXPECT_EQ(imData.BuildData(nullptr), TELEPHONY_ERROR);
}

HWTEST_F(ContactDataExtraTest, ExtraImData_BuildData_Fail, Function | MediumTest | Level1)
{
    auto mock = std::make_shared<DataShareResultSetMock>();
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::DETAIL_INFO, _))
        .WillRepeatedly(Return(DataShare::E_ERROR));
    VCardImData imData;
    EXPECT_EQ(imData.BuildData(mock), TELEPHONY_ERROR);
}

HWTEST_F(ContactDataExtraTest, ExtraImData_BuildValuesBucket, Function | MediumTest | Level1)
{
    VCardImData imData;
    imData.SetAddress("address");
    imData.SetLabelId("labelId");
    DataShare::DataShareValuesBucket valuesBucket;
    EXPECT_EQ(imData.BuildValuesBucket(valuesBucket), TELEPHONY_SUCCESS);
}

HWTEST_F(ContactDataExtraTest, ExtraEventData_BuildData_Success, Function | MediumTest | Level1)
{
    auto mock = std::make_shared<DataShareResultSetMock>();
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::DETAIL_INFO, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(INDEX_ZERO), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::LABEL_NAME, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(INDEX_ONE), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::LABEL_ID, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(INDEX_TWO), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetString(INDEX_ZERO, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>("2024-01-01"), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetString(INDEX_ONE, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>("labelName"), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetString(INDEX_TWO, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>("labelId"), Return(DataShare::E_OK)));
    VCardEventData eventData;
    EXPECT_EQ(eventData.BuildData(mock), TELEPHONY_SUCCESS);
    EXPECT_EQ(eventData.GetEventDate(), "2024-01-01");
    EXPECT_EQ(eventData.GetLabelName(), "labelName");
    EXPECT_EQ(eventData.GetLabelId(), "labelId");
}

HWTEST_F(ContactDataExtraTest, ExtraEventData_BuildData_Null, Function | MediumTest | Level1)
{
    VCardEventData eventData;
    EXPECT_EQ(eventData.BuildData(nullptr), TELEPHONY_ERROR);
}

HWTEST_F(ContactDataExtraTest, ExtraEventData_BuildData_Fail, Function | MediumTest | Level1)
{
    auto mock = std::make_shared<DataShareResultSetMock>();
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::DETAIL_INFO, _))
        .WillRepeatedly(Return(DataShare::E_ERROR));
    VCardEventData eventData;
    EXPECT_EQ(eventData.BuildData(mock), TELEPHONY_ERROR);
}

HWTEST_F(ContactDataExtraTest, ExtraGroupData_Setters, Function | MediumTest | Level1)
{
    VCardGroupData groupData;
    groupData.SetGroupName("Friends");
    groupData.SetGroupId(7);
    EXPECT_EQ(groupData.GetGroupName(), "Friends");
    EXPECT_EQ(groupData.GetGroupId(), 7);
}

HWTEST_F(ContactDataExtraTest, ExtraGroupData_BuildData_Success, Function | MediumTest | Level1)
{
    auto mock = std::make_shared<DataShareResultSetMock>();
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::DETAIL_INFO, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(INDEX_ZERO), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetString(INDEX_ZERO, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>("6"), Return(DataShare::E_OK)));
    auto nameResultSet = std::make_shared<DataShareResultSetMock>();
    EXPECT_CALL(*nameResultSet, GoToFirstRow()).WillRepeatedly(Return(TELEPHONY_ERR_SUCCESS));
    EXPECT_CALL(*nameResultSet, GetColumnIndex(Group::GROUP_NAME, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(INDEX_ZERO), Return(DataShare::E_OK)));
    EXPECT_CALL(*nameResultSet, GetString(INDEX_ZERO, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>("Family"), Return(DataShare::E_OK)));
    EXPECT_CALL(*nameResultSet, Close()).WillRepeatedly(Return(DataShare::E_OK));
    auto helper = std::make_shared<MockDataShareHelper>();
    EXPECT_CALL(*helper, Query(_, _, _, _)).WillRepeatedly(Return(
        std::static_pointer_cast<DataShare::DataShareResultSet>(nameResultSet)));
    VCardRdbHelper::SetDataHelper(helper);
    VCardGroupData groupData;
    EXPECT_EQ(groupData.BuildData(mock), TELEPHONY_SUCCESS);
    EXPECT_EQ(groupData.GetGroupId(), 6);
    EXPECT_EQ(groupData.GetGroupName(), "Family");
    VCardRdbHelper::SetDataHelper(nullptr);
}

HWTEST_F(ContactDataExtraTest, ExtraGroupData_BuildData_Null, Function | MediumTest | Level1)
{
    VCardGroupData groupData;
    EXPECT_EQ(groupData.BuildData(nullptr), TELEPHONY_ERROR);
}

HWTEST_F(ContactDataExtraTest, ExtraGroupData_BuildValuesBucket, Function | MediumTest | Level1)
{
    VCardGroupData groupData;
    groupData.SetGroupId(3);
    DataShare::DataShareValuesBucket valuesBucket;
    EXPECT_EQ(groupData.BuildValuesBucket(valuesBucket), TELEPHONY_SUCCESS);
}

HWTEST_F(ContactDataExtraTest, ExtraPostalData_BuildData_Success, Function | MediumTest | Level1)
{
    auto mock = std::make_shared<DataShareResultSetMock>();
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::POBOX, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(INDEX_ZERO), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::POSTCODE, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(INDEX_ONE), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::REGION, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(INDEX_TWO), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::STREET, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(INDEX_THREE), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::COUNTRY, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(INDEX_FOUR), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::CITY, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(INDEX_FIVE), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::LABEL_ID, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(INDEX_SIX), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::LABEL_NAME, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(INDEX_SEVEN), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::DETAIL_INFO, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(INDEX_EIGHT), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetString(INDEX_ZERO, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>("pobox"), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetString(INDEX_ONE, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>("postcode"), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetString(INDEX_TWO, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>("region"), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetString(INDEX_THREE, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>("street"), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetString(INDEX_FOUR, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>("country"), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetString(INDEX_FIVE, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>("city"), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetString(INDEX_SIX, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>("labelId"), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetString(INDEX_SEVEN, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>("labelName"), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetString(INDEX_EIGHT, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>("address"), Return(DataShare::E_OK)));
    VCardPostalData postalData;
    EXPECT_EQ(postalData.BuildData(mock), TELEPHONY_SUCCESS);
    EXPECT_EQ(postalData.GetPOBox(), "pobox");
    EXPECT_EQ(postalData.GetPostCode(), "postcode");
    EXPECT_EQ(postalData.GetRegion(), "region");
    EXPECT_EQ(postalData.GetStreet(), "street");
    EXPECT_EQ(postalData.GetCountry(), "country");
    EXPECT_EQ(postalData.GetCity(), "city");
    EXPECT_EQ(postalData.GetLabelId(), "labelId");
    EXPECT_EQ(postalData.GetLabelName(), "labelName");
    EXPECT_EQ(postalData.GetPostalAddress(), "address");
}

HWTEST_F(ContactDataExtraTest, ExtraPostalData_BuildData_AllColumnsFail, Function | MediumTest | Level1)
{
    auto mock = std::make_shared<DataShareResultSetMock>();
    EXPECT_CALL(*mock, GetColumnIndex(_, _)).WillRepeatedly(Return(DataShare::E_ERROR));
    VCardPostalData postalData;
    EXPECT_EQ(postalData.BuildData(mock), TELEPHONY_SUCCESS);
}

HWTEST_F(ContactDataExtraTest, ExtraPostalData_BuildData_Null, Function | MediumTest | Level1)
{
    VCardPostalData postalData;
    EXPECT_EQ(postalData.BuildData(nullptr), TELEPHONY_ERROR);
}

HWTEST_F(ContactDataExtraTest, ExtraPostalData_InitPostalData_TooManyElements, Function | MediumTest | Level1)
{
    std::vector<std::string> propValueList(POSTAL_VALUE_COUNT, "value");
    VCardPostalData postalData;
    postalData.InitPostalData(propValueList, 1, "label");
    EXPECT_EQ(postalData.GetLabelName(), "label");
    EXPECT_EQ(postalData.GetLabelId(), "1");
    EXPECT_FALSE(postalData.GetPostalAddress().empty());
}

HWTEST_F(ContactDataExtraTest, ExtraPostalData_InitPostalData_LongElement, Function | MediumTest | Level1)
{
    std::vector<std::string> propValueList;
    propValueList.emplace_back(std::string(MAX_ADDRESS_LENGTH + 1, 'a'));
    VCardPostalData postalData;
    postalData.InitPostalData(propValueList, 2, "label");
    EXPECT_TRUE(postalData.GetPostalAddress().empty());
    EXPECT_EQ(postalData.GetLabelName(), "label");
}

HWTEST_F(ContactDataExtraTest, ExtraPostalData_Setters, Function | MediumTest | Level1)
{
    VCardPostalData postalData;
    postalData.SetNeighborhood("neighborhood");
    postalData.SetLabelId("labelId");
    postalData.SetLabelName("labelName");
    postalData.SetPostalAddress("address");
    EXPECT_EQ(postalData.GetNeighborhood(), "neighborhood");
    EXPECT_EQ(postalData.GetLabelId(), "labelId");
    EXPECT_EQ(postalData.GetLabelName(), "labelName");
    EXPECT_EQ(postalData.GetPostalAddress(), "address");
}

HWTEST_F(ContactDataExtraTest, ExtraPostalData_BuildValuesBucket, Function | MediumTest | Level1)
{
    VCardPostalData postalData;
    postalData.SetPOBox("pobox");
    postalData.SetPostCode("postcode");
    postalData.SetRegion("region");
    postalData.SetStreet("street");
    postalData.SetCountry("country");
    postalData.SetCity("city");
    postalData.SetLabelId("labelId");
    postalData.SetLabelName("labelName");
    postalData.SetPostalAddress("address");
    DataShare::DataShareValuesBucket valuesBucket;
    EXPECT_EQ(postalData.BuildValuesBucket(valuesBucket), TELEPHONY_SUCCESS);
}

HWTEST_F(ContactDataExtraTest, ExtraPhotoData_BuildData_Success, Function | MediumTest | Level1)
{
    auto mock = std::make_shared<DataShareResultSetMock>();
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::BLOB_DATA, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(INDEX_ZERO), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetBlob(INDEX_ZERO, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(std::vector<uint8_t>{'a', 'b', 'c'}), Return(DataShare::E_OK)));
    VCardPhotoData photoData;
    EXPECT_EQ(photoData.BuildData(mock), TELEPHONY_SUCCESS);
    EXPECT_EQ(photoData.GetBytes(), "abc");
}

HWTEST_F(ContactDataExtraTest, ExtraPhotoData_BuildData_Null, Function | MediumTest | Level1)
{
    VCardPhotoData photoData;
    EXPECT_EQ(photoData.BuildData(nullptr), TELEPHONY_ERROR);
}

HWTEST_F(ContactDataExtraTest, ExtraPhotoData_BuildData_ColumnFail, Function | MediumTest | Level1)
{
    auto mock = std::make_shared<DataShareResultSetMock>();
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::BLOB_DATA, _))
        .WillRepeatedly(Return(DataShare::E_ERROR));
    VCardPhotoData photoData;
    EXPECT_EQ(photoData.BuildData(mock), TELEPHONY_SUCCESS);
    EXPECT_TRUE(photoData.GetBytes().empty());
}

HWTEST_F(ContactDataExtraTest, ExtraPhotoData_Setters, Function | MediumTest | Level1)
{
    VCardPhotoData photoData;
    std::string format = "JPEG";
    std::string hexBytes = "0x1234";
    photoData.InitPhotoData(format, hexBytes);
    EXPECT_EQ(photoData.GetFormat(), "JPEG");
    EXPECT_EQ(photoData.GetBytes(), "0x1234");

    std::string newFormat = "PNG";
    std::string newHexBytes = "0x5678";
    photoData.SetFormat(newFormat);
    photoData.SetHexBytes(newHexBytes);
    EXPECT_EQ(photoData.GetFormat(), "PNG");
    EXPECT_EQ(photoData.GetHexBytes(), "0x5678");

    photoData.SetBytes("bytes");
    EXPECT_EQ(photoData.GetBytes(), "bytes");
}

HWTEST_F(ContactDataExtraTest, ExtraPhotoData_BuildValuesBucket, Function | MediumTest | Level1)
{
    VCardPhotoData photoData;
    photoData.SetBytes("bytes");
    DataShare::DataShareValuesBucket valuesBucket;
    EXPECT_EQ(photoData.BuildValuesBucket(valuesBucket), TELEPHONY_SUCCESS);
}

HWTEST_F(ContactDataExtraTest, ExtraSipData_BuildData_Success, Function | MediumTest | Level1)
{
    auto mock = std::make_shared<DataShareResultSetMock>();
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::DETAIL_INFO, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(INDEX_ZERO), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::LABEL_NAME, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(INDEX_ONE), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::LABEL_ID, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(INDEX_TWO), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetString(INDEX_ZERO, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>("address"), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetString(INDEX_ONE, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>("labelName"), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetString(INDEX_TWO, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>("labelId"), Return(DataShare::E_OK)));
    VCardSipData sipData;
    EXPECT_EQ(sipData.BuildData(mock), TELEPHONY_SUCCESS);
    EXPECT_EQ(sipData.GetAddress(), "address");
    EXPECT_EQ(sipData.GetLabelName(), "labelName");
    EXPECT_EQ(sipData.GetLabelId(), "labelId");
}

HWTEST_F(ContactDataExtraTest, ExtraSipData_BuildData_Null, Function | MediumTest | Level1)
{
    VCardSipData sipData;
    EXPECT_EQ(sipData.BuildData(nullptr), TELEPHONY_ERROR);
}

HWTEST_F(ContactDataExtraTest, ExtraSipData_Type, Function | MediumTest | Level1)
{
    VCardSipData sipData;
    sipData.SetType(3);
    EXPECT_EQ(sipData.GetType(), 3);
}

HWTEST_F(ContactDataExtraTest, ExtraSipData_InitSipData, Function | MediumTest | Level1)
{
    VCardSipData sipData;
    sipData.InitSipData("sip:test@test.com", 1, "label");
    EXPECT_EQ(sipData.GetAddress(), "test@test.com");
    EXPECT_EQ(sipData.GetType(), 1);
    EXPECT_EQ(sipData.GetLabelName(), "label");

    sipData.InitSipData("test@test.com", 2, "label2");
    EXPECT_EQ(sipData.GetAddress(), "test@test.com");
    EXPECT_EQ(sipData.GetType(), 2);
}

HWTEST_F(ContactDataExtraTest, ExtraUidData_BuildValuesBucket, Function | MediumTest | Level1)
{
    VCardUidData uidData;
    uidData.SetUid("uid");
    DataShare::DataShareValuesBucket valuesBucket;
    EXPECT_EQ(uidData.BuildValuesBucket(valuesBucket), TELEPHONY_SUCCESS);
    EXPECT_EQ(uidData.GetUid(), "uid");
}

HWTEST_F(ContactDataExtraTest, ExtraUidData_SetUidTooLong, Function | MediumTest | Level1)
{
    VCardUidData uidData;
    uidData.SetUid("uid");
    uidData.SetUid(std::string(MAX_UID_LENGTH + 1, 'a'));
    EXPECT_EQ(uidData.GetUid(), "uid");
}

HWTEST_F(ContactDataExtraTest, ExtraUidData_BuildData, Function | MediumTest | Level1)
{
    VCardUidData uidData;
    EXPECT_EQ(uidData.BuildData(nullptr), TELEPHONY_SUCCESS);
}

HWTEST_F(ContactDataExtraTest, ExtraAnniversaryData_BuildData, Function | MediumTest | Level1)
{
    VCardAnniversaryData anniversaryData;
    EXPECT_EQ(anniversaryData.BuildData(nullptr), TELEPHONY_SUCCESS);
    anniversaryData.InitAnniversaryData("2024-01-01");
    EXPECT_EQ(anniversaryData.GetAnniversary(), "2024-01-01");
    DataShare::DataShareValuesBucket valuesBucket;
    EXPECT_EQ(anniversaryData.BuildValuesBucket(valuesBucket), TELEPHONY_SUCCESS);
}

HWTEST_F(ContactDataExtraTest, ExtraBirthdayData_BuildData, Function | MediumTest | Level1)
{
    VCardBirthdayData birthdayData;
    EXPECT_EQ(birthdayData.BuildData(nullptr), TELEPHONY_SUCCESS);
    std::string birthday = "2024-01-01";
    birthdayData.InitBirthdayData(birthday);
    EXPECT_EQ(birthdayData.GetBirthday(), "2024-01-01");
    DataShare::DataShareValuesBucket valuesBucket;
    EXPECT_EQ(birthdayData.BuildValuesBucket(valuesBucket), TELEPHONY_SUCCESS);
}

HWTEST_F(ContactDataExtraTest, ExtraNameData_PhoneticName, Function | MediumTest | Level1)
{
    VCardNameData nameData;
    nameData.SetPhoneticName("phonetic");
    EXPECT_EQ(nameData.GetPhoneticName(), "phonetic");
}

HWTEST_F(ContactDataExtraTest, ExtraNameData_BuildValuesBucket, Function | MediumTest | Level1)
{
    VCardNameData nameData;
    nameData.SetGiven("given");
    nameData.SetFamily("family");
    nameData.SetMiddle("middle");
    nameData.SetPrefix("prefix");
    nameData.SetSuffix("suffix");
    nameData.setSort("sort");
    nameData.setDispalyName("display");
    DataShare::DataShareValuesBucket valuesBucket;
    EXPECT_EQ(nameData.BuildValuesBucket(valuesBucket), TELEPHONY_SUCCESS);
}

HWTEST_F(ContactDataExtraTest, ExtraNoteData_BuildData_Success, Function | MediumTest | Level1)
{
    auto mock = std::make_shared<DataShareResultSetMock>();
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::DETAIL_INFO, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(INDEX_ZERO), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetString(INDEX_ZERO, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>("note"), Return(DataShare::E_OK)));
    VCardNoteData noteData;
    EXPECT_EQ(noteData.BuildData(mock), TELEPHONY_SUCCESS);
    EXPECT_EQ(noteData.GetNote(), "note");
}

HWTEST_F(ContactDataExtraTest, ExtraNoteData_BuildData_Null, Function | MediumTest | Level1)
{
    VCardNoteData noteData;
    EXPECT_EQ(noteData.BuildData(nullptr), TELEPHONY_ERROR);
}

HWTEST_F(ContactDataExtraTest, ExtraNoteData_SetNote, Function | MediumTest | Level1)
{
    VCardNoteData noteData;
    noteData.SetNote("note");
    EXPECT_EQ(noteData.GetNote(), "note");
    DataShare::DataShareValuesBucket valuesBucket;
    EXPECT_EQ(noteData.BuildValuesBucket(valuesBucket), TELEPHONY_SUCCESS);
}

HWTEST_F(ContactDataExtraTest, ExtraOrganizationData_BuildData_Success, Function | MediumTest | Level1)
{
    auto mock = std::make_shared<DataShareResultSetMock>();
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::DETAIL_INFO, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(INDEX_ZERO), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::POSITION, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(INDEX_ONE), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetString(INDEX_ZERO, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>("company"), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetString(INDEX_ONE, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>("title"), Return(DataShare::E_OK)));
    VCardOrganizationData organizationData;
    EXPECT_EQ(organizationData.BuildData(mock), TELEPHONY_SUCCESS);
    EXPECT_EQ(organizationData.GetCompany(), "company");
    EXPECT_EQ(organizationData.GetTitle(), "title");
}

HWTEST_F(ContactDataExtraTest, ExtraOrganizationData_BuildData_Null, Function | MediumTest | Level1)
{
    VCardOrganizationData organizationData;
    EXPECT_EQ(organizationData.BuildData(nullptr), TELEPHONY_ERROR);
}

HWTEST_F(ContactDataExtraTest, ExtraOrganizationData_BuildData_ColumnFail, Function | MediumTest | Level1)
{
    auto mock = std::make_shared<DataShareResultSetMock>();
    EXPECT_CALL(*mock, GetColumnIndex(_, _)).WillRepeatedly(Return(DataShare::E_ERROR));
    VCardOrganizationData organizationData;
    EXPECT_EQ(organizationData.BuildData(mock), TELEPHONY_SUCCESS);
}

HWTEST_F(ContactDataExtraTest, ExtraOrganizationData_Setters, Function | MediumTest | Level1)
{
    VCardOrganizationData organizationData;
    organizationData.SetCompany("company");
    organizationData.SetType(2);
    organizationData.SetPhoneticName("phonetic");
    EXPECT_EQ(organizationData.GetCompany(), "company");
    EXPECT_EQ(organizationData.GetType(), 2);
    EXPECT_EQ(organizationData.GetPhoneticName(), "phonetic");
}

HWTEST_F(ContactDataExtraTest, ExtraOrganizationData_BuildValuesBucket, Function | MediumTest | Level1)
{
    VCardOrganizationData organizationData;
    DataShare::DataShareValuesBucket emptyBucket;
    EXPECT_EQ(organizationData.BuildValuesBucket(emptyBucket), TELEPHONY_SUCCESS);

    organizationData.SetCompany("company");
    organizationData.SetTitle("title");
    DataShare::DataShareValuesBucket valuesBucket;
    EXPECT_EQ(organizationData.BuildValuesBucket(valuesBucket), TELEPHONY_SUCCESS);
}

HWTEST_F(ContactDataExtraTest, ExtraPhoneData_BuildData_Success, Function | MediumTest | Level1)
{
    auto mock = std::make_shared<DataShareResultSetMock>();
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::DETAIL_INFO, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(INDEX_ZERO), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::LABEL_NAME, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(INDEX_ONE), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::LABEL_ID, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(INDEX_TWO), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetString(INDEX_ZERO, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>("number"), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetString(INDEX_ONE, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>("labelName"), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetString(INDEX_TWO, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>("labelId"), Return(DataShare::E_OK)));
    VCardPhoneData phoneData;
    EXPECT_EQ(phoneData.BuildData(mock), TELEPHONY_SUCCESS);
    EXPECT_EQ(phoneData.GetNumber(), "number");
    EXPECT_EQ(phoneData.GetLabelName(), "labelName");
    EXPECT_EQ(phoneData.GetLabelId(), "labelId");
}

HWTEST_F(ContactDataExtraTest, ExtraPhoneData_BuildData_Null, Function | MediumTest | Level1)
{
    VCardPhoneData phoneData;
    EXPECT_EQ(phoneData.BuildData(nullptr), TELEPHONY_ERROR);
}

HWTEST_F(ContactDataExtraTest, ExtraPhoneData_BuildData_ColumnFail, Function | MediumTest | Level1)
{
    auto mock = std::make_shared<DataShareResultSetMock>();
    EXPECT_CALL(*mock, GetColumnIndex(_, _)).WillRepeatedly(Return(DataShare::E_ERROR));
    VCardPhoneData phoneData;
    EXPECT_EQ(phoneData.BuildData(mock), TELEPHONY_SUCCESS);
}

HWTEST_F(ContactDataExtraTest, ExtraPhoneData_Setters, Function | MediumTest | Level1)
{
    VCardPhoneData phoneData;
    phoneData.SetNumber("number");
    phoneData.SetLabelId("labelId");
    phoneData.SetLabelName("labelName");
    phoneData.SetType(1);
    EXPECT_EQ(phoneData.GetNumber(), "number");
    EXPECT_EQ(phoneData.GetLabelId(), "labelId");
    EXPECT_EQ(phoneData.GetLabelName(), "labelName");
    EXPECT_EQ(phoneData.GetType(), 1);

    DataShare::DataShareValuesBucket valuesBucket;
    EXPECT_EQ(phoneData.BuildValuesBucket(valuesBucket), TELEPHONY_SUCCESS);
}

HWTEST_F(ContactDataExtraTest, ExtraRelationData_BuildValuesBucket, Function | MediumTest | Level1)
{
    VCardRelationData relationData;
    relationData.SetRelationName("name");
    relationData.SetLabelId("labelId");
    relationData.SetLabelName("labelName");
    DataShare::DataShareValuesBucket valuesBucket;
    EXPECT_EQ(relationData.BuildValuesBucket(valuesBucket), TELEPHONY_SUCCESS);
    EXPECT_EQ(relationData.GetRelationName(), "name");
    EXPECT_EQ(relationData.GetLabelId(), "labelId");
    EXPECT_EQ(relationData.GetLabelName(), "labelName");
}

HWTEST_F(ContactDataExtraTest, ExtraRelationData_BuildData_Success, Function | MediumTest | Level1)
{
    auto mock = std::make_shared<DataShareResultSetMock>();
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::DETAIL_INFO, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(INDEX_ZERO), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::LABEL_NAME, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(INDEX_ONE), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::LABEL_ID, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(INDEX_TWO), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetString(INDEX_ZERO, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>("name"), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetString(INDEX_ONE, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>("labelName"), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetString(INDEX_TWO, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>("labelId"), Return(DataShare::E_OK)));
    VCardRelationData relationData;
    EXPECT_EQ(relationData.BuildData(mock), TELEPHONY_SUCCESS);
    EXPECT_EQ(relationData.GetRelationName(), "name");
    EXPECT_EQ(relationData.GetLabelName(), "labelName");
    EXPECT_EQ(relationData.GetLabelId(), "labelId");
}

HWTEST_F(ContactDataExtraTest, ExtraRelationData_BuildData_Null, Function | MediumTest | Level1)
{
    VCardRelationData relationData;
    EXPECT_EQ(relationData.BuildData(nullptr), TELEPHONY_ERROR);
}

HWTEST_F(ContactDataExtraTest, ExtraWebsiteData_BuildData_Success, Function | MediumTest | Level1)
{
    auto mock = std::make_shared<DataShareResultSetMock>();
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::DETAIL_INFO, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(INDEX_ZERO), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::LABEL_NAME, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(INDEX_ONE), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetColumnIndex(ContactData::LABEL_ID, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>(INDEX_TWO), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetString(INDEX_ZERO, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>("website"), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetString(INDEX_ONE, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>("labelName"), Return(DataShare::E_OK)));
    EXPECT_CALL(*mock, GetString(INDEX_TWO, _))
        .WillRepeatedly(DoAll(SetArgReferee<1>("labelId"), Return(DataShare::E_OK)));
    VCardWebsiteData websiteData;
    EXPECT_EQ(websiteData.BuildData(mock), TELEPHONY_SUCCESS);
    EXPECT_EQ(websiteData.GetWebsite(), "website");
    EXPECT_EQ(websiteData.GetLabelName(), "labelName");
    EXPECT_EQ(websiteData.GetLabelId(), "labelId");
}

HWTEST_F(ContactDataExtraTest, ExtraWebsiteData_BuildData_Null, Function | MediumTest | Level1)
{
    VCardWebsiteData websiteData;
    EXPECT_EQ(websiteData.BuildData(nullptr), TELEPHONY_ERROR);
}

HWTEST_F(ContactDataExtraTest, ExtraWebsiteData_Setters, Function | MediumTest | Level1)
{
    VCardWebsiteData websiteData;
    websiteData.SetWebsite("website");
    websiteData.SetLabelId("labelId");
    websiteData.SetLabelName("labelName");
    EXPECT_EQ(websiteData.GetWebsite(), "website");
    EXPECT_EQ(websiteData.GetLabelId(), "labelId");
    EXPECT_EQ(websiteData.GetLabelName(), "labelName");

    DataShare::DataShareValuesBucket valuesBucket;
    EXPECT_EQ(websiteData.BuildValuesBucket(valuesBucket), TELEPHONY_SUCCESS);
}
} // namespace Telephony
} // namespace OHOS