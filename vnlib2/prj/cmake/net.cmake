FILE(GLOB VN_NET_SRC ${CMAKE_CURRENT_LIST_DIR}/../../src/net/*.cpp)
ADD_LIBRARY(vnnet STATIC ${VN_NET_SRC})