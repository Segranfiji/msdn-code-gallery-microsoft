/*++

// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//
// Copyright (c) Microsoft Corporation. All rights reserved


Module Name:

    SensorDDI.h

Abstract:

    This module contains the type definitions for the 
    ISensorDriver interface which is used by the Sensor Class Extension.
--*/

#pragma once

//#include "internal.h"

class CSensorManager; //forward declaration

class CSensorDDI : 
    public CComObjectRoot,
    public ISensorDriver
{
public:
    CSensorDDI();
    virtual ~CSensorDDI();

    DECLARE_NOT_AGGREGATABLE(CSensorDDI)

    BEGIN_COM_MAP(CSensorDDI)
        COM_INTERFACE_ENTRY(ISensorDriver)
    END_COM_MAP()

// Public method
public:
    HRESULT InitSensorDevice(_In_ IWDFDevice* pWdfDevice );
    HRESULT DeInitSensorDevice();
    HRESULT RequestDeviceInfo(
        _Inout_ SensorType* pSensType,
        //this SAL annotation required to be very specific about size of
        // buffer that is being passed in. With just a normal _In_out or
        //_Out_ annotion, these are seen as possible buffer overruns.
        _Inout_updates_(DESCRIPTOR_MAX_LENGTH) LPWSTR pwszManufacturer,
        _Inout_updates_(DESCRIPTOR_MAX_LENGTH) LPWSTR pwszProduct,
        _Inout_updates_(DESCRIPTOR_MAX_LENGTH) LPWSTR pwszSerialNumber,
        _Inout_updates_(DESCRIPTOR_MAX_LENGTH) LPWSTR pwszDeviceID
        );

public:
    HRESULT UpdateSensorPropertyValues( 
        _In_ LPWSTR ObjectID,
        _In_ BOOL fSettableOnly 
        );

    HRESULT UpdateSensorDataFieldValues( 
        _In_ LPWSTR ObjectID
        );

    HRESULT CheckForSubscriber( 
        _In_ CSensor*  pSensor,
        _In_ IWDFFile* appID,
        _Out_ BOOL* pfClientIsSubscribed
        );

    HRESULT RemoveSubscriber( 
        _In_ CSensor*  pSensor,
        _In_ IWDFFile* appID 
        );

    inline HRESULT EnterProcessing(DWORD64 dwControlFlag);
    inline void    ExitProcessing(DWORD64 dwControlFlag);
//
// COM Interface methods
//
public:

    // ISensorDriver methods.
    HRESULT STDMETHODCALLTYPE OnGetSupportedSensorObjects(
        _Out_ IPortableDeviceValuesCollection** ppSensorObjectCollection
        );

    HRESULT STDMETHODCALLTYPE OnGetSupportedProperties(
        _In_  LPWSTR pwszObjectID,
        _Out_ IPortableDeviceKeyCollection** ppSupportedProperties
        );

    HRESULT STDMETHODCALLTYPE OnGetSupportedDataFields(
        _In_  LPWSTR pwszObjectID,
        _Out_ IPortableDeviceKeyCollection** ppSupportedDataFields
        );

    HRESULT STDMETHODCALLTYPE OnGetSupportedEvents(
        _In_  LPWSTR pwszObjectID,
        _Out_ GUID** ppSupportedEvents,
        _Out_ ULONG* pulEventCount
        );

    HRESULT STDMETHODCALLTYPE OnGetProperties(
        _In_  IWDFFile* pClientFile,
        _In_  LPWSTR pwszObjectID,
        _In_  IPortableDeviceKeyCollection* pProperties,
        _Out_ IPortableDeviceValues** ppPropertyValues
        );

    HRESULT STDMETHODCALLTYPE OnGetDataFields(
        _In_  IWDFFile* pClientFile,
        _In_  LPWSTR pwszObjectID,
        _In_  IPortableDeviceKeyCollection* pDataFields,
        _Out_ IPortableDeviceValues** ppDataValues
        );

    HRESULT STDMETHODCALLTYPE OnSetPropertiesOriginal(
        _In_  IWDFFile* pClientFile,
        _In_  LPWSTR pwszObjectID,
        _In_  IPortableDeviceValues* pPropertiesToSet,
        _Out_ IPortableDeviceValues** ppResults
        );

    HRESULT STDMETHODCALLTYPE OnSetProperties(
        _In_  IWDFFile* pClientFile,
        _In_  LPWSTR pwszObjectID,
        _In_  IPortableDeviceValues* pPropertiesToSet,
        _Out_ IPortableDeviceValues** ppResults
        );

    HRESULT STDMETHODCALLTYPE OnClientConnect(
        _In_ IWDFFile* pClientFile,
        _In_ LPWSTR pwszObjectID
        );

    HRESULT STDMETHODCALLTYPE OnClientDisconnect(
        _In_ IWDFFile* pClientFile,
        _In_ LPWSTR pwszObjectID
        );

    HRESULT STDMETHODCALLTYPE OnClientSubscribeToEvents(
        _In_ IWDFFile* pClientFile,
        _In_ LPWSTR pwszObjectID
        );

    HRESULT STDMETHODCALLTYPE OnClientUnsubscribeFromEvents(
        _In_ IWDFFile* pClientFile,
        _In_ LPWSTR pwszObjectID
        );

    HRESULT STDMETHODCALLTYPE OnProcessWpdMessage(
        _In_ IUnknown* pUnkPortableDeviceValuesParams,
        _In_ IUnknown* pUnkPortableDeviceValuesResults
        );

    // Radio Management IO functions
    HRESULT OnGetRadioState(
        _In_ IWDFIoRequest* pRequest,
        _In_ bool fPreviousState
        );

    HRESULT OnSetRadioState(
        _In_ IWDFIoRequest* pRequest,
        _In_ bool fPreviousState
        );

// Private member functions
private:
    HRESULT OnGetPropertyValues(
        _In_ IWDFFile* appId,
        _In_ LPWSTR  wszObjectID,
        _In_ IPortableDeviceKeyCollection* pKeys,
        _Out_ IPortableDeviceValues** ppPortableDeviceValues
        );

    HRESULT FindClientFromAppID(
        _In_ CSensor*  pSensor,
        _In_ IWDFFile* appID, 
        _Out_ ULONG* pClientReportInterval, 
        _Out_ ULONG* pClientLocationDesiredAccuracy, 
        // this SAL annotation required to be very specific about size of 
        // buffer that is being passed in. With just a normal _Inout_ or 
        //_Out_ annotion, these are seen as possible buffer overruns
        _Inout_updates_(MAX_NUM_DATA_FIELDS) FLOAT* pClientChangeSensitivities
        );

    HRESULT CheckForClient( 
        _In_ CSensor*  pSensor,
        _In_ IWDFFile* appID,
        _Out_ BOOL* pfClientPresent
        );

    HRESULT RemoveClient( 
        _In_ CSensor*  pSensor,
        _In_ IWDFFile* appID 
        );

    VOID AddDevicePropertyKeys(
        _In_ IPortableDeviceKeyCollection* pKeys
        );

    VOID AddCommonPropertyKeys(
        _In_ IPortableDeviceKeyCollection* pKeys
        );

    HRESULT AddSensorPropertyKeys(
        _In_ SensorType sensType, 
        _In_ DWORD sensIndex, 
        _In_ IPortableDeviceKeyCollection* pKeys
        );

    HRESULT AddSupportedPropertyKeys(
        _In_ LPWSTR wszObjectID, 
        _In_ IPortableDeviceKeyCollection* pKeys
        );

    HRESULT AddSensorDataFieldKeys(
        _In_ SensorType sensType, 
        _In_ DWORD sensIndex, 
        _In_ IPortableDeviceKeyCollection* pKeys
        );

    HRESULT AddSupportedDataFieldKeys(
        _In_ LPWSTR wszObjectID, 
        _In_ IPortableDeviceKeyCollection* pKeys
        );

    HRESULT CopyPropertyKeys(
        _In_  IPortableDeviceKeyCollection *pSourceKeys,
        _In_ IPortableDeviceKeyCollection *pTargetKeys
        );
        
    BOOL FindPropertyKey(
        _In_ PROPERTYKEY Key, 
        _In_ IPortableDeviceKeyCollection *pKeys
        );

    BOOL IsSettablePropertyKey(
        _In_ DWORD sIndex, 
        _In_ PROPERTYKEY Key
        );

    HRESULT FindSensorTypeFromObjectID(
        _In_ LPWSTR pwszObjectID, 
        _Out_ SensorType* pSensorType, 
        _Out_ DWORD* pSensorIdx
        );

    HRESULT SelectClientReportInterval(
        _In_ CSensor* pSensor
        );

    HRESULT SelectClientLocationDesiredAccuracy(
        _In_ CSensor* pSensor
        );

    HRESULT SelectClientChangeSensitivity(
        _In_ CSensor* pSensor
        );

    HRESULT GetSensorObject(
        _In_ ULONG ulIndex,
        _Out_ CSensor** ppSensor
        );

    HRESULT GetGeolocationSensorObject(
        _Out_ CSensor** ppSensor
        );

    friend class CSensorManager;

public:
    CSensorManager*                    m_pSensorManager;

// Private data members.
private:
    LONG                               m_nNotifyOnSensorUpdate;

    CComAutoCriticalSection            m_CriticalSection; // This is used to make all calls to polled data requests thread safe
    CComAutoCriticalSection            m_CriticalSectionConnectDisconnect; // This is used to make all connect/disconnect calls thread safe
};
