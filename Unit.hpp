#ifndef UNIT_H
#define UNIT_H

#include "Vehicule.hpp"
#include <list>

class QuadTree;

class Unit : public Vehicule
{
    public:
        Unit();
        virtual ~Unit();

        virtual void update(const float& frameTime);
        virtual void update(const float& frameTime, std::vector<Unit>& others);
        virtual void update(const float& frameTime, QuadTree& qTree);
        virtual void update(const float& frameTime, std::list<Unit*>& others);

        virtual void draw(sf::RenderTarget& App);

        void go(Vector dest);
        virtual void setPath(std::list<Vector> pathWay, bool loop = false, float width=1);
        void idle();
        void setWander();

        bool clicked(float x, float y);
        bool intersect(const sf::FloatRect& rect);

        void trace(bool trace = true);

        enum Command
        {
            GO,
            FOLLOW,
            WANDER,
            IDLE
        };

    protected:
        std::list<Command> m_commands;
        Vector m_target;

        bool m_trace;
        sf::Clock m_traceClock;
        sf::VertexArray m_traceArray;
};

#endif // UNIT_H
