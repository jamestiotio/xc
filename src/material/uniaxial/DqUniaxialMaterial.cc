//----------------------------------------------------------------------------
//  XC program; finite element analysis code
//  for structural analysis and design.
//
//  Copyright (C)  Luis C. Pérez Tato
//
//  This program derives from OpenSees <http://opensees.berkeley.edu>
//  developed by the  «Pacific earthquake engineering research center».
//
//  Except for the restrictions that may arise from the copyright
//  of the original program (see copyright_opensees.txt)
//  XC is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or 
//  (at your option) any later version.
//
//  This software is distributed in the hope that it will be useful, but 
//  WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details. 
//
//
// You should have received a copy of the GNU General Public License 
// along with this program.
// If not, see <http://www.gnu.org/licenses/>.
//----------------------------------------------------------------------------
//DqUniaxialMaterial.cc

#include "DqUniaxialMaterial.h"
#include "utility/matrix/Vector.h"
#include "utility/matrix/Matrix.h"

#include "utility/matrix/ID.h"
#include "utility/actor/actor/MovableVector.h"
#include "utility/actor/actor/MovableID.h"

//! @brief Copy the list being passed as parameter.
void XC::DqUniaxialMaterial::copy_list(const DqUniaxialMaterial &other,SectionForceDeformation *s)
  {
    free_mem();
    const size_t numMats= other.size();
    if(numMats>0)
      {
        resize(numMats);
        for( size_t i= 0;i<numMats;i++)
          {
            if(!other[i])
              {
	        std::cerr << getClassName() << "::" << __FUNCTION__
			  << "; null uniaxial material pointer.\n";
                return;
              }
            if(s)
              (*this)[i] = other[i]->getCopy(s);
            else
              (*this)[i]= other[i]->getCopy();
            if(!(*this)[i])
              {
	        std::cerr << getClassName() << "::" << __FUNCTION__
		          << "; failed to copy uniaxial material\n";
	        exit(-1);
              }
          }
      }
    else
      erase(begin(),end());
  }

void XC::DqUniaxialMaterial::free_mem(void)
  {
    for(iterator i= begin();i!=end();i++)
      if(*i)
        {
          delete *i;
          (*i)= nullptr;
        }
  }

void XC::DqUniaxialMaterial::resize(const size_t &n)
  {
    free_mem();
    lst_ptr::resize(n,nullptr);
  }

//! @brief Constructor.
XC::DqUniaxialMaterial::DqUniaxialMaterial(CommandEntity *owner,const size_t &sz)
  : CommandEntity(owner), MovableObject(0), lst_ptr(sz) 
  {
    for(size_t i=0;i<sz;i++)
      lst_ptr::operator[](i)= nullptr;
  }

//! @brief Constructor.
XC::DqUniaxialMaterial::DqUniaxialMaterial(CommandEntity *owner,const UniaxialMaterial &um)
  : CommandEntity(owner), MovableObject(0), lst_ptr()
  { push_back(&um); }

//! @brief Constructor.
XC::DqUniaxialMaterial::DqUniaxialMaterial(CommandEntity *owner,const UniaxialMaterial &um,const size_t &sz)
  : CommandEntity(owner), MovableObject(0), lst_ptr(sz)
  {
    for(size_t i=0;i<sz;i++)
      lst_ptr::operator[](i)= um.getCopy();
  }

//! @brief Constructor.
XC::DqUniaxialMaterial::DqUniaxialMaterial(CommandEntity *owner,const UniaxialMaterial *um,const size_t &sz)
  : CommandEntity(owner), MovableObject(0), lst_ptr(sz)
  {
    if(um)
      {
        for(size_t i=0;i<sz;i++)
          lst_ptr::operator[](i)= um->getCopy();
      }
  }


//! @brief Copy constructor.
XC::DqUniaxialMaterial::DqUniaxialMaterial(const DqUniaxialMaterial &other)
  : CommandEntity(other), MovableObject(other), lst_ptr()
  { copy_list(other); }

//! @brief Copy constructor.
XC::DqUniaxialMaterial::DqUniaxialMaterial(const DqUniaxialMaterial &other,SectionForceDeformation *s)
  : CommandEntity(other), MovableObject(0), lst_ptr()
  { copy_list(other,s); }

//! @brief Assignment operator.
XC::DqUniaxialMaterial &XC::DqUniaxialMaterial::operator=(const DqUniaxialMaterial &other)
  {
    CommandEntity::operator=(other);
    MovableObject::operator=(other);
    copy_list(other);
    return *this;
  }

//! @brief Destructor.
XC::DqUniaxialMaterial::~DqUniaxialMaterial(void)
  { clearAll(); }

//! @brief Clears the container.
void XC::DqUniaxialMaterial::clear(void)
  {
    free_mem();
    lst_ptr::clear();
  }

//! @brief Clears the container and deletes object properties.
void XC::DqUniaxialMaterial::clearAll(void)
  {
    clear();
    CommandEntity::clearPyProps();
  }

//! @brief Commit materials state (normally when convergence is achieved).
int XC::DqUniaxialMaterial::commitState(void)
  {
    int err = 0;
    for( iterator i= begin();i!=end(); i++)
      {
        int tmp= (*i)->commitState();
        if(tmp!=0)
          {
	    std::cerr << getClassName() << "::" << __FUNCTION__
		      << "; MaterialModel failed to commitState():"
	              << std::endl;
	    (*i)->Print(std::cerr);
	  }
        err+= tmp;
      }
    return err;
  }

//! @brief Returns to the last committed state.
int XC::DqUniaxialMaterial::revertToLastCommit(void)
  {
    int err = 0;
    for( iterator i= begin();i!=end(); i++)
      {
        int tmp= (*i)->revertToLastCommit();
        if(tmp!=0)
          {
	    std::cerr << getClassName() << "::" << __FUNCTION__
		      << "; MaterialModel failed to revertToLastCommit():"
	              << std::endl;
	    (*i)->Print(std::cerr);
	  }
        err+= tmp;
      }
    return err;
  }

//! @brief Returns the materials to its initial state.
int XC::DqUniaxialMaterial::revertToStart(void)
  {
    int err = 0;
    for( iterator i= begin();i!=end(); i++)
      {
        int tmp= (*i)->revertToStart();
        if(tmp!=0)
          {
	    std::cerr << getClassName() << "::" << __FUNCTION__
		      << "; MaterialModel failed to revertToStart():"
	              << std::endl;
	    (*i)->Print(std::cerr);
	  }
        err+= tmp;
      }
    return err;
  }

//! @brief Sets initial strains.
//! @param strains: initial strains.
//! @param offset: offset for the strains index.
int XC::DqUniaxialMaterial::setInitialStrain(const Vector &strains,const size_t &offset)
  {
    int err= 0;
    size_t j= offset;
    assert(static_cast<size_t>(strains.Size()) >= (size()+offset));
    for( iterator i= begin();i!=end(); i++,j++)
      err+= (*i)->setInitialStrain(strains(j));
    return err;
  }

//! @brief Increment initial strains.
//! @param strains: initial strains.
//! @param offset: offset for the strains index.
int XC::DqUniaxialMaterial::incrementInitialStrain(const Vector &strainInc,const size_t &offset)
  {
    int err= 0;
    size_t j= offset;
    assert(static_cast<size_t>(strains.Size()) >= (size()+offset));
    for( iterator i= begin();i!=end(); i++,j++)
      err+= (*i)->incrementInitialStrain(strainInc(j));
    return err;
  }

//! @brief Zeroes initial strains.
void XC::DqUniaxialMaterial::zeroInitialStrain(void)
  {
    for( iterator i= begin();i!=end(); i++)
      (*i)->zeroInitialStrain();
  }


//! @brief Sets trial strain.
int XC::DqUniaxialMaterial::setTrialStrain(const Vector &def,const size_t &offset)
  {
    int err= 0;
    size_t j= offset;
    assert(static_cast<size_t>(def.Size()) >= (size()+offset));
    for( iterator i= begin();i!=end(); i++,j++)
      err += (*i)->setTrialStrain(def(j));
    return err;
  }

//! @brief Asigna la trial strain (se emplea en ParallelMaterial).
int XC::DqUniaxialMaterial::setTrialStrain(const double &strain,const double &strainRate)
  {
    int err= 0;
    for( iterator i= begin();i!=end(); i++)
      err += (*i)->setTrialStrain(strain,strainRate);
    return err;
  }


//! @brief Returns the initial strains.
void XC::DqUniaxialMaterial::getInitialStrain(Vector &def,const size_t &offset) const
  {
    size_t j= offset;
    assert(static_cast<size_t>(def.Size()) >= (size()+offset));
    for( const_iterator i= begin();i!=end(); i++,j++)
      def(j)= (*i)->getInitialStrain();
  }

//! @brief Returns strains vector.
void XC::DqUniaxialMaterial::getStrain(Vector &def,const size_t &offset) const
  {
    size_t j= offset;
    assert(static_cast<size_t>(def.Size()) >= (size()+offset));
    for( const_iterator i= begin();i!=end(); i++,j++)
      def(j)= (*i)->getStrain();
  }

//! @brief Return the tangent stiffness matrix.
void XC::DqUniaxialMaterial::getTangent(Matrix &k,const size_t &offset) const
  {
    size_t j= offset;
    assert(static_cast<size_t>(k.noRows()) >= (size()+offset));
    for( const_iterator i= begin();i!=end(); i++,j++)
      k(j,j)= (*i)->getTangent();
  }

//! @brief Return the initial tangent stiffness matrix.
void XC::DqUniaxialMaterial::getInitialTangent(Matrix &k,const size_t &offset) const
  {
    size_t j= offset;
    assert(static_cast<size_t>(k.noRows()) >= (size()+offset));
    for( const_iterator i= begin();i!=end(); i++,j++)
      k(j,j)= (*i)->getInitialTangent();
  }

//! @brief Return the flexibility matrix.
void XC::DqUniaxialMaterial::getFlexibility(Matrix &f,const size_t &offset) const
  {
    size_t j= offset;
    assert(static_cast<size_t>(f.noRows()) >= (size()+offset));
    for( const_iterator i= begin();i!=end(); i++,j++)
      f(j,j)= (*i)->getFlexibility();
  }

//! @brief Return the initial flexibility matrix.
void XC::DqUniaxialMaterial::getInitialFlexibility(Matrix &f,const size_t &offset) const
  {
    size_t j= offset;
    assert(static_cast<size_t>(f.noRows()) >= (size()+offset));
    for( const_iterator i= begin();i!=end(); i++,j++)
      f(j,j)= (*i)->getInitialFlexibility();
  }

//! @brief Returns the tensiones.
void XC::DqUniaxialMaterial::getStress(Vector &s,const size_t &offset) const
  {
    size_t j= offset;
    assert(static_cast<size_t>(s.Size()) >= (size()+offset));
    for( const_iterator i= begin();i!=end(); i++,j++)
      s(j)= (*i)->getStress();
  }

void XC::DqUniaxialMaterial::push_back(const UniaxialMaterial *t,SectionForceDeformation *s)
  {
    if(!t)
      std::cerr << getClassName() << "::" << __FUNCTION__
		<< "; pointer to material is null." << std::endl;
    else
      {
        UniaxialMaterial *tmp= nullptr;
        if(s)
          tmp= t->getCopy(s);
        else
          tmp= t->getCopy();
        if(tmp)
          lst_ptr::push_back(tmp);
        else
          std::cerr << getClassName() << "::" << __FUNCTION__
		    << "; can't create an UniaxialMaterial" << std::endl;
      }
  }

void XC::DqUniaxialMaterial::push_front(const UniaxialMaterial *t,SectionForceDeformation *s)
  {
    UniaxialMaterial *tmp= nullptr;
    if(!t)
      std::cerr << getClassName() << "::" << __FUNCTION__
		<< "; pointer to material is null." << std::endl;
    else
      {
        if(s)
          tmp= t->getCopy(s);
        else
          tmp= t->getCopy();
        if(tmp)
          lst_ptr::push_front(tmp);
        else
          std::cerr << getClassName() << "::" << __FUNCTION__
		    << "; can't create UniaxialMaterial" << std::endl;
      }
  }

//! @brief Returns a vector to store the dbTags
//! of the class members.
XC::DbTagData &XC::DqUniaxialMaterial::getDbTagData(void) const
  {
    static DbTagData retval(2);
    return retval;
  }

//! @brief Sends object through the communicator argument.
int XC::DqUniaxialMaterial::sendData(Communicator &comm)
  {
    const size_t sz= size();
    setDbTagDataPos(0,sz);
    DbTagData cpMat(sz*2);
    int res= 0;
    for(size_t i= 0;i<sz;i++)
      res+= comm.sendBrokedPtr((*this)[i],cpMat,BrokedPtrCommMetaData(i,0,i+sz));
    res+= cpMat.send(getDbTagData(),comm,CommMetaData(1));
    return res;
  }

//! @brief Receives object through the communicator argument.
int XC::DqUniaxialMaterial::recvData(const Communicator &comm)
  {
    const size_t sz= getDbTagDataPos(0);
    DbTagData cpMat(sz*2);
    int res= cpMat.receive(getDbTagData(),comm,CommMetaData(1));

    for(size_t i= 0;i<sz;i++)
      {
        // Receive the material
        (*this)[i]= comm.getBrokedMaterial((*this)[i],cpMat,BrokedPtrCommMetaData(i,0,i+sz));
        if(!(*this)[i])
          std::cerr << getClassName() << "::" << __FUNCTION__
	            << "; material " << i
		    << "failed to recv itself.\n";
      }
    return res;
  }

//! @brief Sends object through the communicator argument.
int XC::DqUniaxialMaterial::sendSelf(Communicator &comm)
  {
    setDbTag(comm);
    const int dataTag= getDbTag();
    int res= sendData(comm);
    const DbTagData &dbTagData= getDbTagData();
    inicComm(dbTagData.Size());
    res+= comm.sendIdData(dbTagData,dataTag);
    if(res < 0)
      std::cerr << getClassName() << "::" << __FUNCTION__
		<< "; " << dataTag << " failed to send.";
    return res;
  }

//! @brief Receives object through the communicator argument.
int XC::DqUniaxialMaterial::recvSelf(const Communicator &comm)
  {
    const int dataTag= getDbTag();
    DbTagData dbTagData= getDbTagData();
    inicComm(dbTagData.Size());
    int res= comm.receiveIdData(dbTagData,dataTag);
    if(res<0)
      std::cerr << getClassName() << "::" << __FUNCTION__
		<< "; " << dataTag << " failed to receive ID\n";
    else
      {
        res+= recvData(comm);
        if(res < 0)
          std::cerr << getClassName() << "::" << __FUNCTION__
	            << "; - failed to receive data\n";
      }
    return res;
  }

//! @brief Print stuff.
void XC::DqUniaxialMaterial::Print(std::ostream &s, int flag) const
  {
    const size_t sz= size();
    for(size_t i= 0;i<sz;i++)
      s << "\t\tUniaxial XC::Material, tag: " << (*this)[i]->getTag() << std::endl;
  }

int XC::DqUniaxialMaterial::setParameter(const std::vector<std::string> &argv, Parameter &param)
  {
    const size_t argc= argv.size();
    const size_t sz= size(); // number of materials.
    
    if(argv[0]=="material")
      {
	if (argc < 3)
	  return -1;
        else
	  {
	    const int materialTag = atoi(argv[1]);
	    for(size_t i = 0; i < sz; i++)
	      {
	        if(materialTag == (*this)[i]->getTag())
		  {
		    std::vector<std::string> newArgs(argv);
		    newArgs.erase(argv.begin()); // remove first element.
		    newArgs.erase(argv.begin()); // remobe second element.
		    (*this)[i]->setParameter(newArgs, param);
		  }
	      }
          }
      }
    else
      { // Send to everything
        for(size_t i = 0; i < sz; i++)
	  (*this)[i]->setParameter(argv, param);
      }
    return 0;
  }

double XC::DqUniaxialMaterial::getStressSensitivity(int gradIndex, bool conditional)
  {
    double dsdh = 0.0;
    const size_t sz= size();
    for(size_t i = 0; i < sz; i++)
      dsdh += (*this)[i]->getStressSensitivity(gradIndex, conditional);

    for(size_t i = 1; i < sz; i++)
      {
        double k0 = (*this)[i]->getInitialTangent();
        //dsdh += k0/(29000*144.0);
      }
    //dsdh -= 0.125;
    return dsdh;
  }

double XC::DqUniaxialMaterial::getTangentSensitivity(int gradIndex)
  {
    double dEdh = 0.0;
    const size_t sz= size();
    for(size_t i = 0; i < sz; i++)
      dEdh += (*this)[i]->getTangentSensitivity(gradIndex);

    return dEdh;
  }

double XC::DqUniaxialMaterial::getInitialTangentSensitivity(int gradIndex)
  {
    double dEdh = 0.0;
    const size_t sz= size();
    for(size_t i = 0; i < sz; i++)
      dEdh += (*this)[i]->getInitialTangentSensitivity(gradIndex);

    return dEdh;
  }

double XC::DqUniaxialMaterial::getDampTangentSensitivity(int gradIndex)
  {
    double dEdh = 0.0;
    const size_t sz= size();
    for(size_t i = 0; i < sz; i++)
      dEdh += (*this)[i]->getDampTangentSensitivity(gradIndex);
    return dEdh;
  }

double XC::DqUniaxialMaterial::getRhoSensitivity(int gradIndex)
  {
    double dpdh = 0.0;
    const size_t sz= size();
    for(size_t i = 0; i < sz; i++)
      dpdh += (*this)[i]->getRhoSensitivity(gradIndex);

    return dpdh;
  }

int XC::DqUniaxialMaterial::commitSensitivity(double dedh, int gradIndex, int numGrads)
  {
    int ok = 0;
    const size_t sz= size();
    for(size_t i= 0;i<sz;i++)
      ok+= (*this)[i]->commitSensitivity(dedh, gradIndex, numGrads);
    return ok;
  }
