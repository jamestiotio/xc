# -*- coding: utf-8 -*-

from __future__ import print_function

__author__= "Luis C. Pérez Tato (LCPT) and Ana Ortega (AO_O)"
__copyright__= "Copyright 2015, LCPT and AO_O"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@ciccp.es ana.ortega@ciccp.es"

import geom
pos1= geom.Pos2d(1,2)

xPt= pos1.x
yPt= pos1.y

#print("pos1.x= ",pos1.x," pos1.y= ",pos1.y)

import os
from misc_utils import log_messages as lmsg
fname= os.path.basename(__file__)
if(xPt==1. and yPt==2.):
    print('test: '+fname+': ok.')
else:
    lmsg.error('test: '+fname+' ERROR.')
 
