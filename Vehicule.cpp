#include "Vehicule.hpp"

#include <iostream>

Vehicule::Vehicule() :
    m_position(Vector(0,0)),
    m_velocity(Vector(0,0)),
    m_maxSpeed(MAX_SPEED),
    m_maxForce(MAX_FORCE),
    m_mass(DEFAULT_MASS),
    m_radius(RADIUS),
    m_arrival(true),
    m_waypointIndex(0)
{
    m_drawing.setPointCount(3);
    m_drawing.setPoint(0, sf::Vector2f(-5,5));
    m_drawing.setPoint(1, sf::Vector2f(-5,-5));
    m_drawing.setPoint(2, sf::Vector2f(10,0));
    m_drawing.setFillColor(sf::Color::Black);
}

Vehicule::~Vehicule()
{
}

/**
    d�fini la couleur du vehicule
*/
void Vehicule::setColor(sf::Color color)
{
    m_drawing.setFillColor(color);
}

/**
    impl�mentation de la fonction virtuelle de mise � jour de l'objet
*/
void Vehicule::updatePos(const float& frameTime)
{
    m_steering.truncate(m_maxForce*frameTime);

    m_velocity += (m_steering/m_mass);
    m_velocity.truncate(m_maxSpeed*frameTime);

    m_position += m_velocity;

    m_steering = Vector(0,0);
}

/**
    dessin de l'objet sur la renderTarget pass�e en argument
*/
void Vehicule::draw(sf::RenderTarget& App)
{
    m_drawing.setPosition(m_position);
    m_drawing.setRotation(m_velocity.getAngle());
    App.draw(m_drawing);
}

/// helpers ///

/**
    retourne la position de l'unit� dans le futur
*/
Vector Vehicule::getFuturePosition(const float& futureTime)
{
    return m_position + m_velocity*futureTime;
}


///Steering Behaviors///

/**
    balade
*/
Vector Vehicule::wander()
{
    Vector centre = m_velocity;
    centre.normalize();
    centre.setLengh(3*m_maxForce);/// on d�fini le centre du cercle

    Vector random((rand()%101)-50, (rand()%101)-50);
    random.truncate(m_maxForce); /// on cr�e une force al�atoire;

    m_wanderFactor += random;
    m_wanderFactor.truncate(m_maxForce); /// on l'ajoute � l'ancienne force de wander

    Vector steering = m_wanderFactor+centre;
    steering.truncate(m_maxForce);

    return steering;
}

/**
    cherche � atteindre la cible
*/
Vector Vehicule::seek(const Vector& target, const float& offset)
{
    Vector _target = target - m_position;
    _target.setLengh(_target.norme()-offset);
    _target = _target + m_position;

    Vector desiredVelocity, steering;

    desiredVelocity = _target - m_position;
    if(desiredVelocity.norme() > APPROX_ARRIVAL)
    {
        if(m_arrival)
        {
            float slowDist = SLOW_DIST_FACTOR * m_maxSpeed * m_mass;
            float ramped_speed = m_maxSpeed * (desiredVelocity.norme() / slowDist);
            float clipped_speed;
            if (ramped_speed < m_maxSpeed)
            {
                clipped_speed = ramped_speed;
            }
            else
                clipped_speed = m_maxSpeed;

            desiredVelocity.setLengh(clipped_speed);
        }


        steering = desiredVelocity - m_velocity;
        steering.truncate(m_maxForce);
    }
    else
    {
        steering -= m_velocity;
        steering.truncate(m_maxForce);
    }

    return steering;
}

/**
    cherche � s'�loigner du point cible
*/
Vector Vehicule::flee(const Vector& target)
{
    Vector desiredVelocity, steering;

    desiredVelocity = -(target - m_position);
    desiredVelocity.normalize();
    desiredVelocity.setLengh(m_maxSpeed);
    steering = desiredVelocity - m_velocity;
    steering.truncate(m_maxForce);

    return steering;
}

/**
    cherche � attrapper une unit�
*/
Vector Vehicule::pursue(Vehicule& target, const float& offset)
{
    Vector diff = target.getPosition() - m_position;
    float distance = diff.norme();
    float anticipation_time=0;
    if(target.getVelocity().norme()>0)
        anticipation_time = distance / target.getVelocity().norme();

    Vector futur_position = target.getPosition() + (target.getVelocity()*anticipation_time/2.f);
    return seek(futur_position, offset);
}

/**
    cherche � �viter une unit�
*/
Vector Vehicule::evade(Vehicule& target)
{
    Vector diff = target.getPosition() - m_position;
    float distance = diff.norme();
    float anticipation_time=0;
    if(target.getVelocity().norme()>0)
        anticipation_time = distance / target.getVelocity().norme();

    Vector futur_position = target.getPosition() + (target.getVelocity()*anticipation_time/2.f);
    return flee(futur_position);
}

/**
    calcule la force necessaire pour �viter les collision � venir avec d'autres unit�es
*/
Vector Vehicule::unallignedCollisionAvoidance(std::vector<Vehicule*>& others)
{
    Vector steering(0,0);
    int density = 0;

    for(unsigned int i=0; i<others.size(); i++)
    {
        if(others[i] != this)
        {
            /// si les deux unit� sont assez proches
            if(Vector(others[i]->getPosition()-m_position).norme() < 50.f)
            {
                Vector futurDiff(others[i]->getFuturePosition(UCATIME) - getFuturePosition(UCATIME));
                if(futurDiff.norme() < 1.5*m_radius)
                {
                    if(futurDiff.norme()>m_radius)
                        futurDiff.setLengh(futurDiff.norme()-m_radius);
                    steering -= futurDiff;
                    density++;
                }
            }
        }
    }

    return steering;
}

/**
    calcule la force necessaire pour �viter les collision � venir avec d'autres unit�es
*/
Vector Vehicule::unallignedCollisionAvoidance(std::vector<Vehicule>& others)
{
    std::vector<Vehicule*> unitsPtr;
    for(unsigned int i=0; i<others.size(); i++)
    {
        unitsPtr.push_back(&(others[i]));
    }

    return unallignedCollisionAvoidance(unitsPtr);
}
