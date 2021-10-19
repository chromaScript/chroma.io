#include "include/gladHelper.h"

#include "include/WinStylusHandler.h"
#include "include/input/input.h"

#include <RTSCOM.h>
#include <RTSCOM_i.c>
#include <Ole2.h>
#include <msinkaut.h>

#include "include/Application.h"
void bindCursorCallbacks();
void unbindCursorCallbacks();

#include <iostream>

// Notifies the implementing object that this plug-in 
// or one of the previous plug-ins in either the 
// IStylusAsyncPlugin or IStylusSyncPlugin collection threw an exception.
HRESULT WinStylusHandler::Error(IRealTimeStylus* piRtsSrc, IStylusPlugin* piPlugin, RealTimeStylusDataInterest dataInterest, 
	HRESULT hrErrorCode, LONG_PTR* lptrKey)
{
	std::cout << "WINSTYLUSHANDLER::ERROR::CODE" << hrErrorCode << std::endl;
	return S_OK;
}

// Notifies the object implementing the plug-in that the tablet pen is moving on the digitizer (Stylus is in contact).
HRESULT WinStylusHandler::Packets(IRealTimeStylus* pStylus, const StylusInfo* pStylusInfo, ULONG nPackets, 
	ULONG nPacketBuf, LONG* pPackets, ULONG* nOutPackets, LONG** ppOutPackets)
{
	if (stylusVars_isDirty == true)
	{
		stylusVars_isDirty = setStylusVariables(pStylus, pStylusInfo);
		return TPC_E_INVALID_CONFIGURATION;
	}
	// Create an empty Input, and then build it.
	Input mouse;
	InputModKey op = owner.get()->getModKeys();
	mouse.action = InputAction::press;
	mouse.modKey = op;
	// The application will not use Wacom SDK, so it sacrifices some precision and uses GLFW's getCursorPos
	Input pos = owner.get()->getMousePosition(false);
	LONG stylusX = (LONG)pPackets[0];
	LONG stylusY = (LONG)pPackets[1];
	updateDownscaleFactor(stylusX, stylusY, pos.x, pos.y);
	mouse.x = (double)stylusX / downscaleFactor;
	mouse.y = (double)stylusY / downscaleFactor;
	if (glm::distance(
		glm::vec2(mouse.x, mouse.y),
		glm::vec2(owner.get()->getMouseBuffer_back()->x, owner.get()->getMouseBuffer_back()->y)) < (ROOT2 / 2.0f)) {
		return S_OK;
	}
	mouse.time = pos.time;
	// Get the index packet.
	LONG* lastPacket = pPackets + (nPacketBuf - propertiesNumber);
	mouse.flagPrimary = InputFlag::null;
	mouse.flagSecondary = InputFlag::null;
	mouse.pressure = (float)lastPacket[propIndex_pressure] / (float)stylusPressure_max;
	mouse.tiltX = (float)lastPacket[propIndex_xTilt] / (float)stylusTiltX_max;
	mouse.tiltY = (float)lastPacket[propIndex_yTilt] / (float)stylusTiltY_max;
	float a, b, t;
	a = (float)stylusRotation_max / 2.0f;
	b = (float)stylusRotation_max / -2.0f;
	t = (float)lastPacket[propIndex_rotation] / (float)stylusRotation_max;
	mouse.rotation = (a + (t * (b - a))) / 100.0f;
	owner.get()->updateMouseBuffer(mouse);
	mouse.velocity = owner.get()->getMouseVelocity(0.7f);
	owner.get()->mousePosEventHandler(mouse);
	return S_OK;
}
// Notifies the object implementing the plug-in that the stylus is moving above the digitizer.
HRESULT WinStylusHandler::InAirPackets(IRealTimeStylus* pStylus, const StylusInfo* pStylusInfo, ULONG nPackets, ULONG nPacketBuf, LONG* pPackets, ULONG* nOutPackets, LONG** ppOutPackets)
{
	if (stylusVars_isDirty == true)
	{
		stylusVars_isDirty = setStylusVariables(pStylus, pStylusInfo);
		return TPC_E_INVALID_CONFIGURATION;
	}
	// Create an empty Input, and then build it.
	Input mouse;
	InputModKey op = owner.get()->getModKeys();
	mouse.action = InputAction::release;
	mouse.modKey = op;
	// The application will not use Wacom SDK, so it sacrifices some precision and uses GLFW's getCursorPos
	// Warning: Need to clarify the pen data here later. Not sure if tilt & rotation should be zero
	Input pos = owner.get()->getMousePosition(false);
	LONG stylusX = (LONG)pPackets[0];
	LONG stylusY = (LONG)pPackets[1];
	updateDownscaleFactor(stylusX, stylusY, pos.x, pos.y);
	mouse.x = (double)stylusX / downscaleFactor;
	mouse.y = (double)stylusY / downscaleFactor;
	mouse.time = pos.time;
	mouse.flagPrimary = InputFlag::null;
	mouse.flagSecondary = InputFlag::null;
	mouse.pressure = 0.0f;
	mouse.tiltX = 0.0f;
	mouse.tiltY = 0.0f;
	mouse.rotation = 0.0f;
	owner.get()->updateMouseBuffer(mouse);
	mouse.velocity = owner.get()->getMouseVelocity(0.7f);
	owner.get()->mousePosEventHandler(mouse);
	return S_OK;
}
// Notifies the implementing plug-in that the tablet pen has touched the digitizer surface.
HRESULT WinStylusHandler::StylusDown(IRealTimeStylus* pStylus, const StylusInfo* pStylusInfo, ULONG nPackets, LONG* _pPackets, LONG** ppOutPackets)
{
	if (stylusVars_isDirty == true)
	{
		stylusVars_isDirty = setStylusVariables(pStylus, pStylusInfo);
		return TPC_E_INVALID_CONFIGURATION;
	}
	// Create an empty Input, and then build it.
	Input mouse;
	InputModKey op = owner.get()->getModKeys();
	mouse.action = InputAction::press;
	mouse.modKey = op;
	// The application will not use Wacom SDK, so it sacrifices some precision and uses GLFW's getCursorPos
	Input pos = owner.get()->getMousePosition(false);
	LONG stylusX = (LONG)_pPackets[0];
	LONG stylusY = (LONG)_pPackets[1];
	mouse.x = (double)stylusX / downscaleFactor;
	mouse.y = (double)stylusY / downscaleFactor;
	mouse.time = pos.time;
	// Get the index packet, StylusDown appears to only return one properties packet.
	LONG* lastPacket = &_pPackets[0];
	mouse.flagPrimary = InputFlag::null;
	mouse.flagSecondary = InputFlag::null;
	mouse.pressure = (float)lastPacket[propIndex_pressure] / (float)stylusPressure_max;
	mouse.tiltX = (float)lastPacket[propIndex_xTilt] / (float)stylusTiltX_max;
	mouse.tiltY = (float)lastPacket[propIndex_yTilt] / (float)stylusTiltY_max;
	float a, b, t;
	a = (float)stylusRotation_max / 2.0f;
	b = (float)stylusRotation_max / -2.0f;
	t = (float)lastPacket[propIndex_rotation] / (float)stylusRotation_max;
	mouse.rotation = (a + (t * (b - a))) / 100.0f;
	owner.get()->updateMouseBuffer(mouse);
	mouse.velocity = owner.get()->getMouseVelocity(0.7f);
	owner.get()->clickEventHandler(InputMouseButton::left, InputAction::press, mouse, false);
	return S_OK;
}
// Notifies the implementing plug-in that the user has raised the tablet pen from the tablet digitizer surface.
HRESULT WinStylusHandler::StylusUp(IRealTimeStylus* pStylus, const StylusInfo* pStylusInfo, ULONG nPackets, LONG* _pPackets, LONG** ppOutPackets)
{
	if (stylusVars_isDirty == true)
	{
		stylusVars_isDirty = setStylusVariables(pStylus, pStylusInfo);
		return TPC_E_INVALID_CONFIGURATION;
	}
	// Create an empty Input, and then build it.
	Input mouse;
	InputModKey op = owner.get()->getModKeys();
	mouse.action = InputAction::release;
	mouse.modKey = op;
	// The application will not use Wacom SDK, so it sacrifices some precision and uses GLFW's getCursorPos
	Input pos = owner.get()->getMousePosition(false);
	LONG stylusX = (LONG)_pPackets[0];
	LONG stylusY = (LONG)_pPackets[1];
	mouse.x = (double)stylusX / downscaleFactor;
	mouse.y = (double)stylusY / downscaleFactor;
	mouse.time = pos.time;
	// Get the index packet, StylusUp appears to only return one properties packet.
	LONG* lastPacket = &_pPackets[0];
	mouse.flagPrimary = InputFlag::null;
	mouse.flagSecondary = InputFlag::null;
	mouse.pressure = (float)lastPacket[propIndex_pressure] / (float)stylusPressure_max;
	mouse.tiltX = (float)lastPacket[propIndex_xTilt] / (float)stylusTiltX_max;
	mouse.tiltY = (float)lastPacket[propIndex_yTilt] / (float)stylusTiltY_max;
	float a, b, t;
	a = (float)stylusRotation_max / 2.0f;
	b = (float)stylusRotation_max / -2.0f;
	t = (float)lastPacket[propIndex_rotation] / (float)stylusRotation_max;
	mouse.rotation = (a + (t * (b - a))) / 100.0f;
	owner.get()->updateMouseBuffer(mouse);
	mouse.velocity = owner.get()->getMouseVelocity(0.7f);
	owner.get()->clickEventHandler(InputMouseButton::left, InputAction::release, mouse, false);
	return S_OK; 
}
// Notifies the implementing plug-in that the stylus is entering the detection range of the digitizer.
HRESULT WinStylusHandler::StylusInRange(IRealTimeStylus* pStylus, TABLET_CONTEXT_ID tcid, STYLUS_ID sid)
{
	std::cout << "WINSTYLUSHANDLER::STYLUS::INRANGE" << std::endl;
	stylusInRange = true;
	return S_OK;
}
// Notifies the implementing plug-in that the stylus has left the detection range of the digitizer.
HRESULT WinStylusHandler::StylusOutOfRange(IRealTimeStylus* pStylus, TABLET_CONTEXT_ID tcid, STYLUS_ID sid)
{
	std::cout << "WINSTYLUSHANDLER::STYLUS::OUTOFRANGE" << std::endl;
	stylusInRange = false;
	return S_OK;
}
// Retrieves the events for which the plug - in is to receive notifications.
HRESULT WinStylusHandler::DataInterest(RealTimeStylusDataInterest* pEventInterest)
{
	*pEventInterest = (RealTimeStylusDataInterest)(
		RTSDI_Packets | 
		RTSDI_InAirPackets | 
		RTSDI_StylusDown | 
		RTSDI_StylusUp | 
		RTSDI_StylusInRange | 
		RTSDI_StylusOutOfRange
		);
	return S_OK;
}
// Iunknown Methods
ULONG WinStylusHandler::AddRef()
{
	return InterlockedIncrement(&refCount);
}
ULONG WinStylusHandler::Release()
{
	ULONG nNewRef = InterlockedDecrement(&refCount);
	if (nNewRef == 0)
		delete this;

	return nNewRef;
}
HRESULT WinStylusHandler::QueryInterface(REFIID riid, LPVOID *ppvObj)
{
	if ((riid == IID_IStylusSyncPlugin) || (riid == IID_IUnknown))
	{
		*ppvObj = this;
		AddRef();
		return S_OK;
	}
	else if ((riid == IID_IMarshal) && (FTMarshaler != NULL))
	{
		return FTMarshaler->QueryInterface(riid, ppvObj);
	}

	*ppvObj = NULL;
	return E_NOINTERFACE;
}
// Custom Methods
HRESULT WinStylusHandler::setPropertiesCount(ULONG count)
{
	propertiesNumber = count;
	return S_OK;
}
bool WinStylusHandler::setStylusVariables(IRealTimeStylus* pStylus, const StylusInfo* pStylusInfo)
{
	bool isDirty = true;
	isCursorInverted = pStylusInfo->bIsInvertedCursor;
	stylusID = pStylusInfo->cid;
	tabletID = pStylusInfo->tcid;
	pStylus->GetPacketDescriptionData(tabletID, &scaleX, &scaleY, &propertiesNumber, &packetProperties);
	std::cout << "WINSTYLUSHANDLER::VARIABLE" <<
		"::CURSORINVERTED=" << isCursorInverted <<
		"::STYLUSID=" << stylusID << 
		"::TABID=" << tabletID << 
		"::SCALEX=" << scaleX << 
		"::SCALEY=" << scaleY << std::endl;
	for (ULONG j = 0; j < propertiesNumber; j++)
	{
		if (packetProperties[j].guid == GUID_PACKETPROPERTY_GUID_X)
		{
			propIndex_x = j;
			tabResolutionX_min = packetProperties[j].PropertyMetrics.nLogicalMin;
			tabResolutionX_max = packetProperties[j].PropertyMetrics.nLogicalMax;
			std::cout << "WINSTYLUSHANDLER::PROPERTY::POS_X::REGISTERED::INDEX=" << propIndex_x << 
				"::METRIC_LOGICALMIN=" << packetProperties[j].PropertyMetrics.nLogicalMin <<
				"::METRIC_LOGICALMAX=" << packetProperties[j].PropertyMetrics.nLogicalMax <<
				"::METRIC_FRESOLUTION=" << packetProperties[j].PropertyMetrics.fResolution <<
				"::METRIC_UNITS=" << packetProperties[j].PropertyMetrics.Units << std::endl;
		}
		if (packetProperties[j].guid == GUID_PACKETPROPERTY_GUID_Y)
		{
			propIndex_y = j;
			tabResolutionY_min = packetProperties[j].PropertyMetrics.nLogicalMin;
			tabResolutionY_max = packetProperties[j].PropertyMetrics.nLogicalMax;
			std::cout << "WINSTYLUSHANDLER::PROPERTY::POS_Y::REGISTERED::INDEX=" << propIndex_y << 
				"::METRIC_LOGICALMIN=" << packetProperties[j].PropertyMetrics.nLogicalMin <<
				"::METRIC_LOGICALMAX=" << packetProperties[j].PropertyMetrics.nLogicalMax <<
				"::METRIC_FRESOLUTION=" << packetProperties[j].PropertyMetrics.fResolution <<
				"::METRIC_UNITS=" << packetProperties[j].PropertyMetrics.Units << std::endl;
		}
		if (packetProperties[j].guid == GUID_PACKETPROPERTY_GUID_NORMAL_PRESSURE)
		{
			propIndex_pressure = j;
			stylusPressure_min = (float)packetProperties[j].PropertyMetrics.nLogicalMin;
			stylusPressure_max = (float)packetProperties[j].PropertyMetrics.nLogicalMax;
			std::cout << "WINSTYLUSHANDLER::PROPERTY::PRESSURE::REGISTERED::INDEX=" << propIndex_pressure << 
				"::METRIC_LOGICALMIN=" << packetProperties[j].PropertyMetrics.nLogicalMin <<
				"::METRIC_LOGICALMAX=" << packetProperties[j].PropertyMetrics.nLogicalMax <<
				"::METRIC_FRESOLUTION=" << packetProperties[j].PropertyMetrics.fResolution <<
				"::METRIC_UNITS=" << packetProperties[j].PropertyMetrics.Units << std::endl;
		}
		if (packetProperties[j].guid == GUID_PACKETPROPERTY_GUID_X_TILT_ORIENTATION)
		{
			propIndex_xTilt = j;
			stylusTiltX_min = (float)packetProperties[j].PropertyMetrics.nLogicalMin;
			stylusTiltX_max = (float)packetProperties[j].PropertyMetrics.nLogicalMax;
			std::cout << "WINSTYLUSHANDLER::PROPERTY::TILT_X::REGISTERED::INDEX=" << propIndex_xTilt <<
				"::METRIC_LOGICALMIN=" << packetProperties[j].PropertyMetrics.nLogicalMin <<
				"::METRIC_LOGICALMAX=" << packetProperties[j].PropertyMetrics.nLogicalMax <<
				"::METRIC_FRESOLUTION=" << packetProperties[j].PropertyMetrics.fResolution <<
				"::METRIC_UNITS=" << packetProperties[j].PropertyMetrics.Units << std::endl;
		}
		if (packetProperties[j].guid == GUID_PACKETPROPERTY_GUID_Y_TILT_ORIENTATION)
		{
			propIndex_yTilt = j;
			stylusTiltY_min = (float)packetProperties[j].PropertyMetrics.nLogicalMin;
			stylusTiltY_max = (float)packetProperties[j].PropertyMetrics.nLogicalMax;
			std::cout << "WINSTYLUSHANDLER::PROPERTY::TILT_Y::REGISTERED::INDEX=" << propIndex_yTilt <<
				"::METRIC_LOGICALMIN=" << packetProperties[j].PropertyMetrics.nLogicalMin <<
				"::METRIC_LOGICALMAX=" << packetProperties[j].PropertyMetrics.nLogicalMax <<
				"::METRIC_FRESOLUTION=" << packetProperties[j].PropertyMetrics.fResolution <<
				"::METRIC_UNITS=" << packetProperties[j].PropertyMetrics.Units << std::endl;
		}
		if (packetProperties[j].guid == GUID_PACKETPROPERTY_GUID_AZIMUTH_ORIENTATION)
		{
			propIndex_rotation = j;
			stylusRotation_min = (float)packetProperties[j].PropertyMetrics.nLogicalMin;
			stylusRotation_max = (float)packetProperties[j].PropertyMetrics.nLogicalMax;
			std::cout << "WINSTYLUSHANDLER::PROPERTY::ROTATION::REGISTERED::INDEX=" << propIndex_rotation <<
				"::METRIC_LOGICALMIN=" << packetProperties[j].PropertyMetrics.nLogicalMin <<
				"::METRIC_LOGICALMAX=" << packetProperties[j].PropertyMetrics.nLogicalMax <<
				"::METRIC_FRESOLUTION=" << packetProperties[j].PropertyMetrics.fResolution <<
				"::METRIC_UNITS=" << packetProperties[j].PropertyMetrics.Units << std::endl;
		}
	}

	updateTabletDPI();
	isDirty = false;
	CoTaskMemFree(packetProperties);
	return isDirty;
}
void WinStylusHandler::updateTabletDPI()
{
	if (owner.get() == nullptr)
	{
		return;
	}
	bool isLandscape = owner.get()->getWindowOrientation();
	if (isLandscape == true)
	{
		tabletDPIfactor = float((double)owner.get()->getWindowWidth() / (double)tabResolutionX_max);
	}
	else
	{
		tabletDPIfactor = float((double)owner.get()->getWindowHeight() / (double)tabResolutionY_max);
	}
}
bool WinStylusHandler::getStylusIsHover()
{
	return stylusInRange;
}

// The stylus API is not capable of reading wacom tablet resolutions, so this is the only way to accurately
// ascertain the downscale factor for the tablet. Rate of adjustment slows as the count approaches the 
// calibration maximum count
bool WinStylusHandler::updateDownscaleFactor(LONG &xpos, LONG &ypos, double &mouseX, double &mouseY)
{
	if (scaleSetCount > scaleCountMax || doneCalibratingStylus) { return false; }
	if (xpos >= 1 && ypos >= 1)
	{
		int scaleWeight = 0;
		if (scaleSetCount <= scaleCountMax / 1000) { scaleWeight = scaleCountMax / 1000; }
		else if (scaleSetCount <= scaleCountMax / 100) { scaleWeight = scaleCountMax / 500; }
		else if (scaleSetCount <= scaleCountMax / 20) { scaleWeight = scaleCountMax / 50; }
		else if (scaleSetCount <= scaleCountMax / 5) { scaleWeight = scaleCountMax / 20; }
		else if (scaleSetCount <= scaleCountMax / 2) { scaleWeight = scaleCountMax / 10; }
		else { scaleWeight = 1000; }
		double scaleAvg = (((double)xpos / mouseX) + ((double)ypos / mouseY)) / 2.0;
		double newFactor = ((downscaleFactor * ((double)scaleWeight - 1)) + scaleAvg) / (double)scaleWeight;
		if (!isinf(newFactor) && !isnan(newFactor)) { downscaleFactor = newFactor; }
		scaleSetCount++; 
		if (scaleSetCount == scaleCountMax) { 
			doneCalibratingStylus = true; owner.get()->saveDownscaleFactor(downscaleFactor);
			std::cout << "WINSTYLUSHANDLER::CALIBRATION_FINISHED" << std::endl; 
		}
		return true;
	}
	return false;
}

void WinStylusHandler::resetCalibration()
{
	doneCalibratingStylus = false; scaleSetCount = 0; downscaleFactor = 0;
}
void WinStylusHandler::stopCalibration()
{
	doneCalibratingStylus = true;
}