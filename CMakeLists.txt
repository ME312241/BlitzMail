cmake_minimum_required(VERSION 3.10)
project(BlitzMail VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${OPENGL_INCLUDE_DIRS}
    ${GLUT_INCLUDE_DIRS}
)

# Source files
set(SOURCES
    OpenGL3DTemplate.cpp
)

# Header files
set(HEADERS
    ModelLoader.h
    glut.h
)

# Create executable
add_executable(BlitzMail ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(BlitzMail
    ${OPENGL_LIBRARIES}
    ${GLUT_LIBRARIES}
)

# Platform-specific settings
if(WIN32)
    # Windows-specific settings
    set_target_properties(BlitzMail PROPERTIES
        WIN32_EXECUTABLE OFF  # Keep console window
    )
    
    # Copy GLUT DLLs to output directory
    add_custom_command(TARGET BlitzMail POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_CURRENT_SOURCE_DIR}/glut32.dll"
        $<TARGET_FILE_DIR:BlitzMail>
        COMMENT "Copying GLUT DLL"
    )
    
elseif(UNIX AND NOT APPLE)
    # Linux-specific settings
    target_link_libraries(BlitzMail m)  # Link math library
    
elseif(APPLE)
    # macOS-specific settings
    find_library(COCOA_LIBRARY Cocoa)
    find_library(IOKIT_LIBRARY IOKit)
    target_link_libraries(BlitzMail ${COCOA_LIBRARY} ${IOKIT_LIBRARY})
endif()

# Copy models directory to build directory
add_custom_command(TARGET BlitzMail POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/models"
    "$<TARGET_FILE_DIR:BlitzMail>/models"
    COMMENT "Copying models directory"
)

# Installation rules
install(TARGETS BlitzMail DESTINATION bin)
install(DIRECTORY models DESTINATION bin)

# Print configuration summary
message(STATUS "")
message(STATUS "BlitzMail Configuration Summary")
message(STATUS "================================")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "OpenGL: ${OPENGL_LIBRARIES}")
message(STATUS "GLUT: ${GLUT_LIBRARIES}")
message(STATUS "")

# Add build instructions
message(STATUS "Build Instructions:")
message(STATUS "  mkdir build && cd build")
message(STATUS "  cmake ..")
message(STATUS "  cmake --build .")
message(STATUS "  ./BlitzMail  (or BlitzMail.exe on Windows)")
message(STATUS "")
