import time
import paho.mqtt.client as paho
import hashlib
import json
import sys
import os

class Firmware_Receiver_Over_MQTT(object):
    def __init__(self,configuration_file="/root/WaterATM_config.json"):
        self.Pid = str(os.getpid())
        print("Process ID: "+self.Pid)
        self.load_configuration(configuration_file)
        self.run_flag = True
        self.new_file_success = False
        self.bytes_in=0
        self.in_hash_md5 = hashlib.md5()
        self.client= paho.Client("client-receive-"+self.config["device_id"])
        self.client.on_message= self.on_message
        self.client.on_connect = self.on_connect
        self.client.mid_value=None
        self.client.connect(self.config["broker"])
        self.topic = "FU/"+self.config["device_id"]+"/"+self.config["password"]
        self.publish_topic = self.topic+"/reply"
        print("Reply topic: "+self.publish_topic)
        self.publish_string = "mosquitto_pub -h "+self.config["broker"]+" -p "+str(self.config["port"])+" -t \""+self.publish_topic+"\""+" -m "
        print("publish_string: "+self.publish_string)
        self.client.subscribe(self.topic)
        self.output_filename = ""       
    def load_configuration(self,configuration_file):
        with open(configuration_file,"r") as conf:
            self.config = json.load(conf)
    def on_connect(self, client, userdata, flags, rc):
        if self.config["debug_mode"]:
            print("Connected to broker : ",self.config["broker"])
    def on_message(self, client, userdata, message):
       if self.process_message(message.payload):
          try:
              self.fout.write(message.payload)
          except Exception as eWrite:
              print("unable to write to file")
              if self.config["debug_mode"]:
                  print("Received message: " +message.payload)
              print(eWrite)
    def extract_file_data(self, file_data):
       data=json.loads(file_data)
       filename=data["filename"]
       return filename
    def process_message(self, msg):    
       if len(msg)==200:
          msg_in=msg.decode("utf-8","ignore")
          msg_in=msg_in.split(",,")
          if msg_in[0]=="header": #header
             filename=self.extract_file_data(msg_in[1])
             file_out=filename
             self.output_filename = filename
             self.fout=open(file_out+"_fail_safe","wb") 
             self.new_file_success = False
          if msg_in[0]=="end": #is it really last packet?
             in_hash_final=self.in_hash_md5.hexdigest()
             if in_hash_final==msg_in[2]:
                self.new_file_success = True
             else:
                self.run_flag = False
             return False
          else:
             if msg_in[0]!="header":
                self.in_hash_md5.update(msg)
                return True
             else:
                return False
       else:
          self.in_hash_md5.update(msg)
          if len(msg) <100 and self.config["debug_mode"]:
             print(msg)
          return True
    def wait_for_firmware_arival(self):
        time.sleep(1)
        while True:
            self.bytes_in=0
            start=time.time()
            self.time_taken=time.time()-start
            self.in_hash_md5 = hashlib.md5()
            self.run_flag=True
            self.client.reconnect()
            self.client.subscribe(self.topic)
            #self.client.loop_start()
            while self.run_flag:
               self.client.loop(10)
               if self.new_file_success:
                   self.fout.close()
                   os.system(self.publish_string+"{\\\"Received\\\":\\\""+self.output_filename+"\\\"}")
                   time.sleep(0.1)
                   os.system("mv ./"+self.output_filename+"_fail_safe"+" ./"+self.output_filename)
                   if self.config["run-avrdude"]==self.output_filename[-4:]:
                       os.system(self.publish_string+"{\\\"Action\\\":\\\"executing_run-avrdude_"+self.output_filename+"_1\\\"}")
                       time.sleep(0.1)
                       os.system("/usr/bin/run-avrdude "+self.output_filename+" 1")
                       os.system(self.publish_string+"{\\\"Action\\\":\\\"hexfile_burned_successfully\\\"}")
                       time.sleep(0.1)
                   if self.output_filename in self.config["file_name_plase"].keys():
                       os.system("mv ./"+self.output_filename+" "+str(self.config["file_name_plase"][self.output_filename])+self.output_filename)
                       os.system(self.publish_string+"{\\\"Action\\\":\\\"File_"+self.output_filename+"_Moved_to_:_"+str(self.config["file_name_plase"][self.output_filename])+self.output_filename+"\\\"}")
                       time.sleep(0.1)
                   else:
                       try:
                           os.system("mv ./"+self.output_filename+" /root/"+self.output_filename)
                       except Exception as eMove:
                           print("Unable to move file")
                           print(eMove)
                       os.system(self.publish_string+"{\\\"Action\\\":\\\"File_"+self.output_filename+"_Moved_to_:_/root/"+self.output_filename+"\\\"}")
                       time.sleep(0.1)
                   if self.output_filename in self.config["file_name_chmod"].keys():
                       if self.output_filename in self.config["file_name_plase"].keys():
                           os.system("chmod "+str(self.config["file_name_chmod"][self.output_filename])+" "+str(self.config["file_name_plase"][self.output_filename])+self.output_filename)
                       else:
                           os.system("chmod "+str(self.config["file_name_chmod"][self.output_filename])+" /root/"+self.output_filename)
                       os.system(self.publish_string+"{\\\"Action\\\":\\\"File_Mode_for_"+self.output_filename+"_changed_to_"+str(self.config["file_name_chmod"][self.output_filename])+"\\\"}")
                       time.sleep(0.1)
                   if self.output_filename in self.config["routine_file_to_enable"].keys():
                       if int(str(self.config["routine_file_to_enable"][self.output_filename])):
                           if self.output_filename in self.config["file_name_plase"].keys():
                               os.system(str(self.config["file_name_plase"][self.output_filename])+self.output_filename+" enable")
                           else:
                               os.system("/root/"+self.output_filename+" enable")
                           os.system(self.publish_string+"{\\\"Action\\\":\\\"enabling_"+self.output_filename+"\\\"}")
                   if self.output_filename in self.config["reboot_on_file_arrive"]:
                       os.system(self.publish_string+"{\\\"Action\\\":\\\"System_Rebooting\\\"}")
                       time.sleep(0.1)
                       os.system("reboot")
                       os.system("kill "+self.Pid)
                   self.new_file_success = False
                   self.output_filename = ""
                   self.run_flag = False
                   break
    def __del__(self):
        pass
if __name__=='__main__':
    try:
        Firmware_Updater =  Firmware_Receiver_Over_MQTT()
        if len(sys.argv)==2:
            if sys.argv[1]=="debug" or sys.argv[1]=="-debug" or sys.argv[1]=="--debug":
                Firmware_Updater.config["debug_mode"] = 1
        Firmware_Updater.wait_for_firmware_arival()
    except KeyboardInterrupt:
        print("Quiting by Keyboard Interrupt")
    
