#include <SFML/Graphics.hpp>
#include <iostream>
#include <chrono>
#include <thread>

int main()
{

    unsigned int width = 800;
    unsigned int height = 600;

    sf::RenderWindow window(sf::VideoMode({width, height}), "SFML works!");
    window.setFramerateLimit(60);
    sf::CircleShape shape(45.f);
    shape.setFillColor(sf::Color::Green);

    sf::CircleShape circle(45.0f);
    circle.setOrigin(circle.getGeometricCenter());
    circle.setPosition({width / 2.0f, height / 2.0f});
    circle.setFillColor(sf::Color::Red);
    circle.setOutlineThickness(52.f);
    circle.setOutlineColor(sf::Color::Blue);
    

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.draw(circle);
        window.display();
    }
}