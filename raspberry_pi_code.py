from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import cv2

import serial

import argparse
import sys
import time

import gc

import numpy as np
import tensorflow as tf

# memory debuggingS
#from pympler.tracker import SummaryTracker
#import objgraph

# check the port the Arduino acquires by connecting and disconnecting it and seeing what is new in "ls /dev/tty*"
port = "/dev/ttyACM0" 
s1 = serial.Serial(port, 9600)
s1.flushInput()

camera = cv2.VideoCapture(0)
model_file = "tf_files/retrained_graph.pb"
label_file = "tf_files/retrained_labels.txt"
graph = load_graph(model_file)
labels = load_labels(label_file)
desiredsize = 224
input_layer = "input"
output_layer = "final_result"
input_name = "import/" + input_layer
output_name = "import/" + output_layer
input_operation = graph.get_operation_by_name(input_name);
output_operation = graph.get_operation_by_name(output_name);
template = "{} (score={:0.5f})"

sess = tf.Session()
sess2 = tf.Session(graph=graph)

input_height = 224
input_width = 224
input_mean = 128
input_std = 128

def load_graph(model_file):
  graph = tf.Graph()
  graph_def = tf.GraphDef()

  with open(model_file, "rb") as f:
    graph_def.ParseFromString(f.read())
  with graph.as_default():
    tf.import_graph_def(graph_def)

  return graph
  
def load_labels(label_file):
  label = []
  proto_as_ascii_lines = tf.gfile.GFile(label_file).readlines()
  for l in proto_as_ascii_lines:
    label.append(l.rstrip())
  return label

while True:
	if s1.inWaitin() > 0:
		inputValue = s1.read(1)
		
		# if Arduino sends "1" over serial, it will trigger object classification
		if inputValue == 1:
		
			print("Arduino triggered object classification")

			# look at image and run classification until a classification with more than 80% confidence is made
			classification
			classification_number
			confidence = 0.0;
			while confidence < 0.8:
				return_value, image = camera.read()
				height, width, channels = image.shape 
				start_x = int((width - desiredsize)/2)
				start_y = int((height - desiredsize)/2)
				crop_img = image[start_y:start_y + desiredsize, start_x:start_x + desiredsize]
				
				# adhere to TS graph input structure
				float_caster = tf.cast(np.asarray(crop_img), tf.float32)
				dims_expander = tf.expand_dims(float_caster, 0);
				normalized = tf.divide(tf.subtract(dims_expander, [input_mean]), [input_std])
				t = sess.run(normalized)
			
				
				#with tf.Session(graph=graph) as sess2:
				start = time.time()
				results = sess2.run(output_operation.outputs[0],
							  {input_operation.outputs[0]: t})
				end = time.time()
				results = np.squeeze(results)
				top_k = results.argsort()[-5:][::-1]

				confidence = results[top_k[0]]
				classification = labels[top_k[0]]
				classification_number = top_k[0]
			
			print("Final classification made: ")
			print(classification)
			print(classification_number)
			print(confidence)
			
			# send number corresponding to classification to Arduino
			# cloth - 0
			# metal - 1
			# sandpaper - 2
			# wood - 3
			s1.write(classification_number)
del(camera)