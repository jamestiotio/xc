# -*- coding: utf-8 -*-

from __future__ import division
from __future__ import print_function

import geom
import xc
from model import predefined_spaces
from materials import typical_materials
import math

__author__= "Luis C. Pérez Tato (LCPT)"
__copyright__= "Copyright 2014, LCPT"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@gmail.com"

NumDiv= 25
CooMax= 10.0

feProblem= xc.FEProblem()
preprocessor=  feProblem.getPreprocessor
nodes= preprocessor.getNodeHandler

# Problem type
modelSpace= predefined_spaces.SolidMechanics3D(nodes)

# Material
elast= typical_materials.defElasticMaterial(preprocessor, "elast",3000)


# Problem geometry
points= preprocessor.getMultiBlockTopology.getPoints
pt1= points.newPoint(geom.Pos3d(0,0,0))
pt2= points.newPoint(geom.Pos3d(CooMax,CooMax,CooMax))

lines= preprocessor.getMultiBlockTopology.getLines
l= lines.newLine(pt1.tag,pt2.tag)
l.nDiv= NumDiv

testEqualOperator= (l==l)

# Mesh generation
seedElemHandler= preprocessor.getElementHandler.seedElemHandler
seedElemHandler.dimElem= 3 # Bars defined in a three dimensional space.
seedElemHandler.defaultMaterial= elast.name
truss= seedElemHandler.newElement("Truss",xc.ID([0,0]))
truss.sectionArea= 10

setTotal= preprocessor.getSets.getSet("total")
setTotal.genMesh(xc.meshDir.I)

mesh= feProblem.getDomain.getMesh
nnod= mesh.getNumNodes()
nelem= mesh.getNumElements()

# for n in l.nodes:
#   print(n.tag)
# for e in l.elements:
#   print(e.tag)
# nIter= mesh.getNodeIter
# nod= nIter.next()
# while not(nod is None):
#   print("tag= ", nod.tag, " x= ", nod.getCoo[0], " y= ", nod.getCoo[1], " z= ", nod.getCoo[2])
#   nod= nIter.next()

vteor2= math.pow(CooMax/NumDiv,2)
lteor= math.sqrt(3*vteor2)

testOK= True
eIter= mesh.getElementIter
elem= eIter.next()
while not(elem is None):
#   print(elem.tag," nod. I: ",elem.getNodes[0].tag," nod. J: ",elem.getNodes[1].tag," L= ",elem.getLength(True))
    ratio1= (lteor - elem.getLength(True))/lteor
    testOK= testOK and (ratio1<1e-12)
    elem= eIter.next()

#print("number of nodes: ",nnod)
#print("number of elements: ",nelem)

import os
from misc_utils import log_messages as lmsg
fname= os.path.basename(__file__)
if (testOK and testEqualOperator):
    print("test "+fname+": ok.")
else:
    lmsg.error(fname+' ERROR.')


#Graphic stuff.
# from postprocess import output_handler
# oh= output_handler.OutputHandler(modelSpace)

# oh.displayBlocks()#setToDisplay= )
# oh.displayFEMesh()#setsToDisplay=[])
# oh.displayLocalAxes()


