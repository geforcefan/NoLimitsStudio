#include "transition.h"

Transition::Transition(TransitionManager *_transition) {
    transition = _transition;
    lastY = 0.0f;
    mode = 0;
}
