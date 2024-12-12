#include <cassert>
#include <iostream>

#include "shared_ptr.hpp"

#include "gtest/gtest.h"

namespace {

using namespace my_std;

TEST(SharedPtrTest, testDefaultInit) {
    SharedPtr<int> ptr;
    EXPECT_EQ(ptr.empty(), true);
    EXPECT_EQ(ptr.getUseCount(), 0);
}

TEST(SharedPtrTest, testInitFromPtr) {
    auto* i = new int(1);
    SharedPtr<int> ptr(i);
    EXPECT_EQ(ptr.empty(), false);
    EXPECT_EQ(ptr.getUseCount(), 1);
    EXPECT_EQ(*ptr, 1);
    EXPECT_EQ(ptr.get(), i);
}

TEST(SharedPtrTest, testMoveCtr) {
    SharedPtr<int> ptr(new int(1));

    auto new_ptr = std::move(ptr);
    EXPECT_EQ(ptr.empty(), true);
    EXPECT_EQ(new_ptr.empty(), false);
    EXPECT_EQ(ptr.getUseCount(), 0);
    EXPECT_EQ(new_ptr.getUseCount(), 1);
    EXPECT_EQ(*new_ptr, 1);
}

TEST(SharedPtrTest, testInitFromPtrDestructor) {
    auto* val = new int(1);
    {
        SharedPtr<int> ptr(val);
        EXPECT_EQ(ptr.empty(), false);
        EXPECT_EQ(ptr.getUseCount(), 1);
        EXPECT_EQ(*ptr, 1);
    }
    
    // Raw pointer should be deleted at this moment :)
    // but neither my nor std:: version sets raw to nullptr
    // TODO: rewrite it
    assert(val != nullptr);
}

TEST(SharedPtrTest, testDereferenceOp) {
    auto* val = new int(1);
    SharedPtr<int> ptr(val);

    EXPECT_EQ(ptr.empty(), false);
    EXPECT_EQ(ptr.getUseCount(), 1);
    EXPECT_EQ(*ptr, *val);
    EXPECT_EQ(ptr.get(), val);
    
    *val = 5;
    EXPECT_EQ(*val, *ptr);

    *ptr = 1;
    EXPECT_EQ(*val, *ptr);
}

// TODO: implement this test by checking layout
/*
TEST(SharedPtrTest, testMakeShared) {
    auto ptr = make_shared<int>(1);
}
*/

}

int main() {
    testing::InitGoogleTest();

    return RUN_ALL_TESTS();
}
