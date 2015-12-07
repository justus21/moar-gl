#include "myapp.h"
#include "../engine/common/globals.h"
#include "../engine/renderer.h"
#include "../engine/material.h"

#include <boost/math/constants/constants.hpp>
#include <cmath>
#include <memory>

MyApp::MyApp() :
    camera(nullptr),
    input(nullptr),
    renderSettings(nullptr),
    offset(nullptr),
    bar(nullptr),
    monkey1(nullptr),
    monkey2(nullptr),
    monkey3(nullptr),
    icosphere(nullptr),
    light1(nullptr),
    light2(nullptr),
    light3(nullptr),
    dirLight(nullptr),
    rotationAxis(0.0f, 1.0f, 0.0f),
    rotationSpeed(0.5f),
    fps(0),
    fpsCounter(0),
    drawCount(&moar::DRAW_COUNT),
    timeCounter(0.0)
{
}

MyApp::~MyApp()
{
}

void MyApp::start()
{
    camera = engine->getCamera();
    input = engine->getInput();
    renderSettings = engine->getRenderSettings();

    moar::Object* planeBot = createRenderObject("diffuse", "plane.3ds", "white.png");
    planeBot->setPosition(glm::vec3(0.0f, -1.0f, 0.0f));
    planeBot->setScale(glm::vec3(10.0f, 1.0f, 10.0f));
    planeBot->setName("plane");

    moar::Object* planeR = createRenderObject("diffuse", "plane.3ds", "white.png");
    planeR->setPosition(glm::vec3(5.0f, 0.0f, 0.0f));
    planeR->setScale(glm::vec3(10.0f, 1.0f, 10.0f));
    planeR->rotate(glm::vec3(0.0f, 0.0f, 1.0f), 1.57f);

    moar::Object* planeL = createRenderObject("diffuse", "plane.3ds", "white.png");
    planeL->setPosition(glm::vec3(-5.0f, 0.0f, 0.0f));
    planeL->setScale(glm::vec3(10.0f, 1.0f, 10.0f));
    planeL->rotate(glm::vec3(0.0f, 0.0f, 1.0f), 1.57f);

//    moar::Object* planeF = createRenderObject("diffuse", "plane.3ds", "white.png");
//    planeF->setPosition(glm::vec3(0.0f, 0.0f, -5.0f));
//    planeF->setScale(glm::vec3(10.0f, 1.0f, 10.0f));
//    planeF->rotate(glm::vec3(1.0f, 0.0f, 0.0f), 1.57f);

    moar::Object* planeB = createRenderObject("diffuse", "plane.3ds", "white.png");
    planeB->setPosition(glm::vec3(0.0f, 0.0f, 5.0f));
    planeB->setScale(glm::vec3(10.0f, 1.0f, 10.0f));
    planeB->rotate(glm::vec3(1.0f, 0.0f, 0.0f), 1.57f);


    monkey1 = createRenderObject("diffuse", "monkey.3ds", "checker.png");
    monkey1->setPosition(glm::vec3(0.0f, 0.0f, -3.0f));
    monkey1->setName("diffuse_monkey");

    monkey2 = createRenderObject("specular", "monkey.3ds", "checker.png");
    monkey2->setPosition(glm::vec3(3.0f, 0.0f, 0.0f));
    monkey2->setName("specular_monkey");
    moar::Material* mat = monkey2->getComponent<moar::Material>();
    mat->setUniform("specularity", std::bind(glUniform1f, moar::SPECULAR_LOCATION, 50.0f), moar::SPECULAR_LOCATION);

    icosphere = createRenderObject("normalmap", "icosphere.3ds", "brick.png");
    icosphere->setPosition(glm::vec3(-3.0f, 0.0f, 0.0f));
    icosphere->setName("icosphere");
    mat = icosphere->getComponent<moar::Material>();
    mat->setTexture(engine->getResourceManager()->getTexture("brick_nmap.png"), moar::Material::TextureType::NORMAL, GL_TEXTURE_2D);

    light1 = createLight(glm::vec4(0.0f, 1.0f, 0.0f, 3.0f));
    light1->setPosition(glm::vec3(0.0f, 1.5f, -3.0f));
    light2 = createLight(glm::vec4(1.0f, 0.0f, 0.0f, 5.0f));
    light2->setPosition(glm::vec3(0.0f, 1.5f, 0.0f));
//    light2->getComponent<moar::Light>()->setShadowingEnabled(false);
//    light3 = createLight(glm::vec4(0.0f, 0.0f, 1.0f, 5.0f));
//    light3->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    dirLight = createLight(glm::vec4(1.0f, 1.0f, 1.0f, 1.2f), moar::Light::DIRECTIONAL);
    dirLight->setPosition(glm::vec3(0.0f, 3.0f, -10.0f));
    dirLight->setRotation(glm::vec3(-0.7f, 3.14f, 0.0f));
//    offset = camera->addPostprocess("offset", engine->getResourceManager()->getShader("offset"), 1);
//    offset->setUniform("screensize", std::bind(glUniform2f, moar::SCREEN_SIZE_LOCATION, renderSettings->windowWidth, renderSettings->windowHeight));
//    camera->addPostprocess("invert", engine->getResourceManager()->getShader("invert"), 1);

    initGUI();
}

void MyApp::handleInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        return;
    } else if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_RELEASE)  {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        quit();
    }

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        camera->move(camera->getForward() * input->getMovementSpeed());
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        camera->move(-camera->getForward() * input->getMovementSpeed());
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        camera->move(camera->getLeft() * input->getMovementSpeed());
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        camera->move(-camera->getLeft() * input->getMovementSpeed());
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        camera->setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
        camera->setRotation(glm::vec3(0.0f, 0.0f, 0.0f));
    }

    camera->rotate(moar::Object::UP, -input->getCursorDeltaX() * boost::math::constants::degree<double>());
    camera->rotate(moar::Object::LEFT, input->getCursorDeltaY() * boost::math::constants::degree<double>());
}

void MyApp::update(double time, double deltaTime)
{
    timeCounter += deltaTime;
    if (timeCounter < 1.0) {
        ++fpsCounter;
    } else {
        fps = fpsCounter;
        timeCounter = 0.0;
        fpsCounter = 0;
    }

    monkey1->rotate(rotationAxis, rotationSpeed * boost::math::constants::degree<double>());
    monkey2->rotate(rotationAxis, std::fabs(sin(time)) * rotationSpeed * boost::math::constants::degree<double>());

    light1->move(glm::vec3(0.0f, sin(time) * 0.01f, 0.0f));
    light2->move(glm::vec3(0.0f, cos(time) * 0.01f, 0.0f));

//    offset->setUniform("time", std::bind(glUniform1f, moar::TIME_LOCATION, glfwGetTime()));
}

void MyApp::initGUI()
{
    bar = TwNewBar("TweakBar");
    TwAddVarRW(bar, "axis", TW_TYPE_DIR3F, &rotationAxis, "");
    TwAddVarRW(bar, "speed", TW_TYPE_FLOAT, &rotationSpeed, "");
    TwAddVarRO(bar, "fps", TW_TYPE_INT32, &fps, "");
    TwAddVarRO(bar, "draw count", TW_TYPE_UINT32, drawCount, "");
}

moar::Object* MyApp::createRenderObject(const std::string& shader, const std::string& modelName, const std::string& textureName)
{
    moar::Object* renderObj= engine->createObject();
    moar::Renderer* renderer = renderObj->addComponent<moar::Renderer>();
    moar::Model* model = engine->getResourceManager()->getModel(modelName);
    renderer->setModel(model);

    moar::Material* material = renderObj->addComponent<moar::Material>();
    GLuint texture = 0;
    if (!textureName.empty()) {
        texture = engine->getResourceManager()->getTexture(textureName);
    }
    material->setShaderType(shader);
    material->setTexture(texture, moar::Material::TextureType::DIFFUSE, GL_TEXTURE_2D);
    if (shader == "diffuse") {
        material->setUniform("solidColor",
                             std::bind(glUniform3f, moar::SOLID_COLOR_LOCATION, 1.0f, 1.0f, 1.0f),
                             moar::SOLID_COLOR_LOCATION);
    }

    return renderObj;
}

moar::Object* MyApp::createLight(const glm::vec4& color, moar::Light::Type type)
{
    moar::Object* light = engine->createObject();
    light->setScale(glm::vec3(0.1f, 0.1f, 0.1f));
    moar::Light* lightComponent = light->addComponent<moar::Light>();
    lightComponent->setColor(color);
    lightComponent->setType(type);

    moar::Material* material = light->addComponent<moar::Material>();
    material->setShaderType("diffuse");
    GLuint texture = engine->getResourceManager()->getTexture("white.png");
    material->setTexture(texture, moar::Material::TextureType::DIFFUSE, GL_TEXTURE_2D);
    material->setUniform("solidColor",
                         std::bind(glUniform3f, moar::SOLID_COLOR_LOCATION, color.x, color.y, color.z),
                         moar::SOLID_COLOR_LOCATION);

    moar::Renderer* renderer = light->addComponent<moar::Renderer>();
    std::string modelName = type == moar::Light::POINT ? "sphere.3ds" : "cylinder.3ds";
    moar::Model* model = engine->getResourceManager()->getModel(modelName);
    renderer->setModel(model);
    renderer->setShadowCaster(false);
    renderer->setShadowReceiver(false);

    return light;
}
