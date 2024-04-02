#ifndef __LIBSBML_NE_FRUCHTERMAN_REINGOLD_ALGORITHM_H
#define __LIBSBML_NE_FRUCHTERMAN_REINGOLD_ALGORITHM_H

#ifndef SWIG
#include "sbml/SBMLTypes.h"
#include "sbml/packages/layout/common/LayoutExtensionTypes.h"
#endif

#include "libsbml_ne_autolayout_object_base.h"
#include "libsbml_ne_autolayout_point.h"

namespace LIBSBML_NETWORKEDITOR_CPP_NAMESPACE {

class FruthtermanReingoldAlgorithm {

public:

    FruthtermanReingoldAlgorithm();

    void setElements(Model* model, Layout* layout);

    void setStiffness(const double& stiffness);

    void setGravity(const double& gravity);

    void setUseMagnetism(const bool& useMagnetism);

    void setUseBoundary(const bool& useBoundary);

    void setUseGrid(const bool& useGrid);

    void setNodesLockedStatus(Layout *layout, const std::vector<std::string>& lockedNodeIds);

    void setPadding(const double& padding);

    void apply();

    void initialize();

    void iterate();

    void clearDisplacements();

    void computeRepulsiveForces();

    void computeAttractiveForces();

    void applyMagnetism();

    void applyGravity();

    void adjustCoordinates();

    void adjustNodeCoordinates(AutoLayoutObjectBase* node);

    void adjustWithinTheBoundary(AutoLayoutObjectBase* node);

    void adjustOnTheGrids(AutoLayoutObjectBase* node);

    void updateNodesDimensions();

    void adjustCoordinateOrigin();

    void updateConnectionsControlPoints();

    void updateConnectionControlPoints(AutoLayoutObjectBase* connection);

    void calculateCenterControlPoint(AutoLayoutObjectBase* connection);

    void adjustCenterControlPoint(AutoLayoutObjectBase* connection);

    void setCurvePoints(AutoLayoutObjectBase* connection);

    void adjustCurvePoints(AutoLayoutObjectBase* connection);

    void adjustUniUniConnections(AutoLayoutObjectBase* connection);

    void setUniUniConnectionCurvePoints(AutoLayoutObjectBase* connection, const AutoLayoutPoint& nodesCenter, const double& slope);

protected:

    void setNodes(Model* model, Layout* layout);

    void setConnections(Model* model, Layout* layout);

    void setNodesDegrees();

    double _stiffness;
    double _gravity;
    bool _useMagnetism;
    bool _useBoundary;
    bool _useGrid;
    double _padding;

    std::vector<AutoLayoutObjectBase*> _connections;
    std::vector<AutoLayoutObjectBase*> _nodes;

    int _maximumIterations;
    double _initialTemperature;
    double _currentTemperature;
    double _time;
    double _alpha;
    double _timeIncrement;
    double _width;
    double _height;
    AutoLayoutPoint _barycenter;
    AutoLayoutPoint _centerControlPoint;
};

const double calculateEuclideanDistance(AutoLayoutPoint point1, AutoLayoutPoint point2);

const double calculateEuclideanDistance(AutoLayoutPoint point);

const double calculateEuclideanDistance(const double& distanceX, const double& distanceY);

const double calculateStiffnessAdjustmentFactor(AutoLayoutObjectBase* vNode, AutoLayoutObjectBase* uNode);

const double calculateRepulsionForce(const double& stiffness, const double& distance);

const double calculateAttractionForce(const double& stiffness, const double& distance);

AutoLayoutPoint adjustPointPosition(AutoLayoutPoint firstPoint, AutoLayoutPoint secondPoint, double degreesFactor, double distanceFactor, bool isRelativeDistance);

AutoLayoutPoint calculateCurveNodeSidePoint(AutoLayoutPoint source, AutoLayoutObjectBase* target, double distance);

AutoLayoutPoint calculateLeftSideIntersectionPoint(AutoLayoutPoint source, AutoLayoutObjectBase* target);

AutoLayoutPoint calculateRightSideIntersectionPoint(AutoLayoutPoint source, AutoLayoutObjectBase* target);

AutoLayoutPoint calculateTopSideIntersectionPoint(AutoLayoutPoint source, AutoLayoutObjectBase* target);

AutoLayoutPoint calculateBottomSideIntersectionPoint(AutoLayoutPoint source, AutoLayoutObjectBase* target);

AutoLayoutPoint calculateIntersectionPoint(AutoLayoutPoint p1, AutoLayoutPoint p2, AutoLayoutPoint q1, AutoLayoutPoint q2);

AutoLayoutPoint calculateCenterWardIntersectionPoint(AutoLayoutPoint source, AutoLayoutObjectBase* target, double distance);

AutoLayoutObjectBase* findObject(std::vector<AutoLayoutObjectBase*> objects, const std::string& objectId);

const int numOfConnectionsBetweenTheSameNodes(std::vector<AutoLayoutObjectBase*> connections, std::vector<std::string> connectionNodeIds);

const int indexOfConnectionBetweenTheSameNodes(std::vector<AutoLayoutObjectBase*> connections, std::vector<std::string> connectionNodeIds, AutoLayoutObjectBase* connection);

const double getNodePairSlope(std::vector<AutoLayoutObjectBase*> nodes, const std::string &node1Id, const std::string &node2Id);

const double getNodePairSlope(std::vector<AutoLayoutObjectBase*> nodes, AutoLayoutObjectBase* node1, AutoLayoutObjectBase* node2);

const double getPointPairSlope(AutoLayoutPoint point1, AutoLayoutPoint point2);

const double getConnectionCenterPadding(std::vector<AutoLayoutObjectBase*> connections, AutoLayoutObjectBase* connection);

AutoLayoutPoint getNodesCenter(std::vector<AutoLayoutObjectBase*> nodes, std::vector<std::string> nodeIds);

const bool compare(std::vector<std::string> strings1, std::vector<std::string> strings2);

const bool whetherGraphicalObjectIsLocked(Layout* layout, GraphicalObject* graphicalObject, const std::vector<std::string>& lockedNodeIds);

}


#endif