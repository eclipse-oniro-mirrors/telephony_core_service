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

#include <gtest/gtest.h>
#include <memory>

#include "telephony_errors.h"

// 直接编译被测源码，访问静态分支函数（ConvertErrorCode / ParseStartHiddenCameraDetectionParams /
// CreateResolvedPromise / ReportInfo 等），避免为覆盖分支修改业务代码
#include "napi_sense.cpp"

namespace OHOS {
namespace Telephony {
using namespace testing;
using namespace testing::ext;

namespace {
constexpr int32_t INVALID_ERROR_CODE = 12345;
constexpr int32_t ERR_CAPABILITY_NOT_SUPPORTED_VALUE = 801;
} // namespace

class SenseNapiBranchTest : public testing::Test {
public:
    static void SetUpTestCase() {}
    static void TearDownTestCase() {}
    void SetUp()
    {
        ResetSenseState();
    }
    void TearDown()
    {
        ResetSenseState();
    }

private:
    static void ResetSenseState()
    {
        auto &state = GetSenseManagerState();
        state.callback = nullptr;
        state.registered = false;
        state.env = nullptr;
        state.thisVar = nullptr;
        state.callbackRef = nullptr;
    }
};

// ============================ ConvertErrorCode 新增分支 ============================

/**
 * @tc.number  Telephony_Sense_ConvertErrorCode_0100
 * @tc.name    success code and codes in valid range keep unchanged
 */
HWTEST_F(SenseNapiBranchTest, ConvertErrorCode001, Function | MediumTest | Level1)
{
    SenseHiddenCameraDetectionRsltCode rsltCode = SenseHiddenCameraDetectionRsltCode::ERR_INTERNAL_SYSTEM_ERROR;
    ConvertErrorCode(TELEPHONY_SUCCESS, rsltCode);
    EXPECT_EQ(rsltCode, SenseHiddenCameraDetectionRsltCode::SUCCESS);
    ConvertErrorCode(static_cast<int32_t>(SenseHiddenCameraDetectionRsltCode::ERR_CREATE_ASYNC_WORK_ERROR),
        rsltCode);
    EXPECT_EQ(rsltCode, SenseHiddenCameraDetectionRsltCode::ERR_CREATE_ASYNC_WORK_ERROR);
    ConvertErrorCode(static_cast<int32_t>(SenseHiddenCameraDetectionRsltCode::ERR_INTERNAL_SYSTEM_ERROR), rsltCode);
    EXPECT_EQ(rsltCode, SenseHiddenCameraDetectionRsltCode::ERR_INTERNAL_SYSTEM_ERROR);
}

/**
 * @tc.number  Telephony_Sense_ConvertErrorCode_0200
 * @tc.name    permission error and capability error are mapped to dedicated codes
 */
HWTEST_F(SenseNapiBranchTest, ConvertErrorCode002, Function | MediumTest | Level1)
{
    SenseHiddenCameraDetectionRsltCode rsltCode = SenseHiddenCameraDetectionRsltCode::SUCCESS;
    ConvertErrorCode(TELEPHONY_ERR_PERMISSION_ERR, rsltCode);
    EXPECT_EQ(rsltCode, SenseHiddenCameraDetectionRsltCode::ERR_PERMISSION_DENY);
    ConvertErrorCode(ERR_CAPABILITY_NOT_SUPPORTED_VALUE, rsltCode);
    EXPECT_EQ(rsltCode, SenseHiddenCameraDetectionRsltCode::ERR_CAPABILITY_NOT_SUPPORTED);
}

/**
 * @tc.number  Telephony_Sense_ConvertErrorCode_0300
 * @tc.name    unknown error falls back to ERR_INTERNAL_SYSTEM_ERROR
 */
HWTEST_F(SenseNapiBranchTest, ConvertErrorCode003, Function | MediumTest | Level1)
{
    SenseHiddenCameraDetectionRsltCode rsltCode = SenseHiddenCameraDetectionRsltCode::SUCCESS;
    ConvertErrorCode(INVALID_ERROR_CODE, rsltCode);
    EXPECT_EQ(rsltCode, SenseHiddenCameraDetectionRsltCode::ERR_INTERNAL_SYSTEM_ERROR);
}

// ============================ CreateResolvedPromise 创建失败分支 ============================

/**
 * @tc.number  Telephony_Sense_CreateResolvedPromise_0100
 * @tc.name    promise creation failure returns nullptr
 */
HWTEST_F(SenseNapiBranchTest, CreateResolvedPromise001, Function | MediumTest | Level1)
{
    napi_value promise = CreateResolvedPromise(nullptr, SenseHiddenCameraDetectionRsltCode::SUCCESS);
    EXPECT_EQ(promise, nullptr);
}

// ============================ ReportInfo 重组后的守护分支 ============================

/**
 * @tc.number  Telephony_Sense_ReportInfo_0100
 * @tc.name    mismatched env / unset refs are rejected by guard branches
 */
HWTEST_F(SenseNapiBranchTest, ReportInfo001, Function | MediumTest | Level1)
{
    HiddenCameraDetectionRptInfo info;
    auto &state = GetSenseManagerState();

    // 先短路分支：envExecute 不匹配
    EXPECT_EQ(ReportInfo(info, reinterpret_cast<napi_env>(0x1)), TELEPHONY_ERROR);

    // env 匹配但 thisVar 未设置
    EXPECT_EQ(ReportInfo(info, nullptr), TELEPHONY_ERROR);

    // thisVar 已设置但 callbackRef 未设置
    state.thisVar = reinterpret_cast<napi_ref>(0x1);
    EXPECT_EQ(ReportInfo(info, nullptr), TELEPHONY_ERROR);
}

/**
 * @tc.number  Telephony_Sense_ReportInfo_0200
 * @tc.name    guard passes but handle scope creation fails
 */
HWTEST_F(SenseNapiBranchTest, ReportInfo002, Function | MediumTest | Level1)
{
    HiddenCameraDetectionRptInfo info;
    auto &state = GetSenseManagerState();
    state.thisVar = reinterpret_cast<napi_ref>(0x1);
    state.callbackRef = reinterpret_cast<napi_ref>(0x2);
    EXPECT_EQ(ReportInfo(info, nullptr), TELEPHONY_ERROR);
}

// ============================ ReportInfoToApp 守护分支 ============================

/**
 * @tc.number  Telephony_Sense_ReportInfoToApp_0100
 * @tc.name    unset callback info is rejected
 */
HWTEST_F(SenseNapiBranchTest, ReportInfoToApp001, Function | MediumTest | Level1)
{
    HiddenCameraDetectionRptInfo info;
    EXPECT_EQ(ReportInfoToApp(info), TELEPHONY_ERROR);
}

// ============================ StartHiddenCameraDetectionAsync 分支 ============================

/**
 * @tc.number  Telephony_Sense_StartAsync_0100
 * @tc.name    unregistered callback returns ERR_INTERNAL_SYSTEM_ERROR
 */
HWTEST_F(SenseNapiBranchTest, StartAsync001, Function | MediumTest | Level1)
{
    HiddenCameraDetectionConfig config;
    EXPECT_EQ(StartHiddenCameraDetectionAsync(config),
        static_cast<int32_t>(SenseHiddenCameraDetectionRsltCode::ERR_INTERNAL_SYSTEM_ERROR));
}

/**
 * @tc.number  Telephony_Sense_StartAsync_0200
 * @tc.name    registered callback goes through client call which fails without service
 */
HWTEST_F(SenseNapiBranchTest, StartAsync002, Function | MediumTest | Level1)
{
    GetSenseManagerState().callback = sptr<HiddenCameraDetectionCallback>::MakeSptr();
    HiddenCameraDetectionConfig config;
    int32_t ret = StartHiddenCameraDetectionAsync(config);
    EXPECT_NE(ret, static_cast<int32_t>(SenseHiddenCameraDetectionRsltCode::SUCCESS));
}

// ============================ RegisterHiddenCameraDetection 参数分支 ============================

/**
 * @tc.number  Telephony_Sense_Register_0100
 * @tc.name    invalid env makes reference creation fail
 */
HWTEST_F(SenseNapiBranchTest, Register001, Function | MediumTest | Level1)
{
    napi_value thisVar = reinterpret_cast<napi_value>(0x1);
    napi_value parFrist = reinterpret_cast<napi_value>(0x2);
    EXPECT_EQ(RegisterHiddenCameraDetection(nullptr, thisVar, parFrist),
        static_cast<int32_t>(SenseHiddenCameraDetectionRsltCode::ERR_INTERNAL_SYSTEM_ERROR));
}

// ============================ UnregisterHiddenCameraDetection 分支 ============================

/**
 * @tc.number  Telephony_Sense_Unregister_0100
 * @tc.name    calling on different thread returns ERR_API_CALLED_IN_DIFFERENT_THREAD
 */
HWTEST_F(SenseNapiBranchTest, Unregister001, Function | MediumTest | Level1)
{
    GetSenseManagerState().env = reinterpret_cast<napi_env>(0x1);
    EXPECT_EQ(UnregisterHiddenCameraDetection(nullptr),
        static_cast<int32_t>(SenseHiddenCameraDetectionRsltCode::ERR_API_CALLED_IN_DIFFERENT_THREAD));
}

/**
 * @tc.number  Telephony_Sense_Unregister_0200
 * @tc.name    normal unregister clears registered state and refs
 */
HWTEST_F(SenseNapiBranchTest, Unregister002, Function | MediumTest | Level1)
{
    auto &state = GetSenseManagerState();
    state.callback = sptr<HiddenCameraDetectionCallback>::MakeSptr();
    state.registered = true;
    state.thisVar = reinterpret_cast<napi_ref>(0x1);
    state.callbackRef = reinterpret_cast<napi_ref>(0x2);
    EXPECT_EQ(UnregisterHiddenCameraDetection(nullptr),
        static_cast<int32_t>(SenseHiddenCameraDetectionRsltCode::SUCCESS));
    EXPECT_EQ(state.callback, nullptr);
    EXPECT_FALSE(state.registered);
    EXPECT_EQ(state.thisVar, nullptr);
    EXPECT_EQ(state.callbackRef, nullptr);
}

// ============================ Stop/Keep Async 未注册分支与客户端失败分支 ============================

/**
 * @tc.number  Telephony_Sense_StopAsync_0100
 * @tc.name    stop without start returns ERR_NO_HIDDEN_CAMERA_DETECTION
 */
HWTEST_F(SenseNapiBranchTest, StopAsync001, Function | MediumTest | Level1)
{
    EXPECT_EQ(StopHiddenCameraDetectionAsync(),
        static_cast<int32_t>(SenseHiddenCameraDetectionRsltCode::ERR_NO_HIDDEN_CAMERA_DETECTION));
}

/**
 * @tc.number  Telephony_Sense_StopAsync_0200
 * @tc.name    stop with registered state fails through client without service
 */
HWTEST_F(SenseNapiBranchTest, StopAsync002, Function | MediumTest | Level1)
{
    GetSenseManagerState().registered = true;
    EXPECT_NE(StopHiddenCameraDetectionAsync(), static_cast<int32_t>(SenseHiddenCameraDetectionRsltCode::SUCCESS));
}

/**
 * @tc.number  Telephony_Sense_KeepAsync_0100
 * @tc.name    keep without start returns ERR_NO_HIDDEN_CAMERA_DETECTION
 */
HWTEST_F(SenseNapiBranchTest, KeepAsync001, Function | MediumTest | Level1)
{
    EXPECT_EQ(KeepHiddenCameraDetectionAsync(),
        static_cast<int32_t>(SenseHiddenCameraDetectionRsltCode::ERR_NO_HIDDEN_CAMERA_DETECTION));
}

/**
 * @tc.number  Telephony_Sense_KeepAsync_0200
 * @tc.name    keep with registered state fails through client without service
 */
HWTEST_F(SenseNapiBranchTest, KeepAsync002, Function | MediumTest | Level1)
{
    GetSenseManagerState().registered = true;
    EXPECT_NE(KeepHiddenCameraDetectionAsync(), static_cast<int32_t>(SenseHiddenCameraDetectionRsltCode::SUCCESS));
}

// ============================ ParseStartHiddenCameraDetectionParams 参数校验分支 ============================

/**
 * @tc.number  Telephony_Sense_ParseParams_0100
 * @tc.name    invalid env leads to parameter mismatch
 */
HWTEST_F(SenseNapiBranchTest, ParseParams001, Function | MediumTest | Level1)
{
    napi_value parameters[SENSE_PARAM_COUNT_TWO] = { 0 };
    napi_value thisVar = nullptr;
    int32_t period = 0;
    EXPECT_FALSE(ParseStartHiddenCameraDetectionParams(nullptr, nullptr, parameters, thisVar, period));
}

// ============================ NAPI 入口函数分支 ============================

/**
 * @tc.number  Telephony_Sense_NapiEntry_0100
 * @tc.name    start entry returns resolved ERR_PARAMETER_INCORRECT path when params invalid
 */
HWTEST_F(SenseNapiBranchTest, NapiEntry001, Function | MediumTest | Level1)
{
    EXPECT_EQ(StartHiddenCameraDetection(nullptr, nullptr), nullptr);
}

/**
 * @tc.number  Telephony_Sense_NapiEntry_0200
 * @tc.name    stop entry on different thread takes ERR_API_CALLED_IN_DIFFERENT_THREAD path
 */
HWTEST_F(SenseNapiBranchTest, NapiEntry002, Function | MediumTest | Level1)
{
    GetSenseManagerState().env = reinterpret_cast<napi_env>(0x1);
    EXPECT_EQ(StopHiddenCameraDetection(nullptr, nullptr), nullptr);
}

/**
 * @tc.number  Telephony_Sense_NapiEntry_0300
 * @tc.name    stop entry takes queue async work failure path
 */
HWTEST_F(SenseNapiBranchTest, NapiEntry003, Function | MediumTest | Level1)
{
    EXPECT_EQ(StopHiddenCameraDetection(nullptr, nullptr), nullptr);
}

/**
 * @tc.number  Telephony_Sense_NapiEntry_0400
 * @tc.name    keep entry on different thread takes ERR_API_CALLED_IN_DIFFERENT_THREAD path
 */
HWTEST_F(SenseNapiBranchTest, NapiEntry004, Function | MediumTest | Level1)
{
    GetSenseManagerState().env = reinterpret_cast<napi_env>(0x1);
    EXPECT_EQ(KeepHiddenCameraDetection(nullptr, nullptr), nullptr);
}

/**
 * @tc.number  Telephony_Sense_NapiEntry_0500
 * @tc.name    keep entry takes queue async work failure path
 */
HWTEST_F(SenseNapiBranchTest, NapiEntry005, Function | MediumTest | Level1)
{
    EXPECT_EQ(KeepHiddenCameraDetection(nullptr, nullptr), nullptr);
}

/**
 * @tc.number  Telephony_Sense_NapiEntry_0600
 * @tc.name    module init with invalid env returns nothing created
 */
HWTEST_F(SenseNapiBranchTest, NapiEntry006, Function | MediumTest | Level1)
{
    EXPECT_EQ(InitNapiSense(nullptr, nullptr), nullptr);
}

// ============================ 异步工作回调分支 ============================

/**
 * @tc.number  Telephony_Sense_NativeExecute_0100
 * @tc.name    native execute handles null and non-null context
 */
HWTEST_F(SenseNapiBranchTest, NativeExecute001, Function | MediumTest | Level1)
{
    NativeStartHiddenCameraDetection(nullptr, nullptr);
    NativeStopHiddenCameraDetection(nullptr, nullptr);
    NativeKeepHiddenCameraDetection(nullptr, nullptr);

    auto context = std::make_unique<SenseAsyncContext>();
    SenseAsyncContext *rawContext = context.get();
    NativeStartHiddenCameraDetection(nullptr, rawContext);
    EXPECT_NE(rawContext->result, static_cast<int32_t>(SenseHiddenCameraDetectionRsltCode::SUCCESS));
    NativeStopHiddenCameraDetection(nullptr, rawContext);
    NativeKeepHiddenCameraDetection(nullptr, rawContext);
    EXPECT_NE(rawContext->result, static_cast<int32_t>(SenseHiddenCameraDetectionRsltCode::SUCCESS));
}

/**
 * @tc.number  Telephony_Sense_CompleteCallback_0100
 * @tc.name    start callback with failure result unregisters detection
 */
HWTEST_F(SenseNapiBranchTest, CompleteCallback001, Function | MediumTest | Level1)
{
    auto &state = GetSenseManagerState();
    state.callback = sptr<HiddenCameraDetectionCallback>::MakeSptr();
    state.registered = true;

    auto context = std::make_unique<SenseAsyncContext>();
    context->result = static_cast<int32_t>(SenseHiddenCameraDetectionRsltCode::ERR_INTERNAL_SYSTEM_ERROR);
    StartHiddenCameraDetectionCallback(nullptr, napi_ok, context.release());
    EXPECT_FALSE(state.registered);
    EXPECT_EQ(state.callback, nullptr);
}

/**
 * @tc.number  Telephony_Sense_CompleteCallback_0200
 * @tc.name    start callback with success result keeps registration and resolves deferred
 */
HWTEST_F(SenseNapiBranchTest, CompleteCallback002, Function | MediumTest | Level1)
{
    auto &state = GetSenseManagerState();
    state.callback = sptr<HiddenCameraDetectionCallback>::MakeSptr();
    state.registered = true;

    auto context = std::make_unique<SenseAsyncContext>();
    context->result = static_cast<int32_t>(SenseHiddenCameraDetectionRsltCode::SUCCESS);
    context->context.deferred = reinterpret_cast<napi_deferred>(0x1);
    StartHiddenCameraDetectionCallback(nullptr, napi_ok, context.release());
    EXPECT_TRUE(state.registered);
}

/**
 * @tc.number  Telephony_Sense_CompleteCallback_0300
 * @tc.name    stop callback with success result unregisters and resolves deferred
 */
HWTEST_F(SenseNapiBranchTest, CompleteCallback003, Function | MediumTest | Level1)
{
    auto &state = GetSenseManagerState();
    state.callback = sptr<HiddenCameraDetectionCallback>::MakeSptr();
    state.registered = true;

    auto context = std::make_unique<SenseAsyncContext>();
    context->result = static_cast<int32_t>(SenseHiddenCameraDetectionRsltCode::SUCCESS);
    context->context.deferred = reinterpret_cast<napi_deferred>(0x1);
    StopHiddenCameraDetectionCallback(nullptr, napi_ok, context.release());
    EXPECT_FALSE(state.registered);
}

/**
 * @tc.number  Telephony_Sense_CompleteCallback_0400
 * @tc.name    stop/keep callbacks with null context and failure result take early paths
 */
HWTEST_F(SenseNapiBranchTest, CompleteCallback004, Function | MediumTest | Level1)
{
    StopHiddenCameraDetectionCallback(nullptr, napi_ok, nullptr);
    KeepHiddenCameraDetectionCallback(nullptr, napi_ok, nullptr);

    auto &state = GetSenseManagerState();
    state.callback = sptr<HiddenCameraDetectionCallback>::MakeSptr();
    state.registered = true;

    auto stopContext = std::make_unique<SenseAsyncContext>();
    stopContext->result = static_cast<int32_t>(SenseHiddenCameraDetectionRsltCode::ERR_INTERNAL_SYSTEM_ERROR);
    StopHiddenCameraDetectionCallback(nullptr, napi_ok, stopContext.release());
    EXPECT_TRUE(state.registered);

    auto keepContext = std::make_unique<SenseAsyncContext>();
    keepContext->result = static_cast<int32_t>(SenseHiddenCameraDetectionRsltCode::SUCCESS);
    KeepHiddenCameraDetectionCallback(nullptr, napi_ok, keepContext.release());
    EXPECT_FALSE(state.registered);
}
} // namespace Telephony
} // namespace OHOS
