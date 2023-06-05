# If not stated otherwise in this file or this component's LICENSE file the
# following copyright and licenses apply:
#
# Copyright 2023 Liberty Global Service B.V.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

cmake_minimum_required(VERSION 3.5)
project(rialto-servermanager)

find_package(PkgConfig)

set(CMAKE_CXX_STANDARD 14)
set(CXX_STANDARD_REQUIRED YES)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

include_directories(src)

pkg_search_module(JSONCPP REQUIRED "jsoncpp")
pkg_search_module(JSONRPCCPP_CLIENT REQUIRED "libjsonrpccpp-client")
find_package(rpcserver REQUIRED)

include_directories(
    ${JSONCPP_INCLUDE_DIRS}
    ${JSONRPCCPP_CLIENT_INCLUDE_DIRS}
)

link_directories(
    ${JSONCPP_LIBRARY_DIRS}
    ${JSONRPCCPP_CLIENT_LIBRARY_DIRS}
)

set(SOURCE_FILES
        src/Main.cpp
        src/RialtoRpcWs.cpp
        src/RialtoConnection.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME}
        ${JSONCPP_LIBRARIES}
        ${JSONRPCCPP_CLIENT_LIBRARIES}
        rpcserver
        RialtoServerManager
        )

install (TARGETS ${PROJECT_NAME} DESTINATION bin)
