cmake_minimum_required(VERSION 3.0.0)
project(pathTracing VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS  "-w -O3")

# Flags for compiling
if(NO_OMP STREQUAL 1)
    message(STATUS "OpenMP disabled.")
    add_definitions(-DNO_OMP)

else()
    message(STATUS "OpenMP enabled.")
    message("OPENMP FOUND")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
    remove_definitions(-DNO_OMP)
endif()

# Find OpenMP
find_package(OpenMP)
include_directories(${OpenMP_CXX_INCLUDE_DIRS})

if(NO_OMP STREQUAL 0 AND OpenMP_FOUND)
    message(STATUS "OpenMP Found.")

endif()

# Add include path
include_directories(
        src
        3rdLibs
        include
)

# Find source files
file(GLOB_RECURSE SOURCE_FILES
        src/*.cpp
        include/*.hpp)


# Specific flags
if(WIN32)
    add_definitions(-D_AMD64_)
endif()

# Executables
add_executable(pathTracing ${SOURCE_FILES} include/Camera.hpp include/Ray.hpp include/utils.hpp src/Camera.cpp src/test_components.cpp include/test_componet.hpp include/geometry.hpp include/BRDF.hpp include/Interaction.hpp src/BRDF.cpp src/geometry.cpp include/scene.hpp include/light.hpp src/light.cpp src/scene.cpp src/Integrator.cpp include/Integrator.hpp src/utils.cpp src/Config.cpp include/AABB.hpp src/AABB.cpp include/objloader.hpp src/objloader.cpp)
# Link OpenMP
if(NO_OMP STREQUAL 0)
    target_link_libraries(pathTracing OpenMP::OpenMP_CXX)
endif()