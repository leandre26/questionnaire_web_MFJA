cmake_minimum_required(VERSION 3.20)
project(questionnaire VERSION 1.0.2 LANGUAGES CXX)

# Set default build type to Release if not specified
if(CMAKE_BUILD_TYPE STREQUAL "")
  set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build." FORCE)
endif()
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set_property(CACHE CMAKE_INSTALL_PREFIX PROPERTY VALUE "/usr/local")
endif()

message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
message(STATUS "CMAKE_INSTALL_PREFIX: ${CMAKE_INSTALL_PREFIX}") 

# PROJECT SETTINGS #############################################################
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(SRC_DIR ${CMAKE_CURRENT_LIST_DIR}/src)

# Detect platform
if(UNIX AND NOT APPLE)
  set(LINUX TRUE)
endif()

# DEPENDENCIES #################################################################
include(FetchContent)

FetchContent_Declare(json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG        v3.11.3
  GIT_SHALLOW    TRUE
)

set(FETCHCONTENT_UPDATES_DISCONNECTED TRUE)
FetchContent_MakeAvailable(json)


# BUILD SETTINGS ###############################################################
if (APPLE)
  set(CMAKE_INSTALL_RPATH "@executable_path/../lib")
  include_directories(/opt/homebrew/include)
  link_directories(/opt/homebrew/lib)
else()
  set(CMAKE_INSTALL_RPATH "\$ORIGIN/../lib;/usr/local/lib")
endif()

include_directories(${json_SOURCE_DIR}/include)

include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/include/crow)
include_directories(${CMAKE_SOURCE_DIR}/include/crow/include)
include_directories(${CMAKE_SOURCE_DIR}/include/crow/include/crow)
include_directories(${CMAKE_SOURCE_DIR}/include/crow/include/crow/middlewares)

include_directories(${ZMQ_INCLUDE_DIR})
include_directories(${ZMQ_INCLUDE_DIR}/usr/include)
include_directories(${ZMQ_INCLUDE_DIR}/usr/local/include)

find_package(Threads REQUIRED)
find_library(ZMQ_LIB zmq)

# Add the server executable
add_executable(server ${SRC_DIR}/server.cpp)

# Link dependencies correctly
target_include_directories(server PRIVATE ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(server PRIVATE 
    Threads::Threads 
    ${ZMQ_LIBR}
)

# INSTALL ######################################################################
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set_property(CACHE CMAKE_INSTALL_PREFIX PROPERTY VALUE ${USR_DIR})
endif()
if(APPLE)
  install(TARGETS server
    RUNTIME DESTINATION bin
  )
else()
  install(TARGETS server
    RUNTIME DESTINATION bin
  )
endif()
