#!/usr/bin/env python
# -*- coding=utf-8 -*-
import numpy as np

def star_polygon(spikes=5, outer_radius=1.0, inner_radius=0.4, rotation=0.0, close=True):
    """
    返回棱角分明的星形顶点 (x, y)。
    spikes: 尖角数量
    outer_radius: 尖角处半径
    inner_radius: 凹陷处半径
    rotation: 整体旋转（弧度）
    """
    pts = 2 * spikes
    angles = np.linspace(0, 2*np.pi, pts, endpoint=False) + rotation
    radii = np.empty(pts)
    radii[::2] = outer_radius
    radii[1::2] = inner_radius
    x = radii * np.cos(angles)
    y = radii * np.sin(angles)
    if close:
        x = np.append(x, x[0])
        y = np.append(y, y[0])
    return x, y

if __name__ == '__main__':
    star_polygon();
