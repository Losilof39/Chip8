#include "core/core.h"
#include <glad/glad.h>
#include "app.h"

static Application* appPtr;

void Application_Startup()
{
    // Creates Window
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
        log_error("Failed to init SDL3");

    log_info("SDL3 init successful");

    if(appPtr->width == 0 || appPtr->height == 0)
    {
        appPtr->width = (u32)WINDOW_DEFAULT_WIDTH;
        appPtr->height = (u32)WINDOW_DEFAULT_HEIGHT;
    }

    appPtr->pWindow = SDL_CreateWindow((const char*)appPtr->title,
                        appPtr->width, appPtr->height, SDL_WINDOW_OPENGL);


    if ( !(appPtr->pWindow) )
    {
        log_error("Failed to create a SDL window!");
    }

    log_info("Window created");

    SDL_ShowWindow(appPtr->pWindow);

    SDL_ShowCursor();

    log_info("Initializing OpenGL 4.6 ...");

    log_info("Init zone memory system...");
    Z_Init();
    //Filesystem_Init();
    log_info("Init audio system...");
    Audio_Init();
    log_info("Init input system...");
    Input_Init();
    log_info("Init renderer system...");
    Renderer2D_Init(appPtr->pWindow);
}

void Application_Update()
{
    SDL_Event ev;
    SDL_Time start;
    SDL_Time end;
    f32 delta;

    appPtr->exit = false;

    while ( (!appPtr->exit) )
    {
        start = SDL_GetTicks();

        R2D_StartRendition();

        appPtr->Update();

        while (SDL_PollEvent(&ev))
        {
            switch (ev.type)
            {
            case SDL_EVENT_QUIT:
            {
                appPtr->exit = true;
            }break;

            case SDL_EVENT_KEY_DOWN:
            case SDL_EVENT_KEY_UP:
            {
                Input_ProcessKey(&ev.key);
            }break;

            case SDL_EVENT_MOUSE_BUTTON_DOWN:
            case SDL_EVENT_MOUSE_BUTTON_UP:
            {
                Input_ProcessMouseButton(&ev.button);
            }break;

            case SDL_EVENT_MOUSE_MOTION:
            {
                Input_ProcessMouseMotion(&ev.motion);
            }break;

            default:
                break;
            }
        }

        R2D_StopRendition();

        end = SDL_GetTicks();

        delta = (f32)(end - start) / 1000.0f;

        Time_SetDeltaTime(delta);
    }
}

u8 Application_GetHeapSize()
{
    return appPtr->mbUsing;
}

void Application_Cleanup()
{
    appPtr->Cleanup();

    Z_FreeMemory();
    Input_Cleanup();
    Renderer2D_Cleanup();
    Audio_Cleanup();

    SDL_DestroyWindow(appPtr->pWindow);
    SDL_Quit();

    log_info("Engine Shutdown");
}

u32 Application_GetWindowWidth()
{
    return appPtr->width;
}

u32 Application_GetWindowHeight()
{
    return appPtr->height;
}

void Application_Run(Application* app)
{
    appPtr = app;

    Application_Startup();

    appPtr->Init();

    Application_Update();
    Application_Cleanup();
}