project(MPQAccess)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
include(bitness REQUIRED)
include(options REQUIRED)
find_package(StormLib REQUIRED)
include(PrecompiledHeaders OPTIONAL)

set(SRCS 
	MPQArchive.cpp
	MPQArchive.h
	MPQArchive_p.cpp
	MPQArchive_p.h
	MPQConfig.h
	MPQConsts.h
	MPQFile.cpp
	MPQFile.h
	MPQFile_p.cpp
	MPQFile_p.h
	MPQFileWalker.cpp
	MPQFileWalker.h
	MPQFwdDcls.h
	MPQInterfaces.h
)

# ADD_PRECOMPILED_HEADER(stdafx_mpqaccess.h stdafx_mpqaccess.cpp SRCS)

include_directories(BEFORE ${STORMLIB_INCLUDE_DIRS})
if (BUILD_SHARED_LIBS)
	add_library(mpqaccess SHARED ${SRCS})
else (BUILD_SHARED_LIBS)
	add_library(mpqaccess STATIC ${SRCS})
endif (BUILD_SHARED_LIBS)
target_link_libraries(mpqaccess ${STORMLIB_LIBRARIES})
