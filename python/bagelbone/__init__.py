"""
Simple manager class for talking to 
ThrowingBagels LED UDP RX instances
"""
import socket
import numpy as np

def packetize(frame):
  """
  Convert a frame into a Bagel compatible packet pair.

  The frame is split in half, and each packet gets an
  index prepended to identify which half of the frame
  it is.
  """
  size = int(len(frame) / 2)
  p1 = np.concatenate(([0], frame[0:size]))
  p2 = np.concatenate(([1], frame[size:size*2]))
  return (p1, p2)

class BagelThrower:
  def __init__(self, configDict:dict):
    self.cfg = configDict
    self.sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
  
  def get_frame_buffer(self):
    return np.zeros((
      self.cfg["frameSize"]["width"],
      self.cfg["frameSize"]["height"],
      3
    ), dtype=np.uint8)

  def send_frame(self, frame_data):
    for target in self.cfg["targets"]:
      x = target["x"]
      y = target["y"]
      w = target["w"]
      h = target["h"]
      addr = (target["hostname"], target.get("port", 9999))
      sliced = frame_data[x:x+w,y:y+h].flatten()
      packets = packetize(sliced)
      try:
        for p in packets:
          self.sock.sendto(bytearray(p.astype(np.uint8)), addr)
      except:
        print(f"Failed to send to {addr}")

