#include "../include/entities/Fonts.h"
#include "../include/structs.h"

#include <glad/glad.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <map>
#include <filesystem>
#include <iostream>

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;

// Store the relevant information for the font Face
struct CachedFace
{
	std::string name;
	std::string filetype;
	std::filesystem::path path;
	FT_Face face;
	std::vector<std::shared_ptr<CachedCharBitmap>> cachedBitmaps;
	CachedFace(std::string name, std::string filetype, std::filesystem::path path)
	{
		this->name = name;
		this->filetype = filetype;
		this->path = path;
	}
};
// A struct to hold a character that's been rasterized as a bitmap
struct FaceCharacter
{
	unsigned int texID;
	glm::ivec2 size;
	glm::ivec2 bearing;
	unsigned int advance;
};
// A character map with a size property for avoiding re-building bitmaps of the same pixel-size for a font
struct CachedCharBitmap
{
	int pxSize;
	std::map<char, FaceCharacter> bitmaps;
	CachedCharBitmap(int pxSize)
	{
		this->pxSize = pxSize;
	}
};

Fonts::Fonts() {
	// Initialize the FT_Library object
	int initErr;
	initErr = FT_Init_FreeType(&ftLib);
	if (initErr != 0) { std::cout << "FONTS::ERROR::FREETYPE::FAILED_FT_LIB_INITIALIZATION" << std::endl; }
	else { std::cout << "FONTS::FREETYPE::LIB_INITIALIZED" << std::endl; }
}

// Load Font Files
int Fonts::loadFontFromFile(std::filesystem::path path)
{
	// Validate that the extension of the font is supported
	std::string ext = path.extension().string();
	bool validExtension = false;
	for (std::string type : acceptedFontTypes)
	{
		if (ext == type) { validExtension = true; break; }
	}
	if (validExtension == false) { return -2; }
	// Make sure that the font has a valid name to use (Do not accept fonts named "" - .stem() returns these as ".ttf")
	std::string name = path.stem().string();
	if (name.find_last_of(".", 0) == 0) { return -3; }
	// Create a new CachedFace and load it's face object
	faces.emplace_back(std::make_shared<CachedFace>(name, ext, path));
	int loadErr = loadFace(faces.back());
	if (loadErr != 0) { return -4; }
	return 0;
}
// Load Face
int Fonts::loadFace(std::shared_ptr<CachedFace> face)
{
	int faceErr = FT_New_Face(ftLib, face.get()->path.string().c_str(), 0, &face.get()->face);
	if (faceErr != 0) { return -1; }
	std::cout << "FONTS::LOADED_NEW_FACE::NAME=" << face.get()->name << "::NUM_GLYPHS=" << face.get()->face->num_glyphs << std::endl;
	return 0;
}
// Get Face
std::shared_ptr<CachedFace> Fonts::getFace(std::filesystem::path queryPath)
{
	std::shared_ptr<CachedFace> outFace = nullptr;
	for (std::shared_ptr<CachedFace> getFace : faces)
	{
		if (getFace.get()->path == queryPath) { outFace = getFace; break; }
	}
	return outFace;
}
// Get Bitmap
std::shared_ptr<CachedCharBitmap> Fonts::getBitmap(std::shared_ptr<CachedFace> queryFace, int fontSize)
{
	std::shared_ptr<CachedCharBitmap> outBitmap = nullptr;
	for (std::shared_ptr<CachedCharBitmap> getBitmap : queryFace.get()->cachedBitmaps)
	{
		if (getBitmap.get()->pxSize == fontSize) { outBitmap = getBitmap; }
	}
	// Bitmaps are not made on initialization immediately, must create them here if a text-type object requests one
	if (outBitmap == nullptr)
	{
		outBitmap = loadNewBitmap(queryFace, fontSize);
	}
	return outBitmap;
}
// Load New Bitmap
std::shared_ptr<CachedCharBitmap> Fonts::loadNewBitmap(std::shared_ptr<CachedFace> face, int fontSize)
{
	face.get()->cachedBitmaps.emplace_back(std::make_shared<CachedCharBitmap>(fontSize));
	std::shared_ptr<CachedCharBitmap> outBitmap = face.get()->cachedBitmaps.back();
	// Set the size to load glyphs as
	FT_Set_Pixel_Sizes(face.get()->face, 0, fontSize);
	// Disable the byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	// Load the first 128 characters of the ASCII set
	for (unsigned char c = 0; c < 128; c++)
	{
		int glyphErr = FT_Load_Char(face.get()->face, c, FT_LOAD_RENDER);
		if (glyphErr != 0) { std::cout << "ERROR::FREETYTPE::FAILED_TO_LOAD_GLYPH" << std::endl; continue; }
		// Generate the Texture ID
		unsigned int texID;
		glGenTextures(1, &texID);
		glBindTexture(GL_TEXTURE_2D, texID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
			face.get()->face->glyph->bitmap.width, face.get()->face->glyph->bitmap.rows,
			0, GL_RED, GL_UNSIGNED_BYTE, face.get()->face->glyph->bitmap.buffer);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
		//	face.get()->face->glyph->bitmap.width, face.get()->face->glyph->bitmap.rows,
		//	0, GL_RED, GL_UNSIGNED_BYTE, data);
		// Set Texture Options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Store the character into the CachedCharBitmap
		//face.get()->face->glyph->li
		FaceCharacter faceChar = {
			texID,
			glm::ivec2(face.get()->face->glyph->bitmap.width, face.get()->face->glyph->bitmap.rows),
			glm::ivec2(face.get()->face->glyph->bitmap_left, face.get()->face->glyph->bitmap_top),
			static_cast<unsigned int>(face.get()->face->glyph->advance.x)
		};
		outBitmap.get()->bitmaps.insert(std::pair<char, FaceCharacter>(c, faceChar));
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	std::cout << "FONTS::GENERATED_NEW_BITMAP::NAME=" << face.get()->name << "::SIZE=" << fontSize << std::endl;
	return outBitmap;
}
// Find Bitmap
std::shared_ptr<CachedCharBitmap> Fonts::findBitmap(int fontSize, std::filesystem::path fontPath)
{
	// First check if any of the currently created faces has a matching fontPath
	 // Avoid rendering missing faces, should have loaded correctly during UI initialization
	// Then check if that CachedFace has a matching CachedCharBitmap with the requested size
	std::shared_ptr<CachedFace> renderFace = getFace(fontPath);
	std::shared_ptr<CachedCharBitmap> renderBitmap = getBitmap(renderFace, fontSize);
	return renderBitmap;
}
// Find Size of TextLine - Used by Text-type widgets to get their size. Similar to renderTextLine but without drawing
glm::ivec2 Fonts::findSizeTextLine(std::string textLine, 
	std::shared_ptr<CachedFace> thisFace, std::shared_ptr<CachedCharBitmap> thisCurrentBitmap)
{
	int lineHeight = thisCurrentBitmap.get()->pxSize;
	// Iterate over the text string
	std::string::const_iterator c_iter;
	glm::vec2 outSize = glm::vec2(0, 0);
	if (textLine.find("..") != std::string::npos)
	{
		int i = 0;
	}
	for (c_iter = textLine.begin(); c_iter != textLine.end(); c_iter++)
	{
		FaceCharacter character = thisCurrentBitmap.get()->bitmaps[*c_iter];
		float scale = 1.0f;

		// Be aware size may not be 100% accurate due to scale or use of float values
		float offsetX = (float)character.bearing.x * scale;
		float xpos = (float)outSize.x + offsetX;
		float offsetY = float(character.size.y - character.bearing.y) * scale;
		//float offsetY = float(lineHeight - character.bearing.y) * scale;
		float ypos = (float)outSize.y - offsetY;

		float w = character.size.x * scale;
		float h = character.size.y * scale;
		// Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
		outSize.x += (character.advance >> 6) * scale;
	}
	return outSize;
}
// Render Text
void Fonts::renderTextLine(std::shared_ptr<Shader> shader, glm::vec3 textColor, float alpha, std::string textLine,
	std::shared_ptr<CachedFace> thisFace, std::shared_ptr<CachedCharBitmap> thisCurrentBitmap, 
	glm::vec2 startLoc, glm::ivec2 offsetPos,
	int VAO, int VBO, int EBO)
{
	// Setup the Shader
	shader->use();
	shader->setVec3("textColor", textColor);
	shader->setFloat("textAlpha", alpha);
	shader->setInt("texture1", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAO);
	
	// Setup the location variables and scale
	float scale = 1.0f;
	glm::vec2 textStartLoc = glm::vec2(startLoc.x - 1, startLoc.y + 1);
	glm::vec2 anchorOffset = glm::vec2(
		((float(offsetPos.x * scale * 2) / WINDOW_WIDTH)),
		((float(offsetPos.y * scale * 2) / WINDOW_HEIGHT)));
	int lineHeight = thisCurrentBitmap.get()->pxSize;
	if (textLine.find("..") != std::string::npos)
	{
		int i = 0;
	}
	// Iterate over the text string
	std::string::const_iterator c_iter;
	for (c_iter = textLine.begin(); c_iter != textLine.end(); c_iter++)
	{
		FaceCharacter character = thisCurrentBitmap.get()->bitmaps[*c_iter];

		float offsetX = ((float(character.bearing.x * scale * 2) / WINDOW_WIDTH));
		float xpos = (float)textStartLoc.x + offsetX + anchorOffset.x;
		//float offsetY = ((float((character.size.y - character.bearing.y) * scale * 2) / WINDOW_HEIGHT));
		//float offsetY = ((float((lineHeight - (character.bearing.y - character.size.y)) * scale * 2) / WINDOW_HEIGHT));
		float offsetY = ((float((lineHeight - character.bearing.y) * scale * 2) / WINDOW_HEIGHT));
		float ypos = (float)textStartLoc.y - offsetY - anchorOffset.y;

		float w = character.size.x * scale;
		w = (float(w * 2) / WINDOW_WIDTH);
		float h = character.size.y * scale;
		h = (float(h * 2) / WINDOW_HEIGHT);
		// update VBO for each character
		float verticies1[20] = {
			// The top and bottom are normal texture coords because this renders in respect
			// to the screen space coordinates
			// positions					// texture coords - 
			xpos + w, ypos - h, 0.0f,		1.0f, 1.0f, // top right
			xpos + w, ypos, 0.0f,			1.0f, 0.0f, // bottom right
			xpos, ypos,	0.0f,				0.0f, 0.0f,  // bottom left
			xpos, ypos - h, 0.0f,			0.0f, 1.0f // top left
		};

		// Render the glyph texture
		glBindTexture(GL_TEXTURE_2D, character.texID);
		// update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(verticies1), verticies1); // be sure to use glBufferSubData and not glBufferData
		//glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render quad
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		textStartLoc.x += (((character.advance >> 6) * scale) * 2) / WINDOW_WIDTH; // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
}