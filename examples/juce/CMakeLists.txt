cmake_minimum_required(VERSION 3.21)
project(EventSystem VERSION 0.0.1)

#including CPM.cmake, a package manager:
#https://github.com/TheLartians/CPM.cmake
include(CMake/CPM.cmake)

#Minimum MacOS target, set globally
set(CMAKE_OSX_DEPLOYMENT_TARGET "10.9" CACHE STRING "Minimum OS X deployment version" FORCE)

option(UniversalBinary "Build universal binary for mac" OFF)

if (UniversalBinary)
    set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64" CACHE INTERNAL "")
endif()

#static linking in Windows
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

#Adds all the module sources so they appear correctly in the IDE
set_property(GLOBAL PROPERTY USE_FOLDERS YES)
option(JUCE_ENABLE_MODULE_SOURCE_GROUPS "Enable Module Source Groups" ON)

#set any of these to "ON" if you want to build one of the juce examples
#or extras (Projucer/AudioPluginHost, etc):
option(JUCE_BUILD_EXTRAS "Build JUCE Extras" OFF)
option(JUCE_BUILD_EXAMPLES "Build JUCE Examples" OFF)

#Fetching JUCE from git
#IF you want to instead point it to a local version, you can invoke CMake with
#-DCPM_JUCE_SOURCE="Path_To_JUCE"
CPMAddPackage(
        NAME JUCE
        GITHUB_REPOSITORY juce-framework/JUCE
        GIT_TAG origin/master)

CPMAddPackage (
    NAME raw_keyboard_module
    GITHUB_REPOSITORY izzyreal/juce-raw-keyboard-input-module
    GIT_TAG origin/main
)

juce_add_gui_app(EventSystem)

set(SourceFolders
    Source)

set(SourceFiles

    Source/Main.cpp
    Source/MainComponent.cpp
    Source/MainComponent.h
    ../../EventSystem.h
)

set(JUCE_DEPENDENCIES
    juce::juce_gui_basics
)

target_compile_features(EventSystem PRIVATE cxx_std_17)

target_sources(EventSystem PRIVATE ${SourceFiles})

set_target_properties(EventSystem PROPERTIES FOLDER "")

target_include_directories(EventSystem PRIVATE "${SourceFolders}")

target_link_libraries(EventSystem
    PRIVATE
    ${JUCE_DEPENDENCIES}
    raw_keyboard_input
    PUBLIC
    juce::juce_recommended_config_flags
    juce::juce_recommended_lto_flags
    juce::juce_recommended_warning_flags
)
