cmake_minimum_required(VERSION 3.15)

project(robotSimulator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
#set(CMAKE_VERBOSE_MAKEFILE ON)

set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")

include(ExternalProject)
ExternalProject_Add(Jsoncpp
    GIT_REPOSITORY https://github.com/open-source-parsers/jsoncpp.git
    GIT_TAG 1.9.5
    CMAKE_ARGS -DJSONCPP_WITH_TESTS=OFF -DJSONCPP_WITH_POST_BUILD_UNITTEST=OFF
        -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR} -DBUILD_SHARED_LIBS=OFF
)

link_directories(${CMAKE_BINARY_DIR}/lib)
include_directories(${CMAKE_BINARY_DIR}/include)

add_executable(robotSimulator
    src/main.cpp
    src/robot.cpp
    src/irProximitySensor.cpp
    src/map.cpp
    src/lineTrackSensor.cpp
    src/switchSensor.cpp
    src/ultrasoundSensor.cpp
    src/encoderWheelSensor.cpp
    src/robotCommand.cpp
    src/jsonrpctcpserver.cpp
)

target_link_libraries(robotSimulator jsoncpp sfml-graphics sfml-window sfml-system)
if (WIN32)
    target_link_libraries(robotSimulator ws2_32)
endif ()
add_dependencies(robotSimulator Jsoncpp)

target_compile_options(robotSimulator PRIVATE
  "$<${gcc_like_cxx}:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused>"
  "$<${msvc_cxx}:-W3>"
)

#add_compile_definitions(COLLISION_DEBUG)
#add_compile_definitions(MOTOR_DEBUG)
#add_compile_definitions(JSONRPC_CONNECT_DEBUG)
#add_compile_definitions(JSONRPC_MSG_DEBUG)
if (WIN32)
    add_compile_definitions(_WIN32_WINNT=0x0501)
endif ()
