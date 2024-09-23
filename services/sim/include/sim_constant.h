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

#ifndef OHOS_SIM_CONSTANT_H
#define OHOS_SIM_CONSTANT_H

namespace OHOS {
namespace Telephony {
enum ControllerType {
    ICC_CONTROLLER_REQ_BASE = 0x16EFF0,
    ICC_CONTROLLER_REQ_NONE,
    ICC_CONTROLLER_REQ_SEND_RESPONSE
};

// mnc Length type
enum MncType {
    UNINITIALIZED_MNC = -1,
    UNKNOWN_MNC = 0,
};

enum MWIType {
    MSG_SIM_MWI = 0,
    MSG_SIM_CFI = 1,
    MSG_SIM_SPN = 2
};

enum StateMessage {
    MSG_SIM_OBTAIN_ICC_FILE_DONE = 100,
    MSG_ICC_REFRESH = 31,
    MSG_SIM_APP_READY = 1
};

enum CallForwardType {
    ICC_CALL_FORWARD_TYPE_DISABLED = 0,
    ICC_CALL_FORWARD_TYPE_ENABLED = 1,
    ICC_CALL_FORWARD_TYPE_UNKNOWN = -1
};

enum SimMessage {
    MSG_ID_DEFAULT = 0,
    MSG_SIM_OBTAIN_IMSI_DONE = 0x03,
    MSG_SIM_OBTAIN_ICCID_DONE,
    MSG_SIM_OBTAIN_MBI_DONE,
    MSG_SIM_OBTAIN_MBDN_DONE,
    MSG_SIM_OBTAIN_MWIS_DONE,
    MSG_SIM_OBTAIN_VOICE_MAIL_INDICATOR_CPHS_DONE,
    MSG_SIM_OBTAIN_AD_DONE, // Admin data
    MSG_SIM_OBTAIN_MSISDN_DONE,
    MSG_SIM_OBTAIN_CPHS_MAILBOX_DONE,
    MSG_SIM_OBTAIN_SPN_DONE,
    MSG_SIM_OBTAIN_LI_LANGUAGE_DONE,
    MSG_SIM_OBTAIN_PL_LANGUAGE_DONE,
    MSG_SIM_OBTAIN_SPDI_DONE,
    MSG_SIM_UPDATE_DONE,
    MSG_SIM_OBTAIN_PNN_DONE,
    MSG_SIM_OBTAIN_SST_DONE,
    MSG_SIM_OBTAIN_ALL_SMS_DONE,
    MSG_SIM_MARK_SMS_READ_DONE,
    MSG_SIM_SET_MBDN_DONE,
    MSG_SIM_SMS_ON_SIM,
    MSG_SIM_OBTAIN_SMS_DONE,
    MSG_SIM_OBTAIN_CFF_DONE,
    MSG_SIM_SET_CPHS_MAILBOX_DONE,
    MSG_SIM_OBTAIN_INFO_CPHS_DONE,
    MSG_SIM_SET_MSISDN_DONE,
    MSG_SIM_OBTAIN_CFIS_DONE,
    MSG_SIM_OBTAIN_CSP_CPHS_DONE,
    MSG_SIM_OBTAIN_GID1_DONE,
    MSG_SIM_OBTAIN_GID2_DONE,
    MSG_SIM_OBTAIN_PLMN_W_ACT_DONE,
    MSG_SIM_OBTAIN_OPLMN_W_ACT_DONE,
    MSG_SIM_OBTAIN_HPLMN_W_ACT_DONE,
    MSG_SIM_OBTAIN_EHPLMN_DONE,
    MSG_SIM_OBTAIN_FPLMN_DONE,
    MSG_SIM_OBTAIN_DEVICE_IDENTITY_DONE,
    MSG_SIM_OBTAIN_CDMA_SUBSCRIPTION_DONE,
    MSG_SIM_SMS_ON_RUIM,
    MSG_SIM_OBTAIN_OPL_DONE,
    MSG_SIM_OBTAIN_OPL5G_DONE,
    MSG_SIM_OBTAIN_IMPI_DONE,
    MSG_SIM_OBTAIN_CSIM_SPN_DONE,
    MSG_SIM_OBTAIN_IST_DONE,
#ifdef CORE_SERVICE_SUPPORT_ESIM
    MSG_ESIM_OPEN_CHANNEL_DONE,
    MSG_ESIM_CLOSE_CHANNEL_DONE,
    MSG_ESIM_OBTAIN_EID_DONE,
    MSG_ESIM_OBTAIN_EUICC_CHALLENGE_DONE,
    MSG_ESIM_OBTAIN_EUICC_INFO2_DONE,
    MSG_ESIM_OBTAIN_EUICC_INFO_1_DONE,
    MSG_ESIM_OBTAIN_DEFAULT_SMDP_ADDRESS_DONE,
    MSG_ESIM_ESTABLISH_DEFAULT_SMDP_ADDRESS_DONE,
    MSG_ESIM_DELETE_PROFILE,
    MSG_ESIM_IS_ESIM_SUPPORT,
    MSG_ESIM_SWITCH_PROFILE,
    MSG_ESIM_DISABLE_PROFILE,
    MSG_ESIM_RESET_MEMORY,
    MSG_ESIM_REMOVE_NOTIFICATION,
    MSG_ESIM_REQUEST_ALL_PROFILES,
    MSG_ESIM_LIST_NOTIFICATION,
    MSG_ESIM_SET_NICK_NAME,
    MSG_ESIM_CANCEL_SESSION,
    MSG_ESIM_OBTAIN_SMDS_ADDRESS,
    MSG_ESIM_GET_PROFILE,
    MSG_ESIM_RETRIEVE_NOTIFICATION_DONE,
    MSG_ESIM_RETRIEVE_NOTIFICATION_LIST,
    MSG_ESIM_AUTHENTICATE_SERVER,
    MSG_ESIM_PREPARE_DOWNLOAD_DONE,
    MSG_ESIM_SEND_APUD_DATA,
    MSG_ESIM_LOAD_BOUND_PROFILE_PACKAGE,
    MSG_ESIM_REQUEST_RULES_AUTH_TABLE,
#endif
};

enum ElementaryFile {
    // ETSI TS 102 221 V3.3.0 section 13.3 Preferred Languages
    ELEMENTARY_FILE_PL = 0x2F05,
    // CPHS V4.2 section B.3.1.1, B.4.1 - B.4.7 CPHS information Call forwarding flags and so on
    ELEMENTARY_FILE_VOICE_MAIL_INDICATOR_CPHS = 0x6F11,
    ELEMENTARY_FILE_CFF_CPHS = 0x6F13,
    ELEMENTARY_FILE_SPN_CPHS = 0x6F14,
    ELEMENTARY_FILE_CSP_CPHS = 0x6F15,
    ELEMENTARY_FILE_INFO_CPHS = 0x6F16,
    ELEMENTARY_FILE_MAILBOX_CPHS = 0x6F17,
    ELEMENTARY_FILE_SPN_SHORT_CPHS = 0x6F18,
    // 3GPP TS 51.011 V4.1.0 section 10 contents of the Elementary Files
    ELEMENTARY_FILE_ICCID = 0x2FE2,
    ELEMENTARY_FILE_OPL5G = 0x4F08,
    ELEMENTARY_FILE_IMG = 0x4F20,
    ELEMENTARY_FILE_HPLMN = 0x6F31,
    ELEMENTARY_FILE_SST = 0x6F38,
    ELEMENTARY_FILE_ADN = 0x6F3A,
    ELEMENTARY_FILE_FDN = 0x6F3B,
    ELEMENTARY_FILE_SMS = 0x6F3C,
    ELEMENTARY_FILE_GID1 = 0x6F3E,
    ELEMENTARY_FILE_GID2 = 0x6F3F,
    ELEMENTARY_FILE_MSISDN = 0x6F40,
    ELEMENTARY_FILE_SPN = 0x6F46,
    ELEMENTARY_FILE_SDN = 0x6F49,
    ELEMENTARY_FILE_EXT1 = 0x6F4A,
    ELEMENTARY_FILE_EXT2 = 0x6F4B,
    ELEMENTARY_FILE_EXT3 = 0x6F4C,
    ELEMENTARY_FILE_EXT4 = 0x6F4E,
    ELEMENTARY_FILE_PLMN_W_ACT = 0x6F60,
    ELEMENTARY_FILE_OPLMN_W_ACT = 0x6F61,
    ELEMENTARY_FILE_HPLMN_W_ACT = 0x6F62,
    ELEMENTARY_FILE_FPLMN = 0x6F7B,
    ELEMENTARY_FILE_AD = 0x6FAD,
    ELEMENTARY_FILE_PNN = 0x6FC5,
    ELEMENTARY_FILE_OPL = 0x6FC6,
    ELEMENTARY_FILE_MBDN = 0x6FC7,
    ELEMENTARY_FILE_EXT6 = 0x6FC8,
    ELEMENTARY_FILE_MBI = 0x6FC9,
    ELEMENTARY_FILE_MWIS = 0x6FCA,
    ELEMENTARY_FILE_CFIS = 0x6FCB,
    // 3GPP TS 31.102 V3.5.0 section 4.7 files of ADF USIM
    ELEMENTARY_FILE_PBR = 0x4F30,
    ELEMENTARY_FILE_LI = 0x6F05,
    // for usim extensiom
    ELEMENTARY_FILE_EXT5 = 0x6F4E,
    ELEMENTARY_FILE_SPDI = 0x6FCD,
    ELEMENTARY_FILE_EHPLMN = 0x6FD9,
    ELEMENTARY_FILE_LRPLMNSI = 0x6FDC,
    ELEMENTARY_FILE_HPPLMN = 0x6F31,
    ELEMENTARY_FILE_USIM_ADN = 0x4F3A,
    ELEMENTARY_FILE_USIM_IAP = 0x4F3B,
    // CDMA RUIM file ids from 3GPP2 C.S0023-0
    ELEMENTARY_FILE_CST = 0x6F32,
    ELEMENTARY_FILE_RUIM_SPN = 0x6F41,
    ELEMENTARY_FILE_CSIM_LI = 0x6F3A,
    ELEMENTARY_FILE_CSIM_SPN = 0x6F41,
    ELEMENTARY_FILE_CSIM_MDN = 0x6F44,
    ELEMENTARY_FILE_CSIM_IMSIM = 0x6F22,
    ELEMENTARY_FILE_CSIM_CDMAHOME = 0x6F28,
    ELEMENTARY_FILE_CSIM_EPRL = 0x6F5A,
    ELEMENTARY_FILE_CSIM_MIPUPP = 0x6F4D,
    // ISIM access
    ELEMENTARY_FILE_IMPU = 0x6F04,
    ELEMENTARY_FILE_IMPI = 0x6F02,
    ELEMENTARY_FILE_DOMAIN = 0x6F03,
    ELEMENTARY_FILE_IST = 0x6F07,
    ELEMENTARY_FILE_PCSCF = 0x6F09
};

// file controller constant
enum ControllerRequest {
    CONTROLLER_REQ_READ_BINARY = 0xb0,
    CONTROLLER_REQ_UPDATE_BINARY = 0xd6,
    CONTROLLER_REQ_READ_RECORD = 0xb2,
    CONTROLLER_REQ_UPDATE_RECORD = 0xdc,
    CONTROLLER_REQ_SEEK = 0xa2,
    CONTROLLER_REQ_GET_RESPONSE = 0xc0
};

enum ElementaryFileTile {
    ELEMENTARY_FILE_TYPE_TRANSPARENT = 0,
    ELEMENTARY_FILE_TYPE_LINEAR_FIXED = 1,
    ELEMENTARY_FILE_TYPE_CYCLIC = 3
};

enum MainFile {
    ICC_MASTER_FILE = 1,
    ICC_DEDICATED_FILE = 2,
    ICC_ELEMENTARY_FILE = 4
};

enum ResponseFile {
    GET_RESPONSE_ELEMENTARY_FILE_SIZE_BYTES = 15,
    GET_RESPONSE_ELEMENTARY_FILE_IMG_SIZE_BYTES = 10
};

// controller file type
enum SizeType {
    SIZE_ONE_OF_FILE = 2,
    SIZE_TWO_OF_FILE = 3,
    TYPE_OF_FILE = 6,
    STRUCTURE_OF_DATA = 13,
    LENGTH_OF_RECORD = 14
};

enum ControllerMessage {
    MSG_SIM_OBTAIN_SIZE_OF_FIXED_ELEMENTARY_FILE_DONE = 1,
    MSG_SIM_OBTAIN_FIXED_ELEMENTARY_FILE_DONE = 2,
    MSG_SIM_OBTAIN_SIZE_OF_TRANSPARENT_ELEMENTARY_FILE_DONE = 3,
    MSG_SIM_OBTAIN_TRANSPARENT_ELEMENTARY_FILE_DONE = 4,
    MSG_SIM_OBTAIN_SIZE_OF_LINEAR_ELEMENTARY_FILE_DONE = 5,
    MSG_SIM_UPDATE_TRANSPARENT_ELEMENTARY_FILE_DONE = 6,
    MSG_SIM_UPDATE_LINEAR_FIXED_FILE_DONE = 7,
    MSG_SIM_OBTAIN_ICON_DONE = 10
};

enum {
    INVALID_VALUE = -1,
    BYTE_LENGTH = 8,
    BYTE_VALUE = 0xFF
};

enum LockMsg {
    MSG_SIM_LOCKED_RECORDS_LOADED = 0xf3,
    MSG_SIM_NETWORK_LOCKED_RECORDS_LOADED = 0xf4,
    MSG_SIM_SPN_UPDATED = 0xf5
};

enum SpnType { SPN_INVALID = -1, SPN_COND_PLMN = 1, SPN_COND = 2 };

enum SpnShowType {
    SPN_CONDITION_DISPLAY_SPN = 0x01,
    SPN_CONDITION_DISPLAY_PLMN = 0x02
};

enum ActiveState {
    DEACTIVE = 0,
    ACTIVE = 1
};

enum CardShape {
    ENTITY_CARD = 1,
    VIRTUAL_CARD = 11
};

enum CardStand {
    NOT_MAIN = 0,
    MAIN_CARD = 1
};

enum IccStatus {
    ICC_STATE_UNKNOWN = 0,
    ICC_STATE_NOT_READY = 1,
    ICC_STATE_ABSENT = 2,
    ICC_STATE_READY = 3,
    ICC_STATE_LOADED = 4,
    ICC_STATE_IMSI = 5,
    ICC_STATE_PIN = 6,
    ICC_STATE_PUK = 7,
    ICC_STATE_SIMLOCK = 8
};

enum IccDiallingNumberConstant {
    LOADER_ERROR = 200,
    PARAMETER_INCORRECT = 201,
    NAME_CHAR_POS = 36,
    CSIM_SPN_OCTET = 0x00,
    CSIM_SPN_7BIT_ASCII = 0x02,
    CSIM_SPN_IA5 = 0x03,
    CSIM_SPN_UNICODE_16 = 0x04,
    CSIM_SPN_LATIN = 0x08,
    CSIM_SPN_7BIT_ALPHABET = 0x09
};

enum RadioProtocolEvent {
    MSG_SIM_TIME_OUT_ACTIVE = 2000,
    MSG_SIM_SET_ACTIVE = 2001,
    MSG_SIM_FORGET_ALLDATA = 2002,
};

enum VoiceMailConstant {
    DEFAULT_VOICE_MAIL_COUNT = -2,
    UNKNOWN_VOICE_MAIL_COUNT = -1,
};

enum CallForwardingStatus {
    CALL_FORWARDING_STATUS_DISABLED = 0,
    CALL_FORWARDING_STATUS_ENABLED = 1,
    CALL_FORWARDING_STATUS_UNKNOWN = -1,
};

inline const std::string DATASHARE_READY_EVENT = "usual.event.DATA_SHARE_READY";
inline const std::string BUNDLE_SCAN_FINISHED_EVENT = "usual.event.BUNDLE_SCAN_FINISHED";
} // namespace Telephony
} // namespace OHOS
#endif // OHOS_SIM_CONSTANT_H
