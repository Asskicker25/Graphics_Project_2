#pragma once

#include <glm/glm.hpp>

static glm::vec3 Lerp(const glm::vec3& start, const glm::vec3& end, float t) 
{
    // Ensure t is in the range [0, 1]
    t = glm::clamp(t, 0.0f, 1.0f);

    // Linear interpolation formula: result = start + t * (end - start)
    return start + t * (end - start);
}

// Function to calculate t for constant speed linear interpolation
static float calculateT(float currentT, float deltaTime, float lerpSpeed)
{
    // Calculate the step based on lerpSpeed
    float step = lerpSpeed * deltaTime;

    // Increment the current t by the step
    return glm::clamp(currentT + step, 0.0f, 1.0f);
}