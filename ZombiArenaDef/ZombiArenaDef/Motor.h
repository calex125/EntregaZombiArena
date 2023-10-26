#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include "ZombieArena.h"
#include "Player.h"
#include "TextureHolder.h"
#include "Bullet.h"
#include "Pickup.h"

using namespace sf;

class Motor {
public:
    Motor();
    void run();

private:
    TextureHolder holder;
    enum class State { PAUSED, LEVELING_UP, GAME_OVER, PLAYING };
    State state;
    Vector2f resolution;
    RenderWindow window;
    View mainView;
    Clock clock;
    Time gameTimeTotal;
    Vector2f mouseWorldPosition;
    Vector2i mouseScreenPosition;
    Player player;
    IntRect arena;
    VertexArray background;
    Texture textureBackground;
    int numZombies;
    int numZombiesAlive;
    Zombie* zombies;

    //Bullets should do
    Bullet bullets[100];
    int currentBullet = 0;
    int bulletsSpare = 24;
    int bulletsInClip = 6;
    int clipSize = 6;
    float fireRate = 1;
    Sprite spriteCrosshair;
    Time lastPressed;
    Texture textureCrosshair;
    RectangleShape healthBar;
    Sprite spriteGameOver;
    Texture textureGameOver;
};

#endif

