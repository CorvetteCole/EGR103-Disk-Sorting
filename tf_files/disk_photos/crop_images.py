#!/usr/bin/python

from PIL import Image
import os, sys

directory = 'carbon_fiber'

path = os.path.join(os.getcwd(), directory)
dirs = os.listdir( path )
desiredsize = 224

def crop_image(input_image):
	"""Pass input name image, output name image, x coordinate to start croping, y coordinate to start croping, width to crop, height to crop """
	input_img = Image.open(input_image)
	f,e = os.path.splitext(input_image)
	start_x = (input_img.size[0] - desiredsize)/2
	start_y = (input_img.size[1] - desiredsize)/2
	box = (start_x, start_y, start_x + desiredsize, start_y + desiredsize)
	output_img = input_img.crop(box)
	output_img.save(f +".jpg", 'JPEG')

def resize():
	for item in dirs:
		fullPath = os.path.join(path, item)
		if os.path.isfile(fullPath):
			crop_image(fullPath)
		
resize()