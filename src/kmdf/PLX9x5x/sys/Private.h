/*++

Copyright (c) Microsoft Corporation.  All rights reserved.

    THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
    KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
    PURPOSE.

Module Name:

    Private.h

Abstract:

Environment:

    Kernel mode

--*/


#if !defined(_PCI9656_H_)
#define _PCI9659_H_

//
// The device extension for the device object
//
typedef struct _DEVICE_EXTENSION {

    WDFDEVICE               Device;

    // Following fields are specific to the hardware
    // Configuration

    // HW Resources
    PPCI9656_REGS           Regs;             // Registers address
    PUCHAR                  RegsBase;         // Registers base address
    ULONG                   RegsLength;       // Registers base length

    PUCHAR                  PortBase;         // Port base address
    ULONG                   PortLength;       // Port base length

    PUCHAR                  SRAMBase;         // SRAM base address
    ULONG                   SRAMLength;       // SRAM base length

    PUCHAR                  SRAM2Base;        // SRAM (alt) base address
    ULONG                   SRAM2Length;      // SRAM (alt) base length

    WDFINTERRUPT            Interrupt;     // Returned by InterruptCreate

    union {
        INT_CSR bits;
        ULONG   ulong;
    }                       IntCsr;

    union {
        DMA_CSR bits;
        UCHAR   uchar;
    }                       Dma0Csr;

    union {
        DMA_CSR bits;
        UCHAR   uchar;
    }                       Dma1Csr;

    // DmaEnabler
    WDFDMAENABLER           DmaEnabler;
    ULONG                   MaximumTransferLength;

    // Write
    WDFQUEUE                WriteQueue;

    WDFDMATRANSACTION       CurrentWriteDmaTransaction;

    ULONG                   WriteTransferElements;
    WDFCOMMONBUFFER         WriteCommonBuffer;
    size_t                  WriteCommonBufferSize;
    PUCHAR                  WriteCommonBufferBase;
    PHYSICAL_ADDRESS        WriteCommonBufferBaseLA;  // Logical Address

    // Read
    ULONG                   ReadTransferElements;
    WDFCOMMONBUFFER         ReadCommonBuffer;
    size_t                  ReadCommonBufferSize;
    PUCHAR                  ReadCommonBufferBase;
    PHYSICAL_ADDRESS        ReadCommonBufferBaseLA;   // Logical Address

    WDFDMATRANSACTION       CurrentReadDmaTransaction;

    WDFQUEUE                ReadQueue;

    ULONG                   HwErrCount;

}  DEVICE_EXTENSION, *PDEVICE_EXTENSION;

//
// This will generate the function named PLxGetDeviceContext to be use for
// retreiving the DEVICE_EXTENSION pointer.
//
WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(DEVICE_EXTENSION, PLxGetDeviceContext)

#if !defined(ASSOC_WRITE_REQUEST_WITH_DMA_TRANSACTION)
//
// The context structure used with WdfDmaTransactionCreate
//
typedef struct TRANSACTION_CONTEXT {

    WDFREQUEST     Request;

} TRANSACTION_CONTEXT, * PTRANSACTION_CONTEXT;

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(TRANSACTION_CONTEXT, PLxGetTransactionContext)

#endif

//
// Function prototypes
//
DRIVER_INITIALIZE DriverEntry;


NTSTATUS
PLxEvtDeviceAdd(
    IN WDFDRIVER        Driver,
    IN PWDFDEVICE_INIT  DeviceInit
    );

NTSTATUS
PLxEvtDevicePrepareHardware (
    WDFDEVICE       hDevice,
    WDFCMRESLIST   Resources,
    WDFCMRESLIST   ResourcesTranslated
    );

NTSTATUS
PLxEvtDeviceReleaseHardware(
    IN  WDFDEVICE Device,
    IN  WDFCMRESLIST ResourcesTranslated
    );

NTSTATUS
PLxEvtDeviceD0Entry(
    IN  WDFDEVICE Device,
    IN  WDF_POWER_DEVICE_STATE PreviousState
    );

NTSTATUS
PLxEvtDeviceD0Exit(
    IN  WDFDEVICE Device,
    IN  WDF_POWER_DEVICE_STATE TargetState
    );

NTSTATUS
PLxSetIdleAndWakeSettings(
        IN PDEVICE_EXTENSION FdoData
    );

VOID
PLxEvtIoWrite(
    IN WDFQUEUE    Queue,
    IN WDFREQUEST  Request,
    IN size_t       Length
    );

VOID
PLxEvtIoRead(
    IN WDFQUEUE    Queue,
    IN WDFREQUEST  Request,
    IN size_t       Length
    );

NTSTATUS
PLxInitializeDeviceExtension(
    IN PDEVICE_EXTENSION DevExt
    );


NTSTATUS
PLxPrepareHardware(
    IN PDEVICE_EXTENSION DevExt,
    IN WDFCMRESLIST     ResourcesTranslated
    );

NTSTATUS
PLxInitRead(
    IN PDEVICE_EXTENSION DevExt
    );

NTSTATUS
PLxInitWrite(
    IN PDEVICE_EXTENSION DevExt
    );

//
// WDFINTERRUPT Support
//
NTSTATUS
PLxInterruptCreate(
    IN PDEVICE_EXTENSION DevExt
    );

BOOLEAN
PLxEvtInterruptIsr(
    IN WDFINTERRUPT Interrupt,
    IN ULONG        MessageID
    );

VOID
PLxEvtInterruptDpc(
    IN WDFINTERRUPT Interrupt,
    IN WDFOBJECT    Object
    );

NTSTATUS
PLxEvtInterruptEnable(
    IN WDFINTERRUPT Interrupt,
    IN WDFDEVICE    Device
    );

NTSTATUS
PLxEvtInterruptDisable(
    IN WDFINTERRUPT Interrupt,
    IN WDFDEVICE    Device
    );


VOID
PLxReadRequestComplete(
    IN WDFDMATRANSACTION  DmaTransaction,
    IN NTSTATUS           Status
    );

VOID
PLxWriteRequestComplete(
    IN WDFDMATRANSACTION  DmaTransaction,
    IN NTSTATUS           Status
    );


NTSTATUS
PLxInitializeHardware(
    IN PDEVICE_EXTENSION DevExt
    );

VOID
PLxShutdown(
    IN PDEVICE_EXTENSION DevExt
    );


BOOLEAN
PLxEvtProgramWriteDma(
    IN  WDFDMATRANSACTION       Transaction,
    IN  WDFDEVICE               Device,
    IN  PVOID                   Context,
    IN  WDF_DMA_DIRECTION       Direction,
    IN  PSCATTER_GATHER_LIST    SgList
    );

BOOLEAN
PLxEvtProgramReadDma(
    IN  WDFDMATRANSACTION       Transaction,
    IN  WDFDEVICE               Device,
    IN  PVOID                   Context,
    IN  WDF_DMA_DIRECTION       Direction,
    IN  PSCATTER_GATHER_LIST    SgList
    );

VOID
PLxHardwareReset(
    IN PDEVICE_EXTENSION    DevExt
    );

NTSTATUS
PLxInitializeDMA(
    IN PDEVICE_EXTENSION DevExt
    );

VOID
PlxEvtDriverContextCleanup(
    IN WDFDRIVER Driver
    );

#pragma warning(disable:4127) // avoid conditional expression is constant error with W4

#endif  // _PCI9656_H_


