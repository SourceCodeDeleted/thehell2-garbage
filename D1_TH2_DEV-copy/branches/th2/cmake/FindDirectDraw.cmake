cmake_minimum_required(VERSION 2.8)

if(WIN32)
	find_library(DDRAW_LIBRARY "ddraw" HINTS ${CMAKE_SOURCE_DIR}/directx)
	find_library(DXGUID_LIBRARY "dxguid" HINTS ${CMAKE_SOURCE_DIR}/directx)
	find_file(DDRAW_HEADER "ddraw.h" HINTS ${CMAKE_SOURCE_DIR}/directx)
	if (NOT "${DDRAW_HEADER}" STREQUAL "DDRAW_HEADER-NOTFOUND")
		get_filename_component(DDRAW_INCLUDE_DIR ${DDRAW_HEADER} DIRECTORY)
		set(DDRAW_INCLUDE_DIR ${DDRAW_INCLUDE_DIR} CACHE PATH "Directory that contains ddraw.h include file")
		set_property(CACHE DDRAW_HEADER PROPERTY TYPE INTERNAL)
	endif()
	
	include(FindPackageHandleStandardArgs REQUIRED)
	FIND_PACKAGE_HANDLE_STANDARD_ARGS(DirectDraw DEFAULT_MSG DDRAW_LIBRARY DDRAW_INCLUDE_DIR DXGUID_LIBRARY)
endif(WIN32)