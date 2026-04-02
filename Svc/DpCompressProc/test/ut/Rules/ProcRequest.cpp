// ======================================================================
// \title  ProcRequest.cpp
// \author Gerik Kubiak
// \brief  ProcRequest class implementation
//
// \copyright
// Copyright (C) 2024 California Institute of Technology.
// ALL RIGHTS RESERVED.  United States Government sponsorship
// acknowledged.
// ======================================================================

#include <limits>
#include <string>

#include "Os/Stub/test/File.hpp"
#include "STest/Pick/Pick.hpp"
#include "Svc/DpCompressProc/test/ut/Rules/ProcRequest.hpp"
#include "Svc/DpCompressProc/test/ut/Rules/Testers.hpp"

namespace Svc {

// ----------------------------------------------------------------------
// Rule definitions
// ----------------------------------------------------------------------

bool TestState ::precondition__ProcRequest__Compressible() const {
    return true;
}

void TestState ::action__ProcRequest__Compressible() {

    const FwSizeType chunk_size = 4096;
    std::vector<AbstractState::Chunk> chunks = {
        AbstractState::Chunk(AbstractState::COMPRESSED, 0xA5),
        AbstractState::Chunk(AbstractState::COMPRESSED, 0x55),
        AbstractState::Chunk(AbstractState::COMPRESSED, 0x23)
    };

    Fw::Buffer container_buf =
        this->abstractState.build_compress_buffer(
            chunk_size,
            chunks);

    paramSet_ENABLE_CHUNKING(Fw::Enabled::ENABLED,
                             Fw::ParamValid::VALID);
    paramSet_CHUNK_SIZE(chunk_size,
                        Fw::ParamValid::VALID);
    paramSet_ENABLE(Fw::Enabled::ENABLED,
                    Fw::ParamValid::VALID);
    this->component.loadParameters();

    this->invoke_to_procRequest(0, container_buf);

    // Expect compressed chunks to compress by 50%
    FwSizeType exp_size =
        this->abstractState.expected_compressed_size(
            chunk_size,
            chunks);

    Fw::DpContainer container_out(0, container_buf);
    container_out.deserializeHeader();

    ASSERT_EQ(exp_size, container_out.getDataSize());

    delete[] container_buf.getData();

    /*
    // Clear the history
    this->clearHistory();
    // Reset the saved proc types
    // These are updated in the from_procBufferSendOut handler
    this->abstractState.m_procTypes = 0;
    // Reset the file pointer in the stub file implementation
    auto& fileData = Os::Stub::File::Test::StaticData::data;
    fileData.pointer = 0;
    // Update m_NumBuffersReceived
    this->abstractState.m_NumBuffersReceived.value++;
    // Construct a random buffer
    Fw::Buffer buffer = this->abstractState.getDpBuffer();
    // Send the buffer
    this->invoke_to_bufferSendIn(0, buffer);
    this->doDispatch();
    // Deserialize the container header
    Fw::DpContainer container;
    container.setBuffer(buffer);
    const Fw::SerializeStatus status = container.deserializeHeader();
    ASSERT_EQ(status, Fw::FW_SERIALIZE_OK);
    // Check events
    ASSERT_EVENTS_SIZE(1);
    ASSERT_EVENTS_FileWritten_SIZE(1);
    Fw::FileNameString fileName;
    this->constructDpFileName(container.getId(), container.getTimeTag(), fileName);
    ASSERT_EVENTS_FileWritten(0, static_cast<U32>(buffer.getSize()), fileName.toChar());
    // Check processing types
    this->checkProcTypes(container);
    // Check DP notification
    ASSERT_from_dpWrittenOut_SIZE(1);
    ASSERT_from_dpWrittenOut(0, fileName, container.getPriority(), buffer.getSize());
    // Check deallocation
    ASSERT_from_deallocBufferSendOut_SIZE(1);
    ASSERT_from_deallocBufferSendOut(0, buffer);
    // Check file write
    ASSERT_EQ(buffer.getSize(), fileData.pointer);
    ASSERT_EQ(0, ::memcmp(buffer.getData(), fileData.writeResult, buffer.getSize()));
    // Check data checksum is valid for the container buffer
    Utils::HashBuffer storedHash;
    Utils::HashBuffer computedHash;
    ASSERT_EQ(Fw::Success::SUCCESS, container.checkDataHash(storedHash, computedHash));
    // Update m_NumBytesWritten
    this->abstractState.m_NumBytesWritten.value += buffer.getSize();
    // Update m_NumSuccessfulWrites
    this->abstractState.m_NumSuccessfulWrites.value++;
    */
}

namespace ProcRequest {

// ----------------------------------------------------------------------
// Tests
// ----------------------------------------------------------------------

void Tester::Compressible() {
    this->ruleCompressible.apply(this->testState);
    //this->testState.printEvents();
}


}  // namespace ProcRequest

}  // namespace Svc
