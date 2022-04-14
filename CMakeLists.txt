# CyberpunkToolkit/
cmake_minimum_required (VERSION 3.20)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/307lib/307modules")

set(ENV{CyberpunkToolkit_VERSION} "0.0.0")

include(VersionTag)
GET_VERSION_TAG("${CMAKE_CURRENT_SOURCE_DIR}" "CyberpunkToolkit")

project ("CyberpunkToolkit" VERSION "${CyberpunkToolkit_VERSION}" LANGUAGES CXX)

# Include dependencies
add_subdirectory("307lib")
# Include executable project
add_subdirectory("cyberkit")
