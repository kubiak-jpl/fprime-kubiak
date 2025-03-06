// ======================================================================
// \title  OsTimeTester.cpp
// \author kubiak
// \brief  cpp file for OsTime component test harness implementation class
// ======================================================================

#include "OsTimeTester.hpp"

namespace Svc {

  // ----------------------------------------------------------------------
  // Construction and destruction
  // ----------------------------------------------------------------------

  OsTimeTester ::
    OsTimeTester() :
      OsTimeGTestBase("OsTimeTester", OsTimeTester::MAX_HISTORY_SIZE),
      component("OsTime")
  {
    this->initComponents();
    this->connectPorts();
  }

  OsTimeTester ::
    ~OsTimeTester()
  {

  }

  // ----------------------------------------------------------------------
  // Tests
  // ----------------------------------------------------------------------

  void OsTimeTester ::
    toDo()
  {
    // TODO
  }

}
