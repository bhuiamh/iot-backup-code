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

import time
import paho.mqtt.client as paho
import hashlib
import json
import sys
import os

class Firmware_Sender(object):
    def __init__(self,configuration_file="WaterATM_config.json"):
        self.load_configuration(configuration_file)
        self.Run_flag=True
        self.qos = 1
        self.bytes_out=0
        self.data_block_size=2000
        self.out_hash_md5 = hashlib.md5()
        self.in_hash_md5 = hashlib.md5()
        self.client= paho.Client("client-transmit-for-"+self.config["device_id"])
        ######
        #client.on_message=on_message
        self.client.on_publish = self.on_publish
        self.file_size = 0
        self.unable_to_connect = True
        
    
    def load_file(self, filename):
        self.filename = filename
        try:
            self.fo=open(self.filename,"rb")
            self.file_size = len(self.fo.read())
            self.fo.close()
            self.fo=open(self.filename,"rb")
        except Exception as eOpen:
            print("Unable to load file : ", self.filename)
            print(eOpen)
            sys.exit(0)
        self.client.puback_flag=False #use flag in publish ack
        self.client.mid_value=None
        #####
        self.bytes_out=0
        if self.config["debug_mode"]:
            print("connecting to broker ",self.config["broker"])
        try:
            self.client.connect(self.config["broker"])#connect
            self.client.loop_start() #start loop to process received messages
            self.unable_to_connect = False
        except Exception as eBrokerConnection:
            self.unable_to_connect = True
            print("Uanble to connect to broker")
            print(eBrokerConnection)
            
        if self.config["debug_mode"]:
            print("subscribing ")
        self.topic = "FU/"+self.config["device_id"]+"/"+self.config["password"]
        self.client.subscribe(self.topic)#subscribe
        if self.config["debug_mode"]:
            print("file Loaded")
        time.sleep(0.25)

    def load_configuration(self, configuration_file):
        with open(configuration_file,"r") as conf:
            self.config = json.load(conf)
          
    def process_message(self, msg):
       """ This is the main receiver code
       """
       if self.config["debug_mode"]:
           print("received ")
       if len(msg)==200: #is header or end
          msg_in=msg.decode("utf-8")
          msg_in=msg_in.split(",,")
          if msg_in[0]=="end": #is it really last packet?
             in_hash_final=self.in_hash_md5.hexdigest()
             if in_hash_final==msg_in[2]:
                print("File copied OK -valid hash  ",in_hash_final)
             else:
                print("Bad file receive   ",in_hash_final)
             return False
          else:
             if msg_in[0]!="header":
                self.in_hash_md5.update(msg)
                return True
             else:
                return False
       else:
          self.in_hash_md5.update(msg)
          return True
    
    def on_message(self, client, userdata, message):
       #time.sleep(1)
       #print("received message =",str(message.payload.decode("utf-8")))
       if self.process_message(message.payload):
          pass
          #fout.write(message.payload)
    
    def on_publish(self, client, userdata, mid):
        #logging.debug("pub ack "+ str(mid))
        client.mid_value=mid
        client.puback_flag=True  
    
    ## waitfor loop
    def wait_for(self, client,msgType,period=0.25,wait_time=40,running_loop=False):
        client.running_loop=running_loop #if using external loop
        wcount=0
        #return True
        while True:
            #print("waiting"+ msgType)
            if msgType=="PUBACK":
                if client.on_publish:        
                    if client.puback_flag:
                        return True
         
            if not client.running_loop:
                client.loop(.01)  #check for messages manually
            time.sleep(period)
            #print("loop flag ",client.running_loop)
            wcount+=1
            if wcount>wait_time:
                print("return from wait loop taken too long")
                return False
        return True 
    
    def send_header(self):
       just_file_name = os.path.basename(self.filename)
       if self.config["debug_mode"]:
           print("just file name : "+just_file_name)
       file_data={"filename":just_file_name}
       file_data_json=json.dumps(file_data)
       header="header"+",,"+file_data_json + ",,"
       header=bytearray(header,"utf-8")
       header.extend(b'x'*(200-len(header)))
       if self.config["debug_mode"]:
           print(header)
       self.c_publish(header)
    
    def send_end(self):
       just_file_name = os.path.basename(self.filename)
       end="end"+",,"+just_file_name+",,"+self.out_hash_md5.hexdigest()+",,"
       end=bytearray(end,"utf-8")
       end.extend(b'x'*(200-len(end)))
       if self.config["debug_mode"]:
           print(end)
       self.c_publish(end)
    
    def c_publish(self,out_message):
       res,mid=self.client.publish(self.topic,out_message,self.qos)#publish
       #return
    
       if res==0: #published ok
          if self.wait_for(self.client,"PUBACK",running_loop=True):
             if mid==self.client.mid_value:
                if self.config["debug_mode"]:
                    print("match mid ",str(mid))
                self.client.puback_flag=False #reset flag
             else:
                print("quitting")
                #raise SystemExit("not got correct puback mid so quitting")
             
          else:
             print("Quitting") 
             #raise SystemExit("not got puback so quitting")
    
    def start_sending_firmware(self):
        self.client.puback_flag=False
        start=time.time()
        if self.config["debug_mode"]:
            print("publishing ")
        self.send_header()
        self.Run_flag=True
        self.count=0
        self.out_hash_md5 = hashlib.md5()
        self.bytes_out=0
        
        while self.Run_flag:
           chunk=self.fo.read(self.data_block_size)
           if chunk:
              self.out_hash_md5.update(chunk) #update hash
              out_message=chunk
              #print(" length =",type(out_message))
              self.bytes_out=self.bytes_out+len(out_message)
        
              self.c_publish(out_message)
        
                 
           else:
              #end of file so send hash
              out_message=self.out_hash_md5.hexdigest()
              self.send_end()
              self.Run_flag=False
        time_taken=time.time()-start
        print("took ",time_taken)
        print("bytes sent from sender =",self.bytes_out)
        self.fo.close()
        self.client.disconnect() #disconnect
        time.sleep(0.2)
    
    def __del__(self):
        pass
    
if __name__=='__main__':
    if len(sys.argv)<2:
        print("print please provide one or more filenames or path")
        sys.exit(0)
    else:
        #print(sys.argv)
        Sender = Firmware_Sender()
        if len(sys.argv)>=3:
            if sys.argv[1]=="debug" or sys.argv[1]=="--debug" or sys.argv[1]=="-debug":
                fileNames = sys.argv[2:]
                Sender.config["debug_mode"] = 1
            else:
                fileNames = sys.argv[1:]
        else:
            fileNames = sys.argv[1:]

        for file in fileNames:
            Sender.load_file(file)
            Sender.start_sending_firmware()