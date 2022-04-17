/*
* This is temporaraly
*/

#pragma once

#include <cstdio>
#include <string>
#include <vector>

#include <GL/glew.h>

class UIState
{
	public:
		int winWidth = 0, winHeight = 0;
		int textureToRenderTo;
		float lightX, lightY, lightZ;
		bool isRunning;
		bool isWinSizeChanged = false;
};
