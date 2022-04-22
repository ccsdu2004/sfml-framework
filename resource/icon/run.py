#!/usr/bin/env python
# -*- coding: utf-8 -*-
 
import os
 
def file_name(file_dir):
    fileList = []
    for root,dirs,files in os.walk(file_dir):
        for file in files:
            if os.path.splitext(file)[1] == '.png':
                name = os.path.join(root,file)
                name = '"./../resource/' + name[2:] + '"'
                fileList.append(name)
    return fileList
 
 
res = file_name("./")
for name in res:
    print (name)
