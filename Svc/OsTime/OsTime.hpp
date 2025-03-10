// ======================================================================
// \title  OsTime.hpp
// \author kubiak
// \brief  hpp file for OsTime component implementation class
// ======================================================================

#ifndef Svc_OsTime_HPP
#define Svc_OsTime_HPP

#include "Svc/OsTime/OsTimeComponentAc.hpp"

#include <Fw/Time/Time.hpp>
#include <Os/RawTime.hpp>

namespace Svc {

class OsTime final : public OsTimeComponentBase {
  public:
    // ----------------------------------------------------------------------
    // Component construction and destruction
    // ----------------------------------------------------------------------

    //! Construct OsTime object
    OsTime(const char* const compName  //!< The component name
    );

    //! Destroy OsTime object
    ~OsTime();

    //! Set an epoch time that is used to offset
    //! future Os::RawTime readings
    void set_epoch(const Fw::Time& fw_time, const Os::RawTime& os_time);

  PRIVATE:
    // ----------------------------------------------------------------------
    // Handler implementations for user-defined typed input ports
    // ----------------------------------------------------------------------

    //! Handler implementation for timeGetPort
    //!
    //! Port to retrieve time
    void timeGetPort_handler(FwIndexType portNum,  //!< The port number
                             Fw::Time& time        //!< Reference to Time object
                             ) override;

    Fw::Time m_epoch_fw_time;
    Os::RawTime m_epoch_os_time;
    bool m_epoch_valid;
};

}  // namespace Svc

#endif
