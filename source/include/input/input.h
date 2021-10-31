#ifndef INPUT_H
#define INPUT_H

#include "keys.h"

#include <glm.hpp>

#include <deque>
#include <vector>
#include <memory>
#include <filesystem>
#include <map>
#include <string>

#include <iostream>

enum class InputFlag
{
	null,
	newInput,
	beginStream,
	point,
	start,
	last,
	end,
	endConnect,
	connectStart,
	connectOrigin,
	connectOriginSwapMeta,
	swapMetadata,
	connectTarget,
	connectOriginSwapClearMetadata,
	swapClearMetadata,
	origin,
	originPoint,
	endStream,
	waypoint,
	pivot,
	scrub,
	pause,
	connect,
	newRecording,
	endRecording,
	previewLine,
	previewLineTip,
	updateData,
};

inline std::map<InputFlag, std::string> inputFlagStringMap = {
	{ InputFlag::null, "null" },
	{ InputFlag::newInput, "newInput" },
	{ InputFlag::beginStream, "beginStream" },
	{ InputFlag::point, "point" },
	{ InputFlag::start, "start" },
	{ InputFlag::last, "last" },
	{ InputFlag::end, "end" },
	{ InputFlag::endConnect, "endConnect" },
	{ InputFlag::connectStart, "connectStart" },
	{ InputFlag::connectOrigin, "connectOrigin" },
	{ InputFlag::connectOriginSwapMeta, "connectOriginSwapMeta" },
	{ InputFlag::swapMetadata, "swapMetadata" },
	{ InputFlag::connectTarget, "connectTarget" },
	{ InputFlag::connectOriginSwapClearMetadata, "connectOriginSwapClearMetadata" },
	{ InputFlag::swapClearMetadata, "swapClearMetadata" },
	{ InputFlag::origin, "origin" },
	{ InputFlag::originPoint, "originPoint" },
	{ InputFlag::endStream, "endStream" },
	{ InputFlag::waypoint, "waypoint" },
	{ InputFlag::pivot, "pivot" },
	{ InputFlag::scrub, "scrub" },
	{ InputFlag::pause, "pause" },
	{ InputFlag::connect, "connect" },
	{ InputFlag::newRecording, "newRecording" },
	{ InputFlag::endRecording, "endRecording" },
	{ InputFlag::previewLine, "previewLine" },
	{ InputFlag::updateData, "updateData" }
};

struct Input
{
	//
	InputModKey modKey = InputModKey::none;
	InputMouseButton button = InputMouseButton::left;
	InputAction action = InputAction::release;
	bool isDouble = false; // This gets set after construction by eventHandler
	//
	InputFlag flagPrimary = InputFlag::null;
	InputFlag flagSecondary = InputFlag::null;
	//
	double x = 0.0;
	double y = 0.0;
	float time = 0.0f;
	//
	float pressure = 0.0f;
	float rotation = 0.0f;
	float tiltX = 0.0f;
	float tiltY = 0.0f;
	float velocity = 0.0f;
	//
	Input();
	Input(float pressure, float rotation, float tiltX, float tiltY, float velocity);
	Input(InputModKey modKey, InputMouseButton button, InputAction action,
		InputFlag flagPrimary, InputFlag flagSecondary,
		double x, double y, float time, 
		float pressure, float rotation, float tiltX, float tiltY, float velocity);
	void resetAll();
	void resetAction();
	void resetPos();
	void resetStylus();
};
//(modKey, button, action, flagPrimary, flagSecondary, x, y, time, pressure, rotation, tiltX, tiltY, velocity)

// InputData is used for packaging Inputs into a single object
struct InputData
{
private:
	std::vector<Input> emptyEvents = {};
public:
	InputModKey inputModKey = InputModKey::none;
	Input start;
	Input end;
	std::vector<Input> inputEvents;
	InputData();
	void reset();
};

#endif
