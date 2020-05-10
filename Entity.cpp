#include "Entity.h"

Entity::Entity()
{
    position = glm::vec3(0);
    acceleration = glm::vec3(0);
    velocity = glm::vec3(0);
    modelMatrix = glm::mat4(1.0f);
    rotation = glm::vec3(0);
    scale = glm::vec3(1);

    int remainingLives = 3;
    billboard = false;

    width = 1.0f;
    height = 1.0f;
    depth = 1.0f;
    
    speed = 0.0f;

    jump = false;
    jumpPower = 0;
}


bool Entity::CheckCollision(Entity* other)
{
    float xdist = fabs(position.x - other->position.x) - ((width + other->width) / 2.0f);
    float ydist = fabs(position.y+1 - other->position.y) - ((height + other->height) / 2.0f);
    float zdist = fabs(position.z - other->position.z) - ((depth + other->depth) / 2.0f);
    if (xdist < 0 && ydist < 0 && zdist < 0) return true;
    return false;
}

bool Entity::CheckCollisionsY(Entity* objects, int objectCount)
{
    for (int i = 0; i < objectCount; i++)
    {
        Entity* object = &objects[i];
        if (CheckCollision(object))
        {
            float ydist = fabs(position.y - object->position.y);
            float penetrationY = fabs(ydist - (height / 2.0f) - (object->height / 2.0f));
            if (velocity.y > 0) {
                position.y -= penetrationY;
                velocity.y = 0;
                collidedTop = true;
                return true;
            }
            else if (velocity.y < 0) {
                position.y += penetrationY;
                velocity.y = 0;
                collidedBottom = true;
                stairIndex = i+1;
                return true;
            }
        }

    }
    return false;
}

bool Entity::CheckCollisionsX(Entity* objects, int objectCount)
{
    for (int i = 0; i < objectCount; i++)
    {
        Entity* object = &objects[i];
        if (CheckCollision(object))
        {
            float xdist = fabs(position.x - object->position.x);
            float penetrationX = fabs(xdist - (width / 2.0f) - (object->width / 2.0f));
            if (velocity.x > 0) {
                position.x -= penetrationX;
                velocity.x = 0;
                collidedRight = true;
                return true;
            }
            else if (velocity.x < 0) {
                position.x += penetrationX;
                velocity.x = 0;
                collidedLeft = true;
                return true;
            }
        }
    }
    return false;
}

void Entity::Update(float deltaTime, Entity* player, Entity* objects, int objectCount, Entity* enemies, int enemyCount)
{
    if (isActive == false) return;

    collidedTop = false;
    collidedBottom = false;
    collidedLeft = false;
    collidedRight = false;
    
    glm::vec3 previousPosition = position;

    if (billboard) {
        float directionX = position.x - player->position.x;
        float directionZ = position.z - player->position.z;
        rotation.y = glm::degrees(atan2f(directionX, directionZ));
        
        if (position.x >= 5.0f) {
            velocity.x = -0.5f;
        }
        if (position.x <= -5.0f) {
            velocity.x = 0.5f;
        }
    }

    if (jump) {
        velocity.y += jumpPower;
        jump = false;
    }

<<<<<<< HEAD
    if (jump == true) {
        jump == false;
        velocity.y += jumpPower;
    }

    velocity += acceleration * deltaTime;
    position += velocity * deltaTime;
=======
    velocity.y += acceleration.y * deltaTime;
    position += velocity * speed * deltaTime;
>>>>>>> Soo-V2
    
    if (entityType == PLAYER) {
        if (position.y < 0) { 
            position = glm::vec3(0, 3.0f, -0.9f);
            rotation = glm::vec3(0.0f, 0, 0);
            remainingLives -= 1;
        }
        for (int i = 0; i < objectCount; i++)
        {
<<<<<<< HEAD
            if (objects[i].entityType == FLOOR) continue;
            if (CheckCollision(&objects[i])) {
                position = previousPosition;
=======
            if (CheckCollisionsX(&objects[i], objectCount)) {
                position.x = previousPosition.x;
                break;
            }
            if (CheckCollisionsY(&objects[i], objectCount)) {
                position.y = previousPosition.y;
                break;
            }
        }
        for (int i = 0; i < enemyCount; i++)
        {
            if (CheckCollision(&enemies[i])) {
                remainingLives -= 1;
                position.z += 1.0f;
>>>>>>> Soo-V2
                break;
            }
            if (position.y < 1) { position.y = 1; }
        }
    }

    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    modelMatrix = glm::scale(modelMatrix, scale);
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
}

void Entity::DrawBillboard(ShaderProgram* program) {
    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };
    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);
    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

void Entity::Render(ShaderProgram *program) {
    program->SetModelMatrix(modelMatrix);
    
    glBindTexture(GL_TEXTURE_2D, textureID);

    if (billboard) {
        DrawBillboard(program);
    }
    else {
        mesh->Render(program);
    }
    
}



