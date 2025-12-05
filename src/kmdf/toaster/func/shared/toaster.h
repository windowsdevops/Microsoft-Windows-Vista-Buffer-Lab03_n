/*++

Copyright (c) 1990-2000 Microsoft Corporation All Rights Reserved

Module Name:

    Toaster.h

Abstract:

    Header file for the toaster driver modules.

Environment:

    Kernel mode

--*/


#if !defined(_TOASTER_H_)
#define _TOASTER_H_

#include <ntddk.h>
#include <wdf.h>

#define NTSTRSAFE_LIB
#include <ntstrsafe.h>

#include "wmilib.h"
#include <initguid.h>
#include "..\inc\driver.h"
#include "..\inc\public.h"

#define TOASTER_POOL_TAG (ULONG) 'saoT'

#define MOFRESOURCENAME L"ToasterWMI"

//
// The device extension for the device object
//
typedef struct _FDO_DATA
{

    WDFWMIINSTANCE WmiDeviceArrivalEvent;

    BOOLEAN     WmiPowerDeviceEnableRegistered;

    TOASTER_INTERFACE_STANDARD BusInterface;

}  FDO_DATA, *PFDO_DATA;

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(FDO_DATA, ToasterFdoGetData)


//
// Connector Types
//

#define TOASTER_WMI_STD_I8042 0
#define TOASTER_WMI_STD_SERIAL 1
#define TOASTER_WMI_STD_PARALEL 2
#define TOASTER_WMI_STD_USB 3

DRIVER_INITIALIZE DriverEntry;


NTSTATUS
ToasterEvtDeviceAdd(
    IN WDFDRIVER Driver,
    IN PWDFDEVICE_INIT DeviceInit
    );


NTSTATUS
ToasterEvtDevicePrepareHardware(
    WDFDEVICE Device,
    WDFCMRESLIST Resources,
    WDFCMRESLIST ResourcesTranslated
    );

NTSTATUS
ToasterEvtDeviceReleaseHardware(
    IN  WDFDEVICE Device,
    IN  WDFCMRESLIST ResourcesTranslated
    );

NTSTATUS
ToasterEvtDeviceSelfManagedIoInit(
    IN  WDFDEVICE Device
    );

VOID
ToasterEvtDeviceContextCleanup(
    IN WDFDEVICE Device
    );

NTSTATUS
ToasterWmiRegistration(
    __in WDFDEVICE Device
);

//
// Power events callbacks
//

WDF_POWER_DEVICE_STATE
ToasterEvtDeviceAdjustPowerState(
    IN WDFDEVICE            Device,
    IN SYSTEM_POWER_STATE   SystemState,
    IN WDF_POWER_DEVICE_STATE   DeviceState
    );

NTSTATUS
ToasterEvtDeviceD0Entry(
    IN WDFDEVICE            Device,
    IN WDF_POWER_DEVICE_STATE   RecentPowerState
    );

NTSTATUS
ToasterEvtDeviceD0Exit(
    IN WDFDEVICE            Device,
    IN WDF_POWER_DEVICE_STATE   PowerState
    );

NTSTATUS
ToasterEvtDeviceWakeArm(
    IN WDFDEVICE Device
    );

VOID
ToasterEvtDeviceWakeDisarm(
    IN WDFDEVICE Device
    );

VOID
ToasterEvtDeviceWakeTriggered(
    IN WDFDEVICE Device
    );

PCHAR
DbgDevicePowerString(
    IN WDF_POWER_DEVICE_STATE Type
    );

//
// Io events callbacks.
//
VOID
ToasterEvtIoDeviceControl(
    IN WDFQUEUE Queue,
    IN WDFREQUEST Request,
    IN size_t OutputBufferLength,
    IN size_t InputBufferLength,
    IN ULONG IoControlCode
    );

VOID
ToasterEvtDeviceFileCreate (
    IN WDFDEVICE hDevice,
    IN WDFREQUEST Request,
    IN WDFFILEOBJECT FileObject
    );


VOID
ToasterEvtFileClose (
    IN WDFFILEOBJECT FileObject
    );

VOID
ToasterEvtIoRead (
    WDFQUEUE      Queue,
    WDFREQUEST    Request,
    size_t         Length
    );

VOID
ToasterEvtIoWrite (
    WDFQUEUE      Queue,
    WDFREQUEST    Request,
    size_t         Length
    );
//
// WMI event callbacks
//
NTSTATUS
EvtWmiInstanceStdDeviceDataQueryInstance(
    __in  WDFWMIINSTANCE WmiInstance,
    __in  ULONG OutBufferSize,
    __in  PVOID OutBuffer,
    __out PULONG BufferUsed
    );

NTSTATUS
EvtWmiInstanceStdDeviceDataSetInstance(
    __in WDFWMIINSTANCE WmiInstance,
    __in ULONG InBufferSize,
    __in PVOID InBuffer
    );

NTSTATUS
EvtWmiInstanceStdDeviceDataSetItem(
    __in WDFWMIINSTANCE WmiInstance,
    __in ULONG DataItemId,
    __in ULONG InBufferSize,
    __in PVOID InBuffer
    );


NTSTATUS
ToasterFireArrivalEvent(
    __in WDFDEVICE  Device
    );


NTSTATUS
EvtWmiInstanceToasterControlQueryInstance(
    __in  WDFWMIINSTANCE WmiInstance,
    __in  ULONG OutBufferSize,
    __in  PVOID OutBuffer,
    __out PULONG BufferUsed
    );

NTSTATUS
EvtWmiInstanceToasterControlSetInstance(
    __in WDFWMIINSTANCE WmiInstance,
    __in ULONG InBufferSize,
    __in PVOID InBuffer
    );

NTSTATUS
EvtWmiInstanceToasterControlSetItem(
    __in WDFWMIINSTANCE WmiInstance,
    __in ULONG DataItemId,
    __in ULONG InBufferSize,
    __in PVOID InBuffer
    );

NTSTATUS
EvtWmiInstanceToasterControlExecuteMethod(
    __in    WDFWMIINSTANCE WmiInstance,
    __in    ULONG MethodId,
    __in    ULONG InBufferSize,
    __in    ULONG OutBufferSize,
    __inout PVOID Buffer,
    __out   PULONG BufferUsed
    );

#endif  // _TOASTER_H_

