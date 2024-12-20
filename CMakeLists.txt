cmake_minimum_required(VERSION 3.5)

project(WhateverGame LANGUAGES CXX)

add_executable(BS main.cpp vec3.hpp vec3.cpp color.hpp hittable.hpp sphere.hpp hittable_list.hpp ray.hpp rtweekend.hpp)

target_compile_features(BS PRIVATE cxx_std_17)
