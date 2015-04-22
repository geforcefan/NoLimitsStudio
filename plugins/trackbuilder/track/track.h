#ifndef TRACK_H
#define TRACK_H

#include "mnode.h"
#include <QList>
#include <fstream>
#include <QString>
#include "section.h"

class optionsMenu;
class sectionHandler;
class trackWidget;

class track
{
public:
    track();
    track(glm::vec3 startPos, float startYaw, float heartLine = 0.0);
    ~track();
    void removeSection(int index);
    void removeSection(section* fromSection);
    void updateTrack(int index, int iNode);
    void updateTrack(section* fromSection, int iNode);
    section *newSection(enum secType type, int index = -1);

    mnode* getPoint(int index);
    int getNumPoints(section* until = NULL);
    int getSectionNumber(section* _section);
    int getSectionIndex(section* fromSection);

    bool hasChanged;
    bool drawTrack;
    bool drawHeartline;

    mnode* anchorNode;

    glm::vec3 startPos;
    float startYaw;
    float startPitch;

    section* activeSection;
    float fHeart;
    float fFriction;
    QList<section*> lSections;
    QString name;
};

#endif // TRACK_H
