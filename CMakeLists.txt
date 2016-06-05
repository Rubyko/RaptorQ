cmake_minimum_required(VERSION 3.5.1)
project(RaptorQ)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

#-fprofile-use -fprofile-correction     ||| -fprofile-generate    http://stackoverflow.com/questions/4365980/how-to-use-profile-guided-optimizations-in-g    -fno-rtti  -fstack-protector-all -fstrict-aliasing -ftrapv -fwrapv

set(GCC_COVERAGE_COMPILE_FLAGS -std=c++14 -Ofast -Wall -pedantic)  
set(GCC_COVERAGE_LINK_FLAGS    			)

set(CXX_COMPILER_FLAGS ${CXX_COMPILER_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS})
set(CXX_LINKER_FLAGS ${CXX_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS})

find_package(Threads REQUIRED)
find_package(Eigen3 REQUIRED)

include_directories(SYSTEM ${EIGEN3_INCLUDE_DIR})
include_directories(include)
include(cotire)

set(SOURCES src/stub.cpp)

add_library(RaptorQ_Static STATIC ${SOURCES} )
target_compile_options(RaptorQ_Static PRIVATE ${CXX_COMPILER_FLAGS})
target_link_libraries(RaptorQ_Static PRIVATE ${CXX_LINKER_FLAGS} pthread)
cotire(RaptorQ_Static)

add_executable(Decoder test/complete.cpp)	
target_compile_options(Decoder PRIVATE ${CXX_COMPILER_FLAGS})
add_dependencies(Decoder RaptorQ_Static)
target_link_libraries(Decoder PRIVATE ${CXX_LINKER_FLAGS} RaptorQ_Static)
cotire(Decoder)











