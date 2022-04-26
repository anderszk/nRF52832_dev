# -*- coding: utf-8 -*-
"""
Created on Thu Apr 21 17:47:52 2022

@author: filip
"""

import numpy as np
import matplotlib.pyplot as plt

dataFile = np.loadtxt('data_file.txt', delimiter=',')

delta = dataFile[:, 0]
sigma = dataFile[:, 1]

plt.xlabel("Angle")
plt.ylabel("dBi")
plt.figure()
plt.subplot(211)
plt.plot(delta)

plt.subplot(212)
plt.plot(sigma)
plt.show()