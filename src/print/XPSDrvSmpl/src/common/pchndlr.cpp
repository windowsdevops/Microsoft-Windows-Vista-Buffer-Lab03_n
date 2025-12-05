/*++

Copyright (c) 2005 Microsoft Corporation

All rights reserved.

File Name:

    pchndlr.cpp

Abstract:

    Base PrintCapabilities handler class implementation. This class provides common
    PrintCapabilities handling functionality for any feature that requires
    PrintCapabilities handling. A feature specific handler can derive from
    this class to get feature unspecific XML handling functionality.

--*/


//
// Note on handling missing DOM nodes:
//
// Convert MSXML's S_FALSE to E_ELEMENT_NOT_FOUND. This allows clients to
// use the SUCCEEDED macro more effectively.
//
// E_ELEMENT_NOT_FOUND should not be propogated as an error to the
// filter pipeline or config module - treat as though the requested feature
// has not been enabled.
//

#include "precomp.h"
#include "debug.h"
#include "xdstring.h"
#include "pchndlr.h"

using XDPrintSchema::PRINTCAPABILITIES_NAME;
using XDPrintSchema::PARAM_DEF_ELEMENT_NAME;
using XDPrintSchema::NAME_ATTRIBUTE_NAME;
using XDPrintSchema::SCHEMA_CONDITIONAL;
using XDPrintSchema::SCHEMA_INTEGER;
using XDPrintSchema::DATATYPE_VALUE_NAME;
using XDPrintSchema::SCHEMA_QNAME;
using XDPrintSchema::DEFAULTVAL_VALUE_NAME;
using XDPrintSchema::SCHEMA_STRING;
using XDPrintSchema::MAX_VALUE_NAME;
using XDPrintSchema::MIN_VALUE_NAME;
using XDPrintSchema::MAX_LENGTH_NAME;
using XDPrintSchema::MIN_LENGTH_NAME;
using XDPrintSchema::MANDATORY_VALUE_NAME;
using XDPrintSchema::UNITTYPE_VALUE_NAME;
using XDPrintSchema::MULTIPLE_VALUE_NAME;
using XDPrintSchema::DISPLAYNAME_VALUE_NAME;
using XDPrintSchema::SCHEMA_DECIMAL;
using XDPrintSchema::FEATURE_ELEMENT_NAME;
using XDPrintSchema::PICKONE_VALUE_NAME;
using XDPrintSchema::SELECTIONTYPE_VALUE_NAME;
using XDPrintSchema::OPTION_ELEMENT_NAME;
using XDPrintSchema::PARAM_REF_ELEMENT_NAME;
using XDPrintSchema::FRAMEWORK_URI;
using XDPrintSchema::KEYWORDS_URI;

/*++

Routine Name:

    CPCHandler::CPCHandler

Routine Description:

    CPCHandler class constructor

Arguments:

    pPrintCapabilities - Pointer to the DOM document representation of the PrintCapabilities

Return Value:

    None

    Note: Base Class (CPSHandler) - Throws CXDException(HRESULT) on an error

--*/
CPCHandler::CPCHandler(
    __in IXMLDOMDocument2 *pDOMDocument
    ) :
    CPSHandler(CComBSTR(PRINTCAPABILITIES_NAME), pDOMDocument)
{
}

/*++

Routine Name:

    CPCHandler::~CPCHandler

Routine Description:

    CPCHandler class destructor

Arguments:

    None

Return Value:

    None

--*/
CPCHandler::~CPCHandler()
{
}

/*++

Routine Name:

    CPCHandler::CreateParameterDef

Routine Description:

    Creates a Integer type ParameterDef Element.

Arguments:

    bstrParamName - Keyword value name for the parameter
    bstrDisplayName - Optional string containing a description of the parameter definition.
    defaultValue - Default value of the parameter.
    minLength - Minimium valid value.
    maxLength - Maximium valid value.
    multiple - value can be increased or decreased in multiples of.
    bstrUnitType - String containing a description of the unit type of the parameter value.
    ppParameterDef - Pointer to an IXMLDOMElement that recieves the new element.

Return Value:

    HRESULT
    S_OK - On success
    E_*  - On error

--*/
HRESULT
CPCHandler::CreateParameterDef(
    __in_z      CONST BSTR       bstrParamName,
    __in_z_opt  CONST BSTR       bstrDisplayName,
    __in        CONST INT        defaultValue,
    __in        CONST INT        minLength,
    __in        CONST INT        maxLength,
    __in        CONST INT        multiple,
    __in_z      CONST BSTR       bstrUnitType,
    __deref_out IXMLDOMElement** ppParameterDef
    )
{
    HRESULT hr = S_OK;

    //
    // Create the parameterDef
    //
    CComBSTR bstrParameter(m_bstrFrameworkPrefix);
    bstrParameter += PARAM_DEF_ELEMENT_NAME;

    //
    // PageWatermarkSizeWidth and PageWatermarkSizeHeight are non-standard
    // parameter names so need to be accessed using a user defined namespace
    //
    CComBSTR bstrParameterAttrib;
    if (wcscmp(bstrParamName, L"PageWatermarkSizeWidth") == 0 ||
        wcscmp(bstrParamName, L"PageWatermarkSizeHeight") == 0)
    {
        bstrParameterAttrib.Append(m_bstrUserKeywordsPrefix);
    }
    else
    {
        bstrParameterAttrib.Append(m_bstrKeywordsPrefix);
    }

    bstrParameterAttrib += bstrParamName;

    if(SUCCEEDED(hr))
    {
        hr = CreateXMLElement(bstrParameter, FRAMEWORK_URI, ppParameterDef);
    }

    if(SUCCEEDED(hr))
    {
        if(SUCCEEDED(hr = CreateXMLAttribute(*ppParameterDef, NAME_ATTRIBUTE_NAME, NULL, bstrParameterAttrib )))
        {
            CComPtr<IXMLDOMElement> pDataTypeProp(NULL);
            CComPtr<IXMLDOMElement> pDefValProp(NULL);
            CComPtr<IXMLDOMElement> pMaxLengthProp(NULL);
            CComPtr<IXMLDOMElement> pMinLengthProp(NULL);
            CComPtr<IXMLDOMElement> pMandatoryProp(NULL);
            CComPtr<IXMLDOMElement> pUnitTypeProp(NULL);

            CComBSTR bstrMandatory(m_bstrKeywordsPrefix);
            bstrMandatory += SCHEMA_CONDITIONAL;

            CStringXDW cstrDefaultValue;
            cstrDefaultValue.Format(L"%i", defaultValue);

            CStringXDW cstrMaxLength;
            cstrMaxLength.Format(L"%i", maxLength);

            CStringXDW cstrMinLength;
            cstrMinLength.Format(L"%i", minLength);

            CComBSTR bstrIntegerType(m_bstrSchemaPrefix);
            bstrIntegerType += SCHEMA_INTEGER;

            if (SUCCEEDED(hr = CreateProperty(CComBSTR(DATATYPE_VALUE_NAME), CComBSTR(SCHEMA_QNAME), bstrIntegerType, &pDataTypeProp)) &&
                SUCCEEDED(hr = CreateProperty(CComBSTR(DEFAULTVAL_VALUE_NAME), CComBSTR(SCHEMA_STRING), CComBSTR(cstrDefaultValue), &pDefValProp)) &&
                SUCCEEDED(hr = CreateProperty(CComBSTR(MAX_VALUE_NAME), CComBSTR(SCHEMA_INTEGER), CComBSTR(cstrMaxLength), &pMaxLengthProp)) &&
                SUCCEEDED(hr = CreateProperty(CComBSTR(MIN_VALUE_NAME), CComBSTR(SCHEMA_INTEGER), CComBSTR(cstrMinLength), &pMinLengthProp)) &&
                SUCCEEDED(hr = CreateProperty(CComBSTR(MANDATORY_VALUE_NAME), CComBSTR(SCHEMA_QNAME), bstrMandatory, &pMandatoryProp)) &&
                SUCCEEDED(hr = CreateProperty(CComBSTR(UNITTYPE_VALUE_NAME), CComBSTR(SCHEMA_STRING), bstrUnitType, &pUnitTypeProp)) &&
                SUCCEEDED(hr = (*ppParameterDef)->appendChild(pDataTypeProp, NULL)) &&
                SUCCEEDED(hr = (*ppParameterDef)->appendChild(pDefValProp, NULL)) &&
                SUCCEEDED(hr = (*ppParameterDef)->appendChild(pMaxLengthProp, NULL)) &&
                SUCCEEDED(hr = (*ppParameterDef)->appendChild(pMinLengthProp, NULL)) &&
                SUCCEEDED(hr = (*ppParameterDef)->appendChild(pMandatoryProp, NULL)) &&
                SUCCEEDED(hr = (*ppParameterDef)->appendChild(pUnitTypeProp, NULL)))
            {
                if (multiple > 0)
                {
                    CComPtr<IXMLDOMElement> pMultipleProp(NULL);
                    CStringXDW cstrMultiple;
                    cstrMultiple.Format(L"%i", multiple);

                    if (SUCCEEDED(hr = CreateProperty(CComBSTR(MULTIPLE_VALUE_NAME), CComBSTR(SCHEMA_INTEGER), CComBSTR(cstrMultiple), &pMultipleProp)))
                    {
                        hr = (*ppParameterDef)->appendChild(pMultipleProp, NULL);
                    }
                }

                if (SUCCEEDED(hr) &&
                    SysStringLen(bstrDisplayName) > 0)
                {
                    CComPtr<IXMLDOMElement> pDisplayProp(NULL);

                    if (SUCCEEDED(hr = CreateProperty(CComBSTR(DISPLAYNAME_VALUE_NAME), CComBSTR(SCHEMA_STRING), bstrDisplayName, &pDisplayProp)))
                    {
                        hr = (*ppParameterDef)->appendChild(pDisplayProp, NULL);
                    }
                }
            }
        }
    }

    ERR_ON_HR(hr);
    return hr;
}

/*++

Routine Name:

    CPCHandler::CreateParameterDef

Routine Description:

    Creates a String type ParameterDef Element.

Arguments:

    bstrParamName - Keyword value name for the parameter.
    bstrDisplayName - Optional string containing a description of the parameter definition.
    defaultValue - Default string.
    minLength - Minimium valid length of string.
    maxLength - Maximium valid length of string.
    bstrUnitType - String containing a description of the unit type of the parameter value.
    ppParameterDef - Pointer to an IXMLDOMElement that recieves the new element.

Return Value:

    HRESULT
    S_OK - On success
    E_*  - On error

--*/
HRESULT
CPCHandler::CreateParameterDef(
    __in_z      CONST BSTR       bstrParamName,
    __in_z_opt  CONST BSTR       bstrDisplayName,
    __in_z      CONST BSTR       bstrDefaultValue,
    __in        CONST INT        minLength,
    __in        CONST INT        maxLength,
    __in_z      CONST BSTR       bstrUnitType,
    __deref_out IXMLDOMElement** ppParameterDef
    )
{
    HRESULT hr = S_OK;

    //
    // Create the parameterDef
    //
    CComBSTR bstrParameter(m_bstrFrameworkPrefix);
    bstrParameter += PARAM_DEF_ELEMENT_NAME;

    CComBSTR bstrParameterAttrib(m_bstrKeywordsPrefix);
    bstrParameterAttrib += bstrParamName;

    if(SUCCEEDED(hr))
    {
        hr = CreateXMLElement(bstrParameter, FRAMEWORK_URI, ppParameterDef);
    }

    if(SUCCEEDED(hr))
    {
        if(SUCCEEDED(hr = CreateXMLAttribute(*ppParameterDef, NAME_ATTRIBUTE_NAME, NULL, bstrParameterAttrib )))
        {
            PTDOMElementVector      propertyList;

            CComPtr<IXMLDOMElement> pDataTypeProp(NULL);
            CComPtr<IXMLDOMElement> pDefValProp(NULL);
            CComPtr<IXMLDOMElement> pMaxLengthProp(NULL);
            CComPtr<IXMLDOMElement> pMinLengthProp(NULL);
            CComPtr<IXMLDOMElement> pMandatoryProp(NULL);
            CComPtr<IXMLDOMElement> pUnitTypeProp(NULL);

            CComBSTR bstrMandatory(m_bstrKeywordsPrefix);
            bstrMandatory += SCHEMA_CONDITIONAL;

            CStringXDW cstrMaxLength;
            cstrMaxLength.Format(L"%i", maxLength);

            CStringXDW cstrMinLength;
            cstrMinLength.Format(L"%i", minLength);

            CComBSTR bstrStringType(m_bstrSchemaPrefix);
            bstrStringType += SCHEMA_STRING;

            if (SUCCEEDED(hr = CreateProperty(CComBSTR(DATATYPE_VALUE_NAME), CComBSTR(SCHEMA_QNAME), bstrStringType, &pDataTypeProp)) &&
                SUCCEEDED(hr = CreateProperty(CComBSTR(DEFAULTVAL_VALUE_NAME), CComBSTR(SCHEMA_STRING), bstrDefaultValue, &pDefValProp)) &&
                SUCCEEDED(hr = CreateProperty(CComBSTR(MAX_LENGTH_NAME), CComBSTR(SCHEMA_INTEGER), CComBSTR(cstrMaxLength), &pMaxLengthProp)) &&
                SUCCEEDED(hr = CreateProperty(CComBSTR(MIN_LENGTH_NAME), CComBSTR(SCHEMA_INTEGER), CComBSTR(cstrMinLength), &pMinLengthProp)) &&
                SUCCEEDED(hr = CreateProperty(CComBSTR(MANDATORY_VALUE_NAME), CComBSTR(SCHEMA_QNAME), bstrMandatory, &pMandatoryProp)) &&
                SUCCEEDED(hr = CreateProperty(CComBSTR(UNITTYPE_VALUE_NAME), CComBSTR(SCHEMA_STRING), bstrUnitType, &pUnitTypeProp)) &&
                SUCCEEDED(hr = (*ppParameterDef)->appendChild(pDataTypeProp, NULL)) &&
                SUCCEEDED(hr = (*ppParameterDef)->appendChild(pDefValProp, NULL)) &&
                SUCCEEDED(hr = (*ppParameterDef)->appendChild(pMaxLengthProp, NULL)) &&
                SUCCEEDED(hr = (*ppParameterDef)->appendChild(pMinLengthProp, NULL)) &&
                SUCCEEDED(hr = (*ppParameterDef)->appendChild(pMandatoryProp, NULL)) &&
                SUCCEEDED(hr = (*ppParameterDef)->appendChild(pUnitTypeProp, NULL)))
            {
                if (SysStringLen(bstrDisplayName) > 0)
                {
                    CComPtr<IXMLDOMElement> pDisplayProp(NULL);

                    if (SUCCEEDED(hr = CreateProperty(CComBSTR(DISPLAYNAME_VALUE_NAME), CComBSTR(SCHEMA_STRING), bstrDisplayName, &pDisplayProp)))
                    {
                        hr = (*ppParameterDef)->appendChild(pDisplayProp, NULL);
                    }
                }
            }
        }
    }

    ERR_ON_HR(hr);
    return hr;
}


/*++

Routine Name:

    CPCHandler::CreateParameterDef

Routine Description:

    Creates a Real type ParameterDef Element.

Arguments:

    bstrParamName - Keyword value name for the parameter.
    bstrDisplayName - Optional string containing a description of the parameter definition.
    defaultValue - Default value of the parameter.
    minLength - Minimium valid value.
    maxLength - Maximium valid value.
    multiple - value can be increased or decreased in multiples of.
    bstrUnitType - String containing a description of the unit type of the parameter value.
    ppParameterDef - Pointer to an IXMLDOMElement that recieves the new element.

Return Value:

    HRESULT
    S_OK - On success
    E_*  - On error

--*/
HRESULT
CPCHandler::CreateParameterDef(
    __in_z      CONST BSTR       bstrParamName,
    __in_z_opt  CONST BSTR       bstrDisplayName,
    __in        CONST REAL       defaultValue,
    __in        CONST REAL       minLength,
    __in        CONST REAL       maxLength,
    __in        CONST REAL       multiple,
    __in_z      CONST BSTR       bstrUnitType,
    __deref_out IXMLDOMElement** ppParameterDef
    )
{
    HRESULT hr = S_OK;

    //
    // Create the parameterDef
    //
    CComBSTR bstrParameter(m_bstrFrameworkPrefix);
    bstrParameter += PARAM_DEF_ELEMENT_NAME;

    CComBSTR bstrParameterAttrib(m_bstrKeywordsPrefix);
    bstrParameterAttrib += bstrParamName;

    if(SUCCEEDED(hr))
    {
        hr = CreateXMLElement(bstrParameter, FRAMEWORK_URI, ppParameterDef);
    }

    if(SUCCEEDED(hr))
    {
        if(SUCCEEDED(hr = CreateXMLAttribute(*ppParameterDef, NAME_ATTRIBUTE_NAME, NULL, bstrParameterAttrib )))
        {
            CComPtr<IXMLDOMElement> pDataTypeProp(NULL);
            CComPtr<IXMLDOMElement> pDefValProp(NULL);
            CComPtr<IXMLDOMElement> pMaxLengthProp(NULL);
            CComPtr<IXMLDOMElement> pMinLengthProp(NULL);
            CComPtr<IXMLDOMElement> pMandatoryProp(NULL);
            CComPtr<IXMLDOMElement> pUnitTypeProp(NULL);

            CComBSTR bstrMandatory(m_bstrKeywordsPrefix);
            bstrMandatory += SCHEMA_CONDITIONAL;

            CStringXDW cstrDefaultValue;
            cstrDefaultValue.Format(L"%.2f", defaultValue);

            CStringXDW cstrMaxLength;
            cstrMaxLength.Format(L"%i", maxLength);
            cstrMaxLength.Format(L"%.2f", maxLength);

            CStringXDW cstrMinLength;
            cstrMinLength.Format(L"%i", minLength);
            cstrMinLength.Format(L"%.2f", minLength);

            CComBSTR bstrDecimalType(m_bstrSchemaPrefix);
            bstrDecimalType += SCHEMA_DECIMAL;

            if (SUCCEEDED(hr = CreateProperty(CComBSTR(DATATYPE_VALUE_NAME), CComBSTR(SCHEMA_QNAME), bstrDecimalType, &pDataTypeProp)) &&
                SUCCEEDED(hr = CreateProperty(CComBSTR(DEFAULTVAL_VALUE_NAME), CComBSTR(SCHEMA_STRING), CComBSTR(cstrDefaultValue), &pDefValProp)) &&
                SUCCEEDED(hr = CreateProperty(CComBSTR(MAX_VALUE_NAME), CComBSTR(SCHEMA_DECIMAL), CComBSTR(cstrMaxLength), &pMaxLengthProp)) &&
                SUCCEEDED(hr = CreateProperty(CComBSTR(MIN_VALUE_NAME), CComBSTR(SCHEMA_DECIMAL), CComBSTR(cstrMinLength), &pMinLengthProp)) &&
                SUCCEEDED(hr = CreateProperty(CComBSTR(MANDATORY_VALUE_NAME), CComBSTR(SCHEMA_QNAME), bstrMandatory, &pMandatoryProp)) &&
                SUCCEEDED(hr = CreateProperty(CComBSTR(UNITTYPE_VALUE_NAME), CComBSTR(SCHEMA_STRING), bstrUnitType, &pUnitTypeProp)) &&
                SUCCEEDED(hr = (*ppParameterDef)->appendChild(pDataTypeProp, NULL)) &&
                SUCCEEDED(hr = (*ppParameterDef)->appendChild(pDefValProp, NULL)) &&
                SUCCEEDED(hr = (*ppParameterDef)->appendChild(pMaxLengthProp, NULL)) &&
                SUCCEEDED(hr = (*ppParameterDef)->appendChild(pMinLengthProp, NULL)) &&
                SUCCEEDED(hr = (*ppParameterDef)->appendChild(pMandatoryProp, NULL)) &&
                SUCCEEDED(hr = (*ppParameterDef)->appendChild(pUnitTypeProp, NULL)))
            {
                if (multiple > 0)
                {
                    CComPtr<IXMLDOMElement> pMultipleProp(NULL);
                    CStringXDW cstrMultiple;
                    cstrMultiple.Format(L"%.2f", multiple);

                    if (SUCCEEDED(hr = CreateProperty(CComBSTR(MULTIPLE_VALUE_NAME), CComBSTR(SCHEMA_DECIMAL), CComBSTR(cstrMultiple), &pMultipleProp)))
                    {
                        hr = (*ppParameterDef)->appendChild(pMultipleProp, NULL);
                    }
                }

                if (SUCCEEDED(hr) &&
                    SysStringLen(bstrDisplayName) > 0)
                {
                    CComPtr<IXMLDOMElement> pDisplayProp(NULL);

                    if (SUCCEEDED(hr = CreateProperty(CComBSTR(DISPLAYNAME_VALUE_NAME), CComBSTR(SCHEMA_STRING), bstrDisplayName, &pDisplayProp)))
                    {
                        hr = (*ppParameterDef)->appendChild(pDisplayProp, NULL);
                    }
                }
            }
        }
    }

    ERR_ON_HR(hr);
    return hr;
}

/*++

Routine Name:

    CPCHandler::CreateFeature

Routine Description:

    Creates a Feature Element.

Arguments:

    bstrFeatureName - Keyword value name for the feature.
    bstrDisplayName - Optional string containing a description of the feature.
    ppFeatureElement - Pointer to an IXMLDOMElement that recieves the new element.

Return Value:

    HRESULT
    S_OK - On success
    E_*  - On error

--*/
HRESULT
CPCHandler::CreateFeature(
    __in_z      CONST BSTR bstrFeatureName,
    __in_z_opt  CONST BSTR bstrDisplayName,
    __deref_out IXMLDOMElement** ppFeatureElement
    )
{
    HRESULT hr = S_OK;

    //
    // Create the base feature node
    //
    CComBSTR bstrFeature(m_bstrFrameworkPrefix);
    bstrFeature += FEATURE_ELEMENT_NAME;

    CComBSTR bstrFeatureAttrib(m_bstrKeywordsPrefix);
    bstrFeatureAttrib += bstrFeatureName;

    if(SUCCEEDED(hr))
    {
        hr = CreateXMLElement(bstrFeature, FRAMEWORK_URI, ppFeatureElement);
    }
    
    if (SUCCEEDED(hr))
    {
        hr = CreateXMLAttribute(*ppFeatureElement, NAME_ATTRIBUTE_NAME, NULL, bstrFeatureAttrib );
    }

    //
    // Append the Display Name property type
    //
    if (SUCCEEDED(hr) &&
        SysStringLen(bstrDisplayName) > 0)
    {
        CComPtr<IXMLDOMElement> pDisplayProp(NULL);

        if (SUCCEEDED(hr = CreateProperty(CComBSTR(DISPLAYNAME_VALUE_NAME), CComBSTR(SCHEMA_STRING), bstrDisplayName, &pDisplayProp)))
        {
            hr = (*ppFeatureElement)->appendChild(pDisplayProp, NULL);
        }
    }

    ERR_ON_HR(hr);
    return hr;
}

/*++

Routine Name:

    CPCHandler::CreateFeatureSelection

Routine Description:

    Creates a Selection Feature Element.

Arguments:

    bstrFeatureName - Keyword value name for the feature.
    bstrDisplayName - Optional string containing a description of the feature.
    ppFeatureElement - Pointer to an IXMLDOMElement that recieves the new element.

Return Value:

    HRESULT
    S_OK - On success
    E_*  - On error

--*/
HRESULT
CPCHandler::CreateFeatureSelection(
    __in_z      CONST BSTR bstrFeatureName,
    __in_z_opt  CONST BSTR bstrDisplayName,
    __deref_out IXMLDOMElement** ppFeatureElement
    )
{
    HRESULT hr = S_OK;

    if (SUCCEEDED(hr = CreateFeature(bstrFeatureName, bstrDisplayName, ppFeatureElement)))
    {
        //
        // Append the property type
        //
        CComPtr<IXMLDOMElement> pPropertyTypeElement(NULL);

        //
        // Create the keyname
        //
        CComBSTR bstrPickOne(m_bstrKeywordsPrefix);
        bstrPickOne += PICKONE_VALUE_NAME;

        if (SUCCEEDED(hr = CreateFWProperty(CComBSTR(SELECTIONTYPE_VALUE_NAME), CComBSTR(SCHEMA_QNAME), bstrPickOne, &pPropertyTypeElement)))
        {
            hr = (*ppFeatureElement)->appendChild(pPropertyTypeElement, NULL);
        }
    }

    ERR_ON_HR(hr);
    return hr;
}

/*++

Routine Name:

    CPCHandler::CreateOption

Routine Description:

    Creates an Option Type Element.

Arguments:

    bstrOptionName - Keyword value name for the option.
    bstrDisplayName - Optional string containing a description of the feature.
    ppOptionElement - Pointer to an IXMLDOMElement that recieves the new element.

Return Value:

    HRESULT
    S_OK - On success
    E_*  - On error

--*/
HRESULT
CPCHandler::CreateOption(
    __in_z      CONST BSTR bstrOptionName,
    __in_z_opt  CONST BSTR bstrDisplayName,
    __deref_out IXMLDOMElement** ppOptionElement
    )
{
    HRESULT hr = S_OK;

    //
    // Create the base feature node
    //
    CComBSTR bstrOption(m_bstrFrameworkPrefix);
    bstrOption += OPTION_ELEMENT_NAME;

    CComBSTR bstrOptionAttrib(m_bstrKeywordsPrefix);
    bstrOptionAttrib += bstrOptionName;

    if(SUCCEEDED(hr))
    {
        hr = CreateXMLElement(bstrOption, FRAMEWORK_URI, ppOptionElement);
    }

    if (SUCCEEDED(hr))
    {
        hr = CreateXMLAttribute(*ppOptionElement, NAME_ATTRIBUTE_NAME, NULL, bstrOptionAttrib );
    }

    //
    // Append the Display Name property type
    //
    if (SUCCEEDED(hr) &&
        SysStringLen(bstrDisplayName) > 0)
    {
        CComPtr<IXMLDOMElement> pDisplayProp(NULL);

        if (SUCCEEDED(hr = CreateProperty(CComBSTR(DISPLAYNAME_VALUE_NAME), CComBSTR(SCHEMA_STRING), bstrDisplayName, &pDisplayProp)))
        {
            hr = (*ppOptionElement)->appendChild(pDisplayProp, NULL);
        }
    }

    ERR_ON_HR(hr);
    return hr;
}

/*++

Routine Name:

    CPCHandler::CreateParameterRef

Routine Description:

    Creates a Reference to a ParameterDef Element.

Arguments:

    bstrParamRefName - Keyword value name for the parameter reference.
    ppParamRefElement - Pointer to an IXMLDOMElement that recieves the new element.

Return Value:

    HRESULT
    S_OK - On success
    E_*  - On error

--*/
HRESULT
CPCHandler::CreateParameterRef(
    __in_z      CONST BSTR bstrParamRefName,
    __deref_out IXMLDOMElement** ppParamRefElement
    )
{
    HRESULT hr = S_OK;

    //
    // Create the base feature node
    //
    CComBSTR bstrParameterRef(m_bstrFrameworkPrefix);
    bstrParameterRef += PARAM_REF_ELEMENT_NAME;


    //
    // PageWatermarkSizeWidth and PageWatermarkSizeHeight are non-standard
    // parameter names so need to be accessed using a user defined namespace
    //
    CComBSTR bstrParamRefAttrib;
    if (wcscmp(bstrParamRefName, L"PageWatermarkSizeWidth") == 0 ||
        wcscmp(bstrParamRefName, L"PageWatermarkSizeHeight") == 0)
    {
        bstrParamRefAttrib.Append(m_bstrUserKeywordsPrefix);
    }
    else
    {
        bstrParamRefAttrib.Append(m_bstrKeywordsPrefix);
    }

    bstrParamRefAttrib += bstrParamRefName;

    //
    //Create the IXMLDOMNode for ParameterRef
    //
    CComPtr<IXMLDOMNode> pParameterRefNode(NULL);

    if(SUCCEEDED(hr))
    {
        hr = CreateXMLElement(bstrParameterRef, FRAMEWORK_URI, ppParamRefElement);
    }

    if (SUCCEEDED(hr))
    {
        hr = CreateXMLAttribute(*ppParamRefElement, NAME_ATTRIBUTE_NAME, NULL, bstrParamRefAttrib );
    }

    ERR_ON_HR(hr);
    return hr;
}


