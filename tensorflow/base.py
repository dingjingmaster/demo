#!/usr/bin/env python3.6
# -*- encoding=utf8 -*-
import tensorflow as tf
import numpy as np
"""
	tensorflow 基本使用
"""


def tensor_test():
	""" 固定张量 """
	zero_tsr = tf.zeros([2, 3])                     # 创建 2行 3列 的零张量
	ones_tsr = tf.ones([2, 3])                      # 创建 2行 3列 的单位张量
	fill_tsr = tf.fill([2, 3], 40)                  # 创建 2行 3列 的填充张量
	const_tsr = tf.constant([1, 2, 3, 4])           # 用已知常数创建一个张量

	""" 相似张量 """
	zero_similar = tf.zeros_like(const_tsr)         # 与 const_tsr 相似的 零 张量
	ones_similar = tf.ones_like(const_tsr)          # 与 ones_similar 相似的 单位 张量

	""" 序列张量 """
	linear_tsr = tf.linspace(start=0.0, stop=1.0, num=3)            # 线性序列张量
	limit_tsr = tf.range(start=0.0, limit=13, delta=3.0)            # delta 是步长 limit 限制最大值(不能超过或等于最大值)

	""" 随机张量 """
	random_tsr = tf.random_uniform([2, 3], minval=0, maxval=1)      # 随机张量
	random_tsr1 = tf.random_normal([2, 3], mean=0.0, stddev=1.0)    # 正态分布的随机数
	random_tsr2 = tf.truncated_normal([2, 3], mean=0.0, stddev=1.0) # 指定边界的正态分布

	""" 张量随机化 """
	# suffled_output = tf.random_shuffle(random_tsr2)                 # 张量随机化
	# cropped_output = tf.random_crop(random_tsr2,)                   # 张量随机剪裁

	print(zero_tsr)
	print(ones_tsr)
	print(fill_tsr)
	print(const_tsr)
	print(zero_similar)
	print(ones_similar)
	print(linear_tsr)
	print(limit_tsr)
	print(random_tsr)
	print(random_tsr1)
	print(random_tsr2)
	print(tf.convert_to_tensor([1, 2, 3, 4, 5, 6]))
	return


def var_test():
	""" 创建变量并初始化的例子 """
	# my_var = tf.Variable(tf.zeros([2, 3]))
	# sess = tf.Session()
	# init = tf.global_variables_initializer()      # 初始化
	# sess.run(init)

	""" 占位符使用 """
	sess = tf.Session()
	x = tf.placeholder(tf.float32, shape=[2, 3])
	y = tf.identity(x)                              # identity 操作返回占位符传入的数据本身
	x_vals = np.random.rand(2, 3)
	sess.run(y, feed_dict={x: x_vals})
	print(y)
	return


def matrix_test():
	""" 创建矩阵 """
	sess = tf.Session()
	m1 = tf.diag([1.0, 1.0, 1.0])                   # 创建对角阵
	m2 = tf.truncated_normal([3, 3])
	m3 = tf.truncated_normal([3, 2])
	m4 = tf.convert_to_tensor(np.array([
		[1, 2, 3, 0],
		[-1, -2, -3, 0],
		[0, 5, 2, 4],
	]))                                             # 根据 numpy 创建矩阵张量
	# print(m1)
	# print(m2)
	# print(m3)
	# print(sess.run(m1))
	# print(sess.run(m3))

	""" 矩阵加法、减法、乘法 """
	# print(sess.run(m1 + m2))
	# print(sess.run(m1 - m1))
	# print(sess.run(tf.matmul(m2, m3)))

	""" 矩阵转置 """
	# print(sess.run(tf.transpose(m4)))

	""" 矩阵行列式 """
	# print(sess.run(tf.matrix_determinant(m1)))

	""" 矩阵的逆矩阵 """
	# print(sess.run(tf.matrix_inverse(m2)))

	""" Cholesky 矩阵分解法 """
	# print(sess.run(tf.cholesky(m1)))

	""" 矩阵的特征值 和 特征向量 """
	# print(sess.run(tf.self_adjoint_eig(m2)))        # 第一行为特征值，剩下的向量时对应的向量
	return


def stimulate_test():
	sess = tf.Session()
	""" 整流线性单元 """
	print(sess.run(tf.nn.relu([-3., 3., 10.])))
	return


def graph_test():
	sess = tf.Session()
	""" 计算图——乘法 """
	# x_vals = np.array([1., 2., 3., 4., 5., 6., 7., 8., 9.])
	# x_data = tf.placeholder(tf.float32)
	# m_const = tf.constant(3.)
	# m = tf.multiply(x_data, m_const)
	# for i in x_vals:
	# 	print(sess.run(m, feed_dict={x_data: i}))

	""" 嵌入层 """
	arr1 = np.array([
		[1., 3., 5., 7., 9.],
		[-2., 0, 2., 4., 6.],
		[-6., 3., 0, -3., 6.]
	])
	x_vals = np.array([arr1, arr1 + 1])
	x_data = tf.placeholder(tf.float32, shape=(3, 5))
	m1 = tf.constant([
		[1.],
		[-1.0],
		[0.],
		[2.],
		[4.]
	])
	m2 = tf.constant([
		[2.]
	])
	a1 = tf.constant([
		[10.]
	])
	p1 = tf.matmul(x_data, m1)
	p2 = tf.matmul(p1, m2)
	p3 = tf.add(p2, a1)
	for i in x_vals:
		print(sess.run(p3, feed_dict={x_data: i}))
	return


if __name__ == '__main__':
	""" 声明张量 """
	# tensor_test()

	""" 占位符和变量 """
	# var_test()

	""" 操作(计算)矩阵 """
	# matrix_test()

	""" 激励函数 """
	# stimulate_test()

	""" 图操作 """
	graph_test()
	exit(0)
