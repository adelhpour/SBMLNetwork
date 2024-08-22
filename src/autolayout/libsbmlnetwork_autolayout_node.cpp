#include "libsbmlnetwork_autolayout_node.h"
#include "../libsbmlnetwork_layout_helpers.h"

// AutoLayoutNodeBase

AutoLayoutNodeBase::AutoLayoutNodeBase(Model* model, Layout* layout, GraphicalObject* graphicalObject, const bool& useNameAsTextLabel, const bool& locked) : AutoLayoutObjectBase(model, layout) {
    _degree = 0;
    _useNameAsTextLabel = useNameAsTextLabel;
    _graphicalObject = graphicalObject;
    setLocked(locked);
}

void AutoLayoutNodeBase::setPosition(const AutoLayoutPoint position) {
    setX(position.getX() - 0.5 * getWidth());
    setY(position.getY() - 0.5 * getHeight());
}

const AutoLayoutPoint AutoLayoutNodeBase::getPosition() {
    return AutoLayoutPoint(getX() + 0.5 * getWidth(), getY() + 0.5 * getHeight());
}

const double AutoLayoutNodeBase::getDisplacementX() {
    return _displacementX;
}

void AutoLayoutNodeBase::setDisplacementX(const double& dx) {
    _displacementX = dx;
}

const double AutoLayoutNodeBase::getDisplacementY() {
    return _displacementY;
}

void AutoLayoutNodeBase::setDisplacementY(const double& dy) {
    _displacementY= dy;
}

void AutoLayoutNodeBase::setDisplacement(const AutoLayoutPoint displacement) {
    setDisplacementX(displacement.getX());
    setDisplacementY(displacement.getY());
}

const AutoLayoutPoint AutoLayoutNodeBase::getDisplacement() {
    return AutoLayoutPoint(getDisplacementX(), getDisplacementY());
}

const int AutoLayoutNodeBase::getDegree() {
    return _degree;
}

void AutoLayoutNodeBase::incrementDegree() {
    _degree++;
}

const bool AutoLayoutNodeBase::isLocked() {
    return _locked;
}

void AutoLayoutNodeBase::setLocked(const bool& locked) {
    _locked = locked;
}

void AutoLayoutNodeBase::updateLockedStatus() {;
    if (LIBSBMLNETWORK_CPP_NAMESPACE::getUserData(_graphicalObject, "locked") == "true") {
        setLocked(true);
        setX(std::stod(LIBSBMLNETWORK_CPP_NAMESPACE::getUserData(_graphicalObject, "x")));
        setY(std::stod(LIBSBMLNETWORK_CPP_NAMESPACE::getUserData(_graphicalObject, "y")));
    }
}

// AutoLayoutNode

AutoLayoutNode::AutoLayoutNode(Model* model, Layout* layout, GraphicalObject* graphicalObject, const bool& useNameAsTextLabel, const bool& locked) : AutoLayoutNodeBase(model, layout, graphicalObject, useNameAsTextLabel, locked) {

}

const std::string AutoLayoutNode::getId() {
    return _graphicalObject->getId();
}

GraphicalObject* AutoLayoutNode::getGraphicalObject() {
    return _graphicalObject;
}

void AutoLayoutNode::updateDimensions() {
    std::string fixedWidth = LIBSBMLNETWORK_CPP_NAMESPACE::getUserData(getGraphicalObject(), "width");
    if (fixedWidth.empty())
        setWidth(std::max(calculateWidth(), std::max(getWidth(), getDefaultWidth())));
    else
        setWidth(std::stod(fixedWidth));
    std::string fixedHeight = LIBSBMLNETWORK_CPP_NAMESPACE::getUserData(getGraphicalObject(), "height");
    if (fixedHeight.empty())
        setHeight(std::max(calculateHeight(), std::max(getHeight(), getDefaultHeight())));
    else
        setHeight(std::stod(fixedHeight));
}

const double AutoLayoutNode::getX() {
    return _graphicalObject->getBoundingBox()->x();
}

void AutoLayoutNode::setX(const double& x) {
    _graphicalObject->getBoundingBox()->setX(x);
}

const double AutoLayoutNode::getY() {
    return _graphicalObject->getBoundingBox()->y();
}

void AutoLayoutNode::setY(const double& y) {
    _graphicalObject->getBoundingBox()->setY(y);
}

const double AutoLayoutNode::getWidth() {
    return _graphicalObject->getBoundingBox()->width();
}

const double AutoLayoutNode::getDefaultWidth() {
    return 60.0;
}

void AutoLayoutNode::setWidth(const double& width) {
    _graphicalObject->getBoundingBox()->setWidth(width);
}

const double AutoLayoutNode::getHeight() {
    return _graphicalObject->getBoundingBox()->height();
}

const double AutoLayoutNode::getDefaultHeight() {
    return 36.0;
}

void AutoLayoutNode::setHeight(const double& height) {
    _graphicalObject->getBoundingBox()->setHeight(height);
}

const double AutoLayoutNode::calculateWidth() {
    SpeciesGlyph* speciesGlyph = (SpeciesGlyph*)_graphicalObject;
    std::string displayedText = speciesGlyph->getSpeciesId();
    Species *species = LIBSBMLNETWORK_CPP_NAMESPACE::findSpeciesGlyphSpecies(_model, speciesGlyph);
    if (species && species->isSetName() && _useNameAsTextLabel)
        displayedText = species->getName();

    return std::max(60.0, displayedText.size() * 15.0);
}

const double AutoLayoutNode::calculateHeight() {
    return std::max(36.0, getHeight());
}

// AutoLayoutCentroidNode


AutoLayoutCentroidNode::AutoLayoutCentroidNode(Model* model, Layout* layout, GraphicalObject* graphicalObject, const bool& useNameAsTextLabel, const bool& locked) : AutoLayoutNodeBase(model, layout, graphicalObject, useNameAsTextLabel, locked) {

}

const std::string AutoLayoutCentroidNode::getId() {
    return _graphicalObject->getId();
}

GraphicalObject* AutoLayoutCentroidNode::getGraphicalObject() {
    return _graphicalObject;
}

void AutoLayoutCentroidNode::updateDimensions() {
    std::string fixedWidth = LIBSBMLNETWORK_CPP_NAMESPACE::getUserData(getGraphicalObject(), "width");
    if (fixedWidth.empty())
        setBoundingBoxWidth(std::max(calculateWidth(), getWidth()));
    else {
        setWidth(std::stod(fixedWidth));
        setBoundingBoxWidth(std::stod(fixedWidth));
    }
    std::string fixedHeight = LIBSBMLNETWORK_CPP_NAMESPACE::getUserData(getGraphicalObject(), "height");
    if (fixedHeight.empty())
        setBoundingBoxHeight(std::max(calculateHeight(), getHeight()));
    else {
        setHeight(std::stod(fixedHeight));
        setBoundingBoxHeight(std::stod(fixedHeight));
    }
}

const double AutoLayoutCentroidNode::getX() {
    return 0.5 * (getCurve()->getCurveSegment(0)->getStart()->x() + getCurve()->getCurveSegment(0)->getEnd()->x());
}

void AutoLayoutCentroidNode::setX(const double& x) {
    getCurve()->getCurveSegment(0)->getStart()->setX(x);
    ((CubicBezier*)(getCurve()->getCurveSegment(0)))->getBasePoint1()->setX(x);
    getCurve()->getCurveSegment(0)->getEnd()->setX(x);
    ((CubicBezier*)(getCurve()->getCurveSegment(0)))->getBasePoint2()->setX(x);
    _graphicalObject->getBoundingBox()->setX(x);
}

const double AutoLayoutCentroidNode::getY() {
    return 0.5 * (getCurve()->getCurveSegment(0)->getStart()->y() + getCurve()->getCurveSegment(0)->getEnd()->y());
}

void AutoLayoutCentroidNode::setY(const double& y) {
    getCurve()->getCurveSegment(0)->getStart()->setY(y);
    ((CubicBezier*)(getCurve()->getCurveSegment(0)))->getBasePoint1()->setY(y);
    getCurve()->getCurveSegment(0)->getEnd()->setY(y);
    ((CubicBezier*)(getCurve()->getCurveSegment(0)))->getBasePoint2()->setY(y);
    _graphicalObject->getBoundingBox()->setY(y);
}

const double AutoLayoutCentroidNode::getWidth() {
    return getCurve()->getCurveSegment(0)->getEnd()->x() - getCurve()->getCurveSegment(0)->getStart()->x();
}

const double AutoLayoutCentroidNode::getDefaultWidth() {
    return getWidth();
}

void AutoLayoutCentroidNode::setWidth(const double& width) {
    if (std::abs(width - getWidth())) {
        getCurve()->getCurveSegment(0)->getStart()->setX(getCurve()->getCurveSegment(0)->getStart()->x() - 0.5 * std::abs(width - getWidth()));
        ((CubicBezier*)(getCurve()->getCurveSegment(0)))->getBasePoint1()->setX(((CubicBezier*)(getCurve()->getCurveSegment(0)))->getBasePoint1()->x() - 0.5 * std::abs(width - getWidth()));
        getCurve()->getCurveSegment(0)->getEnd()->setX(getCurve()->getCurveSegment(0)->getEnd()->x() + 0.5 * std::abs(width - getWidth()));
        ((CubicBezier*)(getCurve()->getCurveSegment(0)))->getBasePoint2()->setX(((CubicBezier*)(getCurve()->getCurveSegment(0)))->getBasePoint2()->x() - 0.5 * std::abs(width - getWidth()));
    }
}

void AutoLayoutCentroidNode::setBoundingBoxWidth(const double& width) {
    _graphicalObject->getBoundingBox()->setWidth(width);
}

const double AutoLayoutCentroidNode::getHeight() {
    return getCurve()->getCurveSegment(0)->getEnd()->y() - getCurve()->getCurveSegment(0)->getStart()->y();
}

const double AutoLayoutCentroidNode::getDefaultHeight() {
    return getHeight();
}

void AutoLayoutCentroidNode::setHeight(const double& height) {
    if (std::abs(height - getHeight())) {
        getCurve()->getCurveSegment(0)->getStart()->setY(getCurve()->getCurveSegment(0)->getStart()->y() - 0.5 * std::abs(height - getHeight()));
        ((CubicBezier*)(getCurve()->getCurveSegment(0)))->getBasePoint1()->setY(((CubicBezier*)(getCurve()->getCurveSegment(0)))->getBasePoint1()->y() - 0.5 * std::abs(height - getHeight()));
        getCurve()->getCurveSegment(0)->getEnd()->setY(getCurve()->getCurveSegment(0)->getEnd()->y() + 0.5 * std::abs(height - getHeight()));
        ((CubicBezier*)(getCurve()->getCurveSegment(0)))->getBasePoint2()->setY(((CubicBezier*)(getCurve()->getCurveSegment(0)))->getBasePoint2()->y() - 0.5 * std::abs(height - getHeight()));
    }
}

void AutoLayoutCentroidNode::setBoundingBoxHeight(const double& height) {
    _graphicalObject->getBoundingBox()->setHeight(height);
}

const double AutoLayoutCentroidNode::calculateWidth() {
    ReactionGlyph* reactionGlyph = (ReactionGlyph*)_graphicalObject;
    std::string displayedText = reactionGlyph->getReactionId();
    Reaction *reaction = LIBSBMLNETWORK_CPP_NAMESPACE::findReactionGlyphReaction(_model, reactionGlyph);
    if (reaction && reaction->isSetName() && _useNameAsTextLabel)
        displayedText = reaction->getName();

    return std::max(30.0, displayedText.size() * 9.0);
}

const double AutoLayoutCentroidNode::calculateHeight() {
    return std::max(20.0, getHeight());
}

Curve* AutoLayoutCentroidNode::getCurve() {
    ReactionGlyph* reactionGlyph = (ReactionGlyph*)_graphicalObject;
    return reactionGlyph->getCurve();
}
