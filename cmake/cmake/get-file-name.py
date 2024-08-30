#!/usr/bin/env python
# -*- coding=utf-8 -*-
import sys

if __name__ == '__main__':
    if len(sys.argv) != 2:
        sys.exit(1)
    print (sys.argv[1].split("/")[-1])
    sys.exit(0)
