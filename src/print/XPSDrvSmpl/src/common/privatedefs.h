/*++

Copyright (c) 2005 Microsoft Corporation

All rights reserved.

THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
PARTICULAR PURPOSE.

File Name:

   privatedefs.h

Abstract:

   Definitions for private PrintTicket/PrintCapabilities properties for watermark and page scaling features.

--*/

#pragma once

//
// Macro to return the number of elements in an array
//
#define numof(n) (sizeof(n) / sizeof(n[0]))

//
// Structure definition for the Private Parameter Defs (Integer Type)
//
typedef struct _PRIVATE_DEF_INTS
{
    PCSTR property_name;
    PCSTR display_name;
    INT   default_value;
    INT   min_length;
    INT   max_length;
    INT   multiple;
    PCSTR unit_type;
}PRIVATE_DEF_INTEGERS;

//
// Structure definition for the Private Parameter Defs (String Type)
//
typedef struct _PRIVATE_DEF_STRS
{
    PCSTR property_name;
    PCSTR display_name;
    PCSTR default_value;
    INT   min_length;
    INT   max_length;
    PCSTR unit_type;
}PRIVATE_DEF_STRINGS;

//
// Structure definition for the Private Parameter Defs (Decimal Type)
//
typedef struct _PRIVATE_DEF_DECS
{
    PCSTR property_name;
    PCSTR display_name;
    REAL  default_value;
    REAL  min_length;
    REAL  max_length;
    REAL  multiple;
    PCSTR unit_type;
}PRIVATE_DEF_DECIMALS;

//
// Table of values for private integer watermark properties.
//
CONST PRIVATE_DEF_INTEGERS wmParamDefIntegers[] =
{
    {
        "PageWatermarkTextAngle",
        NULL,
        0,
        0,
        360,
        1,
        "Degrees"
    },
    {
        "PageWatermarkOriginWidth",
        NULL,
        0,
        -INT_MAX,
        INT_MAX,
        1,
        "Microns"
    },
    {
        "PageWatermarkOriginHeight",
        NULL,
        HUNDREDTH_OFINCH_TO_MICRON(100),
        -INT_MAX,
        INT_MAX,
        1,
        "Microns"
    },
    {
        "PageWatermarkSizeWidth",
        NULL,
        HUNDREDTH_OFINCH_TO_MICRON(100),
        0,
        INT_MAX,
        1,
        "Microns"
    },
    {
        "PageWatermarkSizeHeight",
        NULL,
        HUNDREDTH_OFINCH_TO_MICRON(100),
        0,
        INT_MAX,
        1,
        "Microns"
    },
    {
        "PageWatermarkTextFontSize",
        NULL,
        72,
        1,
        INT_MAX,
        1,
        "Points Per Inch"
    },
    {
        "PageWatermarkTextColor",
        NULL,
        0xFFFF0000,
        0,
        UINT_MAX,
        1,
        "sRGB"
    },
    {
        "PageWatermarkTransparency",
        NULL,
        50,
        0,
        100,
        1,
        "Transparency"
    },
};

//
// enum lookup into above table
//
enum eWMParamDefIntegers
{
    ePageWatermarkAngle = 0,
    ePageWatermarkOriginWidth,
    ePageWatermarkOriginHeight,
    ePageWatermarkSizeWidth,
    ePageWatermarkSizeHeight,
    ePageWatermarkTextFontSize,
    ePageWatermarkTextColor,
    ePageWatermarkTransparency,
};

//
// Table of values for private string watermark properties.
//
CONST PRIVATE_DEF_STRINGS wmParamDefStrings[] =
{
    {
        "PageWatermarkTextText",
        NULL,
        "CONFIDENTIAL",
        0,
        20,
        "Characters"
    }
};

//
// enum lookup into above table
//
enum eWMParamDefStrings
{
    ePageWatermarkTextText = 0
};

//
// Table of values for private integer page scaling properties.
//
CONST PRIVATE_DEF_INTEGERS pgscParamDefIntegers[] =
{
    {
        "PageScalingScaleWidth",
        NULL,
        100,
        1,
        1000,
        1,
        "Percent"
    },
    {
        "PageScalingScaleHeight",
        NULL,
        100,
        1,
        1000,
        1,
        "Percent"
    },
    {
        "PageScalingScale",
        NULL,
        100,
        1,
        1000,
        1,
        "Percent"
    },
    {
        "PageScalingOffsetWidth",
        NULL,
        0,
        -INT_MAX,
        INT_MAX,
        1,
        "Microns"
    },
    {
        "PageScalingOffsetHeight",
        NULL,
        0,
        -INT_MAX,
        INT_MAX,
        1,
        "Microns"
    },
};

//
// enum lookup into above table
//
enum ePGSCParamDefIntegers
{
    ePageScalingScaleWidth = 0,
    ePageScalingScaleHeight,
    ePageScalingScale,
    ePageScalingOffsetWidth,
    ePageScalingOffsetHeight,
};

//
// Table of values for private integer booklet gutter properties.
//
CONST PRIVATE_DEF_INTEGERS bkParamDefIntegers[] =
{
    {
        "JobBindAllDocumentsGutter",
        NULL,
        0,
        -INT_MAX,
        INT_MAX,
        0,
        "Microns"
    },
    {
        "DocumentBindingGutter",
        NULL,
        0,
        -INT_MAX,
        INT_MAX,
        0,
        "Microns"
    }
};

//
// enum lookup into above table
//
enum eBindingParamDefIntegers
{
    eJobBindAllDocumentsGutter = 0,
    eDocumentBindingGutter,
};


