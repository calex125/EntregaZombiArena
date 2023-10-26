#include "Motor.h"

Motor::Motor()
    : state(State::GAME_OVER),
    resolution(VideoMode::getDesktopMode().width, VideoMode::getDesktopMode().height),
    window(VideoMode(resolution.x, resolution.y), "Zombie Arena", Style::Fullscreen),
    mainView(sf::FloatRect(0, 0, resolution.x, resolution.y)),
    textureBackground(TextureHolder::GetTexture("graphics/background_sheet.png")),
    zombies(NULL) {
    window.setMouseCursorVisible(true);

    textureCrosshair = TextureHolder::GetTexture("graphics/crosshair.png");
    spriteCrosshair.setTexture(textureCrosshair);
    spriteCrosshair.setOrigin(25, 25);
    textureGameOver = TextureHolder::GetTexture("graphics/background.png");

    spriteGameOver.setTexture(textureGameOver);
    spriteGameOver.setPosition(0, 0);

}



void Motor::run() {
    View uiView(sf::FloatRect(-1600, -200, resolution.x, resolution.y));

    Pickup healthPickup(1);
    Pickup ammoPickup(2);
    RectangleShape healthBar;
    healthBar.setFillColor(Color::Red);
    healthBar.setPosition(450, 980);
    int framesSinceLastHUDUpdate = 0;
    int score = 0;
    int hiScore = 0;
    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Return && state == State::PLAYING) {
                    state = State::PAUSED;
                }
                else if (event.key.code == Keyboard::Return && state == State::PAUSED) {
                    state = State::PLAYING;
                    clock.restart();
                }
                else if (event.key.code == Keyboard::Return && state == State::GAME_OVER) {
                    state = State::LEVELING_UP;
                }
                if (state == State::PLAYING) {
                }
                if (Mouse::isButtonPressed(sf::Mouse::Left))
                {
                    if (gameTimeTotal.asMilliseconds() - lastPressed.asMilliseconds() > 1000 / fireRate && bulletsInClip > 0)
                    {

                        bullets[currentBullet].shoot(
                            player.getCenter().x, player.getCenter().y,
                            mouseWorldPosition.x, mouseWorldPosition.y
                        );

                        currentBullet++;
                        if (currentBullet > 99)
                        {
                            currentBullet = 0;
                        }
                        lastPressed = gameTimeTotal;


                        bulletsInClip--;
                    }
                }

            }

        }
            
        if (Keyboard::isKeyPressed(Keyboard::Escape)) {
            window.close();
        }

        if (state == State::PLAYING) {
            if (Keyboard::isKeyPressed(Keyboard::W)) {
                player.moveUp();
            }
            else {
                player.stopUp();
            }

            if (Keyboard::isKeyPressed(Keyboard::S)) {
                player.moveDown();
            }
            else {
                player.stopDown();
            }

            if (Keyboard::isKeyPressed(Keyboard::A)) {
                player.moveLeft();
            }
            else {
                player.stopLeft();
            }

            if (Keyboard::isKeyPressed(Keyboard::D)) {
                player.moveRight();
            }
            else {
                player.stopRight();
            }
            // Fire a bullet
           

        }

        if (state == State::LEVELING_UP)
        {
            if (event.key.code == Keyboard::Num1)
            {
                state = State::PLAYING;
            }

            if (event.key.code == Keyboard::Num2)
            {
                state = State::PLAYING;
            }

            if (event.key.code == Keyboard::Num3)
            {
                state = State::PLAYING;
            }

            if (event.key.code == Keyboard::Num4)
            {
                state = State::PLAYING;
            }

            if (event.key.code == Keyboard::Num5)
            {
                state = State::PLAYING;
            }

            if (event.key.code == Keyboard::Num6)
            {
                state = State::PLAYING;
            }

            if (state == State::PLAYING)
            {
                arena.width = 500;
                arena.height = 500;
                arena.left = 0;
                arena.top = 0;

                int tileSize = createBackground(background, arena);

                player.spawn(arena, resolution, tileSize);
                healthPickup.setArena(arena);
                ammoPickup.setArena(arena);

                numZombies = 10;

                delete[] zombies;
                zombies = createHorde(numZombies, arena);
                numZombiesAlive = numZombies;
               
                clock.restart();
            }
        }

        if (state == State::PLAYING) {
            Time dt = clock.restart();
            gameTimeTotal += dt;
            float dtAsSeconds = dt.asSeconds();

            mouseScreenPosition = Mouse::getPosition();
            mouseWorldPosition = window.mapPixelToCoords(Mouse::getPosition(), mainView);

            // Set the crosshair to the mouse world location
            spriteCrosshair.setPosition(mouseWorldPosition);    

            player.update(dtAsSeconds, Mouse::getPosition());
            Vector2f playerPosition(player.getCenter());
            mainView.setCenter(player.getCenter());

            // Collision detection
          // Ha estat disparat algun zombi?
            for (int i = 0; i < 100; i++)
            {
                for (int j = 0; j < numZombies; j++)
                {
                    if (bullets[i].isInFlight() &&
                        zombies[j].isAlive())
                    {
                        if (bullets[i].getPosition().intersects
                        (zombies[j].getPosition()))
                        {
                            
                            bullets[i].stop();

                            
                            if (zombies[j].hit())
                            {
                                
                                score += 10;
                                if (score >= hiScore)
                                {
                                    hiScore = score;
                                }

                                numZombiesAlive--;

                               
                                if (numZombiesAlive == 0)
                                {
                                    state = State::LEVELING_UP;
                                }
                            }


                        }
                    }
                }
            } // End zombie being shot
            for (int i = 0; i < numZombies; i++)
            {
                if (player.getPosition().intersects
                (zombies[i].getPosition()) && zombies[i].isAlive())
                {
                   

                    if (player.getHealth() <= 0)
                    {
                        state = State::GAME_OVER;

                    }
                }
            } // End player get touched 
              // Have any zombies touched the player
            for (int i = 0; i < numZombies; i++)
            {
                if (player.getPosition().intersects
                (zombies[i].getPosition()) && zombies[i].isAlive())
                {
                    if (player.hit(gameTimeTotal))
                    {
                      
                    }

                    if (player.getHealth() <= 0)
                    {
                        state = State::GAME_OVER;
                    }
                }
            }
            if (player.getPosition().intersects(healthPickup.getPosition())&& healthPickup.isSpawned()) {
                player.increaseHealthLevel(healthPickup.gotIt());
            }
            for (int i = 0; i < numZombies; i++) {
                if (zombies[i].isAlive()) {
                    zombies[i].update(dt.asSeconds(), playerPosition);
                }
            }
            if (player.getPosition().intersects
            (ammoPickup.getPosition()) && ammoPickup.isSpawned())
            {
                bulletsSpare += ammoPickup.gotIt();

            }
            healthBar.setSize(Vector2f(player.getHealth() * 3, 50));
            framesSinceLastHUDUpdate++;


            // Update any bullets that are in-flight
            for (int i = 0; i < 100; i++)
            {
                if (bullets[i].isInFlight())
                {
                    bullets[i].update(dtAsSeconds);
                }
            }
            healthPickup.update(dtAsSeconds);
            ammoPickup.update(dtAsSeconds);
           

        if (state == State::PLAYING) {
            window.clear();
            window.setView(mainView);

            window.draw(background, &textureBackground);

            for (int i = 0; i < numZombies; i++) {
                window.draw(zombies[i].getSprite());
            }
            for (int i = 0; i < 100; i++)
            {
                if (bullets[i].isInFlight())
                {
                    window.draw(bullets[i].getShape());
                }
            }
        }
            window.draw(player.getSprite());
            window.draw(spriteCrosshair);
            window.setView(uiView);
            window.draw(healthBar);
            window.setView(mainView);
            if (ammoPickup.isSpawned())
            {
                window.draw(ammoPickup.getSprite());
            }

            if (healthPickup.isSpawned())
            {
                window.draw(healthPickup.getSprite());
            }

        }

        if (state == State::LEVELING_UP) {
            window.draw(spriteGameOver);
        }

        if (state == State::PAUSED) {
        }

        if (state == State::GAME_OVER) {
            window.draw(spriteGameOver);
        }

        window.display();
    }
}