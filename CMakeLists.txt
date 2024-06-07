cmake_minimum_required(VERSION 3.3)
project(x11_gui)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -Wall -std=c++14 -fPIC")
add_definitions(-DTEST_ENABLE)

include_directories(src src/lib src/lib/3dr_party)
file(GLOB_RECURSE ALL_LIBRIRY_SRCS "src/*.c*")

set(THIRD_PARTY_LIBS_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src/lib/3dr_party/libs")
link_directories(${THIRD_PARTY_LIBS_PATH})
add_executable(${PROJECT_NAME} ${ALL_LIBRIRY_SRCS})
target_link_libraries(${PROJECT_NAME} PRIVATE "${THIRD_PARTY_LIBS_PATH}/libXext.so.6.4.0" "${THIRD_PARTY_LIBS_PATH}/libX11.so.6.3.0")
