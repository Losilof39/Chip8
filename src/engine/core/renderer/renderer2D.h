#ifndef RENDERER2D_H
#define RENDERER2D_H

#include <glad/glad.h>
#include "core/data_types.h"
#include "r_shader.h"

void GLAPIENTRY
MessageCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam);
	
typedef struct texture2d
{
	char* filePath;
	u64 id;
	u16 width;
	u16 height;
	u8 bpp;
}texture2d;

void Renderer2D_Init(SDL_Window* window);
void Renderer2D_Cleanup();

void R2D_StartRendition(void);
void R2D_StopRendition(void);
//void R2D_DrawSprite(vec3* position, vec2 size, GLTexData* tex);
//void R2D_DrawSpriteFromName(vec3* position, vec2 size, const char* name);
void R2D_DrawColoredQuad(vec3 position, vec2 size, vec3 color);
void R2D_RenderTexturedQuad(vec3 position, vec2 size, texture2d texture);
void R2D_DrawText(char* text, float x, float y, float scale, vec3 color);
//void R2D_DrawLightSprite(vec3* position, vec2 size, GLTexData* tex, float light);		// sprite influenced by sector lighting

#endif