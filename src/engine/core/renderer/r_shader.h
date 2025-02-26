#ifndef R_SHADER_H
#define R_SHADER_H


#include <cglm.h>

typedef struct Shader
{
	const char* name;
	unsigned int programID;
} Shader;

char* Shader_GetSource(const char* fileName);
Shader Shader_Create(const char* name, const char* pVertPath, const char* pFragPath);
void Shader_Use(Shader shader);
void Shader_Unbind();
void Shader_SetInt(Shader shader, const char* uniform, int value);
void Shader_SetBoolean(Shader shader, const char* uniform, bool value);
void Shader_SetFloat(Shader shader, const char* uniform, float value);
void Shader_SetMat4(Shader shader, const char* uniform, mat4 value);
void Shader_SetVec3(Shader shader, const char* uniform, vec3 value);
void Shader_SetVec4(Shader shader, const char* uniform, vec4 value);

#endif