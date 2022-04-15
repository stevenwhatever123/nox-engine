/*
 * An interface: comunication with Renderer system of the engine.
 * Allows a class that implements this interface to be rendered by Renderer
 */


#pragma once

#include <vector>

class IPosition
{
public:

	float x = 0, y = 0, z = 0;
};