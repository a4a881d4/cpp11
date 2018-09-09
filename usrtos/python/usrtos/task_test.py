import sys
sys.path.append('work')

from usrtos import UsrtTaskAPI as api

a = api("/tmp/usrtos")
a.HelloWorld()
