import sys
sys.path.append('work')

import usrtos

a = usrtos.UsrtMem(sys.argv[1])

a.dump()

a.dumpHead();

aGp = a.newGP()

print(aGp.offset,aGp.objsize)

a.dump()

bp = a.GPandLP(aGp)

print(aGp.offset,aGp.objsize)

a.dump()

