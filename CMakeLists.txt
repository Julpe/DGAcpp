cmake_minimum_required(VERSION 3.21)

set(CMAKE_INSTALL_PREFIX "../build/")
set(CMAKE_BUILD_TYPE "Debug")
#set(CMAKE_PREFIX_PATH "/usr/")
set(EXECUTABLE_OUTPUT_PATH "../")
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(XTENSOR_USE_XSIMD 1)
set(XTENSOR_USE_OPENMP 1)
set(HIGHFIVE_USE_XTENSOR 1)
set(HIGHFIVE_USE_BOOST 0)
set(HIGHFIVE_USE_INSTALL_DEPS 0)

project(DGAcpp)

find_package(xtl REQUIRED)
find_package(xtensor REQUIRED)
find_package(HighFive REQUIRED)
find_package(HDF5 REQUIRED)
find_package(MPI REQUIRED)
find_package(YAML-CPP REQUIRED)

if(MSVC)
    set(CMAKE_EXE_LINKER_FLAGS /MANIFEST:NO)
endif()

file(GLOB SOURCES "src/*.cpp")
add_executable(DGAcpp ${SOURCES})

target_include_directories(DGAcpp PUBLIC "header_files" "src")
target_compile_options(DGAcpp PUBLIC -march=native -Wall -mavx2 -ffast-math) #-O3)
target_link_libraries(DGAcpp PUBLIC xtensor xtensor::optimize xtensor::use_xsimd HighFive yaml-cpp MPI::MPI_CXX)