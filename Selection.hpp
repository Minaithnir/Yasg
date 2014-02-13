#ifndef SELECTION_H
#define SELECTION_H

#include "Unit.hpp"

class Selection
{
    public:
        Selection();
        virtual ~Selection();

        void setSelectable(std::vector<Unit>* selectPtr);
        void setSelection(std::list<Unit*> selection);
        std::list<Unit*> getSelection();

        void startSelect(float x, float y);
        void endSelect(float x, float y);

        void draw(sf::RenderWindow& screen);

        void trace(bool trace);

        void giveDest(Vector dest, float offset=20);
        void givePath(std::list<Vector> path, bool loop=false, float width=50, float offset=20);
        void giveFreedom();

        Selection operator=(const Selection& other);

    protected:
        sf::FloatRect m_selectRect;
        bool m_select;
        std::vector<Unit>* m_selectableUnit;
        std::list<Unit*> m_selection;
        long m_size;

        sf::CircleShape m_shapeIcon;
        sf::VertexArray m_selectShape;
};

#endif // SELECTION_H
