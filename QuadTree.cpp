#include "QuadTree.hpp"
#include <iostream>
#include <sstream>

/**
Constructor
define the boundaries of the node
*/
QuadTree::QuadTree(sf::FloatRect boundaries) : m_boundaries(boundaries), m_nw(NULL), m_ne(NULL), m_sw(NULL), m_se(NULL)
{
    m_lines = sf::VertexArray(sf::LinesStrip, 3);
    m_lines[0].position = sf::Vector2f(m_boundaries.left+m_boundaries.width, m_boundaries.top);
    m_lines[0].color = sf::Color::Black;
    m_lines[1].position = sf::Vector2f(m_boundaries.left+m_boundaries.width, m_boundaries.top+m_boundaries.height);
    m_lines[1].color = sf::Color::Black;
    m_lines[2].position = sf::Vector2f(m_boundaries.left, m_boundaries.top+m_boundaries.height);
    m_lines[2].color = sf::Color::Black;
}

/**
Destructor
Relese the memory allocated to the children node if any
*/
QuadTree::~QuadTree()
{
    m_entities.clear();

    if(m_nw != NULL)
    {
        delete m_nw;
        delete m_ne;
        delete m_sw;
        delete m_se;
    }
}

/**
Insert
Insert an entity into the node or one of the child node if the capacity is exceeded
*/
bool QuadTree::insert(Unit* entity)
{
    if(!m_boundaries.contains(entity->getPosition()))
        return false;

    if((m_entities.size() < NODE_CAPACITY && m_nw == NULL) || (m_boundaries.height/2 < MIN_NODE_SIZE) || (m_boundaries.width/2 < MIN_NODE_SIZE))
    {
        m_entities.push_back(entity);
        return true;
    }

    if(m_nw == NULL)
        split();

    if(m_nw->insert(entity))
        return true;
    if(m_ne->insert(entity))
        return true;
    if(m_sw->insert(entity))
        return true;
    if(m_se->insert(entity))
        return true;

    return false;
}

/**
Allocate the four child node and insert the entity currently in the node into the childrens
*/
void QuadTree::split()
{
    float halfWidth = m_boundaries.width/2.0f;
    float halfHeight = m_boundaries.height/2.0f;
    float x = m_boundaries.left;
    float y = m_boundaries.top;
    m_nw = new QuadTree(sf::FloatRect(x, y,
                                      halfWidth, halfHeight));
    m_ne = new QuadTree(sf::FloatRect(x+halfWidth, y,
                                      halfWidth, halfHeight));
    m_sw = new QuadTree(sf::FloatRect(x, y+halfHeight,
                                      halfWidth, halfHeight));
    m_se = new QuadTree(sf::FloatRect(x+halfWidth, y+halfHeight,
                                      halfWidth, halfHeight));

    Unit* entity;
    while(!m_entities.empty())
    {
        entity = m_entities.back();
        insert(entity);
        m_entities.pop_back();
    }
}

/**
Retrieve entities from a node and its childrens if any
*/
void QuadTree::getEntities(std::list<Unit*>& entities)
{
    std::list<Unit*> childEntities;

    if(m_nw != NULL)
    {
        childEntities.clear();
        m_nw->getEntities(childEntities);
        entities.insert(entities.begin(), childEntities.begin(), childEntities.end());
        childEntities.clear();
        m_ne->getEntities(childEntities);
        entities.insert(entities.begin(), childEntities.begin(), childEntities.end());
        childEntities.clear();
        m_sw->getEntities(childEntities);
        entities.insert(entities.begin(), childEntities.begin(), childEntities.end());
        childEntities.clear();
        m_se->getEntities(childEntities);
        entities.insert(entities.begin(), childEntities.begin(), childEntities.end());
    }
    else
        entities.insert(entities.begin(), m_entities.begin(), m_entities.end());
}

/**
Retrieve entities from the node or its children if in range of a point specified in the arguments
*/
void QuadTree::range(float x, float y, float range, std::list<Unit*>& inRange)
{
    if(m_boundaries.intersects(sf::FloatRect(x-range, y-range, 2.0f*range, 2.0f*range)))
    {
        if(m_nw==NULL)
        {
            inRange.insert(inRange.begin(), m_entities.begin(), m_entities.end());
        }
        else
        {
            std::list<Unit*> child;
            m_nw->range(x,y,range, child);
            inRange.insert(inRange.begin(), child.begin(), child.end());
            child.clear();
            m_ne->range(x,y,range, child);
            inRange.insert(inRange.begin(), child.begin(), child.end());
            child.clear();
            m_sw->range(x,y,range, child);
            inRange.insert(inRange.begin(), child.begin(), child.end());
            child.clear();
            m_se->range(x,y,range, child);
            inRange.insert(inRange.begin(), child.begin(), child.end());
        }
    }
}

/**
Empty the node and delete its childrens
*/
void QuadTree::clear()
{
    if(m_nw != NULL)
    {
        m_nw->clear();
        delete m_nw;
        m_nw = NULL;

        m_ne->clear();
        delete m_ne;

        m_sw->clear();
        delete m_sw;

        m_se->clear();
        delete m_se;
    }
    else
    {
        m_entities.clear();
    }
}

/**
Display the node's children if any or the south and east boundaries if no childrens
*/
void QuadTree::display(sf::RenderTarget& screen)
{
    if(m_nw != NULL)
    {
        m_nw->display(screen);
        m_ne->display(screen);
        m_sw->display(screen);
        m_se->display(screen);
    }
    else
        screen.draw(m_lines);
}

/**
Return the number of entity in the current node and childs
*/
long QuadTree::getCount()
{
    if(m_nw != NULL)
        return m_nw->getCount() + m_ne->getCount() + m_sw->getCount() + m_se->getCount();
    else
        return m_entities.size();
}
