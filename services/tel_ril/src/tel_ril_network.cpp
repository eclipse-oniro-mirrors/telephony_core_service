/*
 * Copyright (C) 2021-2024 Huawei Device Co., Ltd.
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

#include "tel_ril_network.h"

#include <securec.h>

#include "radio_event.h"

using namespace std;
using OHOS::IRemoteObject;
using OHOS::sptr;
namespace OHOS {
namespace Telephony {
TelRilNetwork::TelRilNetwork(int32_t slotId, sptr<HDI::Ril::V1_3::IRil> rilInterface,
    std::shared_ptr<ObserverHandler> observerHandler, std::shared_ptr<TelRilHandler> handler)
    : TelRilBase(slotId, rilInterface, observerHandler, handler)
{}

int32_t TelRilNetwork::GetSignalStrength(const AppExecFwk::InnerEvent::Pointer &response)
{
    return Request(TELEPHONY_LOG_FUNC_NAME, response, &HDI::Ril::V1_1::IRil::GetSignalStrength);
}

int32_t TelRilNetwork::GetCsRegStatus(const AppExecFwk::InnerEvent::Pointer &response)
{
    return Request(TELEPHONY_LOG_FUNC_NAME, response, &HDI::Ril::V1_1::IRil::GetCsRegStatus);
}

int32_t TelRilNetwork::GetPsRegStatus(const AppExecFwk::InnerEvent::Pointer &response)
{
    return Request(TELEPHONY_LOG_FUNC_NAME, response, &HDI::Ril::V1_1::IRil::GetPsRegStatus);
}

int32_t TelRilNetwork::GetOperatorInfo(const AppExecFwk::InnerEvent::Pointer &response)
{
    return Request(TELEPHONY_LOG_FUNC_NAME, response, &HDI::Ril::V1_1::IRil::GetOperatorInfo);
}

int32_t TelRilNetwork::GetNetworkSearchInformation(const AppExecFwk::InnerEvent::Pointer &response)
{
    return Request(TELEPHONY_LOG_FUNC_NAME, response, &HDI::Ril::V1_1::IRil::GetNetworkSearchInformation);
}

int32_t TelRilNetwork::GetNetworkSelectionMode(const AppExecFwk::InnerEvent::Pointer &response)
{
    return Request(TELEPHONY_LOG_FUNC_NAME, response, &HDI::Ril::V1_1::IRil::GetNetworkSelectionMode);
}

int32_t TelRilNetwork::SetNetworkSelectionMode(
    int32_t automaticFlag, std::string oper, const AppExecFwk::InnerEvent::Pointer &response)
{
    HDI::Ril::V1_1::SetNetworkModeInfo setNetworkModeInfo;
    setNetworkModeInfo.selectMode = automaticFlag;
    setNetworkModeInfo.oper = oper;
    return Request(
        TELEPHONY_LOG_FUNC_NAME, response, &HDI::Ril::V1_1::IRil::SetNetworkSelectionMode, setNetworkModeInfo);
}

int32_t TelRilNetwork::SetPreferredNetwork(
    int32_t preferredNetworkType, const AppExecFwk::InnerEvent::Pointer &response)
{
    return Request(TELEPHONY_LOG_FUNC_NAME, response, &HDI::Ril::V1_1::IRil::SetPreferredNetwork, preferredNetworkType);
}

int32_t TelRilNetwork::GetPreferredNetwork(const AppExecFwk::InnerEvent::Pointer &response)
{
    return Request(TELEPHONY_LOG_FUNC_NAME, response, &HDI::Ril::V1_1::IRil::GetPreferredNetwork);
}

int32_t TelRilNetwork::GetCellInfoList(const AppExecFwk::InnerEvent::Pointer &response)
{
    return Request(TELEPHONY_LOG_FUNC_NAME, response, &HDI::Ril::V1_1::IRil::GetNeighboringCellInfoList);
}

int32_t TelRilNetwork::GetCurrentCellInfo(const AppExecFwk::InnerEvent::Pointer &response)
{
    return Request(TELEPHONY_LOG_FUNC_NAME, response, &HDI::Ril::V1_1::IRil::GetCurrentCellInfo);
}

int32_t TelRilNetwork::GetPhysicalChannelConfig(const AppExecFwk::InnerEvent::Pointer &response)
{
    return Request(TELEPHONY_LOG_FUNC_NAME, response, &HDI::Ril::V1_1::IRil::GetPhysicalChannelConfig);
}

int32_t TelRilNetwork::SetLocateUpdates(RegNotifyMode mode, const AppExecFwk::InnerEvent::Pointer &response)
{
    return Request(TELEPHONY_LOG_FUNC_NAME, response, &HDI::Ril::V1_1::IRil::SetLocateUpdates,
        static_cast<HDI::Ril::V1_1::RilRegNotifyMode>(mode));
}

int32_t TelRilNetwork::SetNotificationFilter(int32_t newFilter, const AppExecFwk::InnerEvent::Pointer &response)
{
    return Request(TELEPHONY_LOG_FUNC_NAME, response, &HDI::Ril::V1_1::IRil::SetNotificationFilter, newFilter);
}

int32_t TelRilNetwork::SetDeviceState(
    int32_t deviceStateType, bool deviceStateOn, const AppExecFwk::InnerEvent::Pointer &response)
{
    return Request(
        TELEPHONY_LOG_FUNC_NAME, response, &HDI::Ril::V1_1::IRil::SetDeviceState, deviceStateType, deviceStateOn);
}

int32_t TelRilNetwork::SetNrOptionMode(int32_t mode, const AppExecFwk::InnerEvent::Pointer &response)
{
    return Request(TELEPHONY_LOG_FUNC_NAME, response, &HDI::Ril::V1_1::IRil::SetNrOptionMode, mode);
}

int32_t TelRilNetwork::GetNrOptionMode(const AppExecFwk::InnerEvent::Pointer &response)
{
    return Request(TELEPHONY_LOG_FUNC_NAME, response, &HDI::Ril::V1_1::IRil::GetNrOptionMode);
}

int32_t TelRilNetwork::GetRrcConnectionState(const AppExecFwk::InnerEvent::Pointer &response)
{
    return Request(TELEPHONY_LOG_FUNC_NAME, response, &HDI::Ril::V1_1::IRil::GetRrcConnectionState);
}

int32_t TelRilNetwork::GetNrSsbId(const AppExecFwk::InnerEvent::Pointer &response)
{
    return Request(TELEPHONY_LOG_FUNC_NAME, response, &HDI::Ril::V1_2::IRil::GetNrSsbId);
}

int32_t TelRilNetwork::SignalStrengthUpdated(const HDI::Ril::V1_1::Rssi &rssi)
{
    std::shared_ptr<Rssi> signalStrength = std::make_shared<Rssi>();
    BuildSignalStrength(signalStrength, rssi);
    return Notify<Rssi>(TELEPHONY_LOG_FUNC_NAME, signalStrength, RadioEvent::RADIO_SIGNAL_STRENGTH_UPDATE);
}

int32_t TelRilNetwork::NetworkCsRegStatusUpdated(const HDI::Ril::V1_1::CsRegStatusInfo &csRegStatusInfo)
{
    std::shared_ptr<CsRegStatusInfo> regStatusInfo = std::make_shared<CsRegStatusInfo>();
    BuildCsRegStatusInfo(regStatusInfo, csRegStatusInfo);
    return Notify<CsRegStatusInfo>(TELEPHONY_LOG_FUNC_NAME, regStatusInfo, RadioEvent::RADIO_NETWORK_STATE);
}

int32_t TelRilNetwork::NetworkPsRegStatusUpdated(const HDI::Ril::V1_1::PsRegStatusInfo &psRegStatusInfo)
{
    std::shared_ptr<PsRegStatusResultInfo> regStatusInfo = std::make_shared<PsRegStatusResultInfo>();
    BuildPsRegStatusInfo(regStatusInfo, psRegStatusInfo);
    return Notify<PsRegStatusResultInfo>(TELEPHONY_LOG_FUNC_NAME, regStatusInfo, RadioEvent::RADIO_NETWORK_STATE);
}

int32_t TelRilNetwork::NetworkTimeZoneUpdated(const std::string &timeZoneStr)
{
    return Notify<std::string>(TELEPHONY_LOG_FUNC_NAME, std::make_shared<std::string>(timeZoneStr),
        RadioEvent::RADIO_NETWORK_TIME_ZONE_UPDATE);
}

int32_t TelRilNetwork::NetworkTimeUpdated(const std::string &timeStr)
{
    return Notify<std::string>(
        TELEPHONY_LOG_FUNC_NAME, std::make_shared<std::string>(timeStr), RadioEvent::RADIO_NETWORK_TIME_UPDATE);
}

int32_t TelRilNetwork::NetworkPhyChnlCfgUpdated(const HDI::Ril::V1_1::ChannelConfigInfoList &channelConfigInfoList)
{
    std::shared_ptr<ChannelConfigInfoList> channelConfigList = std::make_shared<ChannelConfigInfoList>();
    BuildChannelConfigInfoList(channelConfigList, channelConfigInfoList);
    return Notify<ChannelConfigInfoList>(
        TELEPHONY_LOG_FUNC_NAME, channelConfigList, RadioEvent::RADIO_CHANNEL_CONFIG_UPDATE);
}

int32_t TelRilNetwork::NetworkCurrentCellUpdated(const HDI::Ril::V1_1::CellListCurrentInfo &cellListCurrentInfo)
{
    std::shared_ptr<CellListCurrentInfo> currentCellList = std::make_shared<CellListCurrentInfo>();
    BuildCurrentCellInfoList(currentCellList, cellListCurrentInfo);
    return Notify<CellListCurrentInfo>(TELEPHONY_LOG_FUNC_NAME, currentCellList, RadioEvent::RADIO_CURRENT_CELL_UPDATE);
}

int32_t TelRilNetwork::GetRrcConnectionStateUpdated(int32_t state)
{
    return Notify<Int32Parcel>(TELEPHONY_LOG_FUNC_NAME, std::make_shared<Int32Parcel>(state),
        RadioEvent::RADIO_RRC_CONNECTION_STATE_UPDATE);
}

int32_t TelRilNetwork::NetworkCurrentCellUpdated_1_1(
    const HDI::Ril::V1_1::CellListCurrentInfo_1_1 &cellListCurrentInformation)
{
    std::shared_ptr<CellListCurrentInformation> currentCellList = std::make_shared<CellListCurrentInformation>();
    BuildCurrentCellInformationList(currentCellList, cellListCurrentInformation);
    return Notify<CellListCurrentInformation>(
        TELEPHONY_LOG_FUNC_NAME, currentCellList, RadioEvent::RADIO_CURRENT_CELL_UPDATE);
}

int32_t TelRilNetwork::NetworkCurrentCellUpdated_1_2(
    const HDI::Ril::V1_2::CellListCurrentInfo_1_2 &cellListCurrentInformation)
{
    std::shared_ptr<CellListCurrentInformation> currentCellList = std::make_shared<CellListCurrentInformation>();
    BuildCurrentCellInformationList(currentCellList, cellListCurrentInformation);
    return Notify<CellListCurrentInformation>(
        TELEPHONY_LOG_FUNC_NAME, currentCellList, RadioEvent::RADIO_CURRENT_CELL_UPDATE);
}

int32_t TelRilNetwork::ResidentNetworkUpdated(const std::string &plmn)
{
    return Notify<std::string>(
        TELEPHONY_LOG_FUNC_NAME, std::make_shared<std::string>(plmn), RadioEvent::RADIO_RESIDENT_NETWORK_CHANGE);
}

int32_t TelRilNetwork::GetSignalStrengthResponse(
    const HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo, const HDI::Ril::V1_1::Rssi &rssi)
{
    std::shared_ptr<Rssi> signalStrength = std::make_shared<Rssi>();
    BuildSignalStrength(signalStrength, rssi);
    return Response<Rssi>(TELEPHONY_LOG_FUNC_NAME, responseInfo, signalStrength);
}

int32_t TelRilNetwork::GetCsRegStatusResponse(
    const HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo, const HDI::Ril::V1_1::CsRegStatusInfo &csRegStatusInfo)
{
    auto getDataFunc = [&csRegStatusInfo, this](std::shared_ptr<TelRilRequest> telRilRequest) {
        std::shared_ptr<CsRegStatusInfo> regStatusInfo = std::make_shared<CsRegStatusInfo>();
        this->BuildCsRegStatusInfo(regStatusInfo, csRegStatusInfo);
        regStatusInfo->flag = telRilRequest->pointer_->GetParam();
        return regStatusInfo;
    };
    return Response<CsRegStatusInfo>(TELEPHONY_LOG_FUNC_NAME, responseInfo, getDataFunc);
}

int32_t TelRilNetwork::GetPsRegStatusResponse(
    const HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo, const HDI::Ril::V1_1::PsRegStatusInfo &psRegStatusInfo)
{
    auto getDataFunc = [&psRegStatusInfo, this](std::shared_ptr<TelRilRequest> telRilRequest) {
        std::shared_ptr<PsRegStatusResultInfo> regStatusInfo = std::make_shared<PsRegStatusResultInfo>();
        this->BuildPsRegStatusInfo(regStatusInfo, psRegStatusInfo);
        regStatusInfo->flag = telRilRequest->pointer_->GetParam();
        return regStatusInfo;
    };
    return Response<PsRegStatusResultInfo>(TELEPHONY_LOG_FUNC_NAME, responseInfo, getDataFunc);
}

int32_t TelRilNetwork::GetOperatorInfoResponse(
    const HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo, const HDI::Ril::V1_1::OperatorInfo &operatorInfo)
{
    auto getDataFunc = [&operatorInfo, this](std::shared_ptr<TelRilRequest> telRilRequest) {
        std::shared_ptr<OperatorInfoResult> operatorResult = std::make_shared<OperatorInfoResult>();
        this->BuildOperatorInfo(operatorResult, operatorInfo);
        operatorResult->flag = telRilRequest->pointer_->GetParam();
        return operatorResult;
    };
    return Response<OperatorInfoResult>(TELEPHONY_LOG_FUNC_NAME, responseInfo, getDataFunc);
}

int32_t TelRilNetwork::GetNetworkSearchInformationResponse(const HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo,
    const HDI::Ril::V1_1::AvailableNetworkList &availableNetworkList)
{
    auto getDataFunc = [&availableNetworkList, this](std::shared_ptr<TelRilRequest> telRilRequest) {
        std::shared_ptr<AvailableNetworkList> availableNetworkInfos = std::make_shared<AvailableNetworkList>();
        this->BuildAvailableNetworkList(availableNetworkInfos, availableNetworkList);
        availableNetworkInfos->flag = telRilRequest->pointer_->GetParam();
        return availableNetworkInfos;
    };
    return Response<AvailableNetworkList>(TELEPHONY_LOG_FUNC_NAME, responseInfo, getDataFunc);
}

int32_t TelRilNetwork::GetNetworkSelectionModeResponse(const HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo,
    const HDI::Ril::V1_1::SetNetworkModeInfo &setNetworkModeInfo)
{
    auto getDataFunc = [&setNetworkModeInfo, this](std::shared_ptr<TelRilRequest> telRilRequest) {
        std::shared_ptr<SetNetworkModeInfo> networkModeInfo = std::make_shared<SetNetworkModeInfo>();
        this->BuildNetworkModeInfo(networkModeInfo, setNetworkModeInfo);
        networkModeInfo->flag = telRilRequest->pointer_->GetParam();
        return networkModeInfo;
    };
    return Response<SetNetworkModeInfo>(TELEPHONY_LOG_FUNC_NAME, responseInfo, getDataFunc);
}

int32_t TelRilNetwork::SetNetworkSelectionModeResponse(const HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo)
{
    return Response(TELEPHONY_LOG_FUNC_NAME, responseInfo);
}

int32_t TelRilNetwork::GetNeighboringCellInfoListResponse(
    const HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo, const HDI::Ril::V1_1::CellListNearbyInfo &cellInfoList)
{
    std::shared_ptr<CellListNearbyInfo> cellListNearbyInfo = std::make_shared<CellListNearbyInfo>();
    BuildNeighboringCellInfoList(cellListNearbyInfo, cellInfoList);
    return Response<CellListNearbyInfo>(TELEPHONY_LOG_FUNC_NAME, responseInfo, cellListNearbyInfo);
}

int32_t TelRilNetwork::GetNeighboringCellInfoListResponse_1_2(const HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo,
    const HDI::Ril::V1_2::CellListNearbyInfo_1_2 &cellInfoList)
{
    std::shared_ptr<CellListNearbyInfo> cellListNearbyInfo = std::make_shared<CellListNearbyInfo>();
    BuildNeighboringCellInfoList(cellListNearbyInfo, cellInfoList);
    return Response<CellListNearbyInfo>(TELEPHONY_LOG_FUNC_NAME, responseInfo, cellListNearbyInfo);
}

int32_t TelRilNetwork::GetCurrentCellInfoResponse(
    const HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo, const HDI::Ril::V1_1::CellListCurrentInfo &cellInfoList)
{
    std::shared_ptr<CellListCurrentInfo> currentCellList = std::make_shared<CellListCurrentInfo>();
    BuildCurrentCellInfoList(currentCellList, cellInfoList);
    return Response<CellListCurrentInfo>(TELEPHONY_LOG_FUNC_NAME, responseInfo, currentCellList);
}

int32_t TelRilNetwork::GetCurrentCellInfoResponse_1_1(const HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo,
    const HDI::Ril::V1_1::CellListCurrentInfo_1_1 &cellListCurrentInformation)
{
    std::shared_ptr<CellListCurrentInformation> currentCellList = std::make_shared<CellListCurrentInformation>();
    BuildCurrentCellInformationList(currentCellList, cellListCurrentInformation);
    return Response<CellListCurrentInformation>(TELEPHONY_LOG_FUNC_NAME, responseInfo, currentCellList);
}

int32_t TelRilNetwork::GetCurrentCellInfoResponse_1_2(const HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo,
    const HDI::Ril::V1_2::CellListCurrentInfo_1_2 &cellListCurrentInformation)
{
    std::shared_ptr<CellListCurrentInformation> currentCellList = std::make_shared<CellListCurrentInformation>();
    BuildCurrentCellInformationList(currentCellList, cellListCurrentInformation);
    return Response<CellListCurrentInformation>(TELEPHONY_LOG_FUNC_NAME, responseInfo, currentCellList);
}

int32_t TelRilNetwork::SetPreferredNetworkResponse(const HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo)
{
    return Response(TELEPHONY_LOG_FUNC_NAME, responseInfo);
}

int32_t TelRilNetwork::GetPreferredNetworkResponse(const HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo,
    const HDI::Ril::V1_1::PreferredNetworkTypeInfo &preferredNetworkTypeInfo)
{
    auto getDataFunc = [&preferredNetworkTypeInfo](std::shared_ptr<TelRilRequest> telRilRequest) {
        std::shared_ptr<PreferredNetworkTypeInfo> preferredTypeInfo = std::make_shared<PreferredNetworkTypeInfo>();
        preferredTypeInfo->preferredNetworkType = preferredNetworkTypeInfo.preferredNetworkType;
        preferredTypeInfo->flag = telRilRequest->pointer_->GetParam();
        return preferredTypeInfo;
    };
    return Response<PreferredNetworkTypeInfo>(TELEPHONY_LOG_FUNC_NAME, responseInfo, getDataFunc);
}

int32_t TelRilNetwork::GetPhysicalChannelConfigResponse(const HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo,
    const HDI::Ril::V1_1::ChannelConfigInfoList &channelConfigInfoList)
{
    auto getDataFunc = [&channelConfigInfoList, this](std::shared_ptr<TelRilRequest> telRilRequest) {
        std::shared_ptr<ChannelConfigInfoList> channelConfigList = std::make_shared<ChannelConfigInfoList>();
        this->BuildChannelConfigInfoList(channelConfigList, channelConfigInfoList);
        channelConfigList->flag = telRilRequest->pointer_->GetParam();
        return channelConfigList;
    };
    return Response<ChannelConfigInfoList>(TELEPHONY_LOG_FUNC_NAME, responseInfo, getDataFunc);
}

int32_t TelRilNetwork::SetLocateUpdatesResponse(const HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo)
{
    return Response(TELEPHONY_LOG_FUNC_NAME, responseInfo);
}

int32_t TelRilNetwork::SetNotificationFilterResponse(const HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo)
{
    return Response(TELEPHONY_LOG_FUNC_NAME, responseInfo);
}

int32_t TelRilNetwork::SetDeviceStateResponse(const HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo)
{
    return Response(TELEPHONY_LOG_FUNC_NAME, responseInfo);
}

int32_t TelRilNetwork::SetNrOptionModeResponse(const HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo)
{
    return Response(TELEPHONY_LOG_FUNC_NAME, responseInfo);
}

int32_t TelRilNetwork::GetNrOptionModeResponse(const HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo, int32_t mode)
{
    auto getDataFunc = [mode](std::shared_ptr<TelRilRequest> telRilRequest) {
        std::shared_ptr<NrModeInfo> nrModeInfo = std::make_shared<NrModeInfo>();
        nrModeInfo->nrMode = mode;
        nrModeInfo->flag = telRilRequest->pointer_->GetParam();
        return nrModeInfo;
    };
    return Response<NrModeInfo>(TELEPHONY_LOG_FUNC_NAME, responseInfo, getDataFunc);
}

int32_t TelRilNetwork::GetRrcConnectionStateResponse(
    const HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo, int32_t rrcConnectionState)
{
    return Response<Int32Parcel>(
        TELEPHONY_LOG_FUNC_NAME, responseInfo, std::make_shared<Int32Parcel>(rrcConnectionState));
}

int32_t TelRilNetwork::GetNrSsbIdResponse(
    const HDI::Ril::V1_1::RilRadioResponseInfo &responseInfo, const HDI::Ril::V1_2::NrCellSsbIds &nrCellSsbIds)
{
    std::shared_ptr<NrCellSsbIds> nrCellSsbIdsInfo = std::make_shared<NrCellSsbIds>();
    if (nrCellSsbIdsInfo == nullptr) {
        return Response<NrCellSsbIds>(TELEPHONY_LOG_FUNC_NAME, responseInfo, nrCellSsbIdsInfo);
    }
    TELEPHONY_LOGI("Get ssbid response from hril");
    nrCellSsbIdsInfo->arfcn = nrCellSsbIds.arfcn;
    nrCellSsbIdsInfo->cid = nrCellSsbIds.cid;
    nrCellSsbIdsInfo->pic = nrCellSsbIds.pic;
    nrCellSsbIdsInfo->rsrp = nrCellSsbIds.rsrp;
    nrCellSsbIdsInfo->sinr = nrCellSsbIds.sinr;
    nrCellSsbIdsInfo->timeAdvance = nrCellSsbIds.timeAdvance;
    nrCellSsbIdsInfo->nbCellCount = nrCellSsbIds.nbCellCount;
    for (const auto &info : nrCellSsbIds.sCellSsbList) {
        SsbIdInfo ssbIdInfo;
        ssbIdInfo.ssbId = info.ssbId;
        ssbIdInfo.rsrp = info.rsrp;
        nrCellSsbIdsInfo->sCellSsbList.push_back(ssbIdInfo);
    }
    int32_t nbCellCount =
        (nrCellSsbIdsInfo->nbCellCount > MAX_NBCELL_COUNT) ? MAX_NBCELL_COUNT : nrCellSsbIdsInfo->nbCellCount;
    for (int32_t i = 0; i < nbCellCount; i++) {
        NeighboringCellSsbInfo neighboringCellSsbInfo;
        neighboringCellSsbInfo.pci = nrCellSsbIds.nbCellSsbList[i].pci;
        neighboringCellSsbInfo.arfcn = nrCellSsbIds.nbCellSsbList[i].arfcn;
        neighboringCellSsbInfo.rsrp = nrCellSsbIds.nbCellSsbList[i].rsrp;
        neighboringCellSsbInfo.sinr = nrCellSsbIds.nbCellSsbList[i].sinr;
        for (const auto &info : nrCellSsbIds.nbCellSsbList[i].ssbIdList) {
            SsbIdInfo nbCellSsbIdInfo;
            nbCellSsbIdInfo.ssbId = info.ssbId;
            nbCellSsbIdInfo.rsrp = info.rsrp;
            neighboringCellSsbInfo.ssbIdList.push_back(nbCellSsbIdInfo);
        }
        nrCellSsbIdsInfo->nbCellSsbList.push_back(neighboringCellSsbInfo);
    }
    return Response<NrCellSsbIds>(TELEPHONY_LOG_FUNC_NAME, responseInfo, nrCellSsbIdsInfo);
}

void TelRilNetwork::BuildSignalStrength(std::shared_ptr<Rssi> signalStrength, const HDI::Ril::V1_1::Rssi &rssi)
{
    signalStrength->gw.rxlev = rssi.gw.rxlev;
    signalStrength->gw.ber = rssi.gw.ber;
    signalStrength->cdma.absoluteRssi = rssi.cdma.absoluteRssi;
    signalStrength->cdma.ecno = rssi.cdma.ecno;
    signalStrength->wcdma.rxlev = rssi.wcdma.rxlev;
    signalStrength->wcdma.ber = rssi.wcdma.ber;
    signalStrength->wcdma.ecio = rssi.wcdma.ecio;
    signalStrength->wcdma.rscp = rssi.wcdma.rscp;
    signalStrength->lte.rxlev = rssi.lte.rxlev;
    signalStrength->lte.rsrp = rssi.lte.rsrp;
    signalStrength->lte.rsrq = rssi.lte.rsrq;
    signalStrength->lte.snr = rssi.lte.snr;
    signalStrength->tdScdma.rscp = rssi.tdScdma.rscp;
    signalStrength->nr.rsrp = rssi.nr.rsrp;
    signalStrength->nr.rsrq = rssi.nr.rsrq;
    signalStrength->nr.sinr = rssi.nr.sinr;
}

void TelRilNetwork::BuildCsRegStatusInfo(
    std::shared_ptr<CsRegStatusInfo> regStatusInfo, const HDI::Ril::V1_1::CsRegStatusInfo &csRegStatusInfo)
{
    regStatusInfo->notifyType = csRegStatusInfo.notifyType;
    regStatusInfo->regStatus = static_cast<TelRilRegStatus>(csRegStatusInfo.regStatus);
    regStatusInfo->lacCode = csRegStatusInfo.lacCode;
    regStatusInfo->cellId = csRegStatusInfo.cellId;
    regStatusInfo->radioTechnology = static_cast<TelRilRadioTech>(csRegStatusInfo.radioTechnology);
}

void TelRilNetwork::BuildPsRegStatusInfo(
    std::shared_ptr<PsRegStatusResultInfo> regStatusInfo, const HDI::Ril::V1_1::PsRegStatusInfo &psRegStatusInfo)
{
    regStatusInfo->notifyType = psRegStatusInfo.notifyType;
    regStatusInfo->regStatus = static_cast<TelRilRegStatus>(psRegStatusInfo.regStatus);
    regStatusInfo->lacCode = psRegStatusInfo.lacCode;
    regStatusInfo->cellId = psRegStatusInfo.cellId;
    regStatusInfo->radioTechnology = static_cast<TelRilRadioTech>(psRegStatusInfo.radioTechnology);
    regStatusInfo->isNrAvailable = psRegStatusInfo.isNrAvailable;
    regStatusInfo->isEnDcAvailable = psRegStatusInfo.isEnDcAvailable;
    regStatusInfo->isDcNrRestricted = psRegStatusInfo.isDcNrRestricted;
}

void TelRilNetwork::BuildOperatorInfo(
    std::shared_ptr<OperatorInfoResult> operatorInfoResult, const HDI::Ril::V1_1::OperatorInfo &operatorInfo)
{
    operatorInfoResult->longName = operatorInfo.longName;
    operatorInfoResult->shortName = operatorInfo.shortName;
    operatorInfoResult->numeric = operatorInfo.numeric;
}

void TelRilNetwork::BuildAvailableNetworkList(std::shared_ptr<AvailableNetworkList> availableNetworkInfos,
    const HDI::Ril::V1_1::AvailableNetworkList &availableNetworkList)
{
    AvailableNetworkInfo operInfo = {};
    availableNetworkInfos->itemNum = availableNetworkList.itemNum;
    for (auto availableInfo : availableNetworkList.availableNetworkInfo) {
        AvailableNetworkInfo info;
        info.status = availableInfo.status;
        info.longName = availableInfo.longName;
        info.numeric = availableInfo.numeric;
        info.shortName = availableInfo.shortName;
        info.rat = availableInfo.rat;
        availableNetworkInfos->availableNetworkInfo.push_back(info);
    }
}

void TelRilNetwork::BuildNetworkModeInfo(
    std::shared_ptr<SetNetworkModeInfo> networkModeInfo, const HDI::Ril::V1_1::SetNetworkModeInfo &setNetworkModeInfo)
{
    networkModeInfo->selectMode = setNetworkModeInfo.selectMode;
    networkModeInfo->oper = setNetworkModeInfo.oper;
}

void TelRilNetwork::BuildNeighboringCellInfoList(
    std::shared_ptr<CellListNearbyInfo> cellListNearbyInfo, const HDI::Ril::V1_1::CellListNearbyInfo &cellInfoList)
{
    cellListNearbyInfo->itemNum = cellInfoList.itemNum;
    for (auto info : cellInfoList.cellNearbyInfo) {
        CellNearbyInfo cellInfo;
        FillCellNearbyInfo(cellInfo, info);
        cellListNearbyInfo->cellNearbyInfo.push_back(cellInfo);
    }
}

void TelRilNetwork::FillGsmCellNearbyInfo(CellNearbyInfo &cellInfo, const HDI::Ril::V1_1::CellNearbyInfo &info)
{
    cellInfo.ServiceCellParas.gsm.band = info.serviceCells.gsm.band;
    cellInfo.ServiceCellParas.gsm.arfcn = info.serviceCells.gsm.arfcn;
    cellInfo.ServiceCellParas.gsm.bsic = info.serviceCells.gsm.bsic;
    cellInfo.ServiceCellParas.gsm.cellId = info.serviceCells.gsm.cellId;
    cellInfo.ServiceCellParas.gsm.lac = info.serviceCells.gsm.lac;
    cellInfo.ServiceCellParas.gsm.rxlev = info.serviceCells.gsm.rxlev;
}

void TelRilNetwork::FillLteCellNearbyInfo(CellNearbyInfo &cellInfo, const HDI::Ril::V1_1::CellNearbyInfo &info)
{
    cellInfo.ServiceCellParas.lte.arfcn = info.serviceCells.lte.arfcn;
    cellInfo.ServiceCellParas.lte.pci = info.serviceCells.lte.pci;
    cellInfo.ServiceCellParas.lte.rsrp = info.serviceCells.lte.rsrp;
    cellInfo.ServiceCellParas.lte.rsrq = info.serviceCells.lte.rsrq;
    cellInfo.ServiceCellParas.lte.rxlev = info.serviceCells.lte.rxlev;
}

void TelRilNetwork::FillWcdmaCellNearbyInfo(CellNearbyInfo &cellInfo, const HDI::Ril::V1_1::CellNearbyInfo &info)
{
    cellInfo.ServiceCellParas.wcdma.arfcn = info.serviceCells.wcdma.arfcn;
    cellInfo.ServiceCellParas.wcdma.psc = info.serviceCells.wcdma.psc;
    cellInfo.ServiceCellParas.wcdma.rscp = info.serviceCells.wcdma.rscp;
    cellInfo.ServiceCellParas.wcdma.ecno = info.serviceCells.wcdma.ecno;
}

void TelRilNetwork::FillCdmaCellNearbyInfo(CellNearbyInfo &cellInfo, const HDI::Ril::V1_1::CellNearbyInfo &info)
{
    cellInfo.ServiceCellParas.cdma.systemId = info.serviceCells.cdma.systemId;
    cellInfo.ServiceCellParas.cdma.networkId = info.serviceCells.cdma.networkId;
    cellInfo.ServiceCellParas.cdma.baseId = info.serviceCells.cdma.baseId;
    cellInfo.ServiceCellParas.cdma.zoneId = info.serviceCells.cdma.zoneId;
    cellInfo.ServiceCellParas.cdma.pilotPn = info.serviceCells.cdma.pilotPn;
    cellInfo.ServiceCellParas.cdma.pilotStrength = info.serviceCells.cdma.pilotStrength;
    cellInfo.ServiceCellParas.cdma.channel = info.serviceCells.cdma.channel;
    cellInfo.ServiceCellParas.cdma.longitude = info.serviceCells.cdma.longitude;
    cellInfo.ServiceCellParas.cdma.latitude = info.serviceCells.cdma.latitude;
}

void TelRilNetwork::FillTdscdmaCellNearbyInfo(
    CellNearbyInfo &cellNearbyInfo, const HDI::Ril::V1_1::CellNearbyInfo &info)
{
    cellNearbyInfo.ServiceCellParas.tdscdma.arfcn = info.serviceCells.tdscdma.arfcn;
    cellNearbyInfo.ServiceCellParas.tdscdma.syncId = info.serviceCells.tdscdma.syncId;
    cellNearbyInfo.ServiceCellParas.tdscdma.sc = info.serviceCells.tdscdma.sc;
    cellNearbyInfo.ServiceCellParas.tdscdma.cellId = info.serviceCells.tdscdma.cellId;
    cellNearbyInfo.ServiceCellParas.tdscdma.lac = info.serviceCells.tdscdma.lac;
    cellNearbyInfo.ServiceCellParas.tdscdma.rscp = info.serviceCells.tdscdma.rscp;
    cellNearbyInfo.ServiceCellParas.tdscdma.drx = info.serviceCells.tdscdma.drx;
    cellNearbyInfo.ServiceCellParas.tdscdma.rac = info.serviceCells.tdscdma.rac;
    cellNearbyInfo.ServiceCellParas.tdscdma.cpid = info.serviceCells.tdscdma.cpid;
}

void TelRilNetwork::FillNrCellNearbyInfo(CellNearbyInfo &cellInfo, const HDI::Ril::V1_1::CellNearbyInfo &info)
{
    cellInfo.ServiceCellParas.nr.nrArfcn = info.serviceCells.nr.nrArfcn;
    cellInfo.ServiceCellParas.nr.pci = info.serviceCells.nr.pci;
    cellInfo.ServiceCellParas.nr.tac = info.serviceCells.nr.tac;
    cellInfo.ServiceCellParas.nr.nci = info.serviceCells.nr.nci;
}

void TelRilNetwork::FillCellNearbyInfo(CellNearbyInfo &cellInfo, const HDI::Ril::V1_1::CellNearbyInfo &info)
{
    cellInfo.ratType = info.ratType;
    switch (info.ratType) {
        case NETWORK_TYPE_GSM:
            FillGsmCellNearbyInfo(cellInfo, info);
            break;
        case NETWORK_TYPE_LTE:
            FillLteCellNearbyInfo(cellInfo, info);
            break;
        case NETWORK_TYPE_WCDMA:
            FillWcdmaCellNearbyInfo(cellInfo, info);
            break;
        case NETWORK_TYPE_CDMA:
            FillCdmaCellNearbyInfo(cellInfo, info);
            break;
        case NETWORK_TYPE_TDSCDMA:
            FillTdscdmaCellNearbyInfo(cellInfo, info);
            break;
        case NETWORK_TYPE_NR:
            FillNrCellNearbyInfo(cellInfo, info);
            break;
        default:
            TELEPHONY_LOGE("TelRilNetwork::FillCellNearbyInfo invalid ratType");
            break;
    }
}

void TelRilNetwork::BuildNeighboringCellInfoList(
    std::shared_ptr<CellListNearbyInfo> cellListNearbyInfo, const HDI::Ril::V1_2::CellListNearbyInfo_1_2 &cellInfoList)
{
    cellListNearbyInfo->itemNum = cellInfoList.itemNum;
    for (auto info : cellInfoList.cellNearbyInfo) {
        CellNearbyInfo cellInfo;
        FillCellNearbyInfo(cellInfo, info);
        cellListNearbyInfo->cellNearbyInfo.push_back(cellInfo);
    }
}

void TelRilNetwork::FillCellNearbyInfo(CellNearbyInfo &cellInfo, const HDI::Ril::V1_2::CellNearbyInfo_1_2 &info)
{
    cellInfo.ratType = info.ratType;
    switch (info.ratType) {
        case NETWORK_TYPE_GSM:
            FillGsmCellNearbyInfo(cellInfo, info);
            break;
        case NETWORK_TYPE_LTE:
            FillLteCellNearbyInfo(cellInfo, info);
            break;
        case NETWORK_TYPE_WCDMA:
            FillWcdmaCellNearbyInfo(cellInfo, info);
            break;
        case NETWORK_TYPE_CDMA:
            FillCdmaCellNearbyInfo(cellInfo, info);
            break;
        case NETWORK_TYPE_TDSCDMA:
            FillTdscdmaCellNearbyInfo(cellInfo, info);
            break;
        case NETWORK_TYPE_NR:
            FillNrCellNearbyInfo(cellInfo, info);
            break;
        default:
            TELEPHONY_LOGE("invalid ratType");
            break;
    }
}

void TelRilNetwork::FillGsmCellNearbyInfo(CellNearbyInfo &cellInfo, const HDI::Ril::V1_2::CellNearbyInfo_1_2 &info)
{
    cellInfo.ServiceCellParas.gsm.band = info.serviceCells.gsm.band;
    cellInfo.ServiceCellParas.gsm.arfcn = info.serviceCells.gsm.arfcn;
    cellInfo.ServiceCellParas.gsm.bsic = info.serviceCells.gsm.bsic;
    cellInfo.ServiceCellParas.gsm.cellId = info.serviceCells.gsm.cellId;
    cellInfo.ServiceCellParas.gsm.lac = info.serviceCells.gsm.lac;
    cellInfo.ServiceCellParas.gsm.rxlev = info.serviceCells.gsm.rxlev;
}

void TelRilNetwork::FillLteCellNearbyInfo(CellNearbyInfo &cellInfo, const HDI::Ril::V1_2::CellNearbyInfo_1_2 &info)
{
    cellInfo.ServiceCellParas.lte.arfcn = info.serviceCells.lte.arfcn;
    cellInfo.ServiceCellParas.lte.pci = info.serviceCells.lte.pci;
    cellInfo.ServiceCellParas.lte.rsrp = info.serviceCells.lte.rsrp;
    cellInfo.ServiceCellParas.lte.rsrq = info.serviceCells.lte.rsrq;
    cellInfo.ServiceCellParas.lte.rxlev = info.serviceCells.lte.rxlev;
}

void TelRilNetwork::FillWcdmaCellNearbyInfo(CellNearbyInfo &cellInfo, const HDI::Ril::V1_2::CellNearbyInfo_1_2 &info)
{
    cellInfo.ServiceCellParas.wcdma.arfcn = info.serviceCells.wcdma.arfcn;
    cellInfo.ServiceCellParas.wcdma.psc = info.serviceCells.wcdma.psc;
    cellInfo.ServiceCellParas.wcdma.rscp = info.serviceCells.wcdma.rscp;
    cellInfo.ServiceCellParas.wcdma.ecno = info.serviceCells.wcdma.ecno;
}

void TelRilNetwork::FillCdmaCellNearbyInfo(CellNearbyInfo &cellInfo, const HDI::Ril::V1_2::CellNearbyInfo_1_2 &info)
{
    cellInfo.ServiceCellParas.cdma.systemId = info.serviceCells.cdma.systemId;
    cellInfo.ServiceCellParas.cdma.networkId = info.serviceCells.cdma.networkId;
    cellInfo.ServiceCellParas.cdma.baseId = info.serviceCells.cdma.baseId;
    cellInfo.ServiceCellParas.cdma.zoneId = info.serviceCells.cdma.zoneId;
    cellInfo.ServiceCellParas.cdma.pilotPn = info.serviceCells.cdma.pilotPn;
    cellInfo.ServiceCellParas.cdma.pilotStrength = info.serviceCells.cdma.pilotStrength;
    cellInfo.ServiceCellParas.cdma.channel = info.serviceCells.cdma.channel;
    cellInfo.ServiceCellParas.cdma.longitude = info.serviceCells.cdma.longitude;
    cellInfo.ServiceCellParas.cdma.latitude = info.serviceCells.cdma.latitude;
}

void TelRilNetwork::FillTdscdmaCellNearbyInfo(
    CellNearbyInfo &cellNearbyInfo, const HDI::Ril::V1_2::CellNearbyInfo_1_2 &info)
{
    cellNearbyInfo.ServiceCellParas.tdscdma.arfcn = info.serviceCells.tdscdma.arfcn;
    cellNearbyInfo.ServiceCellParas.tdscdma.syncId = info.serviceCells.tdscdma.syncId;
    cellNearbyInfo.ServiceCellParas.tdscdma.sc = info.serviceCells.tdscdma.sc;
    cellNearbyInfo.ServiceCellParas.tdscdma.cellId = info.serviceCells.tdscdma.cellId;
    cellNearbyInfo.ServiceCellParas.tdscdma.lac = info.serviceCells.tdscdma.lac;
    cellNearbyInfo.ServiceCellParas.tdscdma.rscp = info.serviceCells.tdscdma.rscp;
    cellNearbyInfo.ServiceCellParas.tdscdma.drx = info.serviceCells.tdscdma.drx;
    cellNearbyInfo.ServiceCellParas.tdscdma.rac = info.serviceCells.tdscdma.rac;
    cellNearbyInfo.ServiceCellParas.tdscdma.cpid = info.serviceCells.tdscdma.cpid;
}

void TelRilNetwork::FillNrCellNearbyInfo(CellNearbyInfo &cellInfo, const HDI::Ril::V1_2::CellNearbyInfo_1_2 &info)
{
    cellInfo.ServiceCellParas.nr.nrArfcn = info.serviceCells.nr.nrArfcn;
    cellInfo.ServiceCellParas.nr.pci = info.serviceCells.nr.pci;
    cellInfo.ServiceCellParas.nr.tac = info.serviceCells.nr.tac;
    cellInfo.ServiceCellParas.nr.nci = info.serviceCells.nr.nci;
}

void TelRilNetwork::BuildCurrentCellInfoList(
    std::shared_ptr<CellListCurrentInfo> currentCellList, const HDI::Ril::V1_1::CellListCurrentInfo &cellInfoList)
{
    currentCellList->itemNum = cellInfoList.itemNum;
    for (auto &info : cellInfoList.cellCurrentInfo) {
        CurrentCellInfo cellInfo;
        FillCurrentCellInfo(cellInfo, info);
        currentCellList->cellCurrentInfo.push_back(cellInfo);
    }
}

void TelRilNetwork::FillCurrentCellInfo(CurrentCellInfo &cellInfo, const HDI::Ril::V1_1::CurrentCellInfo &info)
{
    cellInfo.ratType = info.ratType;
    cellInfo.mcc = info.mcc;
    cellInfo.mnc = info.mnc;
    switch (info.ratType) {
        case NETWORK_TYPE_GSM:
            FillGsmCurrentCellInfo(cellInfo, info);
            break;
        case NETWORK_TYPE_LTE:
            FillLteCurrentCellInfo(cellInfo, info);
            break;
        case NETWORK_TYPE_WCDMA:
            FillWcdmaCurrentCellInfo(cellInfo, info);
            break;
        case NETWORK_TYPE_CDMA:
            FillCdmaCurrentCellInfo(cellInfo, info);
            break;
        case NETWORK_TYPE_TDSCDMA:
            FillTdscdmaCurrentCellInfo(cellInfo, info);
            break;
        case NETWORK_TYPE_NR:
            FillNrCurrentCellInfo(cellInfo, info);
            break;
        default:
            TELEPHONY_LOGE("TelRilNetwork::FillCurrentCellInfo invalid ratType");
            break;
    }
}

void TelRilNetwork::FillGsmCurrentCellInfo(CurrentCellInfo &cellInfo, const HDI::Ril::V1_1::CurrentCellInfo &info)
{
    cellInfo.ServiceCellParas.gsm.band = info.serviceCells.gsm.band;
    cellInfo.ServiceCellParas.gsm.arfcn = info.serviceCells.gsm.arfcn;
    cellInfo.ServiceCellParas.gsm.bsic = info.serviceCells.gsm.bsic;
    cellInfo.ServiceCellParas.gsm.cellId = info.serviceCells.gsm.cellId;
    cellInfo.ServiceCellParas.gsm.lac = info.serviceCells.gsm.lac;
    cellInfo.ServiceCellParas.gsm.rxlev = info.serviceCells.gsm.rxlev;
    cellInfo.ServiceCellParas.gsm.rxQuality = info.serviceCells.gsm.rxQuality;
    cellInfo.ServiceCellParas.gsm.ta = info.serviceCells.gsm.ta;
}

void TelRilNetwork::FillLteCurrentCellInfo(CurrentCellInfo &cellInfo, const HDI::Ril::V1_1::CurrentCellInfo &info)
{
    cellInfo.ServiceCellParas.lte.arfcn = info.serviceCells.lte.arfcn;
    cellInfo.ServiceCellParas.lte.cellId = info.serviceCells.lte.cellId;
    cellInfo.ServiceCellParas.lte.pci = info.serviceCells.lte.pci;
    cellInfo.ServiceCellParas.lte.tac = info.serviceCells.lte.tac;
    cellInfo.ServiceCellParas.lte.rsrp = info.serviceCells.lte.rsrp;
    cellInfo.ServiceCellParas.lte.rsrq = info.serviceCells.lte.rsrq;
    cellInfo.ServiceCellParas.lte.rssi = info.serviceCells.lte.rssi;
}

void TelRilNetwork::FillWcdmaCurrentCellInfo(CurrentCellInfo &cellInfo, const HDI::Ril::V1_1::CurrentCellInfo &info)
{
    cellInfo.ServiceCellParas.wcdma.arfcn = info.serviceCells.wcdma.arfcn;
    cellInfo.ServiceCellParas.wcdma.cellId = info.serviceCells.wcdma.cellId;
    cellInfo.ServiceCellParas.wcdma.psc = info.serviceCells.wcdma.psc;
    cellInfo.ServiceCellParas.wcdma.lac = info.serviceCells.wcdma.lac;
    cellInfo.ServiceCellParas.wcdma.rxlev = info.serviceCells.wcdma.rxlev;
    cellInfo.ServiceCellParas.wcdma.rscp = info.serviceCells.wcdma.rscp;
    cellInfo.ServiceCellParas.wcdma.ecno = info.serviceCells.wcdma.ecno;
    cellInfo.ServiceCellParas.wcdma.ura = info.serviceCells.wcdma.ura;
    cellInfo.ServiceCellParas.wcdma.drx = info.serviceCells.wcdma.drx;
}

void TelRilNetwork::FillCdmaCurrentCellInfo(CurrentCellInfo &cellInfo, const HDI::Ril::V1_1::CurrentCellInfo &info)
{
    cellInfo.ServiceCellParas.cdma.systemId = info.serviceCells.cdma.systemId;
    cellInfo.ServiceCellParas.cdma.networkId = info.serviceCells.cdma.networkId;
    cellInfo.ServiceCellParas.cdma.baseId = info.serviceCells.cdma.baseId;
    cellInfo.ServiceCellParas.cdma.zoneId = info.serviceCells.cdma.zoneId;
    cellInfo.ServiceCellParas.cdma.pilotPn = info.serviceCells.cdma.pilotPn;
    cellInfo.ServiceCellParas.cdma.pilotStrength = info.serviceCells.cdma.pilotStrength;
    cellInfo.ServiceCellParas.cdma.channel = info.serviceCells.cdma.channel;
    cellInfo.ServiceCellParas.cdma.longitude = info.serviceCells.cdma.longitude;
    cellInfo.ServiceCellParas.cdma.latitude = info.serviceCells.cdma.latitude;
}

void TelRilNetwork::FillTdscdmaCurrentCellInfo(
    CurrentCellInfo &currentCellInfo, const HDI::Ril::V1_1::CurrentCellInfo &info)
{
    currentCellInfo.ServiceCellParas.tdscdma.arfcn = info.serviceCells.tdscdma.arfcn;
    currentCellInfo.ServiceCellParas.tdscdma.syncId = info.serviceCells.tdscdma.syncId;
    currentCellInfo.ServiceCellParas.tdscdma.sc = info.serviceCells.tdscdma.sc;
    currentCellInfo.ServiceCellParas.tdscdma.cellId = info.serviceCells.tdscdma.cellId;
    currentCellInfo.ServiceCellParas.tdscdma.lac = info.serviceCells.tdscdma.lac;
    currentCellInfo.ServiceCellParas.tdscdma.rscp = info.serviceCells.tdscdma.rscp;
    currentCellInfo.ServiceCellParas.tdscdma.drx = info.serviceCells.tdscdma.drx;
    currentCellInfo.ServiceCellParas.tdscdma.rac = info.serviceCells.tdscdma.rac;
    currentCellInfo.ServiceCellParas.tdscdma.cpid = info.serviceCells.tdscdma.cpid;
}

void TelRilNetwork::FillNrCurrentCellInfo(CurrentCellInfo &cellInfo, const HDI::Ril::V1_1::CurrentCellInfo &info)
{
    cellInfo.ServiceCellParas.nr.nrArfcn = info.serviceCells.nr.nrArfcn;
    cellInfo.ServiceCellParas.nr.pci = info.serviceCells.nr.pci;
    cellInfo.ServiceCellParas.nr.tac = info.serviceCells.nr.tac;
    cellInfo.ServiceCellParas.nr.nci = info.serviceCells.nr.nci;
}

void TelRilNetwork::BuildCurrentCellInformationList(std::shared_ptr<CellListCurrentInformation> currentCellList,
    const HDI::Ril::V1_1::CellListCurrentInfo_1_1 &cellInfoList)
{
    currentCellList->itemNum = cellInfoList.itemNum;
    for (auto &info : cellInfoList.cellCurrentInfo) {
        CurrentCellInformation cellInfo;
        FillCurrentCellInformation(cellInfo, info);
        currentCellList->cellCurrentInfo.push_back(cellInfo);
    }
}

void TelRilNetwork::FillCurrentCellInformation(
    CurrentCellInformation &cellInfo, const HDI::Ril::V1_1::CurrentCellInfo_1_1 &info)
{
    cellInfo.ratType = info.ratType;
    cellInfo.mcc = info.mcc;
    cellInfo.mnc = info.mnc;
    switch (info.ratType) {
        case NETWORK_TYPE_GSM:
            FillGsmCurrentCellInformation(cellInfo, info);
            break;
        case NETWORK_TYPE_LTE:
            FillLteCurrentCellInformation(cellInfo, info);
            break;
        case NETWORK_TYPE_WCDMA:
            FillWcdmaCurrentCellInformation(cellInfo, info);
            break;
        case NETWORK_TYPE_CDMA:
            FillCdmaCurrentCellInformation(cellInfo, info);
            break;
        case NETWORK_TYPE_TDSCDMA:
            FillTdscdmaCurrentCellInformation(cellInfo, info);
            break;
        case NETWORK_TYPE_NR:
            FillNrCurrentCellInformation(cellInfo, info);
            break;
        default:
            TELEPHONY_LOGE("TelRilNetwork::FillCurrentCellInformation invalid ratType");
            break;
    }
}

void TelRilNetwork::FillGsmCurrentCellInformation(
    CurrentCellInformation &cellInfo, const HDI::Ril::V1_1::CurrentCellInfo_1_1 &info)
{
    cellInfo.ServiceCellParas.gsm.band = info.serviceCells.gsm.band;
    cellInfo.ServiceCellParas.gsm.arfcn = info.serviceCells.gsm.arfcn;
    cellInfo.ServiceCellParas.gsm.bsic = info.serviceCells.gsm.bsic;
    cellInfo.ServiceCellParas.gsm.cellId = info.serviceCells.gsm.cellId;
    cellInfo.ServiceCellParas.gsm.lac = info.serviceCells.gsm.lac;
    cellInfo.ServiceCellParas.gsm.rxlev = info.serviceCells.gsm.rxlev;
    cellInfo.ServiceCellParas.gsm.rxQuality = info.serviceCells.gsm.rxQuality;
    cellInfo.ServiceCellParas.gsm.ta = info.serviceCells.gsm.ta;
}

void TelRilNetwork::FillLteCurrentCellInformation(
    CurrentCellInformation &cellInfo, const HDI::Ril::V1_1::CurrentCellInfo_1_1 &info)
{
    cellInfo.ServiceCellParas.lte.arfcn = info.serviceCells.lte.arfcn;
    cellInfo.ServiceCellParas.lte.cellId = info.serviceCells.lte.cellId;
    cellInfo.ServiceCellParas.lte.pci = info.serviceCells.lte.pci;
    cellInfo.ServiceCellParas.lte.tac = info.serviceCells.lte.tac;
    cellInfo.ServiceCellParas.lte.rsrp = info.serviceCells.lte.rsrp;
    cellInfo.ServiceCellParas.lte.rsrq = info.serviceCells.lte.rsrq;
    cellInfo.ServiceCellParas.lte.rssi = info.serviceCells.lte.rssi;
}

void TelRilNetwork::FillWcdmaCurrentCellInformation(
    CurrentCellInformation &cellInfo, const HDI::Ril::V1_1::CurrentCellInfo_1_1 &info)
{
    cellInfo.ServiceCellParas.wcdma.arfcn = info.serviceCells.wcdma.arfcn;
    cellInfo.ServiceCellParas.wcdma.cellId = info.serviceCells.wcdma.cellId;
    cellInfo.ServiceCellParas.wcdma.psc = info.serviceCells.wcdma.psc;
    cellInfo.ServiceCellParas.wcdma.lac = info.serviceCells.wcdma.lac;
    cellInfo.ServiceCellParas.wcdma.rxlev = info.serviceCells.wcdma.rxlev;
    cellInfo.ServiceCellParas.wcdma.rscp = info.serviceCells.wcdma.rscp;
    cellInfo.ServiceCellParas.wcdma.ecno = info.serviceCells.wcdma.ecno;
    cellInfo.ServiceCellParas.wcdma.ura = info.serviceCells.wcdma.ura;
    cellInfo.ServiceCellParas.wcdma.drx = info.serviceCells.wcdma.drx;
}

void TelRilNetwork::FillCdmaCurrentCellInformation(
    CurrentCellInformation &cellInfo, const HDI::Ril::V1_1::CurrentCellInfo_1_1 &info)
{
    cellInfo.ServiceCellParas.cdma.systemId = info.serviceCells.cdma.systemId;
    cellInfo.ServiceCellParas.cdma.networkId = info.serviceCells.cdma.networkId;
    cellInfo.ServiceCellParas.cdma.baseId = info.serviceCells.cdma.baseId;
    cellInfo.ServiceCellParas.cdma.zoneId = info.serviceCells.cdma.zoneId;
    cellInfo.ServiceCellParas.cdma.pilotPn = info.serviceCells.cdma.pilotPn;
    cellInfo.ServiceCellParas.cdma.pilotStrength = info.serviceCells.cdma.pilotStrength;
    cellInfo.ServiceCellParas.cdma.channel = info.serviceCells.cdma.channel;
    cellInfo.ServiceCellParas.cdma.longitude = info.serviceCells.cdma.longitude;
    cellInfo.ServiceCellParas.cdma.latitude = info.serviceCells.cdma.latitude;
}

void TelRilNetwork::FillTdscdmaCurrentCellInformation(
    CurrentCellInformation &cellInfo, const HDI::Ril::V1_1::CurrentCellInfo_1_1 &info)
{
    cellInfo.ServiceCellParas.tdscdma.arfcn = info.serviceCells.tdscdma.arfcn;
    cellInfo.ServiceCellParas.tdscdma.syncId = info.serviceCells.tdscdma.syncId;
    cellInfo.ServiceCellParas.tdscdma.sc = info.serviceCells.tdscdma.sc;
    cellInfo.ServiceCellParas.tdscdma.cellId = info.serviceCells.tdscdma.cellId;
    cellInfo.ServiceCellParas.tdscdma.lac = info.serviceCells.tdscdma.lac;
    cellInfo.ServiceCellParas.tdscdma.rscp = info.serviceCells.tdscdma.rscp;
    cellInfo.ServiceCellParas.tdscdma.drx = info.serviceCells.tdscdma.drx;
    cellInfo.ServiceCellParas.tdscdma.rac = info.serviceCells.tdscdma.rac;
    cellInfo.ServiceCellParas.tdscdma.cpid = info.serviceCells.tdscdma.cpid;
}

void TelRilNetwork::FillNrCurrentCellInformation(
    CurrentCellInformation &cellInfo, const HDI::Ril::V1_1::CurrentCellInfo_1_1 &info)
{
    cellInfo.ServiceCellParas.nr.nrArfcn = info.serviceCells.nr.nrArfcn;
    cellInfo.ServiceCellParas.nr.pci = info.serviceCells.nr.pci;
    cellInfo.ServiceCellParas.nr.tac = info.serviceCells.nr.tac;
    cellInfo.ServiceCellParas.nr.nci = info.serviceCells.nr.nci;
    cellInfo.ServiceCellParas.nr.rsrp = info.serviceCells.nr.rsrp;
    cellInfo.ServiceCellParas.nr.rsrq = info.serviceCells.nr.rsrq;
}

void TelRilNetwork::BuildCurrentCellInformationList(std::shared_ptr<CellListCurrentInformation> currentCellList,
    const HDI::Ril::V1_2::CellListCurrentInfo_1_2 &cellInfoList)
{
    currentCellList->itemNum = cellInfoList.itemNum;
    for (auto &info : cellInfoList.cellCurrentInfo) {
        CurrentCellInformation cellInfo;
        FillCurrentCellInformation(cellInfo, info);
        currentCellList->cellCurrentInfo.push_back(cellInfo);
    }
}

void TelRilNetwork::FillCurrentCellInformation(
    CurrentCellInformation &cellInfo, const HDI::Ril::V1_2::CurrentCellInfo_1_2 &info)
{
    cellInfo.ratType = info.ratType;
    cellInfo.mcc = info.mcc;
    cellInfo.mnc = info.mnc;
    switch (info.ratType) {
        case NETWORK_TYPE_GSM:
            FillGsmCurrentCellInformation(cellInfo, info);
            break;
        case NETWORK_TYPE_LTE:
            FillLteCurrentCellInformation(cellInfo, info);
            break;
        case NETWORK_TYPE_WCDMA:
            FillWcdmaCurrentCellInformation(cellInfo, info);
            break;
        case NETWORK_TYPE_CDMA:
            FillCdmaCurrentCellInformation(cellInfo, info);
            break;
        case NETWORK_TYPE_TDSCDMA:
            FillTdscdmaCurrentCellInformation(cellInfo, info);
            break;
        case NETWORK_TYPE_NR:
            FillNrCurrentCellInformation(cellInfo, info);
            break;
        default:
            TELEPHONY_LOGE("invalid ratType");
            break;
    }
}

void TelRilNetwork::FillGsmCurrentCellInformation(
    CurrentCellInformation &cellInfo, const HDI::Ril::V1_2::CurrentCellInfo_1_2 &info)
{
    cellInfo.ServiceCellParas.gsm.band = info.serviceCells.gsm.band;
    cellInfo.ServiceCellParas.gsm.arfcn = info.serviceCells.gsm.arfcn;
    cellInfo.ServiceCellParas.gsm.bsic = info.serviceCells.gsm.bsic;
    cellInfo.ServiceCellParas.gsm.cellId = info.serviceCells.gsm.cellId;
    cellInfo.ServiceCellParas.gsm.lac = info.serviceCells.gsm.lac;
    cellInfo.ServiceCellParas.gsm.rxlev = info.serviceCells.gsm.rxlev;
    cellInfo.ServiceCellParas.gsm.rxQuality = info.serviceCells.gsm.rxQuality;
    cellInfo.ServiceCellParas.gsm.ta = info.serviceCells.gsm.ta;
}

void TelRilNetwork::FillLteCurrentCellInformation(
    CurrentCellInformation &cellInfo, const HDI::Ril::V1_2::CurrentCellInfo_1_2 &info)
{
    cellInfo.ServiceCellParas.lte.arfcn = info.serviceCells.lte.arfcn;
    cellInfo.ServiceCellParas.lte.cellId = info.serviceCells.lte.cellId;
    cellInfo.ServiceCellParas.lte.pci = info.serviceCells.lte.pci;
    cellInfo.ServiceCellParas.lte.tac = info.serviceCells.lte.tac;
    cellInfo.ServiceCellParas.lte.rsrp = info.serviceCells.lte.rsrp;
    cellInfo.ServiceCellParas.lte.rsrq = info.serviceCells.lte.rsrq;
    cellInfo.ServiceCellParas.lte.rssi = info.serviceCells.lte.rssi;
}

void TelRilNetwork::FillWcdmaCurrentCellInformation(
    CurrentCellInformation &cellInfo, const HDI::Ril::V1_2::CurrentCellInfo_1_2 &info)
{
    cellInfo.ServiceCellParas.wcdma.arfcn = info.serviceCells.wcdma.arfcn;
    cellInfo.ServiceCellParas.wcdma.cellId = info.serviceCells.wcdma.cellId;
    cellInfo.ServiceCellParas.wcdma.psc = info.serviceCells.wcdma.psc;
    cellInfo.ServiceCellParas.wcdma.lac = info.serviceCells.wcdma.lac;
    cellInfo.ServiceCellParas.wcdma.rxlev = info.serviceCells.wcdma.rxlev;
    cellInfo.ServiceCellParas.wcdma.rscp = info.serviceCells.wcdma.rscp;
    cellInfo.ServiceCellParas.wcdma.ecno = info.serviceCells.wcdma.ecno;
    cellInfo.ServiceCellParas.wcdma.ura = info.serviceCells.wcdma.ura;
    cellInfo.ServiceCellParas.wcdma.drx = info.serviceCells.wcdma.drx;
}

void TelRilNetwork::FillCdmaCurrentCellInformation(
    CurrentCellInformation &cellInfo, const HDI::Ril::V1_2::CurrentCellInfo_1_2 &info)
{
    cellInfo.ServiceCellParas.cdma.systemId = info.serviceCells.cdma.systemId;
    cellInfo.ServiceCellParas.cdma.networkId = info.serviceCells.cdma.networkId;
    cellInfo.ServiceCellParas.cdma.baseId = info.serviceCells.cdma.baseId;
    cellInfo.ServiceCellParas.cdma.zoneId = info.serviceCells.cdma.zoneId;
    cellInfo.ServiceCellParas.cdma.pilotPn = info.serviceCells.cdma.pilotPn;
    cellInfo.ServiceCellParas.cdma.pilotStrength = info.serviceCells.cdma.pilotStrength;
    cellInfo.ServiceCellParas.cdma.channel = info.serviceCells.cdma.channel;
    cellInfo.ServiceCellParas.cdma.longitude = info.serviceCells.cdma.longitude;
    cellInfo.ServiceCellParas.cdma.latitude = info.serviceCells.cdma.latitude;
}

void TelRilNetwork::FillTdscdmaCurrentCellInformation(
    CurrentCellInformation &cellInfo, const HDI::Ril::V1_2::CurrentCellInfo_1_2 &info)
{
    cellInfo.ServiceCellParas.tdscdma.arfcn = info.serviceCells.tdscdma.arfcn;
    cellInfo.ServiceCellParas.tdscdma.syncId = info.serviceCells.tdscdma.syncId;
    cellInfo.ServiceCellParas.tdscdma.sc = info.serviceCells.tdscdma.sc;
    cellInfo.ServiceCellParas.tdscdma.cellId = info.serviceCells.tdscdma.cellId;
    cellInfo.ServiceCellParas.tdscdma.lac = info.serviceCells.tdscdma.lac;
    cellInfo.ServiceCellParas.tdscdma.rscp = info.serviceCells.tdscdma.rscp;
    cellInfo.ServiceCellParas.tdscdma.drx = info.serviceCells.tdscdma.drx;
    cellInfo.ServiceCellParas.tdscdma.rac = info.serviceCells.tdscdma.rac;
    cellInfo.ServiceCellParas.tdscdma.cpid = info.serviceCells.tdscdma.cpid;
}

void TelRilNetwork::FillNrCurrentCellInformation(
    CurrentCellInformation &cellInfo, const HDI::Ril::V1_2::CurrentCellInfo_1_2 &info)
{
    cellInfo.ServiceCellParas.nr.nrArfcn = info.serviceCells.nr.nrArfcn;
    cellInfo.ServiceCellParas.nr.pci = info.serviceCells.nr.pci;
    cellInfo.ServiceCellParas.nr.tac = info.serviceCells.nr.tac;
    cellInfo.ServiceCellParas.nr.nci = info.serviceCells.nr.nci;
    cellInfo.ServiceCellParas.nr.rsrp = info.serviceCells.nr.rsrp;
    cellInfo.ServiceCellParas.nr.rsrq = info.serviceCells.nr.rsrq;
}

void TelRilNetwork::BuildChannelConfigInfoList(std::shared_ptr<ChannelConfigInfoList> channelConfigList,
    const HDI::Ril::V1_1::ChannelConfigInfoList &channelConfigInfoList)
{
    channelConfigList->itemNum = channelConfigInfoList.itemNum;
    for (int32_t i = 0; i < channelConfigList->itemNum; i++) {
        PhysicalChannelConfig phyChnlCfg;
        phyChnlCfg.cellConnStatus =
            static_cast<CellConnectionStatus>(channelConfigInfoList.channelConfigInfos[i].cellConnStatus);
        phyChnlCfg.cellBandwidthDownlinkKhz = channelConfigInfoList.channelConfigInfos[i].cellBandwidthDownlinkKhz;
        phyChnlCfg.cellBandwidthUplinkKhz = channelConfigInfoList.channelConfigInfos[i].cellBandwidthUplinkKhz;
        phyChnlCfg.ratType = static_cast<TelRilRadioTech>(channelConfigInfoList.channelConfigInfos[i].ratType);
        phyChnlCfg.freqRange = channelConfigInfoList.channelConfigInfos[i].freqRange;
        phyChnlCfg.downlinkChannelNum = channelConfigInfoList.channelConfigInfos[i].downlinkChannelNum;
        phyChnlCfg.uplinkChannelNum = channelConfigInfoList.channelConfigInfos[i].uplinkChannelNum;
        phyChnlCfg.physicalCellId = channelConfigInfoList.channelConfigInfos[i].physicalCellId;
        phyChnlCfg.contextIdNum = channelConfigInfoList.channelConfigInfos[i].contextIdNum;
        for (int32_t j = 0; j < phyChnlCfg.contextIdNum; j++) {
            phyChnlCfg.contextIds.push_back(channelConfigInfoList.channelConfigInfos[i].contextIds[j]);
        }
        channelConfigList->channelConfigInfos.push_back(phyChnlCfg);
    }
}
} // namespace Telephony
} // namespace OHOS
