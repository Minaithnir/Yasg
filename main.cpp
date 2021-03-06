#include <SFML/Graphics.hpp>

#include "Unit.hpp"
#include "Selection.hpp"
#include "QuadTree.hpp"

#include <iostream>

#define WIDTH 1280
#define HEIGHT 696
#define DEMO_UNIT_COUNT 100

#define SCROLL_RANGE 50
#define SCROLL_SPEED 200

int main()
{
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Yet another strategy game", sf::Style::Titlebar | sf::Style::Close);
    window.setKeyRepeatEnabled(false);
    bool pause = false;
    bool fullscreen = false;

    std::vector<Unit> units;
    QuadTree qTree(sf::FloatRect(-2000,-2000,4000,4000));
    for(unsigned int i=0; i<DEMO_UNIT_COUNT; i++)
    {
        units.push_back(Unit());
        units.back().setPosition(Vector(rand()%WIDTH - WIDTH/2, rand()%HEIGHT - HEIGHT/2));
    }

    Selection select;
    select.setSelectable(&units);
    sf::View camera(sf::FloatRect((int) -(window.getSize().x/2), (int) -(window.getSize().y/2),window.getSize().x, window.getSize().y));

    Selection group[10];

    std::list<Vector> path;
    sf::VertexArray pathView(sf::LinesStrip);

    sf::Clock clock;
    float frameTime;
    sf::Vector2f wPos;

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
            case sf::Event::Resized :
                camera.setSize(event.size.width, event.size.height);
                break;
            case sf::Event::LostFocus :
                pause = true;
                break;
            case sf::Event::GainedFocus :
                pause = false;
                break;
            case sf::Event::MouseButtonPressed :
                switch(event.mouseButton.button)
                {
                case sf::Mouse::Left :
                    wPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                    select.startSelect(wPos.x, wPos.y);
                    break;
                case sf::Mouse::Right :
                    if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
                    {
                        wPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                        path.push_back(wPos);
                        sf::Vertex v(wPos, sf::Color::Blue);
                        pathView.append(v);
                    }
                    else
                    {
                        wPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                        select.giveDest(Vector(wPos.x, wPos.y), 3*UNIT_RADIUS);
                    }
                    break;
                default :
                    break;
                }
                break;
            case sf::Event::MouseButtonReleased :
                switch(event.mouseButton.button)
                {
                case sf::Mouse::Left :
                    wPos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                    select.endSelect(wPos.x, wPos.y);
                    break;
                default :
                    break;
                }
                break;
            case sf::Event::KeyPressed :
                switch(event.key.code)
                {
                case sf::Keyboard::F4 :
                    if(event.key.alt)
                        window.close();
                    break;
                case sf::Keyboard::Return :
                    if(event.key.alt)
                    {
                        if(fullscreen)
                        {
                            fullscreen = false;
                            window.create(sf::VideoMode(window.getSize().x, window.getSize().y), "Yet another strategy game", sf::Style::Titlebar | sf::Style::Close);
                            window.setKeyRepeatEnabled(false);
                        }
                        else
                        {
                            fullscreen = true;
                            window.create(sf::VideoMode(window.getSize().x, window.getSize().y), "Yet another strategy game", sf::Style::Fullscreen);
                            window.setKeyRepeatEnabled(false);
                        }
                    }
                    break;
                case sf::Keyboard::LShift :
                    path.clear();
                    pathView.clear();
                    break;
                case sf::Keyboard::W :
                    select.giveFreedom();
                    break;
                case sf::Keyboard::R :
                    for(unsigned int i=0; i<DEMO_UNIT_COUNT; i++)
                    {
                        units[i].setPosition(Vector(rand()%WIDTH - WIDTH/2, rand()%HEIGHT - HEIGHT/2));
                        units[i].idle();
                    }
                    break;
                case sf::Keyboard::T :
                    if(event.key.shift)
                        select.trace(false);
                    else
                        select.trace(true);
                    break;
                case sf::Keyboard::A :
                    if(event.key.control)
                    {
                        std::list<Unit*> all;
                        for(std::vector<Unit>::iterator it=units.begin(); it!=units.end(); it++)
                        {
                            all.push_back(&(*it));
                        }
                        select.setSelection(all);
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
            case sf::Event::KeyReleased :
                switch(event.key.code)
                {
                case sf::Keyboard::LShift :
                    if(event.key.alt)
                        select.givePath(path, false, 70, 3*UNIT_RADIUS);
                    else
                        select.givePath(path, true, 70);
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

        //update view from mouse position
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::LAlt))
        {
            sf::Vector2i localPosition = sf::Mouse::getPosition(window);
            sf::Vector2f newPos = camera.getCenter();
            if(localPosition.x < SCROLL_RANGE)
                newPos.x -= SCROLL_SPEED*frameTime;
            else if(localPosition.x > (int) (window.getSize().x)-SCROLL_RANGE)
                newPos.x += SCROLL_SPEED*frameTime;

            if(localPosition.y < SCROLL_RANGE)
                newPos.y -= SCROLL_SPEED*frameTime;
            else if(localPosition.y > (int) (window.getSize().y)-SCROLL_RANGE)
                newPos.y += SCROLL_SPEED*frameTime;

            camera.setCenter(newPos);
        }

        if(!pause)
        {
            for(unsigned int i=0; i<DEMO_UNIT_COUNT; i++)
            {
                units[i].update(frameTime, qTree);
            }

            qTree.clear();
            for(unsigned int i=0; i<DEMO_UNIT_COUNT; i++)
            {
                units[i].updatePos(frameTime);
                qTree.insert(&units[i]);
            }
        }


        window.clear(sf::Color::White);
        window.setView(camera);

        if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
            window.draw(pathView);

        select.draw(window);

        for(unsigned int i=0; i<DEMO_UNIT_COUNT; i++)
        {
            units[i].draw(window);
        }
        window.display();
    }

    return 0;
}
