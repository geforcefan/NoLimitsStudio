#ifndef TRANSITION_H
#define TRANSITION_H

#include <QString>
#include <vector>

class TransitionManager;

class Transition {
public:
    Transition(TransitionManager *_transition);

    virtual double getY(double x) = 0;
    virtual std::vector<QString> getModes() = 0;
    double getLastY() { return lastY; }

    void setMode(int _mode) { mode = _mode; }
    int getMode() { return mode; }

protected:
    TransitionManager *transition;
    double lastY;

    int mode;
};

#endif // TRANSITION_H
