#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1

#include <vector>
#include <string>
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_mixer.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#include "Util.h"
#include "Entity.h"


SDL_Window* displayWindow;
bool gameIsRunning = true;
bool gameStart = false;
bool gameEnd = false;

Mix_Music* music;
Mix_Chunk* bounce;
Mix_Chunk* success;
Mix_Chunk* failure;
Mix_Chunk* gameover;

enum GameResult {WIN, LOSE, PLAYING};
GameResult gameResult = PLAYING;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;
glm::mat4 uiViewMatrix, uiProjectionMatrix;
GLuint fontTextureID;
GLuint heartTextureID;



#define OBJECT_COUNT 100
#define ENEMY_COUNT 20

struct GameState {
    Entity *player;
    Entity* objects;
    Entity* enemies;
};

GameState state;

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("3D!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);
    
#ifdef _WINDOWS
    glewInit();
#endif
    
    glViewport(0, 0, 1280, 720);
    
    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");
    
    //Music
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4996);
    music = Mix_LoadMUS("background.mp3");
    Mix_PlayMusic(music, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 3);
    bounce = Mix_LoadWAV("bounce.wav");
    Mix_VolumeChunk(bounce, MIX_MAX_VOLUME / 3);
    success = Mix_LoadWAV("success.wav");
    Mix_VolumeChunk(success, MIX_MAX_VOLUME / 3);
    failure = Mix_LoadWAV("failure.wav");
    Mix_VolumeChunk(failure, MIX_MAX_VOLUME / 3);
    gameover = Mix_LoadWAV("gameover.wav");
    Mix_VolumeChunk(gameover, MIX_MAX_VOLUME / 3);

    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::perspective(glm::radians(45.0f), 1.777f, 0.1f, 100.0f);

    uiViewMatrix = glm::mat4(1.0);
    uiProjectionMatrix = glm::ortho(-6.4f, 6.4f, -3.6f, 3.6f, -1.0f, 1.0f);
    fontTextureID = Util::LoadTexture("font.png");
    heartTextureID = Util::LoadTexture("platformPack_item017.png");
    
    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    program.SetColor(1.0f, 1.0f, 1.0f, 1.0f);
    
    glUseProgram(program.programID);
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);


    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    

    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->remainingLives = 3;
    state.player->position = glm::vec3(0, 2.0f, -0.9f);
    state.player->rotation = glm::vec3(0.0f, 0, 0);
    state.player->acceleration = glm::vec3(0, -9.81f, 0);
    state.player->speed = 0.5f;
    state.player->jumpPower = 5.0f;
    
    
    state.objects = new Entity[OBJECT_COUNT];

    GLuint stairTextureID = Util::LoadTexture("defStair.png");
    GLuint endTextureID = Util::LoadTexture("endStair.jpg");
    Mesh* stairMesh = new Mesh();
    stairMesh->LoadOBJ("Realistic_Cube.obj", 1);

    for (int i = 0; i < OBJECT_COUNT; i++) {
        state.objects[i].mesh = stairMesh;
        state.objects[i].textureID = stairTextureID;
        state.objects[i].rotation = glm::vec3(0, 0, 0);
        state.objects[i].acceleration = glm::vec3(0, 0, 0);
        state.objects[i].scale = glm::vec3(1, 0.3f, 1);
        state.objects[i].entityType = STAIR;
        state.objects[i].position = glm::vec3(0, 0.5*i, -1-i);
    }
    state.objects[0].scale = glm::vec3(3, 0.3f, 3);
    state.objects[OBJECT_COUNT-1].textureID = endTextureID;

    state.enemies = new Entity[ENEMY_COUNT];

    GLuint enemyTextureID = Util::LoadTexture("enemy.png");
    for (int i = 0; i < ENEMY_COUNT; i++) {
        state.enemies[i].billboard = true;
        state.enemies[i].textureID = enemyTextureID;
        state.enemies[i].position = glm::vec3(0, 1.7f+(0.5*5*i), -3.0f - (5*i));
        //        state.enemies[i].position = glm::vec3(0, 0.5f, 0);
        //        state.enemies[i].position = glm::vec3(rand() % 20 - 10, 0.5, rand() % 20 - 10);
        state.enemies[i].rotation = glm::vec3(0, 0, 0);
        state.enemies[i].acceleration = glm::vec3(0, 0, 0);
        state.enemies[i].velocity.x = -0.5f;
        state.enemies[i].speed = 1.0f + (0.25 * i);
        
        
        //        state.enemies[i].velocity.z = -1.0f;
    }

}

void ProcessInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                gameIsRunning = false;
                break;
                
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym) {
                    case SDLK_SPACE:
                        if (gameStart && !gameEnd) {
                            if (state.player->collidedBottom) {
                                state.player->jump = true;
                                Mix_PlayChannel(-1, bounce, 0);
                            }
                            break;
                        }

                    case SDLK_RETURN:
                        if (!gameStart) {
                            Mix_PlayChannel(-1, success, 0);
                            gameStart = true;
                        }

                }
                break;
            
        }
    }
    const Uint8* keys = SDL_GetKeyboardState(NULL);
    if (gameStart && !gameEnd) {
        //if (keys[SDL_SCANCODE_LEFT]) {
        //    state.player->rotation.y += 1.0f;
        //}
        //else if (keys[SDL_SCANCODE_RIGHT]) {
        //    state.player->rotation.y -= 1.0f;
        //}

        state.player->velocity.x = 0;
        state.player->velocity.z = 0;

        if (keys[SDL_SCANCODE_UP]) {
            state.player->velocity.z = cos(glm::radians(state.player->rotation.y)) * -2.0f;
            state.player->velocity.x = sin(glm::radians(state.player->rotation.y)) * -2.0f;
        }
        else if (keys[SDL_SCANCODE_DOWN]) {
            state.player->velocity.z = cos(glm::radians(state.player->rotation.y)) * 2.0f;
            state.player->velocity.x = sin(glm::radians(state.player->rotation.y)) * 2.0f;
        }
    }



    
}

#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;

void Update() {
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;
    
    deltaTime += accumulator;
    if (deltaTime < FIXED_TIMESTEP) {
        accumulator = deltaTime;
        return;
    }
    
    while ((deltaTime >= FIXED_TIMESTEP) && !gameEnd) {
        for (int i = 0; i < OBJECT_COUNT; i++) {
            state.objects[i].Update(FIXED_TIMESTEP, state.player, state.objects, OBJECT_COUNT, state.enemies, ENEMY_COUNT);
        }

        for (int i = 0; i < ENEMY_COUNT; i++) {
            state.enemies[i].Update(FIXED_TIMESTEP, state.player, state.objects, OBJECT_COUNT, state.enemies, ENEMY_COUNT);
        }

        state.player->Update(FIXED_TIMESTEP, state.player, state.objects, OBJECT_COUNT, state.enemies, ENEMY_COUNT);

        deltaTime -= FIXED_TIMESTEP;
    }
    
    if (state.player->stairIndex == OBJECT_COUNT && state.player->position.y > 48) {
        gameEnd = true;
        gameResult = WIN;
    }
    else if (state.player->remainingLives == 0) {
        gameEnd = true;
        gameResult = LOSE;
    }

    accumulator = deltaTime;

    viewMatrix = glm::mat4(1.0f);
    viewMatrix = glm::rotate(viewMatrix, glm::radians(state.player->rotation.y), glm::vec3(0, -1.0f, 0));
    viewMatrix = glm::rotate(viewMatrix, glm::radians(state.player->rotation.x), glm::vec3(-0.5f, 0, 0));
    viewMatrix = glm::translate(viewMatrix, -state.player->position);
}


void Render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);
    
    if (gameStart && !gameEnd) {
        for (int i = 0; i < OBJECT_COUNT; i++) {
            state.objects[i].Render(&program);
        }

        for (int i = 0; i < ENEMY_COUNT; i++) {
            state.enemies[i].Render(&program);
        }
    }

    program.SetProjectionMatrix(uiProjectionMatrix);
    program.SetViewMatrix(uiViewMatrix);

    if (!gameStart) {
        Util::DrawText(&program, fontTextureID, "One Hundred Stairs", 1.5f, -0.95f, glm::vec3(-4.5f, 2.0f, -1.0f));
        Util::DrawText(&program, fontTextureID, "Press Enter to Start", 0.5, -0.3f, glm::vec3(-2.0f, 0, -1.0f));
    }
    else if (!gameEnd){
        if (state.player->position.y < 6.0f) {
            Util::DrawText(&program, fontTextureID, "Press Up & Down to Move", 0.5, -0.2f, glm::vec3(-6, 0, 0.0f));
            Util::DrawText(&program, fontTextureID, "Press Space to Jump", 0.5, -0.2f, glm::vec3(-6, -0.5f, 0.0f));
            Util::DrawText(&program, fontTextureID, "Don't Get Hit by the Ghost!", 0.5, -0.2f, glm::vec3(-6, -1.0f, 0.0f));
        }
        Util::DrawText(&program, fontTextureID, "Lives: " + std::to_string(state.player->remainingLives), 0.5, -0.3f, glm::vec3(-6, 3.2, 0));
        Util::DrawText(&program, fontTextureID, "Stairs Left: " + (std::to_string(OBJECT_COUNT - state.player->stairIndex)), 0.5, -0.25f, glm::vec3(-3, 3.2, 0));
        for (int i = 0; i < state.player->remainingLives; i++)
        {
            // These icons are small, so just move 0.5 to the right for each one.
            Util::DrawIcon(&program, heartTextureID, glm::vec3(5 + (i * 0.5f), 3.2, 0));
        }
    }
    else if (gameEnd && (gameResult == WIN)){
        Mix_PlayChannel(-1, success, 0);
        Util::DrawText(&program, fontTextureID, "YOU WIN", 0.5, -0.2f, glm::vec3(-1.7f, 0, -1.0f));
        //Mix_VolumeChunk(success, 0);
        Mix_HaltMusic();
    }
    else if (gameEnd && (gameResult == LOSE)) {
        Mix_PlayChannel(-1, failure, 0);
        Util::DrawText(&program, fontTextureID, "GAME OVER", 0.5, -0.2f, glm::vec3(-1.7f, 0, -1.0f));
        //Mix_VolumeChunk(gameover, 0);
        Mix_HaltMusic();
    }
    SDL_GL_SwapWindow(displayWindow);
}

void Shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();
    
    while (gameIsRunning) {
        ProcessInput();
        Update();
        Render();
    }
    
    Shutdown();
    return 0;
}
