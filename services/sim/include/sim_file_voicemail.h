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

#ifndef OHOS_SIM_FILE_VOICEMAIL_H
#define OHOS_SIM_FILE_VOICEMAIL_H

#include <cstdint>
#include <memory>
#include <string>

#include "sim_file.h"

namespace OHOS {
namespace Telephony {
// Helper holding SimFile's voice-mail / call-forwarding / CPhs-mailbox logic.
// Friend of SimFile: accesses its (and inherited IccFile's) members via the
// SimFile& parameter, mirroring the SimFileParse / SimFileInit pattern.
class SimFileVoiceMail {
public:
    std::string GetVoiceMailNumber(SimFile &simFile);
    void SetVoiceMailNumber(const std::string mailNumber, SimFile &simFile);
    bool UpdateVoiceMail(const std::string &mailName, const std::string &mailNumber, SimFile &simFile);
    bool SetVoiceMailCount(int32_t voiceMailCount, SimFile &simFile);
    bool SetVoiceCallForwarding(bool enable, const std::string &number, SimFile &simFile);
    bool CphsVoiceMailAvailable(SimFile &simFile);
    bool EfCfisAvailable(SimFile &simFile, int32_t size);
    void GetCphsMailBox(SimFile &simFile);

private:
    bool VoiceMailNotEditToSim(SimFile &simFile);
    bool FillNumber(std::shared_ptr<unsigned char> efCfisData, int32_t efCfisSize, const std::string &number,
        SimFile &simFile);
};
} // namespace Telephony
} // namespace OHOS
#endif // OHOS_SIM_FILE_VOICEMAIL_H