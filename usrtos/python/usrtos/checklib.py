import sys
sys.path.append('work')

import usrtos

# a = usrtos.UsrtMem(sys.argv[1])

# a.dump()

# a.dumpHead();

# aGp = a.newGP()

# print(aGp.offset,aGp.objsize)

# a.dump()

# bp = a.GPandLP(aGp)

# print(aGp.offset,aGp.objsize)

# a.dump()

os = usrtos.UsrtScript(sys.argv[1],256)
os.moveal(0,1)
os.selfrd(0)
os.selfro(0,usrtos.AnyType(1000000))
tid = usrtos.UUID("8ea09e05-fd67-5949-a9ab-e722a3dae01c")
os.immeid(0,tid)
os.ret()

os.test()
