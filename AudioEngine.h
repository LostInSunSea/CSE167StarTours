#ifndef _AUDIO_ENGINE_H_
#define _AUDIO_ENGINE_H_

#define GLFW_INCLUDE_GLEXT
#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
// Use of degrees is deprecated. Use radians instead.
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif

#include "./inc/fmod/fmod_studio.hpp"
#include "./inc/fmod/fmod.hpp"
#include <string>
#include <map>
#include <vector>
#include <math.h>
#include <iostream>

class AudioEngine
{
public:
	AudioEngine();
	~AudioEngine();
};

#endif


