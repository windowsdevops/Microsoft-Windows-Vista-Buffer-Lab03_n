/*++

Copyright (c) 1990-2000 Microsoft Corporation, All Rights Reserved

Module Name:

    toastmon.h

Abstract:

Environment:

    Kernel mode

--*/

#if     !defined(__TOASTMON_H__)
#define __TOASTMON_H__

#include <ntddk.h> //wdm.h>
#include <wdf.h>    // Driver Framework.

#define DRIVER_TAG                  'NOMT'
#define READ_BUF_SIZE           100
#define WRITE_BUF_SIZE          120

#define PERIODIC_TIMER_INTERVAL  1000 //ms

typedef enum _WORKITEM_STATE {
    WORKITEM_STATE_FREE =0,
    WORKITEM_STATE_BUSY = 1
} WORKITEM_STATE;


typedef struct _DEVICE_EXTENSION {
    WDFDEVICE           WdfDevice;
    WDFIOTARGET         ToasterTarget;
    PVOID               NotificationHandle; // Interface notification handle
    WDFCOLLECTION       TargetDeviceCollection;
    WDFWAITLOCK         TargetDeviceCollectionLock;
    PVOID               WMIDeviceArrivalNotificationObject;
} DEVICE_EXTENSION, *PDEVICE_EXTENSION;

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(DEVICE_EXTENSION, GetDeviceExtension)


typedef struct  _TARGET_DEVICE_INFO {
    PDEVICE_EXTENSION    DeviceExtension; // Our FDO device extension
    LIST_ENTRY           ListEntry; // Entry to chain to the listhead
    WDFREQUEST          ReadRequest;
    WDFREQUEST          WriteRequest;
    WDFTIMER            TimerForPostingRequests;
    WDFWORKITEM       WorkItemForPostingRequests;
    WORKITEM_STATE     WorkItemState;
} TARGET_DEVICE_INFO, *PTARGET_DEVICE_INFO;

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(TARGET_DEVICE_INFO, GetTargetDeviceInfo)

typedef struct _TIMER_CONTEXT {

   WDFIOTARGET IoTarget;

} TIMER_CONTEXT, *PTIMER_CONTEXT ;

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(TIMER_CONTEXT, GetTimerContext)

typedef struct _WORKITEM_CONTEXT {

   WDFIOTARGET IoTarget;

} WORKITEM_CONTEXT, *PWORKITEM_CONTEXT ;

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(WORKITEM_CONTEXT, GetWorkItemContext)


#if DBG
#define DebugPrint(_x_) \
               DbgPrint ("ToastMon:"); \
               DbgPrint _x_;

#define TRAP() DbgBreakPoint()

#else
#define DebugPrint(_x_)
#define TRAP()
#endif


/********************* function prototypes ***********************************/
//
DRIVER_INITIALIZE DriverEntry;

NTSTATUS
ToastMon_EvtDeviceAdd(
    IN WDFDRIVER Driver,
    IN PWDFDEVICE_INIT DeviceInit
    );

VOID
ToastMon_EvtDeviceContextCleanup(
    IN WDFDEVICE hDevice
    );

NTSTATUS
ToastMon_PnpNotifyInterfaceChange(
    IN  PDEVICE_INTERFACE_CHANGE_NOTIFICATION NotificationStruct,
    IN  PVOID                        Context
    );

NTSTATUS
ToastMon_EvtIoTargetQueryRemove(
    WDFIOTARGET IoTarget
);

VOID
ToastMon_EvtIoTargetRemoveCanceled(
    WDFIOTARGET IoTarget
);

VOID
ToastMon_EvtIoTargetRemoveComplete(
    WDFIOTARGET IoTarget
);

NTSTATUS
Toastmon_OpenDevice(
    WDFDEVICE Device,
    PUNICODE_STRING SymbolicLink,
    WDFIOTARGET *Target
    );

VOID
Toastmon_EvtWorkItemPostRequests(
    IN WDFWORKITEM WorkItem
    );

VOID
Toastmon_EvtTimerPostRequests(
    IN WDFTIMER Timer
    );

NTSTATUS
ToastMon_PostReadRequests(
    IN WDFIOTARGET IoTarget
    );

NTSTATUS
ToastMon_PostWriteRequests(
    IN WDFIOTARGET IoTarget
    );

VOID
Toastmon_ReadRequestCompletionRoutine(
    IN WDFREQUEST                  Request,
    IN WDFIOTARGET                 Target,
    PWDF_REQUEST_COMPLETION_PARAMS CompletionParams,
    IN WDFCONTEXT                  Context
    );


VOID
Toastmon_WriteRequestCompletionRoutine(
    IN WDFREQUEST                  Request,
    IN WDFIOTARGET                 Target,
    PWDF_REQUEST_COMPLETION_PARAMS CompletionParams,
    IN WDFCONTEXT                  Context
    );

NTSTATUS
RegisterForWMINotification(
    PDEVICE_EXTENSION DeviceExt
    );

VOID
UnregisterForWMINotification(
    PDEVICE_EXTENSION DeviceExt
);

VOID
WmiNotificationCallback(
    IN PVOID Wnode,
    IN PVOID Context
    );

#endif

