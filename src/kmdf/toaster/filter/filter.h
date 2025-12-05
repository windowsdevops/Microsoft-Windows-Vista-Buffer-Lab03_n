/*++

Copyright (c) Microsoft Corporation.  All rights reserved.

    THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
    KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
    PURPOSE.

Module Name:

    filter.h

Abstract:

    Contains structure definitions and function prototypes for filter driver.

Environment:

    Kernel mode

--*/

#include <ntddk.h>
#include <wdf.h>
#include <wdmsec.h> // for SDDLs
#define NTSTRSAFE_LIB
#include <ntstrsafe.h>

#if !defined(_FILTER_H_)
#define _FILTER_H_


#define DRIVERNAME "DevUpper.sys: "


typedef struct _FILTER_EXTENSION {
    ULONG     SerialNo;
} FILTER_EXTENSION, *PFILTER_EXTENSION;

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(FILTER_EXTENSION,
                                        FilterGetData)

#define NTDEVICE_NAME_STRING      L"\\Device\\ToasterFilter"
#define SYMBOLIC_NAME_STRING      L"\\DosDevices\\ToasterFilter"

typedef struct _CONTROL_DEVICE_EXTENSION {

    PVOID   ControlData; // Store your control data here

} CONTROL_DEVICE_EXTENSION, *PCONTROL_DEVICE_EXTENSION;

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(CONTROL_DEVICE_EXTENSION,
                                        ControlGetData)

DRIVER_INITIALIZE DriverEntry;

VOID
FilterEvtDriverUnload(
    WDFDRIVER Driver
    );

NTSTATUS
FilterEvtDeviceAdd(
    IN WDFDRIVER        Driver,
    IN PWDFDEVICE_INIT  DeviceInit
    );

VOID
FilterEvtDeviceContextCleanup(
    IN WDFDEVICE Device
    );

#ifdef IOCTL_INTERFACE

NTSTATUS
FilterCreateControlDevice(
    WDFDEVICE Device
    );

VOID
FilterDeleteControlDevice(
    WDFDEVICE Device
    );

VOID
FilterEvtIoDeviceControl(
    IN WDFQUEUE Queue,
    IN WDFREQUEST Request,
    IN size_t OutputBufferLength,
    IN size_t InputBufferLength,
    IN ULONG IoControlCode
    );
#endif

#endif

