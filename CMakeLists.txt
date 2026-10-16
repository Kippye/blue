cmake_minimum_required(VERSION 3.31)
include(FetchContent)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

############ PROJECT DEFINITION ############
project(blue VERSION 1.3.0)
set(CPP_VERSION "c++20")
set(CMAKE_CXX_STANDARD 20)

# Force even Release builds to show and output to a console
option(FORCE_CONSOLE "Should the console be visible even in Release builds?" OFF)

######### GLOBALS #########
set(PROGRAM_NAME ${PROJECT_NAME})
set(BLUE_EXPERIMENTAL 0)
### SUBDIRS ###
set(LIBRARY_DIR "libs")
set(BLUE_SRC    "source")
### INSTALL ###
set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install")
### PACKAGING ###
set(PACKAGING_DIR "${CMAKE_SOURCE_DIR}/packaging")

######### MAIN TARGET #########
add_executable(${PROGRAM_NAME})

######### PLATFORM #########
include("cmake/platform.cmake")

######### PACKAGES #########
message("~~~~ Configuring ${PROGRAM_NAME} ${PROJECT_VERSION}")
### LINUX PACKAGES / LIBRARIES ###
IF (LINUX)
    find_package(PkgConfig REQUIRED)
    # glib2
    # pkg_check_modules(deps REQUIRED IMPORTED_TARGET glib-2.0)
    # gdkpixbuf
    # find_package(GDKPixBuf REQUIRED)
    # pkg_check_modules(pixbuf REQUIRED IMPORTED_TARGET gdk-pixbuf-2.0)
ENDIF()

######### REMOTE FETCH LIBRARIES #########
FetchContent_Declare(
    GLFW
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG e7ea71be039836da3a98cea55ae5569cb5eb885c # latest as of 23.06.2025; GLFW 3.5
)
FetchContent_GetProperties(GLFW)
IF (NOT glfw_POPULATED)
  FetchContent_MakeAvailable(GLFW)
#   add_subdirectory(${glfw_SOURCE_DIR} ${glfw_BINARY_DIR} EXCLUDE_FROM_ALL)
ENDIF()
# FetchContent_MakeAvailable(GLFW)
######### SUBDIRS / LOCAL LIBRARIES #########
add_subdirectory("${LIBRARY_DIR}/blf" EXCLUDE_FROM_ALL)
set_target_properties(blf PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS OFF
)
add_subdirectory("${LIBRARY_DIR}/glad")
add_subdirectory("${LIBRARY_DIR}/glm")
set(BUILD_TESTS OFF CACHE BOOL "Enable tests" FORCE)
set(BUILD_EXAMPLES OFF CACHE BOOL "Enable examples" FORCE)
add_subdirectory("${LIBRARY_DIR}/libconfig")
add_subdirectory("${LIBRARY_DIR}/rectpack2D")

add_subdirectory("${LIBRARY_DIR}/imgui") # imgui 1.91.9b
set(imgui_FOUND TRUE)
target_link_libraries(imgui glfw) # imgui is dependent on glfw and causes a compile error if not linked with it here
message(STATUS "~~ v Don't listen to ImGuiFileDialog - imgui is linked to it.")
add_subdirectory("${LIBRARY_DIR}/ImGuiFileDialog")
target_link_libraries(ImGuiFileDialog imgui)

######### DEFINES #########
### Build configuration ###
target_compile_definitions(${PROGRAM_NAME} PUBLIC "$<$<CONFIG:Debug>:DEBUG>")
target_compile_definitions(${PROGRAM_NAME} PUBLIC "$<$<NOT:$<CONFIG:Debug>>:NDEBUG>")
target_compile_definitions(${PROGRAM_NAME} PUBLIC "$<$<CONFIG:Release>:RELEASE>")
### GLFW no include ###
# target_compile_definitions(${PROGRAM_NAME} PUBLIC "GLFW_INCLUDE_NONE")

######### COMPILE OPTIONS #########
add_library(compiler_flags INTERFACE)
# Add compile options defined by preset (might be empty)
target_compile_options(compiler_flags INTERFACE ${PRESET_ADD_COMPILE_OPTIONS})

message(STATUS "~~ Generator ${CMAKE_GENERATOR} on system ${CMAKE_SYSTEM_NAME}")
message(STATUS "~~ Using CXX compiler ${CMAKE_CXX_COMPILER_ID} version ${CMAKE_CXX_COMPILER_VERSION}")
IF (CMAKE_GENERATOR STREQUAL "Unix Makefiles")
    message(STATUS "~~ Configuration: ${CMAKE_BUILD_TYPE}")
    IF (${CMAKE_BUILD_TYPE} STREQUAL Debug)
        set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    ENDIF()
    ###### WINDOWS OR MSYS SYSTEM ######
    IF (WIN32)
        ### COMPILER FLAGS ###
        # CLANG
        IF (${CMAKE_CXX_COMPILER_ID} STREQUAL Clang)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64 -static -fuse-ld=lld -mtune=native -lpthread -lcomtl32 -static-libstdc++ -std=${CPP_VERSION} -Wa,-mbig-obj")
        ENDIF()
        # GNU (G++)
        IF (${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64 -static -fuse-ld=lld -mtune=native -lpthread -lcomctl32 -static-libstdc++ -static-libgcc -std=${CPP_VERSION} -Wa,-mbig-obj") # Pass  -Wa,-mbig-obj to avoid error for data_converter.cpp due to its size.
        ENDIF()
        message(STATUS "~~ Compile flags for MinGW: ${CMAKE_CXX_FLAGS}")

        ### LINKER FLAGS ###
        # /ENTRY:mainCRTStartup keeps the same "main" function instead of requiring "WinMain"
        IF ((${CMAKE_BUILD_TYPE} STREQUAL Debug) OR FORCE_CONSOLE)
            set(SUBSYSTEM_LINKER_OPTIONS "-Wl,-subsystem,console" "-Wl,-entry,mainCRTStartup")
        ELSEIF(${CMAKE_BUILD_TYPE} STREQUAL Release)
            set(SUBSYSTEM_LINKER_OPTIONS "-Wl,-subsystem,windows" "-Wl,-entry,mainCRTStartup")
        ENDIF()
        message(STATUS "~~ MinGW linker options: ${SUBSYSTEM_LINKER_OPTIONS}")
    ###### LINUX SYSTEM ######
    ELSEIF(LINUX)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=${CPP_VERSION} -lstdc++")
        message(STATUS "~~ Compile flags for Linux Makefiles: ${CMAKE_CXX_FLAGS}")
    ENDIF()
ELSEIF (CMAKE_GENERATOR MATCHES "Visual Studio")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:${CPP_VERSION} /bigobj") # Pass /bigobj to avoid C1128 for data_converter.cpp due to its templates.
    message(STATUS "~~ Compile flags for Visual Studio: ${CMAKE_CXX_FLAGS}")
ENDIF()

######### LINK OPTIONS #########
target_link_options(${PROGRAM_NAME} PRIVATE ${SUBSYSTEM_LINKER_OPTIONS})
IF (WINDOWS_MSVC)
    target_link_options(${PROGRAM_NAME} PRIVATE
        $<$<OR:$<CONFIG:Debug>,$<BOOL:${FORCE_CONSOLE}>>:/SUBSYSTEM:CONSOLE /ENTRY:mainCRTStartup>
        $<$<AND:$<CONFIG:Release>,$<NOT:$<BOOL:${FORCE_CONSOLE}>>>:/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup>
    )
ENDIF()

######### INCLUDES #########
# Pass information like PROGRAM_NAME, PROJECT_VERSION_MAJOR, PROJECT_VERSION_MINOR and PROJECT_VERSION_PATCH to a header file to include
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/include/program_info.h.in" "${CMAKE_CURRENT_SOURCE_DIR}/include/generated/program_info.h")

target_include_directories(${PROGRAM_NAME}
    PUBLIC "include" 
    PUBLIC "include/generated"
    PUBLIC "libs"
    PUBLIC "libs/ImGuiFileDialog-Lib_Only"
    PUBLIC "libs/libconfig"
    PUBLIC "libs/glm"
    PUBLIC "libs/glm/glm"
    PUBLIC "libs/rectpack2D"
)

###### PLATFORM-SPECIFIC ######
# IF (LINUX)
#     target_include_directories(${PROGRAM_NAME} PUBLIC ${pixbuf_INCLUDE_DIRS})
#     # IF (GDKPIXBUF_FOUND)
#     #     target_include_directories(${PROGRAM_NAME} PUBLIC ${GDKPIXBUF_INCLUDE_DIR})
#     # ENDIF()
# ENDIF()

######### SOURCES #########
target_sources(${PROGRAM_NAME} PRIVATE
    "${BLUE_SRC}/main.cpp"
    "${BLUE_SRC}/blf_converter.cpp"
    "${BLUE_SRC}/camera.cpp"
    "${BLUE_SRC}/editor_tile.cpp"
    "${BLUE_SRC}/editor.cpp"
    "${BLUE_SRC}/file_system.cpp"
    "${BLUE_SRC}/gizmo.cpp"
    "${BLUE_SRC}/gui.cpp"
    "${BLUE_SRC}/input.cpp"
    "${BLUE_SRC}/render.cpp"
    "${BLUE_SRC}/textures.cpp"
    "${BLUE_SRC}/window.cpp"
)

# Include imgui_demo.cpp on non-Release builds if the build type is known
IF (CMAKE_BUILD_TYPE)
    IF (NOT ${CMAKE_BUILD_TYPE} STREQUAL Release)
        target_sources(${PROGRAM_NAME} PRIVATE "${LIBRARY_DIR}/imgui/src/imgui_demo.cpp")
    ENDIF()
ENDIF()

######### LINKS #########
target_link_libraries(${PROGRAM_NAME} PUBLIC compiler_flags 
    blf
    imgui
    ImGuiFileDialog
    glm
    glad 
    glfw
    config++
    rectpack2D
)

### PLATFORM-SPECIFIC ###
IF (WINDOWS_MSVC)

ENDIF(WINDOWS_MSVC)

IF (LINUX)

ENDIF()
