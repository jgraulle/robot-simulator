cmake_minimum_required(VERSION 3.15)

project(robotSimulator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
#set(CMAKE_VERBOSE_MAKEFILE ON)

set(gcc_like_cxx "$<COMPILE_LANG_AND_ID:CXX,ARMClang,AppleClang,Clang,GNU,LCC>")
set(msvc_cxx "$<COMPILE_LANG_AND_ID:CXX,MSVC>")

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

find_package(jsoncpp REQUIRED)
find_package(SFML COMPONENTS graphics window system REQUIRED)
target_link_libraries(robotSimulator jsoncpp_lib sfml-graphics sfml-window sfml-system)
if (WIN32)
    target_link_libraries(robotSimulator ws2_32)
endif ()

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
