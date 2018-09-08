# import sys
# sys.path.append('work')

# import usrtos

# a = usrtos.UsrtConfig(sys.argv[1])

# a.HelloWorld()
import sys
sys.path.append('work')

import usrtconfig as cfg

print(cfg.__dict__)

a = cfg.UsrtConfig(sys.argv[1])

a.HelloWorld()