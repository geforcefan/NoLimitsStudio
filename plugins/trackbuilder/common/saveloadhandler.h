#ifndef SAVELOADHANDLER_H
#define SAVELOADHANDLER_H

#include "trackbuilder.h"

#include <QSpinBox>

#include <qWidgets/qfvdcurve.h>
#include <qWidgets/qsectionlist.h>

class saveloadHandler
{
public:
    saveloadHandler();
    void setNode(pugi::xml_node _node);
    void save();
    void open();

    void saveCurve(pugi::xml_node curveNode, qFVDCurve *curve);
    void readCurve(pugi::xml_node curveNode, qFVDCurve *curve);
private:
    pugi::xml_node node;
};

#endif // SAVELOADHANDLER_H
