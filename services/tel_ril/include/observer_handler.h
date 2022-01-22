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

#ifndef OBSERVER_HANDLER_H
#define OBSERVER_HANDLER_H

#include <unordered_map>
#include "event_handler.h"
#include "telephony_log_wrapper.h"

namespace OHOS {
namespace Telephony {
class ObserverHandler {
public:
    ObserverHandler();

    virtual ~ObserverHandler();

    void RegObserver(int32_t what, const std::shared_ptr<AppExecFwk::EventHandler> handler);

    void RegUniqueObserver(int32_t what, const std::shared_ptr<AppExecFwk::EventHandler> handler);

    void Remove(int32_t what, const std::shared_ptr<AppExecFwk::EventHandler> handler);

    void RemoveAll();

    void NotifyObserver(int32_t what);

    template<typename T>
    void NotifyObserver(int32_t what, T *object)
    {
        auto iter = observerHandlerMap_.find(what);
        if (iter == observerHandlerMap_.end()) {
            TELEPHONY_LOGE("ObserverHandler NotifyObserver %{public}d not register", what);
            return;
        }
        std::shared_ptr<T> msg(object);

        for (auto handlers : iter->second) {
            handlers->SendEvent(what, msg);
        }
    }

    template<typename T>
    void NotifyObserver(int32_t what, std::shared_ptr<T> object)
    {
        auto iter = observerHandlerMap_.find(what);
        if (iter == observerHandlerMap_.end()) {
            TELEPHONY_LOGE("ObserverHandler NotifyObserver %{public}d not register", what);
            return;
        }
        for (auto handlers : iter->second) {
            handlers->SendEvent(what, object);
        }
    }

private:
    std::unordered_map<int32_t, std::list<std::shared_ptr<AppExecFwk::EventHandler>>> observerHandlerMap_;
};
} // namespace Telephony
} // namespace OHOS
#endif // OBSERVER_HANDLER_H
