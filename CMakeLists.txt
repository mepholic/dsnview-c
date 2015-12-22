cmake_minimum_required(VERSION 3.3)
project(dsnview)

# start ulver's shit
set(GENERAL_WARNING_FLAGS          "-Wall -Wextra -pedantic -Wno-unused-parameter -Wno-unused-variable -Wno-variadic-macros -Wno-gnu-zero-variadic-macro-arguments")
set(GENERAL_DEBUG_FLAGS            "-g -gdwarf-2")
set(GENERAL_RELEASE_FLAGS          "-mtune=native -DNDEBUG -O2 -fomit-frame-pointer")
set(GENERAL_COMPILER_FLAGS         "-pipe")

set(CMAKE_CXX_FLAGS                "-std=c++11 ${GENERAL_WARNING_FLAGS} ${GENERAL_COMPILER_FLAGS} -Wshadow -Weffc++ -fnon-call-exceptions -Wno-c99-extensions")
set(CMAKE_CXX_FLAGS_DEBUG          "${GENERAL_DEBUG_FLAGS}")
set(CMAKE_CXX_FLAGS_RELEASE        "${GENERAL_RELEASE_FLAGS}")
set(CMAKE_C_FLAGS                  "-std=c11 ${GENERAL_WARNING_FLAGS} ${GENERAL_COMPILER_FLAGS}")
set(CMAKE_C_FLAGS_DEBUG            "${GENERAL_DEBUG_FLAGS}")
set(CMAKE_C_FLAGS_RELEASE          "${GENERAL_RELEASE_FLAGS}")
set(CMAKE_EXE_LINKER_FLAGS         "")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG   "")
set(CMAKE_EXE_LINKER_FLAGS_RELEASE "-s")
# end ulver's shit

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.c dsnapi.c dsnapi.h)
add_executable(dsnview ${SOURCE_FILES})

# Math Library
find_library(M_LIB m)

# Find cURL
find_package(CURL)
include_directories(${CURL_INCLUDE_DIRS})

# Find libxml2
find_package(LibXml2)
include_directories(${LIBXML2_INCLUDE_DIR})

# Link our libraries
target_link_libraries(dsnview ${M_LIB} ${CURL_LIBRARIES} ${LIBXML2_LIBRARIES})