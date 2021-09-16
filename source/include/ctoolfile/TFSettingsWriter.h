#ifndef TFSETTINGSWRITER_H
#define TFSETTINGSWRITER_H

#include "ChromaToolFile.h"
#include "../Toolbox.h"
#include "../Tool.h"
#include "../ToolSettings.h"

#include <string>
#include <memory>
#include <iostream>
#include <vector>
#include <map>

class TFSettingsWriter
{
private:
	std::shared_ptr<ChromaToolFile> owner = nullptr;
public:
	// Constructor
	TFSettingsWriter(std::shared_ptr<ChromaToolFile> owner);
	// Funtions
	std::string writeSettings_fromInterest(std::shared_ptr<Tool> target, TSetType interest);
	std::string writeSetting_metaData(std::shared_ptr<Tool> target);
	std::string writeSetting_zoom(std::shared_ptr<Tool> target, TSet_Zoom* zoom);
	std::string writeSetting_rotate(std::shared_ptr<Tool> target, TSet_Rotate* rotate);
	std::string writeSetting_pan(std::shared_ptr<Tool> target, TSet_Pan* pan);
	std::string writeSetting_continuousControl(std::shared_ptr<Tool> target, TSet_ContinuousControl* continuous);
	std::string writeSetting_dragControl(std::shared_ptr<Tool> target, TSet_DragControl* drag);
	std::string writeSetting_onePointControl(std::shared_ptr<Tool> target, TSet_OnePointControl* onepoint);
	std::string writeSetting_twoPointControl(std::shared_ptr<Tool> target, TSet_TwoPointControl* twopoint);
	std::string writeSetting_threePointControl(std::shared_ptr<Tool> target, TSet_ThreePointControl* threepoint);
	std::string writeSetting_sampler(std::shared_ptr<Tool> target, TSet_Sampler* sampler);
	//
	std::string writeSetting_blend(std::shared_ptr<Tool> target, TSet_Blend* blend);
	std::string writeSetting_basic(std::shared_ptr<Tool> target, TSet_Basic* basic);
	std::string writeSetting_image(std::shared_ptr<Tool> target, TSet_Image* image);
	std::string writeSetting_character(std::shared_ptr<Tool> target, TSet_Character* character);
	std::string writeSetting_scatter(std::shared_ptr<Tool> target, TSet_Scatter* scatter);
	std::string writeSetting_color(std::shared_ptr<Tool> target, TSet_Color* color);
	std::string writeSetting_alpha(std::shared_ptr<Tool> target, TSet_Alpha* alpha);
	std::string writeSetting_distance(std::shared_ptr<Tool> target, TSet_Distance* distance);
	std::string writeSetting_mixing(std::shared_ptr<Tool> target, TSet_Mixing* mixing);
	std::string writeSetting_multiTip(std::shared_ptr<Tool> target, TSet_MultiTip* multiTip);
	std::string writeSetting_texture(std::shared_ptr<Tool> target, TSet_Texture* texture);
	std::string writeSetting_grain(std::shared_ptr<Tool> target, TSet_Grain* grain);
	std::string writeSetting_wetEdges(std::shared_ptr<Tool> target, TSet_WetEdges* wetEdges);
	std::string writeSetting_repeat(std::shared_ptr<Tool> target, TSet_Repeat* repeat);
	std::string writeSetting_reference(std::shared_ptr<Tool> target, TSet_Reference* reference);
	std::string writeSetting_smoothing(std::shared_ptr<Tool> target, TSet_Smoothing* smoothing);
	std::string writeSetting_antiAliasing(std::shared_ptr<Tool> target, TSet_AntiAliasing* antiAliasing);

	//
	std::string writeSetting_vector(std::shared_ptr<Tool> target, TSet_Vector* vector);
	std::string writeSetting_polygon(std::shared_ptr<Tool> target, TSet_Polygon* polygon);
	std::string writeSetting_polyLine(std::shared_ptr<Tool> target, TSet_PolyLine* polyLine);
	std::string writeSetting_mesh(std::shared_ptr<Tool> target, TSet_Mesh* mesh);
	std::string writeSetting_vortex(std::shared_ptr<Tool> target, TSet_Vortex* vortex);
	std::string writeSetting_fan(std::shared_ptr<Tool> target, TSet_Fan* fan);
	std::string writeSetting_rake(std::shared_ptr<Tool> target, TSet_Rake* rake);
	std::string writeSetting_gradient(std::shared_ptr<Tool> target, TSet_Gradient* gradient);
	std::string writeSetting_fill(std::shared_ptr<Tool> target, TSet_Fill* fill);
	std::string writeSetting_pattern(std::shared_ptr<Tool> target, TSet_Pattern* pattern);
	std::string writeSetting_field(std::shared_ptr<Tool> target, TSet_Field* field);
	std::string writeSetting_shader(std::shared_ptr<Tool> target, TSet_Shader* shader);
	std::string writeSetting_filter(std::shared_ptr<Tool> target, TSet_Filter* filter);

	//
	std::string writeSetting_fx_fill(std::shared_ptr<Tool> target, TSet_FX_Fill* fx_fill);
	std::string writeSetting_fx_gradient(std::shared_ptr<Tool> target, TSet_FX_Gradient* fx_gradient);
	std::string writeSetting_fx_posterize(std::shared_ptr<Tool> target, TSet_FX_Posterize* fx_posterize);
	std::string writeSetting_fx_invert(std::shared_ptr<Tool> target, TSet_FX_Invert* fx_invert);
	std::string writeSetting_fx_threshold(std::shared_ptr<Tool> target, TSet_FX_Threshold* fx_threshold);
	std::string writeSetting_fx_brightContrast(std::shared_ptr<Tool> target, TSet_FX_BrightContrast* fx_brightContrast);
	std::string writeSetting_fx_hsv(std::shared_ptr<Tool> target, TSet_FX_HSV* fx_hsv);
	std::string writeSetting_fx_power(std::shared_ptr<Tool> target, TSet_FX_Power* fx_power);
	std::string writeSetting_fx_modulo(std::shared_ptr<Tool> target, TSet_FX_Modulo* fx_modulo);
	std::string writeSetting_fx_blur(std::shared_ptr<Tool> target, TSet_FX_GaussianBlur* fx_blur);
	//
	std::string writeSetting_effects(std::shared_ptr<Tool> target, TSet_Effects* effects);
};
#endif // !TFSETTINGSWRITER_H
