/*
	Copyright (c) Microsoft Corporation.  All rights reserved.
*/

#ifndef _SDV_NTDDK_H_
#define _SDV_NTDDK_H_

#include <sdv_wdm.h>


#ifndef _SDV_NTIFS_H_

#ifdef HalQuerySystemInformation
#undef HalQuerySystemInformation
#endif
#define HalQuerySystemInformation sdv_HalQuerySystemInformation
NTSTATUS
sdv_HalQuerySystemInformation(
    IN HAL_QUERY_INFORMATION_CLASS  InformationClass,
    IN ULONG  BufferSize,
    OUT PVOID  Buffer,
    OUT PULONG  ReturnedLength
    );

#endif

#endif
