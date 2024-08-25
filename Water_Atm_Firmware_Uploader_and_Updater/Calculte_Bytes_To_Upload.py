#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Sat Aug 15 00:23:52 2020

@author: rango
"""
import os

class Calculte_Bytes_To_Upload(object):
    
    def __init__(self,file_names):
        self.total_bytes = 0
        self.file_error = False
        self.file_point = ""
        for file in file_names:
            try:
                with open(file,"r") as fp:
                    self.total_bytes += len(fp.read())
            except Exception as eFileOpen:
                print("Error in opening and calculating file by size")
                print(eFileOpen)
                self.file_error = True
                self.file_point = os.path.basename(file)
                