cmake_minimum_required(VERSION 3.14)
project(FAT_phy_gas_task C)

set(CMAKE_C_STANDARD 11)

add_executable(FAT_phy_gas_task main.c Atom.c new.c MathCore.c Class.c PhySystem.c)
target_link_libraries(FAT_phy_gas_task m)