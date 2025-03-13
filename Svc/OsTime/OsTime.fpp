module Svc {

    @ A pair of timestamps representing an Epoch time in 
    @ an Fw::Time and Os::RawTime object
    port OsTimeEpoch(
        fw_Time: Fw.Time,
        os_time: Os.RawTime
    )

    @ A time component using OSAL RawTime abstractions
    passive component OsTime {
        include "../Interfaces/TimeInterface.fppi"

        sync input port setEpoch: OsTimeEpoch
    }
}
