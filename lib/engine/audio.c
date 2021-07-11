#include "engine/audio.h"

#include "engine/array.h"

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio/miniaudio.h"

#include "stdio.h"
#include "pthread.h"

typedef struct Sound{
	EntityHeader entityHeader;
	int currentFrame;
	int soundDataIndex;
	float volume;
	bool loop;
	bool stopped;
	enum Audio_SoundTypeEnum type;
}Sound;

typedef struct SoundData{
	float *data;
	int framesLength;
	char *name;
}SoundData;

ma_result result;
ma_decoder_config decoderConfig;
ma_device device;
ma_device_config deviceConfig;

pthread_mutex_t soundMutex = PTHREAD_MUTEX_INITIALIZER;

SoundData soundData[255];

int soundDataLength = 0;

Array sounds;

float volumes[2];

void data_callback(ma_device* device_p, void* output_p, const void* input_p, ma_uint32 frameCount){

	pthread_mutex_lock(&soundMutex);

	float *outputF32_p = (float *)output_p;
	MA_ASSERT(device_p->playback.format == SAMPLE_FORMAT);

	for(int i = 0; i < sounds.length; i++){

		Sound *sound_p = Array_getItemPointerByIndex(&sounds, i);
		SoundData *soundData_p = &soundData[sound_p->soundDataIndex];
		
		if(sound_p->stopped){
			continue;
		}

		sound_p->currentFrame += frameCount;

		if(sound_p->currentFrame + frameCount > soundData_p->framesLength){
			if(sound_p->loop){
				sound_p->currentFrame = 0;
			}else{
				Array_removeItemByIndex(&sounds, i);
				i--;
				continue;
			}
		}

		for(int i = 0; i < frameCount * CHANNEL_COUNT; i++){
			outputF32_p[i] += soundData_p->data[sound_p->currentFrame * CHANNEL_COUNT + i] * sound_p->volume * volumes[sound_p->type];
		}
	
	}

	pthread_mutex_unlock(&soundMutex);

}

void Audio_init(char **soundFiles, int soundFilesLength){

	decoderConfig = ma_decoder_config_init(SAMPLE_FORMAT, CHANNEL_COUNT, SAMPLE_RATE);


	for(int i = 0; i < soundFilesLength; i++){

		SoundData *soundData_p = &soundData[i];

		soundData_p->name = soundFiles[i];

		ma_decoder decoder;

		char path[255];

		sprintf(path, "assets/audio/%s.wav", soundFiles[i]);

		result = ma_decoder_init_file(path, &decoderConfig, &decoder);
		if (result != MA_SUCCESS){
			printf("Could not read file: %s\n", path);
		}

		int numberOfFrames = 0;

		int readFrames = 1;

		float *tmpBuffer = malloc(sizeof(float) * CHANNEL_COUNT * 1000);

		while(readFrames != 0){
			readFrames = ma_decoder_read_pcm_frames(&decoder, tmpBuffer, 1000);
			numberOfFrames += readFrames;
		}

		free(tmpBuffer);

		soundData_p->framesLength = numberOfFrames;
		soundData_p->data = malloc(sizeof(float) * soundData_p->framesLength * CHANNEL_COUNT);

		ma_decoder_seek_to_pcm_frame(&decoder, 0);

		ma_decoder_read_pcm_frames(&decoder, soundData_p->data, soundData_p->framesLength * CHANNEL_COUNT);

		printf("Loaded WAV file: %s\n", soundData_p->name);
		
	}
	
	soundDataLength = soundFilesLength;

	Array_init(&sounds, sizeof(Sound));

	volumes[AUDIO_SOUND_TYPE_SFX] = 1.0;
	volumes[AUDIO_SOUND_TYPE_MUSIC] = 1.0;

    deviceConfig = ma_device_config_init(ma_device_type_playback);
    deviceConfig.playback.format   = SAMPLE_FORMAT;
    deviceConfig.playback.channels = CHANNEL_COUNT;
    deviceConfig.sampleRate        = SAMPLE_RATE;
    deviceConfig.dataCallback      = data_callback;

    if (ma_device_init(NULL, &deviceConfig, &device) != MA_SUCCESS) {
        printf("Failed to open playback device.\n");
    }

    if (ma_device_start(&device) != MA_SUCCESS) {
        printf("Failed to start playback device.\n");
    }

}

void Audio_setVolume(float volume, enum Audio_SoundTypeEnum soundType){

	pthread_mutex_lock(&soundMutex);

	volumes[soundType] = volume;

	pthread_mutex_unlock(&soundMutex);

}

float Audio_getVolume(enum Audio_SoundTypeEnum soundType){

	pthread_mutex_lock(&soundMutex);

	return volumes[soundType];

	pthread_mutex_unlock(&soundMutex);

}

size_t Audio_playSound(char *soundName, float volume, bool loop, enum Audio_SoundTypeEnum soundType){

	pthread_mutex_lock(&soundMutex);

	Sound *sound_p = Array_addItem(&sounds);

	int soundDataIndex = -1;
	for(int i = 0; i < soundDataLength; i++){
		if(strcmp(soundData[i].name, soundName) == 0){
			soundDataIndex = i;
			break;
		}
	}

	if(soundDataIndex == -1){
		printf("No sound with name: %s\n", soundName);
	}

	sound_p->currentFrame = 0;
	sound_p->soundDataIndex = soundDataIndex;
	sound_p->volume = volume;
	sound_p->loop = loop;
	sound_p->type = soundType;
	sound_p->stopped = false;

	pthread_mutex_unlock(&soundMutex);

	return sound_p->entityHeader.ID;

}

void Audio_stopSoundByID(size_t ID){

	pthread_mutex_lock(&soundMutex);

	Sound *sound_p = Array_getItemPointerByID(&sounds, ID);

	sound_p->stopped = true;

	pthread_mutex_unlock(&soundMutex);

}

void Audio_resumeSoundByID(size_t ID){

	pthread_mutex_lock(&soundMutex);

	Sound *sound_p = Array_getItemPointerByID(&sounds, ID);

	sound_p->stopped = false;

	pthread_mutex_unlock(&soundMutex);

}

void Audio_killSoundByID(size_t ID){

	pthread_mutex_lock(&soundMutex);

	Array_removeItemByID(&sounds, ID);

	pthread_mutex_unlock(&soundMutex);

}
