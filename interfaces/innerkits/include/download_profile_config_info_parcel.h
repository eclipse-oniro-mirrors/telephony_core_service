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

#ifndef OHOS_DOWNLOAD_PROFILE_CONFIG_INFO_PARCEL_H
#define OHOS_DOWNLOAD_PROFILE_CONFIG_INFO_PARCEL_H

#include <parcel.h>

namespace OHOS {
namespace Telephony {
struct DownloadProfileConfigInfo : public Parcelable {
    int32_t portIndex_ = 0;
    bool isSwitchAfterDownload_ = false;
    bool isForceDeactivateSim_ = false;
    bool isAlowPpr_ = false;

    bool ReadFromParcel(Parcel &parcel);
    virtual bool Marshalling(Parcel &parcel) const override;
    static DownloadProfileConfigInfo *Unmarshalling(Parcel &parcel);
};
} // namespace Telephony
} // namespace OHOS
#endif // OHOS_DOWNLOAD_PROFILE_CONFIG_INFO_PARCEL_H
