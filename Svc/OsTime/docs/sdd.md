# Svc::OsTime Component

## 1. Introduction

The `Svc::OsTime` is a component that provides system time on any system that implements the RawTime OSAL class. The RawTime interface provides Fw::TimeInterval objects.

## 2. Requirements

| Requirement     | Description                                                                                                        | Verification |
|-----------------|--------------------------------------------------------------------------------------------------------------------|--------------|
| SVC-OS-TIME-001 | `Svc::OsTime` shall return current system time as an `Fw::Time` objects in response to the `timeGetPort` port call | Unit Test    |
| SVC-OS-TIME-002 | `Svc::OsTime` shall return ZERO_TIME as an `Fw::Time` object if the RawTime layer returns an error code            | Unit Test    |

## 3. Design

`Svc::OsTime` has a single port. It has no data types, commands, events, or telemetry channels.

`Svc::OsTime` uses an epoch time provided during initialization to calculate the absolute system time. An epoch consists of both an Fw::Time object and a Os::RawTime object that represent the same moment in time. `Svc::OsTime` calculates the time since the epoch using an Os::RawTime at the current time and the provided epoch time. The time since epoch is added to the Fw::Time timestamp to form a new Fw::Time object representing the current time.

If at any point an error status is returned from the RawTime implementation then a ZERO_TIME value is returned from the port call

### 3.1 Ports

| Port          | Kind       | Data Type | Description                          |
|---------------|------------|-----------|--------------------------------------|
| `timeGetPort` | sync input | `Fw.Time` | Port returning current system design |

## 7. Change Log

Date | Description
---- | -----------
3/10/2025  | Initial Version

