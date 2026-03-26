#!/usr/bin/env python
# -*- coding=utf-8 -*-
import yfinance as yf
import matplotlib.pyplot as plt
import pandas as pd

# 设置中文字体 (防止乱码，Windows 通常用 SimHei, Mac 用 Arial Unicode MS)
plt.rcParams['font.sans-serif'] = ['SimHei', 'Arial Unicode MS', 'DejaVu Sans'] 
plt.rcParams['axes.unicode_minus'] = False

# 1. 获取数据 (黄金现货价格代码为 GC=F，或者 XAUUSD=X)
ticker = "GC=F" # 纽约商品交易所黄金期货
start_date = "2014-01-01"
end_date = "2024-06-01"

try:
    data = yf.download(ticker, start=start_date, end=end_date)

    if not data.empty:
        # 2. 绘制收盘价走势
        plt.figure(figsize=(14, 7))
        plt.plot(data.index, data['Close'], label='Gold Price (USD/oz)', color='#FFD700', linewidth=2)
        
        # 添加标题和标签
        plt.title(f'{ticker} Gold Trend Past 10 Years (2014-2024)', fontsize=16, fontweight='bold')
        plt.xlabel('Date', fontsize=12)
        plt.ylabel('Price (USD/oz)', fontsize=12)
        
        # 添加网格
        plt.grid(True, linestyle='--', alpha=0.7)
        
        # 标记最高点和最低点 (可选)
        max_price = data['Close'].max()
        min_price = data['Close'].min()
        print(f"十年间最高价：{max_price:.2f}")
        print(f"十年间最低价：{min_price:.2f}")

        plt.show()
    else:
        print("未获取到数据，请检查网络或代码库。")

except Exception as e:
    print(f"发生错误：{e}")

