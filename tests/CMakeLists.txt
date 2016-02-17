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

# min config
cmake_minimum_required (VERSION 2.8.1)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14") #enable C++14
set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")

set (SOURCES main.cpp)

include_directories (
    ${PROJECT_SOURCE_DIR}/include
    ${GTEST_INCLUDE_DIR}
)
if (WITH_JSONCPP_BACKEND)
    include_directories (${JSONCPP_INCLUDE_DIR})
endif ()
if (WITH_SQLITE_BACKEND)
    # create the C file juste to make cmake happy
    if (NOT EXISTS "${SQLITE_SRC}")
        file(WRITE "${SQLITE_SRC}" "#error \"you should check you cmake configuration\"")
    endif ()
    include_directories (${SQLITE_INCLUDE_DIR})
    list (APPEND SOURCES ${SQLITE_SRC})
endif ()

add_executable (sad_tests ${SOURCES})
target_link_libraries (sad_tests ${GTEST_LIBRARY_DIR}/libgtest.a)

# link jsoncpp library
if (WITH_JSONCPP_BACKEND)
    target_link_libraries (sad_tests ${JSONCPP_LIBRARY_DIR}/libjsoncpp.a)
endif ()
# link pthread + dl
if (WITH_SQLITE_BACKEND)
    target_link_libraries (sad_tests pthread dl)
endif ()
