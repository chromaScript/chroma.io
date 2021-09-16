#ifndef TFENUMS_H
#define TFENUMS_H

#include <string>
#include <vector>
#include <map>

enum class TFTokenType
{
// Single-Character Tokens
	// Enclosures
	CURLY_LEFT, CURLY_RIGHT,
	// Seperator
	DOUBLE_COLON,
	// Numeric Symbol
	MINUS, ASSIGN,
	// Explicit Terminator
	SEMICOLON,

	// Literals
	NIL, STRING, NUM,

	// Keywords
		// Identifiers
		ID, SID,
		// Terminators
		HEADER, ENDFILE,
		// Reserved Names
			// Settings Names
			METADATA, PAN, ZOOM, ROTATE, SAMPLER,
			CONTINUOUS, DRAG, ONEPOINT, TWOPOINT, THREEPOINT,
			BLEND, BASIC, IMAGE, CHARACTER, SCATTER,
			COLOR, ALPHA, DISTANCE, MIXING, MULTITIP,
			TEXTURE, GRAIN, WETEDGES, REPEAT, REFERENCE,
			SMOOTHING, ANTIALIASING, VECTOR, POLYGON,
			POLYLINE, MESH, VORTEX, FAN, RAKE, GRADIENT,
			FILL, PATTERN, FIELD, SHADER, FILTER,

			FX_FILL, FX_GRADIENT, FX_POSTERIZE, FX_INVERT,
			FX_THRESHOLD, FX_BRIGHTCONTRAST, FX_HSV, FX_POWER,
			FX_MODULO, FX_BLUR,
			
			EFFECTS,

	END
};

inline std::string TFTokenTypeNames[] = {
	// Single-Character Tokens
		// Enclosures
		"{", "}",
		// Seperator
		"::",
		// Numeric Symbol
		"-", "=",
		// Explicit Terminator
		";",

		// Literals
		"nil", "string", "num",

		// Keywords
			// Identifiers
			"ID=", "SID=",
			// Terminators
			"[header]", "[end]",
			// Reserved Names
				// Settings Names
				"metadata", "pan", "zoom", "rotate", "sampler",
				"continuous", "drag", "onepoint", "twopoint", "threepoint",
				"blend", "basic", "image", "character", "scatter",
				"color", "alpha", "distance", "mixing", "multitip",
				"texture", "grain", "wetedges", "repeat", "reference",
				"smoothing", "antialiasing", "vector", "polygon",
				"polyline", "mesh", "vortex", "fan", "rake", "gradient",
				"fill", "pattern", "field", "shader", "filter",

				"fx_fill", "fx_gradient", "fx_posterize", "fx_invert",
				"fx_threshold", "fx_brightcontrast", "fx_hsv", "fx_power",
				"fx_modulo", "fx_blur",
				
				"effects",

		"end"
};

inline std::map<std::string, TFTokenType> TFTokenTypeMap;

inline TFTokenType stringToTFTokenType(std::string query)
{
	for (std::string name : TFTokenTypeNames)
	{
		if (query == name)
		{
			return TFTokenTypeMap.at(name);
		}
	}
	return TFTokenType::NIL;
}

#endif