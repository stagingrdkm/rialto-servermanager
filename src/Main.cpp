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
#include "RialtoRpcWs.h"
#include "Logging.h"
#include <csignal>
#include <condition_variable>
#include <mutex>
#include <atomic>

std::atomic<bool> should_quit{false};
std::mutex mtx;
std::condition_variable cv;

void sig_handler(int sig_num)
{
    DBG("Signal received " << sig_num);
    if (sig_num == SIGTERM || sig_num == SIGINT)
    {
        {
            std::lock_guard<std::mutex> lock(mtx);
            should_quit = true;
        }
        cv.notify_one();
    }
}

int main(int argc, char **argv)
{
    DBG_IN();
    if (initWsRpc(argc, argv)) {
        closeWsRpc();
        DBG_OUT();
        return -1;
    }

    std::signal(SIGTERM, sig_handler);
    std::signal(SIGINT, sig_handler);

    {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [] { return should_quit.load(); });
    }

    closeWsRpc();
    DBG_OUT();
    return 0;
}
