#!/usr/bin/env python
# -*- coding=utf-8 -*-
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

# 网格
x = np.linspace(-3, 3, 600)
y = np.linspace(-3, 3, 600)
X, Y = np.meshgrid(x, y)

# 极坐标
r = np.sqrt(X**2 + Y**2)
theta = np.arctan2(Y, X)

# 星芒函数
Z = np.cos(5 * theta) * (r**2) * np.exp(-r**2 / 10)

# 绘图
fig = plt.figure(figsize=(10, 8))
ax = fig.add_subplot(111, projection='3d')
surf = ax.plot_surface(X, Y, Z, cmap='turbo', linewidth=0, antialiased=True, alpha=0.95)

ax.view_init(elev=35, azim=45)
ax.set_box_aspect([1, 1, 0.6])
ax.axis('off')
fig.colorbar(surf, shrink=0.5, aspect=20)
plt.title(r'$z = \cos(5\theta) \cdot r^{2} \cdot e^{-r^{2}/10}$', fontsize=16, pad=20)
plt.show()
