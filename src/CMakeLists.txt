#****************************************************************************
#* CMakeLists.txt
#****************************************************************************
cmake_minimum_required (VERSION 3.11)


include_directories(
    ${CMAKE_BINARY_DIR}/zsp_ast/src/include
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    "${zsp_parser_INCDIR}"
    "${zsp_arl_dm_INCDIR}"
    "${vsc_dm_INCDIR}"
    "${debug_mgr_INCDIR}"
    )

link_directories(
	${CMAKE_BINARY_DIR}/antlr4/lib
	${CMAKE_BINARY_DIR}/antlr4/lib64
	)

file(GLOB zuspec_fe_parser_SRC
	"*.h"
	"*.cpp")
add_library(zsp-fe-parser SHARED
	${zuspec_fe_parser_SRC}
	)

install(TARGETS zsp-fe-parser
    DESTINATION lib
    EXPORT zsp-targets)
    
#target_link_libraries(zsp-fe-parser ${ZUSPEC_PARSER_LIBS})
set_target_properties(zsp-fe-parser PROPERTIES SKIP_BUILD_RPATH TRUE)

# add_library(zsp-fe-parser_static STATIC
# 	${zuspec_fe_parser_SRC}
# 	)

# add_dependencies(zsp-fe-parser_static ZuspecParser)
	

