cmake_minimum_required(VERSION 3.0)

math(EXPR BITS "8*${CMAKE_SIZEOF_VOID_P}")
if (BITS EQUAL 32)
	set(PLATFORM_SUFFIX x86)
elseif (BITS EQUAL 64)
	set(PLATFORM_SUFFIX x64)
endif (BITS EQUAL 32)