# The MIT License (MIT)
# 
# Copyright (c) 2015 Jeremy Letang
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

cmake_minimum_required(VERSION 2.8.8)
include(ExternalProject)

message(STATUS "Configuring googletest")

set (GTEST_VERSION 1.7.0)
set (GTEST_DIR googletest)
set (GTEST_PATH ${CMAKE_BINARY_DIR}/${GTEST_DIR})

ExternalProject_Add (
    googletest
    PREFIX ${GTEST_PATH}
    URL https://googletest.googlecode.com/files/gtest-${GTEST_VERSION}.zip
    TIMEOUT 10
    CONFIGURE_COMMAND ${CMAKE_COMMAND} "-DCMAKE_INSTALL_PREFIX=${GTEST_PATH}" -DBUILD_TYPE=Release <SOURCE_DIR>
    BUILD_IN_SOURCE ON
    UPDATE_COMMAND ""
    BUILD_COMMAND ${CMAKE_MAKE_PROGRAM}
    INSTALL_COMMAND ""
    LOG_DOWNLOAD ON
    LOG_UPDATE ON
    LOG_CONFIGURE ON
    LOG_BUILD ON
)

ExternalProject_Get_Property (googletest SOURCE_DIR)

set (GTEST_INCLUDE_DIR "${SOURCE_DIR}/include")
set (GTEST_LIBRARY_DIR "${SOURCE_DIR}")
