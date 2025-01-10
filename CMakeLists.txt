cmake_minimum_required(VERSION 3.10)

project(BS LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_executable(BS main.cpp vec3.hpp vec3.cpp color.hpp hittable.hpp sphere.hpp hittable_list.hpp ray.hpp rtweekend.hpp plane.hpp)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS")


target_link_libraries(BS PRIVATE user32)
target_compile_features(BS PRIVATE cxx_std_17)
add_compile_definitions(UNICODE _UNICODE)
