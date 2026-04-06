// ======================================================================
// \title  AbstractState.cpp
// \author Gerik Kubiak
// \brief  Cpp file for abstract state
//
// \copyright
// Copyright (C) 2024 California Institute of Technology.
// ALL RIGHTS RESERVED.  United States Government sponsorship
// acknowledged.
// ======================================================================

#include <cassert>

#include "AbstractState.hpp"
#include "Fw/Dp/DpContainer.hpp"

#include <gtest/gtest.h>

namespace Svc {

Fw::Buffer AbstractState::build_compress_buffer(
    FwSizeType chunk_size,
    std::vector<Chunk> chunks
) {
    FwSizeType mem_size = chunk_size * chunks.size() + Fw::DpContainer::MIN_PACKET_SIZE;
    U8* mem = new U8[mem_size];

    for (FwSizeType idx = 0; idx < chunks.size(); idx++) {
        auto chunk = chunks[idx];

        mem[Fw::DpContainer::DATA_OFFSET + chunk_size * idx] =
            static_cast<U8>(chunk.compressible);

        memset(&mem[Fw::DpContainer::DATA_OFFSET + chunk_size*idx + 1],
               chunk.sentinel,
               chunk_size - 1);
    }

    Fw::Buffer buf(mem, mem_size);
    Fw::DpContainer container(0, buf);

    container.setDataSize(chunk_size * chunks.size());
    container.serializeHeader();

    return buf;
}

FwSizeType AbstractState::expected_compressed_size(
    FwSizeType chunk_size,
    std::vector<Chunk> chunks
) {
    return exp_compressed_size_;
    /*
    FwSizeType sum_size = 0;
    const FwSizeType header_size =
        sizeof(FwDpIdType) + Svc::CompressionMetadata::SERIALIZED_SIZE;

    bool last_uncompressed = false;

    for (auto chunk : chunks) {
        switch (chunk.compressible) {
            case AbstractState::UNCOMPRESSED:
                sum_size += chunk_size;
                if (!last_uncompressed) {
                    sum_size += header_size;
                }
                break;
            case AbstractState::COMPRESSED:
                sum_size += chunk_size/2;
                sum_size += header_size;
                break;
            case AbstractState::MINIMAL_COMPRESSED:
                // Don't handle this
                assert(false);
                break;
        }

        last_uncompressed = chunk.compressible == AbstractState::UNCOMPRESSED;
    }

    return sum_size;
    */
}

void AbstractState::check_uncompressed_data(
    std::vector<U8> data_vec,
    FwSizeType chunk_size,
    std::vector<Chunk> chunks
) {
    ASSERT_EQ(data_vec.size(), chunk_size*chunks.size());

    for (U32 c = 0; c < chunks.size(); c++) {
        ASSERT_EQ(data_vec.at(c*chunk_size),
                  chunks[c].compressible);

        ASSERT_EQ(data_vec.at(c*chunk_size + 1),
                  chunks[c].sentinel);
    }
}

}

