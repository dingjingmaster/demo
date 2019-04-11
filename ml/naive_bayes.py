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
	trainingSet = dict()        # 训练集
	testSet = dict()            # 测试集
	priorProbability = dict()   # 先验概率计算结果
	attrRange = dict()          # 属性范围
	normTrainingSet = dict()    # 归一化之后的训练集
	attrRangeProb = dict()      # 每个属性条件概率
	attrProb = dict()           # 每个属性发生的概率

	# 读取数据集
	with open('./wine.data', 'r') as fr:
		for line in fr.readlines():
			line = line.strip()
			dataSet.add(line)
	dataSetLen = len(dataSet)
	print('数据集：' + str(dataSetLen), end=',\t')

	# 随机拆分数据集
	for i in range(len(dataSet)):
		if random.randint(0, 10) <= 7:
			trainingSet[len(trainingSet)] = dataSet.pop().split(',')
		else:
			testSet[len(testSet)] = dataSet.pop().split(',')
	print('训练集：' + str(len(trainingSet)), end=',\t')
	print('测试集：' + str(len(testSet)), end='\n\n')

	# 计算先验概率
	for ik, iv in trainingSet.items():
		cateName = 'cate' + iv[0]
		if cateName in priorProbability:
			priorProbability[cateName] += 1.0
		else:
			priorProbability[cateName] = 1.0
	for ik, iv in priorProbability.items():
		priorProbability[ik] = (priorProbability[ik], priorProbability[ik] / len(trainingSet))

	# 输出先验概率
	for ik, iv in priorProbability.items():
		print(ik + '\t' + str(iv))
	print('')

	# 分析各个属性范围
	for ik, iv in trainingSet.items():
		for i in range(1, len(iv)):
			attrName = 'attr' + str(i)
			tmp = float(iv[i])
			if attrName in attrRange:
				maxt, mint = attrRange[attrName]
				if maxt <= tmp:
					maxt = tmp
				if mint >= tmp:
					mint = tmp
				attrRange[attrName] = (maxt, mint)
			else:
				attrRange[attrName] = (tmp, tmp)

	# 查看每个属性的范围
	for ik, iv in attrRange.items():
		print(str(ik) + '\t\t' + str(iv[1]) + ' ~ ' + str(iv[0]))
	print('')

	# 归一化每个属性，并将每个属性分为 四档，并划分档次
	for ik, iv in trainingSet.items():
		norm = list()
		norm.append(iv[0])                                              # 分类结果
		for i in range(1, len(iv)):
			attrName = 'attr' + str(i)
			tmp = float(iv[i])
			maxt, mint = attrRange[attrName]
			stept = (maxt - mint) / 4
			for j in range(1, 5):
				if (tmp >= mint) and (tmp <= stept * j + mint):
					tmp = j
					break
			norm.append(tmp)
		normTrainingSet[ik] = norm

	# 打印归一化后的数据
	for ik, iv in normTrainingSet.items():
		print(ik, end=',')
		for i in iv:
			print(i, end=',')
		print('')
	print('')

	# 每个属性发生的条件概率
	for ik, iv in normTrainingSet.items():
		for i in range(1, len(iv)):
			attrName = 'attr' + str(i)
			attrResult = attrName + '|' + iv[0]
			attrValue = iv[i]
			v1 = 0.0
			v2 = 0.0
			v3 = 0.0
			v4 = 0.0
			if attrValue == 4:
				v4 = 1.0
			elif attrValue == 3:
				v3 = 1.0
			elif attrValue == 2:
				v2 = 1.0
			elif attrValue == 1:
				v1 = 1.0
			if attrName in attrProb:
				v1t, v2t, v3t, v4t = attrProb[attrName]
				attrProb[attrName] = (v1 + v1t, v2 + v2t, v3 + v3t, v4 + v4t)
			else:
				attrProb[attrName] = (v1, v2, v3, v4)
			if attrResult in attrRangeProb:
				v1t, v2t, v3t, v4t = attrRangeProb[attrResult]
				attrRangeProb[attrResult] = (v1 + v1t, v2 + v2t, v3 + v3t, v4 + v4t)
			else:
				attrRangeProb[attrResult] = (v1, v2, v3, v4)

	# 计算因子
	for ik, iv in attrRangeProb.items():
		arr = ik.split('|')
		attrName = arr[0]
		cate = 'cate' + str(arr[1])
		cateNum = priorProbability[cate][0]
		v1, v2, v3, v4 = iv
		attrRangeProb[ik] = (float(v1)/cateNum, float(v2)/cateNum, float(v3)/cateNum, float(v4)/cateNum)

	for ik, iv in attrRangeProb.items():
		print(ik, end='\t')
		for i in iv:
			print(str(i), end=',')
		print('')

	for ik, iv in attrProb.items():
		print(ik, end='\t')
		for i in iv:
			print(str(i), end=',')
		print('')

	# 测试训练结果
	right = 0
	wrong = 0
	total = len(testSet)
	for ik, iv in testSet.items():
		cate1 = 0.0
		cate2 = 0.0
		cate3 = 0.0
		trueCate = iv[0]
		calcCate = ''
		maxCate1 = 0.0
		maxCate2 = 0.0
		maxCate3 = 0.0
		for i in range(1, len(iv)):
			# 属性归一化
			attr = float(iv[i])
			attrName = 'attr' + str(i)
			maxt, mint = attrRange[attrName]
			stept = float(maxt - mint) / 4
			norm = 0
			if attr < float(mint) + stept * 1:
				norm = 1
			elif attr < float(mint) + stept * 2:
				norm = 2
			elif attr < float(mint) + stept * 3:
				norm = 3
			elif attr >= float(mint) + stept * 3:
				norm = 4
			# 该属性的概率
			attrP = attrProb[attrName][norm - 1]
			# 该属性下类别的概率
			for j, jv in priorProbability.items():
				tf, gl = jv                                                     # gl 表示该分类结果的先验概率
				res = j.replace('cate', '')                                     # 获得结果名
				a1t, a2t, a3t, a4t = attrRangeProb[attrName + '|' + res]        # 条件概率p(b|c)
				if norm == 1:
					at = a1t
				elif norm == 2:
					at = a2t
				elif norm == 3:
					at = a3t
				else:
					at = a4t
				cateRes = at * gl / attrP
				if res == '1' and maxCate1 <= cateRes:
					maxCate1 = cateRes
				elif res == '2' and maxCate2 <= cateRes:
					maxCate2 = cateRes
				elif res == '3' and maxCate3 <= cateRes:
					maxCate3 = cateRes
		mm = max(maxCate1, maxCate2, maxCate3)
		if mm == maxCate1:
			calcCate = '1'
		elif mm == maxCate2:
			calcCate = '2'
		elif mm == maxCate3:
			calcCate = '3'
		if calcCate == trueCate:
			right += 1
		else:
			wrong += 1

	# 计算预测的正确率
	print()
	print("正确率:" + str(float(right) / total))
	print("错误率:" + str(float(wrong) / total))

	exit(0)
