from scapy.all import *

eth = Ether(dst="ff:ff:ff:ff:ff:ff", src="52:54:00:12:34:56", type=0x0800) / Raw(load="ABCDEFGHIJKLMNOPQRSTUVWXYZ")
sendp(eth, iface="tap0_adeos", count=2)