#ifndef WINSTYLUSHANDLER_H
#define WINSTYLUSHANDLER_H

#include <RTSCOM.h>

#include <memory>
class Application;

class WinStylusHandler : public IStylusAsyncPlugin
{
private:
	bool stylusInRange = false;
	bool stylusVars_isDirty = true;
	TABLET_CONTEXT_ID tabletID = 0;
	STYLUS_ID stylusID = 0;
	bool isCursorInverted = false;
	float scaleX = 0.0f, scaleY = 0.0f;
	float tabletDPIfactor = 0.0f;
	ULONG propertiesNumber = 0;
	UINT32 propIndex_x = 0;
	LONG tabResolutionX_min = 0;
	LONG tabResolutionX_max = 0;
	UINT32 propIndex_y = 0;
	LONG tabResolutionY_min = 0;
	LONG tabResolutionY_max = 0;
	UINT32 propIndex_pressure = 0;
	float stylusPressure_min = 0.0f;
	float stylusPressure_max = 0.0f;
	UINT32 propIndex_xTilt = 0;
	float stylusTiltX_min = 0.0f;
	float stylusTiltX_max = 0.0f;
	UINT32 propIndex_yTilt = 0;
	float stylusTiltY_min = 0.0f;
	float stylusTiltY_max = 0.0f;
	UINT32 propIndex_rotation = 0;
	float stylusRotation_min = 0.0f;
	float stylusRotation_max = 0.0f;
	GUID* propertyGUIDs = NULL;
	PACKET_PROPERTY* packetProperties = NULL;

	//
	int scaleSetCount = 0;
	int scaleCountMax = 2500;
protected:
public:
	// Public Variables
	LONG refCount = 0;			// COM object reference count
	IUnknown* FTMarshaler = nullptr;	// free-threaded marshaller
	// Scaling Factor
	double downscaleFactor = 26.4;
	//Application* owner;
	std::shared_ptr<Application> owner = nullptr;

	// Constructor / Destructor
	WinStylusHandler() : refCount(1), FTMarshaler(NULL) { }
	virtual ~WinStylusHandler()
	{
		if (FTMarshaler != NULL)
			FTMarshaler->Release();
	}
	// Used notify events
	STDMETHOD(DataInterest)(RealTimeStylusDataInterest* pEventInterest);
	STDMETHOD(Packets)(IRealTimeStylus* pStylus, const StylusInfo* pStylusInfo, ULONG nPackets, ULONG nPacketBuf, LONG* pPackets, ULONG* nOutPackets, LONG** ppOutPackets);
	STDMETHOD(InAirPackets)(IRealTimeStylus* pStylus, const StylusInfo* pStylusInfo, ULONG nPackets, ULONG nPacketBuf, LONG* pPackets, ULONG* nOutPackets, LONG** ppOutPackets);
	STDMETHOD(StylusDown)(IRealTimeStylus* pStylus, const StylusInfo* pStylusInfo, ULONG nPackets, LONG* _pPackets, LONG** ppOutPackets);
	STDMETHOD(StylusUp)(IRealTimeStylus* pStylus, const StylusInfo* pStylusInfo, ULONG nPackets, LONG* _pPackets, LONG** ppOutPackets);
	STDMETHOD(StylusInRange)(IRealTimeStylus* pStylus, TABLET_CONTEXT_ID tcid, STYLUS_ID sid);
	STDMETHOD(StylusOutOfRange)(IRealTimeStylus* pStylus, TABLET_CONTEXT_ID tcid, STYLUS_ID sid);
	// Unused notify events
	STDMETHOD(RealTimeStylusEnabled)(IRealTimeStylus*, ULONG, const TABLET_CONTEXT_ID*) { return S_OK; }
	STDMETHOD(RealTimeStylusDisabled)(IRealTimeStylus*, ULONG, const TABLET_CONTEXT_ID*) { return S_OK; }
	STDMETHOD(StylusButtonUp)(IRealTimeStylus*, STYLUS_ID, const GUID*, POINT*) { return S_OK; }
	STDMETHOD(StylusButtonDown)(IRealTimeStylus*, STYLUS_ID, const GUID*, POINT*) { return S_OK; }
	STDMETHOD(SystemEvent)(IRealTimeStylus*, TABLET_CONTEXT_ID, STYLUS_ID, SYSTEM_EVENT, SYSTEM_EVENT_DATA) { return S_OK; }
	STDMETHOD(TabletAdded)(IRealTimeStylus*, IInkTablet*) { return S_OK; }
	STDMETHOD(TabletRemoved)(IRealTimeStylus*, LONG) { return S_OK; }
	STDMETHOD(CustomStylusDataAdded)(IRealTimeStylus*, const GUID*, ULONG, const BYTE*) { return S_OK; }
	STDMETHOD(Error)(IRealTimeStylus* piRtsSrc, IStylusPlugin* piPlugin, RealTimeStylusDataInterest dataInterest, HRESULT hrErrorCode, LONG_PTR* lptrKey);
	STDMETHOD(UpdateMapping)(IRealTimeStylus*) { return S_OK; }

	// Iunknown Methods
	STDMETHOD_(ULONG, AddRef)();
	STDMETHOD_(ULONG, Release)();
	STDMETHOD(QueryInterface)(REFIID riid, LPVOID *ppvObj);

	// Custom Methods
	//MouseEvent buildMouseEvent(LONG* pPackets, int packetIndex, int action);
	HRESULT setPropertiesCount(ULONG count);
	bool setStylusVariables(IRealTimeStylus* pStylus, const StylusInfo* pStylusInfo);
	void updateTabletDPI();
	bool getStylusIsHover();
	// Scale Fixing
	bool updateDownscaleFactor(LONG &xpos, LONG &ypos, double& mouseX, double& mouseY);
};

#endif