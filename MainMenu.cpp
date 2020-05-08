#include "MainMenu.h"
#include "Util.h"
#include "Mesh.h"

#define MAINMENU_ENEMYCOUNT 1
#define MAINMENU_OBJECTCOUNT 5


void MainMenu::Initialize() {
	state.sceneType = MAINMENU;
	state.nextScene = -1;

    // Initialize Game Objects
	state.player = new Entity();
	state.player->isActive = false;

	state.text = new Entity();
	GLuint textTextureID = Util::LoadTexture("font.png");
	state.text->textureID = textTextureID;
	state.EndOfGame = false;

	GLuint stairTextureID = Util::LoadTexture("defStair.jpg");
	Mesh* stairMesh = new Mesh();
	stairMesh->LoadOBJ("Realistic_Cube.obj", 1);
	for (int i = 0; i < LEVEL_OBJECT_COUNT; i++) {
		state.objects[i].mesh = stairMesh;
		state.objects[i].textureID = stairTextureID;
		state.objects[i].rotation = glm::vec3(0, 0, 0);
		state.objects[i].acceleration = glm::vec3(0, 0, 0);
		state.objects[i].scale = glm::vec3(1, 0.3f, 1);
		state.objects[i].entityType = STAIR;
	}

	state.objects[0].position = glm::vec3(0, 0.5f, -2);
	state.objects[1].position = glm::vec3(0, 1.0f, -3);
	state.objects[2].position = glm::vec3(0, 1.5f, -4); 
	state.objects[3].position = glm::vec3(0, 2.0f, -5); 
	state.objects[4].position = glm::vec3(0, 2.5f, -6);

}
void MainMenu::Update(float deltaTime) {
	state.player->Update(deltaTime, state.player, state.enemies, MAINMENU_ENEMYCOUNT, state.map);
	
	for (int i = 0; i < MAINMENU_OBJECTCOUNT; i++) {
		state.objects[i].Update(FIXED_TIMESTEP, state.player, state.objects, MAINMENU_OBJECTCOUNT);
	}

}
void MainMenu::Render(ShaderProgram* program) {
	state.player->Render(program);

	for (int i = 0; i < MAINMENU_OBJECTCOUNT; i++) {
		state.objects[i].Render(&program);
	}
	//Util::DrawText(&program, currentScene->state.text->textureID, "Infinite Stairs", 0.7f, -0.25f, glm::vec3(0.0f, 3.0f, -1.0f));
	//Util::DrawText(&program, currentScene->state.text->textureID, "Press Enter to Start", 0.5f, -0.25f, glm::vec3(0.0f, 2.0f, -1.0f));

}