#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Jul 15 15:01:39 2020
NybSys Pvt Ltd. all rights reserved . Copyright © July2020

@author: rango
@contact: amirkhondokar@gmail.com

Usage:
    python Firmware_sender.py file1 file2 file3 fileN
        or if you have compiled binary then
    Firmware_sender file1 file2 file3 fileN
    
File Target:
    To generate NybSys WaterATM MPU firmware that
    will be used to transmit updated other firmware 
    files sent over MQTT .
    
    Basically this transmitter module will be published to
    "FU/device_id/device_password" topic to transmit files
    that is broadcasted from our IoT Team for firmware upgradation
"""

import paho.mqtt.client as paho
import json
import time
import sys
import threading
import os

class Acknowledgement_Receiver(object):
    def __init__(self,configuration_file="WaterATM_config.json",files_ack_to_receive=["main.py"]):
        self.alive = True
        self.load_configuration(configuration_file)
        #self.config["debug_mode"] = True
        self.files_ack_to_receive =[]
        for filename in files_ack_to_receive:
            self.files_ack_to_receive.append(os.path.basename(filename))
        print("Files Expecting for acknowledgement :")
        print(self.files_ack_to_receive)
        self.number_of_files_remaining_ack = len(self.files_ack_to_receive)
        self.unable_to_connect = True
        self.Acknowledgement = {"Received":["Dummy received"],"Action":["Dummy Action"]}
        self.client= paho.Client("client-acknowledgement-receiver-"+self.config["device_id"])
        self.client.on_message= self.on_message
        self.client.on_connect = self.on_connect
        try:
            print("connecting to broker")
            self.client.connect(self.config["broker"])
            #if self.config["debug_mode"]:
            print("Connected to broker : "+self.config["broker"])
            self.topic = "FU/"+self.config["device_id"]+"/"+self.config["password"]+"/reply"
            #if self.config["debug_mode"]:
            print("Subscribing on topic: "+self.topic)
            self.client.subscribe(self.topic)
            self.unable_to_connect = False
        except Exception as eConnectAck:
            print("Unable to connect from acknowledgement receiver")
            print(eConnectAck)
            self.unable_to_connect = True

    def load_configuration(self, configuration_file):
        with open(configuration_file,"r") as conf:
            self.config = json.load(conf)
    
    def on_message(self, client, userdata, message):
       data_dict = json.loads(message.payload)
       Key = list(data_dict.keys())[0]
       if Key=="Received":
           self.Acknowledgement[Key].append(data_dict[Key])
           if self.config["debug_mode"]:
               print("Acknowledgement for file: "+data_dict[Key])
           if data_dict[Key] in self.files_ack_to_receive:
               self.number_of_files_remaining_ack = self.number_of_files_remaining_ack- 1
               print("counted down")
               if self.config["debug_mode"]:
                   print("Remaining files to receive : "+self.number_of_files_remaining_ack)
           print("dummy pass")
       elif Key=="Action":
           self.Acknowledgement[Key].append(data_dict[Key])
           if self.config["debug_mode"]:
               print("Action acknowledgement : "+self.Acknowledgement[Key][-1])
       else:
           print("###########################################################")
           print("Acknowledgement Received Message")
           print(data_dict)
           print("###########################################################")
    
    def on_connect(self):
        print("--------------------------------------------------------------------------------")
        print("Ready to receive acknowledgement message on topic : ")
        print(self.topic)
        print("--------------------------------------------------------------------------------")
    
    def wait_as_ack_receiver_client(self):
        while self.alive:
            self.client.loop(10)
    
    def __del__(self):
        print("Destroying Acknowledgement Receiver")
        try:
            self.client.disconnect()
        except Exception:
            pass
        
if __name__=='__main__':
    if len(sys.argv) ==1:
        print("please provide list of filenames to match as acknowledgement")
        sys.exit(0)
    elif len(sys.argv) > 2:
        if sys.argv[1]=="debug" or sys.argv[1]=="--debug" or sys.argv[1]=="-debug":
            Obj = Acknowledgement_Receiver(files_ack_to_receive=sys.argv[2:])
            Obj.config["debug_mode"] = 1
        else:
            Obj = Acknowledgement_Receiver(files_ack_to_receive=sys.argv[1:])
    else:
        Obj = Acknowledgement_Receiver(files_ack_to_receive=sys.argv[1:])
    threading.Thread(target=Obj.wait_as_ack_receiver_client,args=()).start()
    while Obj.alive and Obj.number_of_files_remaining_ack>0:
        time.sleep(2)
    Obj.alive = False