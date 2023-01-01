#****************************************************************************
#* CMakeLists.txt
#****************************************************************************
cmake_minimum_required (VERSION 3.11)


include_directories(
    ${CMAKE_BINARY_DIR}/antlr4/include/antlr4-runtime
    ${PACKAGES_DIR}/antlr4-cpp-runtime/runtime/src
    ${CMAKE_CURRENT_BINARY_DIR}/antlr4cpp_generated_src/PSSLexer
    ${CMAKE_CURRENT_BINARY_DIR}/antlr4cpp_generated_src/PSSParser
    ${CMAKE_BINARY_DIR}/zsp_ast/src/include
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${ZUSPEC_PARSER_DIR}/src/include
    ${CMAKE_BINARY_DIR}/zuspec-parser/src/ZuspecParser-build/zsp_ast/src/include
    ${ZUSPEC_ARL_DM_DIR}/src/include
    ${LIBVSC_DM_DIR}/src/include
    ${DEBUG_MGR_DIR}/src/include
    )

link_directories(
	${CMAKE_BINARY_DIR}/antlr4/lib
	${CMAKE_BINARY_DIR}/antlr4/lib64
	)

file(GLOB zuspec_fe_parser_SRC
	"*.h"
	"*.cpp")
add_library(zuspec-fe-parser SHARED
	${zuspec_fe_parser_SRC}
	)

install(TARGETS zuspec-fe-parser
    DESTINATION lib
    EXPORT zsp-targets)
    
add_dependencies(zuspec-fe-parser ZuspecParser)

#target_link_libraries(zuspec-fe-parser ${ZUSPEC_PARSER_LIBS})
set_target_properties(zuspec-fe-parser PROPERTIES SKIP_BUILD_RPATH TRUE)

add_library(zuspec-fe-parser_static STATIC
	${zuspec_fe_parser_SRC}
	)

add_dependencies(zuspec-fe-parser_static ZuspecParser)
	

