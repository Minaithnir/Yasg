#include "Selection.hpp"

#include <iostream>

Selection::Selection() : m_select(NULL)
{
    m_shapeIcon = sf::CircleShape(UNIT_RADIUS,16);
    m_shapeIcon.setOutlineThickness(2);
    m_shapeIcon.setOutlineColor(sf::Color::Red);
    m_shapeIcon.setOrigin(UNIT_RADIUS,UNIT_RADIUS);
    m_selectShape = sf::VertexArray(sf::LinesStrip, 5);
    for(unsigned int i=0; i<5; i++)
    {
        m_selectShape[i].color = sf::Color::Red;
    }
}

Selection::~Selection()
{
}

void Selection::setSelectable(std::vector<Unit>* selectPtr)
{
    m_selectableUnit = selectPtr;
}

std::list<Unit*> Selection::getSelection()
{
    return m_selection;
}

void Selection::startSelect(float x, float y)
{
    m_selection.clear();
    m_selectRect.left = x;
    m_selectRect.top = y;
    m_select = true;
    m_size = 0;

    m_selectShape[0].position = sf::Vector2f(m_selectRect.left, m_selectRect.top);
    m_selectShape[4].position = sf::Vector2f(m_selectRect.left, m_selectRect.top);
}

void Selection::endSelect(float x, float y)
{
    m_selectRect.width = x - m_selectRect.left;
    m_selectRect.width +=1;
    m_selectRect.height = y - m_selectRect.top;
    m_selectRect.height +=1;

    for(unsigned int i=0; i<m_selectableUnit->size(); i++)
    {
        if(m_selectableUnit->at(i).intersect(m_selectRect))
        {
            m_selection.push_back(&(m_selectableUnit->at(i)));
            m_size++;
        }
    }
    m_select = false;
}

Selection Selection::operator=(const Selection& other)
{
    m_selection.clear();
    m_size = 0;
    for(std::list<Unit*>::const_iterator it=other.m_selection.begin(); it!=other.m_selection.end(); it++)
    {
        m_selection.push_back((*it));
        m_size ++;
    }

    return *this;
}

void Selection::draw(sf::RenderWindow& screen)
{
    for(std::list<Unit*>::iterator it=m_selection.begin(); it!=m_selection.end(); it++)
    {
        m_shapeIcon.setPosition((*it)->getPosition().x, (*it)->getPosition().y);
        screen.draw(m_shapeIcon);
    }

    if(m_select)
    {
        sf::Vector2f wPos = screen.mapPixelToCoords(sf::Mouse::getPosition(screen));
        m_selectShape[1].position = sf::Vector2f(wPos.x, m_selectRect.top);
        m_selectShape[2].position = sf::Vector2f(wPos.x, wPos.y);
        m_selectShape[3].position = sf::Vector2f(m_selectRect.left, wPos.y);

        screen.draw(m_selectShape);
    }
}

void Selection::trace(bool trace)
{
    for(std::list<Unit*>::iterator it=m_selection.begin(); it!=m_selection.end(); it++)
    {
        (*it)->trace(trace);
    }
}

void Selection::giveDest(Vector dest, float offset)
{
    int side = sqrt(m_size);
    side = side*side < m_size ? side+1 : side;
    Vector actualDest;
    actualDest.x = dest.x - (side*offset)/2;
    actualDest.y = dest.y - (side*offset)/2;

    int counter=0;

    for(std::list<Unit*>::iterator it=m_selection.begin(); it!=m_selection.end(); it++)
    {
        (*it)->go(actualDest);
        counter++;
        if(counter<side)
            actualDest.x += offset;
        else
        {
            actualDest.x = dest.x - (side*offset)/2;
            actualDest.y += offset;
            counter = 0;
        }

    }
}

void Selection::givePath(std::list<Vector> path, bool loop, float width, float offset)
{
    if(path.empty())
        return;

    int side = sqrt(m_size);
    side = side*side < m_size ? side+1 : side;
    Vector fDest = path.back();
    Vector actualDest;
    actualDest.x = fDest.x - (side*offset)/2;
    actualDest.y = fDest.y - (side*offset)/2;

    int counter=0;

    for(std::list<Unit*>::iterator it=m_selection.begin(); it!=m_selection.end(); it++)
    {
        if(!loop)
        {
            path.pop_back();
            path.push_back(actualDest);
            counter++;
            if(counter<side)
                actualDest.x += offset;
            else
            {
                actualDest.x = fDest.x - (side*offset)/2;
                actualDest.y += offset;
                counter = 0;
            }
        }
        (*it)->setPath(path, loop, width);
    }
}
