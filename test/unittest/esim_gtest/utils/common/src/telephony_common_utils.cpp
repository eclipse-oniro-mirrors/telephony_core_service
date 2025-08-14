/*
 * Copyright (C) 2022 Huawei Device Co., Ltd.
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

#include "telephony_common_utils.h"

#include <charconv>
#include <cstdint>
#include <regex>
#include <string>

#include "ipc_skeleton.h"
#include "telephony_log_wrapper.h"
#include "telephony_permission.h"

namespace OHOS {
namespace Telephony {
constexpr uint32_t INPUT_VALUE_LENGTH = 10;

template<typename T>
bool ConvertStrToNum(const std::string &str, T &value, uint8_t base)
{
    auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), value, base);
    return ec == std::errc{} && ptr == str.data() + str.size();
}

bool ConvertStrToInt(const std::string &str, int &value, uint8_t base)
{
    return ConvertStrToNum(str, value, base);
}

bool ConvertStrToLong(const std::string &str, int64_t &value, uint8_t base)
{
    return ConvertStrToNum(str, value, base);
}

std::string GetBundleName()
{
    int32_t uid = IPCSkeleton::GetCallingUid();
    std::string bundleName = "";
    TelephonyPermission::GetBundleNameByUid(uid, bundleName);
    if (bundleName.empty()) {
        bundleName.append(std::to_string(uid));
        bundleName.append(std::to_string(IPCSkeleton::GetCallingPid()));
    }
    return bundleName;
}

bool IsValidDecValue(const std::string &inputValue)
{
    if (inputValue.length() > INPUT_VALUE_LENGTH) {
        TELEPHONY_LOGE("The value entered is out of range, value:%{public}s", inputValue.c_str());
        return false;
    }
    bool isValueNumber = regex_match(inputValue, std::regex("(-[\\d+]+)|(\\d+)"));
    if (isValueNumber) {
        int64_t numberValue = INT64_MAX;
        bool isSuccess = ConvertStrToLong(inputValue, numberValue, DEC_TYPE);
        if (isSuccess && (numberValue >= INT32_MIN) && (numberValue <= INT32_MAX)) {
            return true;
        }
    }
    TELEPHONY_LOGI("InputValue is not a decimal number");
    return false;
}

bool IsValidHexValue(const std::string &inputValue)
{
    if (inputValue.length() > INPUT_VALUE_LENGTH) {
        TELEPHONY_LOGE("The value entered is out of range, value:%{public}s", inputValue.c_str());
        return false;
    }
    bool isValueNumber = regex_match(inputValue, std::regex("(0[xX][0-9a-fA-F]+)|([0-9a-fA-F]+)"));
    if (isValueNumber) {
        int64_t numberValue = INT64_MAX;
        bool isSuccess = ConvertStrToLong(inputValue, numberValue, HEX_TYPE);
        if (isSuccess && (numberValue >= INT32_MIN) && (numberValue <= INT32_MAX)) {
            return true;
        }
    }
    TELEPHONY_LOGI("InputValue is not a hexadecimal number");
    return false;
}

int32_t GetTokenID()
{
    return static_cast<int32_t>(IPCSkeleton::GetCallingTokenID());
}
} // namespace Telephony
} // namespace OHOS