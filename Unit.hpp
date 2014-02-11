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

        void go(Vector dest);
        void idle();

        bool clicked(float x, float y);
        bool intersect(const sf::FloatRect& rect);

        enum Command
        {
            GO,
            IDLE
        };

    protected:
        std::list<Command> m_commands;
        Vector m_target;
};

#endif // UNIT_H
