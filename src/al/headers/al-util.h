/*
** al-util.h
**
** This file is part of mkxp.
**
** Copyright (C) 2013 Jonas Kulla <Nyocurio@gmail.com>
**
** mkxp is free software: you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation, either version 2 of the License, or
** (at your option) any later version.
**
** mkxp is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with mkxp.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef ALUTIL_H
#define ALUTIL_H

#define AL_ALEXT_PROTOTYPES
#include <al.h>
#include <efx.h>
#include <alext.h>
#include <SDL2/SDL_audio.h>
#include <assert.h>

namespace AL
{

#define DEF_AL_ID \
struct ID \
{ \
	ALuint al; \
	explicit ID(ALuint al = 0)  \
	    : al(al)  \
	{}  \
	ID &operator=(const ID &o)  \
	{  \
		al = o.al;  \
		return *this; \
	}  \
	bool operator==(const ID &o) const  \
	{  \
		return al == o.al;  \
	}  \
};

namespace Buffer
{
	DEF_AL_ID

	inline Buffer::ID gen()
	{
		Buffer::ID id;
		alGenBuffers(1, &id.al);

		return id;
	}

	inline void del(Buffer::ID id)
	{
		alDeleteBuffers(1, &id.al);
	}

	inline void uploadData(Buffer::ID id, ALenum format, const ALvoid *data, ALsizei size, ALsizei freq)
	{
		alBufferData(id.al, format, data, size, freq);
	}

	inline ALint getInteger(Buffer::ID id, ALenum prop)
	{
		ALint value;
		alGetBufferi(id.al, prop, &value);

		return value;
	}

	inline ALint getSize(Buffer::ID id)
	{
		return getInteger(id, AL_SIZE);
	}

	inline ALint getBits(Buffer::ID id)
	{
		return getInteger(id, AL_BITS);
	}

	inline ALint getChannels(Buffer::ID id)
	{
		return getInteger(id, AL_CHANNELS);
	}
}

namespace Filter
{
	DEF_AL_ID

	inline void del(Filter::ID id)
	{
		alDeleteFilters(1, &id.al);
	}

	inline bool isNullFilter(Filter::ID id)
	{
		return id.al == AL_FILTER_NULL;
	}

	inline Filter::ID nullFilter()
	{
		return Filter::ID(AL_FILTER_NULL);
	}

	inline void setInteger(Filter::ID id, ALenum prop, ALint value)
	{
		alFilteri(id.al, prop, value);
	}

	inline void setFloat(Filter::ID id, ALenum prop, ALfloat value)
	{
		alFilterf(id.al, prop, value);
	}

	inline Filter::ID createLowpassFilter(float gain, float gainhf) {
		Filter::ID id;
		alGenFilters(1, &id.al);
		setInteger(id, AL_FILTER_TYPE, AL_FILTER_LOWPASS);
		setFloat(id, AL_LOWPASS_GAIN, gain);
		setFloat(id, AL_LOWPASS_GAINHF, gainhf);
		return id;
	}

	inline Filter::ID createHighpassFilter(float gain, float gainlf) {
		Filter::ID id;
		alGenFilters(1, &id.al);
		setFloat(id, AL_HIGHPASS_GAIN, gain);
		setFloat(id, AL_HIGHPASS_GAINLF, gainlf);
		return id;
	}

	inline Filter::ID createBandpassFilter(float gain, float gainlf, float gainhf) {
		Filter::ID id;
		alGenFilters(1, &id.al);
		setInteger(id, AL_FILTER_TYPE, AL_FILTER_BANDPASS);
		setFloat(id, AL_BANDPASS_GAIN, gain);
		setFloat(id, AL_BANDPASS_GAINLF, gainlf);
		setFloat(id, AL_BANDPASS_GAINHF, gainhf);
		return id;
	}

}

namespace AuxiliaryEffectSlot
{
	struct ID {
		ALuint al;
		ALuint effect;
		explicit ID(ALuint al = 0)
	    	: al(al),
			  effect(AL_EFFECT_NULL)
		{}
		ID &operator=(const ID &o)
		{
			al = o.al;
			effect = o.effect;
			return *this;
		}
		bool operator==(const ID &o) const
		{
			return al == o.al;
		}
};

	inline AuxiliaryEffectSlot::ID gen() {
		AuxiliaryEffectSlot::ID id;
		alGenAuxiliaryEffectSlots(1, &id.al);
		return id;
	}

	inline void attachEffect(AuxiliaryEffectSlot::ID id, ALuint effect) {
		alAuxiliaryEffectSloti(id.al, AL_EFFECTSLOT_EFFECT, effect);
		if (id.effect != AL_EFFECT_NULL) {
			//alDeleteEffects(1, &id.effect);
		}
		id.effect = effect;
	}

	inline void del(AuxiliaryEffectSlot::ID id)
	{
		alDeleteAuxiliaryEffectSlots(1, &id.al);
		if (id.effect != AL_EFFECT_NULL) {
			//alDeleteEffects(1, &id.effect);
		}
	}
}

namespace Source
{
	DEF_AL_ID

	inline Source::ID gen()
	{
		Source::ID id;
		alGenSources(1, &id.al);

		return id;
	}

	inline void del(Source::ID id)
	{
		alDeleteSources(1, &id.al);
	}

	inline void attachBuffer(Source::ID id, Buffer::ID buffer)
	{
		alSourcei(id.al, AL_BUFFER, buffer.al);
	}

	inline void detachBuffer(Source::ID id)
	{
		attachBuffer(id, Buffer::ID(0));
	}

	inline void queueBuffer(Source::ID id, Buffer::ID buffer)
	{
		alSourceQueueBuffers(id.al, 1, &buffer.al);
	}

	inline Buffer::ID unqueueBuffer(Source::ID id)
	{
		Buffer::ID buffer;
		alSourceUnqueueBuffers(id.al, 1, &buffer.al);

		return buffer;
	}

	inline void clearQueue(Source::ID id)
	{
		attachBuffer(id, Buffer::ID(0));
	}

	inline ALint getInteger(Source::ID id, ALenum prop)
	{
		ALint value;
		alGetSourcei(id.al, prop, &value);

		return value;
	}

	inline ALint getProcBufferCount(Source::ID id)
	{
		return getInteger(id, AL_BUFFERS_PROCESSED);
	}

	inline ALenum getState(Source::ID id)
	{
		return getInteger(id, AL_SOURCE_STATE);
	}

	inline ALfloat getSecOffset(Source::ID id)
	{
		ALfloat value;
		alGetSourcef(id.al, AL_SEC_OFFSET, &value);

		return value;
	}

	inline void setFilter(Source::ID id, Filter::ID filter)
	{
		alSourcei(id.al, AL_DIRECT_FILTER, filter.al);
	}

	inline void setAuxEffectSlot(Source::ID id, AuxiliaryEffectSlot::ID effectSlot) {
		alSource3i(id.al, AL_AUXILIARY_SEND_FILTER, effectSlot.al, 0, AL_FILTER_NULL);
	}

	inline void setVolume(Source::ID id, float value)
	{
		alSourcef(id.al, AL_GAIN, value);
	}

	inline void setPitch(Source::ID id, float value)
	{
		alSourcef(id.al, AL_PITCH, value);
	}

	inline void play(Source::ID id)
	{
		alSourcePlay(id.al);
	}

	inline void stop(Source::ID id)
	{
		alSourceStop(id.al);
	}

	inline void pause(Source::ID id)
	{
		alSourcePause(id.al);
	}
}

}

inline uint8_t formatSampleSize(int sdlFormat)
{
	switch (sdlFormat)
	{
	case AUDIO_U8 :
	case AUDIO_S8 :
		return 1;

	case AUDIO_U16LSB :
	case AUDIO_U16MSB :
	case AUDIO_S16LSB :
	case AUDIO_S16MSB :
		return 2;

    case AUDIO_F32LSB :
    case AUDIO_F32MSB :
        return 4;

	default :
		assert(!"Unhandled sample format");
	}

	return 0;
}

inline ALenum chooseALFormat(int sampleSize, int channelCount)
{
	switch (sampleSize)
	{
	case 1 :
		switch (channelCount)
		{
		case 1 : return AL_FORMAT_MONO8;
		case 2 : return AL_FORMAT_STEREO8;
		}
		/* falls through */
	case 2 :
		switch (channelCount)
		{
		case 1 : return AL_FORMAT_MONO16;
		case 2 : return AL_FORMAT_STEREO16;
		}
		
	case 4 :
        switch (channelCount)
        {
        case 1 : return AL_FORMAT_MONO_FLOAT32;
        case 2 : return AL_FORMAT_STEREO_FLOAT32;
        }

	default :
		assert(!"Unhandled sample size / channel count");
	}

	return 0;
}

#define AUDIO_SLEEP 10
#define STREAM_BUF_SIZE 32768
#define GLOBAL_VOLUME 0.8f

#endif // ALUTIL_H