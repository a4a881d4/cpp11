import numpy as np
import matplotlib.pyplot as plt

import sys

fn = sys.argv[1]

h = np.fromfile(fn,dtype=np.int64)

x = np.arange(65501)

y = h[:65501]

plt.plot(x,y,color = 'red', linewidth = 1.0, linestyle = '--')

plt.show()
