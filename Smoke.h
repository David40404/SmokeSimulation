#pragma once

class SmokeParticle {
public:
    sf::CircleShape particle;
    float speed;

    SmokeParticle() : speed(1.5f) {
        particle.setRadius(25.f);
        particle.setPosition(100.f, 100.f);

        srand(time(NULL) + rand());
        int color = 50 + rand() % 50;
        particle.setFillColor(sf::Color::Color(color, color, color));
    }

    SmokeParticle(float x, float y, float r) : speed(1.5f) {
        particle.setRadius(r);
        particle.setPosition(x, y);

        srand(time(NULL) + rand());
        int color = 50 + rand() % 50;
        particle.setFillColor(sf::Color::Color(color, color, color));
    }

    bool collision(sf::RectangleShape& rectShape) {
        return particle.getGlobalBounds().intersects(rectShape.getGlobalBounds());
    }

    bool collision(sf::CircleShape& circleShape) {
        return particle.getGlobalBounds().intersects(circleShape.getGlobalBounds());
    }

    void moving(std::vector<sf::RectangleShape>& shapes) {


        float interf = static_cast <float> (rand()) / RAND_MAX * 2.5 - 1.25;
        //float interf = 0;

        bool all_collision = false;
        bool shape_collision = false;
        int move_side = 0;

        float radius = particle.getRadius();
        sf::Vector2f particlePos = particle.getPosition();

        float xDist;
        float yDist;
        float distance;
        float rectSizesSqrt;

        for (sf::RectangleShape& rectShape : shapes) {

            sf::Vector2f rectPos = rectShape.getPosition();
            sf::Vector2f rectSize = rectShape.getSize();

            xDist = abs(particlePos.x - rectPos.x);
            yDist = abs(particlePos.y - rectPos.y);
            distance = sqrt(xDist*xDist+yDist*yDist);
            rectSizesSqrt = sqrt(rectSize.x * rectSize.x + rectSize.y * rectSize.y);

            if (distance < rectSizesSqrt * 0.99) {
                shape_collision = collision(rectShape);
                all_collision |= shape_collision;

                if (shape_collision)
                    move_side = particlePos.x - (rectPos.x + rectShape.getSize().x / 2 - radius);

            }
        }
        if (all_collision) {
            particle.move((move_side < 0 ? -.8f : .8f), interf);
            return;
        }
        particle.move(interf, -speed);
    }

    void draw(sf::RenderWindow& window) {
        window.draw(particle);
    }
};

class Smoke {
    std::vector <SmokeParticle> particles;
    size_t size;
    unsigned int params;
    float bouncePowerX;
    float bouncePowerY;

    clock_t start, end, lifeTime;

    void bounce() {
        float xDist = 0,
             yDist = 0,
             distance = 0,
             particlesSize = 0,
             particlesRadius1,
             particlesRadius2;

        for (SmokeParticle& particles1 : particles) {
            for (SmokeParticle& particles2 : particles) {

                sf::Vector2f particlesPos1 = particles1.particle.getPosition();
                sf::Vector2f particlesPos2 = particles2.particle.getPosition();
                
                particlesRadius1 = particles1.particle.getRadius();
                particlesRadius2 = particles2.particle.getRadius();
                xDist = abs(particlesPos1.x - particlesPos2.x);
                yDist = abs(particlesPos1.y - particlesPos2.y);
                distance = sqrt(xDist * xDist + yDist * yDist);
                particlesSize = sqrt(particlesRadius1 * 4 + particlesRadius2 * 4);
                
                if (distance < particlesSize * 2) {
                    if (particles1.particle.getGlobalBounds().intersects(particles2.particle.getGlobalBounds())) {
                        particles1.particle.move(-bouncePowerX, 0);
                        particles2.particle.move(bouncePowerX,  0);
                        continue;
                    }
                    else if (distance < particlesSize * 5) {
                        particles1.particle.move(.5f, -.1f);
                        particles2.particle.move(-.5f, .1f);
                    }
                }
            }
        }
    }

public:
    enum SmokeParams {
        Default = 0x00000000,
        Bouncing = 0x00000001
    };

    Smoke() :
        size(100), params(Default), bouncePowerX(.4f), bouncePowerY(.4f), lifeTime(100) {

        srand(time(NULL) + rand());

        for (size_t i = 0; i < size; i++) {
            particles.push_back(SmokeParticle(100 + (rand() % 100), 100 + (rand() % 100), 2 + (rand() % 5)));
        }

        start = clock();
    }

    __vectorcall Smoke(size_t size_ = 250, float x = 0, float y = 0, unsigned int w = 50, unsigned int h = 50, float min_size = 6, float max_size = 9, unsigned int params_ = (Default | Bouncing), clock_t lifeTime_ = 100) :
        size(size_), params(params_), bouncePowerX(.4f), bouncePowerY(.4f), lifeTime(lifeTime_) {

        srand(time(NULL) + rand());

        for (size_t i = 0; i < size; i++) {
            particles.push_back(
                SmokeParticle(
                    x + (rand() % static_cast <unsigned int> (w)),
                    y + (rand() % static_cast <unsigned int> (h)),
                    min_size + (rand() % static_cast <unsigned int> (max_size - min_size))
                )
            );
        }

        start = clock();
    }

    void setBouncePower(float power = .4f) {
        bouncePowerX = bouncePowerY = power;
    }

    void setBouncePower(sf::Vector2f power = sf::Vector2f(.4f, .4f)) {
        bouncePowerX = power.x;
        bouncePowerY = power.y;
    }

    void draw(sf::RenderWindow& window, std::vector<sf::RectangleShape>& shapes) {

        if (params && Bouncing == Bouncing) {
            //sf::Thread bounceThrd(&Smoke::bounce, this);
            //bounceThrd.launch();
            bounce();
        }

        for (SmokeParticle& particle : particles) {
            particle.moving(shapes);
            particle.draw(window);
        }
    }

    void update() {
        end = clock();

        //std::cout << end - start << std::endl;
        if ((end - start) > lifeTime) {
            if (particles.size()) particles.pop_back();
        }
    }
};
