#ifndef COMPONENT_H
#define COMPONENT_H

#include <string>

namespace moar
{

class Object;

class Component
{
public:
    enum Type
    {
        RENDERER,
        MATERIAL,
        LIGHT,
        CAMERA,
        CUSTOM
    };

    Component();
    virtual ~Component();
    Component(const Component&) = delete;
    Component(Component&&) = delete;
    Component& operator=(const Component&) = delete;
    Component& operator=(Component&&) = delete;

    virtual void execute() = 0;

    void setParent(Object* parentObject);

    Object* getParent() const;
    virtual std::string getName() = 0;
    virtual Component::Type getType() = 0;

protected:
    Object* parent;
};

} // moar

#endif // COMPONENT_H
