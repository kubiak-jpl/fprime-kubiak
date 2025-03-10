// ======================================================================
// \title  OsTimeTester.cpp
// \author kubiak
// \brief  cpp file for OsTime component test harness implementation class
// ======================================================================

#include "OsTimeTester.hpp"
#include "Svc/OsTime/OsTime.hpp"

#include <cstdint>
#include <chrono>
#include <thread>

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
    zeroEpochTest()
  {
      Os::RawTime epoch_os_time;
      Os::RawTime::Status os_stat = epoch_os_time.now();
      ASSERT_EQ(os_stat, Os::RawTime::OP_OK);
      component.set_epoch(Fw::ZERO_TIME, epoch_os_time);

      const std::chrono::milliseconds dur_200ms(200);

      // 200 ms
      std::this_thread::sleep_for(dur_200ms);
      Fw::Time time_200ms;
      invoke_to_timeGetPort(0, time_200ms);
      ASSERT_NE(time_200ms, Fw::ZERO_TIME);

      const double time_200 = time_200ms.getSeconds() + (time_200ms.getUSeconds() / (1000.*1000.));
      ASSERT_NEAR(0.2, time_200, 0.1);

      // 400 ms
      std::this_thread::sleep_for(dur_200ms);
      Fw::Time time_400ms;
      invoke_to_timeGetPort(0, time_400ms);
      ASSERT_NE(time_400ms, Fw::ZERO_TIME);

      const double time_400 = time_400ms.getSeconds() + (time_400ms.getUSeconds() / (1000.*1000.));
      ASSERT_NEAR(0.4, time_400, 0.1);

      // 600 ms
      std::this_thread::sleep_for(dur_200ms);
      Fw::Time time_600ms;
      invoke_to_timeGetPort(0, time_600ms);
      ASSERT_NE(time_600ms, Fw::ZERO_TIME);

      const double time_600 = time_600ms.getSeconds() + (time_600ms.getUSeconds() / (1000.*1000.));
      ASSERT_NEAR(0.6, time_600, 0.1);

  }

  void OsTimeTester ::
    offsetEpochTest()
  {
      Os::RawTime epoch_os_time;
      Os::RawTime::Status os_stat = epoch_os_time.now();
      Fw::Time start_time(12345, 800000);
      double start_time_d = 12345.8;
      ASSERT_EQ(os_stat, Os::RawTime::OP_OK);
      component.set_epoch(start_time, epoch_os_time);

      const std::chrono::milliseconds dur_200ms(200);

      // 200 ms
      std::this_thread::sleep_for(dur_200ms);
      Fw::Time time_200ms;
      invoke_to_timeGetPort(0, time_200ms);
      ASSERT_NE(time_200ms, Fw::ZERO_TIME);

      const double time_200 = time_200ms.getSeconds() + (time_200ms.getUSeconds() / (1000.*1000.));
      ASSERT_NEAR(start_time_d + 0.2, time_200, 0.1);
  }

}
