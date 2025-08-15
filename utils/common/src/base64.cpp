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

#include "base64.h"

#include "openssl/evp.h"

namespace OHOS {
namespace Telephony {
constexpr unsigned int MAX_BASE64_BUF_PADDING_SIZE = 2;
std::shared_ptr<std::string> Base64::Encode(const std::vector<unsigned char> &input)
{
    auto size = input.size();
    size_t bufLen = BASE64_ENCODED_UNIT * ((size + BASE64_INPUT_UNIT_PAD) / BASE64_INPUT_UNIT) +
        BASE64_OUTPUT_PADDING;
    std::vector<unsigned char> outBuffer(bufLen, 0);
    auto outLen = EVP_EncodeBlock(outBuffer.data(), input.data(), size);
    if (outLen < 0) {
        return nullptr;
    }
    outBuffer.resize(outLen);
    return std::make_shared<std::string>(outBuffer.begin(), outBuffer.end());
}

std::shared_ptr<std::string> Base64::Encode(const std::string &input)
{
    size_t inputSize = input.size();
    size_t encodeSize = BASE64_ENCODED_UNIT * ((inputSize + BASE64_INPUT_UNIT_PAD) / BASE64_INPUT_UNIT) +
        BASE64_OUTPUT_PADDING;
    auto outStr = std::make_shared<std::string>(encodeSize, '\0');
    auto inputPtr = reinterpret_cast<const unsigned char*>(input.data());
    auto outStrPtr = reinterpret_cast<unsigned char*>(outStr->data());

    int actualSize = EVP_EncodeBlock(outStrPtr, inputPtr, static_cast<int>(inputSize));
    if (actualSize < 0) {
        return nullptr;
    }
    outStr->resize(actualSize);
    return outStr;
}

std::shared_ptr<std::vector<unsigned char>> Base64::Decode(const std::string &input)
{
    auto size = input.size();
    size_t bufLen = size / BASE64_ENCODED_UNIT * BASE64_INPUT_UNIT;
    auto outBuffer = std::make_shared<std::vector<unsigned char>>(bufLen, 0);
    auto outLen = EVP_DecodeBlock(reinterpret_cast<unsigned char *>(outBuffer->data()),
                                  reinterpret_cast<const unsigned char *>(input.c_str()), size);
    if (outLen < 0) {
        return nullptr;
    }
    for (unsigned int i = 0; i < MAX_BASE64_BUF_PADDING_SIZE; ++i) {
        if (size > 1 + i && input.at(size - 1 - i) == '=') {
            outLen--;
        } else {
            break;
        }
    }

    outBuffer->resize(outLen);
    return outBuffer;
}
}
}