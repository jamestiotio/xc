# -*- coding: utf-8 -*-
''' Verification test of a rectangular fiber section with an
    elasto plastic material model.
    written from "Nociones de cálculo plástico." Carlos Benito Hernández.
    page 26 and following.
    url={https://books.google.es/books?id=v7bbMwEACAAJ},
'''
from __future__ import print_function

__author__= "Luis C. Pérez Tato (LCPT)"
__copyright__= "Copyright 2014, LCPT"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@gmail.com"

import os
import sys
import xc
from materials.sections import section_properties
from misc import scc3d_testing_bench
from materials.ehe import EHE_materials
from materials.ehe import EHE_limit_state_checking
from solution import predefined_solutions
from model import predefined_spaces
from materials import typical_materials

# Rectangular cross-section definition
b= 10 # Cross section width  [cm]
h= 20 # Cross section depth [cm]
scc10x20= section_properties.RectangularSection('scc10x20',b,h)
scc10x20.nDivIJ= 32 # number of cells in IJ direction  
scc10x20.nDivJK= 32 # number of cells in JK direction

pth= os.path.dirname(__file__)
if(not pth):
  pth= "."
sys.path.append(pth+"/../../../../../../aux/")
import fiber_section_test_macros

fy= 2600 # Yield stress of the material expressed in kp/cm2.
E= 2.1e6 # Young’s modulus of the material (kp/cm2).

feProblem= xc.FEProblem()
feProblem.logFileName= "/tmp/erase.log" # Ignore warning messages
preprocessor=  feProblem.getPreprocessor
# Materials definition
epp= typical_materials.defElasticPPMaterial(preprocessor, "epp",E,fy,-fy) # elastic perfectly-plastic uniaxial material
respT= typical_materials.defElasticMaterial(preprocessor, "respT",1e10) # Torsion response.
respVy= typical_materials.defElasticMaterial(preprocessor, "respVy",1e6) # Shear response in y direction.
respVz= typical_materials.defElasticMaterial(preprocessor, "respVz",1e3) # Shear response in y direction.
# Section geometry
# creation
geomRectang= preprocessor.getMaterialHandler.newSectionGeometry("geomRectang")
# generation of a quadrilateral region of the scc10x20 sizes and number of
# divisions made of material nmbMat
reg= scc10x20.getRegion(gm=geomRectang,nmbMat= epp.name)
sa= preprocessor.getMaterialHandler.newMaterial("fiberSectionShear3d","sa")
fiberSectionRepr= sa.getFiberSectionRepr()
fiberSectionRepr.setGeomNamed(geomRectang.name)
sa.setupFibers()
fiber_section_test_macros.extractFiberSectionProperties(sa,scc10x20, fy)
sa.setRespVyByName(respVy.name)
sa.setRespVzByName(respVz.name)
sa.setRespTByName(respT.name)

zlElement, nodA, nodB= scc3d_testing_bench.sectionModel(preprocessor, sa.name)

# Constraints
modelSpace= predefined_spaces.getStructuralMechanics3DSpace(preprocessor)
modelSpace.fixNode000_000(nodA.tag)

# Loads definition
lp0= modelSpace.newLoadPattern(name= '0')

loadVy= 2e4
loadVz= 3e4
loadMx= 1e3
loadMz= 0.999*scc10x20.getPlasticMomentZ(fy)
lp0.newNodalLoad(nodB.tag,xc.Vector([0,loadVy,loadVz,loadMx,0,loadMz]))

# We add the load case to domain.
modelSpace.addLoadCaseToDomain(lp0.name)


# Solution procedure
analysis= predefined_solutions.plain_newton_raphson(feProblem)
analOk= analysis.analyze(1)
if(analOk!=0):
    print("Error!; failed to converge.")
    exit()



nodes= preprocessor.getNodeHandler
nodes.calculateNodalReactions(True,1e-7)
n1= nodA
RVy= n1.getReaction[1] 
RVz= n1.getReaction[2] 
RMx= n1.getReaction[3] 
RMz= n1.getReaction[5] 

scc= zlElement.getSection()
esfVy= scc.getStressResultantComponent("Vy")
esfVz= scc.getStressResultantComponent("Vz")
esfMx= scc.getStressResultantComponent("T")
esfMz= scc.getStressResultantComponent("Mz")



referenceCenterOfMassY= 0.0
referenceCenterOfMassZ= 0.0
ratio1= (fiber_section_test_macros.sumAreas-scc10x20.A())/scc10x20.A()
ratio2= fiber_section_test_macros.centerOfMassY-referenceCenterOfMassY
ratio3= fiber_section_test_macros.centerOfMassZ-referenceCenterOfMassZ
ratio4= (fiber_section_test_macros.I1-scc10x20.Iz())/scc10x20.Iz()
ratio5= (fiber_section_test_macros.I2-scc10x20.Iy())/scc10x20.Iy()
ratio6= (fiber_section_test_macros.i1-scc10x20.iz())/scc10x20.iz()
ratio7= (fiber_section_test_macros.i2-scc10x20.iy())/scc10x20.iy()
ratio8= (fiber_section_test_macros.Me1-scc10x20.getYieldMomentZ(fy))/scc10x20.getYieldMomentZ(fy)
ratio9= (fiber_section_test_macros.Me2-scc10x20.getYieldMomentY(fy))/scc10x20.getYieldMomentY(fy)
ratio10= (fiber_section_test_macros.SzPosG-scc10x20.getPlasticSectionModulusZ())/scc10x20.getPlasticSectionModulusZ()
ratio11= (fiber_section_test_macros.SyPosG-scc10x20.getPlasticSectionModulusY())/scc10x20.getPlasticSectionModulusY()
ratio12= (RMz+loadMz)/loadMz
ratio13= (esfMz-loadMz)/loadMz

ratio14= (RMx+loadMx)/loadMx
ratio15= (esfMx-loadMx)/loadMx

ratio16= (RVy+loadVy)/loadVy
ratio17= (esfVy-loadVy)/loadVy

ratio18= (RVz+loadVz)/loadVz
ratio19= (esfVz-loadVz)/loadVz

''' 
    print("RVy= ",(RVy))
    print("esfVy= ",(esfVy))
    print("loadVy= ",(loadVy))

    print("RVz= ",(RVz))
    print("esfVz= ",(esfVz))
    print("loadVz= ",(loadVz))

    print("RMx= ",(RMx))
    print("esfMx= ",(esfMx))
    print("loadMx= ",(loadMx))

    print("RMz= ",(RMz))
    print("esfMz= ",(esfMz))
    print("loadMz= ",(loadMz))
   '''

from misc_utils import log_messages as lmsg
fname= os.path.basename(__file__)
if (abs(ratio1)<1e-5) & (abs(ratio2)<1e-5) & (abs(ratio3)<1e-5) & (abs(ratio4)<1e-3) & (abs(ratio5)<1e-2) & (abs(ratio6)<1e-3) & (abs(ratio7)<1e-2) & (abs(ratio8)<1e-3) & (abs(ratio9)<1e-3) & (abs(ratio10)<1e-5) & (abs(ratio11)<1e-5) & (abs(ratio12)<1e-5) & (abs(ratio13)<1e-5) & (abs(ratio14)<1e-5) & (abs(ratio15)<1e-5) & (abs(ratio16)<1e-5) & (abs(ratio17)<1e-5) & (abs(ratio18)<1e-5) & (abs(ratio19)<1e-5) & (analOk == 0.0) :
    print('test '+fname+': ok.')
else:
    lmsg.error(fname+' ERROR.')
