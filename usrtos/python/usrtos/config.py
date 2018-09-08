import sys
sys.path.append('work')

import usrtconfig as cfg

a = cfg.UsrtConfig(sys.argv[1])

caps = [
	  "HelloWorld"
	, "DelCapByKey"
	, "DumpQueue"
	, "DumpThread"
	, "Exit"
	, "keeper"
	, "ListCap"
	, "SetCap"
	, "SetCapByKey"
	, "SetDefaultKeeper"
	, "SetKeeper"
	, "Start"
	, "UnlockReady"
	, "UnlockWait"
	]

keys = {}
for cap in caps:
	keys[cap] = a.getKey(cap)

a.byKey(keys["Exit"])
a.byKey(keys["HelloWorld"])

