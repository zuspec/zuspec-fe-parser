
cmake_minimum_required(VERSION 2.8.8)

ExternalProject_Add(GTEST
	PREFIX gtest
        SOURCE_DIR "${PACKAGES_DIR}/gtest"
        CMAKE_CACHE_ARGS
          -DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_BINARY_DIR}/gtest
	  -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
	  -DBUILD_SHARED_LIBS:BOOL=${BUILD_SHARED_LIBS}
    )

file(GLOB TEST_SRCS 
	"src/*.cpp" 
	"src/*.h")

link_directories(
	${CMAKE_BINARY_DIR}/gtest/lib
	${CMAKE_BINARY_DIR}/gtest/lib64
	${CMAKE_BINARY_DIR}/lib
	${CMAKE_BINARY_DIR}/lib64
	${CMAKE_BINARY_DIR}/zuspec-parser/src/ZuspecParser-build/antlr4/lib
	${CMAKE_BINARY_DIR}/zuspec-parser/src/ZuspecParser-build/antlr4/lib64
    "${zsp_parser_LIBDIR}"
    "${zsp_arl_dm_LIBDIR}"
    "${vsc_dm_LIBDIR}"
    "${debug_mgr_LIBDIR}"
	)

set(CompilerFlags
        CMAKE_CXX_FLAGS
        CMAKE_CXX_FLAGS_DEBUG
        CMAKE_CXX_FLAGS_RELEASE
        CMAKE_C_FLAGS
        CMAKE_C_FLAGS_DEBUG
        CMAKE_C_FLAGS_RELEASE
        )
foreach(CompilerFlag ${CompilerFlags})
  string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
  string(REPLACE "/MDd" "/MTd" ${CompilerFlag} "${${CompilerFlag}}")
endforeach()

#********************************************************************
#* GoogleTest unit tests
#********************************************************************
add_executable(zsp-fe-parser-testmain ${TEST_SRCS})

set_property(TARGET zsp-fe-parser-testmain PROPERTY
  MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

target_include_directories(zsp-fe-parser-testmain PRIVATE
	${CMAKE_BINARY_DIR}
	${CMAKE_BINARY_DIR}/gtest/include
	${PACKAGES_DIR}/gtest/googletest/include
	${CMAKE_CURRENT_SOURCE_DIR}/../src
	${CMAKE_CURRENT_SOURCE_DIR}/../src/include
    "${zsp_parser_INCDIR}"
    "${zsp_arl_dm_INCDIR}"
    "${vsc_dm_INCDIR}"
    "${debug_mgr_INCDIR}"
)

message(STATUS "CMAKE_BUILD_TYPE: ${CMAKE_BUILD_TYPE}")
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    message(STATUS "Taking debug libs")
    list(APPEND ZSP_TESTMAIN_LIBS gtestd gtest_maind)
else()
    message(STATUS "Taking release libs")
    list(APPEND ZSP_TESTMAIN_LIBS gtest gtest_main)
endif()

list(APPEND ZSP_TESTMAIN_LIBS zsp-fe-parser zsp-parser ast zsp-arl-dm vsc-dm debug-mgr)

if (WIN32)
	if (BUILD_SHARED_LIBS)
		list(APPEND ZSP_TESTMAIN_LIBS antlr4-runtime)
	else()
		list(APPEND ZSP_TESTMAIN_LIBS antlr4-runtime-static)
	endif()
else()
	list(APPEND ZSP_TESTMAIN_LIBS antlr4-runtime pthread)
endif()



target_link_libraries(zsp-fe-parser-testmain ${ZSP_TESTMAIN_LIBS})
add_dependencies(zsp-fe-parser-testmain GTEST)

add_test(
	NAME unit
	COMMAND ${CMAKE_BINARY_DIR}/tests/zsp-fe-parser-testmain)


