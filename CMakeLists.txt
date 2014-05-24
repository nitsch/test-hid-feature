project( test-hid-feature C )
cmake_minimum_required( VERSION 2.6 )

set( REQUIRED_LIBS )


# build options
option( DEBUG "Build with debug information" OFF )


# debug build configuration
if( DEBUG )
	set( INFO_IS_DEBUG "Yes" )
	add_definitions( -DDEBUG )
	add_definitions( -g -O0 -Wall )
else()
	set( INFO_IS_DEBUG "No" )
endif()


# library dependencies
if( ${CMAKE_SYSTEM_NAME} MATCHES "Darwin" )
	set( INFO_SYSTEM_NAME "MacOS" )

	find_library( FOUNDATION Foundation )
	find_library( AVFOUNDATION AVFoundation )
	find_library( IOKIT IOKit )
	find_library( COREFOUNDATION CoreFoundation )
	find_library( IOBLUETOOTH IOBluetooth )
	list( APPEND REQUIRED_LIBS ${FOUNDATION} )
	list( APPEND REQUIRED_LIBS ${AVFOUNDATION} )
	list( APPEND REQUIRED_LIBS ${IOKIT} )
	list( APPEND REQUIRED_LIBS ${COREFOUNDATION} )
	list( APPEND REQUIRED_LIBS ${IOBLUETOOTH} )

	set(HIDAPI_SOURCE ${PROJECT_SOURCE_DIR}/external/hidapi/mac/hid.c)
elseif( ${CMAKE_SYSTEM_NAME} MATCHES "Linux" )
	set( INFO_SYSTEM_NAME "Linux" )

	include( FindPkgConfig )

	pkg_check_modules( UDEV REQUIRED libudev )
	include_directories( ${UDEV_INCLUDE_DIRS} )
	list( APPEND REQUIRED_LIBS ${UDEV_LIBRARIES} )

	pkg_check_modules( BLUEZ REQUIRED bluez )
	include_directories( ${BLUEZ_INCLUDE_DIRS} )
	list( APPEND REQUIRED_LIBS ${BLUEZ_LIBRARIES} )

	set( HIDAPI_SOURCE ${PROJECT_SOURCE_DIR}/external/hidapi/linux/hid.c )
else()
	message( FATAL_ERROR "Unsupported platform \"" ${CMAKE_SYSTEM_NAME} "\"." )
endif()


# include directories
include_directories( ${PROJECT_SOURCE_DIR}/external/hidapi/hidapi )
include_directories( ${PROJECT_SOURCE_DIR}/src )

# build targets

add_library( hidapi STATIC ${HIDAPI_SOURCE} )
target_link_libraries( hidapi ${REQUIRED_LIBS} )

file( GLOB SOURCE_LIST
	"${PROJECT_SOURCE_DIR}/src/*.h"
	"${PROJECT_SOURCE_DIR}/src/*.c"
)
add_executable( ${PROJECT_NAME} ${SOURCE_LIST} )
target_link_libraries( ${PROJECT_NAME}
	hidapi
	${REQUIRED_LIBS}
)


# print build configuration and info
message( "" )
message( "  Build configuration" )
message( "    Debug build:      " ${INFO_IS_DEBUG} )
message( "" )
message( "  Platform information" )
message( "    System:           " ${INFO_SYSTEM_NAME} )
message( "" )

