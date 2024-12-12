# project(shared_ptr_lib)
set(LIB_NAME "shared_ptr_lib")
project(${LIB_NAME})

file(GLOB SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_library(${LIB_NAME} STATIC ${SRC_FILES})

# if (CLANG_TIDY_EXE)
#     set_target_properties(${LIB_NAME} PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")
#     set(CMAKE_CXX_CLANG_TIDY_EXPORT_FIXES_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
#     message(${CLANG_TIDY_COMMAND})
# endif()

target_include_directories(${LIB_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

install(TARGETS ${LIB_NAME} DESTINATION lib)