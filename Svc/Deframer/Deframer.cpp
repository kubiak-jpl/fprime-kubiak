// ======================================================================
// \title  Deframer.cpp
// \author mstarch, bocchino
// \brief  cpp file for Deframer component implementation class
//
// \copyright
// Copyright 2009-2022, by the California Institute of Technology.
// ALL RIGHTS RESERVED.  United States Government Sponsorship
// acknowledged.
//
// ======================================================================

#include <cstring>

#include "Fw/Com/ComPacket.hpp"
#include "Fw/Logger/Logger.hpp"
#include <FpConfig.hpp>
#include "Svc/Deframer/Deframer.hpp"

namespace Svc {

// ----------------------------------------------------------------------
// Static assertions
// ----------------------------------------------------------------------

static_assert(
    DeframerCfg::POLL_BUFFER_SIZE > 0,
    "poll buffer size must be greater than zero"
);
static_assert(
    DeframerCfg::RING_BUFFER_SIZE > 0,
    "ring buffer size must be greater than zero"
);

// ----------------------------------------------------------------------
// Construction, initialization, and destruction
// ----------------------------------------------------------------------

Deframer ::Deframer(const char* const compName) :
    DeframerComponentBase(compName),
    DeframingProtocolInterface(),
    m_protocol(nullptr),
    m_inRing(m_ringBuffer, sizeof m_ringBuffer)
{
    (void) memset(m_pollBuffer, 0, sizeof m_pollBuffer);
}

Deframer ::~Deframer() {}

void Deframer ::setup(DeframingProtocol& protocol) {
    // Check that this is the first time we are calling setup
    FW_ASSERT(m_protocol == nullptr);
    // Assign the protocol passed in to m_protocol
    m_protocol = &protocol;
    // Pass *this as the DeframingProtocolInstance to protocol setup
    // Deframer is derived from and implements DeframingProtocolInterface
    protocol.setup(*this);
}

// ----------------------------------------------------------------------
// Handler implementations for user-defined typed input ports
// ----------------------------------------------------------------------

void Deframer ::cmdResponseIn_handler(
    FwIndexType portNum,
    FwOpcodeType opcode,
    U32 cmdSeq,
    const Fw::CmdResponse& response
) {
    // Nothing to do
}

void Deframer ::framedIn_handler(
    const FwIndexType portNum,
    Fw::Buffer& recvBuffer,
    const Drv::RecvStatus& recvStatus
) {
    // Check whether there is data to process
    if (recvStatus.e == Drv::RecvStatus::RECV_OK) {
        // There is: process the data
        processBuffer(recvBuffer);
    }
    // Deallocate the buffer
    framedDeallocate_out(0, recvBuffer);
}

void Deframer ::schedIn_handler(
    const FwIndexType portNum,
    U32 context
) {
    // Check for data
    Fw::Buffer buffer(m_pollBuffer, sizeof(m_pollBuffer));
    const Drv::PollStatus status = framedPoll_out(0, buffer);
    if (status.e == Drv::PollStatus::POLL_OK) {
        // Data exists: process it
        processBuffer(buffer);
    }
}

// ----------------------------------------------------------------------
// Implementation of DeframingProtocolInterface
// ----------------------------------------------------------------------

Fw::Buffer Deframer ::allocate(const U32 size)  {
    return bufferAllocate_out(0, size);
}

void Deframer ::route(Fw::Buffer& packetBuffer) {

    // Read the packet type from the packet buffer
    FwPacketDescriptorType packetType = Fw::ComPacket::FW_PACKET_UNKNOWN;
    Fw::SerializeStatus status = Fw::FW_SERIALIZE_OK;
    {
        Fw::SerializeBufferBase& serial = packetBuffer.getSerializeRepr();
        status = serial.setBuffLen(packetBuffer.getSize());
        FW_ASSERT(status == Fw::FW_SERIALIZE_OK);
        status = serial.deserialize(packetType);
    }

    // Whether to deallocate the packet buffer
    bool deallocate = true;

    // Process the packet
    if (status == Fw::FW_SERIALIZE_OK) {
        U8 *const packetData = packetBuffer.getData();
        const FwSizeType packetSize = packetBuffer.getSize();
        switch (packetType) {
            // Handle a command packet
            case Fw::ComPacket::FW_PACKET_COMMAND: {
                // Allocate a com buffer on the stack
                Fw::ComBuffer com;
                // Copy the contents of the packet buffer into the com buffer
                status = com.setBuff(packetData, packetSize);
                if (status == Fw::FW_SERIALIZE_OK) {
                    // Send the com buffer
                    comOut_out(0, com, 0);
                }
                else {
                    Fw::Logger::log(
                        "[ERROR] Serializing com buffer failed with status %d\n",
                        status
                    );
                }
                break;
            }
            // Handle a file packet
            case Fw::ComPacket::FW_PACKET_FILE: {
                // If the file uplink output port is connected,
                // send the file packet. Otherwise take no action.
                if (isConnected_bufferOut_OutputPort(0)) {
                    FW_ASSERT((packetSize - sizeof(packetType)) < std::numeric_limits<U32>::max(),
                              static_cast<FwAssertArgType>(packetSize - sizeof(packetType)));
                    // Shift the packet buffer to skip the packet type
                    // The FileUplink component does not expect the packet
                    // type to be there.
                    packetBuffer.setData(packetData + sizeof(packetType));
                    packetBuffer.setSize(static_cast<U32>(packetSize - sizeof(packetType)));
                    // Send the packet buffer
                    bufferOut_out(0, packetBuffer);
                    // Transfer ownership of the buffer to the receiver
                    deallocate = false;
                }
                break;
            }
            // Take no action for other packet types
            default:
                break;
        }
    }
    else {
        Fw::Logger::log(
            "[ERROR] Deserializing packet type failed with status %d\n",
            status
        );
    }

    if (deallocate) {
        // Deallocate the packet buffer
        bufferDeallocate_out(0, packetBuffer);
    }

}

// ----------------------------------------------------------------------
// Helper methods
// ----------------------------------------------------------------------

void Deframer ::processBuffer(Fw::Buffer& buffer) {

    const FwSizeType bufferSize = buffer.getSize();
    U8 *const bufferData = buffer.getData();
    // Current offset into buffer
    FwSizeType offset = 0;
    // Remaining data in buffer
    FwSizeType remaining = bufferSize;

    for (FwSizeType i = 0; i < bufferSize; ++i) {
        // If there is no data left, exit the loop
        if (remaining == 0) {
            break;
        }
        // Compute the size of data to serialize
        const FwSizeType ringFreeSize = m_inRing.get_free_size();
        const FwSizeType serSize = (ringFreeSize <= remaining) ?
            ringFreeSize : static_cast<FwSizeType>(remaining);
        // Serialize data into the ring buffer
        const Fw::SerializeStatus status =
            m_inRing.serialize(&bufferData[offset], serSize);
        // If data does not fit, there is a coding error
        FW_ASSERT(
            status == Fw::FW_SERIALIZE_OK,
            static_cast<FwAssertArgType>(status),
            static_cast<FwAssertArgType>(offset),
            static_cast<FwAssertArgType>(serSize));
        // Process the data
        processRing();
        // Update buffer offset and remaining
        offset += serSize;
        remaining -= serSize;
    }

    // In every iteration, either remaining == 0 and we break out
    // of the loop, or we consume at least one byte from the buffer.
    // So there should be no data left in the buffer.
    FW_ASSERT(remaining == 0, static_cast<FwAssertArgType>(remaining));

}

void Deframer ::processRing() {

    FW_ASSERT(m_protocol != nullptr);

    // The number of remaining bytes in the ring buffer
    FwSizeType remaining = 0;
    // The protocol status
    DeframingProtocol::DeframingStatus status =
        DeframingProtocol::DEFRAMING_STATUS_SUCCESS;
    // The ring buffer capacity
    const FwSizeType ringCapacity = m_inRing.get_capacity();

    // Process the ring buffer looking for at least the header
    for (FwSizeType i = 0; i < ringCapacity; i++) {
        // Get the number of bytes remaining in the ring buffer
        remaining = m_inRing.get_allocated_size();
        // If there are none, we are done
        if (remaining == 0) {
            break;
        }
        // Needed is an out-only variable
        // Initialize it to zero
        U32 needed = 0;
        // Call the deframe method of the protocol, getting
        // needed and status
        status = m_protocol->deframe(m_inRing, needed);
        // Deframing protocol must not consume data in the ring buffer
        FW_ASSERT(
            m_inRing.get_allocated_size() == remaining,
            static_cast<FwAssertArgType>(m_inRing.get_allocated_size()),
            static_cast<FwAssertArgType>(remaining)
        );
        // On successful deframing, consume data from the ring buffer now
        if (status == DeframingProtocol::DEFRAMING_STATUS_SUCCESS) {
            // If deframing succeeded, protocol should set needed
            // to a non-zero value
            FW_ASSERT(needed != 0);
            FW_ASSERT(
                needed <= remaining,
                static_cast<FwAssertArgType>(needed),
                static_cast<FwAssertArgType>(remaining));
            m_inRing.rotate(needed);
            FW_ASSERT(
                m_inRing.get_allocated_size() == remaining - needed,
                static_cast<FwAssertArgType>(m_inRing.get_allocated_size()),
                static_cast<FwAssertArgType>(remaining),
                static_cast<FwAssertArgType>(needed)
            );
        }
        // More data needed
        else if (status == DeframingProtocol::DEFRAMING_MORE_NEEDED) {
            // Deframing protocol should not report "more is needed"
            // unless more is needed
            FW_ASSERT(
                needed > remaining,
                static_cast<FwAssertArgType>(needed),
                static_cast<FwAssertArgType>(remaining));
            // Break out of loop: suspend deframing until we receive
            // another buffer
            break;
        }
        // Error occurred
        else {
            // Skip one byte of bad data
            m_inRing.rotate(1);
            FW_ASSERT(
                m_inRing.get_allocated_size() == remaining - 1,
                static_cast<FwAssertArgType>(m_inRing.get_allocated_size()),
                static_cast<FwAssertArgType>(remaining)
            );
            // Log checksum errors
            // This is likely a real error, not an artifact of other data corruption
            if (status == DeframingProtocol::DEFRAMING_INVALID_CHECKSUM) {
                Fw::Logger::log("[ERROR] Deframing checksum validation failed\n");
            }
        }
    }

    // If more not needed, circular buffer should be empty
    if (status != DeframingProtocol::DEFRAMING_MORE_NEEDED) {
        FW_ASSERT(remaining == 0, static_cast<FwAssertArgType>(remaining));
    }

}

}  // end namespace Svc
