// ======================================================================
// \title  DpCompressProcTestMain.cpp
// \author kubiak
// \brief  cpp file for DpCompressProc component test main function
// ======================================================================

#include "DpCompressProcTester.hpp"
#include "Svc/DpCompressProc/test/ut/Rules/Testers.hpp"

TEST(Nominal, Compressible) {
    Svc::Testers::procRequest.Compressible();
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
