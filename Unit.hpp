#ifndef UNIT_H
#define UNIT_H

#include "Vehicule.hpp"
#include <list>


class Unit : public Vehicule
{
    public:
        Unit();
        virtual ~Unit();

        virtual void update(const float& frameTime);
        virtual void update(const float& frameTime, std::vector<Unit>& others);

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
