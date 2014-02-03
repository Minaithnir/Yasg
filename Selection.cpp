#include "Selection.hpp"

Selection::Selection() : m_select(NULL)
{
    m_shapeIcon = sf::CircleShape(10,16);
    m_shapeIcon.setOutlineThickness(2);
    m_shapeIcon.setOutlineColor(sf::Color::Red);
    m_shapeIcon.setOrigin(10,10);
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

void Selection::draw(sf::RenderWindow& screen)
{
    for(std::list<Unit*>::iterator it=m_selection.begin(); it!=m_selection.end(); it++)
    {
        m_shapeIcon.setPosition((*it)->getPosition().x, (*it)->getPosition().y);
        screen.draw(m_shapeIcon);
    }

    if(m_select)
    {
        m_selectShape[1].position = sf::Vector2f(sf::Mouse::getPosition(screen).x, m_selectRect.top);
        m_selectShape[2].position = sf::Vector2f(sf::Mouse::getPosition(screen).x, sf::Mouse::getPosition(screen).y);
        m_selectShape[3].position = sf::Vector2f(m_selectRect.left, sf::Mouse::getPosition(screen).y);

        screen.draw(m_selectShape);
    }
}

void Selection::giveDest(Vector dest, float offset)
{
    int side = sqrt(m_size);
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
