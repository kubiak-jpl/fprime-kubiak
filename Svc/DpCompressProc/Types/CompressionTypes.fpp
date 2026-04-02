module Svc {

    enum CompressionAlgorithm {
        UNCOMPRESSED = 0
        ZLIB_DEFLATE = 1
    }

    struct CompressionMetadata {
        algorithm: CompressionAlgorithm,
        payload_size: FwSizeType
    }
}
