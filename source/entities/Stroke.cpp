#include "../include/entities/Stroke.h"
#include "../include/Color.h"
#include "../include/ToolSettings.h"
#include "../include/toolSettings/ToolSettings_Forward.h"

#include <glad/glad.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/quaternion.hpp>

#include "../include/Tool.h"

#include <iostream>
#include <algorithm>
#include <memory>
#include <random>

extern bool doStrokeDebugFrames;

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;

#ifndef APPLICATION_H
#include "../include/Application.h"
#endif
#ifndef CAMERA_H
#include "../include/Camera.h"
#endif

extern std::shared_ptr<Application> chromaIO;

// Warning: Do not forget to implement usage of velocity, tilt, and rotation in modulating
// the shard properties. Make sure that velocity is properly clamped.

// Constructor / Destructor
Stroke::Stroke()
{
	generateUEID();
}
// New Settings-Based Constructor
Stroke::Stroke(std::shared_ptr<Shader> shader, std::shared_ptr<Tool> tool)
{
	generateUEID();
	fragData = VertexData();
	this->shader = shader;
	this->basic = *tool.get()->getBasic();
	this->image = *tool.get()->getImage();
	this->character = *tool.get()->getCharacter();
	this->alpha = *tool.get()->getAlpha();
	this->color = *tool.get()->getColor();
	this->scatter = *tool.get()->getScatter();
	this->effects = *tool.get()->getEffects();
	// Run first-time set-up for any Tool Settings that require it
	if (effects.isEnabled) 
	{ 
		effects.initializeData(basic.currentFGColor, basic.currentBGColor, image.tipSize);
		if (effects.gradient.isEnabled)
		{
			effects.gradient.gradient.generateTexture();
			bindTexture_utility(
				4, effects.gradient.gradient.textureResolution, 1,
				effects.gradient.gradient.textureID,
				effects.gradient.gradient.texture);
		}
	}
	if (scatter.isEnabled) { scatter.updatePadding(); }

	this->frameShader = chromaIO.get()->getFrameShader();
	this->compositeShader = chromaIO.get()->getCompositeShader();
	this->compositeFrameShader = chromaIO.get()->getCompositeFrameShader();
	this->debugLineShader = chromaIO.get()->getDebugLineShader();
	// Note: Must use the default class transform variable to store
	setTransform(glm::vec3(0.0f, 0.0f, 0.0f), Bounds(0, 0, 0, 0), 0.0f);
	// Each shard shares the same Buffer ID's & texData. The shader is used to specify the alpha, color,
	// scale, etc. of shards.
	// Warning: The 'stroke-image' data should be moved into a new Image class which stores data that multiple
	// objects can access. This will let the vertex-data be freed again for other uses.
	setVertData_stroke((float)image.tipSize, (image.isEnabled) ? image.scaleX : 1.0f, (image.isEnabled) ? image.scaleY : 1.0f);
	initializeData(image.tipSize * image.tipSize);
	// Warning: For now fill data with square shape, must change later!
	CColor_uc brushColor = basic.currentFGColor.makeCColor_uc();
	int count = 0;
	int radius = image.tipSize / 2;
	for (int y = 0; y < image.tipSize; y++)
	{
		for (int x = 0; x < image.tipSize; x++)
		{
			data[((x + (y * image.tipSize)) * 4) + 0] = brushColor.r;
			data[((x + (y * image.tipSize)) * 4) + 1] = brushColor.g;
			data[((x + (y * image.tipSize)) * 4) + 2] = brushColor.b;
			if (pow((x - radius), 2) + pow((y - radius), 2) < pow(radius, 2))
			{
				data[((x + (y * image.tipSize)) * 4) + 3] = 255;
			}
			else
			{
				data[((x + (y * image.tipSize)) * 4) + 3] = 0;
			}
		}
	}
	generateBuffers(&(this->VAO), &(this->VBO), &(this->EBO), &(this->TEX0));
	bindBuffers();
	bindTexture(image.tipSize, image.tipSize);

	if (doStrokeDebugFrames)
	{
		initializeDebugData();
	}

	// Rendering Pipeline Testing - Should negate the code above for tempBuffer
	compositePadding = int(((effects.blur.isEnabled) ? effects.blur.radius * 1.45f : 0.0f) +
		((scatter.isEnabled) ? scatter.fetchPadding(image.tipSize) * 1.15f : 0.0f) +
		(image.tipSize * 1.2f));
	// Set the initial size of the compositeData
	setVertData_composite(compositePadding, compositePadding);

	glGenVertexArrays(1, &quadVAO); 
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	// Initialize the composite permanent buffer
	glGenVertexArrays(1, &compVAO);
	glGenBuffers(1, &compVBO);
	glGenBuffers(1, &compEBO);

	glBindVertexArray(compVAO);
	glBindBuffer(GL_ARRAY_BUFFER, compVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(compositeVerts), compositeVerts, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, compEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertArrangement_size, vertArrangement, GL_STATIC_DRAW);
	// Set vertex position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, attribStride, (void*)0);
	glEnableVertexAttribArray(0);
	// Set texture coordinate attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, attribStride, (void*)tAttribOffset);
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);


	glGenFramebuffers(1, &compBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, compBuffer);
	// create a color attachment texture
	glGenTextures(1, &compColorBuffer);
	glBindTexture(GL_TEXTURE_2D, compColorBuffer);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, compositeSize.x, compositeSize.y, 0, GL_RGBA, GL_FLOAT, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, compColorBuffer, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Initialize the composite temporary buffer
	glGenVertexArrays(1, &compTempVAO);
	glGenBuffers(1, &compTempVBO);
	glGenBuffers(1, &compTempEBO);

	glBindVertexArray(compTempVAO);
	glBindBuffer(GL_ARRAY_BUFFER, compTempVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(compositeVerts), compositeVerts, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, compTempEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertArrangement_size, vertArrangement, GL_STATIC_DRAW);
	// Set vertex position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, attribStride, (void*)0);
	glEnableVertexAttribArray(0);
	// Set texture coordinate attribute
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, attribStride, (void*)tAttribOffset);
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	glGenFramebuffers(1, &compTempBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, compTempBuffer);
	// create a color attachment texture
	glGenTextures(1, &compTempColorBuffer);
	glBindTexture(GL_TEXTURE_2D, compTempColorBuffer);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, compositeSize.x, compositeSize.y, 0, GL_RGBA, GL_FLOAT, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, compTempColorBuffer, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Generate Buffers for the lineVAO/lineVBO
	glGenVertexArrays(1, &linesVAO);
	glGenBuffers(1, &linesVBO);
	glGenVertexArrays(1, &boundsVAO);
	glGenBuffers(1, &boundsVBO);
}

void Stroke::cleanup_stroke()
{
	glDeleteBuffers(1, &this->quadVBO);
	glDeleteVertexArrays(1, &this->quadVAO);

	glDeleteBuffers(1, &this->compVBO);
	glDeleteBuffers(1, &this->compEBO);
	glDeleteVertexArrays(1, &this->compVAO);

	glDeleteFramebuffers(1, &this->compBuffer);
	glDeleteTextures(1, &this->compColorBuffer);

	glDeleteVertexArrays(1, &compTempVAO);
	glDeleteBuffers(1, &compTempVBO);
	glDeleteBuffers(1, &compTempEBO);

	glDeleteFramebuffers(1, &compTempBuffer);
	glDeleteTextures(1, &compTempColorBuffer);

	glDeleteVertexArrays(1, &linesVAO);
	glDeleteBuffers(1, &linesVBO);
	glDeleteVertexArrays(1, &boundsVAO);
	glDeleteBuffers(1, &boundsVBO);

	frameShader.reset();
	compositeShader.reset();
	compositeFrameShader.reset();
	debugLineShader.reset();
}

Stroke::~Stroke()
{
	delete []strokeImageData;
	delete strokeImageData;
	delete []lineLoop;
	delete lineLoop;
	delete []tempImageData;
	delete tempImageData;
}

void Stroke::updateGenData(int lastShard, glm::vec3 pos, glm::vec3 dir, float count, float trueSpacing, float tipSize)
{
	totalPos.x += pos.x; totalPos.y += pos.y;
	averagePos = glm::vec3(totalPos.x / (float)lastShardID, totalPos.y / (float)lastShardID, (float)lastShardID);
	if (rebuildBounds) { fillX.x = fillX.y = pos.x; fillY.x = fillY.y = pos.y; rebuildBounds = false; }
	else
	{
		if (pos.x + tipSize > fillX.y) { fillX.y = pos.x + tipSize; }
		else if (pos.x - tipSize < fillX.x) { fillX.x = pos.x - tipSize; }
		if (pos.y + tipSize > fillY.y) { fillY.y = pos.y + tipSize; }
		else if (pos.y - tipSize < fillY.x) { fillY.x = pos.y + tipSize; }
		if ((fillX.y - fillX.x) < (abs(fillY.y - fillY.x))) { isPortrait = true; }
		else { isPortrait = false; }
	}

	glm::vec2 size = glm::vec2(fillX.y - fillX.x, fillY.y - fillY.x);
	center = glm::vec3(size.x / 2.0f, size.y / 2.0f, 0.0f);
	glm::vec2 posCenter = glm::vec2(pos.x - fillX.x, pos.y - fillY.x);
	glm::vec3 dirOut = glm::normalize(glm::vec3(posCenter.x - center.x, posCenter.y - center.y, 0.0f));
	dirOut = dir;
	float tipTrueSpacingRatio = tipSize / trueSpacing;

	float length = sqrtf(center.x * center.x + center.y * center.y);
	if (!isnan(dir.x) && count > 0)
	{
		if (count > (2 * (maskDir_spacingFactor / tipTrueSpacingRatio)))
		{
			float countWeight = count;
			if (count < (15 * (maskDir_spacingFactor / tipTrueSpacingRatio)))
			{
				countWeight = clampf(0.6f * maskDir_cutoffFactor, 1.0f, 20.0f);
			}
			else if (count < (80 * (maskDir_spacingFactor / tipTrueSpacingRatio)))
			{
				countWeight = clampf(0.1f * maskDir_cutoffFactor, 1.0f, 20.0f);
			}
			else if (count < (140 * (maskDir_spacingFactor / tipTrueSpacingRatio)))
			{
				countWeight = clampf(1.5f * maskDir_cutoffFactor, 1.0f, 20.0f);
			}
			else { countWeight = 20.0f; }
			dir_average.x = ((dir_average.x * count) + (dir.x / countWeight))
				/ count;
			dir_average.y = ((dir_average.y * count) + (dir.y / countWeight))
				/ count;
			dir_average = glm::normalize(dir_average);
		}
		//std::cout << glm::degrees(atan2f(dir_average.x, dir_average.y)) << std::endl;
	}
	dirIntersectA = (center - (dir_average * (length / 1.0f))); dirIntersectA.y = size.y - dirIntersectA.y;
	dirIntersectB = (center + (dir_average * (length / 1.0f))); dirIntersectB.y = size.y - dirIntersectB.y;
}

// Container Functions
// Overload for passing for direct construction
void Stroke::generateShards(int& shardCount, int& lastShardID, glm::vec3 pos, glm::vec3 dir, float scale,
	CColor fgColorIn, CColor bgColorIn, float pressure,
	float rotation, float tiltx, float tilty, float velocity)
{
	float outOpacity = 1.0f; float outFlow = 1.0f;
	float outScale = 1.0f;
	glm::vec3 finalPos = pos;
	CColor outFGCol = fgColorIn;
	CColor outBGCol = bgColorIn;
	float colValue = 0.0f;
	if (scatter.isEnabled)
	{
		//glm::vec3 savePos = pos;
		int count = scatter.modulateCount();
		
		for (int i = 0; i < count; i++)
		{
			outScale = character.noiseScale(scale, pressure);
			finalPos = pos;
			outFGCol = fgColorIn;
			outBGCol = bgColorIn;
			if (color.isEnabled)
			{
				color.modulateColor(outFGCol, outBGCol, dir, pressure, rotation, tiltx, tilty, velocity);
			}
			if (alpha.isEnabled)
			{
				alpha.modulateAlpha(outOpacity, outFlow, dir, pressure, rotation, tiltx, tilty, velocity);
			}
			// Apply Scattering
			finalPos = scatter.modulatePosition(finalPos, dir, image.tipSize);

			colValue = outFGCol.makeGreyscale();
			if (colValue > shardValue_max) { shardValue_max = colValue; }
			else if (colValue < shardValue_min) { shardValue_min = colValue; }
			// Create New Shard
			createNewShard(lastShardID, finalPos, dir, outScale,
				outFGCol, outOpacity, outFlow, pressure,
				rotation, tiltx, tilty, velocity);
			lastShardID++;
			shardCount++;
		}
	}
	else
	{
		outScale = character.noiseScale(scale, pressure);
		if (color.isEnabled)
		{
			color.modulateColor(outFGCol, outBGCol, dir, pressure, rotation, tiltx, tilty, velocity);
		}
		if (alpha.isEnabled)
		{
			alpha.modulateAlpha(outOpacity, outFlow, dir, pressure, rotation, tiltx, tilty, velocity);
		}

		colValue = outFGCol.makeGreyscale();
		if (colValue > shardValue_max) { shardValue_max = colValue; }
		else if (colValue < shardValue_min) { shardValue_min = colValue; }
		// Create New Shard
		createNewShard(lastShardID, finalPos, dir, outScale,
			outFGCol, outOpacity, outFlow, pressure,
			rotation, tiltx, tilty, velocity);
		lastShardID++;
		shardCount++;
	}

}
void Stroke::createNewShard(int ID, glm::vec3 pos, glm::vec3 dir, float scale,
	CColor color, float opacity, float flow, float pressure, float rotation, float tiltx, float tilty, float velocity)
{
	shards.emplace_back(StrokeShard(ID, pos, dir, scale,
		color, opacity, flow, pressure,
		rotation, tiltx, tilty, velocity));
}

// Process new Anchor
void Stroke::processNewAnchor()
{
	//std::cout << "STROKE::PROCESSNEWANCHOR" << std::endl;
	int size = (int)fragData.anchors.size();
	int shardCount = 0;
	if (fragData.anchors.size() == 0) { return; } // Kick back if bad call
	int flagNew = fragData.anchors.back().flag;
	if (fragData.anchors.size() == 1 || flagNew == FLAG_POINT || flagNew == FLAG_ORIGIN_POINT)
	{
		// Place first Shard
		FragmentAnchor* fa = &fragData.anchors.front();
		
		glm::vec3 outDir;
		if (isnan(fa->dir.x)) { outDir = glm::vec3(0.0f, 0.0f, 0.0f); }
		else { outDir = fa->dir; }

		// Update Effects Data
		updateGenData(lastShardID, fa->pos, outDir, averagePos.z, image.trueSpacing, image.tipSize);

		generateShards(shardCount, lastShardID, fa->pos, outDir, character.modulateScale(fa->pressure),
			basic.currentFGColor, basic.currentBGColor,
			fa->pressure, fa->rotation, fa->tiltx, fa->tilty, fa->velocity);
	}
	else
	{
		FragmentAnchor* faNew = &fragData.anchors[size_t(size) - 1];
		FragmentAnchor* faPrev = nullptr;
		switch (faNew->flag)
		{
		case FLAG_NULL:
			faPrev = &fragData.anchors[size_t(size) - 2];
			break;
		case FLAG_CONORIGIN:
			faPrev = &fragData.anchors.front(); 
			faPrev->copyStylusData(faNew);
			break;
		case FLAG_CONORIGIN_SWAPMETA:
			faPrev = &fragData.anchors.front();
			faPrev->swapStylusData(faNew);
			break;
		case FLAG_CONORIGIN_SWAPCLEARMETA:
			faPrev = &fragData.anchors.front();
			faPrev->swapStylusData(faNew);
			faNew->clearStylusData();
			break;
		default:
			faPrev = &fragData.anchors[size_t(size) - 2];
		}
		
		if (faNew->flag == FLAG_START) { return; }
		// 1. Define the length of AnchorP -> anchorN (where P = previous, N = new)
		// 2. Define the X/Y amount to add between each new shard based on spacing value
		float segLen = glm::length(faNew->pos - faPrev->pos);
		float incX = (faNew->pos.x - faPrev->pos.x);
		float incY = (faNew->pos.y - faPrev->pos.y);
		// Note, calculate a new dir here, rather than use anchor.dir. Anchor.dir is used for shard properties
		glm::vec3 dir = glm::normalize(glm::vec3(incX, incY, 0.0f));
		if (isnan(dir.x))
		{
			dir = glm::vec3(0.0f, 0.0f, 0.0f);
		}
		// Update Effects Data
		updateGenData(lastShardID, faNew->pos, dir, averagePos.z, image.trueSpacing, image.tipSize);
		// Becausue length of dir should always be 1, dir can be used as a reliable source for the X/Y spacing
		incX = dir.x * image.trueSpacing;
		incY = dir.y * image.trueSpacing;
		float spacingLen = glm::length(glm::vec2(incX, incY));
		// 3. Beginning from AnchorP, for float = 0, less than length Ap -> An

			// A. Check if the spacing length minus storedLength is greater than segment length
			// StoredLength is the amount of distance consumed towards the next shard placement,
			// it accumulates when the next shard position exceeds the available length on the segment
		if (spacingLen - storedLength > segLen)
		{
			// A1. Add the segment into the stored length, reducing the threshold to pass this logic
			// on the next new anchor placement.
			storedLength += segLen;
		}
		else
		{

			// Set initial outbound position to the previous anchor position
			glm::vec3 outPos = faPrev->pos;

			float usedLen = 0;
			while (usedLen < segLen)
			{
				// B1. Calculate the lerp values for the pen data first. They are needed to fix the spacing value.
				float t = usedLen / segLen;
				float outPressure = faPrev->pressure + (t * (faNew->pressure - faPrev->pressure));
				float outRotation = faPrev->rotation + (t * (faNew->rotation - faPrev->rotation));
				float outTiltx = faPrev->tiltx + (t * (faNew->tiltx - faPrev->tiltx));
				float outTilty = faPrev->tilty + (t * (faNew->tilty - faPrev->tilty));
				float outVelocity = faPrev->velocity + (t * (faNew->velocity - faPrev->velocity));
				
				// B2. Calculate the amount of length left.
				float outScale = character.modulateScale(outPressure);
				if (storedLength > 0.01)
				{
					float remainder = ((spacingLen * outScale) - storedLength) / (spacingLen * outScale);
					outPos += glm::vec3(
						((incX * outScale) * remainder),
						((incY * outScale) * remainder),
						0.0f);
					storedLength = 0;
					usedLen += remainder * (spacingLen * outScale);
				}
				else if (shardCount != 0)
				{
					outPos += glm::vec3(
						(incX * outScale),
						(incY * outScale),
						0.0f);
					usedLen += (spacingLen * outScale);
				}

				// B3. Find the relative position of shard.pos between Ap -> An
				
				// B4. Use this value to assign floating values based on tool settings
				// Using lerp formula a + t(b - a), where A = 0.0, and B = 1.0
				// Make sure that neither dir isnan, Note: this can be removed later once real time anchor culling is implemented
				glm::vec3 outDir, dirA, dirB;
				if (isnan(faPrev->dir.x)) { dirA = glm::vec3(0.0f, 0.0f, 0.0f); }
				else { dirA = faPrev->dir; }
				if (isnan(faNew->dir.x)) { dirB = glm::vec3(0.0f, 0.0f, 0.0f); }
				else { dirB = faNew->dir; }
				outDir = lerpDir(dirA, dirB, t * faNew->dirInterpFactor);

				generateShards(shardCount, lastShardID, outPos, outDir, outScale,
					basic.currentFGColor, basic.currentBGColor,
					outPressure, outRotation, outTiltx, outTilty, outVelocity);

				// B7. Break if the next anchor would not be placable.
				if (usedLen + spacingLen > segLen)
				{
					storedLength = abs(segLen - usedLen); // Use abs for sign safety
					break;
				}
			}
		}
	}

	// Render the new Shards
	// Do first-time setup
	if (fragData.anchors.size() == 1)
	{
		compositePos = fragData.anchors.back().pos;
		compositePos.x -= (compositeSize.x / 2);
		compositePos.y += (compositeSize.y / 2);
		compMinX = compositePos.x;
		compMinY = compositePos.y;
		compMaxX = compositePos.x + compositeSize.x;
		compMaxY = compositePos.y - compositeSize.y;
		setVertData_composite(compMaxX - compMinX, compMinY - compMaxY);

		// Resize Vertex Data
		glBindVertexArray(compVAO);
		glBindBuffer(GL_ARRAY_BUFFER, compVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(compositeVerts), &compositeVerts, GL_DYNAMIC_DRAW);
		glBindVertexArray(compTempVAO);
		glBindBuffer(GL_ARRAY_BUFFER, compTempVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(compositeVerts), &compositeVerts, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Resize Textures
		glBindTexture(GL_TEXTURE_2D, compColorBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, compositeSize.x, compositeSize.y, 0, GL_RGBA, GL_FLOAT, 0);
		glBindTexture(GL_TEXTURE_2D, compTempColorBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, compositeSize.x, compositeSize.y, 0, GL_RGBA, GL_FLOAT, 0);
		// Clear compBuffer
		glBindFramebuffer(GL_FRAMEBUFFER, compBuffer);
		glClearColor(basic.currentFGColor.r, basic.currentFGColor.g, basic.currentFGColor.b, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	// Check if the bounding box needs to be resized
	glm::ivec3 oldPos = compositePos;
	glm::ivec2 oldSize = compositeSize;
	glm::vec3 newPos = fragData.anchors.back().pos;
	bool didUpdate = false;

	if (newPos.x + compositePadding > compMaxX) { compMaxX = (int)newPos.x + compositePadding; didUpdate = true; }
	if (newPos.y < compMaxY + compositePadding) { compMaxY = (int)newPos.y - compositePadding; didUpdate = true; }
	if (newPos.x - compositePadding < compMinX) { compMinX = (int)newPos.x - compositePadding; compositePos.x = compMinX; didUpdate = true; }
	if (newPos.y > compMinY - compositePadding) { compMinY = (int)newPos.y + compositePadding; compositePos.y = compMinY; didUpdate = true; }

	if (didUpdate)
	{

		// 1. Capture the old data
		float* oldData = new float[(size_t)oldSize.x * (size_t)oldSize.y * 4];
		glGetTextureImage(compColorBuffer, 0, GL_RGBA, GL_FLOAT, sizeof(float) * oldSize.x * oldSize.y * 4, oldData);
		// 2. Update the frame sizes and clear the data
		setVertData_composite(compMaxX - compMinX, compMinY - compMaxY);
		// Get the offset value
		glm::ivec2 offset = glm::ivec2(oldPos.x - compositePos.x, compositePos.y - oldPos.y);
		// Resize Vertex Data
		glBindVertexArray(compVAO);
		glBindBuffer(GL_ARRAY_BUFFER, compVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(compositeVerts), compositeVerts, GL_DYNAMIC_DRAW);
		glBindVertexArray(compTempVAO);
		glBindBuffer(GL_ARRAY_BUFFER, compTempVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(compositeVerts), &compositeVerts, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Resize Textures
		glBindTexture(GL_TEXTURE_2D, compColorBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, compositeSize.x, compositeSize.y, 0, GL_RGBA, GL_FLOAT, 0);
		glBindTexture(GL_TEXTURE_2D, compTempColorBuffer);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, compositeSize.x, compositeSize.y, 0, GL_RGBA, GL_FLOAT, 0);
		// Clear buffers
		glBindFramebuffer(GL_FRAMEBUFFER, compBuffer);
		glClearColor(basic.currentFGColor.r, basic.currentFGColor.g, basic.currentFGColor.b, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindFramebuffer(GL_FRAMEBUFFER, compTempBuffer);
		glClearColor(basic.currentFGColor.r, basic.currentFGColor.g, basic.currentFGColor.b, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// 3. Manually blit the old data into the new frame
		glTextureSubImage2D(compColorBuffer, 0, offset.x, offset.y, oldSize.x, oldSize.y, GL_RGBA, GL_FLOAT, oldData);
		delete[] oldData;
	}

	glm::mat4 projection = glm::ortho(float(-compositeSize.x), 0.0f, float(compositeSize.y), 0.0f, -10.0f, 10.0f);
	glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(0.0f, 1.0f, 0.0f));
	glViewport(0, 0, compositeSize.x, compositeSize.y);

	size_t shardFirst = shards.size() - shardCount;
	for (int i = (int)shardFirst; i < (int)shards.size(); i++)
	{
		// Draw the shard on it's own buffer texture
		StrokeShard* obj = &shards[i];

		glBindFramebuffer(GL_FRAMEBUFFER, compTempBuffer);
		glClearColor(obj->color.r, obj->color.g, obj->color.b, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glBlendEquation(GL_FUNC_ADD);

		glBlendEquationSeparate(GL_MAX, GL_MAX);
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

		// Later, a single shader should be used that handles all of this without needing to swap
		// to a different program
		shader->use();
		shader->setInt("texture1", 0);
		shader->setMat4("projection", projection);
		shader->setMat4("view", view);
		glBindVertexArray(VAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TEX0);

		glm::vec3 transPos = (glm::vec3(compositePos) - obj->pos);
		transPos.y = compositeSize.y - transPos.y;
		transPos.x = -(compositeSize.x + transPos.x);

		modelMatrix = glm::mat4(1.0f);
		
		modelMatrix = glm::translate(modelMatrix, transPos + glm::vec3(image.offsetX, image.offsetY, 0.0f));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(obj->scale));
		if (isnan(obj->dir.x) || isnan(obj->dir.y))
		{
			modelMatrix = glm::rotate(modelMatrix, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));
		}
		else
		{
			modelMatrix = glm::rotate(modelMatrix, atan2(obj->dir.x, obj->dir.y) + glm::radians(image.rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		}
		shader->setMat4("model", modelMatrix);
		// Drawing with opacity by pressure enabled
		glm::vec4 rgba = glm::vec4(obj->color.r, obj->color.g, obj->color.b, 1.0f);
		shader->setVec4("rgba", rgba);
		// Flow with pressure disabled, set to constant
		// This is a flat reductive multiplier to the resulting opacity of the shard (brush tip)
		shader->setFloat("flow", obj->flow);
		shader->setFloat("opacity", obj->opacity);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// Bind the framebuffer(combined output) and draw the tempbuffer into it, 
		// calculating the alpha by hand, then using MAX to
		// get the correct 'opacity' effect.
		glBindFramebuffer(GL_FRAMEBUFFER, compBuffer);
		//glClearColor(obj->color.r, obj->color.g, obj->color.b, 0.0f);
		
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Old Blend Function Pre-Color-Dynamics
		//glBlendEquation(GL_MAX); // Old Blend Equation Pre-Color-Dynamics
		
		glBlendEquationSeparate(GL_FUNC_ADD, GL_MAX);
		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

		compositeFrameShader->use();
		compositeFrameShader->setInt("dstTex", 0);
		compositeFrameShader->setInt("srcTex", 1);
		compositeFrameShader->setBool("preMultiply", true);
		// Drawing with opacity by pressure enabled, this clamps the resulting alpha to the current pressure,
		// regardless of flow build up, ie. flow builds up the opacity to the current allowed max.
		compositeFrameShader->setFloat("opacity", obj->opacity);
		glBindVertexArray(quadVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, compColorBuffer);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, compTempColorBuffer);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
}

// rebuildStroke should be called when a complete reinterp of a stroke is required
// This is usually called at the end of output post-processing or when manipulating the stroke anchors
// with other tools (Control Point, Transform, Redraw Line). For stroke rebuilds that don't require rebuilding,
// use rebuildAnchors instead
void Stroke::rebuildFragmentShards()
{
	return; // For safety while developing the function
	// Use the same process as set in processNewAnchor to clear the current shards and 
	// rebuild them from the anchor data. This is hypothetically expensive and should not 
	// be done often.
	// Suggestion: this function may be a good candidate for multi-threading later
}

// rebuildAnchorShards is used to restructure sets of anchor points independently of anchors outside their influence
// To use rebuildAnchors, pass an array of ID numbers for the anchors to this functions. Be aware that this function
// should not be responsible for anything other than rebuilding the shards for the anchors. Use other functions
// to apply movements/transforms & adjustments to the anchor data.
void Stroke::rebuildAnchorShards(int anchorID)
{
	// Note: Need to improve the manner in which the anchor is selected. Should not be cycling through
	// with a for loop in order to get the anchor
}

// Data Functions
// Need a custom setVertData for stroke specific needs
// This sets the vert data for the brush tip itself
void Stroke::setVertData_stroke(float size, float scaleX, float scaleY)
{
	float x = (-size / 2.0f) * scaleX; float y = (size / 2.0f) * scaleY;
	float tx = 1.0f; float ty = 1.0f;
	// Now create a new float array with these bounds
	float newData[20] = {
		// The top and bottom are swapped so that 0,0 is in the upper left corner
		// positions			// texture coords - 

		x, y, 0.0f,				tx, 0.0f, // top right
		x, -y,	0.0f,			tx, ty, // bottom right
		-x, -y, 0.0f,			0.0f, ty,  // bottom left
		-x, y, 0.0f,			0.0f, 0.0f // top left
	};
	// Overwrite the old data with the new data
	for (int i = 0; i < 20; i++)
	{
		this->vertData[i] = newData[i];
	}
}

void Stroke::setCompositePos(float x, float y)
{

}

void Stroke::reframeCompositeData(float widthNew, float heightNew, float xposNew, float yposNew)
{

}

void Stroke::setVertData_composite(int width, int height)
{
	//int size = (width >= height) ? width : height;
	compositeSize = glm::ivec2(abs(width), abs(height));
	float ratio = (float)width / (float)height;
	float vx = (float)width; float vy = (float)-height;
	float tx = 1.0f; float ty = 1.0f;
	// Now create a new float array with these bounds
	if (ratio >= 1.0f)
	{
		tx = 1.0f;
		ty = tx / ratio;
	}
	else
	{
		ty = 1.0f;
		tx = ty * ratio;
	}
	// Now create a new float array with these bounds
	float newData[20] = {
		// The top and bottom are swapped so that 0,0 is in the upper left corner
		// positions			// texture coords - 
		vx, 0, 0.0f,			tx, 0, // top right
		vx, vy,	0.0f,			tx, ty, // bottom right
		0, vy, 0.0f,			0, ty,  // bottom left
		0, 0, 0.0f,				0,0 // top left
	};
	// Overwrite the old data with the new data
	for (int i = 0; i < 20; i++) { compositeVerts[i] = newData[i]; }
}

// Line Drawing Functions
void Stroke::setLineDraw(std::vector<glm::vec3> lines)
{
	drawShapeLines = true;
	delete[] lineLoop;
	lineLoopLen = (int)lines.size();
	lineLoop = new float [(size_t)lineLoopLen * 3];
	for (int i = 0; i < (int)lines.size(); i++)
	{
		lineLoop[(i * 3) + 0] = (float)lines[i].x;
		lineLoop[(i * 3) + 1] = (float)lines[i].y;
		lineLoop[(i * 3) + 2] = (float)lines[i].z;
	}
	// Bind the vertex data
	glBindVertexArray(linesVAO);
	glBindBuffer(GL_ARRAY_BUFFER, linesVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(lineLoop), lineLoop, GL_STATIC_DRAW);
	// Set vertex position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}
void Stroke::disableLineDraw()
{
	drawShapeLines = false;
}
void Stroke::setBoundsDraw(EntityTransform transform)
{
	drawBoundsLines = true;
	// Apply rotation here because it's easier and saves time in the draw call
	glm::quat rotation = glm::quat(glm::vec3(0, 0, glm::radians(fragData.transform.roll)));
	std::vector<glm::vec3> points = {
		fragData.transform.origin + (rotation * (fragData.transform.bounds.p1 - fragData.transform.origin)),
		fragData.transform.origin + (rotation * (fragData.transform.bounds.p2 - fragData.transform.origin)),
		fragData.transform.origin + (rotation * (fragData.transform.bounds.p3 - fragData.transform.origin)),
		fragData.transform.origin + (rotation * (fragData.transform.bounds.p4 - fragData.transform.origin)),
		fragData.transform.origin
	};
	// Update the boundsVerts with the new points
	for (int i = 0; i < 5; i++)
	{
		boundsVerts[(i * 3) + 0] = (float)points[i].x;
		boundsVerts[(i * 3) + 1] = (float)points[i].y;
		boundsVerts[(i * 3) + 2] = (float)points[i].z;
	}
	// Bind the vertex data
	glBindVertexArray(boundsVAO);
	glBindBuffer(GL_ARRAY_BUFFER, boundsVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(boundsVerts), boundsVerts, GL_STATIC_DRAW);
	// Set vertex position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}
void Stroke::disableBoundsDraw()
{
	drawBoundsLines = false;
}

// Render Functions
void Stroke::draw(ShaderTransform xform)
{
	if ((int)fragData.anchors.size() != 0 && (int)shards.size() != 0)
	{
		// Note: This block enables/disables debug drawing of the stroke. For now leave this in case
		// it ever breaks or work needs to be done on the engine.
		if (false)
		{
			drawDebugData(xform);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBlendEquation(GL_FUNC_ADD);
		modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(xform.m, glm::vec3(compositePos));

		setCompositeShaderUniforms(xform);
		if (effects.isEnabled)
		{
			glBindVertexArray(compVAO);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, compColorBuffer);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, effects.gradient.gradient.textureID);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
		else
		{
			glBindVertexArray(compVAO);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, compColorBuffer);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
		
	}
	if (drawShapeLines && lineLoop != nullptr)
	{
		debugLineShader->use();
		debugLineShader->setMat4("projection", xform.p);
		debugLineShader->setMat4("view", xform.v);
		
		glm::vec4 color = glm::vec4(basic.currentFGColor.makeVec3(), 1.0f);
		debugLineShader->setVec4("lineColor", color);
		glPointSize(lineSize);
		glBindVertexArray(linesVAO);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, linesVBO);
		for (int i = 0; i < lineLoopLen; i++)
		{
			glPointSize(2 + ((fragData.anchors[i].pressure / 1.0f) * (lineSize - 2)));
			modelMatrix = glm::mat4(1.0f);
			modelMatrix = glm::translate(xform.m, glm::vec3(lineLoop[(i * 3) + 0], lineLoop[(i * 3) + 1], 0));
			debugLineShader->setMat4("model", modelMatrix);
			glDrawArrays(GL_POINTS, i, 1);
		}
	}
	if (drawBoundsLines)
	{
		debugLineShader->use();
		debugLineShader->setMat4("projection", xform.p);
		debugLineShader->setMat4("view", xform.v);
		
		glm::vec4 color = glm::vec4(blue.r, blue.g, blue.b, 1.0f);
		debugLineShader->setVec4("lineColor", color);
		glPointSize(4);
		glBindVertexArray(boundsVAO);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, boundsVBO);
		glm::quat rotation = glm::quat(glm::vec3(0, 0, glm::radians(fragData.transform.roll)));
		for (int i = 0; i < 5; i++)
		{
			modelMatrix = glm::mat4(1.0f);
			switch (i)
			{
			case 0:
				modelMatrix = glm::translate(xform.m, glm::vec3(0, 0, 0)); break;
			case 1:
				modelMatrix = glm::translate(xform.m, glm::vec3(0, 0, 0)); break;
			case 2:
				modelMatrix = glm::translate(xform.m, glm::vec3(0, 0, 0)); break;
			case 3:
				modelMatrix = glm::translate(xform.m, glm::vec3(0, 0, 0)); break;
			case 4:
			default:
				modelMatrix = glm::translate(xform.m, glm::vec3(0, 0, 0)); break;
			}
			debugLineShader->setMat4("model", modelMatrix);
			glDrawArrays(GL_POINTS, i, 1);
		}
	}
}
// Render to file
void Stroke::render(ShaderTransform xform, unsigned int targetBuffer)
{
	glBindFramebuffer(GL_FRAMEBUFFER, targetBuffer);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);

	if ((int)fragData.anchors.size() != 0 && (int)shards.size() != 0)
	{
		modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(xform.m, glm::vec3(compositePos));

		setCompositeShaderUniforms(xform);
		glBindVertexArray(compVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, compColorBuffer);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
//
// Debugging Tools
//
////////////////////////////////////////////////////////////////////////////////////////////////

void Stroke::initializeDebugData()
{
	// Testing for stroke opacity
	glGenVertexArrays(1, &quadVAO); glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glGenTextures(1, &frameColorbuffer);
	glBindTexture(GL_TEXTURE_2D, frameColorbuffer);
	strokeImageData = new float[(size_t)WINDOW_WIDTH * (size_t)WINDOW_HEIGHT * 4];
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGBA, GL_FLOAT, strokeImageData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frameColorbuffer, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Testing for stroke flow
	glGenVertexArrays(1, &tempVAO);
	glGenBuffers(1, &tempVBO);
	glBindVertexArray(tempVAO);
	glBindBuffer(GL_ARRAY_BUFFER, tempVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glGenFramebuffers(1, &tempbuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, tempbuffer);
	// create a color attachment texture
	glGenTextures(1, &tempColorbuffer);
	glBindTexture(GL_TEXTURE_2D, tempColorbuffer);
	tempImageData = new float[(size_t)WINDOW_WIDTH * (size_t)WINDOW_HEIGHT * 4];
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WINDOW_WIDTH, WINDOW_HEIGHT, 0, GL_RGBA, GL_FLOAT, tempImageData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tempColorbuffer, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Stroke::drawDebugData(ShaderTransform xform)
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindFramebuffer(GL_FRAMEBUFFER, tempbuffer);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//for (int i = 0; i < (int)shards.size(); i++)
	for (int i = 0; i < (int)shards.size(); i++)
	{
		StrokeShard* obj = &shards[i];
		// Draw the shard on it's own buffer texture
		glBindFramebuffer(GL_FRAMEBUFFER, tempbuffer);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBlendEquation(GL_MAX);
		// Later, a single shader should be used that handles all of this without needing to swap
		// to a different program
		shader->use();
		shader->setInt("texture1", 0);
		shader->setMat4("projection", xform.p);
		shader->setMat4("view", xform.v);
		glBindVertexArray(VAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, TEX0);

		modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(xform.m, obj->pos);
		modelMatrix = glm::rotate(modelMatrix, atan2(obj->dir.x, obj->dir.y), glm::vec3(0.0f, 0.0f, 1.0f));
		shader->setMat4("model", modelMatrix);
		// Drawing with opacity by pressure enabled
		glm::vec4 rgba = glm::vec4(obj->color.r, obj->color.g, obj->color.b, obj->opacity);
		shader->setVec4("rgba", rgba);
		// Flow with pressure disabled, set to constant
		// This is a flat reductive multiplier to the resulting opacity of the shard (brush tip)
		shader->setFloat("flow", 0.2f);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		// Bind the framebuffer(combined output) and draw the tempbuffer into it, calculating the alpha by hand, then using MAX to
		// get the correct 'opacity' effect.
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBlendEquation(GL_MAX);
		frameShader->use();
		frameShader->setInt("dstTex", 0);
		frameShader->setInt("srcTex", 1);
		frameShader->setBool("preMultiply", true);
		// Drawing with opacity by pressure enabled, this clamps the resulting alpha to the current pressure,
		// regardless of flow build up, ie. flow builds up the opacity to the current allowed max.
		frameShader->setFloat("opacity", obj->opacity);
		glBindVertexArray(quadVAO);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, frameColorbuffer);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, tempColorbuffer);
		glDrawArrays(GL_TRIANGLES, 0, 6);

	}
	// Now enable the default framebuffer, and blend the textureColorbuffer (framebuffer COLOR)
	// into it with normal blend func
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBlendEquation(GL_FUNC_ADD);
	frameShader->use();
	frameShader->setInt("dstTex", 0);
	// This is the global 'opacity' value for the stroke. The frameColorbuffer (combination of all brush shards)
	// has it's alpha re-scaled to this value as maximum.
	frameShader->setFloat("strokeOpacity", 1);
	frameShader->setBool("preMultiply", false);
	glBindVertexArray(quadVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, frameColorbuffer);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Stroke::setCompositeShaderUniforms(ShaderTransform xform)
{
	compositeShader->use();
	compositeShader->setInt("texture1", 0);
	compositeShader->setInt("texture2", 1);
	compositeShader->setMat4("projection", xform.p);
	compositeShader->setMat4("view", xform.v);
	compositeShader->setMat4("model", modelMatrix);
	compositeShader->setFloat("strokeOpacity", alpha.maxOpacity);
	// Set Effects Uniforms
	if (effects.isEnabled)
	{
		compositeShader->setInt("fx_count", effects.effectsCount);
		compositeShader->setFloat("fx_totalBlend", effects.totalBlend);
		compositeShader->setFloat("fx_totalMask_alphaCenter", effects.totalMask_alphaCenter);
		compositeShader->setFloat("fx_totalMask_alphaRange", effects.totalMask_alphaRange);
		compositeShader->setIntArray("fx_slots", 16, effects.effectsOrdering);

		compositeShader->setVec3("fx_avgDir", dir_average);
		compositeShader->setVec3("fx_dirPointA", dirIntersectA);
		compositeShader->setVec3("fx_dirPointB", dirIntersectB);
		glm::vec4 fillArea = glm::vec4(fillX.x, fillX.y, fillY.x, fillY.y);
		compositeShader->setVec4("fx_gen_area", fillArea);
		compositeShader->setFloat("fx_isPortrait", (isPortrait) ? 1.0f : 0.0f);

		if (effects.fill.isEnabled)
		{
			compositeShader->setBool("fx_doFill_byCenter", effects.fill.maskUsingCenter);
			compositeShader->setBool("fx_doFill_byAngle", effects.fill.maskUsingAngle);

			compositeShader->setFloat("fx_fill_XMask", (effects.fill.finalMaskX) ? 1.0f : 0.0f);
			compositeShader->setFloat("fx_fill_XMask_invert", (effects.fill.finalMaskXInvert) ? 1.0f : 0.0f);
			compositeShader->setFloat("fx_fill_YMask", (effects.fill.finalMaskY) ? 1.0f : 0.0f);
			compositeShader->setFloat("fx_fill_YMask_invert", (effects.fill.finalMaskYInvert) ? 1.0f : 0.0f);

			compositeShader->setFloat("fx_fill_XMaskVariance", effects.fill.maskX_variance);
			compositeShader->setFloat("fx_fill_YMaskVariance", effects.fill.maskY_variance);
			compositeShader->setFloat("fx_fill_XChunkSize", effects.fill.finalChunkSizeX);
			compositeShader->setFloat("fx_fill_YChunkSize", effects.fill.finalChunkSizeY);

			compositeShader->setFloat("fx_fill_angle", effects.fill.maskAngle);


			glm::vec2 fillCenter = glm::vec2(
				(center.x - fillX.x) + effects.fill.maskCenter_offsetX,
				(center.y - fillY.x) + effects.fill.maskCenter_offsetY);
			compositeShader->setVec2("fx_fill_center", fillCenter);
			compositeShader->setFloat("fx_fill_center_invert", (effects.fill.finalMaskCenterInvert) ? 1.0f : 0.0f);
			compositeShader->setFloat("fx_fill_combineCenter", (effects.fill.combineCenterMask) ? 1.0f : 0.0f);

			glm::vec4 fillColor = effects.fill.finalColor.makeVec4();
			compositeShader->setVec4("fx_fillColor", fillColor);
			int channel = effects.fill.getChannelMask();
			compositeShader->setInt("fx_fillChannelMask", channel);
		}
		if (effects.gradient.isEnabled)
		{
			compositeShader->setFloat("fx_gradient_mixAmount", effects.gradient.mixAmount);
		}
		if (effects.posterize.isEnabled)
		{
			compositeShader->setFloat("fx_poster_mixAmount", effects.posterize.mixAmount);
			compositeShader->setFloat("fx_poster_levels", effects.posterize.levels);
			float modifier = (effects.posterize.useValueRange) ? clampf((shardValue_max - shardValue_min), 0.1f, 1.0f) : 1.0f;
			compositeShader->setFloat("fx_poster_levelModifier", modifier);
			compositeShader->setFloat("fx_poster_gamma", effects.posterize.gamma);
		}
		if (effects.invert.isEnabled)
		{
			compositeShader->setFloat("fx_invert_mixAmount", effects.invert.mixAmount);
			compositeShader->setFloat("fx_invert_red", (float)effects.invert.invertRed);
			compositeShader->setFloat("fx_invert_green", (float)effects.invert.invertGreen);
			compositeShader->setFloat("fx_invert_blue", (float)effects.invert.invertBlue);
		}
		if (effects.threshold.isEnabled)
		{
			compositeShader->setFloat("fx_threshold_level", effects.threshold.level);
			glm::vec3 thresholdMixers = glm::vec3(effects.threshold.mixAmount, effects.threshold.mixLower, effects.threshold.mixUpper);
			compositeShader->setVec3("fx_threshold_mixAmount", thresholdMixers);
			glm::vec3 lowColor = effects.threshold.lowColor.makeVec3();
			glm::vec3 upperColor = effects.threshold.upperColor.makeVec3();
			compositeShader->setVec3("fx_threshold_lowColor", lowColor);
			compositeShader->setVec3("fx_threshold_upperColor", upperColor);
		}
		if (effects.brightContrast.isEnabled)
		{
			compositeShader->setFloat("fx_bright_mixAmount", effects.brightContrast.mixAmount);
			compositeShader->setFloat("fx_bright_brightness", effects.brightContrast.finalBrightness);
			compositeShader->setFloat("fx_bright_brightMix", effects.brightContrast.brightnessMix);
			compositeShader->setFloat("fx_bright_contrast", effects.brightContrast.finalContrast);
			compositeShader->setFloat("fx_bright_contrastMix", effects.brightContrast.contrastMix);
		}
		if (effects.hsv.isEnabled)
		{
			glm::vec3 hsv = glm::vec3(
				effects.hsv.finalHue, effects.hsv.finalSat,
				effects.hsv.finalVal);
			compositeShader->setVec3("fx_hsv_color", hsv);
			compositeShader->setFloat("fx_hsv_mixAmount", effects.hsv.mixAmount);
		}
		if (effects.power.isEnabled)
		{
			compositeShader->setFloat("fx_power_mixAmount", effects.power.mixAmount);
			glm::vec3 gammaChannels = glm::vec3(effects.power.gammaRed, effects.power.gammaGreen, effects.power.gammaBlue);
			compositeShader->setVec3("fx_power_gamma", gammaChannels);
		}
		if (effects.modulo.isEnabled)
		{
			compositeShader->setFloat("fx_modulo_mixAmount", effects.modulo.mixAmount);
			compositeShader->setFloat("fx_modulo_valueA", effects.modulo.valueA);
			compositeShader->setFloat("fx_modulo_valueB", effects.modulo.valueB);
			compositeShader->setFloat("fx_modulo_useX", (float)effects.modulo.useX);
			compositeShader->setFloat("fx_modulo_useY", (float)effects.modulo.useY);
			compositeShader->setFloat("fx_modulo_useDir", (float)effects.modulo.useDirection);
			glm::vec3 moduloXMix = glm::vec3(effects.modulo.XMixRed, effects.modulo.XMixGreen, effects.modulo.XMixBlue);
			compositeShader->setVec3("fx_modulo_XChannels", moduloXMix);
			glm::vec3 moduloYMix = glm::vec3(effects.modulo.YMixRed, effects.modulo.YMixGreen, effects.modulo.YMixBlue);
			compositeShader->setVec3("fx_modulo_YChannels", moduloYMix);
			glm::vec3 moduloDirMix = glm::vec3(effects.modulo.DirMixRed, effects.modulo.DirMixGreen, effects.modulo.DirMixBlue);
			compositeShader->setVec3("fx_modulo_DirChannels", moduloDirMix);
		}
		if (effects.blur.isEnabled)
		{
			glm::vec4 blurChannels = glm::vec4(
				effects.blur.mixRed, effects.blur.mixGreen,
				effects.blur.mixBlue, effects.blur.mixAlpha);
			compositeShader->setVec4("fx_blur_mixChannels", blurChannels);
			compositeShader->setFloat("fx_blur_mixAmount", effects.blur.mixAmount);
			compositeShader->setFloat("fx_blur_quality", effects.blur.quality);
			compositeShader->setFloat("fx_blur_direction", effects.blur.directions);
			compositeShader->setFloat("fx_blur_radius", effects.blur.radius);
		}
	}
	else
	{
		compositeShader->setInt("fx_count", 0);
		compositeShader->setFloat("fx_totalBlend", 0.0f);
		compositeShader->setFloat("fx_totalMask_alphaCenter", effects.totalMask_alphaCenter);
		compositeShader->setFloat("fx_totalMask_alphaRange", effects.totalMask_alphaRange);
		compositeShader->setIntArray("fx_slots", 16, effects.effectsOrdering);
		compositeShader->setBool("fx_doFill", false);
		compositeShader->setBool("fx_doGradient", false);
		compositeShader->setBool("fx_doThreshold", false);
		compositeShader->setBool("fx_doPosterize", false);
		compositeShader->setBool("fx_doInvert", false);
		compositeShader->setBool("fx_doBrightContrast", false);
		compositeShader->setBool("fx_doHSV", false);
	}
}