cmake_minimum_required(VERSION 3.10)

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(lab
    LANGUAGES CXX C
    DESCRIPTION "Data structure course lab."
)

# -------------------------- miniSTL --------------------------

set(miniSTL_SOURCE_DIR miniSTL)
add_subdirectory(${miniSTL_SOURCE_DIR})

# set(miniSTL_DIR "D:/Projects/cpp/hyx-lab/miniSTL/lib/share/miniSTL/cmake")
# find_package(miniSTL REQUIRED)

#--------------------------- Project -----------------------

file(GLOB airplane_sources project/src/*.cpp project/include/*.hpp)

add_executable(airplane ${airplane_sources})

target_include_directories(airplane PRIVATE project/include)
target_link_libraries(airplane PRIVATE miniSTL::miniSTL)