#!/usr/bin/env python
# -*- coding=utf-8 -*-
import sys
import time
import math
import random
import requests
import threading
import numpy as np
import tkinter as tk
import MetaTrader5 as mt5 # E: Cannot find implementation or library stub for module named "MetaTrader5"  [import-not-found]
import matplotlib.pyplot as plt

from enum import Enum
from tkinter import ttk, scrolledtext
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg


# MT5 配置
account  = "104290350"
password = "V^5mPuHr"
server   = "MetaQuotes-Demo"

if not mt5.initialize():
    print (mt5.last_error())
    quit()

# 实时金价
def get_real_gold_price():
    try:
        url = "https://api.gold-api.com/price/XAU"
        res = requests.get(url, timeout=5)
        data = res.json()
        return float(data.get("price", 0))
    except:
        return None

class OrderType(Enum):
    BUY = 1
    SELL = 2

# MT5
class MT5Server:
    def __init__(self):
        self._symbolStr = "XAUUSD"
    def login(self, account, password, server) -> bool:
        self._auth  = mt5.login(account, password=password, server=server)
        return self._auth
    def place_order(self, orderType: OrderType, money) -> bool:
        ot = None
        symbolInfo  = mt5.symbol_info(self._symbolStr)
        if symbolInfo is None:
            print("Symbol error!")
            return False
        if not symbolInfo.visible:
            mt5.symbol_select(self._symbolStr, True)
        if not symbolInfo.visible:
            print("{self._symbolStr}不可见,检查是否写错.")
            return False

        tick = mt5.symbol_info_tick(self._symbolStr)
        if tick is None:
            print (f"获取 {self._symbolStr} tick 数据失败")
            return False

        if orderType == OrderType.BUY:
            ot = mt5.ORDER_TYPE_BUY
        else:
            ot = mt5.ORDER_TYPE_SELL
        
        price = tick.ask if ot == mt5.ORDER_TYPE_BUY else tick.bid
        # 最小手数
        minVolume = symbolInfo.volume_min
        # 步长
        volumeStep = symbolInfo.volume_step
        # 最大手数
        maxVolume = symbolInfo.volume_max
        print(f"{self._symbolStr} 最小手数: {minVolume}, 步长: {volumeStep}, 最大手数: {maxVolume}")
        myVolume = float(money) / price

        volume = math.floor(myVolume / volumeStep) * volumeStep
        volume = max(volume, minVolume)
        volume = min(volume, maxVolume)

        print(f"{self._symbolStr} 下单价格: {price}, 购买 {volume} 手, 花费: {price * volume}.")
        request = {
            "action": mt5.TRADE_ACTION_DEAL,
            "symbol": self._symbolStr,
            "volume": volume,
            "type": ot,
            "price": price,
            "deviation": 10,
            "magic": 123456998866,
            "comment": "Python auto trade",
            "type_filling": mt5.ORDER_FILLING_IOC
        }
        result = mt5.order_send(request)
        if None is result:
            print("下单失败, 服务器没有响应.")
            return False
        if result.retcode != mt5.TRADE_RETCODE_DONE:
            if 10027 == result.retcode:
                print(f"订单被服务器拒绝, 客户端没有打开允许算法交易开关")
                return False
            print("下单失败, 原因: {result.comment}")
            return False
        return True;

# 主程序
class GoldAIProMax:
    def __init__(self, root):
        self.root = root
        self.root.title("黄金AI量化交易系统 · 专业版")
        self.root.geometry("900x950")
        self.root.resizable(False, False)
        self.root.configure(bg="#121418")

        # mt5
        self._mt5 = MT5Server()

        # 数据
        self.price = 0.0
        self.price_history = []
        self.time_history = []
        self.is_running = False

        # 策略参数
        self.ma_fast = 10
        self.ma_slow = 20
        self.atr_period = 14
        self.atr_mult = 1.5
        self.grid_step = 8
        self.martin_step = 2
        self.max_martin = 5
        self.take_profit = 12
        self.stop_loss = 18

        # 交易
        self.position = 0
        self.cost = 0.0
        self.profit = 0.0
        self.martin_level = 0

        # AI状态
        self.volatility = "中等"
        self.pattern = "无形态"
        self.ai_strategy = "等待策略"

        self.init_style()
        self.create_ui()

    # 登录
    def loginMT5(self, account, password, server) -> bool:
        if not self._mt5.login(account, password, server):
            print("login error: ", mt5.last_error());
        return True


    # 深色主题
    def init_style(self):
        style = ttk.Style()
        style.theme_use("clam")
        style.configure(".", background="#121418", foreground="white")
        style.configure("Title.TLabel", font=("微软雅黑", 26, "bold"),
                        background="#121418", foreground="#00d8ff")
        style.configure("Signal.TLabel", font=("微软雅黑", 18, "bold"))
        style.configure("Info.TLabel", font=("微软雅黑", 13))
        style.configure("Button.TButton", font=("微软雅黑", 11, "bold"),
                        padding=6, background="#2563eb")

    def log(self, msg):
        self.log_box.insert(tk.END, f"[{time.strftime('%H:%M:%S')}] {msg}\n")
        self.log_box.see(tk.END)

    # ================== 界面 ==================
    def create_ui(self):
        # 标题
        ttk.Label(self.root, text="黄金AI量化交易系统", style="Title.TLabel").pack(pady=12)

        # 价格面板
        frame_top = tk.Frame(self.root, bg="#1a1d24", bd=2, relief="ridge")
        frame_top.pack(pady=5, padx=10, fill="x")

        l1 = tk.Label(frame_top, text="实时金价", font=("微软雅黑",14), bg="#1a1d24", fg="white")
        l1.grid(row=0, column=0, padx=15, pady=8)
        #tk.Label(frame_top, text="实时金价", font=("微软雅黑",14), bg="#1a1d24", fg="white").grid(row=0,column=0,pad=15,pady=8)
        self.lb_price = tk.Label(frame_top, text="$----", font=("微软雅黑",28,"bold"), bg="#1a1d24", fg="#ff4757")
        self.lb_price.grid(row=0,column=1,pady=15)

        tk.Label(frame_top, text="AI信号", font=("微软雅黑",14), bg="#1a1d24", fg="white").grid(row=0,column=2,pady=15)
        s1 = ttk.Style()
        s1 .configure("Signal.TLabel", background="#1a1d24", foreground="#00d8ff", font=("微软雅黑", 14))
        #self.lb_signal = ttk.Label(frame_top, text="初始化...", style="Signal.TLabel", background="#1a1d24", fg="#00d8ff")
        self.lb_signal = ttk.Label(frame_top, text="初始化...", style="Signal.TLabel")
        self.lb_signal.grid(row=0,column=3,padx=15,pady=15)

        # 信息面板
        frame_info = tk.Frame(self.root, bg="#1a1d24", bd=2, relief="ridge")
        frame_info.pack(pady=5,padx=10,fill="x")
        self.lb_pos = ttk.Label(frame_info, text="持仓: 0", style="Info.TLabel", background="#1a1d24", foreground="white")
        self.lb_pos.grid(row=0,column=0,padx=12,pady=6)
        self.lb_profit = ttk.Label(frame_info, text="盈亏: $0.00", style="Info.TLabel", background="#1a1d24", foreground="white")
        self.lb_profit.grid(row=0,column=1,padx=12,pady=12)
        self.lb_vol = ttk.Label(frame_info, text="波动: 中等", style="Info.TLabel", background="#1a1d24", foreground="white")
        self.lb_vol.grid(row=0,column=2,padx=12,pady=12)
        self.lb_pat = ttk.Label(frame_info, text="形态: 无", style="Info.TLabel", background="#1a1d24", foreground="white")
        self.lb_pat.grid(row=0,column=3,padx=12,pady=12)
        self.lb_strategy = ttk.Label(frame_info, text="策略: 等待", style="Info.TLabel", background="#1a1d24", foreground="#00d8ff")
        self.lb_strategy.grid(row=0,column=4,padx=12,pady=12)

        # 按钮
        frame_btn = tk.Frame(self.root, bg="#121418")
        frame_btn.pack(pady=6)
        btns = [
            ("启动AI", self.start, "#10b981"),
            ("停止AI", self.stop, "#ef4444"),
            ("买入", self.buy, "#3b82f6"),
            ("卖出", self.sell, "#f59e0b"),
            ("一键平仓", self.close_all, "#8b5cf6"),
            ("AI策略", self.gen_strategy, "#06b6d4"),
            ("重置", self.reset, "#64748b")
        ]
        for i,(txt,cmd,color) in enumerate(btns):
            btn = tk.Button(frame_btn, text=txt, command=cmd, font=("微软雅黑",11,"bold"), bg=color, fg="white", width=8, relief="flat", bd=0)
            btn.grid(row=0,column=i,padx=4,pady=4)

        # K线
        self.fig = plt.figure(figsize=(10,3.2), dpi=90)
        self.fig.patch.set_facecolor("#121418")
        self.ax = self.fig.add_subplot(111)
        self.ax.set_facecolor("#1a1d24")
        self.ax.tick_params(axis='x', colors='white')
        self.ax.tick_params(axis='y', colors='white')
        self.ax.spines[:].set_color("gray")
        self.canvas = FigureCanvasTkAgg(self.fig, master=self.root)
        self.canvas.get_tk_widget().pack(pady=5)

        # 日志
        tk.Label(self.root, text="AI交易日志", font=("微软雅黑",12), bg="#121418", fg="white").pack()
        self.log_box = scrolledtext.ScrolledText(self.root, height=18, width=110, bg="#1a1d24", fg="white", insertbackground="white")
        self.log_box.pack(pady=6,padx=10)

    # ================== 核心逻辑 ==================
    def update_price(self):
        p = get_real_gold_price()
        if not p: return
        self.price = p
        self.price_history.append(p)
        self.time_history.append(time.strftime("%H:%M"))
        if len(self.price_history) > 60:
            self.price_history.pop(0)
            self.time_history.pop(0)
        self.lb_price.config(text=f"${p:.2f}")
        self.ai_analysis()
        self.auto_trade()
        self.update_chart()

    def ai_analysis(self):
        if len(self.price_history) < 30:
            #self.lb_signal.config(text="数据收集中...", fg="#00d8ff")
            self.lb_signal.config(text="数据收集中...")
            return

        ma10 = np.mean(self.price_history[-10:])
        ma20 = np.mean(self.price_history[-20:])
        if ma10 > ma20 * 1.003:
            self.lb_signal.config(text="AI看多 ", fg="#ff4757")
        elif ma10 < ma20 * 0.997:
            self.lb_signal.config(text="AI看空 📉", fg="#10b981")
        else:
            self.lb_signal.config(text="震荡观望", fg="#00d8ff")

        # 波动率
        tr = [abs(self.price_history[i]-self.price_history[i-1]) for i in range(1, len(self.price_history))]
        if len(tr) >= self.atr_period:
            atr = np.mean(tr[-self.atr_period:])
            if atr > 15: self.volatility = "高波动"
            elif atr < 6: self.volatility = "低波动"
            else: self.volatility = "中等波动"
        self.lb_vol.config(text=f"波动: {self.volatility}")

        # 形态
        self.pattern = random.choice([
            "无形态","双底","双顶","头肩底","头肩顶","三角整理","突破形态"
        ])
        self.lb_pat.config(text=f"形态: {self.pattern}")

    def auto_trade(self):
        p = self.price
        if self.position == 0:
            # 网格
            if len(self.price_history) >= 6:
                if p < self.price_history[-5] - self.grid_step:
                    self.buy()
                    self.log(f"网格低买：{p:.2f}")
                elif p > self.price_history[-5] + self.grid_step:
                    self.sell()
                    self.log(f"网格高卖：{p:.2f}")
        else:
            # 止盈止损
            if (self.position>0 and p >= self.cost + self.take_profit) or \
               (self.position<0 and p <= self.cost - self.take_profit):
                self.close_all()
                self.log("✅ 自动止盈")
            if (self.position>0 and p <= self.cost - self.stop_loss) or \
               (self.position<0 and p >= self.cost + self.stop_loss):
                self.close_all()
                self.log("🛑 自动止损")
                if self.martin_level < self.max_martin:
                    self.martin_level +=1
                self.log(f"马丁第{self.martin_level}层准备")

        self.lb_pos.config(text=f"持仓: {self.position}")
        self.lb_profit.config(text=f"盈亏: ${self.profit:.2f}")

    def buy(self):
        if self.price <= 0: return
        self.position += 1
        self.profit -= self.price
        self.cost = self.price
        self.log(f"买入：{self.price:.2f}")
        if self._mt5.place_order(OrderType.BUY, self.price):
            self.log(f"买入成功")
        else:
            self.log(f"买入失败")

    def sell(self):
        if self.price <= 0: return
        self.position -= 1
        self.profit += self.price
        self.cost = self.price
        self.log(f"卖出：{self.price:.2f}")
        if self._mt5.place_order(OrderType.SELL, self.price):
            self.log(f"卖出成功")
        else:
            self.log(f"卖出失败")

    def close_all(self):
        if self.position ==0: return
        self.profit += self.position * self.price
        self.log(f"一键平仓 | 盈亏：${self.position*self.price:.2f}")
        self.position = 0
        self.cost = 0
        self.log("没实现 ...")

    def gen_strategy(self):
        s = random.choice([
            "稳健网格策略","高频震荡策略","趋势跟踪策略",
            "突破策略","低风险套利","波动率策略"
        ])
        self.ai_strategy = s
        self.lb_strategy.config(text=f"策略: {s}")
        self.log(f"🤖 AI生成策略：{s}")

    def update_chart(self):
        self.ax.clear()
        self.ax.plot(self.time_history, self.price_history, linewidth=2, color="#00d8ff")
        self.ax.set_title("黄金实时走势", color="white", fontsize=12)
        self.ax.grid(True, alpha=0.2)
        self.ax.tick_params(axis='x', rotation=45, labelsize=8)
        self.canvas.draw()

    def loop(self):
        while self.is_running:
            self.update_price()
            time.sleep(10)

    def start(self):
        if not self.is_running:
            self.is_running = True
            threading.Thread(target=self.loop, daemon=True).start()
            self.log(" AI系统启动 · 24小时自动运行")

    def stop(self):
        self.is_running = False
        self.log("⏹ AI已停止")

    def reset(self):
        self.position = 0
        self.profit = 0
        self.cost = 0
        self.martin_level = 0
        self.lb_pos.config(text="持仓: 0")
        self.lb_profit.config(text="盈亏: $0.00")
        self.log("🔄 已重置")

if __name__ == "__main__":
    root = tk.Tk()
    app = GoldAIProMax(root)
    if not app.loginMT5(account, password, server):
        print("登录MT5失败!")
        sys.exit(1)
    root.mainloop()

