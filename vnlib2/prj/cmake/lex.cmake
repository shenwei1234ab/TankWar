FILE(GLOB VN_LEX_SRC ${CMAKE_CURRENT_LIST_DIR}/../../src/lex/*.cpp)
ADD_LIBRARY(vnlex STATIC ${VN_LEX_SRC})