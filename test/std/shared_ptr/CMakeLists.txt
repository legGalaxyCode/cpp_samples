project(shared_ptr_tests)

# TODO: add googletest here

add_executable(shared_ptr_tests test.cpp)

target_link_libraries(shared_ptr_tests shared_ptr_lib)
target_link_libraries(shared_ptr_tests gtest gtest_main)

#set_target_properties(shared_ptr_tests PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")

# to make test
add_test(SharedPtrTest shared_ptr_tests)