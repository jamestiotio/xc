# -*- coding: utf-8 -*-
''' Compute the minimum and maximum reiforcement amounts for a reinforced concret column member. Home made test.
'''

from __future__ import division
from __future__ import print_function

__author__= "Luis Claudio Pérez Tato (LCPT)"
__copyright__= "Copyright 2024, LCPT"
__license__= "GPL"
__version__= "3.0"
__email__= "l.pereztato@gmail.com"

import os
import math
from materials.ehe import EHE_materials
from materials.ehe import EHE_limit_state_checking
from materials.sections.fiber_section import def_simple_RC_section
from misc_utils import log_messages as lmsg

concrete= EHE_materials.HA35
steel= EHE_materials.B500S
side= 1.5

## Reinforced concrete section.
section= def_simple_RC_section.RCRectangularSection(name='test', width= side, depth= side, concrType= concrete, reinfSteelType= steel)
## Compute minimum and maximum reinforcement areas.
asMinGeomBeam, asMinMechBeam, asMaxBeam= EHE_limit_state_checking.get_min_and_max_reinforcement(memberType='beam', rcSection= section, Nd= 0.0)

# Check results.
ratio1= abs(asMinGeomBeam-63e-4)/63e-4
ratio2= abs(asMinMechBeam-103.82222272358034e-4)/103.82222272358034e-4

'''
print('Minimum amount of reinforcement (geometric ratio): As= ', asMinGeomBeam*1e4, 'cm2')
print("ratio1= ", ratio1)

print('Minimum amount of reinforcement (mechanical ratio): As= ', asMinMechBeam*1e4, 'cm2')
print("ratio2= ", ratio2)
'''

import os
fname= os.path.basename(__file__)
if ((abs(ratio1)<1e-4) and (abs(ratio2)<1e-4)):
    print('test '+fname+': ok.')
else:
    lmsg.error(fname+' ERROR.')
