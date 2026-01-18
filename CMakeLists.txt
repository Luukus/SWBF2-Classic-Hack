cmake_minimum_required(VERSION 4.1)
project(SWBF2_Classic_Hack)

set(CMAKE_CXX_STANDARD 20)

# Include base directories
include_directories(SWBF2ClassicInternalHax/external)
include_directories(SWBF2ClassicInternalHax/src)

# Find ALL source files recursively
file(GLOB_RECURSE ALL_FILES
        "SWBF2ClassicInternalHax/*.cpp"
        "SWBF2ClassicInternalHax/*.c"
        "SWBF2ClassicInternalHax/*.h"
        "SWBF2ClassicInternalHax/*.hpp"
)

add_library(SWBF2_Classic_Hack SHARED ${ALL_FILES})