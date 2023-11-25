#include <SFML/Graphics.hpp>
#include <Windows.h>
#include <iostream>
#include <chrono>
#include "Smoke.h"

std::vector <sf::RectangleShape> bricks;

std::vector <Smoke> smokes;

bool is_point_in_circle(double x, double y, double xc, double yc, double r) {
    if ((x - xc) * (x - xc) + (y - yc) * (y - yc) < r * r) return true;
    return false;
}

int main() {

    sf::RenderWindow window(sf::VideoMode(1080, 720), "Penis");
    window.setFramerateLimit(60);

    for (size_t i = 0; i < 16; i++) {
        sf::RectangleShape wall({ 50, 150 });
        wall.setPosition(0 + i * 80 + rand() % 20, 220 - rand() % 100);
        bricks.push_back(wall);
    }

    std::vector <sf::CircleShape> Points;

    sf::Font font;
    font.loadFromFile("C:/Fonts/BrassMono-Bold.ttf");

    std::chrono::high_resolution_clock::time_point start;
    std::chrono::high_resolution_clock::time_point end;
    float fps;
    bool pointFlag = false;

    sf::Vector2f oldPos;
    bool moving = false;
    float zoom = 1;
    sf::View view = window.getDefaultView();

    while (window.isOpen())
    {
        start = std::chrono::high_resolution_clock::now();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Enter)) {
                srand(time(NULL));
                for (auto& Point : Points)
                    smokes.push_back(Smoke(Point.getFillColor().r * 8, Point.getPosition().x - Point.getFillColor().g / 2, Point.getPosition().y, Point.getFillColor().g, Point.getFillColor().b, 4, 6, (Smoke::Default | Smoke::Bouncing), 150));
            }

            switch (event.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::MouseButtonPressed:
                if (event.mouseButton.button == sf::Mouse::Left) {
                    moving = true;
                    oldPos = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
                }
                if (event.mouseButton.button == sf::Mouse::Right) {
                    pointFlag = true;
                }
                break;
            case  sf::Event::MouseButtonReleased:
                if (event.mouseButton.button == 0) {
                    moving = false;
                }
                if (pointFlag and event.mouseButton.button == sf::Mouse::Right) {
                    sf::Vector2f pos = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));

                    

                    bool secondPointFlag = false;
                    for (size_t i = 0; i < Points.size(); ++i) {
                        sf::Vector2f poingPos = Points[i].getPosition();
                        if (is_point_in_circle(poingPos.x, poingPos.y, pos.x - 10, pos.y - 10, 10)) {
                            Points.erase(Points.begin() + i);
                            secondPointFlag = true;
                            break;
                        }
                    }
                    if (secondPointFlag) break;

                    sf::CircleShape point(10);
                    point.setPosition(pos.x - 10, pos.y - 10);
                    point.setFillColor(sf::Color(100 + rand() % 156, 200 + rand() % 56, 200 + rand() % 56));

                    Points.push_back(point);

                    pointFlag = false;
                    break;
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
                    zoom = std::min(6.f, zoom + .1f);
                else if (event.mouseWheelScroll.delta >= 1)
                    zoom = std::max(.1f, zoom - .1f);

                view.setSize(window.getDefaultView().getSize());
                view.zoom(zoom);
                window.setView(view);
                break;
            }
        }

        window.clear();

        for (auto& wall : bricks)
            window.draw(wall);

        for (auto& point : Points)
            window.draw(point);

        for (auto& smoke : smokes) {
            smoke.draw(window, bricks);
            smoke.update();
        }

        window.display();

        end = std::chrono::high_resolution_clock::now();
        fps = (float)1e9 / (float)std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
        std::cout << "FPS: " << static_cast <int> (fps) << std::endl;
    }

    return 0;
}
