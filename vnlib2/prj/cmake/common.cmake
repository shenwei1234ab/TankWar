INCLUDE_DIRECTORIES(${CMAKE_CURRENT_LIST_DIR}/../../inc/base)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_LIST_DIR}/../../inc/lex)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_LIST_DIR}/../../inc/net)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_LIST_DIR}/../../inc/net_ssl)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_LIST_DIR}/../../inc/mysql_connector)

IF(CMAKE_BUILD_TYPE STREQUAL "Debug")
	ADD_DEFINITIONS(-DDEBUG)
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -g -gdwarf-2 -pg")
ELSE()
	SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2")
ENDIF()

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -Wall -Werror -fPIC")
