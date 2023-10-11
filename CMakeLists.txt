cmake_minimum_required(VERSION 3.0)
project(nmsmc)

# Add your source files
set(SOURCES
    src/fs_utils.c
    src/misc.c
    src/definition.c
    src/main.c
)

# Check if the system is not Windows
if(NOT CMAKE_SYSTEM_NAME MATCHES "Windows")
    list(APPEND SOURCES src/spawn.c)
endif()

# Set the executable output
add_executable(nmsmc ${SOURCES})

# Find libxml2
find_package(LibXml2 REQUIRED)

# Include libxml2 headers
include_directories(${LIBXML2_INCLUDE_DIR})

# Link against libxml2
target_link_libraries(nmsmc PRIVATE ${LIBXML2_LIBRARIES})

# Enable "strip" for the executable
if(CMAKE_COMPILER_IS_GNUCXX)
    add_custom_command(TARGET nmsmc POST_BUILD
        COMMAND strip $<TARGET_FILE:nmsmc>
    )
endif()
