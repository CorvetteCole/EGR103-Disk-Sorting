#!/usr/bin/python

from PIL import Image
import os, sys

directory = 'wood'

path = os.path.join(os.getcwd(), directory)
dirs = os.listdir( path )
basewidth = 224

def resize():
    for item in dirs:
        fullPath = os.path.join(path, item)
        if os.path.isfile(fullPath):
            img = Image.open(fullPath)
            f, e = os.path.splitext(fullPath)
            wpercent = (basewidth/float(img.size[0]))
            hsize = int((float(img.size[1])*float(wpercent)))
            img = img.resize((basewidth,hsize), Image.ANTIALIAS)
            img.save(f +'.jpg', 'JPEG')

resize()