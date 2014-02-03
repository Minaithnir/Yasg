#include "Unit.hpp"
#include <iostream>

Unit::Unit()
{
}

Unit::~Unit()
{
}

void Unit::update(const float& frameTime)
{
}

void Unit::update(const float& frameTime, std::vector<Unit>& others)
{
    std::vector<Vehicule*> othersV;
    for(std::vector<Unit>::iterator it=others.begin(); it!=others.end(); it++)
    {
        othersV.push_back(&(*it));
    }

    m_steering = unallignedCollisionAvoidance(othersV);
    if(m_steering==Vector(0,0))
    {
        if(!m_commands.empty())
        {
            switch(m_commands.front())
            {
            case GO :
                m_steering = seek(m_target);
                if(Vector(m_target-m_position).norme()<=APPROX_ARRIVAL)
                {
                    m_commands.pop_front();
                    m_commands.push_back(IDLE);
                }
                break;
            case IDLE :
                m_steering = -m_velocity;
                if(m_velocity.norme()<=0.0f)
                {
                    m_commands.pop_front();
                    std::cout << "pop" << std::endl;
                }
                break;
            default:
                break;
            }
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
    m_commands.clear();
    m_target = dest;
    m_commands.push_front(GO);
}

void Unit::idle()
{
    m_commands.clear();
    m_commands.push_back(IDLE);
}

bool Unit::clicked(float x, float y)
{
    return m_drawing.getGlobalBounds().contains(x, y);
}

bool Unit::intersect(const sf::FloatRect& rect)
{
    return m_drawing.getGlobalBounds().intersects(rect);
}
