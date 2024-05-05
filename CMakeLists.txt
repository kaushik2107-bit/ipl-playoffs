cmake_minimum_required(VERSION 3.27)
project(ipl VERSION 0.1 LANGUAGES CXX)

# ---- Set C++ standard ----
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---- Add your source files ----
file(GLOB_RECURSE SRCS src/*.c src/*.cpp src/*.h)

# ---- Name of executable ----
set(EXEC "ipl")

# ---- Define the executable target ----
add_executable(${EXEC} ${SRCS})

# ---- Set compiler flags for Debug and Release builds ----
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    # ---- Debug build ----
    message("Debug build")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -Wall -Wextra -g")
    # set(CMAKE_EXE_LINKER_FLAGS " -static")
    else()
    # ---- Release build ----
    message("Release build")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast")
    # set(CMAKE_EXE_LINKER_FLAGS " -static")
endif()

# ---- Specify additional flags for specific compilers or platforms ----
if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
elseif (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
endif()

target_link_libraries(${EXEC} -lreadline -ltinfo -static-libgcc -static-libstdc++)
