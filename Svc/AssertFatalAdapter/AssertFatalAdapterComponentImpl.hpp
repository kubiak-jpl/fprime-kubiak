// ======================================================================
// \title  AssertFatalAdapterImpl.hpp
// \author tcanham
// \brief  hpp file for AssertFatalAdapter component implementation class
//
// \copyright
// Copyright 2009-2015, by the California Institute of Technology.
// ALL RIGHTS RESERVED.  United States Government Sponsorship
// acknowledged.
//
// ======================================================================

#ifndef AssertFatalAdapter_HPP
#define AssertFatalAdapter_HPP

#include "Svc/AssertFatalAdapter/AssertFatalAdapterComponentAc.hpp"

namespace Svc {

  class AssertFatalAdapterComponentImpl final :
    public AssertFatalAdapterComponentBase
  {

    public:

      // ----------------------------------------------------------------------
      // Construction, initialization, and destruction
      // ----------------------------------------------------------------------

      //! Construct object AssertFatalAdapter
      //!
      AssertFatalAdapterComponentImpl(
          const char *const compName /*!< The component name*/
      );

      //! Destroy object AssertFatalAdapter
      //!
      ~AssertFatalAdapterComponentImpl();

      //! Report the assert as a FATAL
      void reportAssert(
          FILE_NAME_ARG file,
          FwSizeType lineNo,
          FwSizeType numArgs,
          FwAssertArgType arg1,
          FwAssertArgType arg2,
          FwAssertArgType arg3,
          FwAssertArgType arg4,
          FwAssertArgType arg5,
          FwAssertArgType arg6
          );

    private:

      class AssertFatalAdapter : public Fw::AssertHook {
          public:
              AssertFatalAdapter();
              ~AssertFatalAdapter();
              void regAssertReporter(AssertFatalAdapterComponentImpl* compPtr);
          private:
              void reportAssert(
                  FILE_NAME_ARG file,
                  FwSizeType lineNo,
                  FwSizeType numArgs,
                  FwAssertArgType arg1,
                  FwAssertArgType arg2,
                  FwAssertArgType arg3,
                  FwAssertArgType arg4,
                  FwAssertArgType arg5,
                  FwAssertArgType arg6
                  );

              // Prevent actual assert since FATAL handler will deal with it
              void doAssert();

              AssertFatalAdapterComponentImpl* m_compPtr;
      };

      AssertFatalAdapter m_adapter;

    };

} // end namespace Svc

#endif
