CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(demo_svr)

INCLUDE(../../../vnlib2/prj/cmake/common.cmake)
INCLUDE(../../../vnlib2/prj/cmake/base.cmake)
INCLUDE(../../../vnlib2/prj/cmake/lex.cmake)
INCLUDE(../../../vnlib2/prj/cmake/net.cmake)
INCLUDE(../../../vnlib2/prj/cmake/mysqlconnector.cmake)

INCLUDE(../../../vnlib2/prj/cmake/pktcoder.cmake)

#ADD_CUSTOM_TARGET(build-protocols ALL COMMAND ../build_protocols.sh ${CMAKE_CURRENT_BINARY_DIR}/vnpktcoder)

FILE(GLOB DEMO_COMMON_SRC ${CMAKE_CURRENT_LIST_DIR}/../../src/common/*.cpp)
FILE(GLOB DEMO_SERVER_SRC ${CMAKE_CURRENT_LIST_DIR}/../../src/server/*.cpp)

ADD_EXECUTABLE(demo_svr ${DEMO_COMMON_SRC} ${DEMO_SERVER_SRC})
TARGET_LINK_LIBRARIES(demo_svr vnmysqlconnector vnnet vnlex vnbase mysqlclient)

#ADD_DEPENDENCIES(build-protocols vnpktcoder)
#ADD_DEPENDENCIES(demo_svr build-protocols)
