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

#ifndef OHOS_OPERATOR_CONFIG_CACHE_H
#define OHOS_OPERATOR_CONFIG_CACHE_H
#include "core_service_errors.h"
#include "operator_file_parser.h"
#include "sim_file_manager.h"
#include "sim_state_manager.h"
#include "telephony_log_wrapper.h"

namespace OHOS {
namespace Telephony {
const std::string KEY_CONST_TELEPHONY_IS_USE_CLOUD_IMS_NV = "const.telephony.is_use_cloud_ims_nv";
const std::string KEY_PERSIST_TELEPHONY_VOLTE_CAP_IN_CHIP = "persist.telephony.volte_cap_in_chip_slot";
class OperatorConfigCache : public TelEventHandler {
public:
    explicit OperatorConfigCache(
        std::weak_ptr<SimFileManager> simFileManager, std::shared_ptr<SimStateManager> simStateManager, int32_t slotId);
    virtual ~OperatorConfigCache() = default;
    void ClearAllCache(int32_t slotId);
    void ClearMemoryCache(int32_t slotId);
    void ClearOperatorValue(int32_t slotId);
    void ClearMemoryAndOpkey(int32_t slotId);
    int32_t LoadOperatorConfig(int32_t slotId, OperatorConfig &poc, int32_t state = 0);
    int32_t GetOperatorConfigs(int32_t slotId, OperatorConfig &poc);
    int32_t UpdateOperatorConfigs(int32_t slotId);
    std::string EncryptIccId(const std::string iccid);
    void ProcessEvent(const AppExecFwk::InnerEvent::Pointer &event);
    bool RegisterForIccChange();
    bool UnRegisterForIccChange();
    bool IsNeedOperatorLoad(int32_t slotId);
    inline static const int32_t STATE_PARA_LOADED = 1;
    inline static const int32_t STATE_PARA_CLEAR = 2;
    inline static const int32_t STATE_PARA_UPDATE = 3;
    void UpdateImsCapFromChip(int32_t slotId, const ImsCapFromChip &imsCapFromChip);
    void UpdatevolteCap(int32_t slotId, OperatorConfig &opc);
    void UpdateOpcBoolValue(OperatorConfig &opc, const std::string &key, const bool value);

private:
    OperatorFileParser parser_;
    std::weak_ptr<SimFileManager> simFileManager_;
    std::shared_ptr<SimStateManager> simStateManager_;
    std::string GetOpKey(int32_t slotId);
    void CopyOperatorConfig(const OperatorConfig &from, OperatorConfig &to);
    void UpdateCurrentOpc(int32_t slotId, OperatorConfig &poc);
    void SendSimMatchedOperatorInfo(int32_t slotId, int32_t state);
    bool AnnounceOperatorConfigChanged(int32_t slotId, int32_t state);
    void notifyInitApnConfigs(int32_t slotId);
    int32_t LoadOperatorConfigFile(int32_t slotId, OperatorConfig &poc);
    void UpdateIccidCache(int32_t state);
    int GetSimState(int32_t slotId, SimState &simState);
    inline static const std::string KEY_SLOTID = "slotId";
    inline static const std::string CHANGE_STATE = "state";
    inline static const int32_t IMS_SWITCH_OFF = 0;
    inline static const int32_t IMS_SWITCH_ON = 1;
    inline static const int32_t IMS_SWITCH_DEFAULT = 2;
    inline static const std::string OPERATOR_CONFIG_CHANGED = "operatorConfigChanged";
    OperatorConfig opc_;
    int32_t slotId_;
    std::string modemSimMatchedOpNameCache_ = "";
    std::string iccidCache_ = "";
    bool isLoadingConfig_ = false;
    std::mutex mutex_;
    bool isUpdateImsCapFromChipDone_ = false;
    int32_t retryBatchInsertApnTimes_ = 0;
    static const int32_t BATCH_INSERT_APN_RETRY_TIMES = 5;
    static const int32_t BATCH_INSERT_APN_RETRY_DEALY = 10*1000;
    enum {
        RADIO_BATCH_INSERT_APN_RETRY = 0,
    };
};
} // namespace Telephony
} // namespace OHOS
#endif // OHOS_OPERATOR_FILE_PARSER_H
