import sys
sys.path.append('work')

import usrtos

a = usrtos.UsrtMem(sys.argv[1])

a.dump()

a.dumpHead();

