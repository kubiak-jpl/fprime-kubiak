module DataProductsConfig {
    #Base ID for the DataProducts Subtopology, all components are offsets from this base ID
    constant BASE_ID = 0x04000000
    
    module QueueSizes {
        constant dpCat    = 10
        constant dpMgr  = 10
        constant dpWriter   = 10
        constant dpBufferManager   = 10
    }
    

    module StackSizes {
        constant dpCat    = 64 * 1024
        constant dpMgr  = 64 * 1024
        constant dpWriter   = 64 * 1024
        constant dpBufferManager   = 64 * 1024
    }

    module Priorities {
        constant dpCat    = 24
        constant dpMgr  = 23
        constant dpWriter   = 22
        constant dpBufferManager  = 21
        constant dpCompressProc = 0
        constant dpZLibCompressor = 0
        constant dpZLibCompressorBufferManager = 0
    }

    # Buffer management constants
    module BuffMgr {
        constant dpBufferStoreSize  = 10000   
        constant dpBufferStoreCount = 10       
        constant dpBufferManagerId  = 300         
    }

    module ZLibBuffMgr {
        constant compressionBufferStoreSize  = 32*1024
        constant compressionBufferStoreCount = 2
        constant allocBufferStoreSize  = 269000
        constant allocBufferStoreCount = 2
        constant dpZLibCompressorBufferManagerId  = 301
    }
    
    # Directory and file paths
    module Paths {
        constant dpDir    = "./DpCat"                
        constant dpState  = "./DpCat/DpState.dat"  
    }
}
