#include "../include/ctoolfile/TFSettingsWriter.h"

class CFunction;
#include "../include/cscript/CInterpreter.h"
#include "../include/cscript/CObject.h"

#include "../include/tool/Toolbox.h"
#include "../include/tool/Tool.h"
#include "../include/tool/ToolSettings.h"
#include "../include/input/keys.h"

#include "../include/math/math.h"



#include <string>
#include <memory>
#include <iostream>
#include <vector>
#include <map>

TFSettingsWriter::TFSettingsWriter(std::shared_ptr<ChromaToolFile> owner)
{
	this->owner = owner;
}

std::string TFSettingsWriter::writeSettings_fromInterest(std::shared_ptr<Tool> target, TSetType interest)
{
	std::string output = "\n";
	switch (interest)
	{
	case TSetType::zoom:
		output = writeSetting_zoom(target, target.get()->getZoom()); break;
	case TSetType::pan:
		output = writeSetting_pan(target, target.get()->getPan()); break;
	case TSetType::rotate:
		output = writeSetting_rotate(target, target.get()->getRotate()); break;
	case TSetType::sampler:
		output = writeSetting_sampler(target, target.get()->getSampler()); break;
		//
		//
	case TSetType::continuous:
		output = writeSetting_continuousControl(target, target.get()->getContinuousControl()); break;
	case TSetType::drag:
		output = writeSetting_dragControl(target, target.get()->getDragControl()); break;
	case TSetType::onepoint:
		output = writeSetting_onePointControl(target, target.get()->getOnePointControl()); break;
	case TSetType::twopoint:
		output = writeSetting_twoPointControl(target, target.get()->getTwoPointControl()); break;
	case TSetType::threepoint:
		output = writeSetting_threePointControl(target, target.get()->getThreePointControl()); break;
		//
		//
	case TSetType::blend:
		output = writeSetting_blend(target, target.get()->getBlend()); break;
	case TSetType::basic:
		output = writeSetting_basic(target, target.get()->getBasic()); break;
	case TSetType::image:
		output = writeSetting_image(target, target.get()->getImage()); break;
	case TSetType::character:
		output = writeSetting_character(target, target.get()->getCharacter()); break;
	case TSetType::scatter:
		output = writeSetting_scatter(target, target.get()->getScatter()); break;
	case TSetType::color:
		output = writeSetting_color(target, target.get()->getColor()); break;
	case TSetType::alpha:
		output = writeSetting_alpha(target, target.get()->getAlpha()); break;
	//case TSetType::distance:
	//	output = writeSetting_distance(target, target.get()->getDistance()); break;
	//case TSetType::mixing:
	//	output = writeSetting_mixing(target, target.get()->getMixing()); break;
	//case TSetType::multiTip:
	//	output = writeSetting_multiTip(target, target.get()->getMultiTip()); break;
	//case TSetType::texture:
	//	output = writeSetting_texture(target, target.get()->getTexture()); break;
	//case TSetType::grain:
	//	output = writeSetting_grain(target, target.get()->getGrain()); break;
	//case TSetType::wetEdges:
	//	output = writeSetting_wetEdges(target, target.get()->getWetEdges()); break;
	//case TSetType::repeat:
	//	output = writeSetting_repeat(target, target.get()->getRepeat()); break;
	//case TSetType::reference:
	//	output = writeSetting_reference(target, target.get()->getReference()); break;
	case TSetType::smoothing:
		output = writeSetting_smoothing(target, target.get()->getSmoothing()); break;
	case TSetType::antiAliasing:
		output = writeSetting_antiAliasing(target, target.get()->getAntiAliasing()); break;
		//
		//
	case TSetType::vector:
		output = writeSetting_vector(target, target.get()->getVector()); break;
	case TSetType::polygon:
		output = writeSetting_polygon(target, target.get()->getPolygon()); break;
	case TSetType::polyline:
		output = writeSetting_polyLine(target, target.get()->getPolyLine()); break;
	//case TSetType::mesh:
	//	output = writeSetting_mesh(target, target.get()->getMesh()); break;
	case TSetType::vortex:
		output = writeSetting_vortex(target, target.get()->getVortex()); break;
	case TSetType::fan:
		output = writeSetting_fan(target, target.get()->getFan()); break;
	case TSetType::rake:
		output = writeSetting_rake(target, target.get()->getRake()); break;
	case TSetType::gradient:
		output = writeSetting_gradient(target, target.get()->getGradient()); break;
	case TSetType::fill:
		output = writeSetting_fill(target, target.get()->getFill()); break;
	case TSetType::pattern:
		output = writeSetting_pattern(target, target.get()->getPattern()); break;
	case TSetType::field:
		output = writeSetting_field(target, target.get()->getField()); break;
	case TSetType::shader:
		output = writeSetting_shader(target, target.get()->getShader()); break;
	case TSetType::filter:
		output = writeSetting_filter(target, target.get()->getFilter()); break;
		//
		//
	case TSetType::effects:
		output = writeSetting_effects(target, target.get()->getEffects()); break;
	default: break;

	}
	return output;
}

std::string TFSettingsWriter::writeSetting_metaData(std::shared_ptr<Tool> target)
{
	std::string output = "metadata \n{\n";
	output += std::get<std::string>(target->putMetaData(nullptr, 0, -1, nullptr, true, true).get()->obj);
	output += std::get<std::string>(target->putMetaData(nullptr, 1, -1, nullptr, true, true).get()->obj);
	output += std::get<std::string>(target->putMetaData(nullptr, 2, -1, nullptr, true, true).get()->obj);
	output += std::get<std::string>(target->putMetaData(nullptr, 3, -1, nullptr, true, true).get()->obj);
	output += std::get<std::string>(target->putMetaData(nullptr, 8, -1, nullptr, true, true).get()->obj);
	output += std::get<std::string>(target->putMetaData(nullptr, 15, -1, nullptr, true, true).get()->obj);
	output += std::get<std::string>(target->putMetaData(nullptr, 16, -1, nullptr, true, true).get()->obj);
	output += std::get<std::string>(target->putMetaData(nullptr, 17, -1, nullptr, true, true).get()->obj);
	output += std::get<std::string>(target->putMetaData(nullptr, 18, -1, nullptr, true, true).get()->obj);
	output += std::get<std::string>(target->putMetaData(nullptr, 19, -1, nullptr, true, true).get()->obj);
	output += std::get<std::string>(target->putMetaData(nullptr, 20, -1, nullptr, true, true).get()->obj);
	output += std::get<std::string>(target->putMetaData(nullptr, 21, -1, nullptr, true, true).get()->obj);
	output += std::get<std::string>(target->putMetaData(nullptr, 24, -1, nullptr, true, true).get()->obj);
	output += std::get<std::string>(target->putMetaData(nullptr, 25, -1, nullptr, true, true).get()->obj);
	output += std::get<std::string>(target->putMetaData(nullptr, 30, -1, nullptr, true, true).get()->obj);
	output += std::get<std::string>(target->putMetaData(nullptr, 31, -1, nullptr, true, true).get()->obj);
	output += std::get<std::string>(target->putMetaData(nullptr, 32, -1, nullptr, true, true).get()->obj);
	output += std::get<std::string>(target->putMetaData(nullptr, 41, -1, nullptr, true, true).get()->obj);
	output += std::get<std::string>(target->putMetaData(nullptr, 51, -1, nullptr, true, true).get()->obj);
	output += std::get<std::string>(target->putMetaData(nullptr, 52, -1, nullptr, true, true).get()->obj);
	output += "}\n\n";
	return output;
}
std::string TFSettingsWriter::writeSetting_zoom(std::shared_ptr<Tool> target, TSet_Zoom* zoom)
{
	std::string output = "zoom \n{\n";
	output += std::get<std::string>(zoom->putProperty(nullptr, target, 200, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(zoom->putProperty(nullptr, target, 201, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(zoom->putProperty(nullptr, target, 202, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(zoom->putProperty(nullptr, target, 203, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(zoom->putProperty(nullptr, target, 204, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(zoom->putProperty(nullptr, target, 205, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(zoom->putProperty(nullptr, target, 206, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(zoom->putProperty(nullptr, target, 210, -1, nullptr, true, false, true).get()->obj);
	output += "}\n\n";
	return output;
}
std::string TFSettingsWriter::writeSetting_rotate(std::shared_ptr<Tool> target, TSet_Rotate* rotate)
{
	std::string output = "rotate \n{\n";
	output += std::get<std::string>(rotate->putProperty(nullptr, target, 300, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(rotate->putProperty(nullptr, target, 301, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(rotate->putProperty(nullptr, target, 310, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(rotate->putProperty(nullptr, target, 311, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(rotate->putProperty(nullptr, target, 312, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(rotate->putProperty(nullptr, target, 313, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(rotate->putProperty(nullptr, target, 314, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(rotate->putProperty(nullptr, target, 315, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(rotate->putProperty(nullptr, target, 320, -1, nullptr, true, false, true).get()->obj);
	output += "}\n\n";
	return output;
}
std::string TFSettingsWriter::writeSetting_pan(std::shared_ptr<Tool> target, TSet_Pan* pan)
{
	std::string output = "pan \n{\n";
	output += std::get<std::string>(pan->putProperty(nullptr, target, 100, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(pan->putProperty(nullptr, target, 101, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(pan->putProperty(nullptr, target, 102, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(pan->putProperty(nullptr, target, 110, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(pan->putProperty(nullptr, target, 111, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(pan->putProperty(nullptr, target, 112, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(pan->putProperty(nullptr, target, 113, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(pan->putProperty(nullptr, target, 116, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(pan->putProperty(nullptr, target, 117, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(pan->putProperty(nullptr, target, 120, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(pan->putProperty(nullptr, target, 122, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(pan->putProperty(nullptr, target, 130, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(pan->putProperty(nullptr, target, 131, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(pan->putProperty(nullptr, target, 132, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(pan->putProperty(nullptr, target, 133, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(pan->putProperty(nullptr, target, 134, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(pan->putProperty(nullptr, target, 135, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(pan->putProperty(nullptr, target, 136, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(pan->putProperty(nullptr, target, 150, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(pan->putProperty(nullptr, target, 151, -1, nullptr, true, false, true).get()->obj);
	output += "}\n\n";
	return output;
}
std::string TFSettingsWriter::writeSetting_sampler(std::shared_ptr<Tool> target, TSet_Sampler* sampler)
{
	std::string output = "sampler \n{\n";
	output += std::get<std::string>(sampler->putProperty(nullptr, target, 900, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(sampler->putProperty(nullptr, target, 901, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(sampler->putProperty(nullptr, target, 902, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(sampler->putProperty(nullptr, target, 928, -1, nullptr, true, false, true).get()->obj);
	//
	output += makeSettingString(target, sampler->getControlNode(930, -1), 930, -1, sampler->getControlNode(930, -1)->nodeName);
	output += makeSettingString(target, sampler->getControlNode(931, -1), 931, -1, sampler->getControlNode(931, -1)->nodeName);
	output += makeSettingString(target, sampler->getControlNode(932, -1), 932, -1, sampler->getControlNode(932, -1)->nodeName);
	output += makeSettingString(target, sampler->getController(933, -1), 933, -1, "CONTROLLER");
	output += makeSettingString(target, sampler->getNoise(934, -1), 934, -1, sampler->getNoise(934, -1)->noiseName);
	output += makeSettingString(target, sampler->getNoise(935, -1), 935, -1, sampler->getNoise(935, -1)->noiseName);
	output += makeSettingString(target, sampler->getNoise(936, -1), 936, -1, sampler->getNoise(936, -1)->noiseName);
	output += std::get<std::string>(sampler->putProperty(nullptr, target, 929, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(sampler->putProperty(nullptr, target, 937, -1, nullptr, true, false, true).get()->obj);
	//
	output += makeSettingString(target, sampler->getControlNode(950, -1), 950, -1, sampler->getControlNode(950, -1)->nodeName);
	output += makeSettingString(target, sampler->getControlNode(951, -1), 951, -1, sampler->getControlNode(951, -1)->nodeName);
	output += makeSettingString(target, sampler->getControlNode(952, -1), 952, -1, sampler->getControlNode(952, -1)->nodeName);
	output += makeSettingString(target, sampler->getController(953, -1), 953, -1, "CONTROLLER");
	output += makeSettingString(target, sampler->getNoise(954, -1), 954, -1, sampler->getNoise(954, -1)->noiseName);
	output += makeSettingString(target, sampler->getNoise(955, -1), 955, -1, sampler->getNoise(955, -1)->noiseName);
	output += makeSettingString(target, sampler->getNoise(956, -1), 956, -1, sampler->getNoise(956, -1)->noiseName);
	output += std::get<std::string>(sampler->putProperty(nullptr, target, 940, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(sampler->putProperty(nullptr, target, 944, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(sampler->putProperty(nullptr, target, 957, -1, nullptr, true, false, true).get()->obj);
	//
	output += std::get<std::string>(sampler->putProperty(nullptr, target, 960, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(sampler->putProperty(nullptr, target, 961, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(sampler->putProperty(nullptr, target, 962, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(sampler->putProperty(nullptr, target, 963, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(sampler->putProperty(nullptr, target, 964, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(sampler->putProperty(nullptr, target, 965, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(sampler->putProperty(nullptr, target, 966, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(sampler->putProperty(nullptr, target, 967, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(sampler->putProperty(nullptr, target, 968, -1, nullptr, true, false, true).get()->obj);
	output += "}\n\n";
	return output;
}

//
std::string TFSettingsWriter::writeSetting_continuousControl(std::shared_ptr<Tool> target, TSet_ContinuousControl* continuous)
{
	std::string output = "continuous \n{\n";
	output += std::get<std::string>(continuous->putProperty(nullptr, target, 400, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(continuous->putProperty(nullptr, target, 401, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(continuous->putProperty(nullptr, target, 402, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(continuous->putProperty(nullptr, target, 403, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(continuous->putProperty(nullptr, target, 404, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(continuous->putProperty(nullptr, target, 405, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(continuous->putProperty(nullptr, target, 406, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(continuous->putProperty(nullptr, target, 407, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(continuous->putProperty(nullptr, target, 408, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(continuous->putProperty(nullptr, target, 410, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(continuous->putProperty(nullptr, target, 411, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(continuous->putProperty(nullptr, target, 412, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(continuous->putProperty(nullptr, target, 413, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(continuous->putProperty(nullptr, target, 414, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(continuous->putProperty(nullptr, target, 415, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(continuous->putProperty(nullptr, target, 416, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(continuous->putProperty(nullptr, target, 417, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(continuous->putProperty(nullptr, target, 430, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(continuous->putProperty(nullptr, target, 431, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(continuous->putProperty(nullptr, target, 432, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(continuous->putProperty(nullptr, target, 433, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(continuous->putProperty(nullptr, target, 435, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(continuous->putProperty(nullptr, target, 436, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(continuous->putProperty(nullptr, target, 437, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(continuous->putProperty(nullptr, target, 440, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(continuous->putProperty(nullptr, target, 441, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(continuous->putProperty(nullptr, target, 442, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(continuous->putProperty(nullptr, target, 445, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(continuous->putProperty(nullptr, target, 446, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(continuous->putProperty(nullptr, target, 447, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(continuous->putProperty(nullptr, target, 450, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(continuous->putProperty(nullptr, target, 451, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(continuous->putProperty(nullptr, target, 452, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(continuous->putProperty(nullptr, target, 453, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(continuous->putProperty(nullptr, target, 454, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(continuous->putProperty(nullptr, target, 455, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(continuous->putProperty(nullptr, target, 456, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(continuous->putProperty(nullptr, target, 457, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(continuous->putProperty(nullptr, target, 458, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(continuous->putProperty(nullptr, target, 459, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(continuous->putProperty(nullptr, target, 460, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(continuous->putProperty(nullptr, target, 461, -1, nullptr, true, false, true).get()->obj);
	output += "}\n\n";
	return output;
}
std::string TFSettingsWriter::writeSetting_dragControl(std::shared_ptr<Tool> target, TSet_DragControl* drag)
{
	std::string output = "drag \n{\n";

	output += "}\n\n";
	return output;
}

std::string TFSettingsWriter::writeSetting_onePointControl(std::shared_ptr<Tool> target, TSet_OnePointControl* onepoint)
{
	std::string output = "onepoint \n{\n";

	output += "}\n\n";
	return output;
}
std::string TFSettingsWriter::writeSetting_twoPointControl(std::shared_ptr<Tool> target, TSet_TwoPointControl* twopoint)
{
	std::string output = "twopoint \n{\n";

	output += "}\n\n";
	return output;
}
std::string TFSettingsWriter::writeSetting_threePointControl(std::shared_ptr<Tool> target, TSet_ThreePointControl* threepoint)
{
	std::string output = "threepoint \n{\n";

	output += "}\n\n";
	return output;
}

//
std::string TFSettingsWriter::writeSetting_blend(std::shared_ptr<Tool> target, TSet_Blend* blend)
{
	std::string output = "blend \n{\n";
	output += makeSettingString(blend->isEnabled, -1, -1, "isEnabled");
	output += "}\n\n";
	return output;
}
std::string TFSettingsWriter::writeSetting_basic(std::shared_ptr<Tool> target, TSet_Basic* basic)
{
	std::string output = "basic \n{\n";
	output += makeSettingString(basic->isEnabled, -1, -1, "isEnabled");
	output += "}\n\n";
	return output;
}
std::string TFSettingsWriter::writeSetting_image(std::shared_ptr<Tool> target, TSet_Image* image)
{
	std::string output = "image \n{\n";
	output += makeSettingString(image->isEnabled, -1, -1, "isEnabled");
	output += std::get<std::string>(image->putProperty(nullptr, target, 2201, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(image->putProperty(nullptr, target, 2202, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(image->putProperty(nullptr, target, 2203, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(image->putProperty(nullptr, target, 2205, -1, nullptr, true, false, true).get()->obj);
	output += "}\n\n";
	return output;
}
std::string TFSettingsWriter::writeSetting_character(std::shared_ptr<Tool> target, TSet_Character* character)
{
	std::string output = "character \n{\n";
	output += makeSettingString(character->isEnabled, -1, -1, "isEnabled");
	output += makeSettingString(target, character->getControlNode(2300, -1), 2300, -1, character->getControlNode(2300, -1)->nodeName);
	output += makeSettingString(target, character->getController(2301, -1), 2301, -1, "CONTROLLER");
	output += makeSettingString(target, character->getNoise(2302, -1), 2302, -1, character->getNoise(2302, -1)->noiseName);
	//
	output += makeSettingString(target, character->getControlNode(2303, -1), 2303, -1, character->getControlNode(2303, -1)->nodeName);
	output += makeSettingString(target, character->getController(2304, -1), 2304, -1, "CONTROLLER");
	output += makeSettingString(target, character->getNoise(2305, -1), 2305, -1, character->getNoise(2305, -1)->noiseName);
	//
	output += makeSettingString(target, character->getControlNode(2306, -1), 2306, -1, character->getControlNode(2306, -1)->nodeName);
	output += makeSettingString(target, character->getController(2307, -1), 2307, -1, "CONTROLLER");
	output += makeSettingString(target, character->getNoise(2308, -1), 2308, -1, character->getNoise(2308, -1)->noiseName);
	//
	output += makeSettingString(target, character->getControlNode(2320, -1), 2320, -1, character->getControlNode(2320, -1)->nodeName);
	output += makeSettingString(target, character->getController(2321, -1), 2321, -1, "CONTROLLER");
	output += makeSettingString(target, character->getNoise(2322, -1), 2322, -1, character->getNoise(2322, -1)->noiseName);
	output += std::get<std::string>(character->putProperty(nullptr, target, 2323, -1, nullptr, true, false, true).get()->obj);
	//
	output += makeSettingString(target, character->getControlNode(2330, -1), 2330, -1, character->getControlNode(2330, -1)->nodeName);
	output += makeSettingString(target, character->getController(2331, -1), 2331, -1, "CONTROLLER");
	output += makeSettingString(target, character->getNoise(2332, -1), 2332, -1, character->getNoise(2332, -1)->noiseName);
	//
	output += makeSettingString(target, character->getControlNode(2333, -1), 2333, -1, character->getControlNode(2333, -1)->nodeName);
	output += makeSettingString(target, character->getController(2334, -1), 2334, -1, "CONTROLLER");
	output += makeSettingString(target, character->getNoise(2335, -1), 2335, -1, character->getNoise(2335, -1)->noiseName);
	//
	output += makeSettingString(target, character->getControlNode(2340, -1), 2340, -1, character->getControlNode(2340, -1)->nodeName);
	output += makeSettingString(target, character->getController(2341, -1), 2341, -1, "CONTROLLER");
	output += makeSettingString(target, character->getNoise(2342, -1), 2342, -1, character->getNoise(2342, -1)->noiseName);
	output += std::get<std::string>(character->putProperty(nullptr, target, 2343, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(character->putProperty(nullptr, target, 2344, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(character->putProperty(nullptr, target, 2345, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(character->putProperty(nullptr, target, 2346, -1, nullptr, true, false, true).get()->obj);
	//
	output += makeSettingString(target, character->getControlNode(2350, -1), 2350, -1, character->getControlNode(2350, -1)->nodeName);
	output += makeSettingString(target, character->getController(2351, -1), 2351, -1, "CONTROLLER");
	output += makeSettingString(target, character->getNoise(2352, -1), 2352, -1, character->getNoise(2352, -1)->noiseName);
	output += std::get<std::string>(character->putProperty(nullptr, target, 2353, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(character->putProperty(nullptr, target, 2354, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(character->putProperty(nullptr, target, 2355, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(character->putProperty(nullptr, target, 2356, -1, nullptr, true, false, true).get()->obj);
	//
	output += makeSettingString(target, character->getControlNode(2360, -1), 2360, -1, character->getControlNode(2360, -1)->nodeName);
	output += makeSettingString(target, character->getController(2361, -1), 2361, -1, "CONTROLLER");
	output += makeSettingString(target, character->getNoise(2362, -1), 2362, -1, character->getNoise(2362, -1)->noiseName);
	output += std::get<std::string>(character->putProperty(nullptr, target, 2363, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(character->putProperty(nullptr, target, 2364, -1, nullptr, true, false, true).get()->obj);
	//
	output += std::get<std::string>(character->putProperty(nullptr, target, 2380, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(character->putProperty(nullptr, target, 2381, -1, nullptr, true, false, true).get()->obj);
	output += "}\n\n";
	return output;
}
std::string TFSettingsWriter::writeSetting_scatter(std::shared_ptr<Tool> target, TSet_Scatter* scatter)
{
	std::string output = "scatter \n{\n";
	output += makeSettingString(scatter->isEnabled, -1, -1, "isEnabled");
	output += makeSettingString(target, scatter->getControlNode(2600, -1), 2600, -1, scatter->getControlNode(2600, -1)->nodeName);
	output += makeSettingString(target, scatter->getController(2601, -1), 2601, -1, "CONTROLLER");
	output += makeSettingString(target, scatter->getNoise(2602, -1), 2602, -1, scatter->getNoise(2602, -1)->noiseName);
	//
	output += makeSettingString(target, scatter->getControlNode(2610, -1), 2610, -1, scatter->getControlNode(2610, -1)->nodeName);
	output += makeSettingString(target, scatter->getController(2611, -1), 2611, -1, "CONTROLLER");
	output += makeSettingString(target, scatter->getNoise(2612, -1), 2612, -1, scatter->getNoise(2612, -1)->noiseName);
	//
	output += makeSettingString(target, scatter->getControlNode(2620, -1), 2620, -1, scatter->getControlNode(2620, -1)->nodeName);
	output += makeSettingString(target, scatter->getController(2621, -1), 2621, -1, "CONTROLLER");
	output += makeSettingString(target, scatter->getNoise(2622, -1), 2622, -1, scatter->getNoise(2622, -1)->noiseName);
	//
	output += makeSettingString(target, scatter->getControlNode(2630, -1), 2630, -1, scatter->getControlNode(2630, -1)->nodeName);
	output += makeSettingString(target, scatter->getController(2631, -1), 2631, -1, "CONTROLLER");
	output += makeSettingString(target, scatter->getNoise(2632, -1), 2632, -1, scatter->getNoise(2632, -1)->noiseName);
	//
	output += std::get<std::string>(scatter->putProperty(nullptr, target, 2660, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(scatter->putProperty(nullptr, target, 2661, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(scatter->putProperty(nullptr, target, 2662, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(scatter->putProperty(nullptr, target, 2663, -1, nullptr, true, false, true).get()->obj);
	output += "}\n\n";
	return output;
}
std::string TFSettingsWriter::writeSetting_color(std::shared_ptr<Tool> target, TSet_Color* color)
{
	std::string output = "color \n{\n";
	output += makeSettingString(color->isEnabled, -1, -1, "isEnabled");
	output += makeSettingString(target, color->getControlNode(2700, -1), 2700, -1, color->getControlNode(2700, -1)->nodeName);
	output += makeSettingString(target, color->getController(2701, -1), 2701, -1, "CONTROLLER");
	output += makeSettingString(target, color->getNoise(2702, -1), 2702, -1, color->getNoise(2702, -1)->noiseName);
	output += std::get<std::string>(color->putProperty(nullptr, target, 2703, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(color->putProperty(nullptr, target, 2706, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(color->putProperty(nullptr, target, 2707, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(color->putProperty(nullptr, target, 2708, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(color->putProperty(nullptr, target, 2709, -1, nullptr, true, false, true).get()->obj);
	//
	output += makeSettingString(target, color->getControlNode(2720, -1), 2720, -1, color->getControlNode(2720, -1)->nodeName);
	output += makeSettingString(target, color->getControlNode(2721, -1), 2721, -1, color->getControlNode(2721, -1)->nodeName);
	output += makeSettingString(target, color->getControlNode(2722, -1), 2722, -1, color->getControlNode(2722, -1)->nodeName);
	output += makeSettingString(target, color->getController(2723, -1), 2723, -1, "CONTROLLER");
	output += makeSettingString(target, color->getNoise(2724, -1), 2724, -1, color->getNoise(2724, -1)->noiseName);
	output += makeSettingString(target, color->getNoise(2725, -1), 2725, -1, color->getNoise(2725, -1)->noiseName);
	output += makeSettingString(target, color->getNoise(2726, -1), 2726, -1, color->getNoise(2726, -1)->noiseName);
	output += std::get<std::string>(color->putProperty(nullptr, target, 2727, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(color->putProperty(nullptr, target, 2728, -1, nullptr, true, false, true).get()->obj);
	//
	output += makeSettingString(target, color->getControlNode(2770, -1), 2770, -1, color->getControlNode(2770, -1)->nodeName);
	output += makeSettingString(target, color->getControlNode(2771, -1), 2771, -1, color->getControlNode(2771, -1)->nodeName);
	output += makeSettingString(target, color->getControlNode(2772, -1), 2772, -1, color->getControlNode(2772, -1)->nodeName);
	output += makeSettingString(target, color->getController(2773, -1), 2773, -1, "CONTROLLER");
	output += makeSettingString(target, color->getNoise(2774, -1), 2774, -1, color->getNoise(2774, -1)->noiseName);
	output += makeSettingString(target, color->getNoise(2775, -1), 2775, -1, color->getNoise(2775, -1)->noiseName);
	output += makeSettingString(target, color->getNoise(2776, -1), 2776, -1, color->getNoise(2776, -1)->noiseName);
	output += std::get<std::string>(color->putProperty(nullptr, target, 2777, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(color->putProperty(nullptr, target, 2778, -1, nullptr, true, false, true).get()->obj);
	//
	output += "}\n\n";
	return output;
}
std::string TFSettingsWriter::writeSetting_alpha(std::shared_ptr<Tool> target, TSet_Alpha* alpha)
{
	std::string output = "alpha \n{\n";
	output += makeSettingString(alpha->isEnabled, -1, -1, "isEnabled");
	output += makeSettingString(target, alpha->getControlNode(2800, -1), 2800, -1, alpha->getControlNode(2800, -1)->nodeName);
	output += makeSettingString(target, alpha->getController(2801, -1), 2801, -1, "CONTROLLER");
	output += makeSettingString(target, alpha->getNoise(2802, -1), 2802, -1, alpha->getNoise(2802, -1)->noiseName);
	output += makeSettingString(target, alpha->getControlNode(2803, -1), 2803, -1, alpha->getControlNode(2803, -1)->nodeName);
	output += makeSettingString(target, alpha->getController(2804, -1), 2804, -1, "CONTROLLER");
	output += makeSettingString(target, alpha->getNoise(2805, -1), 2805, -1, alpha->getNoise(2805, -1)->noiseName);
	output += "}\n\n";
	return output;
}
std::string TFSettingsWriter::writeSetting_distance(std::shared_ptr<Tool> target, TSet_Distance* distance)
{
	std::string output = "distance \n{\n";
	output += makeSettingString(distance->isEnabled, -1, -1, "isEnabled");
	output += "}\n\n";
	return output;
}
std::string TFSettingsWriter::writeSetting_mixing(std::shared_ptr<Tool> target, TSet_Mixing* mixing)
{
	std::string output = "mixing \n{\n";
	output += makeSettingString(mixing->isEnabled, -1, -1, "isEnabled");
	output += "}\n\n";
	return output;
}
std::string TFSettingsWriter::writeSetting_multiTip(std::shared_ptr<Tool> target, TSet_MultiTip* multiTip)
{
	std::string output = "multiTip \n{\n";
	output += makeSettingString(multiTip->isEnabled, -1, -1, "isEnabled");
	output += "}\n\n";
	return output;
}
std::string TFSettingsWriter::writeSetting_texture(std::shared_ptr<Tool> target, TSet_Texture* texture)
{
	std::string output = "texture \n{\n";
	output += makeSettingString(texture->isEnabled, -1, -1, "isEnabled");
	output += "}\n\n";
	return output;
}
std::string TFSettingsWriter::writeSetting_grain(std::shared_ptr<Tool> target, TSet_Grain* grain)
{
	std::string output = "grain \n{\n";
	output += makeSettingString(grain->isEnabled, -1, -1, "isEnabled");
	output += "}\n\n";
	return output;
}
std::string TFSettingsWriter::writeSetting_wetEdges(std::shared_ptr<Tool> target, TSet_WetEdges* wetEdges)
{
	std::string output = "wetEdges \n{\n";
	output += makeSettingString(wetEdges->isEnabled, -1, -1, "isEnabled");
	output += "}\n\n";
	return output;
}
std::string TFSettingsWriter::writeSetting_repeat(std::shared_ptr<Tool> target, TSet_Repeat* repeat)
{
	std::string output = "repeat \n{\n";
	output += makeSettingString(repeat->isEnabled, -1, -1, "isEnabled");
	output += "}\n\n";
	return output;
}
std::string TFSettingsWriter::writeSetting_reference(std::shared_ptr<Tool> target, TSet_Reference* reference)
{
	std::string output = "reference \n{\n";
	output += makeSettingString(reference->isEnabled, -1, -1, "isEnabled");
	output += "}\n\n";
	return output;
}
std::string TFSettingsWriter::writeSetting_smoothing(std::shared_ptr<Tool> target, TSet_Smoothing* smoothing)
{
	std::string output = "smoothing \n{\n";
	output += makeSettingString(smoothing->isEnabled, -1, -1, "isEnabled");
	output += "}\n\n";
	return output;
}
std::string TFSettingsWriter::writeSetting_antiAliasing(std::shared_ptr<Tool> target, TSet_AntiAliasing* antiAliasing)
{
	std::string output = "antiAliasing \n{\n";
	output += makeSettingString(antiAliasing->isEnabled, -1, -1, "isEnabled");
	output += "}\n\n";
	return output;
}

//
std::string TFSettingsWriter::writeSetting_vector(std::shared_ptr<Tool> target, TSet_Vector* vector)
{
	std::string output = "vector \n{\n";
	output += makeSettingString(vector->isEnabled, -1, -1, "isEnabled");
	output += "}\n\n";
	return output;
}
std::string TFSettingsWriter::writeSetting_polygon(std::shared_ptr<Tool> target, TSet_Polygon* polygon)
{
	std::string output = "polygon \n{\n";
	output += makeSettingString(polygon->isEnabled, -1, -1, "isEnabled");
	output += "}\n\n";
	return output;
}
std::string TFSettingsWriter::writeSetting_polyLine(std::shared_ptr<Tool> target, TSet_PolyLine* polyLine)
{
	std::string output = "polyLine \n{\n";
	output += makeSettingString(polyLine->isEnabled, -1, -1, "isEnabled");
	output += "}\n\n";
	return output;
}
std::string TFSettingsWriter::writeSetting_mesh(std::shared_ptr<Tool> target, TSet_Mesh* mesh)
{
	std::string output = "mesh \n{\n";
	output += makeSettingString(mesh->isEnabled, -1, -1, "isEnabled");
	output += "}\n\n";
	return output;
}
std::string TFSettingsWriter::writeSetting_vortex(std::shared_ptr<Tool> target, TSet_Vortex* vortex)
{
	std::string output = "vortex \n{\n";
	output += makeSettingString(vortex->isEnabled, -1, -1, "isEnabled");
	output += "}\n\n";
	return output;
}
std::string TFSettingsWriter::writeSetting_fan(std::shared_ptr<Tool> target, TSet_Fan* fan)
{
	std::string output = "fan \n{\n";
	output += makeSettingString(fan->isEnabled, -1, -1, "isEnabled");
	output += "}\n\n";
	return output;
}
std::string TFSettingsWriter::writeSetting_rake(std::shared_ptr<Tool> target, TSet_Rake* rake)
{
	std::string output = "rake \n{\n";
	output += makeSettingString(rake->isEnabled, -1, -1, "isEnabled");
	output += std::get<std::string>(rake->putProperty(nullptr, target, 5600, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(rake->putProperty(nullptr, target, 5601, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(rake->putProperty(nullptr, target, 5602, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(rake->putProperty(nullptr, target, 5603, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(rake->putProperty(nullptr, target, 5604, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(rake->putProperty(nullptr, target, 5605, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(rake->putProperty(nullptr, target, 5606, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(rake->putProperty(nullptr, target, 5607, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(rake->putProperty(nullptr, target, 5608, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(rake->putProperty(nullptr, target, 5609, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(rake->putProperty(nullptr, target, 5610, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(rake->putProperty(nullptr, target, 5611, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(rake->putProperty(nullptr, target, 5612, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(rake->putProperty(nullptr, target, 5613, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(rake->putProperty(nullptr, target, 5614, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(rake->putProperty(nullptr, target, 5615, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(rake->putProperty(nullptr, target, 5616, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(rake->putProperty(nullptr, target, 5617, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(rake->putProperty(nullptr, target, 5618, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(rake->putProperty(nullptr, target, 5619, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(rake->putProperty(nullptr, target, 5620, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(rake->putProperty(nullptr, target, 5630, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(rake->putProperty(nullptr, target, 5631, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(rake->putProperty(nullptr, target, 5632, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(rake->putProperty(nullptr, target, 5635, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(rake->putProperty(nullptr, target, 5640, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(rake->putProperty(nullptr, target, 5641, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(rake->putProperty(nullptr, target, 5642, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(rake->putProperty(nullptr, target, 5643, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(rake->putProperty(nullptr, target, 5644, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(rake->putProperty(nullptr, target, 5645, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(rake->putProperty(nullptr, target, 5650, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(rake->putProperty(nullptr, target, 5651, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(rake->putProperty(nullptr, target, 5652, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(rake->putProperty(nullptr, target, 5653, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(rake->putProperty(nullptr, target, 5654, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(rake->putProperty(nullptr, target, 5655, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(rake->putProperty(nullptr, target, 5656, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(rake->putProperty(nullptr, target, 5657, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(rake->putProperty(nullptr, target, 5658, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(rake->putProperty(nullptr, target, 5659, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(rake->putProperty(nullptr, target, 5660, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(rake->putProperty(nullptr, target, 5661, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(rake->putProperty(nullptr, target, 5662, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(rake->putProperty(nullptr, target, 5670, -1, nullptr, true, false, true).get()->obj);
	output += "}\n\n";
	return output;
}
std::string TFSettingsWriter::writeSetting_gradient(std::shared_ptr<Tool> target, TSet_Gradient* gradient)
{
	std::string output = "gradient \n{\n";
	output += makeSettingString(gradient->isEnabled, -1, -1, "isEnabled");
	output += "}\n\n";
	return output;
}
std::string TFSettingsWriter::writeSetting_fill(std::shared_ptr<Tool> target, TSet_Fill* fill)
{
	std::string output = "fill \n{\n";
	output += makeSettingString(fill->isEnabled, -1, -1, "isEnabled");
	output += "}\n\n";
	return output;
}
std::string TFSettingsWriter::writeSetting_pattern(std::shared_ptr<Tool> target, TSet_Pattern* pattern)
{
	std::string output = "pattern \n{\n";
	output += makeSettingString(pattern->isEnabled, -1, -1, "isEnabled");
	output += "}\n\n";
	return output;
}
std::string TFSettingsWriter::writeSetting_field(std::shared_ptr<Tool> target, TSet_Field* field)
{
	std::string output = "field \n{\n";
	output += makeSettingString(field->isEnabled, -1, -1, "isEnabled");
	output += "}\n\n";
	return output;
}
std::string TFSettingsWriter::writeSetting_shader(std::shared_ptr<Tool> target, TSet_Shader* shader)
{
	std::string output = "shader \n{\n";
	output += makeSettingString(shader->isEnabled, -1, -1, "isEnabled");
	output += "}\n\n";
	return output;
}
std::string TFSettingsWriter::writeSetting_filter(std::shared_ptr<Tool> target, TSet_Filter* filter)
{
	std::string output = "filter \n{\n";
	output += makeSettingString(filter->isEnabled, -1, -1, "isEnabled");
	output += "}\n\n";
	return output;
}
//
std::string TFSettingsWriter::writeSetting_fx_fill(std::shared_ptr<Tool> target, TSet_FX_Fill* fx_fill)
{
	std::string output = "fx_fill \n{\n";
	//output += makeSettingString(fx_fill->isEnabled, -1, -1, "isEnabled");

	output += "}\n\n";
	return output;
}
std::string TFSettingsWriter::writeSetting_fx_gradient(std::shared_ptr<Tool> target, TSet_FX_Gradient* fx_gradient)
{
	std::string output = "fx_gradient \n{\n";
	//output += makeSettingString(fx_gradient->isEnabled, -1, -1, "isEnabled");
	output += "}\n\n";
	return output;
}
std::string TFSettingsWriter::writeSetting_fx_posterize(std::shared_ptr<Tool> target, TSet_FX_Posterize* fx_posterize)
{
	std::string output = "fx_posterize \n{\n";
	//output += makeSettingString(fx_posterize->isEnabled, -1, -1, "isEnabled");
	output += std::get<std::string>(fx_posterize->putProperty(nullptr, target, 8305, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(fx_posterize->putProperty(nullptr, target, 8310, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(fx_posterize->putProperty(nullptr, target, 8311, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(fx_posterize->putProperty(nullptr, target, 8312, -1, nullptr, true, false, true).get()->obj);
	output += "}\n\n";
	return output;
}
std::string TFSettingsWriter::writeSetting_fx_invert(std::shared_ptr<Tool> target, TSet_FX_Invert* fx_invert)
{
	std::string output = "fx_invert \n{\n";
	//output += makeSettingString(fx_invert->isEnabled, -1, -1, "isEnabled");
	output += std::get<std::string>(fx_invert->putProperty(nullptr, target, 8405, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(fx_invert->putProperty(nullptr, target, 8410, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(fx_invert->putProperty(nullptr, target, 8411, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(fx_invert->putProperty(nullptr, target, 8412, -1, nullptr, true, false, true).get()->obj);
	output += "}\n\n";
	return output;
}
std::string TFSettingsWriter::writeSetting_fx_threshold(std::shared_ptr<Tool> target, TSet_FX_Threshold* fx_threshold)
{
	std::string output = "fx_threshold \n{\n";

	output += "}\n\n";
	return output;
}
std::string TFSettingsWriter::writeSetting_fx_brightContrast(std::shared_ptr<Tool> target, TSet_FX_BrightContrast* fx_brightContrast)
{
	std::string output = "fx_brightContrast \n{\n";
	output += std::get<std::string>(fx_brightContrast->putProperty(nullptr, target, 8605, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(fx_brightContrast->putProperty(nullptr, target, 8610, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(fx_brightContrast->putProperty(nullptr, target, 8611, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(fx_brightContrast->putProperty(nullptr, target, 8612, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(fx_brightContrast->putProperty(nullptr, target, 8613, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(fx_brightContrast->putProperty(nullptr, target, 8614, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(fx_brightContrast->putProperty(nullptr, target, 8615, -1, nullptr, true, false, true).get()->obj);
	output += "}\n\n";
	return output;
}
std::string TFSettingsWriter::writeSetting_fx_hsv(std::shared_ptr<Tool> target, TSet_FX_HSV* fx_hsv)
{
	std::string output = "fx_hsv \n{\n";
	output += std::get<std::string>(fx_hsv->putProperty(nullptr, target, 8705, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(fx_hsv->putProperty(nullptr, target, 8710, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(fx_hsv->putProperty(nullptr, target, 8711, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(fx_hsv->putProperty(nullptr, target, 8712, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(fx_hsv->putProperty(nullptr, target, 8713, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(fx_hsv->putProperty(nullptr, target, 8714, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(fx_hsv->putProperty(nullptr, target, 8715, -1, nullptr, true, false, true).get()->obj);
	output += "}\n\n";
	return output;
}
std::string TFSettingsWriter::writeSetting_fx_power(std::shared_ptr<Tool> target, TSet_FX_Power* fx_power)
{
	std::string output = "fx_power \n{\n";
	output += std::get<std::string>(fx_power->putProperty(nullptr, target, 9005, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(fx_power->putProperty(nullptr, target, 9010, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(fx_power->putProperty(nullptr, target, 9011, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(fx_power->putProperty(nullptr, target, 9012, -1, nullptr, true, false, true).get()->obj);
	output += "}\n\n";
	return output;
}
std::string TFSettingsWriter::writeSetting_fx_modulo(std::shared_ptr<Tool> target, TSet_FX_Modulo* fx_modulo)
{
	std::string output = "fx_modulo \n{\n";
	output += std::get<std::string>(fx_modulo->putProperty(nullptr, target, 9105, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(fx_modulo->putProperty(nullptr, target, 9110, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(fx_modulo->putProperty(nullptr, target, 9110, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(fx_modulo->putProperty(nullptr, target, 9111, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(fx_modulo->putProperty(nullptr, target, 9112, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(fx_modulo->putProperty(nullptr, target, 9113, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(fx_modulo->putProperty(nullptr, target, 9114, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(fx_modulo->putProperty(nullptr, target, 9120, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(fx_modulo->putProperty(nullptr, target, 9121, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(fx_modulo->putProperty(nullptr, target, 9122, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(fx_modulo->putProperty(nullptr, target, 9123, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(fx_modulo->putProperty(nullptr, target, 9124, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(fx_modulo->putProperty(nullptr, target, 9125, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(fx_modulo->putProperty(nullptr, target, 9126, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(fx_modulo->putProperty(nullptr, target, 9127, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(fx_modulo->putProperty(nullptr, target, 9128, -1, nullptr, true, false, true).get()->obj);
	output += "}\n\n";
	return output;
}
std::string TFSettingsWriter::writeSetting_fx_blur(std::shared_ptr<Tool> target, TSet_FX_GaussianBlur* fx_blur)
{
	std::string output = "fx_blur \n{\n";
	output += std::get<std::string>(fx_blur->putProperty(nullptr, target, 9505, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(fx_blur->putProperty(nullptr, target, 9510, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(fx_blur->putProperty(nullptr, target, 9511, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(fx_blur->putProperty(nullptr, target, 9512, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(fx_blur->putProperty(nullptr, target, 9513, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(fx_blur->putProperty(nullptr, target, 9514, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(fx_blur->putProperty(nullptr, target, 9515, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(fx_blur->putProperty(nullptr, target, 9516, -1, nullptr, true, false, true).get()->obj);
	output += "}\n\n";
	return output;
}
//
std::string TFSettingsWriter::writeSetting_effects(std::shared_ptr<Tool> target, TSet_Effects* effects)
{
	std::string output = "effects \n{\n";
	output += makeSettingString(effects->isEnabled, -1, -1, "isEnabled");
	output += std::get<std::string>(effects->putProperty(nullptr, target, 8050, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(effects->putProperty(nullptr, target, 8051, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(effects->putProperty(nullptr, target, 8052, -1, nullptr, true, false, true).get()->obj);
	//
	output += std::get<std::string>(effects->putProperty(nullptr, target, 8010, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(effects->putProperty(nullptr, target, 8011, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(effects->putProperty(nullptr, target, 8012, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(effects->putProperty(nullptr, target, 8013, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(effects->putProperty(nullptr, target, 8014, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(effects->putProperty(nullptr, target, 8015, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(effects->putProperty(nullptr, target, 8016, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(effects->putProperty(nullptr, target, 8017, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(effects->putProperty(nullptr, target, 8018, -1, nullptr, true, false, true).get()->obj);
	output += std::get<std::string>(effects->putProperty(nullptr, target, 8024, -1, nullptr, true, false, true).get()->obj);
	//
	output += std::get<std::string>(effects->putProperty(nullptr, target, 8098, -1, nullptr, true, false, true).get()->obj);
	output += "}\n\n";
	// Add additional settings
	if (effects->fill.isEnabled)
	{
		//output += writeSetting_fx_fill(target, &effects->fill);
	}
	if (effects->gradient.isEnabled)
	{
		//output += writeSetting_fx_gradient(target, &effects->gradient);
	}
	if (effects->posterize.isEnabled)
	{
		output += writeSetting_fx_posterize(target, &effects->posterize);
	}
	if (effects->invert.isEnabled)
	{
		output += writeSetting_fx_invert(target, &effects->invert);
	}
	if (effects->threshold.isEnabled)
	{
		//output += writeSetting_fx_threshold(target, &effects->threshold);
	}
	if (effects->brightContrast.isEnabled)
	{
		output += writeSetting_fx_brightContrast(target, &effects->brightContrast);
	}
	if (effects->hsv.isEnabled)
	{
		output += writeSetting_fx_hsv(target, &effects->hsv);
	}
	if (effects->power.isEnabled)
	{
		output += writeSetting_fx_power(target, &effects->power);
	}
	if (effects->modulo.isEnabled)
	{
		output += writeSetting_fx_modulo(target, &effects->modulo);
	}
	if (effects->blur.isEnabled)
	{
		output += writeSetting_fx_blur(target, &effects->blur);
	}
	return output;
}
