module Svc {
    @ Routes packets deframed by the Deframer to the rest of the system
    passive component FprimeRouter {

        # ----------------------------------------------------------------------
        # Router interface
        # ----------------------------------------------------------------------
        include "../Interfaces/RouterInterface.fppi"

        @ Port for forwarding non-recognized packet types
        output port unknownDataOut: Fw.DataWithContext

        @ Port for deallocating buffers
        output port bufferDeallocate: Fw.BufferSend

        @ An error occurred while serializing a com buffer
        event SerializationError(
                status: U32 @< The status of the operation
            ) \
            severity warning high \
            format "Serializing com buffer failed with status {}"

        @ An error occurred while deserializing a packet
        event DeserializationError(
                status: U32 @< The status of the operation
            ) \
            severity warning high \
            format "Deserializing packet type failed with status {}"


        ###############################################################################
        # Standard AC Ports for Events 
        ###############################################################################
        @ Port for requesting the current time
        time get port timeCaller

        @ Port for sending textual representation of events
        text event port logTextOut

        @ Port for sending events to downlink
        event port logOut

    }
}
