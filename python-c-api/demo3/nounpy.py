#!/usr/bin/env python
# -*- coding=utf-8 -*-

class Student:
    def SetName(self, name):
        self._name = name;
    def PrintName (self):
        print self._name

def hello():
    print "Hello World\n"

def World(name):
    print "name:" + name
