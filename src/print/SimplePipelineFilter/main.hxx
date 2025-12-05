//+--------------------------------------------------------------------------
//
//  Copyright (c) Microsoft Corporation.  All rights reserved.
//
//  This source code is intended only as a supplement to Microsoft
//  Development Tools and/or on-line documentation.  See these other
//  materials for detailed information regarding Microsoft code samples.
//
//  THIS CODE AND INFORMATION ARE PROVIDED AS IS WITHOUT WARRANTY OF ANY
//  KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
//  WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
//
//  Abstract:
//     WDK print filter sample.
//  DllGetClassObject uses GlobalLock to lock the dll in memory after it creates
//  filters. The filters need to call GlobalUnlock in their DTOR.
//
//----------------------------------------------------------------------------

#ifndef _FILTER_MAIN_HXX_
#define _FILTER_MAIN_HXX_

LONG
GlobalLock(
    void
    );

LONG
GlobalUnlock(
    void
    );

#endif


