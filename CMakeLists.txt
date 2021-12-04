cmake_minimum_required(VERSION 3.20)
project(hpc_game_of_life_mpi C)

set(CMAKE_C_STANDARD 99)

add_executable(hpc_game_of_life_mpi main.c game.c game.h 1dtopologie.c kernel.c kernel.h vec2.c vec2.h)

find_package(MPI REQUIRED)
include_directories(SYSTEM ${MPI_INCLUDE_PATH})

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")

target_link_libraries(hpc_game_of_life_mpi ${MPI_C_LIBRARIES})
