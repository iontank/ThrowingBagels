"""
Hue rotation sender program to control multiple 
ThrowingBagels services at once
"""
import numpy as np
import socket
import json
from time import sleep
from argparse import ArgumentParser
from bagelbone import BagelThrower
from colorsys import hsv_to_rgb

parser = ArgumentParser(prog="Color rotator", description="Simple demo client for ThrowingBagels")
parser.add_argument("config", default="color.json")
parser.add_argument("--speed", help="How many seconds it takes to do a full hue rotation", 
  default=4.0, type=float)

args = parser.parse_args()

with open(args.config, "r") as cfg:
  config = json.load(cfg)
speed = args.speed

thrower = BagelThrower(config)

hue = 0

frame_dur = 1.0/30.0

while True:
  col = (np.array(hsv_to_rgb(hue, 1, 1)) * 255).astype(np.uint8)
  frame = thrower.get_frame_buffer()
  frame[::] = col
  thrower.send_frame(frame)
  sleep(frame_dur)
  hue += frame_dur/speed
  