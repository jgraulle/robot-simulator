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

ExternalProject_Add(libjson-rpc-cpp
    GIT_REPOSITORY https://github.com/cinemast/libjson-rpc-cpp.git
    GIT_TAG v1.4.1
    CMAKE_ARGS -DTCP_SOCKET_SERVER=YES -DTCP_SOCKET_CLIENT=YES -DHTTP_SERVER=NO
        -DHTTP_CLIENT=NO -DREDIS_SERVER=NO -DREDIS_CLIENT=NO -DCOMPILE_TESTS=NO
        -DCOMPILE_STUBGEN=NO -DCOMPILE_EXAMPLES=NO -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}
        -DBUILD_SHARED_LIBS=OFF -DBUILD_STATIC_LIBS=ON -DWITH_COVERAGE=OFF
)
ExternalProject_Add_StepDependencies(libjson-rpc-cpp build Jsoncpp)

link_directories(${CMAKE_BINARY_DIR}/lib)
include_directories(${CMAKE_BINARY_DIR}/include)

add_executable(robotSimulator
    src/main.cpp
    src/robot.cpp
    src/irProximitySensor.cpp
    src/map.cpp
    src/lineTrackSensor.cpp
    src/switchSensor.cpp
    src/ultrasonicSensor.cpp
    src/speedSensor.cpp
    src/robotCommand.cpp
)

target_link_libraries(robotSimulator jsonrpccpp-server jsonrpccpp-common jsoncpp sfml-graphics
    sfml-window sfml-system)
if (WIN32)
    target_link_libraries(robotSimulator ws2_32)
endif ()
add_dependencies(robotSimulator Jsoncpp libjson-rpc-cpp)

target_compile_options(robotSimulator PRIVATE
  "$<${gcc_like_cxx}:-Wall;-Wextra;-Wshadow;-Wformat=2;-Wunused>"
  "$<${msvc_cxx}:-W3>"
)

#add_compile_definitions(COLLISION_DEBUG)
