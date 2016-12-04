#include "typemappings.h"

namespace moar
{

const std::vector<TypeMapping> textureTypeMappings =
{
   {aiTextureType_DIFFUSE, Material::DIFFUSE, Shader::DIFFUSE, DIFFUSE_DEFINE},
   {aiTextureType_SPECULAR, Material::SPECULAR, Shader::SPECULAR, SPECULAR_DEFINE},
   {aiTextureType_HEIGHT, Material::NORMAL, Shader::NORMAL, NORMAL_DEFINE},
   {aiTextureType_DISPLACEMENT, Material::BUMP, Shader::BUMP, BUMP_DEFINE}
};

} // moar
