#include "shared_ptr.hpp"

#include <cassert>
#include <iostream>

namespace {

using namespace my_std;

void testDefaultInit() {
    SharedPtr<int> ptr;
    assert(!ptr && ptr.getUseCount() == 0);
}

void testInitFromPtr() {
    SharedPtr<int> ptr(new int(1));
    assert(ptr && ptr.getUseCount() == 1 && *ptr == 1);
}

void testMoveCtr() {
    SharedPtr<int> ptr(new int(1));
    assert(ptr && ptr.getUseCount() == 1 && *ptr == 1);

    auto new_ptr = std::move(ptr);
    assert(!ptr && new_ptr && ptr.getUseCount() == 0 && new_ptr.getUseCount() == 1 && *new_ptr == 1);
}

void testInitFromPtrDestructor() {
    auto* val = new int(1);
    {
        SharedPtr<int> ptr(val);
        assert(ptr && ptr.getUseCount() == 1 && *ptr == 1);
    }
    
    // Raw pointer should be deleted at this moment :)
    // but neither my nor std:: version sets raw to nullptr
    assert(val != nullptr);
}

void testDereferenceOp() {
    auto* val = new int(1);
    SharedPtr<int> ptr(val);

    assert(ptr && ptr.getUseCount() == 1 && *ptr == 1 && val == ptr.get());
    
    *val = 5;
    assert(*val == 5 && *ptr == 5);

    *ptr = 1;
    assert(*val == 1 && *ptr == 1);
    //assert(false);
}

void testMakeShared() {
    auto ptr = make_shared<int>(1);
}

void runAllTests() {
    testDefaultInit();
    testInitFromPtr();
    testInitFromPtrDestructor();
    testMoveCtr();
    testDereferenceOp();
    testMakeShared();
    std::cout << "PASS\n";
}

}

int main() {
    runAllTests();
    //std::cout << "PASS\n";
    return 0;
}