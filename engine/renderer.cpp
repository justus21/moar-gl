#include "renderer.h"
#include "common/globals.h"

#include <glm/glm.hpp>

namespace moar
{

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

void Renderer::execute()
{
    if (shadowReceiver && shader->hasUniform(RECEIVE_SHADOWS_LOCATION)) {
        glUniform1i(RECEIVE_SHADOWS_LOCATION, static_cast<int>(shadowReceiver));
    }
    model->render();
}

void Renderer::setModel(Model* model)
{
    this->model = model;
    updateRequired = true;
}

void Renderer::setShadowCaster(bool caster)
{
    shadowCaster = caster;
}

void Renderer::setShadowReceiver(bool receiver)
{
    shadowReceiver = receiver;
}

Model* Renderer::getModel()
{
    return model;
}

bool Renderer::isShadowCaster() const
{
    return shadowCaster;
}

std::string Renderer::getName()
{
    return "Renderer";
}

Component::Type Renderer::getType()
{
    return Component::Type::RENDERER;
}

} // moar

