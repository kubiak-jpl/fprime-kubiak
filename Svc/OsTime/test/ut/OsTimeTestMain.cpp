// ======================================================================
// \title  OsTimeTestMain.cpp
// \author kubiak
// \brief  cpp file for OsTime component test main function
// ======================================================================

#include "OsTimeTester.hpp"

TEST(Nominal, toDo) {
  Svc::OsTimeTester tester;
  tester.toDo();
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
