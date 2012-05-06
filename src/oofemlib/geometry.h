/*
 *
 *                 #####    #####   ######  ######  ###   ###
 *               ##   ##  ##   ##  ##      ##      ## ### ##
 *              ##   ##  ##   ##  ####    ####    ##  #  ##
 *             ##   ##  ##   ##  ##      ##      ##     ##
 *            ##   ##  ##   ##  ##      ##      ##     ##
 *            #####    #####   ##      ######  ##     ##
 *
 *
 *             OOFEM : Object Oriented Finite Element Code
 *
 *               Copyright (C) 1993 - 2012   Borek Patzak
 *
 *
 *
 *       Czech Technical University, Faculty of Civil Engineering,
 *   Department of Structural Mechanics, 166 29 Prague, Czech Republic
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef geometry_h
#define geometry_h

#include "domain.h"
#include "flotarry.h"
#include "node.h"

namespace oofem {
/**
 * Abstract representation of Geometry
 * Patch inherits from this class
 * @author chamrova
 */
class BasicGeometry //: public Geometry
{
protected:
    /// List of geometry vertices.
    AList< FloatArray > *vertices;
public:
    /// Constructor.
    BasicGeometry();
    /// Destructor.
    virtual ~BasicGeometry();
    /// computes normal signed distance between this object and a point.
    virtual double computeDistanceTo(FloatArray *point) { return 0; }
    /// checks whether an element is interacted, Element reference will be later replaced by Geometry.
    virtual bool intersects(Element *element) { return false; }
    /// gives number of intersection points of Geometry entity with an element, Element reference will be later replaced by Geometry.
    virtual int computeNumberOfIntersectionPoints(Element *element) { return 0; }
    // gives intersection points between this Geometry and Element.
    virtual void computeIntersectionPoints(Element *element, AList< FloatArray > *intersecPoints) { }
    /// Accessor.
    FloatArray *giveVertex(int n);
    /// Modifier.
    void setVertex(FloatArray *vertex);
    /// Accessor.
    AList< FloatArray > *giveVertices() { return this->vertices; }
    /// Initializes the Geometry from the InputRecord.
    virtual IRResultType initializeFrom(InputRecord *ir) { return IRRT_OK; }
    /// Gives class name.
    virtual const char *giveClassName() const { return NULL; }
    /// Returns number of Geometry vertices.
    int giveNrVertices() { return this->vertices->giveSize(); }
    virtual bool isOutside(BasicGeometry *bg) { return false; }
    virtual void printYourself() { }
#ifdef __OOFEG
    virtual void draw(oofegGraphicContext &gc) { }
#endif
};

class Line : public BasicGeometry
{
public:
    Line() : BasicGeometry() { }
    virtual ~Line() { }
    Line(FloatArray *pointA, FloatArray *pointB);
    /// Computes normal distance to a point
    double computeDistanceTo(FloatArray *point);
    /// Computes tangential distance to a point
    double computeTangentialDistanceToEnd(FloatArray *point);
    void computeProjection(FloatArray &answer);
    int computeNumberOfIntersectionPoints(Element *element);
    void computeIntersectionPoints(Element *element, AList< FloatArray > *intersecPoints);
    double computeInclinationAngle();
    void computeTransformationMatrix(FloatMatrix &answer);
    void transformIntoPolar(FloatArray *point, FloatArray &answer);
    IRResultType initializeFrom(InputRecord *ir);
    bool isPointInside(FloatArray *point);
    bool intersects(Element *element);
    bool isOutside(BasicGeometry *bg);
};

class Triangle : public BasicGeometry
{
public:
    Triangle(FloatArray *p1, FloatArray *p2, FloatArray *p3);
    virtual ~Triangle() { }
    double getArea();
    void computeBarycentrCoor(FloatArray &answer);
    double getRadiusOfCircumCircle();
    void computeCenterOfCircumCircle(FloatArray &answer);
    void printYourself();
    int computeNumberOfIntersectionPoints(Element *element) { return 0; }
    bool isOrientedAnticlockwise();
    void changeToAnticlockwise();
};

class Circle : public BasicGeometry
{
protected:
    double radius;
public:
    Circle() : BasicGeometry() { }
    virtual ~Circle() { }
    Circle(FloatArray *center, double radius);
    /// Computes the normal distance to the surface not to the center.
    double computeDistanceTo(FloatArray *point);
    IRResultType initializeFrom(InputRecord *ir);
    const char *giveClassName() const { return "Circle"; }
    bool intersects(Element *element);
    void computeIntersectionPoints(Element *element, AList< FloatArray > *intersecPoints);
    void computeIntersectionPoints(Line *l, AList< FloatArray > *intersecPoints);
    bool isOutside(BasicGeometry *bg);
    void printYourself();
};
} // end namespace oofem
#endif  // geometry_h




