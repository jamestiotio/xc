// -*-c++-*-
//----------------------------------------------------------------------------
//  xc utils library; general purpose classes and functions.
//
//  Copyright (C)  Luis C. Pérez Tato
//
//  XC utils is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or 
//  (at your option) any later version.
//
//  This software is distributed in the hope that it will be useful, but 
//  WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.  
//
// You should have received a copy of the GNU General Public License 
// along with this program.
// If not, see <http://www.gnu.org/licenses/>.
//----------------------------------------------------------------------------
//Polyline3d.

#ifndef POLYLINE3D_H
#define POLYLINE3D_H

#include "../pos_vec/Pos3d.h"
#include "../lists/PolyPos.h"
#include "Linear3d.h"
#include "Segment3d.h"

class Pos3dList;

//! @ingroup GEOM
//
//! @brief Polyline in a three-dimensional space.
class Polyline3d : public Linear3d, public GeomObj::list_Pos3d
  {
  public:
    Polyline3d(void);
    explicit Polyline3d(const GeomObj::list_Pos3d &);
    explicit Polyline3d(const Pos3dList &);
    explicit Polyline3d(const boost::python::list &);
    
    virtual bool operator==(const Polyline3d &) const;
    virtual GeomObj *getCopy(void) const
      { return new Polyline3d(*this); }
    virtual void Move(const Vector3d &);
    const GeomObj::list_Pos3d &getVertices(void) const;
    const GeomObj::list_Pos3d &getVertexList(void) const;
    boost::python::list getVertexListPy(void) const;
    inline size_t getNumVertices(void) const
      { return GeomObj::list_Pos3d::size(); }
    size_t getNumSegments(void) const;

/*     inline virtual void GiraX(double ang) */
/*       { Ref3d::GiraX(ang); TrfPoints(); } */
/*     inline virtual void GiraY(double ang) */
/*       { Ref3d::GiraY(ang); TrfPoints();  } */
/*     inline virtual void GiraZ(double ang) */
/*       { Ref3d::GiraZ(ang); TrfPoints();  } */
    const Pos3d *appendVertex(const Pos3d &);
    const Pos3d *appendVertexLeft(const Pos3d &);
    virtual bool In(const Pos3d &p, const double &tol= 0.0) const;
    virtual GEOM_FT dist2(const Pos3d &p) const;
    GEOM_FT dist(const Pos3d &p) const;

    virtual GEOM_FT Ix(void) const;
    virtual GEOM_FT Iy(void) const;
    virtual GEOM_FT Iz(void) const;

    GEOM_FT getCurvatureAtVertex(const_iterator) const;
    GEOM_FT getCurvatureAtLength(const GEOM_FT &) const;
    Vector3d getCurvatureVectorAtVertex(const_iterator) const;
    Vector3d getCurvatureVectorAtLength(const GEOM_FT &) const;
    Pos3d getPointAtLength(const GEOM_FT &) const;
    Vector3d getIVectorAtLength(const GEOM_FT &) const;
    Vector3d getJVectorAtLength(const GEOM_FT &) const;
    Vector3d getKVectorAtLength(const GEOM_FT &) const;

    VectorPos3d Divide(int num_partes) const;
    boost::python::list DividePy(int num_partes) const;
    VectorPos3d Divide(const std::vector<double> &) const;
    boost::python::list DividePy(const boost::python::list &) const;
    
    //! @brief Return the length of the object.
    virtual GEOM_FT getLength(void) const
      { return GeomObj::list_Pos3d::getLength(); }
    virtual GEOM_FT GetMax(unsigned short int i) const;
    virtual GEOM_FT GetMin(unsigned short int i) const;
    Polyline3d GetMayores(unsigned short int i,const GEOM_FT &d) const;
    Polyline3d GetMenores(unsigned short int i,const GEOM_FT &d) const;
    
    Segment3d getSegment(const list_Pos3d::const_iterator &) const;
    Segment3d getSegment(const size_t &) const;
    const_iterator getNearestSegment(const Pos3d &) const;
    int getNearestSegmentIndex(const Pos3d &) const;
    Segment3d getNearestLink(const Pos3d &) const;
    
    //list_Pos3d Int(unsigned short int i,const GEOM_FT &d) const;
    list_Pos3d getIntersection(const Plane &) const;
    //Polyline3d Corta(unsigned short int i,const GEOM_FT &d) const;
    void insertVertex(const Pos3d &, const GEOM_FT &tol= 1e-6);
    Polyline3d getChunk(const Pos3d &,const short int &, const GEOM_FT &) const;
    Polyline3d getLeftChunk(const Pos3d &, const GEOM_FT &tol) const;
    Polyline3d getRightChunk(const Pos3d &, const GEOM_FT &tol) const;
    boost::python::list split(const Pos3d &) const;
    Pos3d getCenterOfMass(void) const
      { return GeomObj::list_Pos3d::getCenterOfMass(); }

    iterator getFarthestPointFromSegment(iterator it1, iterator it2, GEOM_FT &pMaxDist);

    void Print(std::ostream &stream) const;
  };

#endif






