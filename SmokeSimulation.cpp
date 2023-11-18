#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <iostream>
#include <chrono>
#include "Smoke.h"

std::vector <sf::RectangleShape> bricks;

Smoke smoke(2500, 200, 680, 150, 150, 4, 6, (Smoke::Default | Smoke::Bouncing));

int main() {

    sf::RenderWindow window(sf::VideoMode(1080, 720), "Penis");
    window.setFramerateLimit(60);

    for (size_t i = 0; i < 16; i++) {
        sf::RectangleShape wall({ 50, 150 });
        wall.setPosition(0 + i * 80 + rand() % 20, 220 - rand() % 100);
        bricks.push_back(wall);
    }

    std::chrono::high_resolution_clock::time_point start;
    std::chrono::high_resolution_clock::time_point end;
    float fps;

    sf::Vector2f oldPos;
    bool moving = false;
    float zoom = 1;
    sf::View view = window.getDefaultView();

    smoke.setBouncePower({.7f, 0});

    while (window.isOpen())
    {
        start = std::chrono::high_resolution_clock::now();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == 0) {
                    moving = true;
                    oldPos = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
                }
                break;
            case  sf::Event::MouseButtonReleased:
                if (event.mouseButton.button == 0) {
                    moving = false;
                }
                break;
            case sf::Event::MouseMoved:
            {
                if (!moving)
                    break;
                const sf::Vector2f newPos = window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
                const sf::Vector2f deltaPos = oldPos - newPos;

                view.setCenter(view.getCenter() + deltaPos);
                window.setView(view);

                oldPos = window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
                break;
            }
            case sf::Event::MouseWheelScrolled:
                if (moving)
                    break;

                if (event.mouseWheelScroll.delta <= -1)
                    zoom = std::min(5.f, zoom + .1f);
                else if (event.mouseWheelScroll.delta >= 1)
                    zoom = std::max(.2f, zoom - .1f);

                view.setSize(window.getDefaultView().getSize());
                view.zoom(zoom);
                window.setView(view);
                break;
            }
            
        }


        

        window.clear();
        
        for (sf::RectangleShape& wall : bricks)
            window.draw(wall);

        smoke.draw(window, bricks);

        window.display();

        end = std::chrono::high_resolution_clock::now();
        fps = (float)1e9 / (float)std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        std::cout << "FPS: " << static_cast <int> (fps) << " " << sizeof(SmokeCloud) << std::endl;
    }

    return 0;
}