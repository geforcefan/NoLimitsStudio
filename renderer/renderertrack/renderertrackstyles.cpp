#include <common/objloaderbuffer.h>
#include "renderertrackstyles.h"

rendererTrackStyleRail::rendererTrackStyleRail(float _radius, glm::vec2 _position, int _division) :
    radius(_radius),
    position(_position),
    division(_division)
{}

// Only a track style
rendererTrackStyle::rendererTrackStyle()
{
}

// Class to init few styles
rendererTrackStyles::rendererTrackStyles()
{
    // STYLE_CLASSIC_STEEL_LOOPING
    rendererTrackStyle *STYLE_CLASSIC_STEEL_LOOPING = new rendererTrackStyle();
    STYLE_CLASSIC_STEEL_LOOPING->basicRailSpacing = 0.2f;

    STYLE_CLASSIC_STEEL_LOOPING->basicRails.push_back( NoLimitsRenderer::makeCircle(0.0625f, 6, glm::vec3(0.0f,  0.4375f, 0.0f)) );
    STYLE_CLASSIC_STEEL_LOOPING->basicRails.push_back( NoLimitsRenderer::makeCircle(0.0625f, 6, glm::vec3(0.0f, -0.4375f, 0.0f)) );

    STYLE_CLASSIC_STEEL_LOOPING->Spine0 = new rendererTrackStyleSpineStyle();
    STYLE_CLASSIC_STEEL_LOOPING->Spine1 = new rendererTrackStyleSpineStyle();
    STYLE_CLASSIC_STEEL_LOOPING->Spine2 = new rendererTrackStyleSpineStyle();

    STYLE_CLASSIC_STEEL_LOOPING->Spine0->spines.push_back( NoLimitsRenderer::makeCircle(0.15f, 12, glm::vec3(-0.16f, 0.0f, 0.0f)) );
    STYLE_CLASSIC_STEEL_LOOPING->Spine0->spineSpacing = 0.4f;
    STYLE_CLASSIC_STEEL_LOOPING->Spine0->primaryTie = new objLoaderBuffer(":/trackstyles/CLASSIC_STEEL_LOOPING/Spine1-primaryTie.obj");
    STYLE_CLASSIC_STEEL_LOOPING->Spine0->alternativeTie = new objLoaderBuffer(":/trackstyles/CLASSIC_STEEL_LOOPING/Spine1-primaryTie.obj");
    STYLE_CLASSIC_STEEL_LOOPING->Spine0->tieSpacing = 1.40f;
    STYLE_CLASSIC_STEEL_LOOPING->Spine0->useLastTie = false;
    STYLE_CLASSIC_STEEL_LOOPING->Spine0->supportOffset = 0.25f;
    STYLE_CLASSIC_STEEL_LOOPING->Spine0->useConnector = false;

    STYLE_CLASSIC_STEEL_LOOPING->Spine1->primaryTie = new objLoaderBuffer(":/trackstyles/CLASSIC_STEEL_LOOPING/Spine2-primaryTie.obj");
    STYLE_CLASSIC_STEEL_LOOPING->Spine1->lastTie = new objLoaderBuffer(":/trackstyles/CLASSIC_STEEL_LOOPING/Spine2-lastTie.obj");
    STYLE_CLASSIC_STEEL_LOOPING->Spine1->connector = new objLoaderBuffer(":/trackstyles/CLASSIC_STEEL_LOOPING/Spine2-connector.obj");
    STYLE_CLASSIC_STEEL_LOOPING->Spine1->tieSpacing = 1.0f;
    STYLE_CLASSIC_STEEL_LOOPING->Spine1->useLastTie = true;
    STYLE_CLASSIC_STEEL_LOOPING->Spine1->useConnector = true;
    STYLE_CLASSIC_STEEL_LOOPING->Spine1->supportOffset = 0.3f;

    STYLE_CLASSIC_STEEL_LOOPING->Spine2->spines.push_back( NoLimitsRenderer::makeSquare(glm::vec3(0.7, 0.7f, 0.0f), glm::vec3(-0.40f, 0.0f, 0.0f)) );
    STYLE_CLASSIC_STEEL_LOOPING->Spine2->spineSpacing = 0.4f;
    STYLE_CLASSIC_STEEL_LOOPING->Spine2->primaryTie = new objLoaderBuffer(":/trackstyles/CLASSIC_STEEL_LOOPING/Spine3-primaryTie.obj");
    STYLE_CLASSIC_STEEL_LOOPING->Spine2->alternativeTie = new objLoaderBuffer(":/trackstyles/CLASSIC_STEEL_LOOPING/Spine3-primaryTie.obj");
    STYLE_CLASSIC_STEEL_LOOPING->Spine2->tieSpacing = 1.40f;
    STYLE_CLASSIC_STEEL_LOOPING->Spine2->useLastTie = false;
    STYLE_CLASSIC_STEEL_LOOPING->Spine2->supportOffset = 0.3f;
    STYLE_CLASSIC_STEEL_LOOPING->Spine2->useConnector = false;

    // FLOORLESS_TWISTED_STEEL_COASTER
    rendererTrackStyle *STYLE_FLOORLESS_TWISTED_STEEL_COASTER = new rendererTrackStyle();
    STYLE_FLOORLESS_TWISTED_STEEL_COASTER->basicRailSpacing = 0.2f;
    STYLE_FLOORLESS_TWISTED_STEEL_COASTER->basicRails.push_back( NoLimitsRenderer::makeCircle(0.0625f, 6, glm::vec3(0.0f,  0.6f, 0.0f)) );
    STYLE_FLOORLESS_TWISTED_STEEL_COASTER->basicRails.push_back( NoLimitsRenderer::makeCircle(0.0625f, 6, glm::vec3(0.0f, -0.6f, 0.0f)) );

    STYLE_FLOORLESS_TWISTED_STEEL_COASTER->Spine0 = new rendererTrackStyleSpineStyle();

    STYLE_FLOORLESS_TWISTED_STEEL_COASTER->Spine0->spines.push_back(NoLimitsRenderer::makeFormFormFile(":/trackstyles/FLOORLESS_TWISTED_STEEL_COASTER/SpineForm.txt"));
    STYLE_FLOORLESS_TWISTED_STEEL_COASTER->Spine0->spineSpacing = 1.40f;

    STYLE_FLOORLESS_TWISTED_STEEL_COASTER->Spine0->primaryTie = new objLoaderBuffer(":/trackstyles/FLOORLESS_TWISTED_STEEL_COASTER/Spine1-primaryTie.obj");
    STYLE_FLOORLESS_TWISTED_STEEL_COASTER->Spine0->alternativeTie = new objLoaderBuffer(":/trackstyles/FLOORLESS_TWISTED_STEEL_COASTER/Spine1-alternativeTie.obj");
    STYLE_FLOORLESS_TWISTED_STEEL_COASTER->Spine0->alternativeTieRepeat = 10;

    STYLE_FLOORLESS_TWISTED_STEEL_COASTER->Spine0->tieSpacing = 1.40f;
    STYLE_FLOORLESS_TWISTED_STEEL_COASTER->Spine0->useLastTie = false;
    STYLE_FLOORLESS_TWISTED_STEEL_COASTER->Spine0->supportOffset = 0.68f;

    Styles[CLASSIC_STEEL_LOOPING] = STYLE_CLASSIC_STEEL_LOOPING;
    Styles[STEEL_LOOPING_COASTER] = STYLE_CLASSIC_STEEL_LOOPING;
    Styles[FLOORLESS_TWISTED_STEEL_COASTER] = STYLE_FLOORLESS_TWISTED_STEEL_COASTER;
    Styles[HYPER_COASTER_4] = STYLE_FLOORLESS_TWISTED_STEEL_COASTER;
}
