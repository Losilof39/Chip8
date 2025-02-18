#ifndef APP_H
#define APP_H

#include "core/data_types.h"

#define WINDOW_DEFAULT_WIDTH 640
#define WINDOW_DEFAULT_HEIGHT 480

typedef struct Application
{
	char* title;
	char* version;
	u32 width;
	u32 height;
	SDL_Window* pWindow;
	b8 exit;
	u32 mbUsing;

	void (*Init)();
	void (*Update)();
	void (*Cleanup)();

}Application;

void Application_Startup();
void Application_Run(Application* app);
void Application_Update();
u8 Application_GetHeapSize();
void Application_Cleanup();
u32 Application_GetWindowWidth();
u32 Application_GetWindowHeight();

#endif