#include "window.hpp"
#include "basic.h"
#include "SDL_ttf.h"
#include "SDL_image.h"
#include "map.h"
#include <random>

bas::texture menuScreen;
dynamicObject player;
map donkeyMap;
physicsWorld world;

#define barrel_amou 120

void window::setup() {
    srand(time(NULL));

    TTF_Init();
    IMG_Init(IMG_INIT_PNG);

    donkeyMap.setupColliders(&world);
    donkeyMap.barrels.resize(barrel_amou);
    donkeyMap.barrelsTextures.resize(barrel_amou);
    for (int i = 0; i < barrel_amou; i++) {
        donkeyMap.barrelsTextures[i].loadMedia(renderer, "Textures/Barrels.png");
        world.addDynamicObject(&donkeyMap.barrels[i]);
        donkeyMap.barrels[i].collider.center.y = 800;
    }
    world.addDynamicObject(&player);
    player.collider.dimension ={ 10,10 };
    player.collider.center ={ 129,729 };

    menuScreen.loadMedia(renderer, "Textures/Background.png");
    menuScreen.setImageCoords({ 0,0,224,260 });
    menuScreen.setScreenCoords({ 0, 0, 672, 780 });
}

void window::input() {
    SDL_Event e;
    SDL_PumpEvents();
    const Uint8* keyboardState = SDL_GetKeyboardState(NULL);
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_WINDOWEVENT) {
            switch (e.window.event) {
            case SDL_WINDOWEVENT_CLOSE:
                window::running = false;
                break;
            }
        }
        if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym)
            {
            case SDLK_w:
                player.rigidBody.slope.y = -1;
                break;
            case SDLK_s:
                player.rigidBody.slope.y = 1;
                break;
            case SDLK_a:
                player.rigidBody.slope.x = -1;
                break;
            case SDLK_d:
                player.rigidBody.slope.x = 1;
                break;
            }
        }
        if (e.type == SDL_KEYUP) {
            switch (e.key.keysym.sym)
            {
            case SDLK_w:
                if (player.rigidBody.slope.y == -1)
                    player.rigidBody.slope.y = 0;
                break;
            case SDLK_s:
                if (player.rigidBody.slope.y == 1)
                    player.rigidBody.slope.y = 0;
                break;
            case SDLK_a:
                if (player.rigidBody.slope.x == -1)
                    player.rigidBody.slope.x = 0;
                break;
            case SDLK_d:
                if (player.rigidBody.slope.x == 1)
                    player.rigidBody.slope.x = 0;
                break;
            }
        }
    }
}
void window::update() {
    static bas::timer barrelTimer;
    static int barrelIndex = 0;
    if (barrelTimer.getTime() > 500) {
        dynamicObject* newBarrel = &donkeyMap.barrels[barrelIndex];
        newBarrel->collider.center ={ 120,255 };
        newBarrel->collider.dimension ={ 10,10 };
        newBarrel->rigidBody.slope.x = 1;
        barrelTimer.restartTime();
        barrelIndex++;
        if (barrelIndex > barrel_amou-1)
            barrelIndex = 0;
    }

    for (int i = 0; i < barrel_amou; i++) {
        dynamicObject* current_barrel = &donkeyMap.barrels[i];
        if (current_barrel->collider.center.x > 662)
            current_barrel->rigidBody.slope.x = -1;
        if (current_barrel->collider.center.x < 10)
            current_barrel->rigidBody.slope.x = 1;
        current_barrel->collider.center.x += current_barrel->rigidBody.slope.x;
        current_barrel->collider.center.y += 1.5;
    }
    player.collider.center.x += player.rigidBody.slope.x;
    player.collider.center.y += 1.5;
    world.getCollisions();
    world.resolveCollisions();
}

void window::render() {
    SDL_SetRenderDrawColor(renderer, 255,255,255,0);
    SDL_RenderClear(renderer);

    menuScreen.drawTexture(renderer);

    SDL_SetRenderDrawColor(renderer, 0, 255, 255, 0);

    for (int i = 0; i < barrel_amou; i++)
        donkeyMap.staticObjects[i].collider.aabb.draw(renderer);
    for (int i = 0; i < barrel_amou; i++) {
        donkeyMap.barrels[i].collider.setAABB();
        bas::animatedtexture* current_texture = &donkeyMap.barrelsTextures[i];

        donkeyMap.barrels[i].collider.aabb.draw(renderer);
    }

    player.collider.setAABB();
    player.collider.aabb.draw(renderer);

    SDL_RenderPresent(renderer);
    SDL_UpdateWindowSurface(screen);
}

void window::clean() {

}
