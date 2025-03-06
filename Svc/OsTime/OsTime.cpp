// ======================================================================
// \title  OsTime.cpp
// \author kubiak
// \brief  cpp file for OsTime component implementation class
// ======================================================================

#include "Svc/OsTime/OsTime.hpp"
#include "FpConfig.hpp"

#include <Fw/Time/TimeInterval.hpp>

namespace Svc {

// ----------------------------------------------------------------------
// Component construction and destruction
// ----------------------------------------------------------------------

OsTime ::OsTime(const char* const compName) :
    OsTimeComponentBase(compName),
    m_epoch_fw_time(Fw::ZERO_TIME),
    m_epoch_os_time() {}

OsTime ::~OsTime() {}

void OsTime::set_epoch(const Fw::Time& fw_time, const Os::RawTime& os_time) {
    m_epoch_fw_time = fw_time;
    m_epoch_os_time = os_time;
}

// ----------------------------------------------------------------------
// Handler implementations for user-defined typed input ports
// ----------------------------------------------------------------------

void OsTime ::timeGetPort_handler(FwIndexType portNum, Fw::Time& time) {
    time = Fw::ZERO_TIME;
    if (m_epoch_fw_time == Fw::ZERO_TIME) {
        return;
    }

    Os::RawTime time_now;
    Os::RawTime::Status stat = time_now.now();
    if (stat != Os::RawTime::OP_OK) {
        return;
    }

    Fw::TimeInterval elapsed;
    stat = time_now.getTimeInterval(m_epoch_os_time, elapsed);
    if (stat != Os::RawTime::OP_OK) {
        return;
    }

    time = m_epoch_fw_time;
    time.add(elapsed.getSeconds(), elapsed.getUSeconds());
}

}  // namespace Svc
