cmake_minimum_required(VERSION 2.8)
project(QMetaTcl C CXX)

find_package(Qt4 REQUIRED)
find_package(TclStub REQUIRED)

include_directories(${TCL_INCLUDE_PATH} .)

set(COMMON_CFLAGS "-Wall -Wextra -Wno-unused-parameter -DUSE_TCL_STUBS")
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(COMMON_CFLAGS "${COMMON_CFLAGS} -O0")
else(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(COMMON_CFLAGS "${COMMON_CFLAGS} -O2 -s")
endif(CMAKE_BUILD_TYPE STREQUAL "Debug")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COMMON_CFLAGS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_CFLAGS}")

######################################################################

set(SOURCES
    qmetatcl.cpp
    qmetatcl-qvariant.cpp
)

set(HEADERS
    qmetatcl-qvariant.h
)

qt4_wrap_cpp(MOC_SOURCES ${HEADERS})

add_library(qmetatcl SHARED ${HEADERS} ${SOURCES} ${MOC_SOURCES})

target_link_libraries(qmetatcl
    Qt4::QtCore
    ${TCL_STUB_LIBRARY}
)
