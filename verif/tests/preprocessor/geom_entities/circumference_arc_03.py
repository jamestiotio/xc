# -*- coding: utf-8 -*-
from __future__ import print_function

import geom
import xc
import math
import os
from model import predefined_spaces
from materials import typical_materials

__author__= "Luis C. Pérez Tato (LCPT)"
__copyright__= "Copyright 2014, LCPT"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@gmail.com"

NumDiv= 3
R= 2.0
cos45= math.cos(math.radians(45))
sin45= cos45
cooCentroElemTeor= xc.Vector([1.36603,1.36603,0])

# Problem type
feProblem= xc.FEProblem()
preprocessor=  feProblem.getPreprocessor
nodes= preprocessor.getNodeHandler
modelSpace= predefined_spaces.SolidMechanics3D(nodes)

# Materials definition
elast= typical_materials.defElasticMaterial(preprocessor, "elast",3000)


seedElemHandler= preprocessor.getElementHandler.seedElemHandler
seedElemHandler.defaultMaterial= elast.name
seedElemHandler.dimElem= 3 # Dimension of element space
truss= seedElemHandler.newElement("Truss",xc.ID([0,0]))
truss.sectionArea= 10.0

points= preprocessor.getMultiBlockTopology.getPoints
pt1= points.newPoint(geom.Pos3d(R,0.0,0.0))
pt2= points.newPoint(geom.Pos3d((R*cos45),(R*sin45),0.0))
pt3= points.newPoint(geom.Pos3d(0.0,R,0.0))

lines= preprocessor.getMultiBlockTopology.getLines
l= lines.newCircleArc(pt1.tag,pt2.tag,pt3.tag)
l.nDiv= NumDiv
th1= l.getTheta1()
th2= l.getTheta2()
length= l.getLength()
xC= l.getCenter().x
yC= l.getCenter().y
zC= l.getCenter().z
xi= l.getPInic().x
yi= l.getPInic().y
zi= l.getPInic().z
r= l.getRadius()

l.genMesh(xc.meshDir.I)

nnodes= l.getNumNodes

elements= preprocessor.getElementHandler
ele2= elements.getElement(2)
points= ele2.getCooPoints(2) #Two divisions-> Three points.
cooCentroElem= points.getRow(1)



nnodteor= NumDiv+1
ratio1= (nnodteor/nnodes)
ratio2= (cooCentroElem-cooCentroElemTeor).Norm()

''' 
print("nnodes= ",(nnodes))
print("nnodteor= ",(nnodteor))
print("ratio1= ",(ratio1))
print("theta1= ",(math.radians(th1)))
print("theta2= ",(math.radians(th2)))
print("xC= ",(xC))
print("yC= ",(yC))
print("zC= ",(zC))
print("xi= ",(xi))
print("yi= ",(yi))
print("zi= ",(zi))
print("radius= ",(r))
print("points= ",points)
print("cooCentroElem= ",cooCentroElem)
print("cooCentroElemTeor= ",cooCentroElemTeor)
print("ratio2= ",(ratio2))
   '''

import os
from misc_utils import log_messages as lmsg
fname= os.path.basename(__file__)
if (abs(ratio1-1.0)<1e-12) & (abs(ratio2)<1e-5):
    print('test '+fname+': ok.')
else:
    lmsg.error(fname+' ERROR.')
