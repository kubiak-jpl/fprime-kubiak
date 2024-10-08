// ======================================================================
// \title  FatalHandlerImpl.cpp
// \author lestarch
// \brief  cpp file for FatalHandler component implementation class
// ======================================================================

#include <cstdlib>
#include <Os/Console.hpp>
#include <Svc/FatalHandler/FatalHandlerComponentImpl.hpp>
#include <FpConfig.hpp>

namespace Svc {

    // ----------------------------------------------------------------------
    // Handler implementations for user-defined typed input ports
    // ----------------------------------------------------------------------

    void FatalHandlerComponentImpl::FatalReceive_handler(
            const NATIVE_INT_TYPE portNum,
            FwEventIdType Id) {
        // for **nix, delay then exit with error code
        Os::Log::log("FATAL %d handled.\n",Id);
        while (true) {} // Returning might be bad
    }

} // end namespace Svc
