import cv2
import subprocess
import os

camera = cv2.VideoCapture(0)
desiredsize = 224

for i in range(10):
	return_value, image = camera.read()
	height, width, channels = image.shape 
	start_x = int((width - desiredsize)/2)
	start_y = int((height - desiredsize)/2)
	crop_img = image[start_y:start_y+desiredsize, start_x:start_x+desiredsize]
	cv2.imwrite('opencv.jpg', crop_img)
	subprocess.call("python -m scripts.label_image --graph=tf_files/retrained_graph.pb --image=opencv.jpg", shell=True)
	os.remove("opencv.jpg")
	
del(camera)