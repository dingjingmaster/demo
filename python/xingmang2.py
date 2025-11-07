#!/usr/bin/env python
# -*- coding=utf-8 -*-

import numpy as np
import matplotlib.pyplot as plt

# 参数
N = 1024                    # 图像分辨率
sigma = 3.0                 # 控制星芒扩散

# 坐标网格（中心原点）
x = np.linspace(-10, 10, N)
y = np.linspace(-10, 10, N)
X, Y = np.meshgrid(x, y)

# 极坐标
r = np.sqrt(X**2 + Y**2)
theta = np.arctan2(Y, X)

# 2D 星芒函数
img = np.abs(np.cos(8 * theta)) * np.exp(-r**2 / (2 * sigma**2))
img = img / img.max()       # 归一化到 [0,1]
img = np.clip(img + 0.05, 0, 1)  # 轻微底噪防纯黑

# 绘图（黑底白芒）
plt.figure(figsize=(6,6), dpi=200)
#plt.imshow(1 - img, cmap='gray', extent=(-10,10,-10,10))
plt.imshow(1 - img, cmap='hot', extent=(-10,10,-10,10))
plt.axis('off')
plt.tight_layout(pad=0)
plt.savefig('starburst_2d_clean.png', bbox_inches='tight', pad_inches=0, dpi=300)
plt.show()
