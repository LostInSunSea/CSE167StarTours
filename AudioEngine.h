/*----------------------------------------------------------------
Notes:
channel a = jet noises + lasers
channel b = ambient backgrouhnd sound
channel c = music
channel d = turrets and at-at
----------------------------------------------------------------*/
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

#include "fmod_studio.hpp"
#include "fmod.hpp"
#include <string>
#include <map>
#include <vector>
#include <math.h>
#include <iostream>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

typedef std::map<std::string, FMOD::Sound*> SoundMap;
typedef std::map<int, FMOD::Channel*> ChannelMap;

class AudioEngine
{
private:
	FMOD::System       *system;
	SoundMap sounds;
	ChannelMap channels;
	FMOD_RESULT         result;
	void               *extradriverdata = 0;
	const float DISTANCEFACTOR = 1.0f;          // Units per meter.  I.e feet would = 3.28.  centimeters would = 100.
	float mChannelInd=0;
	bool             listenerflag;
	FMOD_VECTOR      listenerpos;
public:
	AudioEngine();
	~AudioEngine();
	void init(glm::vec3 position);
	void loadSound(const std::string& path, bool b3d, bool loop);
	void streamSound(const std::string& path, bool b3d,bool loop);
	void playSound(const std::string& path, glm::vec3 position, glm::vec3 velocity,float volume);
	void update();
	void setChannelVolume(int id, float volume);
	void setChannel3dPosition(int id, glm::vec3 pos);
	FMOD_VECTOR vec3toFmod(glm::vec3 vec);
	float VolumeTodB(float volume);
};

#endif


