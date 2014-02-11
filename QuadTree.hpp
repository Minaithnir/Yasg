#ifndef QUADTREE_H
#define QUADTREE_H

#include <SFML/Graphics.hpp>
#include "Unit.hpp"
#include <list>

#define NODE_CAPACITY 2
#define MIN_NODE_SIZE UNIT_RADIUS*2

/**
 * QuadTree class
 */
class QuadTree
{
    public:
        QuadTree(sf::FloatRect boundaries);
        virtual ~QuadTree();

        bool insert(Unit* entity);
        bool remove(Unit* entity);
        bool merge();
        void split();

        void getEntities(std::list<Unit*>& entities);
        void range(float x, float y, float range, std::list<Unit*>& entities);
        void clear();

        void display(sf::RenderTarget& screen);

        long getCount();

    protected:
        std::list<Unit*> m_entities;

        sf::FloatRect m_boundaries;

        sf::VertexArray m_lines;

        QuadTree* m_nw;
        QuadTree* m_ne;
        QuadTree* m_sw;
        QuadTree* m_se;
};

#endif // QUADTREE_H
