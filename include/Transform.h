#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

class Transform
{
public:
    Transform()
        : Translation(glm::vec3(0.0f)),
          Rotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)),  // Identity quaternion
          Scale(glm::vec3(1.0f))
    {}

    // Getters
    glm::vec3 GetTranslation() const { return Translation; }
    glm::vec3 GetLocation() const { return Translation; }
    glm::quat GetRotation() const { return Rotation; }
    glm::vec3 GetScale() const { return Scale; }

    // Setters
    void SetTranslation(const glm::vec3& v) { Translation = v; }
    void SetLocation(const glm::vec3& v) { Translation = v; }
    void SetRotation(const glm::vec3& eulerAngles) { Rotation = glm::quat(glm::radians(eulerAngles)); }
    void SetScale(const glm::vec3& v) { Scale = v; }

    // Directional Vectors
    glm::vec3 GetForwardVector() const { return glm::normalize(Rotation * glm::vec3(0.0f, 0.0f, -1.0f)); }

    glm::vec3 GetRightVector() const { return glm::normalize(Rotation * glm::vec3(1.0f, 0.0f, 0.0f)); }

    glm::vec3 GetUpVector() const { return glm::normalize(Rotation * glm::vec3(0.0f, 1.0f, 0.0f)); }

    // Transform Modifiers
    void AddTranslation(const glm::vec3& v) { Translation += v; }

    void Rotate(const glm::vec3& axisOfRotation, float amount)
    {
        glm::quat rotationQuat = glm::angleAxis(glm::radians(amount), glm::normalize(axisOfRotation));
        Rotation = glm::normalize(rotationQuat * Rotation);
    }

    // Transformation Matrix
    glm::mat4 GetMatrix() const
    {
        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), Translation);
        glm::mat4 rotationMatrix = glm::toMat4(Rotation);
        glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), Scale);
        return translationMatrix * rotationMatrix * scaleMatrix;
    }

private:
    glm::vec3 Translation;
    glm::quat Rotation;
    glm::vec3 Scale;
};
