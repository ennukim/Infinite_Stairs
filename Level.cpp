#include "Level.h"
#include "Util.h"
#include "Mesh.h"

#define LEVEL_ENEMY_COUNT 1
#define LEVEL_OBJECT_COUNT 60


void Level::Initialize() {
    state.sceneType = LEVEL;
    state.nextScene = -1;
    state.EndOfGame = false;

    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(0, 1.0f, -2);
    state.player->acceleration = glm::vec3(0, -9.81f, 0);
    state.player->speed = 1.0f;
    state.player->jumpPower = 0.1f;

    state.objects = new Entity[LEVEL_OBJECT_COUNT];


    GLuint floorTextureID = Util::LoadTexture("StoneFloor_Diffuse.jpg");
    Mesh* floorMesh = new Mesh();
    floorMesh->LoadOBJ("Realistic_Cube.obj", 20);
    state.objects[0].mesh = floorMesh;
    state.objects[0].textureID = floorTextureID;
    state.objects[0].position = glm::vec3(0, -3.0f, 0);
    state.objects[0].rotation = glm::vec3(0, 0, 0);
    state.objects[0].acceleration = glm::vec3(0, 0, 0);
    state.objects[0].scale = glm::vec3(100, 0.5f, 100);
    state.objects[0].entityType = FLOOR;

    GLuint stairTextureID = Util::LoadTexture("defStair.jpg");
    GLuint badTextureID = Util::LoadTexture("badStair.jpg");
    GLuint endTextureID = Util::LoadTexture("endStair.jpg");
    Mesh* stairMesh = new Mesh();
    stairMesh->LoadOBJ("Realistic_Cube.obj", 1);

    for (int i = 1; i < LEVEL_OBJECT_COUNT; i++) {
        state.objects[i].mesh = stairMesh;
        state.objects[i].textureID = stairTextureID;
        state.objects[i].rotation = glm::vec3(0, 0, 0);
        state.objects[i].acceleration = glm::vec3(0, 0, 0);
        state.objects[i].scale = glm::vec3(1, 0.3f, 1);
        state.objects[i].entityType = STAIR;
    }


    state.objects[1].position = glm::vec3(0, 0.5f, -2);
    state.objects[2].position = glm::vec3(0, 1.0f, -3);
    state.objects[3].position = glm::vec3(0, 1.5f, -4); // first three steps are stright baby steps
    state.objects[4].position = glm::vec3(1, 2.0f, -5); // Implement a right step
    state.objects[5].position = glm::vec3(0, 2.5f, -6); // Implement a left step
    state.objects[6].position = glm::vec3(0, 2.0f, -7); // Implement a down step
    state.objects[7].position = glm::vec3(0, 2.5f, -8);
    state.objects[8].position = glm::vec3(-1, 3.0f, -9);
    state.objects[9].position = glm::vec3(-2, 3.5f, -10); // Implement a backturn step
    state.objects[10].position = glm::vec3(-3, 4.0f, -9);
    state.objects[11].position = glm::vec3(-4, 4.5f, -8);
    state.objects[12].position = glm::vec3(-4, 5.0f, -7);
    state.objects[13].position = glm::vec3(-4, 5.5f, -6);
    state.objects[14].position = glm::vec3(-4, 5.5f, -4); // Implement a jump step 
    state.objects[15].position = glm::vec3(-4, 6.0f, -3);
    state.objects[16].position = glm::vec3(-3, 6.5f, -2);
    state.objects[17].position = glm::vec3(-2, 7.0f, -1); // Turn directions again
    state.objects[18].position = glm::vec3(-1, 7.5f, -2);
    state.objects[19].position = glm::vec3(-1, 8.0f, -3); // 20 stairs - basics
    state.objects[20].position = glm::vec3(-1, 8.5f, -4);
    state.objects[21].position = glm::vec3(-2, 9.0f, -5); // We give a choice - a bad stair appear for the first time.
    state.objects[22].position = glm::vec3(0, 9.0f, -5);
    state.objects[23].position = glm::vec3(-1, 9.5f, -6);
    state.objects[24].position = glm::vec3(0, 10.0f, -7);
    state.objects[25].position = glm::vec3(0, 10.0f, -9); // Another Jump - Introduce moving AI here
    state.objects[26].position = glm::vec3(1, 10.5f, -10);
    state.objects[27].position = glm::vec3(2, 11.0f, -11);
    state.objects[28].textureID = badTextureID;
    state.objects[28].position = glm::vec3(3, 12.0f, -12); // introduce brittle blocks
    state.objects[29].textureID = badTextureID;
    state.objects[29].position = glm::vec3(3, 12.5f, -13); // brittle
    state.objects[30].textureID = badTextureID;
    state.objects[30].position = glm::vec3(3, 13.0f, -14); // brittle
    state.objects[31].position = glm::vec3(4, 13.5f, -15); // Good Block
    state.objects[32].textureID = badTextureID;
    state.objects[32].position = glm::vec3(2, 13.5f, -15); // Bad Block
    state.objects[33].position = glm::vec3(5, 14.0f, -16);
    state.objects[34].position = glm::vec3(5, 14.5f, -17);
    state.objects[35].position = glm::vec3(5, 13.5f, -19); // Down Long Jump //May 7th 2020
    state.objects[36].position = glm::vec3(5, 14.0f, -20);
    state.objects[37].position = glm::vec3(6, 14.5f, -21);
    state.objects[38].position = glm::vec3(7, 15.0f, -22);
    state.objects[39].position = glm::vec3(8, 15.5f, -23);
    state.objects[40].position = glm::vec3(7, 16.0f, -24);
    state.objects[41].position = glm::vec3(6, 16.5f, -23); // backward turn again
    state.objects[42].position = glm::vec3(5, 17.0f, -22);
    state.objects[43].position = glm::vec3(4, 17.5f, -21);
    state.objects[44].position = glm::vec3(3, 18.0f, -20);
    state.objects[45].position = glm::vec3(1, 18.0f, -19); // X - dividing tiles - Obstacle Avoid Section
    state.objects[46].position = glm::vec3(1, 18.0f, -21);
    state.objects[47].position = glm::vec3(-1, 18.0f, -22); // 3 divided jump tiles - Obstacle
    state.objects[48].position = glm::vec3(-1, 18.0f, -20);
    state.objects[49].position = glm::vec3(-1, 18.0f, -18);
    state.objects[50].position = glm::vec3(-2, 18.5f, -19); // two tiles rising up
    state.objects[51].position = glm::vec3(-2, 18.5f, -21);
    state.objects[52].position = glm::vec3(-3, 19.0f, -19); // Three stairs - before next obstacle
    state.objects[53].position = glm::vec3(-3, 19.0f, -20);
    state.objects[54].position = glm::vec3(-3, 19.0f, -21);
    state.objects[55].position = glm::vec3(-4, 19.5f, -19); // Rising obstacles - 3 tiles that interchange in being an obstacle
    state.objects[56].position = glm::vec3(-4, 19.5f, -20);
    state.objects[57].position = glm::vec3(-4, 19.5f, -21);
    state.objects[58].position = glm::vec3(-5, 20.0f, -21); // Two Steps to End the game
    state.objects[59].textureID = endTextureID;
    state.objects[59].position = glm::vec3(-6, 20.5f, -22);

    state.text = new Entity();
    GLuint textTextureID = Util::LoadTexture("font.png");
    state.text->textureID = textTextureID;

    state.enemies = new Entity[LEVEL_ENEMY_COUNT];
    state.enemies[0].isActive = false;
}
void Level::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.objects, LEVEL_OBJECT_COUNT);

    for (int i = 0; i < LEVEL_OBJECT_COUNT; i++) {
        state.objects[i].Update(deltaTime, state.player, state.objects, LEVEL_OBJECT_COUNT);
    }

    for (int i = 0; i < LEVEL_ENEMY_COUNT; i++) {
        state.enemies[i].Update(deltaTime, state.player, state.objects, LEVEL_OBJECT_COUNT);
    }

	if (state.player->position.x >= 33) {
		state.nextScene = 2;
	}
    if (state.player->life == 0) {
        state.EndOfGame = true;
    }
}
void Level::Render(ShaderProgram* program) {
    for (int i = 0; i < LEVEL_OBJECT_COUNT; i++) {
        state.objects[i].Render(program);
    }

    for (int i = 0; i < LEVEL_ENEMY_COUNT; i++) {
        state.enemies[i].Render(program);
    }


}