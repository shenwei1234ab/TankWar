FILE(GLOB VN_NETSSL_SRC ${CMAKE_CURRENT_LIST_DIR}/../../src/net_ssl/*.cpp)
ADD_LIBRARY(vnnetssl STATIC ${VN_NETSSL_SRC})