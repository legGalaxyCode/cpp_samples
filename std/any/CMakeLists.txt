project(any_lib)

file(GLOB SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(any_lib STATIC ${SRC_FILES})

target_include_directories(any_lib INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS any_lib DESTINATION lib)
