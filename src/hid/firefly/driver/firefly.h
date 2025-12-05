/*++

Copyright (c) Microsoft Corporation.  All rights reserved.

    THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
    KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
    PURPOSE.


Module Name:

    firefly.h

Abstract:

    This header contains Device driver that controls the Firefly device.

Environment:

    Kernel mode

--*/

#if !defined(_FIREFLY_H_)
#define _FIREFLY_H_

#include <ntddk.h>
#include <initguid.h>
#include <wdmguid.h>
#include <wmistr.h>
#include <wmilib.h>
#include "firefly.h"
#include "magic.h"
#include <dontuse.h>

//
// This is the WMI guid for our functionality.
//
//{ab27db29-db25-42e6-a3e7-28bd46bdb666}
DEFINE_GUID(                                                                  \
    FIREFLY_WMI_STD_DATA_GUID,                                                \
    0xAB27DB29, 0xDB25, 0x42E6, 0xA3, 0xE7, 0x28, 0xBD, 0x46, 0xBD, 0xB6, 0x66\
    );


#define DRIVERNAME "firefly.sys: "

#define POOL_TAG   'fFtS'

#if DBG
#define DebugPrint(_x_) \
               DbgPrint(DRIVERNAME); \
               DbgPrint _x_;
#else
#define DebugPrint(_x_)
#endif

#define ARRAYLEN(array) (sizeof(array)/sizeof(array[0]))

//
// These are the states Filter transition to upon receiving a specific PnP Irp.
// Refer to the PnP Device States diagram in DDK documentation for better
// understanding.
//
typedef enum {

    NotStarted = 0,         // Not started yet
    Started,                // Device has received the START_DEVICE IRP
    StopPending,            // Device has received the QUERY_STOP IRP
    Stopped,                // Device has received the STOP_DEVICE IRP
    RemovePending,          // Device has received the QUERY_REMOVE IRP
    SurpriseRemovePending,  // Device has received the SURPRISE_REMOVE IRP
    Deleted                 // Device has received the REMOVE_DEVICE IRP

} DEVICE_PNP_STATE;

#define INITIALIZE_PNP_STATE(_Data_)    \
        (_Data_)->DevicePnPState =  NotStarted;\
        (_Data_)->PreviousPnPState = NotStarted;

#define SET_NEW_PNP_STATE(_Data_, _state_) \
        (_Data_)->PreviousPnPState =  (_Data_)->DevicePnPState;\
        (_Data_)->DevicePnPState = (_state_);

#define RESTORE_PREVIOUS_PNP_STATE(_Data_)   \
        (_Data_)->DevicePnPState =   (_Data_)->PreviousPnPState;\


typedef struct {

    //
    // TRUE if the tail light is on, FALSE otherwise.
    //
    BOOLEAN TailLit;

} FIREFLY_WMI_STD_DATA, *PFIREFLY_WMI_STD_DATA;

typedef struct {

    //
    // A back pointer to the device object.
    //
    PDEVICE_OBJECT Self;

    //
    // The top of the stack before this filter was added.
    //
    PDEVICE_OBJECT NextLowerDriver;

    //
    // Bottom of the stack.
    //
    PDEVICE_OBJECT PhysicalDeviceObject;

    //
    // Current PnP state of the device
    //
    DEVICE_PNP_STATE DevicePnPState;

    //
    // Previous PnP state of the device
    //
    DEVICE_PNP_STATE PreviousPnPState;

    //
    // WMI Information.
    //
    WMILIB_CONTEXT WmiLibInfo;

    //
    // Removelock to track IRPs so that device can be removed and
    // the driver can be unloaded safely.
    //
    IO_REMOVE_LOCK RemoveLock;

    //
    // Our firefly filter data.
    //
    FIREFLY_WMI_STD_DATA StdDeviceData;

} DEVICE_EXTENSION, *PDEVICE_EXTENSION;



DRIVER_INITIALIZE DriverEntry;

DRIVER_ADD_DEVICE FireflyAddDevice;

DRIVER_DISPATCH FireflyDispatchPnp;

DRIVER_DISPATCH FireflyDispatchPower;

DRIVER_DISPATCH FireflyDispatchWmi;

DRIVER_DISPATCH FireflyForwardIrp;

DRIVER_UNLOAD FireflyUnload;

NTSTATUS
FireflySynchronousCompletionRoutine(
    __in PDEVICE_OBJECT   DeviceObject,
    __in PIRP             Irp,
    __in PVOID            Context
    );

NTSTATUS
FireflyRegisterWmi(
    __in  PDEVICE_EXTENSION   DeviceExtension
    );

NTSTATUS
FireflySetWmiDataItem(
    __in PDEVICE_OBJECT   DeviceObject,
    __in PIRP             Irp,
    __in ULONG            GuidIndex,
    __in ULONG            InstanceIndex,
    __in ULONG            DataItemId,
    __in ULONG            BufferSize,
    __in_bcount(BufferSize) PUCHAR Buffer
    );

NTSTATUS
FireflySetWmiDataBlock(
    __in PDEVICE_OBJECT   DeviceObject,
    __in PIRP             Irp,
    __in ULONG            GuidIndex,
    __in ULONG            InstanceIndex,
    __in ULONG            BufferSize,
    __in PUCHAR           Buffer
    );

NTSTATUS
FireflyQueryWmiDataBlock(
    __in      PDEVICE_OBJECT  DeviceObject,
    __in      PIRP            Irp,
    __in      ULONG           GuidIndex,
    __in      ULONG           InstanceIndex,
    __in      ULONG           InstanceCount,
    __inout  PULONG          InstanceLengthArray,
    __in      ULONG           OutBufferSize,
    __out     PUCHAR          Buffer
    );

NTSTATUS
FireflyQueryWmiRegInfo(
    __in  PDEVICE_OBJECT      DeviceObject,
    __out ULONG              *RegFlags,
    __out PUNICODE_STRING     InstanceName,
    __out PUNICODE_STRING    *RegistryPath,
    __out PUNICODE_STRING     MofResourceName,
    __out PDEVICE_OBJECT     *Pdo
    );

PCHAR
PnPMinorFunctionString(
    __in UCHAR            MinorFunction
    );

NTSTATUS
FireflySendIoctl(
    __in  PDEVICE_OBJECT  DeviceObject,
    __in  ULONG           ControlCode,
    __in_opt  PVOID       InputBuffer,
    __in  ULONG           InputBufferLength,
    __out_opt PVOID       OutputBuffer,
    __in  ULONG           OutputBufferLength,
    __in  PFILE_OBJECT    FileObject
    );

NTSTATUS
FireflySetFeature(
    __in  PDEVICE_OBJECT  PhysicalDeviceObject,
    __in  UCHAR           PageId,
    __in  USHORT          FeatureId,
    __in  BOOLEAN         EnableFeature
    );

NTSTATUS
FireflyOpenStack(
    __in  PDEVICE_OBJECT   DeviceObject,
    __out PFILE_OBJECT    *FileObject
    );

#endif // _FIREFLY_H_



