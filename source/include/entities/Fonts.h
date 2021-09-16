#ifndef FONTS_H
#define FONTS_H

#include "../structs.h"
#include "../Shader.h"

#include <glm.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <string>
#include <map>
#include <filesystem>

// Structs for holding data
inline std::string acceptedFontTypes[1] = { ".ttf" };
// Store the relevant information for the font Face
struct CachedFace;
// A struct to hold a character that's been rasterized as a bitmap
struct FaceCharacter;
// A character map with a size property for avoiding re-building bitmaps of the same pixel-size for a font
struct CachedCharBitmap;



class Fonts
{
private:
	FT_Library ftLib;
	std::vector<std::shared_ptr<CachedFace>> faces;
protected:
public:
	Fonts();
	int loadFontFromFile(std::filesystem::path path);
	int loadFace(std::shared_ptr<CachedFace> face);
	std::shared_ptr<CachedFace> getFace(std::filesystem::path queryPath);
	std::shared_ptr<CachedCharBitmap> getBitmap(std::shared_ptr<CachedFace> queryFace, int fontSize);
	std::shared_ptr<CachedCharBitmap> loadNewBitmap(std::shared_ptr<CachedFace> face, int fontSize);
	std::shared_ptr<CachedCharBitmap> findBitmap(int fontSize, std::filesystem::path fontPath);
	// Find TextLine Size
	glm::ivec2 findSizeTextLine(std::string textLine,
		std::shared_ptr<CachedFace> thisFace, std::shared_ptr<CachedCharBitmap> thisCurrentBitmap);
	// Render Functions
	void renderTextLine(std::shared_ptr<Shader> shader, glm::vec4 textColor, float alpha, std::string textLine,
		std::shared_ptr<CachedFace> thisFace, std::shared_ptr<CachedCharBitmap> thisCurrentBitmap, 
		glm::vec2 startLoc, glm::ivec2 offsetPos,
		int VAO, int VBO, int EBO, int overflowValue, glm::vec4 overflowBox);
	void renderTextInput(std::shared_ptr<Shader> textShader, std::string textLine,
		std::shared_ptr<CachedFace> thisFace, std::shared_ptr<CachedCharBitmap> thisCurrentBitmap,
		glm::vec2 startLoc, glm::ivec2 offsetPos, glm::vec4 textColor, glm::vec4 highlightColor, glm::vec4 focusColor,
		std::vector<unsigned int> buffers, glm::ivec4 widgetBounds, int cursorPos, int selectStart, int selectEnd,
		bool drawCursor, bool drawSelection, bool calculateVertData, float* cursorData, float* selectionData, 
		int overflowValue, glm::vec4 overflowBox);
};

#endif