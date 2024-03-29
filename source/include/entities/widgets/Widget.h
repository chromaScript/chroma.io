#ifndef WIDGET_H
#define WIDGET_H

#include "../VisualEntity.h"

#include "WidgetStyle.h"
#include "../../cscript/CObject.h"
#include "../../clayout/LEnums.h"
#include "../../cscript/CStmt.h"
#include "../../input/input.h"
#include "../../math/transform.h"

class CInterpreter;

#include <map>
#include <memory>
#include <deque>

class Widget : public VisualEntity, public std::enable_shared_from_this<Widget> // Might not need this, enabling for now
{
private:
protected:
	// Actual Dimensions
	// Use VisualEntity::transform for position/bounds/rotation
	bool invalid = false; // Used to override visibility/rendering when widgets exceed available space
	bool useBGColor = true;
	int texSizeX = 0;
	int texSizeY = 0;
	int sizeX = 0;
	int sizeY = 0;

	// State Variables
	bool isEntered = false;
	bool wasVisible = false;
	bool isBeingDragged = false;

public:
	// Public variables
	bool isRoot = false;
	std::string rootId = "";
	std::string _namespace = "";
	LTokenType type = LTokenType::NIL;
	std::vector<std::shared_ptr<Widget>> childWidgets;
	std::map<int, std::weak_ptr<Widget>> outsideBoundWidgets;
	std::weak_ptr<Widget> parentWidget;

	// State Variables
	// Warning: maxChildren does not currently do anything besides prevent creation of widget roots with mC = 0;
	int maxChildren = INT_MAX;
	bool isThisDirty = true;
	bool isBeingResized = false; // Might not need this variable
	bool canResize = false; // Might not need this variable
	bool popupIsBlocking = false;
	bool isActive = false;
	bool applyOverflowMask = false;
	// Hidden State Variables - Used for built-in widgets and other tracking. These have generic names, be careful editing them.
	int genIndexA = 0;
	int genIndexB = 0;
	//overflowBox is given as BottomLeft.xy, TopRight.xy
	std::weak_ptr<Widget> overflowTarget;
	glm::vec4 overflowBox = glm::vec4(0.0f);
	glm::vec4 overflowOffset = glm::vec4(0.0f);

	// Style Variables
	WidgetStyle style;
	WidgetStyle savedStyle;
	// Basic Attributes
	std::string idAttrib = "";
	std::string nameAttrib = "";
	std::filesystem::path imgAttrib = "";
	std::string valueAttrib = "";
	std::string innerContent = "";
	// Complex Attributes
	glm::ivec2 location = glm::ivec2(0, 0); // Absolute positioning within the widget's container
	std::vector<std::string> groupsAttribs;
	std::vector<std::string> classAttribs;
	bool isDraggable = false;
	bool isDroppable = false;
	std::string dragType = "";
	std::string dropType = "";
	// Map of callback bindings
	std::map<std::string, std::vector<std::shared_ptr<CStmt>>> callbackMap;

	// Constructors
	Widget(std::weak_ptr<WidgetStyle> style, std::shared_ptr<Shader> shader);

	// Build Widget
	void resetSize();
	glm::ivec2 setSizeByChildren();
	glm::ivec2 setSizeByParent();
	virtual glm::ivec2 getSizeByChildren() = 0;
	virtual glm::ivec2 getSizeByParent() = 0;
	virtual glm::ivec2 findWidgetLocation(std::shared_ptr<Widget> childWidget) = 0;
	int findAvailableWidth(std::shared_ptr<Widget> askingWidget);
	int findAvailableHeight(std::shared_ptr<Widget> askingWidget);
	void placeWidget();
	virtual void buildWidget() = 0;
	void checkOverflow();
	std::shared_ptr<Widget> duplicate(std::shared_ptr<Widget> prototypeRoot, std::shared_ptr<Widget> parent, std::string rootID, bool isRoot);

	// State Functions
	void setDirty() { isThisDirty = true; }
	void setClean() { isThisDirty = false; }
	bool getDirty() { return isThisDirty; }
	void beginResize() { isBeingResized = true; }
	void endResize() { isBeingResized = false; }
	bool getResizeState() { return isBeingResized; }
	// Relationship Functions
	// Note: Must call addChildWidget from the UI in order to fetch windowBounds. Calling from inside Widget should be
	// avoided anyways because widgets should only be added during application initialization
	int getMaxChildren() { return maxChildren; }
	void removeChildWidget() {} // Note: Not sure if this is really needed, but adding it as a reminder

	// Dimension Reporting
	// Use the virtual functions to allow customized behavior in how a widget reports it's dimensions & position
	int findMinX();
	int findMinY();
	int findMaxX();
	int findMaxY();
	std::filesystem::path findFontPath();
	glm::ivec2 getSize() 
	{ 
		return glm::ivec2(sizeX, sizeY); 
	}
	glm::ivec2 reportLocation();
	void setLocation(glm::ivec2 inLoc) { location = inLoc; }
	void setLocation(int x, int y) { location = glm::ivec2(x, y); }
	glm::ivec2 getLocation() { return location; }
	glm::ivec2 chainLocation();
	glm::vec2 getScreenLocation(int offsetX, int offsetY, bool createNDC);
	glm::ivec4 getColliderBox();
	glm::vec4 calculateOverflowMask();
	// Child Organization Functions
	glm::ivec2 horizontalArrangement(std::shared_ptr<Widget> childWidget);
	glm::ivec2 verticalArrangement(std::shared_ptr<Widget> childWidget);

	// Measurement & Data Set/Get
	void setBounds_Widget();
	void setVertData_Widget();

	// Child Collision Check
	void checkOutofBoundsWidgets(std::weak_ptr<Widget> root, glm::ivec4 rootBounds);

	// Click Functions
	bool mouseSweep(double x, double y);
	bool mouseHit(Input* dat, bool dragOnly, bool shouldFocus, 
		std::deque<Widget*>& clickStack, std::deque<Widget*>& focusStackbool, bool zIndexExists, unsigned int& maxZIndex);
	int selfHit(Input* dat, bool dragOnly, bool storeHitEvent, unsigned int& maxZIndex, bool ignoreCollision);
	int selfFocus(Input* dat, bool storeHitEvent, unsigned int& maxZIndex, bool ignoreCollision);
	bool selfBlur();
	bool mouseHover(Input* dat, std::deque<Widget*> &hoverStack, bool zIndexExists, unsigned int& maxZIndex);
	int selfHover(Input* dat, bool storeHitEvent, unsigned int& maxZIndex, bool ignoreCollision);
	bool selfLeave(Input* dat, unsigned int maxZIndex);
	bool selfDrag(Input* dat);
	bool selfDragend(Input* dat);
	bool checkPointCollision_self(glm::ivec2 point);
	bool checkPointCollision_other(glm::ivec2 point, glm::ivec4 bounds);
	int checkBoxCollision_complexSelf(glm::ivec4 testBox);
	int checkBoxCollision_complexOther(glm::ivec4 testBox);
	bool checkVisibility();

	// Parent/Child Get Functions
	std::weak_ptr<Widget> getRoot();
	std::vector<int> reportParentUEIDChain();
	std::weak_ptr<Widget> getChild_byID(std::string id);
	std::weak_ptr<Widget> getChild_byName(std::string id);
	void getChild_byClass(std::shared_ptr<std::vector<std::weak_ptr<Widget>>> bucket, std::string className, std::string idExclusion);
	void getChild_byType(std::shared_ptr<std::vector<std::weak_ptr<Widget>>> bucket, LTokenType type, std::string idExclusion);
	bool removeChild_byID(std::shared_ptr<CInterpreter> interpreter, std::string childID);
	bool removeChildren_byClass(std::shared_ptr<CInterpreter> interpreter, std::string className);
	bool deleteWidget();
	bool deleteSelf();
	virtual void clearData() = 0;


	// Property Get/Set Functions
	bool isInputType(LTokenType type);
	bool updateChildOrder(std::string idName, int moveNumber, bool ignoreHidden, bool asLiteral, bool setVisible);
	std::weak_ptr<Widget> addChildWidget(std::shared_ptr<CInterpreter> interpreter, 
		std::string protoID, std::string childID, std::vector<std::string> extraClasses, int moveNumber);
	bool resetProperty(std::shared_ptr<CInterpreter> interpreter, std::string name);
	bool setProperty(std::shared_ptr<CInterpreter> interpreter, std::string name, std::shared_ptr<CObject> value);
	bool setProperty_value(std::shared_ptr<CInterpreter> interpreter, std::string name, std::shared_ptr<CObject> value);
	bool setChildProperty(int switchType, std::shared_ptr<CInterpreter> interpreter, std::string id, std::string name, std::shared_ptr<CObject> value);
	std::shared_ptr<CObject> getProperty(std::shared_ptr<CInterpreter> interpreter, std::string name);

	// Render Functions
	virtual void render(ShaderTransform* xform, unsigned int targetBuffer) {};
	void notifyVisibilityChanged();
	virtual void draw(ShaderTransform* xform);
	virtual void drawSelf(ShaderTransform* xform) = 0;
};

#endif