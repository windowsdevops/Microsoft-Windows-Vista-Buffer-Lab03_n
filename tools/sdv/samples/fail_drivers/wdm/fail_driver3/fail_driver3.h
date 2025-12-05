/*++

Copyright (c) Microsoft Corporation.  All rights reserved.

Module Name:

    fail_driver3.h

Environment:

    Kernel mode

--*/

#include <ntddk.h>

typedef struct _FDO_DATA
{
    PDEVICE_OBJECT      Self;
    PDEVICE_OBJECT      UnderlyingPDO;
    PDEVICE_OBJECT      NextLowerDriver;
}  FDO_DATA, *PFDO_DATA;

NTSTATUS
DriverEntry(
    IN PDRIVER_OBJECT  DriverObject,
    IN PUNICODE_STRING RegistryPath
    );

NTSTATUS
DriverAddDevice(
    IN PDRIVER_OBJECT DriverObject,
    IN PDEVICE_OBJECT PhysicalDeviceObject
    );

NTSTATUS
DispatchCreate(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP           Irp
    );

NTSTATUS
DispatchClose(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP           Irp
    );

NTSTATUS
DispatchCleanup(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP           Irp
    );

NTSTATUS
DispatchRead(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP           Irp
    );

NTSTATUS
DispatchWrite(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP           Irp
    );

NTSTATUS
DispatchDeviceControl(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP           Irp
    );

NTSTATUS
DispatchInternalDeviceControl(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP           Irp
    );

NTSTATUS
DispatchPower (
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP           Irp
    );

NTSTATUS
DispatchSystemControl(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP           Irp
    );

NTSTATUS
DispatchPnp (
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP Irp
    );

VOID
DriverUnload(
    IN PDRIVER_OBJECT DriverObject
    );
