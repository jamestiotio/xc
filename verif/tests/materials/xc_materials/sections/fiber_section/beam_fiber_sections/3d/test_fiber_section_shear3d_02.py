# -*- coding: utf-8 -*-
''' Verification test of a fiber section with shear and torsion
stiffnesses. Home made test.
'''

from __future__ import print_function

__author__= "Luis C. Pérez Tato (LCPT) and Ana Ortega (A_OO)"
__copyright__= "Copyright 2015, LCPT and AO_O"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@gmail.com ana.ortega.ort@gmal.com"

import os
import sys
import xc
from solution import predefined_solutions
from model import predefined_spaces
from materials import typical_materials
from materials.sections import section_properties
from misc import scc3d_testing_bench

# Rectangular cross-section definition
b= 1 # Cross section width [cm]
h= 1 # Cross section depth [cm]
scc1x1= section_properties.RectangularSection('scc1x1',b,h)
scc1x1.nDivIJ= 32 # number of cells in IJ direction  
scc1x1.nDivJK= 32 # number of cells in JK direction

pth= os.path.dirname(__file__)
if(not pth):
  pth= "."
sys.path.append(pth+"/../../../../../../aux/")
import fiber_section_test_macros

fy= 2600 # yield strength [kp/cm2].
E= 1e6   # elastic moculus [kp/cm2].

feProblem= xc.FEProblem()
feProblem.logFileName= "/tmp/erase.log" # Ignore warning messages
preprocessor=  feProblem.getPreprocessor
# Materials definition
elast= typical_materials.defElasticMaterial(preprocessor, "elast",E)
respT= typical_materials.defElasticMaterial(preprocessor, "respT",1e6) # Torsion response.
respVy= typical_materials.defElasticMaterial(preprocessor, "respVy",1e6) # Shear response in y direction.
respVz= typical_materials.defElasticMaterial(preprocessor, "respVz",1e6) # Shear response in y direction.

# Section geometry
# creation
geomRectang= preprocessor.getMaterialHandler.newSectionGeometry("geomRectang")
reg= scc1x1.getRegion(geomRectang,"elast")
sa= preprocessor.getMaterialHandler.newMaterial("fiberSectionShear3d","sa")
fiberSectionRepr= sa.getFiberSectionRepr()
fiberSectionRepr.setGeomNamed(geomRectang.name)
sa.setupFibers()
fiber_section_test_macros.extractFiberSectionProperties(sa,scc1x1, fy)
sa.setRespVyByName("respVy")
sa.setRespVzByName("respVz")
sa.setRespTByName("respT")

zlElement, nodA, nodB= scc3d_testing_bench.sectionModel(preprocessor, "sa")
# Constraints
modelSpace= predefined_spaces.getStructuralMechanics3DSpace(preprocessor)
modelSpace.fixNode000_000(nodA.tag)

# Loads definition
lp0= modelSpace.newLoadPattern(name= '0')

loadN= -1
loadVy= -2
loadVz= -3
loadMx= -4
loadMy= -5
loadMz= -6
lp0.newNodalLoad(nodB.tag,xc.Vector([loadN,loadVy,loadVz,loadMx,loadMy,loadMz]))

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
reacN1= n1.getReaction

scc= zlElement.getSection()
N= scc.getStressResultantComponent("N")
Vy= scc.getStressResultantComponent("Vy")
Vz= scc.getStressResultantComponent("Vz")
Mx= scc.getStressResultantComponent("T")
My= scc.getStressResultantComponent("My")
Mz= scc.getStressResultantComponent("Mz")
esfElem= xc.Vector([N,Vy,Vz,Mx,My,Mz])
defN= scc.getSectionDeformationByName("defN")
defVy= scc.getSectionDeformationByName("defVy")
defVz= scc.getSectionDeformationByName("defVz")
defT= scc.getSectionDeformationByName("defT")
defMy= scc.getSectionDeformationByName("defMy")
defMz= scc.getSectionDeformationByName("defMz")
defElem= xc.Vector([defN,defVy,defVz,defT,defMy,defMz])

respVy= scc.getRespVy()
respVz= scc.getRespVz()
respT= scc.getRespT()
esfRespVy= respVy.getStress()
esfRespVz= respVz.getStress()
esfRespT= respT.getStress()

defRespVy= respVy.getStrain()
defRespVz= respVz.getStrain()
defRespT= respT.getStrain()

fibers= scc.getFibers()

fibersDef= fibers.getDeformation()

sectionInternalForces= xc.Vector([fibers.getResultant(),esfRespVy,esfRespVz,esfRespT,fibers.getMy(0.0),fibers.getMz(0.0)])
sectionDef= xc.Vector([fibersDef[0],defRespVy,defRespVz,defRespT,fibersDef[2],fibersDef[1]])



ratio1= (reacN1+esfElem).Norm()
ratio2= (reacN1+sectionInternalForces).Norm()
ratio3= (defElem-sectionDef).Norm()

'''
print("reacN1= ",reacN1)
print("esfElem= ",esfElem)
print("sectionInternalForces= ",sectionInternalForces)
print("defElem= ",defElem)
print("sectionDef= ",sectionDef)
print("ratio1= ",ratio1)
print("ratio2= ",ratio2)
print("ratio3= ",ratio3)
''' 


import os
from misc_utils import log_messages as lmsg
fname= os.path.basename(__file__)
if((abs(ratio1)<1e-15) & (abs(ratio2)<1e-15) & (abs(ratio3)<1e-12)):
    print('test '+fname+': ok.')
else:
    lmsg.error(fname+' ERROR.')
