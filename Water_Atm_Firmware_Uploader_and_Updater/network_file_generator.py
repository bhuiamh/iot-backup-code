#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri Aug 14 06:09:58 2020

@author: rango
"""

def network_conf_generator(Device_id):
    file_string = """
config interface 'loopback'
	option ifname 'lo'
	option proto 'static'
	option ipaddr '127.0.0.1'
	option netmask '255.0.0.0'

config interface 'lan'
	option ifname 'eth0'
        option proto 'dhcp'
        option metric '10'
	option hostname 'waterATM_"""+Device_id+"""'

config interface
	option ifname 'bat0'

config interface 'mesh_0'
	option proto 'static'
	option mtu '1528'
	option ipaddr '10.10.1.20'
	option netmask '255.255.255.0'

config interface 'wan'
	option ifname 'eth1'
	option proto 'dhcp'
	option hostname 'nybsys-1be6d0'
"""
    try:
        with open("network","w") as fp:
            fp.write(file_string)
        return True
    except Exception as eNetworkConfGenerator:
        print("Unable to generate network file")
        print(eNetworkConfGenerator)
        return False