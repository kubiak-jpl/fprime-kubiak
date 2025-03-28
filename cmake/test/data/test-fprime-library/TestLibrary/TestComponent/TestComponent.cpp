#include <TestLibrary/TestComponent/TestComponent.hpp>

namespace TestLibrary {

TestComponent ::TestComponent(const char* name) :
    TestComponentComponentBase(name)
{}

void TestComponent ::init(const NATIVE_INT_TYPE instance) {
    TestComponentComponentBase::init(instance);
}

TestComponent ::~TestComponent() {}

void TestComponent ::schedIn_handler(FwIndexType portNum, U32 context) {}
};
