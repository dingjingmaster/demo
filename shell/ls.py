#!/usr/bin/env python
# coding=utf-8

import os

print ('os.system:')
print (os.system('ls -al'))

list = os.popen('ls -al')

print ('os.popen:')
for i in list:
    print (i)

