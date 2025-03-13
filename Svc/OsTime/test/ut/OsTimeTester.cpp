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

#define TIMING_UB (1.5)
#define ASSERT_TIMING_RAW(exp_base, exp_offset, act, ub_ratio) \
{ \
ASSERT_LT(exp_base + exp_offset, act); \
ASSERT_GT(exp_base + (exp_offset * ub_ratio), act); \
}

#define ASSERT_TIMING(exp_base, exp_offset, act) ASSERT_TIMING_RAW(exp_base, exp_offset, act, TIMING_UB)


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
      ASSERT_TIMING(0, 0.2, time_200);

      // 400 ms
      std::this_thread::sleep_for(dur_200ms);
      Fw::Time time_400ms;
      invoke_to_timeGetPort(0, time_400ms);
      ASSERT_NE(time_400ms, Fw::ZERO_TIME);

      const double time_400 = time_400ms.getSeconds() + (time_400ms.getUSeconds() / (1000.*1000.));
      ASSERT_TIMING(0, 0.4, time_400);

      // 600 ms
      std::this_thread::sleep_for(dur_200ms);
      Fw::Time time_600ms;
      invoke_to_timeGetPort(0, time_600ms);
      ASSERT_NE(time_600ms, Fw::ZERO_TIME);

      const double time_600 = time_600ms.getSeconds() + (time_600ms.getUSeconds() / (1000.*1000.));
      ASSERT_TIMING(0, 0.6, time_600);

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
      ASSERT_TIMING(start_time_d, 0.2, time_200);
  }

  void OsTimeTester ::
    noEpochTest()
  {
      // 0 ms
      Fw::Time time_0ms;
      invoke_to_timeGetPort(0, time_0ms);
      ASSERT_EQ(time_0ms, Fw::ZERO_TIME);

      const std::chrono::milliseconds dur_200ms(200);

      // 200 ms
      std::this_thread::sleep_for(dur_200ms);
      Fw::Time time_200ms;
      invoke_to_timeGetPort(0, time_200ms);
      ASSERT_EQ(time_200ms, Fw::ZERO_TIME);
  }

  void OsTimeTester ::
    updateEpochTest()
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
      ASSERT_TIMING(0, 0.2, time_200);

      // 400 ms
      std::this_thread::sleep_for(dur_200ms);
      Fw::Time time_400ms;
      invoke_to_timeGetPort(0, time_400ms);
      ASSERT_NE(time_400ms, Fw::ZERO_TIME);

      const double time_400 = time_400ms.getSeconds() + (time_400ms.getUSeconds() / (1000.*1000.));
      ASSERT_TIMING(0, 0.4, time_400);

      // Change time base
      const Fw::Time new_base(TB_WORKSTATION_TIME, 7, 1234, 0);
      os_stat = epoch_os_time.now();
      ASSERT_EQ(os_stat, Os::RawTime::OP_OK);
      invoke_to_setEpoch(0, new_base, epoch_os_time);

      // Immediately get time
      Fw::Time time_400ms_2;
      invoke_to_timeGetPort(0, time_400ms_2);
      ASSERT_NE(time_400ms_2, Fw::ZERO_TIME);
      ASSERT_EQ(time_400ms_2.getTimeBase(), TB_WORKSTATION_TIME);
      ASSERT_EQ(time_400ms_2.getContext(), 7);

      const double time_400_2 = time_400ms_2.getSeconds() + (time_400ms_2.getUSeconds() / (1000.*1000.));
      //ASSERT_TIMING(1234.0, 0, time_400_2);
      ASSERT_LT(1234.0, time_400_2);
      ASSERT_GT(1234.0 + 0.1, time_400_2);

      // 600 ms
      std::this_thread::sleep_for(dur_200ms);
      Fw::Time time_600ms;
      invoke_to_timeGetPort(0, time_600ms);
      ASSERT_NE(time_600ms, Fw::ZERO_TIME);
      ASSERT_EQ(time_600ms.getTimeBase(), TB_WORKSTATION_TIME);
      ASSERT_EQ(time_600ms.getContext(), 7);

      const double time_600 = time_600ms.getSeconds() + (time_600ms.getUSeconds() / (1000.*1000.));
      ASSERT_TIMING(1234.0, 0.2, time_600);



  }

}
