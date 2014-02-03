#include <SFML/Graphics.hpp>

#include "Unit.hpp"
#include "Selection.hpp"

#define WIDTH 1900
#define HEIGHT 1000
#define DEMO_UNIT_COUNT 500

int main()
{
    sf::RenderWindow window(sf::VideoMode(1900,1000), "Yet another strategy game");

    std::vector<Unit> units;
    for(unsigned int i=0; i<DEMO_UNIT_COUNT; i++)
    {
        units.push_back(Unit());
        units.back().setPosition(Vector(rand()%WIDTH, rand()%HEIGHT));
    }

    Selection select;
    select.setSelectable(&units);

    sf::Clock clock;
    float frameTime;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch(event.type)
            {
            case sf::Event::Closed :
                window.close();
                break;
            case sf::Event::MouseButtonPressed :
                switch(event.mouseButton.button)
                {
                case sf::Mouse::Left :
                    select.startSelect(event.mouseButton.x, event.mouseButton.y);
                    break;
                case sf::Mouse::Right :
                    select.giveDest(Vector(event.mouseButton.x, event.mouseButton.y), 30);
                    break;
                default :
                    break;
                }
                break;
            case sf::Event::MouseButtonReleased :
                switch(event.mouseButton.button)
                {
                case sf::Mouse::Left :
                    select.endSelect(event.mouseButton.x, event.mouseButton.y);
                    break;
                default :
                    break;
                }
                break;
            case sf::Event::KeyPressed :
                switch(event.key.code)
                {
                case sf::Keyboard::R :
                    for(unsigned int i=0; i<DEMO_UNIT_COUNT; i++)
                    {
                        units[i].setPosition(Vector(rand()%WIDTH, rand()%HEIGHT));
                        units[i].idle();
                    }
                    break;
                default :
                    break;
                }
                break;
            default :
                break;
            }
        }

        frameTime = clock.getElapsedTime().asSeconds();
        clock.restart();

        for(unsigned int i=0; i<DEMO_UNIT_COUNT; i++)
        {
            units[i].update(frameTime, units);
        }

        for(unsigned int i=0; i<DEMO_UNIT_COUNT; i++)
        {
            units[i].updatePos(frameTime);
        }

        window.clear(sf::Color::White);

        select.draw(window);

        for(unsigned int i=0; i<DEMO_UNIT_COUNT; i++)
        {
            units[i].draw(window);
        }
        window.display();
    }

    return 0;
}
