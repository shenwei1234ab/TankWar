FILE(GLOB VN_LRBUILDER_SRC ${CMAKE_CURRENT_LIST_DIR}/../../src/lrbuilder/*.cpp)
ADD_EXECUTABLE(vnlrbuilder ${VN_LRBUILDER_SRC})
TARGET_LINK_LIBRARIES(vnlrbuilder vnlex vnbase)