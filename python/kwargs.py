#!/bin/env python
# -*- encoding=utf8 -*-

def my_function1(*args):
    for arg in args:
        print(arg)

def my_function2(**kwargs):
    for k, v in kwargs.items():
        print("" + k + " = " + v)



my_function1('Hello', 'world', 'Python', 'is', 'awesome', 'okk')
my_function2(k1='Hello', k2='world', k3='Python', k4='is', k5='awesome', k6='okk')
