# CyberpunkToolkit/cyberkit
cmake_minimum_required (VERSION 3.20)

include (VersionTag)
MAKE_VERSION_HEADER("${CMAKE_CURRENT_SOURCE_DIR}/resources/version.h" "cyberkit" "${CyberpunkToolkit_VERSION}")

file(GLOB HEADERS
	RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
	CONFIGURE_DEPENDS
	"*.h*"
)
file(GLOB SRCS
	RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}"
	CONFIGURE_DEPENDS
	"*.c*"
)

if (WIN32)
	include(ResourceMaker)
	set(cyberkit_RC "${CMAKE_CURRENT_SOURCE_DIR}/resources/cyberkit.rc")
	MAKE_STRINGRC_VERSIONINFO(cyberkit_resource_version, "${CyberpunkToolkit_VERSION}" "GPLv3" "radj307" "cyberkit" "Commandline toolkit for working with Cyberpunk2077 mods.")
	MAKE_RESOURCE("${cyberkit_RC}" "${cyberkit_resource_version}")
endif()

add_executable(cyberkit "${SRCS}" "${cyberkit_RC}")

set_property(TARGET cyberkit PROPERTY CXX_STANDARD 20)
set_property(TARGET cyberkit PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET cyberkit PROPERTY POSITION_INDEPENDENT_CODE ON)
if (MSVC)
	target_compile_options(cyberkit PRIVATE "/Zc:__cplusplus" "/Zc:preprocessor")
endif()

target_sources(cyberkit PRIVATE "${HEADERS}")

target_link_libraries(cyberkit PRIVATE TermAPI filelib optlib)
