# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'FirmwareUpdateAutomation.ui'
#
# Created by: PyQt5 UI code generator 5.14.1
#
# WARNING! All changes made in this file will be lost!


from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtWidgets import QApplication, QWidget, QInputDialog, QLineEdit, QFileDialog
import os
import time
import threading
import json
import pandas as pd
import sys

from Device_Configuration import Ui_Device_Configuration_Dialog as Config_Form
from Error_Dialog import Ui_Error_Dialog as Error_Form
from File_Selector import File_Selector
from Calculte_Bytes_To_Upload import Calculte_Bytes_To_Upload
from Acknowledgement_Receiver import Acknowledgement_Receiver
from Firmware_Sender import Firmware_Sender

class Ui_FirmwareUpdaterGUI(object):
    def setupUi(self, FirmwareUpdaterGUI):
        self.thread_objects = []
        self.device_configuration_selected = False
        self.Error_Dialog_Shown = False
        self.selected_device_config = {}
        self.files_to_update = []
        self.basic_firmware_files = []
        self.total_bytes_to_upload = 0
        self.total_bytes_uploaded = 0
        self.total_number_of_files_to_upload = 0
        self.progress_bar_value = 0
        self.progress_thread_alive = False
        self.firmware_updating = False
        self.waiting_for_acknowledgement = False
        self.acknowledged_files = []
        self.previouse_receiver_exist = False
        self.acknowledgement_updater_running = False
        self.basic_firmware_config = None
        self.basic_firmware_deployed = False
        self.basic_firmware_config_file = "basic_firmware_commands.json"
        if len(sys.argv)<2:
            print("please provide database csv file as argument")
            sys.exit(1)
        else:
            self.devices_conf_database=sys.argv[1]
        
        self.load_basic_firmware_commands()
        
        FirmwareUpdaterGUI.setObjectName("FirmwareUpdaterGUI")
        FirmwareUpdaterGUI.resize(320, 320)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Minimum, QtWidgets.QSizePolicy.Minimum)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(FirmwareUpdaterGUI.sizePolicy().hasHeightForWidth())
        FirmwareUpdaterGUI.setSizePolicy(sizePolicy)
        FirmwareUpdaterGUI.setMinimumSize(QtCore.QSize(320, 320))
        FirmwareUpdaterGUI.setMaximumSize(QtCore.QSize(320, 320))
        font = QtGui.QFont()
        font.setPointSize(12)
        font.setBold(False)
        font.setItalic(False)
        font.setWeight(50)
        FirmwareUpdaterGUI.setFont(font)
        FirmwareUpdaterGUI.setMouseTracking(False)
        self.groupBox = QtWidgets.QGroupBox(FirmwareUpdaterGUI)
        self.groupBox.setGeometry(QtCore.QRect(10, -20, 300, 50))
        self.groupBox.setTitle("")
        self.groupBox.setObjectName("groupBox")
        self.BasicFirmware = QtWidgets.QRadioButton(self.groupBox)
        self.BasicFirmware.setGeometry(QtCore.QRect(5, 25, 130, 25))
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Fixed, QtWidgets.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.BasicFirmware.sizePolicy().hasHeightForWidth())
        self.BasicFirmware.setSizePolicy(sizePolicy)
        self.BasicFirmware.setMinimumSize(QtCore.QSize(130, 25))
        self.BasicFirmware.setMaximumSize(QtCore.QSize(130, 25))
        font = QtGui.QFont()
        font.setPointSize(12)
        font.setBold(False)
        font.setItalic(False)
        font.setWeight(50)
        self.BasicFirmware.setFont(font)
        self.BasicFirmware.setObjectName("BasicFirmware")
        self.UpdateFirmware = QtWidgets.QRadioButton(self.groupBox)
        self.UpdateFirmware.setGeometry(QtCore.QRect(145, 25, 145, 25))
        self.UpdateFirmware.setChecked(False)
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Fixed, QtWidgets.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.UpdateFirmware.sizePolicy().hasHeightForWidth())
        self.UpdateFirmware.setSizePolicy(sizePolicy)
        self.UpdateFirmware.setMinimumSize(QtCore.QSize(145, 25))
        self.UpdateFirmware.setMaximumSize(QtCore.QSize(145, 25))
        font = QtGui.QFont()
        font.setPointSize(12)
        font.setBold(False)
        font.setItalic(False)
        font.setWeight(50)
        self.UpdateFirmware.setFont(font)
        self.UpdateFirmware.setObjectName("UpdateFirmware")
        self.domain_view = QtWidgets.QLabel(FirmwareUpdaterGUI)
        self.domain_view.setGeometry(QtCore.QRect(10, 40, 80, 25))
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Fixed, QtWidgets.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.domain_view.sizePolicy().hasHeightForWidth())
        self.domain_view.setSizePolicy(sizePolicy)
        self.domain_view.setMinimumSize(QtCore.QSize(80, 25))
        self.domain_view.setMaximumSize(QtCore.QSize(80, 25))
        font = QtGui.QFont()
        font.setPointSize(12)
        font.setBold(True)
        font.setItalic(False)
        font.setWeight(75)
        self.domain_view.setFont(font)
        self.domain_view.setObjectName("domain_view")
        self.port_View = QtWidgets.QLabel(FirmwareUpdaterGUI)
        self.port_View.setGeometry(QtCore.QRect(10, 80, 80, 25))
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Fixed, QtWidgets.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.port_View.sizePolicy().hasHeightForWidth())
        self.port_View.setSizePolicy(sizePolicy)
        self.port_View.setMinimumSize(QtCore.QSize(80, 25))
        self.port_View.setMaximumSize(QtCore.QSize(80, 25))
        font = QtGui.QFont()
        font.setPointSize(12)
        font.setBold(True)
        font.setItalic(False)
        font.setWeight(75)
        self.port_View.setFont(font)
        self.port_View.setObjectName("port_View")
        self.domain = QtWidgets.QLineEdit(FirmwareUpdaterGUI)
        self.domain.setGeometry(QtCore.QRect(100, 40, 210, 25))
        self.domain.setMinimumSize(QtCore.QSize(210, 25))
        self.domain.setMaximumSize(QtCore.QSize(16777215, 25))
        self.domain.setObjectName("domain")
        self.port = QtWidgets.QLineEdit(FirmwareUpdaterGUI)
        self.port.setGeometry(QtCore.QRect(100, 80, 210, 25))
        self.port.setMinimumSize(QtCore.QSize(210, 25))
        self.port.setMaximumSize(QtCore.QSize(16777215, 25))
        self.port.setObjectName("port")
        self.progress_bar = QtWidgets.QProgressBar(FirmwareUpdaterGUI)
        self.progress_bar.setGeometry(QtCore.QRect(10, 120, 300, 25))
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Fixed, QtWidgets.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.progress_bar.sizePolicy().hasHeightForWidth())
        self.progress_bar.setSizePolicy(sizePolicy)
        self.progress_bar.setMinimumSize(QtCore.QSize(300, 25))
        self.progress_bar.setMaximumSize(QtCore.QSize(300, 25))
        font = QtGui.QFont()
        font.setPointSize(12)
        font.setBold(True)
        font.setItalic(False)
        font.setWeight(75)
        self.progress_bar.setFont(font)
        self.progress_bar.setProperty("value", 0)
        self.progress_bar.setObjectName("progress_bar")
        self.log_view = QtWidgets.QLabel(FirmwareUpdaterGUI)
        self.log_view.setGeometry(QtCore.QRect(10, 200, 300, 120))
        self.log_view.setMinimumSize(QtCore.QSize(300, 120))
        font = QtGui.QFont()
        font.setPointSize(12)
        font.setBold(False)
        font.setItalic(False)
        font.setWeight(50)
        self.log_view.setFont(font)
        self.log_view.setText("")
        self.log_view.setObjectName("log_view")
        self.upload_btn = QtWidgets.QPushButton(FirmwareUpdaterGUI)
        self.upload_btn.setGeometry(QtCore.QRect(230, 160, 80, 30))
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Fixed, QtWidgets.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.upload_btn.sizePolicy().hasHeightForWidth())
        self.upload_btn.setSizePolicy(sizePolicy)
        self.upload_btn.setMinimumSize(QtCore.QSize(80, 30))
        self.upload_btn.setMaximumSize(QtCore.QSize(80, 30))
        font = QtGui.QFont()
        font.setPointSize(14)
        font.setBold(True)
        font.setItalic(False)
        font.setWeight(75)
        self.upload_btn.setFont(font)
        self.upload_btn.setObjectName("upload_btn")
        self.set_device_config_btn = QtWidgets.QPushButton(FirmwareUpdaterGUI)
        self.set_device_config_btn.setGeometry(QtCore.QRect(10, 160, 210, 30))
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Fixed, QtWidgets.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.set_device_config_btn.sizePolicy().hasHeightForWidth())
        self.set_device_config_btn.setSizePolicy(sizePolicy)
        self.set_device_config_btn.setMinimumSize(QtCore.QSize(80, 30))
        self.set_device_config_btn.setMaximumSize(QtCore.QSize(210, 30))
        font = QtGui.QFont()
        font.setPointSize(14)
        font.setBold(True)
        font.setItalic(False)
        font.setWeight(75)
        self.set_device_config_btn.setFont(font)
        self.set_device_config_btn.setObjectName("set_device_config_btn")
        
        self.retranslateUi(FirmwareUpdaterGUI)
        
        self.BasicFirmware.toggled.connect(self.basic_firmware_mode)
        self.UpdateFirmware.toggled.connect(self.update_firmware_mode)
        self.upload_btn.clicked.connect(self.upload_firmware_files)
        self.set_device_config_btn.clicked.connect(self.show_device_configuration_selection)
        self.BasicFirmware.setChecked(True)
        self.log_view.mousePressEvent = self.confirm_deployment
        QtCore.QMetaObject.connectSlotsByName(FirmwareUpdaterGUI)

    def retranslateUi(self, FirmwareUpdaterGUI):
        self._translate = QtCore.QCoreApplication.translate
        FirmwareUpdaterGUI.setWindowTitle(self._translate("FirmwareUpdaterGUI", "WaterAtm_Firmware_Updater"))
        self.BasicFirmware.setText(self._translate("FirmwareUpdaterGUI", "Basic Firmware"))
        self.UpdateFirmware.setText(self._translate("FirmwareUpdaterGUI", "Update Firmware"))
        self.domain_view.setText(self._translate("FirmwareUpdaterGUI", "IP/Domain:"))
        self.port_View.setText(self._translate("FirmwareUpdaterGUI", "Port:"))
        self.upload_btn.setText(self._translate("FirmwareUpdaterGUI", "Upload"))
        self.set_device_config_btn.setText(self._translate("FirmwareUpdaterGUI", "Set Device Config"))
        
    def upload_firmware_files(self):
        if not self.Error_Dialog_Shown and (not self.waiting_for_acknowledgement or not self.firmware_updating):
            if self.device_configuration_selected:
                if self.BasicFirmware.isChecked():
                    print("Uploading basic firmware")
                    self.log_view.setText(self._translate("FirmwareUpdaterGUI","Uploading basic firmware \nwith device id: {}".format(self.selected_device_config['device_id'])))
                    self.initiate_variables()
                    self.progress_thread_alive = True
                    self.handle_progress_bar()
                    Th1 = threading.Thread(target=self.send_basic_firmware_files,args=())
                    self.thread_objects.append(Th1)
                    Th1.start()
                elif self.UpdateFirmware.isChecked():
                    print("File selection")
                    self.openFileNameDialog()
                    if len(self.files_to_update):
                        self.Ack_Receiver_Obj = None
                        self.initiate_variables()
                        print("Sending selected Update device firmware to Device: "+self.selected_device_config["device_id"])
                        total_file_size_calcualtor = Calculte_Bytes_To_Upload(self.files_to_update)
                        file_point = total_file_size_calcualtor.file_point
                        if total_file_size_calcualtor.file_error:
                            print("Error in loading file check file exist or have correct permission")
                            self.show_error_message_dialog("Check File permission","Error! in loading "+file_point)
                        else:
                            self.total_number_of_files_to_upload = len(self.files_to_update)
                            self.total_bytes_to_upload = total_file_size_calcualtor.total_bytes
                            self.progress_thread_alive = True
                            print("Starting progressbar , acknowledgement receiver and file sender thread respectively")
                            Th2 = threading.Thread(target=self.handle_progress_bar,args=())
                            self.thread_objects.append(Th2)
                            Th2.start()
                            if self.previouse_receiver_exist:
                                try:
                                    if self.Ack_Receiver_Obj:
                                        self.Ack_Receiver_Obj.alive = False
                                        del self.Ack_Receiver_Obj
                                        self.previouse_receiver_exist = False
                                except Exception:
                                    pass
                            self.Ack_Receiver_Obj = self.wait_for_acknowledgement()
                            Th3 = threading.Thread(target=self.send_update_firmware_files,args=())
                            self.thread_objects.append(Th3)
                            Th3.start()
                            Th4 = threading.Thread(target=self.handle_acknowledgements,args=())
                            self.thread_objects.append(Th4)
                            Th4.start()                            
            else:
                if not self.Error_Dialog_Shown:
                    self.Error_Dialog_Shown = True
                    self.show_error_message_dialog("Please Select Device !","Error ! Device not selected")
                    self.Error_Dialog_Shown = False
    
    def handle_acknowledgements(self):
        self.previouse_receiver_exist = True
        if self.Ack_Receiver_Obj:
            while self.Ack_Receiver_Obj.alive and self.Ack_Receiver_Obj.number_of_files_remaining_ack>=0:
                temp_progress_bar_value = int(((self.total_number_of_files_to_upload-self.Ack_Receiver_Obj.number_of_files_remaining_ack)/self.total_number_of_files_to_upload)*100)
                if temp_progress_bar_value > self.progress_bar_value:
                    if temp_progress_bar_value >=95:
                        self.progress_bar_value = temp_progress_bar_value
                    #self.progress_bar.setProperty("value", self.progress_bar_value)
                    self.log_view.setText(self._translate("FirmwareUpdaterGUI","R: "+str(self.Ack_Receiver_Obj.Acknowledgement["Received"][-1])+"\n"+
                                          "A: "+str(self.Ack_Receiver_Obj.Acknowledgement["Action"][-1])))
                    self.acknowledged_files.append(self.Ack_Receiver_Obj.Acknowledgement["Received"][-1])
                if self.Ack_Receiver_Obj.number_of_files_remaining_ack==0:
                    break
                time.sleep(0.25)
            #Obj.alive = False
            self.waiting_for_acknowledgement = False
            #time.sleep(2)
            self.previouse_receiver_exist = True
            print("acknowledgement completed")
            threading.Thread(target=self.acknowledgement_message_updater,args=()).start()
        else:
            print("nohunkypunky")  
        time.sleep(3)
        self.progress_thread_alive = False

    def show_error_message_dialog(self,message=None,title=None):
        dialog = QtWidgets.QDialog()
        dialog.ui = Error_Form()
        dialog.ui.setupUi(dialog,message,title)
        dialog.exec_()
        dialog.show()
    
    def acknowledgement_message_updater(self):
        if not self.acknowledgement_updater_running:
            self.acknowledgement_updater_running = True
            print("Starting Acknowledgement updater")
            try:
                while self.Ack_Receiver_Obj.alive:
                    self.log_view.setText(self._translate("FirmwareUpdaterGUI","R: "+str(self.Ack_Receiver_Obj.Acknowledgement["Received"][-1])+"\n"+
                                          "A: "+str(self.Ack_Receiver_Obj.Acknowledgement["Action"][-1])))
                    time.sleep(0.5)
            except Exception:
                pass
            print("Stopping Acknowledgement updater")
            self.acknowledgement_updater_running = False
        
    def basic_firmware_mode(self):
        if self.BasicFirmware.isChecked():
            self.device_configuration_selected = False
            self.domain_view.setText(self._translate("FirmwareUpdaterGUI","SSH IP:"))
            self.domain.setText(self._translate("FirmwareUpdaterGUI","10.130.1.1"))
            self.domain.setEnabled(True)
            self.port_View.setText(self._translate("FirmwareUpdaterGUI","SSH Port:"))
            self.port.setText(self._translate("FirmwareUpdaterGUI","22"))
            self.port.setEnabled(False)
            if not self.Error_Dialog_Shown:
                pass
    
    def update_firmware_mode(self):
        if self.UpdateFirmware.isChecked():
            self.device_configuration_selected = False
            self.domain_view.setText(self._translate("FirmwareUpdaterGUI","Broker IP:"))
            self.domain.setText(self._translate("FirmwareUpdaterGUI","dummybroker.com"))
            self.domain.setEnabled(False)
            self.port_View.setText(self._translate("FirmwareUpdaterGUI","Port:"))
            self.port.setText(self._translate("FirmwareUpdaterGUI","1883"))
            self.port.setEnabled(False)
            if not self.Error_Dialog_Shown:
                pass

    def openFileNameDialog(self):
        ex = File_Selector()
        ex.openFileNamesDialog()
        self.files_to_update = ex.selected_files_to_update
        #self.files_to_update.append("./reboot")
            
    def show_device_configuration_selection(self):
        print("Show device conf dialog")
        dialog = QtWidgets.QDialog()
        dialog.ui = Config_Form()
        dialog.ui.setupUi(dialog,self.devices_conf_database)
        dialog.show()
        dialog.exec_()
        if not dialog.ui.discard_change:
            self.selected_device_config = dialog.ui.config
            self.device_configuration_selected = True
            if self.BasicFirmware.isChecked():
                self.domain_view.setText(self._translate("FirmwareUpdaterGUI","SSH IP:"))
                self.domain.setText(self._translate("FirmwareUpdaterGUI","10.130.1.1"))
                self.domain.setEnabled(True)
                self.port_View.setText(self._translate("FirmwareUpdaterGUI","SSH Port:"))
                self.port.setText(self._translate("FirmwareUpdaterGUI","22"))
                self.port.setEnabled(False)
                self.log_view.setText(self._translate("FirmwareUpdaterGUI","Basic Firmware Device Configuration\nSelected for device : {}".format(self.selected_device_config["device_id"])))
            elif self.UpdateFirmware.isChecked():
                self.domain_view.setText(self._translate("FirmwareUpdaterGUI","Broker IP:"))
                self.domain.setText(self._translate("FirmwareUpdaterGUI",self.selected_device_config["broker"]))
                self.domain.setEnabled(False)
                self.port_View.setText(self._translate("FirmwareUpdaterGUI","Port:"))
                self.port.setText(self._translate("FirmwareUpdaterGUI",self.selected_device_config["port"]))
                self.port.setEnabled(False)
                self.log_view.setText(self._translate("FirmwareUpdaterGUI","Firmware Update Device Configuration\nSelected for device : {}".format(self.selected_device_config["device_id"])))
        else:
            self.device_configuration_selected = False
    
    def __update_deployed_device_database(self):
        Data_Frame = pd.read_csv(self.devices_conf_database)
        mask = Data_Frame["device_id"] == self.selected_device_config["device_id"]
        Targeted_index = Data_Frame[mask].index[0]
        Data_Frame["deployed"][Targeted_index] = 1
        Data_Frame.to_csv(self.devices_conf_database,index=False)
        self.basic_firmware_deployed = False
    
    def confirm_deployment(self, event):
        if self.basic_firmware_deployed:
            if self.device_configuration_selected:    
                self.basic_firmware_deployed = False
                print("Updating on database")
                Th9 = threading.Thread(target=self.__update_deployed_device_database,args=())
                self.thread_objects.append(Th9)
                Th9.start()
                
    
    def wait_for_acknowledgement(self):
        if not self.waiting_for_acknowledgement:
            self.waiting_for_acknowledgement = True
            print("Starting firmware update acknowledgement")
            Obj = Acknowledgement_Receiver(configuration_file="WaterATM_config.json",files_ack_to_receive=self.files_to_update)
            Obj.config["debug_mode"] = 1
            Th5 = threading.Thread(target=Obj.wait_as_ack_receiver_client,args=())
            self.thread_objects.append(Th5)
            Th5.start()
            time.sleep(0.1)
            return Obj
        else:
            print("acknowledgement process already started!!")
            return None

    def send_update_firmware_files(self):
        if not self.firmware_updating:
            print("Starting firmware update")
            self.firmware_updating = True
            Trigger_command = "mosquitto_pub -h \""+str(self.selected_device_config["broker"])+"\" -p "+str(self.selected_device_config["port"])+" -t \""+str(self.selected_device_config["subscribtion_topic"])+str(self.selected_device_config["device_id"])+"\" -m \"FU\"" 
            print("-------------------Firmware Update triggering Bot---------------------")
            print(Trigger_command)
            #os.system(Trigger_command)
            print("           Waiting for the device to get ready to receive !!          ")
            print("----------------------------------------------------------------------")
            #for t in range(1,16):
            #    self.progress_bar_value = t
            #    time.sleep(1)
            
            for file_path in self.files_to_update:
                Sender = Firmware_Sender()
                print(" File to send : "+file_path)
                Sender.load_file(file_path)
                previous_bytes_out = Sender.bytes_out
                time.sleep(0.1)
                Th6 = threading.Thread(target=Sender.start_sending_firmware,args=())
                self.thread_objects.append(Th6)
                Th6.start()
                while(Sender.bytes_out!=Sender.file_size):
                    if Sender.bytes_out > previous_bytes_out:
                        print("bytes sent : "+str(Sender.bytes_out))
                        print("File size  : "+str(Sender.file_size))
                        self.total_bytes_uploaded += (Sender.bytes_out-previous_bytes_out)
                        print("total_bytes_uploaded *:"+str(self.total_bytes_uploaded))
                        previous_bytes_out = Sender.bytes_out
                        self.progress_bar_value = int((self.total_bytes_uploaded/self.total_bytes_to_upload)*100)-5
                    #self.progress_bar.setProperty("value", self.progress_bar_value)
                    time.sleep(0.25)
                #os.system("python3 Firmware_Sender.py debug "+file_path)            
                
                try:
                    #while os.path.basename(file_path) not in self.acknowledged_files:
                    while os.path.basename(file_path) not in self.Ack_Receiver_Obj.Acknowledgement["Received"]:
                        time.sleep(1)
                        print("waiting for "+os.path.basename(file_path)+" acknowledgement")
                except Exception:
                    print("exiting from ack wait")
                    
            print("update firmware files sent")
            self.firmware_updating = False
            #time.sleep(2)
            #self.progress_thread_alive = False
        else:
            print("update firmware file sending process already started!!")
            
    def send_basic_firmware_files(self):
        if self.basic_firmware_config:
            self.basic_firmware_deployed = False
            print("Uploading basic firmware")
            self.basic_firmware_config["host"] = self.domain.text()
            print("-----------------ssh key generation Bot----------------------")
            command = self.basic_firmware_config["ssh_base_conf"]+"\""+str(self.basic_firmware_config["host"])+"\""
            print(command)
            os.system(command)
            print("-------------------------------------------------------------")
            self.progress_bar_value = 0
            firmware_file_paths = ["/root/","/etc/","/etc/config/","/etc/init.d/"]
            Destinations_and_files = self.basic_firmware_config["destinations_and_files"]
            ssh_login_base = str(self.basic_firmware_config["user"])+"@"+str(self.basic_firmware_config["host"])
            for file_path in firmware_file_paths:
                Path = Destinations_and_files[file_path]
                File_names = list(Path.keys())
                Command_string = str(self.basic_firmware_config["file_copier"])
                for file_name in File_names:
                    Command_string += file_name+" "
                Command_string += ssh_login_base+":"+file_path 
                try:
                    print("_______________________SCP Bot_______________________")
                    print(Command_string)
                    os.system(Command_string)
                    print("_____________________________________________________")
                    
                except Exception as eFileSend:
                    print("Unable to send file")
                    print("Firmware upload failed")
                    print(eFileSend)
                    self.progress_bar_value = 0
                    self.progress_thread_alive = False
                    return
                self.progress_bar_value += 5
                for file_name in File_names:
                    ssh_command = "echo \"chmod "+Path[file_name]+" "+file_path+file_name+"\" | "+self.basic_firmware_config["command_over_ssh"]+ssh_login_base
                    try:
                        print("--------------------Chmod Bot--------------------")
                        print(ssh_command)
                        os.system(ssh_command)
                        print("-------------------------------------------------")
                        
                        self.progress_bar_value += 7
                    except Exception as eChmod:
                        print("Unable to set file mode for : "+file_name)
                        print(eChmod)
                        continue
                    
                for file in Destinations_and_files["enable"]: 
                    if file in File_names:
                        command = "echo \""+file_path+file+" enable\" | "+self.basic_firmware_config["command_over_ssh"]+ssh_login_base
                        print("____________________Enabler Bot__________________")
                        print(command)
                        os.system(command)
                        print("_________________________________________________")
                        
                        self.progress_bar_value += 7
            for hex_file in Destinations_and_files["run-avrdude"]:
                print("--------------------- AVR-dude Bot-----------------------")
                command = "echo \"/usr/bin/run-avrdude "+str(hex_file)+" 1\" | "+self.basic_firmware_config["command_over_ssh"]+ssh_login_base
                print(command)
                os.system(command)
                print("---------------------------------------------------------")
                
            print("_________________________Reboot Bot__________________________")
            command = "echo \"reboot\" | "+self.basic_firmware_config["command_over_ssh"]+ssh_login_base
            print(command)
            os.system(command)
            print("_____________________________________________________________")
            
            self.progress_bar_value = 100
            self.progress_thread_alive = False
            self.basic_firmware_deployed = True
            time.sleep(2)
        else:
            print("No basic firmware config loaded")
            self.progress_thread_alive = False
        
    def load_basic_firmware_commands(self):
        with open(self.basic_firmware_config_file,"r") as fp:
            self.basic_firmware_config = json.load(fp)
    
    def initiate_variables(self):
        print("Initiating variables")
        self.basic_firmware_files = []
        self.total_bytes_to_upload = 0
        self.total_bytes_uploaded = 0
        self.total_number_of_files_to_upload = 0
        self.progress_bar_value = 0
        self.progress_thread_alive = False
        self.firmware_updating = False
        self.waiting_for_acknowledgement = False
        self.acknowledged_files = []
        
    def handle_progress_bar(self,interval=0.5):
        if self.progress_thread_alive:
            #print("checking update progress")
            self.progress_bar.setProperty("value", self.progress_bar_value)
            Th7 = threading.Timer(interval,self.handle_progress_bar,[interval])
            Th7.start()
        else:
            time.sleep(1)
            self.log_view.setText(self._translate("FirmwareUpdaterGUI","Last Update Was {}% Successful\nClick here to update Database".format(self.progress_bar_value)))
            self.progress_bar_value = 0
            self.progress_bar.setProperty("value", self.progress_bar_value)
            #if self.progress_bar_value == 100:
                #try:
                #    self.Ack_Receiver_Obj.alive = False
                #except Exception :
                #    pass
           #     for i in range(len(self.thread_objects)):
           #         try:
           #             del self.thread_objects[i]
           #         except Exception:
           #             pass
           #     print("Threads cleaned")
            
            print("Resetting progress bar")
            
    def __del__(self):
        self.progress_thread_alive = False
        killed = 0
        try:
            self.Ack_Receiver_Obj.alive = False
        except Exception :
            pass
        for i in range(len(self.thread_objects)):
            try:
                del self.thread_objects[i]
                killed += 1
            except Exception:
                pass

        if self.previouse_receiver_exist:
            try:
                if self.Ack_Receiver_Obj:
                    self.Ack_Receiver_Obj.alive = False
                    del self.Ack_Receiver_Obj
                    killed += 1
                    self.previouse_receiver_exist = False
            except Exception:
                pass
        print("killed {} possible objects".format(killed))
        
        
        
if __name__ == "__main__":
    app = QtWidgets.QApplication(sys.argv)
    FirmwareUpdaterGUI = QtWidgets.QWidget()
    ui = Ui_FirmwareUpdaterGUI()
    ui.setupUi(FirmwareUpdaterGUI)
    try:
        FirmwareUpdaterGUI.show()
        return_code = app.exec_()
    except KeyboardInterrupt:
        pass
    try:
        ui.__del__()
        app.__delattr__()
        del ui
        del app
        print("quiting gui")
    except Exception:
        print("quiting erro")
        
    sys.exit(return_code)
