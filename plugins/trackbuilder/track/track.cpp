#include "track.h"
#include "secstraight.h"
#include "seccurved.h"
#include "secforced.h"

#define RELTHRESH 0.98f

using namespace std;

track::track()
{

}

track::track(glm::vec3 startPos, float startYaw, float heartLine)
{
    this->anchorNode = new mnode(glm::vec3(0.f, 0.f, 0.f), glm::vec3(0, 0, -1), 0., 20.f, 1., 0.);
    this->startPos = startPos;
    this->startYaw = startYaw;
    this->startPitch = 0.f;
    anchorNode->updateNorm();
    this->fHeart = heartLine;
    fFriction = 0.27;
    hasChanged = true;
    drawTrack = true;
    drawHeartline = true;
    activeSection = NULL;
}

track::~track()
{
    while(lSections.size() != 0)
    {
        delete lSections.at(0);
        lSections.removeAt(0);
    }
    delete anchorNode;
}

void track::removeSection(int index)
{
    if(lSections.size() <= index) return;
    else if(index == lSections.size()-1)
    {
        delete this->lSections.at(index);
        this->lSections.removeAt(index);
        if(lSections.size() != 0) activeSection = lSections.at(index-1);
        //updateTrack(index-1, lSections[index-1]->lNodes.size()-2);
    }
    else
    {
        lSections.at(index+1)->lNodes.prepend(lSections.at(index)->lNodes.at(0));
        delete this->lSections.at(index);
        this->lSections.removeAt(index);
        activeSection = lSections.at(index);

        updateTrack(index, lSections[index]->lNodes.size()-2);
    }
}

int track::getSectionIndex(section* fromSection) {
    int i = 0;
    if(lSections.size() == 0) return 0;
    for(; i > lSections.size(); ++i)
    {
        if(lSections.at(i) == fromSection) break;
    }
    return i;
}

void track::removeSection(section* fromSection)
{
    int i = 0;
    if(lSections.size() == 0) return;
    for(; i > lSections.size(); ++i)
    {
        if(lSections.at(i) == fromSection) break;
    }
    removeSection(i);
}

void track::updateTrack(int index, int iNode)
{
    //qDebug("called updateTrack(%d, %d)", index, iNode);
    if(index < 0) index = 0;
    if(lSections.size() <= index)
    {
        hasChanged = true;
        return;   // for savety
    }

    lSections.at(index)->updateSection(iNode);
    for(index++; index < lSections.size(); index++)
    {
        lSections.at(index)->lNodes.prepend(lSections.at(index-1)->lNodes.at(lSections.at(index-1)->lNodes.size()-1));
        lSections.at(index)->updateSection(0);
    }
    hasChanged = true;
}

void track::updateTrack(section* fromSection, int iNode)
{
    int i = 0;
    if(lSections.size() == 0) return;
    for(; i < lSections.size(); ++i)
    {
        if(lSections.at(i) == fromSection) break;
    }
    updateTrack(i, iNode);
}

section *track::newSection(enum secType type, int index)
{
    mnode* startNode;
    if(!lSections.isEmpty())
    {
        section* temp;
        if(index == -1)
        {
            temp = lSections.at(lSections.size()-1);
            startNode = temp->lNodes.at(temp->lNodes.size()-1);
        }
        else if(index == 0)
        {
            startNode = anchorNode;
            lSections.at(0)->lNodes.removeFirst();
        }
        else
        {
            temp = lSections.at(index-1);
            startNode = temp->lNodes[temp->lNodes.size()-1];
            if(lSections.size() > index)
            {
                lSections.at(index)->lNodes.removeFirst();
            }
        }
    }
    else
    {
        startNode = anchorNode;
    }

    section* newSection;
    switch(type)
    {
    case 1:
        newSection = new secstraight(this, startNode, 10);
        break;
    case 2:
        newSection = new seccurved(this, startNode, 90, 15);
        break;
    case 3:
        newSection = new secforced(this, startNode, 1000);
        break;
    case 4:
        //newSection = new secgeometric(this, startNode, 1000);
        break;
    case 5:
        //newSection = new secbezier(this, startNode);
        break;
    default:
        qDebug("Error: Wrong Section type defined!");
    }
    activeSection = newSection;
    if(index == -1)
    {
        lSections.append(newSection);
        newSection->updateSection();
    }
    else if(index == 0)
    {
        lSections.prepend(newSection);
        newSection->updateSection();
        if(lSections.size() > 1)
        {
            lSections.at(1)->lNodes.prepend(newSection->lNodes[newSection->lNodes.size()-1]);
        }
    }
    else
    {
        lSections.insert(index, newSection);
        newSection->updateSection();
        if(lSections.size() > index+1)
        {
            lSections.at(index+1)->lNodes.prepend(newSection->lNodes[newSection->lNodes.size()-1]);
        }
    }
    hasChanged = true;

    return newSection;
}

mnode* track::getPoint(int index)
{
    int i = 0;
    while(lSections.size() > i && index >= lSections.at(i)->lNodes.size()-1)
    {
        index -= lSections.at(i++)->lNodes.size()-1;
    }
    if(lSections.size() == i) return NULL;
    return lSections.at(i)->lNodes[index];
}

int track::getNumPoints(section* until)
{

    int sum = 0;
    for(int i = 0; i < lSections.size(); ++i)
    {
        if(lSections.at(i) == until) return sum;
        sum += lSections.at(i)->lNodes.size()-1;
    }
    return sum;
}

int track::getSectionNumber(section *_section)
{
    int number = 0;
    while(number < lSections.size() && lSections.at(number) != _section) ++number;
    if(number < lSections.size()) {
        return number;
    } else {
        return -1;
    }
}
