// ======================================================================
// \title  DpCompressProcTester.cpp
// \author kubiak
// \brief  cpp file for DpCompressProc component test harness implementation class
// ======================================================================

#include "DpCompressProcTester.hpp"
#include "Svc/DpCompressProc/test/ut/AbstractState.hpp"

namespace Svc {

// ----------------------------------------------------------------------
// Construction and destruction
// ----------------------------------------------------------------------

DpCompressProcTester ::DpCompressProcTester()
    : DpCompressProcGTestBase("DpCompressProcTester", DpCompressProcTester::MAX_HISTORY_SIZE),
      component("DpCompressProc") {
    this->initComponents();
    this->connectPorts();
    this->component.loadParameters();
}

DpCompressProcTester ::~DpCompressProcTester() {}

Svc::CompressionAlgorithm DpCompressProcTester ::
    from_compressChunk_handler(
        FwIndexType portNum,
        Fw::Buffer& buffer,
        FwSizeType min_compression,
        FwSizeType write_offset
) {
    this->pushFromPortEntry_compressChunk(buffer, min_compression, write_offset);

    AbstractState::Compressible c =
        static_cast<AbstractState::Compressible>(buffer.getData()[0]);
    U8 sentinel = buffer.getData()[1];

    switch (c) {
        case AbstractState::UNCOMPRESSED:
            return Svc::CompressionAlgorithm::UNCOMPRESSED;
        case AbstractState::COMPRESSED:
            memset(buffer.getData() + write_offset,
                   sentinel,
                   buffer.getSize()/2);
            buffer.setSize(buffer.getSize()/2);
            return Svc::CompressionAlgorithm::ZLIB_DEFLATE;
        case AbstractState::MINIMAL_COMPRESSED:
            memset(buffer.getData() + write_offset,
                   sentinel,
                   min_compression);
            buffer.setSize(min_compression);
            return Svc::CompressionAlgorithm::ZLIB_DEFLATE;
        default:
            assert(false);
    }
}


// ----------------------------------------------------------------------
// Tests
// ----------------------------------------------------------------------

}  // namespace Svc
