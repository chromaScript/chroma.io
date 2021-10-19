#ifndef METHODTYPE_H
#define METHODTYPE_H

#include <map>
#include <string>

enum class MethodType : int
{
	null = 0,
	// 1-99
	in_draw = 4,
	in_fan = 18,
	in_lasso = 9,
	in_line = 5,
	in_multiSpline = 32,
	in_noInput = 404,
	in_pan = 11,
	in_point = 13,
	in_polygon = 10,
	in_polyLine = 22,
	in_radialField = 28,
	in_rake = 17,
	in_rotate = 2,
	in_sampler = 14,
	in_shapeDraw = 21,
	in_shapeField = 15,
	in_shapeLine = 20,
	in_spline = 31,
	in_vortex = 16,
	in_zoom = 3,
	// Output 100-199
	out_pan = 100,
	out_rotate = 101,
	out_zoom = 102,
	out_sampler = 107,
	out_fill = 106,
	out_gradient = 105,
	out_noOutput = 405,
	out_selection = 110,
	out_stroke = 103,
	out_shape = 111,
	// Control 200-299
	control_default = 406,
	control_continuous = 200,
	control_drag = 201,
	control_onepoint = 202,
	control_twopoint = 203,
	control_threepoint = 204,
	control_fourpoint = 205,
	control_zoom = 206,
	control_rotate = 207,
	control_pan = 208,
	control_sampler = 209
};

inline std::map<MethodType, std::string> methodTypeStringMap = {
	{MethodType::null, "null"},
	{MethodType::in_draw, "in_draw"},
	{MethodType::in_fan, "in_fan"},
	{MethodType::in_lasso, "in_lasso"},
	{MethodType::in_line, "in_line"},
	{MethodType::in_multiSpline, "in_multiSpline"},
	{MethodType::in_noInput, "in_noInput"},
	{MethodType::in_pan, "in_pan"},
	{MethodType::in_point, "in_point"},
	{MethodType::in_polygon, "in_polygon"},
	{MethodType::in_polyLine, "in_polyLine"},
	{MethodType::in_radialField, "in_radialField"},
	{MethodType::in_rake, "in_rake"},
	{MethodType::in_rotate, "in_rotate"},
	{MethodType::in_sampler, "in_sampler"},
	{MethodType::in_shapeDraw, "in_shapeDraw"},
	{MethodType::in_shapeField, "in_shapeField"},
	{MethodType::in_shapeLine, "in_shapeLine"},
	{MethodType::in_spline, "in_spline"},
	{MethodType::in_vortex, "in_vortex"},
	{MethodType::in_zoom, "in_zoom"},
	//
	{MethodType::out_pan, "out_pan"},
	{MethodType::out_rotate, "out_rotate"},
	{MethodType::out_zoom, "out_zoom"},
	{MethodType::out_sampler, "out_sampler"},
	{MethodType::out_fill, "out_fill"},
	{MethodType::out_gradient, "out_gradient"},
	{MethodType::out_noOutput, "out_noOutput"},
	{MethodType::out_selection, "out_selection"},
	{MethodType::out_stroke, "out_stroke"},
	{MethodType::out_shape, "out_shape"},
	{MethodType::control_default, "control_default"},
	{MethodType::control_continuous, "control_continuous"},
	{MethodType::control_drag, "control_drag"},
	{MethodType::control_onepoint, "control_onepoint"},
	{MethodType::control_twopoint, "control_twopoint"},
	{MethodType::control_threepoint, "control_threepoint"},
	{MethodType::control_fourpoint, "control_fourpoint"},
	{MethodType::control_zoom, "control_zoom"},
	{MethodType::control_rotate, "control_rotate"},
	{MethodType::control_pan, "control_pan"},
	{MethodType::control_sampler, "control_sampler"}
};

#endif