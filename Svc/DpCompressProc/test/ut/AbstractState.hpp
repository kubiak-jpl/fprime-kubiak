// ======================================================================
// \title  AbstractState.hpp
// \author Gerik Kubiak
// \brief  Header file for abstract state
//
// \copyright
// Copyright (C) 2024 California Institute of Technology.
// ALL RIGHTS RESERVED.  United States Government sponsorship
// acknowledged.
// ======================================================================

#ifndef Svc_AbstractState_HPP
#define Svc_AbstractState_HPP

#include <cstring>
#include <vector>

#include "Fw/Types/Assert.hpp"
#include "Os/File.hpp"
#include "STest/Pick/Pick.hpp"
#include "Svc/DpCompressProc/DpCompressProc.hpp"
#include "TestUtils/OnChangeChannel.hpp"
#include "TestUtils/Option.hpp"

namespace Svc {

class AbstractState {
  public:
    // ----------------------------------------------------------------------
    // Constants
    // ----------------------------------------------------------------------

  public:
    // ----------------------------------------------------------------------
    // Constructors
    // ----------------------------------------------------------------------

    //! Construct an AbstractState object
    AbstractState() {}

    enum Compressible {
        UNCOMPRESSED,
        COMPRESSED,
        MINIMAL_COMPRESSED
    };

    struct Chunk {
        Compressible compressible;
        U8 sentinel;

        Chunk(Compressible c, U8 s) :
            compressible(c),
            sentinel(s) {}
    };

    Fw::Buffer build_compress_buffer(
            FwSizeType chunk_size,
            std::vector<Chunk> chunks
    );

    FwSizeType expected_compressed_size(
            FwSizeType chunk_size,
            std::vector<Chunk> chunks
    );

  public:
    // ----------------------------------------------------------------------
    // Public member functions
    // ----------------------------------------------------------------------

};

}  // namespace Svc

#endif

