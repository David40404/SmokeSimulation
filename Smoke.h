#pragma once

class SmokeCloud {
public:
    sf::CircleShape cloud;
    float speed;

    SmokeCloud() : speed(1.5f) {
        cloud.setRadius(25.f);
        cloud.setPosition(100.f, 100.f);

        srand(time(NULL) + rand());
        int color = 50 + rand() % 50;
        cloud.setFillColor(sf::Color::Color(color, color, color));
    }

    SmokeCloud(float x, float y, float r) : speed(1.5f) {
        cloud.setRadius(r);
        cloud.setPosition(x, y);

        srand(time(NULL) + rand());
        int color = 50 + rand() % 50;
        cloud.setFillColor(sf::Color::Color(color, color, color));
    }

    bool collision(sf::RectangleShape& rectShape) {
        return cloud.getGlobalBounds().intersects(rectShape.getGlobalBounds());
    }

    bool collision(sf::CircleShape& circleShape) {
        return cloud.getGlobalBounds().intersects(circleShape.getGlobalBounds());
    }

    void moving(std::vector<sf::RectangleShape>& shapes) {


        float interf = static_cast <float> (rand()) / RAND_MAX * 2.5 - 1.25;
        //float interf = 0;

        bool all_collision = false;
        bool shape_collision = false;
        int move_side = 0;

        float radius = cloud.getRadius();
        sf::Vector2f cloudsPos = cloud.getPosition();

        float xDist;
        float yDist;
        float distance;
        float rectSizesSqrt;

        for (sf::RectangleShape& rectShape : shapes) {

            sf::Vector2f rectPos = rectShape.getPosition();
            sf::Vector2f rectSize = rectShape.getSize();

            xDist = abs(cloudsPos.x - rectPos.x);
            yDist = abs(cloudsPos.y - rectPos.y);
            distance = sqrt(xDist*xDist+yDist*yDist);
            rectSizesSqrt = sqrt(rectSize.x * rectSize.x + rectSize.y * rectSize.y);

            if (distance < rectSizesSqrt * 0.99) {
                shape_collision = collision(rectShape);
                all_collision |= shape_collision;

                if (shape_collision)
                    move_side = cloudsPos.x - (rectPos.x + rectShape.getSize().x / 2 - radius);

            }
        }
        if (all_collision) {
            cloud.move((move_side < 0 ? -.8f : .8f), interf);
            return;
        }
        cloud.move(interf, -speed);
    }

    void draw(sf::RenderWindow& window) {
        window.draw(cloud);
    }
};

class Smoke {
    std::vector <SmokeCloud> clouds;
    size_t size;
    unsigned int params;
    float bouncePowerX;
    float bouncePowerY;


    void bounce() {
        float xDist = 0;
        float yDist = 0;
        float distance = 0;
        float cloudSize = 0;
        float cloudRadius1;
        float cloudRadius2;

        for (SmokeCloud& cloud1 : clouds) {
            for (SmokeCloud& cloud2 : clouds) {

                sf::Vector2f cloudPos1 = cloud1.cloud.getPosition();
                sf::Vector2f cloudPos2 = cloud2.cloud.getPosition();
                
                cloudRadius1 = cloud1.cloud.getRadius();
                cloudRadius2 = cloud2.cloud.getRadius();
                xDist = abs(cloudPos1.x - cloudPos2.x);
                yDist = abs(cloudPos1.y - cloudPos2.y);
                distance = sqrt(xDist * xDist + yDist * yDist);
                cloudSize = sqrt(cloudRadius1 * 4 + cloudRadius2 * 4);
                
                if (distance < cloudSize * 2) {
                    if (cloud1.cloud.getGlobalBounds().intersects(cloud2.cloud.getGlobalBounds())) {
                        cloud1.cloud.move(-bouncePowerX, 0);
                        cloud2.cloud.move(bouncePowerX,  0);
                        continue;
                    }
                    else if (distance < cloudSize * 5) {
                        cloud1.cloud.move(.5f, -.1f);
                        cloud2.cloud.move(-.5f, .1f);
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
        size(100), params(Default), bouncePowerX(.4f), bouncePowerY(.4f) {

        srand(time(NULL) + rand());

        for (size_t i = 0; i < size; i++) {
            clouds.push_back(SmokeCloud(100 + (rand() % 100), 100 + (rand() % 100), 2 + (rand() % 5)));
        }
    }

    __vectorcall Smoke(size_t size_, float x, float y, unsigned int w, unsigned int h, float min_size, float max_size, unsigned int params_) :
        size(size_), params(params_), bouncePowerX(.4f), bouncePowerY(.4f) {

        srand(time(NULL) + rand());

        for (size_t i = 0; i < size; i++) {
            clouds.push_back(
                SmokeCloud(
                    x + (rand() % static_cast <unsigned int> (w)),
                    y + (rand() % static_cast <unsigned int> (h)),
                    min_size + (rand() % static_cast <unsigned int> (max_size - min_size))
                )
            );
        }
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

        for (SmokeCloud& cloud : clouds) {
            cloud.moving(shapes);
            cloud.draw(window);
        }
    }
};