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

#include "esim_file.h"

#include <unistd.h>

#include "common_event_manager.h"
#include "common_event_support.h"
#include "core_manager_inner.h"
#include "core_service.h"
#include "parameters.h"
#include "radio_event.h"
#include "sim_number_decode.h"
#include "str_convert.h"
#include "telephony_common_utils.h"
#include "telephony_ext_wrapper.h"
#include "telephony_state_registry_client.h"
#include "telephony_tag_def.h"
#include "vcard_utils.h"

using namespace OHOS::AppExecFwk;
using namespace OHOS::EventFwk;

namespace OHOS {
namespace Telephony {
namespace {
constexpr uint32_t BYTE_TO_HEX_LENGTH = 2;
constexpr uint32_t OFFSET_FOUR_BIT = 4;
constexpr uint32_t VERSION_NUMBER = 11;
constexpr int32_t ATR_LENGTH = 47;
}

ResponseEsimResult EsimFile::ObtainEuiccInfo2(int32_t portIndex)
{
    esimProfile_.portIndex = portIndex;
    SyncOpenChannel();
    AppExecFwk::InnerEvent::Pointer eventEUICCInfo2 = BuildCallerInfo(MSG_ESIM_OBTAIN_EUICC_INFO2_DONE);
    if (!ProcessObtainEuiccInfo2(slotId_, eventEUICCInfo2)) {
        TELEPHONY_LOGE("ProcessObtainEuiccInfo2 encode failed");
        return ResponseEsimResult();
    }
    isEuiccInfo2Ready_ = false;
    std::unique_lock<std::mutex> lock(euiccInfo2Mutex_);
    if (!euiccInfo2Cv_.wait_for(lock, std::chrono::seconds(WAIT_TIME_LONG_SECOND_FOR_ESIM),
        [this]() { return isEuiccInfo2Ready_; })) {
        SyncCloseChannel();
        return ResponseEsimResult();
    }
    SyncCloseChannel();
    return responseInfo2Result_;
}

ResponseEsimResult EsimFile::AuthenticateServer(const AuthenticateConfigInfo &authenticateConfigInfo)
{
    esimProfile_.portIndex = authenticateConfigInfo.portIndex;
    esimProfile_.matchingId = authenticateConfigInfo.matchingId;
    esimProfile_.serverSigned1 = authenticateConfigInfo.serverSigned1;
    esimProfile_.serverSignature1 = authenticateConfigInfo.serverSignature1;
    esimProfile_.euiccCiPkIdToBeUsed = authenticateConfigInfo.euiccCiPkIdToBeUsed;
    esimProfile_.serverCertificate = authenticateConfigInfo.serverCertificate;

    std::u16string imei = u"";
    CoreManagerInner::GetInstance().GetImei(slotId_, imei);
    esimProfile_.imei = imei;
    SyncOpenChannel();
    if (!ProcessAuthenticateServer(slotId_)) {
        TELEPHONY_LOGE("ProcessAuthenticateServer encode failed");
        return ResponseEsimResult();
    }
    isAuthenticateServerReady_ = false;
    std::unique_lock<std::mutex> lock(authenticateServerMutex_);
    if (!authenticateServerCv_.wait_for(lock, std::chrono::seconds(WAIT_TIME_LONG_SECOND_FOR_ESIM),
        [this]() { return isAuthenticateServerReady_; })) {
        SyncCloseChannel();
        return ResponseEsimResult();
    }
    SyncCloseChannel();
    return responseAuthenticateResult_;
}

bool EsimFile::ProcessObtainEuiccInfo2(int32_t slotId, const AppExecFwk::InnerEvent::Pointer &responseEvent)
{
    if (IsLogicChannelOpen()) {
        return false;
    }
    std::shared_ptr<Asn1Builder> builder = std::make_shared<Asn1Builder>(TAG_ESIM_GET_EUICC_INFO_2);
    if (builder == nullptr) {
        TELEPHONY_LOGE("builder is nullptr");
        return false;
    }
    std::string hexStr;
    uint32_t strLen = builder->Asn1BuilderToHexStr(hexStr);
    ApduSimIORequestInfo reqInfo;
    CommBuildOneApduReqInfo(reqInfo, builder);
    if (telRilManager_ == nullptr) {
        return false;
    }
    int32_t apduResult = telRilManager_->SimTransmitApduLogicalChannel(slotId, reqInfo, responseEvent);
    if (apduResult == TELEPHONY_ERR_FAIL) {
        return false;
    }
    return true;
}

void EsimFile::ConvertAuthInputParaFromApiStru(Es9PlusInitAuthResp &dst, EsimProfile &src)
{
    dst.serverSigned1 = OHOS::Telephony::ToUtf8(src.serverSigned1);
    dst.serverSignature1 = OHOS::Telephony::ToUtf8(src.serverSignature1);
    dst.euiccCiPKIdToBeUsed = OHOS::Telephony::ToUtf8(src.euiccCiPkIdToBeUsed);
    dst.serverCertificate = OHOS::Telephony::ToUtf8(src.serverCertificate);
    dst.matchingId = OHOS::Telephony::ToUtf8(src.matchingId);
    dst.imei = OHOS::Telephony::ToUtf8(src.imei);
}

bool EsimFile::ProcessAuthenticateServer(int32_t slotId)
{
    if (IsLogicChannelOpen()) {
        return false;
    }
    Es9PlusInitAuthResp bytes;
    ConvertAuthInputParaFromApiStru(bytes, esimProfile_);
    std::shared_ptr<Asn1Builder> builder = std::make_shared<Asn1Builder>(TAG_ESIM_AUTHENTICATE_SERVER);
    if (builder == nullptr) {
        TELEPHONY_LOGE("builder create failed");
        return false;
    }
    Asn1AddChildAsBase64(builder, bytes.serverSigned1);
    Asn1AddChildAsBase64(builder, bytes.serverSignature1);
    Asn1AddChildAsBase64(builder, bytes.euiccCiPKIdToBeUsed);
    Asn1AddChildAsBase64(builder, bytes.serverCertificate);
    std::shared_ptr<Asn1Builder> ctxParams1Builder = std::make_shared<Asn1Builder>(TAG_ESIM_CTX_COMP_0);
    AddCtxParams1(ctxParams1Builder, &bytes);
    if (ctxParams1Builder == nullptr) {
        TELEPHONY_LOGE("AddCtxParams1 failed");
        return false;
    }
    std::shared_ptr<Asn1Node> ctxNode = ctxParams1Builder->Asn1Build();
    builder->Asn1AddChild(ctxNode);
    std::string hexStr;
    uint32_t hexStrLen = builder->Asn1BuilderToHexStr(hexStr);
    SplitSendLongData(slotId, hexStr);
    return true;
}

void EsimFile::SplitSendLongData(int32_t slotId, std::string hexStr)
{
    RequestApduBuild codec(currentChannelId_);
    codec.BuildStoreData(hexStr);
    std::list<std::unique_ptr<ApduCommand>> apduCommandList = codec.GetCommands();
    for (const auto &cmd : apduCommandList) {
        ApduSimIORequestInfo reqInfo;
        CopyApdCmdToReqInfo(&reqInfo, cmd.get());
        AppExecFwk::InnerEvent::Pointer tmpResponseEvent = BuildCallerInfo(MSG_ESIM_PREPARE_DOWNLOAD_DONE);
        if (telRilManager_ == nullptr) {
            return;
        }
        telRilManager_->SimTransmitApduLogicalChannel(slotId, reqInfo, tmpResponseEvent);
    }
}

void EsimFile::Asn1AddChildAsBase64(std::shared_ptr<Asn1Builder> &builder, const std::string &base64Src)
{
    std::string destString = VCardUtils::DecodeBase64(base64Src);
    std::vector<uint8_t> dest = Asn1Utils::StringToBytes(destString);
    std::shared_ptr<Asn1Decoder> decoder = std::make_shared<Asn1Decoder>(dest, 0, dest.size());
    if (decoder == nullptr) {
        TELEPHONY_LOGE("create decoder failed");
        return;
    }
    std::shared_ptr<Asn1Node> node = decoder->Asn1NextNode();
    if (builder == nullptr) {
        TELEPHONY_LOGE("build is nullptr");
        return;
    }
    builder->Asn1AddChild(node);
}

void EsimFile::AddDeviceCapability(std::shared_ptr<Asn1Builder> &devCapsBuilder)
{
    if (devCapsBuilder == nullptr) {
        TELEPHONY_LOGE("dev caps build failed");
        return;
    }
    std::vector<uint8_t> versionBytes;
    Asn1Utils::UintToBytes(VERSION_NUMBER, versionBytes);
    devCapsBuilder->Asn1AddChildAsBytes(TAG_ESIM_CTX_0, versionBytes, versionBytes.size());
    devCapsBuilder->Asn1AddChildAsBytes(TAG_ESIM_CTX_1, versionBytes, versionBytes.size());
    devCapsBuilder->Asn1AddChildAsBytes(TAG_ESIM_CTX_5, versionBytes, versionBytes.size());
}

void EsimFile::GetImeiBytes(std::vector<uint8_t> &imeiBytes, const std::string &imei)
{
    size_t imeiLen = imei.length();
    if (imeiLen < AUTH_SERVER_IMEI_LEN * BYTE_TO_HEX_LEN - 1) {
        return;
    }
    if (imeiLen != AUTH_SERVER_IMEI_LEN * BYTE_TO_HEX_LEN) {
        std::string newImei = imei;
        newImei += 'F';
        Asn1Utils::BcdToBytes(newImei, imeiBytes);
        unsigned char last = imeiBytes[LAST_BYTE_OF_IMEI];
        imeiBytes[LAST_BYTE_OF_IMEI] = static_cast<unsigned char>((last & 0xFF) <<
            OFFSET_FOUR_BIT | ((last & 0xFF) >> OFFSET_FOUR_BIT));
    } else {
        Asn1Utils::BcdToBytes(imei, imeiBytes);
    }
}

void EsimFile::AddCtxParams1(std::shared_ptr<Asn1Builder> &ctxParams1Builder, Es9PlusInitAuthResp *pbytes)
{
    if (pbytes == nullptr || ctxParams1Builder == nullptr) {
        TELEPHONY_LOGE("AddCtxParams1 pbytes is nullptr");
        return;
    }
    std::string matchingId;
    ctxParams1Builder->Asn1AddChildAsString(TAG_ESIM_CTX_0, matchingId);
    pbytes->matchingId = matchingId;
    std::shared_ptr<Asn1Node> subNode = nullptr;
    std::vector<uint8_t> tacBytes;
    std::vector<uint8_t> imeiBytes;
    Asn1Utils::BcdToBytes(pbytes->imei, tacBytes);
    GetImeiBytes(imeiBytes, pbytes->imei);
    std::shared_ptr<Asn1Builder> subBuilder = std::make_shared<Asn1Builder>(TAG_ESIM_CTX_COMP_1);
    if (subBuilder == nullptr) {
        TELEPHONY_LOGE("AddCtxParams1 subBuilder is nullptr");
        return;
    }
    subBuilder->Asn1AddChildAsBytes(TAG_ESIM_CTX_0, tacBytes, tacBytes.size());
    // add devCap
    std::shared_ptr<Asn1Builder> devCapsBuilder = std::make_shared<Asn1Builder>(TAG_ESIM_CTX_COMP_1);
    if (devCapsBuilder == nullptr) {
        TELEPHONY_LOGE("AddCtxParams1 devCapsBuilder is nullptr");
        return;
    }
    AddDeviceCapability(devCapsBuilder);
    std::shared_ptr<Asn1Node> devCapNode = devCapsBuilder->Asn1Build();
    subBuilder->Asn1AddChild(devCapNode);
    subBuilder->Asn1AddChildAsBytes(TAG_ESIM_CTX_2, imeiBytes, imeiBytes.size());
    subNode = subBuilder->Asn1Build();
    ctxParams1Builder->Asn1AddChild(subNode);
}

bool EsimFile::ProcessObtainEuiccInfo2Done(const AppExecFwk::InnerEvent::Pointer &event)
{
    if (event == nullptr) {
        TELEPHONY_LOGE("event is nullptr!");
        return false;
    }
    std::unique_ptr<IccFromRilMsg> rcvMsg = event->GetUniqueObject<IccFromRilMsg>();
    if (rcvMsg == nullptr) {
        TELEPHONY_LOGE("rcvMsg is nullptr");
        return false;
    }
    IccFileData *result = &(rcvMsg->fileData);
    if (result == nullptr) {
        return false;
    }
    std::vector<uint8_t> responseByte = Asn1Utils::HexStrToBytes(result->resultData);
    uint32_t byteLen = responseByte.size();
    std::shared_ptr<Asn1Node> root = Asn1ParseResponse(responseByte, byteLen);
    if (root == nullptr) {
        TELEPHONY_LOGE("Asn1ParseResponse failed");
        return false;
    }
    EuiccInfo2 euiccInfo2 = {};
    this->EuiccInfo2ParseProfileVersion(&euiccInfo2, root);
    this->EuiccInfo2ParseSvn(&euiccInfo2, root, byteLen);
    this->EuiccInfo2ParseEuiccFirmwareVer(&euiccInfo2, root);
    this->EuiccInfo2ParseExtCardResource(&euiccInfo2, root);
    this->EuiccInfo2ParseUiccCapability(&euiccInfo2, root);
    this->EuiccInfo2ParseTs102241Version(&euiccInfo2, root);
    this->EuiccInfo2ParseGlobalPlatformVersion(&euiccInfo2, root);
    this->EuiccInfo2ParseRspCapability(&euiccInfo2, root);
    this->EuiccInfo2ParseEuiccCiPKIdListForVerification(&euiccInfo2, root);
    this->EuiccInfo2ParseEuiccCiPKIdListForSigning(&euiccInfo2, root);
    this->EuiccInfo2ParseEuiccCategory(&euiccInfo2, root);
    this->EuiccInfo2ParsePpVersion(&euiccInfo2, root);
    {
        std::lock_guard<std::mutex> lock(euiccInfo2Mutex_);
        isEuiccInfo2Ready_ = true;
    }
    euiccInfo2Cv_.notify_one();
    return true;
}

std::string EsimFile::MakeVersionString(std::vector<uint8_t> &versionRaw)
{
    if (versionRaw.size() < BYTE_NUM3) {
        TELEPHONY_LOGE("versionRaw.size error!");
        return "";
    }
    std::string versionHigh("");
    std::string versionMid("");
    std::string versionLow("");
    Asn1Utils::ByteToHexStr(versionRaw[VERSION_HIGH], versionHigh);
    Asn1Utils::ByteToHexStr(versionRaw[VERSION_MIDDLE], versionMid);
    Asn1Utils::ByteToHexStr(versionRaw[VERSION_LOW], versionLow);
    std::string result = versionHigh + "." + versionMid + "." + versionLow;
    return result;
}

void EsimFile::EuiccInfo2ParseProfileVersion(EuiccInfo2 *euiccInfo2, std::shared_ptr<Asn1Node> &root)
{
    if (euiccInfo2 == nullptr) {
        TELEPHONY_LOGE("euiccInfo2 is nullptr");
        return;
    }
    std::shared_ptr<Asn1Node> profileVerNode = root->Asn1GetChild(TAG_ESIM_CTX_1);
    if (profileVerNode == nullptr) {
        TELEPHONY_LOGE("profileVerNode is nullptr");
        return;
    }
    std::vector<uint8_t> profileVersionRaw = {};
    uint32_t profileVersionRawLen = profileVerNode->Asn1AsBytes(profileVersionRaw);
    if (profileVersionRawLen < EUICC_INFO_VERSION_MIN_LENGTH) {
        TELEPHONY_LOGE("invalid profileVersion data");
        return;
    }
    euiccInfo2->profileVersion = MakeVersionString(profileVersionRaw);
}

void EsimFile::EuiccInfo2ParseSvn(
    EuiccInfo2 *euiccInfo2, std::shared_ptr<Asn1Node> &root, uint32_t byteLen)
{
    if (euiccInfo2 == nullptr) {
        TELEPHONY_LOGE("euiccInfo2 is nullptr");
        return;
    }
    std::shared_ptr<Asn1Node> svnNode = root->Asn1GetChild(TAG_ESIM_CTX_2);
    if (svnNode == nullptr) {
        TELEPHONY_LOGE("svnNode is nullptr");
        return;
    }
    std::vector<uint8_t> svnRaw = {};
    uint32_t svnRawLen = svnNode->Asn1AsBytes(svnRaw);
    if (svnRawLen < EUICC_INFO_VERSION_MIN_LENGTH) {
        TELEPHONY_LOGE("invalid SVN data");
        return;
    }
    euiccInfo2->svn = MakeVersionString(svnRaw);
}

void EsimFile::EuiccInfo2ParseEuiccFirmwareVer(EuiccInfo2 *euiccInfo2, std::shared_ptr<Asn1Node> &root)
{
    if (euiccInfo2 == nullptr) {
        TELEPHONY_LOGE("euiccInfo2 is nullptr");
        return;
    }
    std::shared_ptr<Asn1Node> euiccFirmwareVerNode = root->Asn1GetChild(TAG_ESIM_CTX_3);
    if (euiccFirmwareVerNode == nullptr) {
        TELEPHONY_LOGE("euiccFirmwareVerNode is nullptr");
        return;
    }
    std::vector<uint8_t> euiccFirmwareVerRaw = {};
    uint32_t versionLen = euiccFirmwareVerNode->Asn1AsBytes(euiccFirmwareVerRaw);
    if (versionLen < EUICC_INFO_VERSION_MIN_LENGTH) {
        TELEPHONY_LOGE("invalid firmwareVer data");
        return;
    }
    euiccInfo2->svn = MakeVersionString(euiccFirmwareVerRaw);
}

void EsimFile::EuiccInfo2ParseExtCardResource(EuiccInfo2 *euiccInfo2, std::shared_ptr<Asn1Node> &root)
{
    if (euiccInfo2 == nullptr) {
        TELEPHONY_LOGE("euiccInfo2 is nullptr");
        return;
    }
    std::shared_ptr<Asn1Node> extCardResourceNode = root->Asn1GetChild(TAG_ESIM_CTX_4);
    if (extCardResourceNode == nullptr) {
        TELEPHONY_LOGE("extCardResourceNode is nullptr");
        return;
    }
    extCardResourceNode->Asn1AsString(euiccInfo2->extCardResource);
}

void EsimFile::EuiccInfo2ParseUiccCapability(EuiccInfo2 *euiccInfo2, std::shared_ptr<Asn1Node> &root)
{
    if (euiccInfo2 == nullptr) {
        TELEPHONY_LOGE("euiccInfo2 is nullptr");
        return;
    }
    std::shared_ptr<Asn1Node> uiccCapabilityNode = root->Asn1GetChild(TAG_ESIM_CTX_5);
    if (uiccCapabilityNode == nullptr) {
        TELEPHONY_LOGE("uiccCapabilityNode is nullptr");
        return;
    }
    uiccCapabilityNode->Asn1AsString(euiccInfo2->uiccCapability);
}

void EsimFile::EuiccInfo2ParseTs102241Version(EuiccInfo2 *euiccInfo2, std::shared_ptr<Asn1Node> &root)
{
    if (euiccInfo2 == nullptr) {
        TELEPHONY_LOGE("euiccInfo2 is nullptr");
        return;
    }
    std::shared_ptr<Asn1Node> ts102241VersionNode = root->Asn1GetChild(TAG_ESIM_CTX_6);
    if (ts102241VersionNode == nullptr) {
        TELEPHONY_LOGE("ts102241VersionNode is nullptr");
        return;
    }
    std::vector<uint8_t> ts102241VersionRaw = {};
    uint32_t versionLen = ts102241VersionNode->Asn1AsBytes(ts102241VersionRaw);
    if (versionLen < EUICC_INFO_VERSION_MIN_LENGTH) {
        TELEPHONY_LOGE("invalid ts102241VersionNode data");
        return;
    }
    euiccInfo2->ts102241Version = MakeVersionString(ts102241VersionRaw);
}

void EsimFile::EuiccInfo2ParseGlobalPlatformVersion(EuiccInfo2 *euiccInfo2, std::shared_ptr<Asn1Node> &root)
{
    if (euiccInfo2 == nullptr) {
        TELEPHONY_LOGE("euiccInfo2 is nullptr");
        return;
    }
    std::shared_ptr<Asn1Node> globalPlatformVersionNode = root->Asn1GetChild(TAG_ESIM_CTX_7);
    if (globalPlatformVersionNode == nullptr) {
        TELEPHONY_LOGE("globalPlatformVersionNode is nullptr");
        return;
    }
    std::vector<uint8_t> globalPlatformVersionRaw = {};
    uint32_t versionLen = globalPlatformVersionNode->Asn1AsBytes(globalPlatformVersionRaw);
    if (versionLen < EUICC_INFO_VERSION_MIN_LENGTH) {
        TELEPHONY_LOGE("invalid globalplatformVersionRaw data");
        return;
    }
    euiccInfo2->globalPlatformVersion = MakeVersionString(globalPlatformVersionRaw);
}

void EsimFile::EuiccInfo2ParseRspCapability(EuiccInfo2 *euiccInfo2, std::shared_ptr<Asn1Node> &root)
{
    if (euiccInfo2 == nullptr) {
        TELEPHONY_LOGE("euiccInfo2 is nullptr");
        return;
    }
    std::shared_ptr<Asn1Node> rspCapabilityNode = root->Asn1GetChild(TAG_ESIM_CTX_8);
    if (rspCapabilityNode == nullptr) {
        TELEPHONY_LOGE("rspCapabilityNode is nullptr");
        return;
    }
    rspCapabilityNode->Asn1AsString(euiccInfo2->rspCapability);
}

void EsimFile::EuiccInfo2ParseEuiccCiPKIdListForVerification(EuiccInfo2 *euiccInfo2, std::shared_ptr<Asn1Node> &root)
{
    if (euiccInfo2 == nullptr) {
        TELEPHONY_LOGE("euiccInfo2 is nullptr");
        return;
    }
    std::shared_ptr<Asn1Node> ciPKIdListForVerificationNode = root->Asn1GetChild(TAG_ESIM_CTX_COMP_9);
    if (ciPKIdListForVerificationNode == nullptr) {
        TELEPHONY_LOGE("ciPKIdListForVerificationNode is nullptr");
        return;
    }
    ciPKIdListForVerificationNode->Asn1NodeToHexStr(euiccInfo2->euiccCiPKIdListForVerification);
}

void EsimFile::EuiccInfo2ParseEuiccCiPKIdListForSigning(EuiccInfo2 *euiccInfo2, std::shared_ptr<Asn1Node> &root)
{
    if (euiccInfo2 == nullptr) {
        TELEPHONY_LOGE("euiccInfo2 is nullptr");
        return;
    }
    std::shared_ptr<Asn1Node> euiccCiPKIdListForSigningNode = root->Asn1GetChild(TAG_ESIM_CTX_COMP_A);
    if (euiccCiPKIdListForSigningNode == nullptr) {
        TELEPHONY_LOGE("euiccCiPKIdListForSigningNode is nullptr");
        return;
    }
    euiccCiPKIdListForSigningNode->Asn1NodeToHexStr(euiccInfo2->euiccCiPKIdListForSigning);
}

void EsimFile::EuiccInfo2ParseEuiccCategory(EuiccInfo2 *euiccInfo2, std::shared_ptr<Asn1Node> &root)
{
    if (euiccInfo2 == nullptr) {
        TELEPHONY_LOGE("euiccInfo2 is nullptr");
        return;
    }
    std::shared_ptr<Asn1Node> euiccCategoryNode = root->Asn1GetChild(TAG_ESIM_CTX_B);
    if (euiccCategoryNode == nullptr) {
        TELEPHONY_LOGE("euiccCategoryNode is nullptr");
        return;
    }
    euiccInfo2->euiccCategory = euiccCategoryNode->Asn1AsInteger();
}

void EsimFile::EuiccInfo2ParsePpVersion(EuiccInfo2 *euiccInfo2, std::shared_ptr<Asn1Node> &root)
{
    if (euiccInfo2 == nullptr) {
        TELEPHONY_LOGE("euiccInfo2 is nullptr");
        return;
    }
    std::shared_ptr<Asn1Node> ppVersionNode = root->Asn1GetChild(TAG_ESIM_OCTET_STRING_TYPE);
    if (ppVersionNode == nullptr) {
        TELEPHONY_LOGE("ppVersionNode is nullptr");
        return;
    }
    std::vector<uint8_t> ppVersionNodeRaw = {};
    uint32_t versionLen = ppVersionNode->Asn1AsBytes(ppVersionNodeRaw);
    if (versionLen < EUICC_INFO_VERSION_MIN_LENGTH) {
        TELEPHONY_LOGE("invalid ppVersion data");
        return;
    }
    euiccInfo2->ppVersion = MakeVersionString(ppVersionNodeRaw);
}

bool EsimFile::ProcessIfNeedMoreResponse(IccFileData &fileData, uint32_t eventId)
{
    if (fileData.sw1 != SW1_MORE_RESPONSE) {
        return false;
    }
    ApduSimIORequestInfo reqInfo;
    RequestApduBuild codec(currentChannelId_);
    codec.BuildStoreData("");
    std::list<std::unique_ptr<ApduCommand>> lst = codec.GetCommands();
    std::unique_ptr<ApduCommand> apdCmd = std::move(lst.front());
    if (apdCmd == nullptr) {
        return false;
    }
    apdCmd->data.cla = 0;
    apdCmd->data.ins = INS_GET_MORE_RESPONSE;
    apdCmd->data.p1 = 0;
    apdCmd->data.p2 = 0;
    apdCmd->data.p3 = static_cast<int32_t>(fileData.sw2);
    CopyApdCmdToReqInfo(&reqInfo, apdCmd.get());
    AppExecFwk::InnerEvent::Pointer responseEvent = BuildCallerInfo(eventId);
    if (telRilManager_ == nullptr) {
        return false;
    }
    telRilManager_->SimTransmitApduLogicalChannel(slotId_, reqInfo, responseEvent);
    return true;
}

bool EsimFile::CombineResponseDataFinish(IccFileData &fileData)
{
    if (fileData.resultData.length() == 0) {
        return false;
    }
    recvCombineStr_ = recvCombineStr_ + fileData.resultData;
    return (fileData.sw1 == SW1_VALUE_90 && fileData.sw2 == SW2_VALUE_00);
}

bool EsimFile::RealProcsessAuthenticateServerDone(std::string combineHexStr)
{
    std::vector<uint8_t> responseByte = Asn1Utils::HexStrToBytes(combineHexStr);
    std::shared_ptr<Asn1Node> responseNode = Asn1ParseResponse(responseByte, responseByte.size());
    if (responseNode == nullptr) {
        TELEPHONY_LOGE("Asn1ParseResponse failed");
        return false;
    }
    AuthServerResponse authServerResp = { 0 };
    if (responseNode->Asn1HasChild(TAG_ESIM_CTX_COMP_1)) {
        std::shared_ptr<Asn1Node> authServerRespNode = responseNode->Asn1GetChild(TAG_ESIM_CTX_COMP_1);
        if (authServerRespNode == nullptr) {
            TELEPHONY_LOGE("authServerRespNode is nullptr");
            return false;
        }
        if (authServerRespNode->Asn1HasChild(TAG_ESIM_CTX_0) &&
            authServerRespNode->Asn1HasChild(TAG_ESIM_INTEGER_TYPE)) {
            std::shared_ptr<Asn1Node> transactionIdNode = authServerRespNode->Asn1GetChild(TAG_ESIM_CTX_0);
            std::shared_ptr<Asn1Node> errCodeNode = authServerRespNode->Asn1GetChild(TAG_ESIM_INTEGER_TYPE);
            if (transactionIdNode == nullptr || errCodeNode == nullptr) {
                TELEPHONY_LOGE("authServerRespNode failed");
                return false;
            }
            uint32_t tidByteLen = transactionIdNode->Asn1AsString(authServerResp.transactionId);
            if (tidByteLen == 0) {
                TELEPHONY_LOGE("tidByteLen is zero.");
                return false;
            }
            authServerResp.errCode = errCodeNode->Asn1AsInteger();
        } else {
            TELEPHONY_LOGE("the auth server response has no right child");
            return false;
        }
    } else {
        authServerResp.respStr = responseByte;
        authServerResp.respLength = responseByte.size();
    }
    CovertAuthToApiStruct(responseAuthenticateResult_, authServerResp);

    {
        std::lock_guard<std::mutex> lock(authenticateServerMutex_);
        isAuthenticateServerReady_ = true;
    }
    authenticateServerCv_.notify_one();
    return true;
}

bool EsimFile::ProcessAuthenticateServerDone(const AppExecFwk::InnerEvent::Pointer &event)
{
    if (event == nullptr) {
        TELEPHONY_LOGE("event is nullptr!");
        return false;
    }
    std::unique_ptr<IccFromRilMsg> rcvMsg = event->GetUniqueObject<IccFromRilMsg>();
    if (rcvMsg == nullptr) {
        TELEPHONY_LOGE("rcvMsg is nullptr");
        return false;
    }
    IccFileData &iccFileData = rcvMsg->fileData;
    if (!MergeRecvLongDataComplete(iccFileData)) {
        return true;
    }
    return RealProcsessAuthenticateServerDone(recvCombineStr_);
}

bool EsimFile::MergeRecvLongDataComplete(IccFileData &fileData)
{
    if (!CombineResponseDataFinish(fileData)) {
        if (!ProcessIfNeedMoreResponse(fileData, MSG_ESIM_AUTHENTICATE_SERVER)) {
            TELEPHONY_LOGE("try to ProcessIfNeedMoreResponse NOT done. sw1=%{public}02X", fileData.sw1);
            return false;
        }
    }
    return true;
}

void EsimFile::CovertAuthToApiStruct(ResponseEsimResult &dst, AuthServerResponse &src)
{
    dst.resultCode = static_cast<ResultState>(src.errCode);
    std::string hexStr = Asn1Utils::BytesToHexStr(src.respStr);
    dst.response = OHOS::Telephony::ToUtf16(hexStr);
}

void EsimFile::InitMemberFunc()
{
    memberFuncMap_[MSG_ESIM_OBTAIN_EUICC_INFO2_DONE] =
        [this](const AppExecFwk::InnerEvent::Pointer &event) { return ProcessObtainEuiccInfo2Done(event); };
    memberFuncMap_[MSG_ESIM_AUTHENTICATE_SERVER] =
        [this](const AppExecFwk::InnerEvent::Pointer &event) { return ProcessAuthenticateServerDone(event); };
}
} // namespace Telephony
} // namespace OHOS