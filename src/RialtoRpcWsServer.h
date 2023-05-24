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
#ifndef __RIALTO_RPC_SERVER_H__
#define __RIALTO_RPC_SERVER_H__

#include <functional>
#include <memory>
#include <string>
#include <regex>

#include <json/json.h>

#include "Logging.h"
#include "RialtoConnection.h"

#include <rpcserver/IAbstractRpcServer.h>
#include <rpcserver/WsRpcServerBuilder.h>
#include <jsonrpccpp/common/errors.h>
#include <jsonrpccpp/common/exception.h>

#define RPC_METHOD_BASE "com.libertyglobal.rdk.rialto.1."

using namespace rpcserver;

/***************************************************************
 *
 * *************************************************************/
class RialtoRpcWsServer
{
private:
    void emitRialtoManagerStateChangeEvent(const std::string &id,const std::string &state)
    {
        Json::Value eventJson;
        eventJson["id"] = id;
        eventJson["state"] = state;
        m_wsRpcServer->onEvent("StateChangeEvent", eventJson);
    }
    bool isIdAcceptable(const std::string& id)
    {
        std::regex regex("[^A-Za-z0-9._-]");
        return !id.empty() && !std::regex_search(id, regex);
    }
    std::string idFromRequest(const Json::Value &request)
    {
        if (!request.isMember("id"))
        {
            throw jsonrpc::JsonRpcException(1, "BAD_ARGUMENTS");
        }
        auto id = request["id"].asString();
        if (!isIdAcceptable(id))
        {
            DBG("Id >>" << id << "<< is not acceptable");
            throw jsonrpc::JsonRpcException(1, "BAD_ARGUMENTS");
        }
        return id;
    }

public:
    /***************************************************************
     *
     * *************************************************************/
    explicit RialtoRpcWsServer(uint16_t port)
    {
        std::string registerMethodName(RPC_METHOD_REGISTER);
        std::string unregisterMethodName(RPC_METHOD_UNREGISTER);
        std::string getListenersMethodName(RPC_METHOD_GET_LISTENERS);

        WsRpcServerBuilder serverBuilder(port, true);
        m_wsRpcServer =
                std::shared_ptr<IAbstractRpcServer>(
                        serverBuilder
                                .enableServerEvents(registerMethodName, unregisterMethodName, getListenersMethodName)
                                .numThreads(1)
                                .build()
                );

        m_rialtoServerManager = std::make_shared<RialtoConnection>();
        m_rialtoServerManager->initialize();
        m_rialtoServerManager->setStateChangeCallback([this](const std::string& id, const std::string& state){
            emitRialtoManagerStateChangeEvent(id, state);
        });
        m_wsRpcServer->bindMethod(RPC_METHOD_ACTIVATE,
                                  [this](const Json::Value &request, Json::Value &response)
                                  {
                                      DBG(RPC_METHOD_ACTIVATE << " " << request);
                                      auto id = idFromRequest(request);
                                      response = Json::Value();
                                      if (!m_rialtoServerManager->activate(id))
                                      {
                                          throw jsonrpc::JsonRpcException(2, "ERROR_GENERAL");
                                      }
                                      response["socket"] = m_rialtoServerManager->getAppConnectionInfo(id);
                                  });

        m_wsRpcServer->bindMethod(RPC_METHOD_STOP,
                                  [this](const Json::Value &request, Json::Value &response)
                                  {
                                      DBG(RPC_METHOD_STOP << " " << request);
                                      auto id = idFromRequest(request);
                                      response = Json::Value();
                                      if (!m_rialtoServerManager->stop(id))
                                      {
                                          throw jsonrpc::JsonRpcException(2, "ERROR_GENERAL");
                                      }
                                  });

        m_wsRpcServer->bindMethod(RPC_METHOD_DEACTIVATE,
                                  [this](const Json::Value &request, Json::Value &response)
                                  {
                                      DBG(RPC_METHOD_DEACTIVATE << " " << request);
                                      auto id = idFromRequest(request);
                                      response = Json::Value();
                                      if (!m_rialtoServerManager->deactivate(id))
                                      {
                                          throw jsonrpc::JsonRpcException(2, "ERROR_GENERAL");
                                      }
                                      response["socket"] = m_rialtoServerManager->getAppConnectionInfo(id);
                                  });

        m_wsRpcServer->bindMethod(RPC_METHOD_GETSTATE,
                                  [this](const Json::Value &request, Json::Value &response)
                                  {
                                      DBG(RPC_METHOD_GETSTATE << " " << request);
                                      auto id = idFromRequest(request);
                                      response = Json::Value();
                                      response["state"] = stateStr(m_rialtoServerManager->getState(id));
                                  });

    }

    /***************************************************************
     *
     * *************************************************************/
    RialtoRpcWsServer &operator=(const RialtoRpcWsServer &) = delete;

    /***************************************************************
     *
     * *************************************************************/
    RialtoRpcWsServer(const RialtoRpcWsServer &) = delete;

    /***************************************************************
     *
     * *************************************************************/
    virtual ~RialtoRpcWsServer()
    {
        stop();
    }

    /***************************************************************
     *
     * *************************************************************/
    void start()
    {
        m_wsRpcServer->StartListening();
    }

    /***************************************************************
     *
     * *************************************************************/
    void stop()
    {
        m_wsRpcServer->StopListening();
    }

protected:

    static constexpr auto RPC_METHOD_REGISTER = RPC_METHOD_BASE "register";
    static constexpr auto RPC_METHOD_UNREGISTER = RPC_METHOD_BASE "unregister";
    static constexpr auto RPC_METHOD_GET_LISTENERS = RPC_METHOD_BASE "getListeners";
    static constexpr auto RPC_METHOD_ACTIVATE = RPC_METHOD_BASE "activate";
    static constexpr auto RPC_METHOD_STOP = RPC_METHOD_BASE "stop";
    static constexpr auto RPC_METHOD_DEACTIVATE = RPC_METHOD_BASE "deactivate";
    static constexpr auto RPC_METHOD_GETSTATE = RPC_METHOD_BASE "getState";

    std::shared_ptr<IAbstractRpcServer> m_wsRpcServer;
    std::shared_ptr<RialtoConnection> m_rialtoServerManager;
};

#endif
