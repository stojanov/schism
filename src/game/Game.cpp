#include "Game.h"

#include "chess/Chess.h"
#include "schism/Audio/WavLoader.h"
#include "schism/System/Log.h"
#include <AL/al.h>
#include <AL/alc.h>
#include <chrono>
#include <filesystem>
#include <AudioFile.h>
#include <thread>


void checkForError()
{
    auto err = alGetError();

    if (err != AL_NO_ERROR)
    {
        SC_CORE_ERROR("Open AL ERROR {}", err);
    }
}
Game::Game()
	:
	Application(800, 800, "Chess")
{
	m_Ctx->GlobalAssets.Shaders.Load("spriterenderer", "res/shaders/sprite_renderer.vert", "res/shaders/sprite_renderer.frag");

	SpriteRenderer::RegisterShader(m_Ctx->GlobalAssets.Shaders.GetHandle("spriterenderer"));

	m_SceneManager.Register<Chess::Chess>("chess");

    
    ALCdevice* aldevice = alcOpenDevice(nullptr);
    ALCcontext* context = alcCreateContext(aldevice, nullptr);

    if (!alcMakeContextCurrent(context))
    {
        SC_CORE_ERROR("Cannot create context");
    }

    checkForError();
    ALfloat listenerOri[] = { 0.f, 0.f, 1.f, 0.f, 1.f, 0.f };

    alListener3f(AL_POSITION, 0, 0, 1.f);
    alListener3f(AL_VELOCITY, 0, 0, 0);
    alListenerfv(AL_ORIENTATION, listenerOri);

    ALuint source;
    alGenSources(1, &source);
checkForError();

    alSourcef(source, AL_PITCH, 1);
    alSourcef(source, AL_GAIN, 1);
    alSource3f(source, AL_POSITION, 0, 0, 0);
    alSource3f(source, AL_VELOCITY, 0, 0, 0);
    alSourcei(source, AL_LOOPING, AL_FALSE);
    
    ALuint buffer;
    alGenBuffers(1, &buffer);

    checkForError();
    // AudioFile<float> audioFile;
    // //
    // audioFile.load("res/sample-3s.wav");
    Audio::WavLoader file(std::filesystem::path("res/sample-3s.wav"));

    file.Load();

    auto bitsPerSample = file.Header().bitsPerSample;
    auto channels = file.Header().channels;


    // auto bitsPerSample = audioFile.getBitDepth();
    // auto channels = audioFile.getNumChannels();

    ALenum format = -99;
    switch (bitsPerSample)
    {
        case 16:
            {
                if (channels == 2)
                {
                    format = AL_FORMAT_STEREO16;
                }
                else if (channels == 1)
                {
                    format = AL_FORMAT_MONO16;
                }
            }
        case 8:
            {
                if (channels == 2)
                {
                    format = AL_FORMAT_STEREO8;
                }
                else if (channels == 1)
                {
                    format = AL_FORMAT_MONO8;
                }
            }
    }
    SC_CORE_INFO("STUFF {} {} {}", format, file.Header().sampleRate, file.Header().bitsPerSample);
    // auto data = audioFile.samples;
    // alBufferData(buffer, format,  data[0].data(), audioFile.getNumSamplesPerChannel(), audioFile.getSampleRate() );
    alBufferData(buffer, format, file.Data().data(), file.Header().size, file.Header().sampleRate);
    alSourcei(source, AL_BUFFER, buffer);
    checkForError();

    alSourcePlay(source);
    checkForError();


    while (1)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

Game::~Game()
{
	
}
