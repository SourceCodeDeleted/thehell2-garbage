cmake_minimum_required(VERSION 3.0)

function(ADD_PRECOMPILED_HEADER PrecompiledHeader, PrecompiledSource, SourcesVar)
	if (MSVC)
		get_filename_component(PrecompiledHeaderBasename "${PrecompiledHeader}" NAME_WE)
		set(PrecompiledBinary "${CMAKE_CURRENT_BINARY_DIR}/${PrecompiledHeaderBasename}.pch")
		set(Sources ${${SourcesVar}})
		
		set_source_files_properties(${PrecompiledSource} PROPERTIES
			COMPILE_FLAGS "/Yc\"${PrecompiledHeader}\" /Fp\"${PrecompiledBinary}\""
			OBJECT_OUTPUTS "${PrecompiledBinary}")
			
		set_source_files_properties(${Sources} PROPERTIES
			COMPILE_FLAGS "/Yu\"${PrecompiledHeader}\" /FI\"${PrecompiledHeader}\" /Fp\"${PrecompiledBinary}\""
			OBJECT_DEPENDS "${PrecompiledBinary}")
		
		list(APPEND ${SourcesVar} ${PrecompiledSource})
	endif(MSVC)
endfunction(ADD_PRECOMPILED_HEADER)