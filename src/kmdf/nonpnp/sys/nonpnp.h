/*++

Copyright (c) 1997  Microsoft Corporation

Module Name:

    nonpnp.h

Abstract:

    Contains function prototypes and includes other neccessary header files.

Environment:

    Kernel mode only.

--*/

#include <ntddk.h>
#include <wdf.h>

#define NTSTRSAFE_LIB
#include <ntstrsafe.h>
#include <wdmsec.h> // for SDDLs
#include "public.h" // contains IOCTL definitions
#include "Trace.h" // contains macros for WPP tracing

#define NTDEVICE_NAME_STRING      L"\\Device\\NONPNP"
#define SYMBOLIC_NAME_STRING     L"\\DosDevices\\NONPNP"
#define POOL_TAG                   'ELIF'

typedef struct _CONTROL_DEVICE_EXTENSION {

    HANDLE   FileHandle; // Store your control data here

} CONTROL_DEVICE_EXTENSION, *PCONTROL_DEVICE_EXTENSION;

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(CONTROL_DEVICE_EXTENSION,
                                        ControlGetData)

//
// Following request context is used only for the method-neither ioctl case.
//
typedef struct _REQUEST_CONTEXT {

    WDFMEMORY InputMemoryBuffer;
    WDFMEMORY OutputMemoryBuffer;

} REQUEST_CONTEXT, *PREQUEST_CONTEXT;

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(REQUEST_CONTEXT, GetRequestContext)

//
// Device driver routine declarations.
//

DRIVER_INITIALIZE DriverEntry;


VOID
NonPnpShutdown(
    WDFDEVICE Device
    );

NTSTATUS
NonPnpDeviceAdd(
    IN WDFDRIVER Driver,
    IN PWDFDEVICE_INIT DeviceInit
    );

VOID
NonPnpEvtDriverContextCleanup(
    IN WDFDRIVER hDriver
    );


VOID
NonPnpEvtDriverUnload(
    IN WDFDRIVER hDriver
    );


VOID
FileEvtIoDeviceControl(
    IN WDFQUEUE         Queue,
    IN WDFREQUEST       Request,
    IN size_t            OutputBufferLength,
    IN size_t            InputBufferLength,
    IN ULONG            IoControlCode
    );

VOID
NonPnpEvtDeviceFileCreate (
    IN WDFDEVICE            Device,
    IN WDFREQUEST Request,
    IN WDFFILEOBJECT        FileObject
    );

VOID
NonPnpEvtFileClose (
    IN WDFFILEOBJECT    FileObject
    );

VOID
FileEvtIoRead(
    IN WDFQUEUE         Queue,
    IN WDFREQUEST       Request,
    IN size_t            Length
    );

VOID
FileEvtIoWrite(
    IN WDFQUEUE         Queue,
    IN WDFREQUEST       Request,
    IN size_t            Length
    );


VOID
NonPnpEvtDeviceIoInCallerContext(
    IN WDFDEVICE  Device,
    IN WDFREQUEST Request
    );


VOID
PrintChars(
    __in_ecount(CountChars) PCHAR BufferAddress,
    __in size_t CountChars
    );


#pragma warning(disable:4127)

