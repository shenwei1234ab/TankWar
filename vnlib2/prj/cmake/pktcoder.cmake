FILE(GLOB VN_PKTCODER_SRC ${CMAKE_CURRENT_LIST_DIR}/../../src/pktcoder/*.cpp)
ADD_EXECUTABLE(vnpktcoder ${VN_PKTCODER_SRC})
TARGET_LINK_LIBRARIES(vnpktcoder vnnet vnlex vnbase)