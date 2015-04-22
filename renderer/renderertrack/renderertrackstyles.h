#ifndef RENDERERTRACKSTYLES_H
#define RENDERERTRACKSTYLES_H

#include <common/global.h>

class objLoaderBuffer;

// RAIL
class rendererTrackStyleRail {
public:
    float radius;
    float division;
    glm::vec2 position;
    rendererTrackStyleRail(float _radius, glm::vec2 _position, int _division);
};

class rendererTrackStyleSpineStyle {
public:
    std::vector <vec4List> rails;
    std::vector <vec4List> spines;
    float railSpacing;
    float spineSpacing;

    objLoaderBuffer *primaryTie;
    objLoaderBuffer *alternativeTie;
    objLoaderBuffer *lastTie;
    objLoaderBuffer *connector;

    bool useLastTie, useConnector;
    int alternativeTieRepeat;
    float tieSpacing;
    float supportOffset;
};
////////////////////////////////////////////////////////////
class rendererTrackStyle
{
public:
    rendererTrackStyle();

    std::vector <vec4List> basicRails;
    float basicRailSpacing;

    rendererTrackStyleSpineStyle *Spine0;
    rendererTrackStyleSpineStyle *Spine1;
    rendererTrackStyleSpineStyle *Spine2;
};
////////////////////////////////////////////////////////////
class rendererTrackStyles
{
public:
    rendererTrackStyles();
    std::map<int, rendererTrackStyle*> Styles;
};

#endif // RENDERERTRACKSTYLES_H
