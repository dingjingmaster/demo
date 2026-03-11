#!/usr/bin/env python
# -*- coding=utf-8 -*-

import MetaTrader5 as mt5
import pandas as pd
import time

# =========================
# 1. 初始化 MT5
# =========================
mt5.initialize()

account = 12345678        # 替换为你的账号
password = "your_password"
server = "YourBroker-Demo"

authorized = mt5.login(account, password=password, server=server)
if not authorized:
    print("登录失败")
    mt5.shutdown()
    exit()
print("登录成功")

# =========================
# 2. 获取实时金价数据
# =========================
symbol = "XAUUSD"
timeframe = mt5.TIMEFRAME_M1  # 1分钟线
n_bars = 100                  # 获取最近 100 条数据

def get_data():
    rates = mt5.copy_rates_from_pos(symbol, timeframe, 0, n_bars)
    df = pd.DataFrame(rates)
    df['time'] = pd.to_datetime(df['time'], unit='s')
    return df

# =========================
# 3. 简单分析策略
# =========================
# 使用短期/长期移动平均判断买卖信号
short_window = 5
long_window = 20

def analyze(df):
    df['ma_short'] = df['close'].rolling(short_window).mean()
    df['ma_long'] = df['close'].rolling(long_window).mean()
    
    # 简单策略：短期均线上穿长期均线 -> 买入
    # 短期均线下穿长期均线 -> 卖出
    if df['ma_short'].iloc[-2] < df['ma_long'].iloc[-2] and df['ma_short'].iloc[-1] > df['ma_long'].iloc[-1]:
        return "buy"
    elif df['ma_short'].iloc[-2] > df['ma_long'].iloc[-2] and df['ma_short'].iloc[-1] < df['ma_long'].iloc[-1]:
        return "sell"
    else:
        return "hold"

# =========================
# 4. 自动下单
# =========================
lot = 0.01  # 交易手数

def place_order(signal):
    if signal == "buy":
        order_type = mt5.ORDER_TYPE_BUY
    elif signal == "sell":
        order_type = mt5.ORDER_TYPE_SELL
    else:
        return

    price = mt5.symbol_info_tick(symbol).ask if order_type == mt5.ORDER_TYPE_BUY else mt5.symbol_info_tick(symbol).bid
    
    request = {
        "action": mt5.TRADE_ACTION_DEAL,
        "symbol": symbol,
        "volume": lot,
        "type": order_type,
        "price": price,
        "deviation": 10,
        "magic": 123456,
        "comment": "Python auto trade",
        "type_filling": mt5.ORDER_FILLING_IOC
    }

    result = mt5.order_send(request)
    print(result)

# =========================
# 5. 循环执行策略
# =========================
try:
    while True:
        df = get_data()
        signal = analyze(df)
        print(f"{pd.Timestamp.now()} - 信号: {signal}, 最新收盘价: {df['close'].iloc[-1]}")
        if signal != "hold":
            place_order(signal)
        time.sleep(60)  # 每分钟检查一次
except KeyboardInterrupt:
    print("结束程序")
finally:
    mt5.shutdown()
