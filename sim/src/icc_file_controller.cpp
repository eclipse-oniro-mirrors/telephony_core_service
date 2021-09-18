/*
 * Copyright (C) 2021 Huawei Device Co., Ltd.
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

#include "icc_file_controller.h"

using namespace std;
using namespace OHOS::AppExecFwk;

namespace OHOS {
namespace Telephony {
IccFileController::IccFileController(const std::shared_ptr<AppExecFwk::EventRunner> &runner)
    : AppExecFwk::EventHandler(runner)
{}

void IccFileController::ProcessEvent(const AppExecFwk::InnerEvent::Pointer &event)
{
    int id = 0;
    id = event->GetInnerEventId();
    TELEPHONY_LOGI("IccFileController ProcessEvent Id is %{public}d", id);
    if (ProcessErrorResponse(event)) {
        return;
    }
    switch (id) {
        case MSG_SIM_OBTAIN_SIZE_OF_LINEAR_ELEMENTARY_FILE_DONE:
            ProcessLinearRecordSize(event);
            break;
        case MSG_SIM_OBTAIN_SIZE_OF_FIXED_ELEMENTARY_FILE_DONE:
            ProcessRecordSize(event);
            break;
        case MSG_SIM_OBTAIN_SIZE_OF_TRANSPARENT_ELEMENTARY_FILE_DONE:
            ProcessBinarySize(event);
            break;
        case MSG_SIM_OBTAIN_FIXED_ELEMENTARY_FILE_DONE:
            ProcessReadRecord(event);
            break;
        case MSG_SIM_UPDATE_LINEAR_FIXED_FILE_DONE:
        case MSG_SIM_UPDATE_TRANSPARENT_ELEMENTARY_FILE_DONE:
        case MSG_SIM_OBTAIN_TRANSPARENT_ELEMENTARY_FILE_DONE:
            ProcessReadBinary(event);
            break;
        default:
            break;
    }
}

void IccFileController::ProcessLinearRecordSize(const AppExecFwk::InnerEvent::Pointer &event)
{
    std::string str = NULLSTR;
    std::string path = NULLSTR;
    auto eventId = event->GetInnerEventId();
    std::unique_ptr<IccFromRilMsg> rcvMsg = event->GetUniqueObject<IccFromRilMsg>();
    const AppExecFwk::InnerEvent::Pointer &process = rcvMsg->controlHolder->fileLoaded;
    IccFileData *result = &(rcvMsg->fileData);
    std::shared_ptr<IccControllerHolder> &hd = rcvMsg->controlHolder;
    path = hd->filePath;
    const char *iccDdata = (result->resultData).c_str();
    char *rawData = const_cast<char *>(iccDdata);
    unsigned char *fileData = reinterpret_cast<unsigned char *>(rawData); // for unsigned int bitwise

    if (IsValidSizeData(fileData)) {
        int fileSize[] = {0, 0, 0};
        ParseFileSize(fileSize, RECORD_NUM, fileData);
        SendEfLinearResult(process, fileSize, RECORD_NUM);
    } else {
        TELEPHONY_LOGI("IccFileTypeMismatch Error type %{public}d", eventId);
    }
}

void IccFileController::ProcessRecordSize(const AppExecFwk::InnerEvent::Pointer &event)
{
    int size = 0;
    std::string str = NULLSTR;
    std::string path = NULLSTR;
    std::unique_ptr<IccFromRilMsg> rcvMsg = event->GetUniqueObject<IccFromRilMsg>();
    IccFileData *result = &(rcvMsg->fileData);
    std::shared_ptr<IccControllerHolder> &hd = rcvMsg->controlHolder;
    const char *iccDdata = (result->resultData).c_str();
    char *rawData = const_cast<char *>(iccDdata);
    unsigned char *fileData = reinterpret_cast<unsigned char *>(rawData); // for unsigned int bitwise

    path = CheckRightPath(hd->filePath, hd->fileId);
    if (!IsValidSizeData(fileData)) {
        TELEPHONY_LOGE("ProcessRecordSize get error filetype");
    }
    GetFileAndDataSize(fileData, hd->fileSize, size);
    if (hd->fileSize != 0) {
        hd->countFiles = size / hd->fileSize;
    }
    TELEPHONY_LOGI("ProcessRecordSize  %{public}d %{public}d %{public}d", size, hd->fileSize, hd->countFiles);
    if (rilManager_ != nullptr) {
        rilManager_->RequestSimIO(CONTROLLER_REQ_READ_RECORD, hd->fileId, hd->fileNum, ICC_FILE_CURRENT_MODE,
            hd->fileSize, NULLSTR, path, CreatePointer(MSG_SIM_OBTAIN_FIXED_ELEMENTARY_FILE_DONE, hd));
    }
}

void IccFileController::ProcessBinarySize(const AppExecFwk::InnerEvent::Pointer &event)
{
    std::string str = NULLSTR;
    std::string path = NULLSTR;
    TELEPHONY_LOGI("IccFileController::ProcessBinarySize init");
    std::unique_ptr<IccFromRilMsg> rcvMsg = event->GetUniqueObject<IccFromRilMsg>();
    bool isNull = rcvMsg->controlHolder == nullptr;
    const AppExecFwk::InnerEvent::Pointer &evt = rcvMsg->controlHolder->fileLoaded;
    isNull = evt->GetOwner() == nullptr;
    int fileId = rcvMsg->arg1;
    int size = 0;
    AppExecFwk::InnerEvent::Pointer process =
        CreatePointer(MSG_SIM_OBTAIN_TRANSPARENT_ELEMENTARY_FILE_DONE, fileId, 0, evt);
    if (rilManager_ != nullptr) {
        rilManager_->RequestSimIO(
            CONTROLLER_REQ_READ_BINARY, fileId, 0, 0, size, NULLSTR, ObtainElementFilePath(fileId), process);
    }
    TELEPHONY_LOGI("IccFileController::ProcessBinarySize finish %{public}d", fileId);
}

void IccFileController::ProcessReadRecord(const AppExecFwk::InnerEvent::Pointer &event)
{
    std::string str = NULLSTR;
    std::string path = NULLSTR;
    std::unique_ptr<IccFromRilMsg> rcvMsg = event->GetUniqueObject<IccFromRilMsg>();
    const AppExecFwk::InnerEvent::Pointer &process = rcvMsg->controlHolder->fileLoaded;
    IccFileData *result = &(rcvMsg->fileData);
    std::shared_ptr<IccControllerHolder> &hd = rcvMsg->controlHolder;
    TELEPHONY_LOGI("ProcessReadRecord %{public}d %{public}d %{public}d %{public}s", hd->getAllFile, hd->fileNum,
        hd->countFiles, result->resultData.c_str());
    path = CheckRightPath(hd->filePath, hd->fileId);
    if (hd->getAllFile) {
        hd->fileResults.push_back(result->resultData);
        hd->fileNum++;
        if (hd->fileNum > hd->countFiles) {
            SendMultiRecordResult(process, hd->fileResults);
        } else {
            rilManager_->RequestSimIO(CONTROLLER_REQ_READ_RECORD, hd->fileId, hd->fileNum, ICC_FILE_CURRENT_MODE,
                hd->fileSize, NULLSTR, path, CreatePointer(MSG_SIM_OBTAIN_FIXED_ELEMENTARY_FILE_DONE, hd));
        }
    } else {
        SendResponse(rcvMsg->controlHolder, &(rcvMsg->fileData));
    }
}

void IccFileController::ProcessReadBinary(const AppExecFwk::InnerEvent::Pointer &event)
{
    TELEPHONY_LOGI("IccFileController MSG_SIM_OBTAIN_TRANSPARENT_ELEMENTARY_FILE_DONE");
    std::unique_ptr<IccFromRilMsg> rcvMsg = event->GetUniqueObject<IccFromRilMsg>();
    SendResponse(rcvMsg->controlHolder, &(rcvMsg->fileData));
}

std::string IccFileController::ObtainElementFileForPublic(int efId)
{
    std::string mf = MASTER_FILE_SIM;
    switch (efId) {
        case ELEMENTARY_FILE_ADN:
        case ELEMENTARY_FILE_FDN:
        case ELEMENTARY_FILE_MSISDN:
        case ELEMENTARY_FILE_SDN:
        case ELEMENTARY_FILE_EXT1:
        case ELEMENTARY_FILE_EXT2:
        case ELEMENTARY_FILE_EXT3:
            mf.append(DEDICATED_FILE_TELECOM);
            return mf;
        case ELEMENTARY_FILE_ICCID:
        case ELEMENTARY_FILE_PL:
            return mf;
        case ELEMENTARY_FILE_PBR:
            mf.append(DEDICATED_FILE_TELECOM);
            mf.append(DEDICATED_FILE_PHONEBOOK);
            return mf;
        case ELEMENTARY_FILE_IMG:
            mf.append(DEDICATED_FILE_TELECOM);
            mf.append(DEDICATED_FILE_GRAPHICS);
            return mf;
        default:
            break;
    }
    return NULLSTR;
}

// implementation ObtainBinaryFile
void IccFileController::ObtainBinaryFile(int fileId, const AppExecFwk::InnerEvent::Pointer &event)
{
    TELEPHONY_LOGI("IccFileController::ObtainBinaryFile start");
    AppExecFwk::InnerEvent::Pointer process =
        CreatePointer(MSG_SIM_OBTAIN_SIZE_OF_TRANSPARENT_ELEMENTARY_FILE_DONE, fileId, 0, event);
    if (rilManager_ != nullptr) {
        rilManager_->RequestSimIO(CONTROLLER_REQ_GET_RESPONSE, fileId, 0, 0,
            GET_RESPONSE_ELEMENTARY_FILE_SIZE_BYTES, NULLSTR, ObtainElementFilePath(fileId), process);
    }
    TELEPHONY_LOGI("IccFileController::ObtainBinaryFile end");
}

void IccFileController::ObtainBinaryFile(int fileId, int size, const AppExecFwk::InnerEvent::Pointer &event)
{
    AppExecFwk::InnerEvent::Pointer process =
        CreatePointer(MSG_SIM_OBTAIN_TRANSPARENT_ELEMENTARY_FILE_DONE, fileId, 0, event);
    if (rilManager_ != nullptr) {
        rilManager_->RequestSimIO(
            CONTROLLER_REQ_READ_BINARY, fileId, 0, 0, size, NULLSTR, ObtainElementFilePath(fileId), process);
    }
}

// implementation ObtainLinearFixedFile
void IccFileController::ObtainLinearFixedFile(
    int fileId, const std::string &path, int fileNum, const AppExecFwk::InnerEvent::Pointer &event)
{
    std::string filePath = CheckRightPath(path, fileId);
    std::shared_ptr<IccControllerHolder> ctrlHolder =
        std::make_shared<IccControllerHolder>(fileId, fileNum, filePath);
    ctrlHolder->fileLoaded = std::move(const_cast<AppExecFwk::InnerEvent::Pointer &>(event));
    AppExecFwk::InnerEvent::Pointer process =
        CreatePointer(MSG_SIM_OBTAIN_SIZE_OF_FIXED_ELEMENTARY_FILE_DONE, ctrlHolder);
    if (rilManager_ != nullptr) {
        rilManager_->RequestSimIO(CONTROLLER_REQ_GET_RESPONSE, fileId, 0, 0,
            GET_RESPONSE_ELEMENTARY_FILE_SIZE_BYTES, NULLSTR, filePath, process);
    }
}

void IccFileController::ObtainLinearFixedFile(int fileId, int fileNum, const AppExecFwk::InnerEvent::Pointer &event)
{
    ObtainLinearFixedFile(fileId, ObtainElementFilePath(fileId), fileNum, event);
}

// implementation ObtainAllLinearFixedFile
void IccFileController::ObtainAllLinearFixedFile(
    int fileId, const std::string &path, const AppExecFwk::InnerEvent::Pointer &event)
{
    std::string filePath = CheckRightPath(path, fileId);
    std::shared_ptr<IccControllerHolder> ctrlHolder = std::make_shared<IccControllerHolder>(fileId, filePath);
    ctrlHolder->fileLoaded = std::move(const_cast<AppExecFwk::InnerEvent::Pointer &>(event));
    AppExecFwk::InnerEvent::Pointer process =
        CreatePointer(MSG_SIM_OBTAIN_SIZE_OF_FIXED_ELEMENTARY_FILE_DONE, ctrlHolder);
    if (rilManager_ != nullptr) {
        rilManager_->RequestSimIO(CONTROLLER_REQ_GET_RESPONSE, fileId, 0, 0,
            GET_RESPONSE_ELEMENTARY_FILE_SIZE_BYTES, NULLSTR, filePath, process);
    }
}

void IccFileController::ObtainAllLinearFixedFile(int fileId, const AppExecFwk::InnerEvent::Pointer &event)
{
    ObtainAllLinearFixedFile(fileId, ObtainElementFilePath(fileId), event);
}

void IccFileController::ObtainLinearFileSize(
    int fileId, const std::string &path, const AppExecFwk::InnerEvent::Pointer &event)
{
    std::string filePath = CheckRightPath(path, fileId);
    std::shared_ptr<IccControllerHolder> ctrlHolder = std::make_shared<IccControllerHolder>(fileId, filePath);
    ctrlHolder->fileLoaded = std::move(const_cast<AppExecFwk::InnerEvent::Pointer &>(event));
    AppExecFwk::InnerEvent::Pointer process =
        CreatePointer(MSG_SIM_OBTAIN_SIZE_OF_LINEAR_ELEMENTARY_FILE_DONE, ctrlHolder);
    if (rilManager_ != nullptr) {
        rilManager_->RequestSimIO(CONTROLLER_REQ_GET_RESPONSE, fileId, 0, 0,
            GET_RESPONSE_ELEMENTARY_FILE_SIZE_BYTES, NULLSTR, filePath, process);
    }
}

void IccFileController::ObtainLinearFileSize(int fileId, const AppExecFwk::InnerEvent::Pointer &event)
{
    ObtainLinearFileSize(fileId, ObtainElementFilePath(fileId), event);
}

void IccFileController::UpdateLinearFixedFile(int fileId, const std::string &path, int fileNum, std::string data,
    int dataLength, const std::string pin2, const AppExecFwk::InnerEvent::Pointer &onComplete)
{
    std::string filePath = CheckRightPath(path, fileId);
    std::shared_ptr<IccControllerHolder> ctrlHolder = std::make_shared<IccControllerHolder>(fileId);
    ctrlHolder->fileLoaded = std::move(const_cast<AppExecFwk::InnerEvent::Pointer &>(onComplete));
    AppExecFwk::InnerEvent::Pointer process = CreatePointer(MSG_SIM_UPDATE_LINEAR_FIXED_FILE_DONE, ctrlHolder);
    if (rilManager_ != nullptr) {
        rilManager_->RequestSimIO(CONTROLLER_REQ_UPDATE_RECORD, fileId, fileNum, ICC_FILE_CURRENT_MODE, dataLength,
            data, filePath, process);
    }
}

void IccFileController::UpdateLinearFixedFile(int fileId, int fileNum, const std::string data, int dataLength,
    const std::string pin2, const AppExecFwk::InnerEvent::Pointer &onComplete)
{
    std::shared_ptr<IccControllerHolder> ctrlHolder = std::make_shared<IccControllerHolder>(fileId);
    ctrlHolder->fileLoaded = std::move(const_cast<AppExecFwk::InnerEvent::Pointer &>(onComplete));
    AppExecFwk::InnerEvent::Pointer process = CreatePointer(MSG_SIM_UPDATE_LINEAR_FIXED_FILE_DONE, ctrlHolder);
    if (rilManager_ != nullptr) {
        rilManager_->RequestSimIO(CONTROLLER_REQ_UPDATE_RECORD, fileId, fileNum, ICC_FILE_CURRENT_MODE, dataLength,
            data, ObtainElementFilePath(fileId), process);
    }
}

void IccFileController::UpdateBinaryFile(
    int fileId, const std::string data, int dataLength, const AppExecFwk::InnerEvent::Pointer &onComplete)
{
    std::shared_ptr<IccControllerHolder> ctrlHolder = std::make_shared<IccControllerHolder>(fileId);
    ctrlHolder->fileLoaded = std::move(const_cast<AppExecFwk::InnerEvent::Pointer &>(onComplete));
    AppExecFwk::InnerEvent::Pointer process =
        CreatePointer(MSG_SIM_UPDATE_TRANSPARENT_ELEMENTARY_FILE_DONE, ctrlHolder);
    if (rilManager_ != nullptr) {
        rilManager_->RequestSimIO(
            CONTROLLER_REQ_UPDATE_BINARY, fileId, 0, 0, dataLength, data, ObtainElementFilePath(fileId), process);
    }
}

void IccFileController::SendResponse(std::shared_ptr<IccControllerHolder> holder, const IccFileData *fd)
{
    if (holder == nullptr || fd == nullptr) {
        TELEPHONY_LOGE("IccFileController::SendResponse  result is null");
        return;
    }
    AppExecFwk::InnerEvent::Pointer &response = holder->fileLoaded;
    bool isNull = (response == nullptr);
    auto owner = response->GetOwner();
    std::unique_ptr<FileToControllerMsg> cmdData = response->GetUniqueObject<FileToControllerMsg>();
    int id = response->GetInnerEventId();
    bool needShare = (id == MSG_SIM_OBTAIN_ICC_FILE_DONE);
    std::unique_ptr<ControllerToFileMsg> objectUnique = nullptr;
    std::shared_ptr<ControllerToFileMsg> objectShare = nullptr;
    TELEPHONY_LOGI("IccFileController::SendResponse start response %{public}d %{public}d", isNull, needShare);
    if (needShare) {
        objectShare = std::make_shared<ControllerToFileMsg>(cmdData.get(), fd);
    } else {
        objectUnique = std::make_unique<ControllerToFileMsg>(cmdData.get(), fd);
    }

    if ((objectUnique == nullptr) && (objectShare == nullptr)) {
        TELEPHONY_LOGE("IccFileController::SendResponse  create ControllerToFileMsg is null");
        return;
    }

    isNull = (owner == nullptr);
    TELEPHONY_LOGI("IccFileController::SendResponse owner: %{public}d evtId: %{public}d", isNull, id);
    if (needShare) {
        owner->SendEvent(id, objectShare);
    } else {
        owner->SendEvent(id, objectUnique);
    }
    TELEPHONY_LOGI("IccFileController::SendResponse send end");
}

void IccFileController::SendEfLinearResult(
    const AppExecFwk::InnerEvent::Pointer &response, const int val[], int len)
{
    std::shared_ptr<AppExecFwk::EventHandler> handler = response->GetOwner();
    std::shared_ptr<EfLinearResult> object = std::make_shared<EfLinearResult>();
    object->valueData[0] = val[0];
    object->valueData[1] = val[1];
    object->valueData[MAX_FILE_INDEX] = val[MAX_FILE_INDEX];
    int id = response->GetInnerEventId();
    int eventParam = 0;
    AppExecFwk::InnerEvent::Pointer event = AppExecFwk::InnerEvent::Get(id, object, eventParam);
    handler->SendEvent(event);
}

void IccFileController::SendMultiRecordResult(
    const AppExecFwk::InnerEvent::Pointer &response, std::vector<std::string> &strValue)
{
    std::shared_ptr<AppExecFwk::EventHandler> handler = response->GetOwner();
    std::shared_ptr<MultiRecordResult> object = std::make_shared<MultiRecordResult>();
    object->fileResults.assign(strValue.begin(), strValue.end());
    object->resultLength = strValue.size();
    int id = response->GetInnerEventId();
    int eventParam = 0;
    TELEPHONY_LOGI("IccFileController::SendMultiRecordResult send end");
    AppExecFwk::InnerEvent::Pointer event = AppExecFwk::InnerEvent::Get(id, object, eventParam);
    handler->SendEvent(event);
}

AppExecFwk::InnerEvent::Pointer IccFileController::CreatePointer(
    int eventId, std::shared_ptr<IccControllerHolder> &holderObject)
{
    std::unique_ptr<IccToRilMsg> msgTo = std::make_unique<IccToRilMsg>(holderObject);
    if (msgTo == nullptr) {
        TELEPHONY_LOGE("IccFileController::CreatePointer1  create null pointer");
        return AppExecFwk::InnerEvent::Pointer(nullptr, nullptr);
    }
    int64_t eventParam = 0;
    AppExecFwk::InnerEvent::Pointer event = AppExecFwk::InnerEvent::Get(eventId, msgTo, eventParam);
    event->SetOwner(shared_from_this());
    return event;
}

AppExecFwk::InnerEvent::Pointer IccFileController::CreatePointer(
    int eventId, int arg1, int arg2, std::shared_ptr<IccControllerHolder> &holderObject)
{
    std::unique_ptr<IccToRilMsg> msgTo = std::make_unique<IccToRilMsg>(holderObject);
    if (msgTo == nullptr) {
        TELEPHONY_LOGE("IccFileController::CreatePointer2  create null pointer");
        return AppExecFwk::InnerEvent::Pointer(nullptr, nullptr);
    }
    msgTo->arg1 = arg1;
    msgTo->arg2 = arg2;
    int64_t eventParam = 0;
    AppExecFwk::InnerEvent::Pointer event = AppExecFwk::InnerEvent::Get(eventId, msgTo, eventParam);
    event->SetOwner(shared_from_this());
    return event;
}

AppExecFwk::InnerEvent::Pointer IccFileController::CreatePointer(
    int eventId, int arg1, int arg2, const AppExecFwk::InnerEvent::Pointer &msg)
{
    std::shared_ptr<IccControllerHolder> ctrlHolder = std::make_shared<IccControllerHolder>(arg1);
    ctrlHolder->fileLoaded = std::move(const_cast<AppExecFwk::InnerEvent::Pointer &>(msg));
    bool isNull = ctrlHolder->fileLoaded->GetOwner() == nullptr;
    TELEPHONY_LOGI("IccFileController::CreatePointer stage init owner: %{public}d", isNull);
    std::unique_ptr<IccToRilMsg> msgTo = std::make_unique<IccToRilMsg>(ctrlHolder);
    if (msgTo == nullptr) {
        TELEPHONY_LOGE("IccFileController::CreatePointer3  create null pointer");
        return AppExecFwk::InnerEvent::Pointer(nullptr, nullptr);
    }
    TELEPHONY_LOGI("IccFileController::CreatePointer stage end");
    msgTo->arg1 = arg1;
    msgTo->arg2 = arg2;
    int64_t eventParam = 0;
    AppExecFwk::InnerEvent::Pointer event = AppExecFwk::InnerEvent::Get(eventId, msgTo, eventParam);
    event->SetOwner(shared_from_this());
    return event;
}

void IccFileController::ParseFileSize(int val[], int len, const unsigned char *data)
{
    if (len > MAX_FILE_INDEX) {
        GetFileAndDataSize(data, val[0], val[1]);
        val[MAX_FILE_INDEX] = val[1] / val[0];
    }
    TELEPHONY_LOGI("ParseFileSize result %{public}d, %{public}d %{public}d", val[0], val[1], val[MAX_FILE_INDEX]);
}
bool IccFileController::IsValidSizeData(const unsigned char *data)
{
    if (ICC_ELEMENTARY_FILE != data[TYPE_OF_FILE]) {
        TELEPHONY_LOGE("IccFileTypeMismatch ERROR TYPE_OF_FILE");
        return false;
    }
    if (ELEMENTARY_FILE_TYPE_LINEAR_FIXED != data[STRUCTURE_OF_DATA]) {
        TELEPHONY_LOGE("IccFileTypeMismatch ERROR STRUCTURE_OF_DATA");
        return false;
    }
    return true;
}
void IccFileController::GetFileAndDataSize(const unsigned char *data, int &fileSize, int &dataSize)
{
    if (data != nullptr) {
        fileSize = data[LENGTH_OF_RECORD] & BYTE_NUM;
        dataSize = ((data[SIZE_ONE_OF_FILE] & BYTE_NUM) << OFFSET) + (data[SIZE_TWO_OF_FILE] & BYTE_NUM);
    }
}

void IccFileController::SetRilManager(std::shared_ptr<Telephony::IRilManager> ril)
{
    rilManager_ = ril;
    if (rilManager_ == nullptr) {
        TELEPHONY_LOGE("IccFileController set NULL rilManager!!");
    }
}

std::string IccFileController::CheckRightPath(const std::string &path, int fileId)
{
    if (path.empty()) {
        return ObtainElementFilePath(fileId);
    } else {
        return path;
    }
}

bool IccFileController::ProcessErrorResponse(const AppExecFwk::InnerEvent::Pointer &event)
{
    std::shared_ptr<IccFromRilMsg> rcvMsg = event->GetSharedObject<IccFromRilMsg>();
    if (rcvMsg == nullptr) {
        return false;
    }
    AppExecFwk::InnerEvent::Pointer &response = rcvMsg->controlHolder->fileLoaded;
    auto owner = response->GetOwner();
    int id = response->GetInnerEventId();
    std::unique_ptr<FileToControllerMsg> cmdData = response->GetUniqueObject<FileToControllerMsg>();
    bool needShare = (id == MSG_SIM_OBTAIN_ICC_FILE_DONE);
    std::unique_ptr<ControllerToFileMsg> objectUnique = nullptr;
    std::shared_ptr<ControllerToFileMsg> objectShare = nullptr;
    TELEPHONY_LOGI("ProcessErrorResponse start response %{public}d", needShare);
    if (needShare) {
        objectShare = std::make_shared<ControllerToFileMsg>(cmdData.get(), nullptr);
        objectShare->exception = rcvMsg->fileData.exception;
    } else {
        objectUnique = std::make_unique<ControllerToFileMsg>(cmdData.get(), nullptr);
        objectUnique->exception = rcvMsg->fileData.exception;
    }

    if ((objectUnique == nullptr) && (objectShare == nullptr)) {
        TELEPHONY_LOGE("ProcessErrorResponse  create ControllerToFileMsg is null");
        return true;
    }

    TELEPHONY_LOGI("ProcessErrorResponse owner: evtId: %{public}d", id);
    if (needShare) {
        owner->SendEvent(id, objectShare);
    } else {
        owner->SendEvent(id, objectUnique);
    }
    TELEPHONY_LOGI("ProcessErrorResponse send end");
    return true;
}

IccFileController::~IccFileController() {}
} // namespace Telephony
} // namespace OHOS
