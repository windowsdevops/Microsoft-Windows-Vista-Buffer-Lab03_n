/*++

Copyright (c) 1990-2000  Microsoft Corporation

Module Name:

    queue.h

Abstract:

    This is a C version of a very simple sample driver that illustrates
    how to use the driver framework and demonstrates best practices.

--*/

// Set max write length for testing
#define MAX_WRITE_LENGTH 1024*4

// Set timer period in ms
#define TIMER_PERIOD     1000*10

//
// This is the context that can be placed per queue
// and would contain per queue information.
//
typedef struct _QUEUE_CONTEXT {

    // Here we allocate a buffer from a test write so it can be read back
    PVOID Buffer;
    ULONG Length;

    // Timer DPC for this queue
    WDFTIMER   Timer;

    // Virtual I/O
    WDFREQUEST  CurrentRequest;
    NTSTATUS   CurrentStatus;

} QUEUE_CONTEXT, *PQUEUE_CONTEXT;

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(QUEUE_CONTEXT, QueueGetContext)

NTSTATUS
EchoQueueInitialize(
    WDFDEVICE hDevice
    );

VOID
EchoEvtIoQueueContextDestroy(
    WDFOBJECT Object
);

//
// Events from the IoQueue object
//

VOID
EchoEvtRequestCancel(
    IN WDFREQUEST Request
    );

VOID
EchoEvtIoRead(
    IN WDFQUEUE   Queue,
    IN WDFREQUEST Request,
    IN size_t      Length
    );

VOID
EchoEvtIoWrite(
    IN WDFQUEUE   Queue,
    IN WDFREQUEST Request,
    IN size_t      Length
    );


NTSTATUS
EchoTimerCreate(
    IN WDFTIMER*       pTimer,
    IN ULONG           Period,
    IN WDFQUEUE        Queue
    );


VOID
EchoEvtTimerFunc(
    IN WDFTIMER     Timer
    );



