#!/usr/bin/env python3.7
# -*- encoding=utf-8 -*-
import random

"""
朴素贝叶斯——3种红酒分类

数据集：
	第 1    列：红酒分类（1,2,3）三种分类
	第 2~14 列：分别表示13中成分（Alcohol，Malicacid，Ash，Alcalinity of ash，Magnesium，Total phenols，Flavanoids，Nonflavanoid phenols，Proanthocyanins，Color intensity，Hue，OD280/OD315 of diluted wines，Proline
"""

if __name__ == '__main__':
	dataSet = set()
	dataSetLen = 0              # 数据总数
	trainingSet = set()         # 训练集
	testSet = set()             # 测试集

	# 读取数据集
	with open('./wine.data', 'r') as fr:
		for line in fr.readlines():
			line = line.strip()
			dataSet.add(line)
	dataSetLen = len(dataSet)
	print('数据集：' + str(dataSetLen))

	# 随机拆分数据集
	for i in range(len(dataSet)):
		if random.randint(0, 10) <= 7:
			trainingSet.add(dataSet.pop())
		else:
			testSet.add(dataSet.pop())
	print('训练集：' + str(len(trainingSet)))
	print('测试集：' + str(len(testSet)))

	#


	exit(0)
