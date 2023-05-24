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
#include <exception>
#include <memory>
#include "Logging.h"
#include "RialtoRpcWs.h"
#include "RialtoRpcWsServer.h"

#define WSPORT_RIALTO_SERVERMANAGER_ENV "RIALTO_SERVERMANAGER_WS_PORT"
#define WSPORT_RIALTO_SERVERMANAGER_DEFAULT_VALUE 10150

static std::unique_ptr<RialtoRpcWsServer> server = nullptr;

/***************************************************************
 *
 * *************************************************************/
int initWsInterface(void)
{
    DBG_IN();

    long wsPort = 0;
    const char *wsPortSetting;
    wsPortSetting = std::getenv(WSPORT_RIALTO_SERVERMANAGER_ENV);
    if (wsPortSetting)
    {
        char *endptr;
        errno = 0;
        wsPort = strtol(wsPortSetting, &endptr, 10);
        if (errno != 0 || *endptr != '\0')
        {
            DBGE("bad value for port setting "
                         << wsPortSetting << " of " << WSPORT_RIALTO_SERVERMANAGER_ENV);
            return -1;
        }
    }
    else
    {
        wsPort = WSPORT_RIALTO_SERVERMANAGER_DEFAULT_VALUE;
    }

    if (wsPort <= 0 || wsPort > 65535)
    {
        if (wsPort == 0)
        {
            DBG("websocket interface not enabled because port set to 0");
        }
        else
        {
            DBGE("websocket interface not enabled. The port out of range value "
                         << wsPortSetting << " " << wsPort);
        }

        return -1;
    }

    try
    {
        DBG("Starting websocket interface on port " << wsPort);
        server = std::make_unique<RialtoRpcWsServer>(wsPort);
        server->start();
    }
    catch (const std::exception &e)
    {
        server = nullptr;
        DBGE(e.what());
        return -1;
    }
    catch (...)
    {
        server = nullptr;
        DBGE("unknown exception");
        return -1;
    }

    return 0;
}

int initWsRpc(int, char **)
{
    if (initWsInterface())
    {
        DBGE("Failure starting up rialto-servermanager!")
        return -1;
    }

    DBG("Successfully started up rialto-servermanager!")
    return 0;
}

void closeWsRpc()
{
    server = nullptr;
}
