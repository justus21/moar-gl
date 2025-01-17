#ifndef TYPEMAPPINGS
#define TYPEMAPPINGS

#include "globals.h"
#include "../material.h"
#include "../shader.h"
#include "../light.h"

#include <assimp/scene.h>

#include <string>
#include <vector>
#include <map>

namespace moar
{

struct TypeMapping
{
    aiTextureType aiType;
    Material::TextureType materialType;
    Shader::Type shaderType;
    std::string shaderDefine;
};

extern const std::vector<TypeMapping> TEXTURE_TYPE_MAPPINGS;
extern const std::map<Light::Type, std::string> LIGHT_SHADER_NAME_MAPPINGS;

} // moar

#endif // TYPEMAPPINGS

