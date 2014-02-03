#ifndef UNIT_HPP
#define UNIT_HPP

#include "Vector.hpp"

#include <vector>

/// parametre des behaviors ///
#define MAX_SPEED 100.f
#define MAX_FORCE 250.f
#define DEFAULT_MASS 5.f
#define SLOW_DIST_FACTOR 2.5f // SLOW_DiST = SLOW_DIST_FACTOR * MAX_SPEED * DEFAULT_MASS
#define APPROX_ARRIVAL 1.f

#define RADIUS 10.f
#define AHEAD_CHECK (30.f*MAX_SPEED)

#define UCATIME 4

class Vehicule
{
    public:
        Vehicule();
        virtual ~Vehicule();

        /**
            getter & setters
        */
        Vector getPosition() { return m_position; }
        void setPosition(const Vector& pos) { m_position = pos; }

        Vector getVelocity() { return m_velocity; }
        void setVelocity(const Vector& speed) { m_velocity = speed; }

        float getRotation() { return m_rotation; }
        void setRotation(const float& val) { m_rotation = val; }

        float getMaxSpeed() { return m_maxSpeed; }
        void setMaxSpeed(const float& val) { m_maxSpeed = val>0 ? val : MAX_SPEED; }

        float getMaxForce() { return m_maxForce; }
        void setMaxForce(const float& val) { m_maxForce = val>0 ? val : MAX_FORCE; }

        float getMass() { return m_mass; }
        void setMass(const float& val) { m_mass = val>0 ? val : DEFAULT_MASS; }

        float getRadius() { return m_radius; }
        void setRadius(const float& val) { m_radius = val>0 ? val : RADIUS; }

        /**
            interface function
        */

        void setColor(sf::Color);

        virtual void update(const float& frameTime) = 0;
        virtual void updatePos(const float& frameTime);
        virtual void draw(sf::RenderTarget& App);

        /**
            helpers
        */
        Vector getFuturePosition(const float& futureTime);


        /**
            steering behavior functions
        */

        Vector wander();

        Vector seek(const Vector& target, const float& offset = 0);
        Vector flee(const Vector& target);

        Vector pursue(Vehicule& target, const float& offset = 0);
        Vector evade(Vehicule& target);

        Vector unallignedCollisionAvoidance(std::vector<Vehicule*>& others);
        Vector unallignedCollisionAvoidance(std::vector<Vehicule>& others);

    protected:
        /// Attributs physiques
        Vector m_position;
        Vector m_velocity;
        float m_maxSpeed;
        float m_maxForce;
        float m_mass;
        float m_minDist;
        float m_radius;
        float m_rotation;

        /// Attribut comportemental
        Vector m_steering;
        bool m_arrival;
        Vector m_wanderFactor;
        unsigned int m_waypointIndex;

        /// Attribut graphique
        sf::ConvexShape m_drawing;
};

#endif // UNIT_HPP
