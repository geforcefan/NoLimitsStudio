#include "saveloadhandler.h"
#include "transition/transitionmanager.h"
#include "transition/transitiongeneric.h"

saveloadHandler::saveloadHandler()
{
}

void saveloadHandler::setNode(pugi::xml_node _node) {
    node = _node;
}

void saveloadHandler::save() {
    /*for(int i=0; i < trackBuilder->sectionList->Sections.size(); i++) {
        Section *section = trackBuilder->sectionList->Sections[i];

        pugi::xml_node sectionNode = node.append_child("Section");
        sectionNode.append_attribute("name").set_value(section->SectionName);
        sectionNode.append_attribute("type").set_value(section->type);

        sectionNode.append_child("BezierIndex").append_attribute("value").set_value(section->bezierIndex);

        // Save Inital Data
        if(section->type == SECTION_INITAL) {
            SectionInitial *secInital = static_cast<SectionInitial*>(section);

            sectionNode.append_child("Speed").append_attribute("value").set_value(secInital->outputValues->speed);
            sectionNode.append_child("VerticalForce").append_attribute("value").set_value(secInital->outputValues->vert);
            sectionNode.append_child("LateralForce").append_attribute("value").set_value(secInital->outputValues->lat);
            sectionNode.append_child("Roll").append_attribute("value").set_value(secInital->Gui->ui->roll->value());

            pugi::xml_node originNode = sectionNode.append_child("Origin");
            originNode.append_attribute("x").set_value(secInital->Gui->ui->x->value());
            originNode.append_attribute("y").set_value(secInital->Gui->ui->y->value());
            originNode.append_attribute("z").set_value(secInital->Gui->ui->z->value());

            sectionNode.append_child("Yaw").append_attribute("value").set_value(secInital->Gui->ui->yaw->value());
            sectionNode.append_child("Pitch").append_attribute("value").set_value(secInital->Gui->ui->pitch->value());
            sectionNode.append_child("Heartline").append_attribute("value").set_value(secInital->outputValues->heartlineDistance);
            sectionNode.append_child("Friction").append_attribute("value").set_value(secInital->outputValues->friction);
        }
    }*/
}

void saveloadHandler::open() {
    /*
    for(int i=0; i < trackBuilder->sectionList->Sections.size(); i++) {
        trackBuilder->sectionList->Sections[i]->kill();
        delete trackBuilder->sectionList->Sections[i];
    }
    trackBuilder->sectionList->Sections.clear();
    trackBuilder->widgetSectionList->clear();

    int indexOfSection = 0;
    // Loop sections
    for (pugi::xml_node sectionNode = node.child("Section"); sectionNode; sectionNode = sectionNode.next_sibling("Section")) {
        int SectionType = sectionNode.attribute("type").as_int();
        trackBuilder->sectionList->addSection(SectionType);
        Section *section = trackBuilder->sectionList->getSection(indexOfSection);
        indexOfSection++;

        section->bezierIndex = sectionNode.child("BezierIndex").attribute("value").as_float();

        if(section->type == SECTION_INITAL) {
            SectionInitial *secInitial = static_cast<SectionInitial*>(section);

            secInitial->Gui->ui->speed->setValue(sectionNode.child("Speed").attribute("value").as_float());
            secInitial->Gui->ui->vert->setValue(sectionNode.child("VerticalForce").attribute("value").as_float());
            secInitial->Gui->ui->lat->setValue(sectionNode.child("LateralForce").attribute("value").as_float());
            secInitial->Gui->ui->roll->setValue(sectionNode.child("Roll").attribute("value").as_float());

            secInitial->Gui->ui->x->setValue(sectionNode.child("Origin").attribute("x").as_float());
            secInitial->Gui->ui->y->setValue(sectionNode.child("Origin").attribute("y").as_float());
            secInitial->Gui->ui->z->setValue(sectionNode.child("Origin").attribute("z").as_float());

            secInitial->Gui->ui->yaw->setValue(sectionNode.child("Yaw").attribute("value").as_float());
            secInitial->Gui->ui->pitch->setValue(sectionNode.child("Pitch").attribute("value").as_float());
            secInitial->Gui->ui->heartline->setValue(sectionNode.child("Heartline").attribute("value").as_float());
            secInitial->Gui->ui->friction->setValue(sectionNode.child("Friction").attribute("value").as_float());
        }
    }

    trackBuilder->sectionList->recalculate(0);*/
}

void saveloadHandler::saveCurve(pugi::xml_node curveNode, qFVDCurve *curve) {
    for(int j=0; j < curve->points.size(); j++) {
        qFVDCurvePoint *_point = curve->points[j];
        pugi::xml_node pointNode = curveNode.append_child("Point");

        pointNode.append_attribute("x").set_value(_point->x);
        pointNode.append_attribute("y").set_value(_point->y);
        pointNode.append_attribute("refPoint").set_value(_point->refPoint);

        pointNode.append_attribute("type").set_value((int)_point->transition->getTransitionType());
        pointNode.append_attribute("mode").set_value((int)_point->transition->getMode());
        pointNode.append_attribute("center").set_value(_point->transition->getCenter());
        pointNode.append_attribute("tension").set_value(_point->transition->getTension());

        if(_point->transition->getTransitionType() == TransitionManager::GenericTransition) {
            TransitionGeneric *tGeneric = static_cast<TransitionGeneric*>(_point->transition->getTransition());

            pointNode.append_attribute("UseSlope").set_value(tGeneric->useSlope);
            pointNode.append_attribute("Slope").set_value(tGeneric->slope);
            pointNode.append_attribute("Shape1").set_value(tGeneric->shape1);
            pointNode.append_attribute("Shape2").set_value(tGeneric->shape2);
        }
    }
}

void saveloadHandler::readCurve(pugi::xml_node curveNode, qFVDCurve *curve) {
    int pointIndex = 0;
    for (pugi::xml_node pointNode = curveNode.child("Point"); pointNode; pointNode = pointNode.next_sibling("Point")) {
        if(!pointIndex) {
            qFVDCurvePoint curvePoint;
            curvePoint.x = pointNode.attribute("x").as_float();
            curvePoint.y = pointNode.attribute("y").as_float();
            curvePoint.refPoint = pointNode.attribute("refPoint").as_int();

            curvePoint.transition = new TransitionManager();
            int index = pointNode.attribute("type").as_int();

            if(index == 0) curvePoint.transition->setTransitionType(TransitionManager::BasicTransition);
            if(index == 1) curvePoint.transition->setTransitionType(TransitionManager::BumpTransition);
            if(index == 2) curvePoint.transition->setTransitionType(TransitionManager::LinearTransition);
            if(index == 4) curvePoint.transition->setTransitionType(TransitionManager::QuinticTransition);
            if(index == 5) curvePoint.transition->setTransitionType(TransitionManager::GenericTransition);

            curvePoint.transition->setMode(pointNode.attribute("mode").as_int());
            curvePoint.transition->setCenter(pointNode.attribute("center").as_float() / (1.0f/75.0f));
            curvePoint.transition->setTension(pointNode.attribute("tension").as_float() / (1.0f/75.0f));


            if(curvePoint.transition->getTransitionType() == TransitionManager::GenericTransition) {
                TransitionGeneric *tGeneric = static_cast<TransitionGeneric*>(curvePoint.transition->getTransition());

                tGeneric->useSlope = pointNode.attribute("UseSlope").as_bool();
                tGeneric->slope = pointNode.attribute("Slope").as_float();
                tGeneric->shape1 = pointNode.attribute("Shape1").as_float();
                tGeneric->shape2 = pointNode.attribute("Shape2").as_float();
            }

            curve->modifyPoint(0, curvePoint);
        } else {
            qFVDCurvePoint *curvePoint = new qFVDCurvePoint();
            curvePoint->x = pointNode.attribute("x").as_float();
            curvePoint->y = pointNode.attribute("y").as_float();
            curvePoint->refPoint = pointNode.attribute("refPoint").as_int();

            curvePoint->transition = new TransitionManager();
            int index = pointNode.attribute("type").as_int();

            if(index == 0) curvePoint->transition->setTransitionType(TransitionManager::BasicTransition);
            if(index == 1) curvePoint->transition->setTransitionType(TransitionManager::BumpTransition);
            if(index == 2) curvePoint->transition->setTransitionType(TransitionManager::LinearTransition);
            if(index == 4) curvePoint->transition->setTransitionType(TransitionManager::QuinticTransition);
            if(index == 5) curvePoint->transition->setTransitionType(TransitionManager::GenericTransition);

            curvePoint->transition->setMode(pointNode.attribute("mode").as_int());
            curvePoint->transition->setCenter(pointNode.attribute("center").as_float() / (1.0f/75.0f));
            curvePoint->transition->setTension(pointNode.attribute("tension").as_float() / (1.0f/75.0f));


            if(curvePoint->transition->getTransitionType() == TransitionManager::GenericTransition) {
                TransitionGeneric *tGeneric = static_cast<TransitionGeneric*>(curvePoint->transition->getTransition());

                tGeneric->useSlope = pointNode.attribute("UseSlope").as_bool();
                tGeneric->slope = pointNode.attribute("Slope").as_float();
                tGeneric->shape1 = pointNode.attribute("Shape1").as_float();
                tGeneric->shape2 = pointNode.attribute("Shape2").as_float();
            }


            curve->addPoint(curvePoint);
        }
        pointIndex++;
    }
}
