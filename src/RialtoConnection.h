/*
 * If not stated otherwise in this file or this component's LICENSE file the
 * following copyright and licenses apply:
 *
 * Copyright 2023 Liberty Global Service B.V.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef __RIALTO_CONNECTION_H__
#define __RIALTO_CONNECTION_H__

#include <map>
#include <string>
#include <mutex>
#include <functional>
#include <condition_variable>
#include <rialto/ServerManagerServiceFactory.h>

using namespace rialto::servermanager::service;
using namespace firebolt::rialto::common;

constexpr const char *stateStr(firebolt::rialto::common::SessionServerState state)
{
    switch (state)
    {
        case firebolt::rialto::common::SessionServerState::UNINITIALIZED:
            return "uninitialized";
        case firebolt::rialto::common::SessionServerState::INACTIVE:
            return "inactive";
        case firebolt::rialto::common::SessionServerState::ACTIVE:
            return "active";
        case firebolt::rialto::common::SessionServerState::NOT_RUNNING:
            return "notrunning";
        case firebolt::rialto::common::SessionServerState::ERROR:
            return "error";
    }
    return "error";
}

class RialtoConnection : public IStateObserver, public std::enable_shared_from_this<RialtoConnection>
{
public:
    RialtoConnection() = default;
    ~RialtoConnection() override = default;
    using StateChangeCb = std::function<void(const std::string& id, const std::string& state)>;

    void initialize();
    bool activate(const std::string &id);
    bool deactivate(const std::string &id);
    bool stop(const std::string &id);

    std::string getAppConnectionInfo(const std::string &id);
    void stateChanged(const std::string &appId, const SessionServerState &state) override;
    SessionServerState getState(const std::string &id);
    void setStateChangeCallback(StateChangeCb cb)
    {
        m_stateChangeCb = cb;
    }

    RialtoConnection(const RialtoConnection &) = delete;
    RialtoConnection &operator=(const RialtoConnection &) = delete;

private:
    std::map<std::string, SessionServerState> m_states;
    std::unique_ptr<IServerManagerService> m_serverManager;
    std::mutex m_mutex;
    std::string m_socketPath;
    bool m_ready{false};
    StateChangeCb m_stateChangeCb;
};

#endif