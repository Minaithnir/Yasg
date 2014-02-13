#include "Unit.hpp"
#include <iostream>
#include "QuadTree.hpp"

Unit::Unit() : m_trace(false)
{
}

Unit::~Unit()
{
}

void Unit::update(const float& frameTime)
{
}

void Unit::update(const float& frameTime, QuadTree& qTree)
{
    std::list<Unit*> inRange;
    qTree.range(m_position.x, m_position.y, m_radius*5, inRange);
    update(frameTime, inRange);
}

void Unit::update(const float& frameTime, std::vector<Unit>& others)
{
    std::list<Unit*> othersV;
    for(std::vector<Unit>::iterator it=others.begin(); it!=others.end(); it++)
    {
        othersV.push_back(&(*it));
    }
    update(frameTime, othersV);
}

void Unit::update(const float& frameTime, std::list<Unit*>& others)
{
    m_steering = Vector(0,0);
    std::vector<Vehicule*> othersV;
    for(std::list<Unit*>::iterator it=others.begin(); it!=others.end(); it++)
    {
        othersV.push_back(*it);
    }

    bool approx = false;

    m_steering = unallignedCollisionAvoidance(othersV);
    if(m_steering==Vector(0,0))
    {
        if(!m_commands.empty())
        {
            switch(m_commands.front())
            {
            case GO :
                approx = false;
                //Marge de manoeuvre dans le cas où plusieurs unité vont au même end
                for(std::list<Unit*>::iterator it=others.begin(); it!=others.end() && !approx; it++)
                {
                    if(*it!=this && Vector(m_target - (*it)->m_target).norme() < 2*m_radius)
                    {
                        approx = true;
                    }
                }
                m_steering = seek(m_target, 0, approx);
                break;
            case FOLLOW :
                if(&(*m_pathIterator) == &m_pathWay.back() && !m_loopPath)
                {
                    go(m_pathWay.back());
                }
                else
                    m_steering = followPath();
                break;
            case IDLE :
                m_steering = -m_velocity;
                if(m_velocity.norme()<=0.0f)
                {
                    m_commands.pop_front();
                }
                break;
            case WANDER :
                m_steering = wander();
                break;
            default:
                break;
            }
        }
        else
        {
            m_commands.push_back(IDLE);
        }
    }
    else
    {
        if(m_commands.empty())
        {
            m_commands.push_back(IDLE);
        }
    }
}

void Unit::go(Vector dest)
{
    m_arrival = true;
    m_commands.clear();
    m_target = dest;
    m_commands.push_front(GO);
}

void Unit::setPath(std::list<Vector> pathWay, bool loop, float width)
{
    m_commands.clear();
    m_pathWay = pathWay;
    m_loopPath = loop;
    m_pathWidth = width;
    m_pathIterator = m_pathWay.begin();
    m_commands.push_front(FOLLOW);
    m_arrival = false;
}

void Unit::idle()
{
    m_commands.clear();
    m_commands.push_back(IDLE);
}

void Unit::setWander()
{
    m_commands.clear();
    m_commands.push_front(WANDER);
}

bool Unit::clicked(float x, float y)
{
    return m_drawing.getGlobalBounds().contains(x, y);
}

bool Unit::intersect(const sf::FloatRect& rect)
{
    return m_drawing.getGlobalBounds().intersects(rect);
}

void Unit::trace(bool trace)
{
    m_trace = trace;
    if(m_trace)
    {
        m_traceClock.restart();
        m_traceArray.clear();
        //m_traceArray.setPrimitiveType(sf::LinesStrip);
    }
}

void Unit::draw(sf::RenderTarget& App)
{
    if(m_trace)
    {
        sf::VertexArray lines(sf::Lines, 4);
        lines[0].position = m_position;
        lines[1].position = m_position + Vector(m_velocity).setLengh(100);

        lines[0].color = sf::Color::Green;
        lines[1].color = sf::Color::Green;

        lines[2].position = m_position;
        lines[3].position = m_position + Vector(m_steering).setLengh(100);

        lines[2].color = sf::Color::Blue;
        lines[3].color = sf::Color::Blue;

        App.draw(lines);

        if(m_traceClock.getElapsedTime().asSeconds()>.0f)
        {
            sf::Vertex vertex(m_position, sf::Color::Red);
            m_traceArray.append(vertex);
            if(m_traceArray.getVertexCount()>20)
            m_traceClock.restart();
        }

        App.draw(m_traceArray);
    }

    Vehicule::draw(App);
}
