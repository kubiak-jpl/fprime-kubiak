// ======================================================================
// \title  StreamCrossover/test/ut/Tester.hpp
// \author ethanchee
// \brief  hpp file for StreamCrossover test harness implementation class
// ======================================================================

#ifndef TESTER_HPP
#define TESTER_HPP

#include "StreamCrossoverGTestBase.hpp"
#include "Drv/StreamCrossover/StreamCrossover.hpp"

namespace Drv {

  class StreamCrossoverTester :
    public StreamCrossoverGTestBase
  {

      // ----------------------------------------------------------------------
      // Construction and destruction
      // ----------------------------------------------------------------------

    public:
      // Maximum size of histories storing events, telemetry, and port outputs
      static const U32 MAX_HISTORY_SIZE = 10;
      // Instance ID supplied to the component instance under test
      static const FwEnumStoreType TEST_INSTANCE_ID = 0;

      //! Construct object StreamCrossoverTester
      //!
      StreamCrossoverTester();

      //! Destroy object StreamCrossoverTester
      //!
      ~StreamCrossoverTester();

    public:

      // ----------------------------------------------------------------------
      // Tests
      // ----------------------------------------------------------------------

      //! Send a test buffer to streamOut from streamIn
      //!
      void sendTestBuffer();

      //! Send a fail RECV_STATUS to test error
      //!
      void testFail();

    private:

      // ----------------------------------------------------------------------
      // Handlers for typed from ports
      // ----------------------------------------------------------------------

      //! Handler for from_streamOut
      //!
      Drv::SendStatus from_streamOut_handler(
          const FwIndexType portNum, /*!< The port number*/
          Fw::Buffer &sendBuffer 
      );

      //! Handler for from_deallocate
      //!
      void from_errorDeallocate_handler(
          const FwIndexType portNum, /*!< The port number*/
          Fw::Buffer &fwBuffer 
      );

    private:

      // ----------------------------------------------------------------------
      // Helper methods
      // ----------------------------------------------------------------------

      //! Connect ports
      //!
      void connectPorts();

      //! Initialize components
      //!
      void initComponents();

    private:

      // ----------------------------------------------------------------------
      // Variables
      // ----------------------------------------------------------------------

      //! The component under test
      //!
      StreamCrossover component;

  };

} // end namespace Drv

#endif
