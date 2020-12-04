#pragma once

namespace engine 
{
	/////////////////////////////////////////////////////////////////////// Constants
	const float M_PI = 3.14159265358979323846f;
	const float DELTA = (float)1.0e-5;
	const float DEGREES_TO_RADIANS = 0.01745329251994329547f;
	const float RADIANS_TO_DEGREES = 57.29577951308232185913f;

	/////////////////////////////////////////////////////////////////////// SharedVariables
	const std::string MODEL_MATRIX = "ModelMatrix";
	const std::string NORMAL_MATRIX = "NormalMatrix";
	const std::string VIEW_MATRIX = "ViewMatrix";
	const std::string PROJECTION_MATRIX = "ProjectionMatrix";
	const std::string VIEW_PROJECTION_MATRIX = "ViewProjectionMatrices";

	const std::string VERTEX_ATTRIBUTE = "inPosition";
	const std::string TEXCOORDS_ATTRIBUTE = "inTexcoord";
	const std::string NORMAL_ATTRIBUTE = "inNormal";
}