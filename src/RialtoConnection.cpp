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

#include <iostream>
#include "RialtoConnection.h"
#include "Logging.h"

#define RIALTO_SERVERMANAGER_SOCKET_PATH_ENV "RIALTO_SERVERMANAGER_SOCKET_PATH"

#define RIALTO_SERVER_ENV_VARS_ENV "RIALTO_SERVER_ENV_VARS"
// alternative for above
#define SESSION_SERVER_ENV_VARS_ENV "SESSION_SERVER_ENV_VARS"

static std::list<std::string> split(const std::string &str, char separator)
{
    std::list<std::string> parts;
    size_t start = 0;
    auto end = str.find(separator);

    while (end != std::string::npos)
    {
        parts.emplace_back(str.substr(start, end - start));
        start = end + 1;
        end = str.find(separator, start);
    }

    parts.emplace_back(str.substr(start));
    return parts;
}

std::list<std::string> getEnvironmentVariables()
{
    const char *sessionServerEnvVars = std::getenv(RIALTO_SERVER_ENV_VARS_ENV);
    if (!sessionServerEnvVars)
    {
        sessionServerEnvVars = std::getenv(SESSION_SERVER_ENV_VARS_ENV);
    }

    if (sessionServerEnvVars)
    {
        return split(sessionServerEnvVars, ';');
    }
    else
    {
        return {};
    }
}

void RialtoConnection::initialize()
{
    ServerManagerConfig config;
    config.sessionServerEnvVars = getEnvironmentVariables();
    m_serverManager = rialto::servermanager::service::create(shared_from_this(), config);
    const char *socketPath = std::getenv(RIALTO_SERVERMANAGER_SOCKET_PATH_ENV);
    if (socketPath != nullptr)
    {
        m_socketPath = socketPath;
    }
    if (!m_socketPath.empty() && (m_socketPath.back() != '/' && m_socketPath.back() != '\\'))
    {
#ifdef _WIN32
        m_socketPath += "\\";
#else
        m_socketPath += "/";
#endif
    }
    m_ready = true;
}

bool RialtoConnection::activate(const std::string &id)
{
    if (!m_ready)
    {
        DBGE("RialtoServerManager not initialized!");
        return false;
    }
    auto state = getState(id);
    if (state == SessionServerState::NOT_RUNNING)
    {
        DBG("Starting session " << id << " state " << stateStr(state) << " --> " <<
                                stateStr(SessionServerState::ACTIVE));
        return m_serverManager->initiateApplication(id,
                                                    SessionServerState::ACTIVE,
                                                    {m_socketPath + id});
    }
    else if (state == SessionServerState::INACTIVE)
    {
        DBG("Activating session " << id << " state " << stateStr(state) << " --> "
                                << stateStr(SessionServerState::ACTIVE));
        return m_serverManager->changeSessionServerState(id,
                                                         SessionServerState::ACTIVE);
    }
    else if (state == SessionServerState::ACTIVE)
    {
        DBG("Session already activate");
        return true;
    }
    DBGE("Attempted activate while in state " << stateStr(state));
    return false;
}

bool RialtoConnection::deactivate(const std::string &id)
{
    if (!m_ready)
    {
        DBGE("RialtoServerManager not initialized!");
        return false;
    }
    auto state = getState(id);
    if (state == SessionServerState::NOT_RUNNING)
    {
        DBG("Deactivating not-running session " << id << " state " << stateStr(state) <<
                                    " --> " << stateStr(SessionServerState::INACTIVE));
        return m_serverManager->initiateApplication(id,
                                                    SessionServerState::INACTIVE,
                                                    {m_socketPath + id});
    }
    else if (state == SessionServerState::ACTIVE)
    {
        DBG("Deactivating session " << id << " state " << stateStr(state) <<
                                    " --> " << stateStr(SessionServerState::INACTIVE));
        return m_serverManager->changeSessionServerState(id,
                                                         SessionServerState::INACTIVE);
    }
    else if (state == SessionServerState::INACTIVE)
    {
        DBG("Session already deactivated");
        return true;
    }
    DBGE("Attempted deactivate while in state " << stateStr(state));
    return false;
}

SessionServerState RialtoConnection::getState(const std::string &id)
{
    auto state = m_states.find(id);
    if (state != m_states.end())
    {
        return state->second;
    }
    return SessionServerState::NOT_RUNNING;
}

bool RialtoConnection::stop(const std::string &id)
{
    if (!m_ready)
    {
        DBGE("RialtoServerManager not initialized!");
        return false;
    }
    auto state = getState(id);

    if (SessionServerState::ACTIVE == state ||
        SessionServerState::INACTIVE == state)
    {
        DBG("Stopping session " << id << " state " << stateStr(state) <<
                                " --> " << stateStr(SessionServerState::NOT_RUNNING));
        return m_serverManager->changeSessionServerState(id,
                                                         SessionServerState::NOT_RUNNING);
    }
    else if (SessionServerState::NOT_RUNNING == state)
    {
        DBGE("Session already stopped");
        return true;
    }
    else
    {
        DBGE("Attempted stop while in state " << stateStr(state));
        return false;
    }
}

std::string RialtoConnection::getAppConnectionInfo(const std::string &id)
{
    if (!m_ready)
    {
        DBGE("RialtoServerManager not initialized!");
        return "";
    }
    return m_serverManager->getAppConnectionInfo(id);
}

void RialtoConnection::stateChanged(const std::string &id, const SessionServerState &state)
{
    auto old_state = getState(id);
    DBG("Statechange " << id << " : " << stateStr(old_state) << " --> " << stateStr(state));
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        m_states[id] = state;
    }
    if (m_stateChangeCb)
    {
        m_stateChangeCb(id, stateStr(state));
    }
}