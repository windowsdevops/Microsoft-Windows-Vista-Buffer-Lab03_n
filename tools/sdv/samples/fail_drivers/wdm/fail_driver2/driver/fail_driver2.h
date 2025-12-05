/*++

Copyright (c) Microsoft Corporation.  All rights reserved.

Module Name:

    fail_driver2.h

Environment:

    Kernel mode

--*/

#include <wdm.h>
#include "fail_library2.h"

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
DispatchPower (
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP           Irp
    );

NTSTATUS
DispatchSystemControl(
    IN PDEVICE_OBJECT DeviceObject,
    IN PIRP           Irp
    );

VOID
DriverUnload(
    IN PDRIVER_OBJECT DriverObject
    );