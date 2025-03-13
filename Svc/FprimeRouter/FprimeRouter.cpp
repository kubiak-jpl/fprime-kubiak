// ======================================================================
// \title  FprimeRouter.cpp
// \author thomas-bc
// \brief  cpp file for FprimeRouter component implementation class
// ======================================================================

#include "Svc/FprimeRouter/FprimeRouter.hpp"
#include "FpConfig.hpp"
#include "Fw/Com/ComPacket.hpp"
#include "Fw/Logger/Logger.hpp"

namespace Svc {

// ----------------------------------------------------------------------
// Component construction and destruction
// ----------------------------------------------------------------------

FprimeRouter ::FprimeRouter(const char* const compName) : FprimeRouterComponentBase(compName) {}

FprimeRouter ::~FprimeRouter() {}

// ----------------------------------------------------------------------
// Handler implementations for user-defined typed input ports
// ----------------------------------------------------------------------

void FprimeRouter ::dataIn_handler(FwIndexType portNum, Fw::Buffer& packetBuffer, Fw::Buffer& contextBuffer) {
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
        U8* const packetData = packetBuffer.getData();
        const FwSizeType packetSize = packetBuffer.getSize();
        switch (packetType) {
            // Handle a command packet
            case Fw::ComPacket::FW_PACKET_COMMAND: {
                // Allocate a com buffer on the stack
                Fw::ComBuffer com;
                // Copy the contents of the packet buffer into the com buffer
                status = com.setBuff(packetData, packetSize);
                if (status == Fw::FW_SERIALIZE_OK) {
                    // Send the com buffer - critical functionality so it is considered an error not to
                    // have the port connected. This is why we don't check isConnected() before sending.
                    this->commandOut_out(0, com, 0);
                } else {
                    this->log_WARNING_HI_SerializationError(status);
                }
                break;
            }
            // Handle a file packet
            case Fw::ComPacket::FW_PACKET_FILE: {
                // If the file uplink output port is connected,
                // send the file packet. Otherwise take no action.
                if (this->isConnected_fileOut_OutputPort(0)) {
                    // Make sure we can cast down to U32 without overflow
                    FW_ASSERT((packetSize - sizeof(packetType)) < std::numeric_limits<U32>::max(),
                              static_cast<FwAssertArgType>(packetSize - sizeof(packetType)));
                    // Shift the packet buffer to skip the packet type
                    // The FileUplink component does not expect the packet
                    // type to be there.
                    packetBuffer.setData(packetData + sizeof(packetType));
                    packetBuffer.setSize(static_cast<U32>(packetSize - sizeof(packetType)));
                    // Send the packet buffer
                    this->fileOut_out(0, packetBuffer);
                    // Transfer ownership of the packetBuffer to the receiver
                    deallocate = false;
                }
                break;
            }
            default: {
                // Packet type is not known to the F Prime protocol. If the unknownDataOut port is
                // connected, forward packet and context for further processing
                if (this->isConnected_unknownDataOut_OutputPort(0)) {
                    this->unknownDataOut_out(0, packetBuffer, contextBuffer);
                    // Transfer ownership of the packetBuffer to the receiver
                    deallocate = false;
                }
            }
        }
    } else {
        this->log_WARNING_HI_DeserializationError(status);
    }

    if (deallocate) {
        // Deallocate the packet buffer
        this->bufferDeallocate_out(0, packetBuffer);
    }
}

void FprimeRouter ::cmdResponseIn_handler(FwIndexType portNum,
                                          FwOpcodeType opcode,
                                          U32 cmdSeq,
                                          const Fw::CmdResponse& response) {
    // Nothing to do
}
}  // namespace Svc
