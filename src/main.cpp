/*
** main.cpp
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

#ifndef USE_FMOD
#include <alc.h>
#endif

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#ifndef USE_FMOD
#include <SDL_sound.h>
#else
#include <fmod_studio.h>
#include <fmod_errors.h>
#endif
#include <physfs.h>

#ifdef _MSC_VER
#include <direct.h>
#define _chdir chdir
#else
#include <unistd.h>
#endif
#include <string.h>
#include <assert.h>
#include <string>

#include "sharedstate.h"
#include "eventthread.h"
#include "gl-debug.h"
#include "debugwriter.h"
#include "exception.h"
#include "gl-fun.h"
#include "i18n.h"

#include "binding.h"

#include "icon.png.xxd"
#include "version.h"

#ifdef STEAM
#include "steamshim/steamshim_child.h"
#else
#include "gamecontrollerdb.txt.xxd"
#endif

#ifdef __WIN32
#include "win-consoleutils.h"
#endif

static void
rgssThreadError(RGSSThreadData *rtData, const std::string &msg)
{
	rtData->rgssErrorMsg = msg;
	rtData->ethread->requestTerminate();
	rtData->rqTermAck.set();
}

static inline const char*
glGetStringInt(GLenum name)
{
	return (const char*) gl.GetString(name);
}

static void
printGLInfo()
{
	Debug() << "GL Vendor    :" << glGetStringInt(GL_VENDOR);
	Debug() << "GL Renderer  :" << glGetStringInt(GL_RENDERER);
	Debug() << "GL Version   :" << glGetStringInt(GL_VERSION);
	Debug() << "GLSL Version :" << glGetStringInt(GL_SHADING_LANGUAGE_VERSION);
}

int rgssThreadFun(void *userdata)
{
	RGSSThreadData *threadData = static_cast<RGSSThreadData*>(userdata);
	const Config &conf = threadData->config;
	SDL_Window *win = threadData->window;
	SDL_GLContext glCtx;

	/* Setup GL context */
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

#ifndef NDEBUG
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
#endif

	glCtx = SDL_GL_CreateContext(win);

	if (!glCtx)
	{
		rgssThreadError(threadData, std::string("Error creating context: ") + SDL_GetError());
		return 0;
	}

	try
	{
		initGLFunctions();
	}
	catch (const Exception &exc)
	{
		rgssThreadError(threadData, exc.msg);
		SDL_GL_DeleteContext(glCtx);

		return 0;
	}

	if (!conf.enableBlitting)
		gl.BlitFramebuffer = 0;

	gl.ClearColor(0, 0, 0, 1);
	gl.Clear(GL_COLOR_BUFFER_BIT);
	SDL_GL_SwapWindow(win);

	printGLInfo();

	bool vsync = conf.vsync || conf.syncToRefreshrate;
	SDL_GL_SetSwapInterval(vsync ? 1 : 0);

#ifndef NDEBUG
	GLDebugLogger dLogger;
#endif

	#ifndef USE_FMOD
	/* Setup AL context */
	ALCcontext *alcCtx = alcCreateContext(threadData->alcDev, 0);

	if (!alcCtx)
	{
		rgssThreadError(threadData, "Error creating OpenAL context");
		SDL_GL_DeleteContext(glCtx);

		return 0;
	}

	alcMakeContextCurrent(alcCtx);
	#endif

	try
	{
		SharedState::initInstance(threadData);
	}
	catch (const Exception &exc)
	{
		rgssThreadError(threadData, exc.msg);
		#ifndef USE_FMOD
		alcDestroyContext(alcCtx);
		#endif
		SDL_GL_DeleteContext(glCtx);

		return 0;
	}

	/* Start script execution */
	scriptBinding->execute();

	threadData->rqTermAck.set();
	threadData->ethread->requestTerminate();

	SharedState::finiInstance();

	#ifndef USE_FMOD
	alcDestroyContext(alcCtx);
	#endif
	SDL_GL_DeleteContext(glCtx);

	return 0;
}

static void showInitError(const std::string &msg)
{
	Debug() << msg;
	SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "OneShot", msg.c_str(), 0);
}

static void setupWindowIcon(const Config &conf, SDL_Window *win)
{
	SDL_RWops *iconSrc;

	if (conf.iconPath.empty())
		iconSrc = SDL_RWFromConstMem(___assets_icon_png, ___assets_icon_png_len);
	else
		iconSrc = SDL_RWFromFile(conf.iconPath.c_str(), "rb");

	SDL_Surface *iconImg = IMG_Load_RW(iconSrc, SDL_TRUE);

	if (iconImg)
	{
		SDL_SetWindowIcon(win, iconImg);
		SDL_FreeSurface(iconImg);
	}
}

// mainly doing this so journal app knows where to load images from
static void setGamePathInRegistry() {

#if defined WIN32
	// this logic is currently windows specific
	char* dataDir = SDL_GetBasePath();
	if (dataDir)
	{
		HKEY key;
		long keyOpenError = RegOpenKey(HKEY_CURRENT_USER, TEXT("Software\\OneShot\\"), &key);

		if (keyOpenError != ERROR_SUCCESS) {
			// try creating the key first
			long keyCreateError = RegCreateKeyEx(HKEY_CURRENT_USER, TEXT("Software\\OneShot\\"), 0L, NULL, REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &key, NULL);

			if (keyCreateError != ERROR_SUCCESS)
			{
				showInitError("Unable to create key in registry.");
			}
			else {
				keyOpenError = ERROR_SUCCESS;
			}
		}

		if (keyOpenError != ERROR_SUCCESS)
		{
			showInitError("Unable to open registry.");
		}
		else {
			DWORD dataSize = (strlen(dataDir) + 1) * sizeof(char);
			if (RegSetValueEx(key, TEXT("GameDirectory"), 0, REG_SZ, (LPBYTE)dataDir, dataSize) != ERROR_SUCCESS)
			{
				showInitError("Unable to set GameDirectory registry value.");
			}
			RegCloseKey(key);
		}
		SDL_free(dataDir);
	}
#endif
	//TODO handle this for Linux/Mac
}
#ifdef _WIN32
#include <windows.h>
int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	auto argc = __argc;
	auto argv = __argv;
#else
int main(int argc, char *argv[]) {
#endif
	Debug() << "ModShot version" << MODSHOT_VERSION;
	SDL_SetHint(SDL_HINT_VIDEO_MINIMIZE_ON_FOCUS_LOSS, "0");
	SDL_SetHint(SDL_HINT_ACCELEROMETER_AS_JOYSTICK, "0");
	SDL_SetHint(SDL_HINT_VIDEO_HIGHDPI_DISABLED, "1");
	SDL_SetHint(SDL_HINT_VIDEO_X11_NET_WM_BYPASS_COMPOSITOR, "0");

	/* initialize SDL first */
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER) < 0)
	{
		showInitError(std::string("Error initializing SDL: ") + SDL_GetError());
		return 0;
	}

#ifdef STEAM
	if (!STEAMSHIM_init())
	{
		showInitError("Could not initialize Steamworks API");
		return 0;
	}
#endif

	if (!EventThread::allocUserEvents())
	{
		showInitError("Error allocating SDL user events");
		return 0;
	}

#ifndef WORKDIR_CURRENT
	/* set working directory */
	char *dataDir = SDL_GetBasePath();
	if (dataDir)
	{
		int result = chdir(dataDir);
		(void)result;
		SDL_free(dataDir);
	}
#endif

	setGamePathInRegistry();

	/* Initialize physfs here so that config can call PHYSFS_getPrefDir */
	PHYSFS_init(argv[0]);

	loadLanguageMetadata();

	/* now we load the config */
	Config conf;
	conf.read(argc, argv);

	if (!conf.gameFolder.empty())
		if (chdir(conf.gameFolder.c_str()) != 0)
		{
			showInitError(std::string("Unable to switch into gameFolder ") + conf.gameFolder);
			return 0;
		}

#ifdef __WIN32
    // Create a debug console in debug mode
    if (conf.winConsole) {
      if (setupWindowsConsole()) {
        reopenWindowsStreams();
      } else {
        char buf[200];
        snprintf(buf, sizeof(buf), "Error allocating console: %lu",
                GetLastError());
        showInitError(std::string(buf));
      }
    }
#endif


	extern int screenMain(Config &conf);
	if (conf.screenMode)
		return screenMain(conf);

	if (conf.windowTitle.empty())
		conf.windowTitle = conf.game.title;

	int imgFlags = IMG_INIT_PNG;
	if (IMG_Init(imgFlags) != imgFlags)
	{
		showInitError(std::string("Error initializing SDL_image: ") + SDL_GetError());
		SDL_Quit();

		return 0;
	}

	if (TTF_Init() < 0)
	{
		showInitError(std::string("Error initializing SDL_ttf: ") + SDL_GetError());
		IMG_Quit();
		SDL_Quit();

		return 0;
	}

	#ifndef USE_FMOD
	if (Sound_Init() == 0)
	{
		showInitError(std::string("Error initializing SDL_sound: ") + Sound_GetError());
		TTF_Quit();
		IMG_Quit();
		SDL_Quit();

		return 0;
	}
	#endif

	SDL_Window *win;
	Uint32 winFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_INPUT_FOCUS; //| SDL_WINDOW_ALLOW_HIGHDPI;

	// #ifdef __APPLE__
	// 	winFlags |= SDL_WINDOW_RESIZABLE;
	// #endif

	if (conf.fullscreen)
		winFlags |= SDL_WINDOW_FULLSCREEN_DESKTOP;

	win = SDL_CreateWindow(conf.windowTitle.c_str(),
	                       SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
	                       conf.defScreenW, conf.defScreenH, winFlags);

	if (!win)
	{
		showInitError(std::string("Error creating window: ") + SDL_GetError());
		return 0;
	}

	/* OSX and Windows have their own native ways of
	 * dealing with icons; don't interfere with them */
#ifdef __LINUX__
	setupWindowIcon(conf, win);
#else
	(void) setupWindowIcon;
#endif

	#ifndef USE_FMOD

	ALCdevice *alcDev = alcOpenDevice(0);

	if (!alcDev)
	{
		showInitError("Error opening OpenAL device");
		SDL_DestroyWindow(win);
		TTF_Quit();
		IMG_Quit();
		SDL_Quit();

		return 0;
	}

	if(alcIsExtensionPresent(alcDev, "ALC_EXT_EFX") != ALC_TRUE) {
		showInitError("OpenAL device does not support Effects extension.");
	}
	#else
	//FMOD_RESULT result;
	//FMOD_STUDIO_SYSTEM *system = NULL;
	//
	//result = FMOD_Studio_System_Create(&system, FMOD_VERSION);
	//if (result != FMOD_OK) {
	//	showInitError(std::string("Error creating FMOD system: ") + FMOD_ErrorString(result));
	//	return 0;
	//}

	//result = FMOD_Studio_System_Initialize(system, conf.maxFmodChannels, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0);
	//if (result != FMOD_OK) {
	//	showInitError(std::string("Error initializing FMOD system: ") + FMOD_ErrorString(result));
	//	return 0;
	//}
	#endif

	SDL_DisplayMode mode;
	SDL_GetDisplayMode(0, 0, &mode);

	/* Can't sync to display refresh rate if its value is unknown */
	if (!mode.refresh_rate)
		conf.syncToRefreshrate = false;

	EventThread eventThread;
	RGSSThreadData rtData(&eventThread, win,
						  #ifndef USE_FMOD
	                      alcDev,
						  #else
						  // system,
						  #endif
						  mode.refresh_rate, conf);

#ifndef STEAM
	/* Add controller bindings from embedded controller DB */
	SDL_RWops *controllerDB = SDL_RWFromConstMem(___assets_gamecontrollerdb_txt,
	                                             ___assets_gamecontrollerdb_txt_len);
	SDL_GameControllerAddMappingsFromRW(controllerDB, 1);
#endif

	int winW, winH;
	SDL_GetWindowSize(win, &winW, &winH); // SDL_GL_GetDrawableSize(win, &winW, &winH);
	rtData.windowSizeMsg.post(Vec2i(winW, winH));

	/* Load and post key bindings */
	rtData.bindingUpdateMsg.post(loadBindings(conf));

	EventThread::inputMut = SDL_CreateMutex();

	/* Start RGSS thread */
	SDL_Thread *rgssThread =
	        SDL_CreateThread(rgssThreadFun, "rgss", &rtData);

	/* Start event processing */
	eventThread.process(rtData);

	if(!EventThread::forceTerminate) {
		/* Request RGSS thread to stop */
		rtData.rqTerm.set();

		/* Wait for RGSS thread response */
		for (int i = 0; i < 1000; ++i)
		{
			/* We can stop waiting when the request was ack'd */
			if (rtData.rqTermAck)
			{
				Debug() << "RGSS thread ack'd request after" << i*10 << "ms";
				break;
			}

			/* Give RGSS thread some time to respond */
			SDL_Delay(10);
		}

		Debug() << "Waiting for shutdown";
		/* If RGSS thread ack'd request, wait for it to shutdown,
		* otherwise abandon hope and just end the process as is. */
		if (rtData.rqTermAck)
			SDL_WaitThread(rgssThread, 0);
		else
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, conf.windowTitle.c_str(),
									"The RGSS script seems to be stuck and OneShot will now force quit", win);

		if (!rtData.rgssErrorMsg.empty())
		{
			Debug() << rtData.rgssErrorMsg;
			SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, conf.windowTitle.c_str(),
									rtData.rgssErrorMsg.c_str(), win);
		}
	}

	Debug() << "Cleaning up events";

	/* Clean up any remainin events */
	eventThread.cleanup();

	Debug() << "Shutting down.";

	unloadLocale();
	unloadLanguageMetadata();

	#ifndef USE_FMOD
	alcCloseDevice(alcDev);
	#endif
	SDL_DestroyWindow(win);

	#ifndef USE_FMOD
	Sound_Quit();
	#endif
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();

#ifdef STEAM
	STEAMSHIM_deinit();
#endif
	return 0;
}
