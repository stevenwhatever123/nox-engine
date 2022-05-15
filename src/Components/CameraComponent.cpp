#include <Components/CameraComponent.h>

using namespace NoxEngine;

CameraComponent::CameraComponent(): cam(vec3(10.0f, 10.0f, 10.0f)) {

}

CameraComponent::CameraComponent(const CameraComponent& other): cam(other.cam) {

}
