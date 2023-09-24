#define M_PI 3.14159265358979323846
#define _USE_MATH_DEFINES
#include <cmath>

#include "player.h"
#include "types.h"


void Player::Move(float x, float z)
{
    // Convert the player's rotation angle to radians
    float radians = playerHorizRotation * M_PI / 180;

    float targetMoveX;
    float targetMoveZ;

    // Calculate the desired movement direction without taking rotation into account
    if (isCrouching){
        targetMoveX = x * crouchSpeed;
        targetMoveZ = z * crouchSpeed;
    } else {
        targetMoveX = x * speed;
        targetMoveZ = z * speed;
    }

    // Calculate the difference between current and target movement values
    float deltaMoveX = targetMoveX - moveX;
    float deltaMoveZ = targetMoveZ - moveZ;

    // Apply linear interpolation to smoothen the movement
    moveX += deltaMoveX * accelerationSmoothing * (1.0f / 30.0f);
    moveZ += deltaMoveZ * accelerationSmoothing * (1.0f / 30.0f);

    // Calculate the new position
    position.x += moveX * cos(radians) - moveZ * sin(radians);
    position.z += moveX * sin(radians) + moveZ * cos(radians);
}

void Player::Crouch(bool should)
{
    if (should) {
        height = heightLow;
        visualHeight = (visualHeight - heightLow) / visualHeightSmoothing + heightLow;
    } else {
        height = heightStanding;
        visualHeight = (visualHeight - heightStanding) / visualHeightSmoothing + heightStanding;
    }
}

