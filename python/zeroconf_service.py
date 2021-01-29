#!/usr/bin/env python3

import logging
import socket
from time import sleep

from zeroconf import IPVersion, ServiceInfo, Zeroconf

def get_ip():
  '''Return IP Address & Hostname as strings'''
  s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
  try:
    s.connect(('10.255.255.255', 1))
    ip = s.getsockname()[0]
    host = socket.gethostname()
  except Exception:
    ip = '127.0.0.1'
    host = 'BBB'
  finally:
    s.close()
  return (ip, host)

def main():
  '''Register the zeroconf service'''
  logging.basicConfig(level=logging.DEBUG)
  serv_name = '_throwin-bagels._tcp.local.'
  ip_addr, hostname = get_ip()
  serv_info = ServiceInfo(serv_name,
    f'{hostname}.{serv_name}',
    addresses=[socket.inet_aton(ip_addr)],
    port=5440,
    properties={'hello':'world'})

  print(f'Registering: {serv_name}')
  zeroconf = Zeroconf(ip_version=IPVersion.V4Only)
  zeroconf.register_service(serv_info)
  try:
    while True:
      sleep(1)
  except KeyboardInterrupt:
    pass
  finally:
    print(f'Unregistering: {serv_name}')
    zeroconf.unregister_service(serv_info)
    zeroconf.close()

if __name__ == '__main__':
  main()
