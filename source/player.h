#include <string>
#include "types.h"


class Player{
    private:
        float heightStanding;
        float heightLow;
        float visualHeightSmoothing;
        float moveX;
        float moveZ;
    public:
        float playerHorizRotation, playerVerticRotation;
        float visualHeight;
        float health;
        std::string name;
        vec3 position;
        float speed;
        float crouchSpeed;
        float accelerationSmoothing;
        float height;
        bool isCrouching;
        Player() {
            this->playerHorizRotation = 0.0f;
            this->playerVerticRotation = 0.0f;
            this->health = 100.0f;
            this->name = "builderman";
            this->position.x = 0.0f;
            this->position.y = 0.0f;
            this->position.z = 0.0f;
            this->speed = 0.1f;
            this->height = 1.5f;
            this->heightStanding = height;
            this->heightLow = 0.65f;
            this->visualHeight = 1.5f;
            this->visualHeightSmoothing = 1.2f;
            this->accelerationSmoothing = 4.0f;
            this->moveX = 0.0f;
            this->moveZ = 0.0f;
            this->isCrouching = false;
            this->crouchSpeed = 0.06f;
        }
        /// @brief Move the player based on its stats
        /// @param x Forward
        /// @param y Right
        void Move(float x, float z);
        void Crouch(bool should);
};