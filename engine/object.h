#ifndef OBJECT_H
#define OBJECT_H

#include "shader.h"
#include "model.h"
#include "material.h"
#include "light.h"

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <memory>
#include <vector>
#include <typeinfo>
#include <string>
#include <iostream>

namespace moar
{

class Object
{
    friend class Engine;

public:
    struct MeshObject
    {
        Mesh* mesh;
        Material* material;
        Object* parent;

        bool operator==(const MeshObject& rhs) {
            return this->mesh == rhs.mesh && this->material == rhs.material && this->parent == rhs.parent;
        }
    };

    static const glm::vec3 FORWARD;
    static const glm::vec3 UP;
    static const glm::vec3 LEFT;

    static const glm::mat4* projection;
    static const glm::mat4* view;

    static void setMeshDefaultMaterial(Material* material);
    static void setViewMatrixUniform();

    explicit Object();
    virtual ~Object();
    Object(const Object&) = delete;
    Object(Object&&) = delete;
    Object& operator=(const Object&) = delete;
    Object& operator=(Object&&) = delete;

    virtual void move(const glm::vec3& translation);
    virtual void rotate(const glm::vec3& axis, float amount);

    virtual void setPosition(const glm::vec3& position);
    virtual void setRotation(const glm::vec3& rotation);
    virtual void setScale(const glm::vec3& scale);

    unsigned int getId() const;
    glm::vec3 getPosition() const;
    glm::vec3 getRotation() const;
    glm::vec3 getScale() const;
    glm::vec3 getForward() const;
    glm::vec3 getUp() const;
    glm::vec3 getLeft() const;

    std::vector<MeshObject>& getMeshObjects();

    void setShadowCaster(bool caster);
    void setShadowReceiver(bool receiver);
    bool isShadowCaster() const;

    template<typename T>
    T* addComponent();

    template<typename T>
    T* addComponent(T* component);

    template<typename T>
    T* getComponent() const;

    template<typename T>
    bool hasComponent() const;

protected:
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
    glm::vec3 forward = FORWARD;
    glm::vec3 up = UP;
    glm::vec3 left = LEFT;

private:
    static unsigned int idCounter;
    static GLuint transformationBlockBuffer;
    static Material* defaultMaterial;

    void setUniforms(const Shader* shader);
    void updateModelMatrix();
    glm::mat4x4 getModelMatrix() const;

    unsigned int id;

    bool shadowCaster = true;
    bool shadowReceiver = true;

    glm::mat4x4 modelMatrix;

    std::unique_ptr<Light> light = nullptr;
    Model* model = nullptr;
    std::vector<MeshObject> meshObjects;
};

template<typename T>
T* Object::addComponent()
{
    std::cerr << "WARNING: Could not add a component\n";
    return nullptr;
}

template<typename T>
T* Object::addComponent(T*)
{
    std::cerr << "WARNING: Could not add a component\n";
    return nullptr;
}

template<typename T>
T* Object::getComponent() const
{
    std::cerr << "WARNING: Could not get a component\n";
    return nullptr;
}

template<typename T>
bool Object::hasComponent() const
{
    return getComponent<T>() != nullptr;
}

template<>
inline Light* Object::addComponent<Light>()
{
    G_COMPONENT_CHANGED = true;
    light.reset(new Light);
    return light.get();
}

template<>
inline Model* Object::addComponent<Model>(Model* model)
{
    G_COMPONENT_CHANGED = true;
    this->model = model;
    meshObjects.clear();
    for (const auto& mesh : model->getMeshes()) {
        Material* mat = mesh->getDefaultMaterial();
        MeshObject mo = {mesh.get(), (mat == nullptr ? defaultMaterial : mat), this};
        meshObjects.push_back(mo);
    }
    return model;
}

template<>
inline Model* Object::getComponent<Model>() const
{
    return model;
}

template<>
inline Light* Object::getComponent<Light>() const
{
    return light.get();
}

} // moar

#endif // OBJECT_H
