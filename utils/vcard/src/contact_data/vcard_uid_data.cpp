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
 
#include "vcard_uid_data.h"
#include "vcard_rdb_helper.h"
 
#include "telephony_errors.h"
 
namespace OHOS {
namespace Telephony {
 
int32_t VCardUidData::BuildValuesBucket(OHOS::DataShare::DataShareValuesBucket &valuesBucket)
{
    valuesBucket.Put(RawContact::UUID, uid_);
    return TELEPHONY_SUCCESS;
}
 
int32_t VCardUidData::BuildData(std::shared_ptr<DataShare::DataShareResultSet> resultSet)
{
    return 0;
}
 
void VCardUidData::SetUid(std::string uid)
{
    uid_ = uid;
}
 
std::string VCardUidData::GetUid()
{
    return uid_;
}
 
} // namespace Telephony
} // namespace OHOS