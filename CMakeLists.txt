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
)

target_link_libraries(robotSimulator sfml-graphics sfml-window sfml-system)

target_compile_options(robotSimulator PRIVATE
  "$<${gcc_like_cxx}:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused>"
  "$<${msvc_cxx}:-W3>"
)
