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

    Selection group[10];

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
                    select.giveDest(Vector(event.mouseButton.x, event.mouseButton.y), 35);
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
                case sf::Keyboard::Num0 :
                    if(event.key.control)
                        group[0] = select;
                    else
                        select = group[0];
                    break;
                case sf::Keyboard::Num1 :
                    if(event.key.control)
                        group[1] = select;
                    else
                        select = group[1];
                    break;
                case sf::Keyboard::Num2 :
                    if(event.key.control)
                        group[2] = select;
                    else
                        select = group[2];
                    break;
                case sf::Keyboard::Num3 :
                    if(event.key.control)
                        group[3] = select;
                    else
                        select = group[3];
                    break;
                case sf::Keyboard::Num4 :
                    if(event.key.control)
                        group[4] = select;
                    else
                        select = group[4];
                    break;
                case sf::Keyboard::Num5 :
                    if(event.key.control)
                        group[5] = select;
                    else
                        select = group[5];
                    break;
                case sf::Keyboard::Num6 :
                    if(event.key.control)
                        group[6] = select;
                    else
                        select = group[6];
                    break;
                case sf::Keyboard::Num7 :
                    if(event.key.control)
                        group[7] = select;
                    else
                        select = group[7];
                    break;
                case sf::Keyboard::Num8 :
                    if(event.key.control)
                        group[8] = select;
                    else
                        select = group[8];
                    break;
                case sf::Keyboard::Num9 :
                    if(event.key.control)
                        group[9] = select;
                    else
                        select = group[9];
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
