/*++

Copyright (c) Microsoft Corporation.  All rights reserved.

    THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
    KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
    PURPOSE.

Module Name:

    private.h

Abstract:

    Contains structure definitions and function prototypes private to
    the driver.

Environment:

    Kernel mode

--*/

#pragma warning(disable:4200)  // nameless struct/union
#pragma warning(disable:4201)  // nameless struct/union
#pragma warning(disable:4214)  // bit field types other than int
#include <initguid.h>
#include <ntddk.h>
#include "usbdi.h"
#include "usbdlib.h"
#include "public.h"

#pragma warning(default:4200)
#pragma warning(default:4201)
#pragma warning(default:4214)

#include <wdf.h>
#include <wdfusb.h>
#define NTSTRSAFE_LIB
#include <ntstrsafe.h>

#include "trace.h"

#ifndef _PRIVATE_H
#define _PRIVATE_H

#define POOL_TAG (ULONG) 'FRSO'
#define _DRIVER_NAME_ "OSRUSBFX2:"

#define TEST_BOARD_TRANSFER_BUFFER_SIZE (64*1024)
#define DEVICE_DESC_LENGTH 256

//
// Define the vendor commands supported by our device
//
#define USBFX2LK_READ_7SEGMENT_DISPLAY      0xD4
#define USBFX2LK_READ_SWITCHES              0xD6
#define USBFX2LK_READ_BARGRAPH_DISPLAY      0xD7
#define USBFX2LK_SET_BARGRAPH_DISPLAY       0xD8
#define USBFX2LK_IS_HIGH_SPEED              0xD9
#define USBFX2LK_REENUMERATE                0xDA
#define USBFX2LK_SET_7SEGMENT_DISPLAY       0xDB

//
// Define the features that we can clear
//  and set on our device
//
#define USBFX2LK_FEATURE_EPSTALL            0x00
#define USBFX2LK_FEATURE_WAKE               0x01

//
// Order of endpoints in the interface descriptor
//
#define INTERRUPT_IN_ENDPOINT_INDEX    0
#define BULK_OUT_ENDPOINT_INDEX        1
#define BULK_IN_ENDPOINT_INDEX         2

//
// A structure representing the instance information associated with
// this particular device.
//

typedef struct _DEVICE_CONTEXT {

    WDFUSBDEVICE                    UsbDevice;

    WDFUSBINTERFACE                 UsbInterface;

    WDFUSBPIPE                      BulkReadPipe;

    WDFUSBPIPE                      BulkWritePipe;

    WDFUSBPIPE                      InterruptPipe;

    UCHAR                           CurrentSwitchState;

    WDFQUEUE                        InterrputMsgQueue;

} DEVICE_CONTEXT, *PDEVICE_CONTEXT;

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(DEVICE_CONTEXT, GetDeviceContext)

extern ULONG DebugLevel;


DRIVER_INITIALIZE DriverEntry;

VOID
OsrFxEvtDriverContextCleanup(
    IN WDFDRIVER Driver
    );

NTSTATUS
OsrFxEvtDeviceAdd(
    IN WDFDRIVER        Driver,
    IN PWDFDEVICE_INIT  DeviceInit
    );

NTSTATUS
OsrFxEvtDevicePrepareHardware(
    IN WDFDEVICE Device,
    IN WDFCMRESLIST ResourceList,
    IN WDFCMRESLIST ResourceListTranslated
    );

VOID
OsrFxEvtIoRead(
    IN WDFQUEUE     Queue,
    IN WDFREQUEST   Request,
    IN size_t        Length
    );

VOID
OsrFxEvtIoWrite(
    IN WDFQUEUE     Queue,
    IN WDFREQUEST   Request,
    IN size_t        Length
    );


VOID
OsrFxEvtIoDeviceControl(
    IN WDFQUEUE   Queue,
    IN WDFREQUEST Request,
    IN size_t      OutputBufferLength,
    IN size_t      InputBufferLength,
    IN ULONG      IoControlCode
    );


VOID
EvtRequestReadCompletionRoutine(
    IN WDFREQUEST                  Request,
    IN WDFIOTARGET                 Target,
    PWDF_REQUEST_COMPLETION_PARAMS Params,
    WDFCONTEXT                     Context
    );

VOID
EvtRequestWriteCompletionRoutine(
    IN WDFREQUEST                  Request,
    IN WDFIOTARGET                 Target,
    PWDF_REQUEST_COMPLETION_PARAMS Params,
    WDFCONTEXT                     Context
    );

NTSTATUS
ResetPipe(
    IN WDFUSBPIPE             Pipe
    );

NTSTATUS
ResetDevice(
    IN WDFDEVICE Device
    );

NTSTATUS
SelectInterfaces(
    IN WDFDEVICE Device
    );


NTSTATUS
AbortPipes(
    IN WDFDEVICE Device
    );

NTSTATUS
ReenumerateDevice(
    PDEVICE_CONTEXT DevContext
    );

NTSTATUS
GetBarGraphState(
    PDEVICE_CONTEXT DevContext,
    PBAR_GRAPH_STATE BarGraphState
    );

NTSTATUS
SetBarGraphState(
    PDEVICE_CONTEXT DevContext,
    PBAR_GRAPH_STATE BarGraphState
    );

NTSTATUS
GetSevenSegmentState(
    PDEVICE_CONTEXT DevContext,
    PUCHAR SevenSegment
    );

NTSTATUS
SetSevenSegmentState(
    PDEVICE_CONTEXT DevContext,
    PUCHAR SevenSegment
    );

NTSTATUS
GetSwitchState(
    PDEVICE_CONTEXT DevContext,
    PSWITCH_STATE SwitchState
    );

VOID
OsrUsbIoctlGetInterruptMessage(
    WDFDEVICE Device
    );

NTSTATUS
OsrFxSetPowerPolicy(
        IN WDFDEVICE Device
    );

NTSTATUS
OsrFxConfigContReaderForInterruptEndPoint(
    PDEVICE_CONTEXT DeviceContext
    );

VOID
OsrFxEvtUsbInterruptPipeReadComplete(
    WDFUSBPIPE  Pipe,
    WDFMEMORY   Buffer,
    size_t      NumBytesTransferred,
    WDFCONTEXT  Context
    );

BOOLEAN
OsrFxEvtUsbInterruptReadersFailed(
    WDFUSBPIPE      Pipe,
    NTSTATUS        Status,
    USBD_STATUS     UsbdStatus
    );

VOID
OsrFxEvtIoStop(
    IN WDFQUEUE     Queue,
    IN WDFREQUEST   Request,
    IN ULONG        ActionFlags
    );

NTSTATUS
OsrFxEvtDeviceD0Entry(
    IN  WDFDEVICE Device,
    IN  WDF_POWER_DEVICE_STATE PreviousState
    );

NTSTATUS
OsrFxEvtDeviceD0Exit(
    IN  WDFDEVICE Device,
    IN  WDF_POWER_DEVICE_STATE TargetState
    );

BOOLEAN
OsrFxReadFdoRegistryKeyValue(
    __in PWDFDEVICE_INIT  DeviceInit,
    __in PWCHAR           Name,
    __out PULONG          Value
    );

VOID
OsrFxEnumerateChildren(
    IN WDFDEVICE Device
    );

PCHAR
DbgDevicePowerString(
    IN WDF_POWER_DEVICE_STATE Type
    );

#endif

