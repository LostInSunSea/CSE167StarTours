#include "AudioEngine.h"
using namespace std;

AudioEngine::AudioEngine()
{

}

AudioEngine::~AudioEngine()
{
}

void AudioEngine::init(glm::vec3 position)
{
	//position is the initial position of the camera/ship
	result = FMOD::System_Create(&system);
	result = system->init(100, FMOD_INIT_NORMAL, extradriverdata);
	result = system->set3DSettings(1.0, DISTANCEFACTOR, 1.0f);
	listenerflag = true;
	glm::vec3 temp = { position.x, position.y, position.z*DISTANCEFACTOR };
}

// for short sound samples or sounds with multiple instances
void AudioEngine::loadSound(const std::string & path, bool b3d, bool loop)
{
	//check to see if it already exists or not
	auto findSound = sounds.find(path);
	if (findSound != sounds.end()) {
		cerr << "already loaded" << endl;
		return;
	}
	//set up modes for parameters looping and 3d
	FMOD_MODE mLoop;
	FMOD_MODE mB3D;
	//2d or 3d
	if (b3d) {
		mB3D = FMOD_3D;
	}
	else {
		mB3D = FMOD_2D;
	}
	//looping
	if (loop) {
		mLoop = FMOD_LOOP_NORMAL;
	}
	else {
		mLoop = FMOD_LOOP_OFF;
	}
	FMOD::Sound* pSound = nullptr;
	system->createStream(path.c_str(), mB3D, 0, &pSound);
	if (b3d) {
		pSound->set3DMinMaxDistance(0.5f * DISTANCEFACTOR, 5000.0f * DISTANCEFACTOR);
	}
	pSound->setMode(mLoop);
	sounds[path] = pSound;
}

//for longer files 
void AudioEngine::streamSound(const std::string & path, bool b3d, bool loop)
{
	//check to see if it already exists or not
	auto findSound = sounds.find(path);
	if (findSound != sounds.end()) {
		cerr << "already in space"<<endl;
		return;
	}
	//set up modes for parameters looping and 3d
	FMOD_MODE mLoop;
	FMOD_MODE mB3D;
	//2d or 3d
	if (b3d) {
		mB3D = FMOD_3D;
	}
	else {
		mB3D = FMOD_2D;
	}
	//looping
	if (loop) {
		mLoop = FMOD_LOOP_NORMAL;
	}
	else {
		mLoop = FMOD_LOOP_OFF;
	}
	FMOD::Sound* pSound = nullptr;
	system->createStream(path.c_str(),mB3D, 0, &pSound);
	if (b3d) {
		pSound->set3DMinMaxDistance(0.5f * DISTANCEFACTOR, 5000.0f * DISTANCEFACTOR);
	}
	pSound->setMode(mLoop);
	sounds[path] = pSound;
}
//playback
void AudioEngine::playSound(const std::string & path, glm::vec3 position, glm::vec3 velocity,float volume)
{
	int channelId = mChannelInd++;
	auto findSound = sounds.find(path);
	if (findSound == sounds.end()) {
		cerr << "load your fucking shit" << endl;
		return;
	}
	FMOD::Channel* pChannel = nullptr;
	system->playSound(findSound->second,0,false, &pChannel);
	if (pChannel) {
		cout << path << " has loaded" << endl;
		FMOD_MODE curr;
		curr = findSound->second->getMode(&curr);
		if (curr*FMOD_3D) {
			pChannel->set3DAttributes(&vec3toFmod(position), &vec3toFmod(velocity));
		}
		pChannel->setVolume(volume);
		pChannel->setPaused(false);
		channels[channelId] = pChannel;
	}
}

void AudioEngine::update()
{
	system->update();
}

void AudioEngine::setChannelVolume(int id, float volume)
{
	auto findChannel = channels.find(id);
	if (findChannel == channels.end()) {
		cerr << "load your fucking shit" << endl;
		return;
	}
	float debug;
	findChannel->second->getVolume(&debug);
	findChannel->second->setVolume(volume);
}

void AudioEngine::setChannel3dPosition(int id, glm::vec3 pos)
{

}

//helper functions
FMOD_VECTOR AudioEngine::vec3toFmod(glm::vec3 vec)
{
	FMOD_VECTOR fVec = { vec.x, vec.y, vec.z };
	return fVec;
}

float  AudioEngine::VolumeTodB(float volume)
{
	return 20.0f * log10f(volume);
}
