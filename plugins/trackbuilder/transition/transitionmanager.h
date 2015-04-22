#ifndef TRANSITIONMANAGER_H
#define TRANSITIONMANAGER_H

#include <vector>

class Transition;

class TransitionManager
{
public:
    enum Type {
        BasicTransition = 0,
        BumpTransition = 1,
        LinearTransition = 2,
        QuinticTransition = 4,
        GenericTransition = 5
    };

    TransitionManager();

    double getY(double x);
    double getDerivative(double x);

    void setTransitionType(TransitionManager::Type TransitionType);
    TransitionManager::Type getTransitionType() { return transitionType; }

    double setStartX(double _paramStartX) { paramStartX = _paramStartX;  }
    double setStartY(double _paramStartY) { paramStartY = _paramStartY;  }
    double setChangeX(double _paramChangeX) { paramChangeX = _paramChangeX;  }
    double setChangeY(double _paramChangeY) { paramChangeY = _paramChangeY;  }

    double getStartX() { return paramStartX; }
    double getStartY() { return paramStartY; }
    double getChangeX() { return paramChangeX; }
    double getChangeY() { return paramChangeY; }

    double sinFuncA(double a, double b);
    double sinFuncB(double a, double b);

    double getTension();
    double getCenter();
    double setTension(double _tension);
    double setCenter(double _center);

    double getLastY();

    void setPrevTransition(TransitionManager *pTransition) {
        prevTransition = pTransition;
        hasPrevTransition = true;
    }

    void setNextTransition(TransitionManager *nTransition) {
        nextTransition = nTransition;
        hasNextTransition = true;
    }

    Transition *getTransition();

    std::vector<QString> getModes();
    void setMode(int _mode);
    int getMode();

    TransitionManager *prevTransition;
    TransitionManager *nextTransition;

    bool hasPrevTransition;
    bool hasNextTransition;

private:
    double paramStartX;
    double paramStartY;
    double paramChangeX;
    double paramChangeY;

    double tension;
    double center;

    Transition *transition;
    TransitionManager::Type transitionType;
};

#endif // TRANSITIONMANAGER_H
