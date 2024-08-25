# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'Device_Configuration.ui'
#
# Created by: PyQt5 UI code generator 5.14.1
#
# WARNING! All changes made in this file will be lost!


from PyQt5 import QtCore, QtGui, QtWidgets
import pandas as pd
from network_file_generator import network_conf_generator as Net_Conf_Gen
import json

class Ui_Device_Configuration_Dialog(object):
    def setupUi(self, Dialog,devices_conf_database="W_ATM_Device_Record.csv",individual_device_Conf="WaterATM_config.json"):
        self.Dialog = Dialog
        self.discard_change = True
        self.show_deployed_devices_config = True
        self.devices_conf_database = devices_conf_database
        self.individual_device_conf = individual_device_Conf
        try:
            with open(self.individual_device_conf,"r") as fp:
                self.config= json.load(fp)
        except Exception as eConfigLoad:
            print("Unable to load configuration")
            print(eConfigLoad)
            exit(0)
        self.devices_dataframe_base = pd.read_csv(self.devices_conf_database)
        
        self.Dialog.setObjectName("Dialog")
        self.Dialog.resize(400, 300)
        font = QtGui.QFont()
        font.setPointSize(12)
        self.Dialog.setFont(font)
        self.dialog_box = QtWidgets.QDialogButtonBox(self.Dialog)
        self.dialog_box.setGeometry(QtCore.QRect(310, 220, 81, 60))
        self.dialog_box.setOrientation(QtCore.Qt.Vertical)
        self.dialog_box.setStandardButtons(QtWidgets.QDialogButtonBox.Cancel|QtWidgets.QDialogButtonBox.Ok)
        self.dialog_box.setObjectName("dialog_box")
        self.broker_view = QtWidgets.QLabel(self.Dialog)
        self.broker_view.setGeometry(QtCore.QRect(10, 10, 60, 25))
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Fixed, QtWidgets.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.broker_view.sizePolicy().hasHeightForWidth())
        self.broker_view.setSizePolicy(sizePolicy)
        self.broker_view.setMinimumSize(QtCore.QSize(60, 25))
        self.broker_view.setMaximumSize(QtCore.QSize(60, 25))
        font = QtGui.QFont()
        font.setPointSize(12)
        font.setBold(True)
        font.setWeight(75)
        self.broker_view.setFont(font)
        self.broker_view.setObjectName("broker_view")
        self.broker_port_view = QtWidgets.QLabel(self.Dialog)
        self.broker_port_view.setGeometry(QtCore.QRect(10, 40, 60, 25))
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Fixed, QtWidgets.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.broker_port_view.sizePolicy().hasHeightForWidth())
        self.broker_port_view.setSizePolicy(sizePolicy)
        self.broker_port_view.setMinimumSize(QtCore.QSize(60, 25))
        self.broker_port_view.setMaximumSize(QtCore.QSize(60, 25))
        font = QtGui.QFont()
        font.setPointSize(12)
        font.setBold(True)
        font.setWeight(75)
        self.broker_port_view.setFont(font)
        self.broker_port_view.setObjectName("broker_port_view")
        self.password_view = QtWidgets.QLabel(self.Dialog)
        self.password_view.setGeometry(QtCore.QRect(10, 70, 75, 25))
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Fixed, QtWidgets.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.password_view.sizePolicy().hasHeightForWidth())
        self.password_view.setSizePolicy(sizePolicy)
        self.password_view.setMinimumSize(QtCore.QSize(60, 25))
        self.password_view.setMaximumSize(QtCore.QSize(75, 25))
        font = QtGui.QFont()
        font.setPointSize(12)
        font.setBold(True)
        font.setWeight(75)
        self.password_view.setFont(font)
        self.password_view.setObjectName("password_view")
        self.device_id_view = QtWidgets.QLabel(self.Dialog)
        self.device_id_view.setGeometry(QtCore.QRect(10, 100, 75, 25))
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Fixed, QtWidgets.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.device_id_view.sizePolicy().hasHeightForWidth())
        self.device_id_view.setSizePolicy(sizePolicy)
        self.device_id_view.setMinimumSize(QtCore.QSize(60, 25))
        self.device_id_view.setMaximumSize(QtCore.QSize(75, 25))
        font = QtGui.QFont()
        font.setPointSize(12)
        font.setBold(True)
        font.setWeight(75)
        self.device_id_view.setFont(font)
        self.device_id_view.setObjectName("device_id_view")
        self.publish_topic_view = QtWidgets.QLabel(self.Dialog)
        self.publish_topic_view.setGeometry(QtCore.QRect(10, 130, 85, 25))
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Fixed, QtWidgets.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.publish_topic_view.sizePolicy().hasHeightForWidth())
        self.publish_topic_view.setSizePolicy(sizePolicy)
        self.publish_topic_view.setMinimumSize(QtCore.QSize(60, 25))
        self.publish_topic_view.setMaximumSize(QtCore.QSize(85, 25))
        font = QtGui.QFont()
        font.setPointSize(12)
        font.setBold(True)
        font.setWeight(75)
        self.publish_topic_view.setFont(font)
        self.publish_topic_view.setObjectName("publish_topic_view")
        self.subscribtion_topic_view = QtWidgets.QLabel(self.Dialog)
        self.subscribtion_topic_view.setGeometry(QtCore.QRect(10, 160, 85, 25))
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Fixed, QtWidgets.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.subscribtion_topic_view.sizePolicy().hasHeightForWidth())
        self.subscribtion_topic_view.setSizePolicy(sizePolicy)
        self.subscribtion_topic_view.setMinimumSize(QtCore.QSize(60, 25))
        self.subscribtion_topic_view.setMaximumSize(QtCore.QSize(85, 25))
        font = QtGui.QFont()
        font.setPointSize(12)
        font.setBold(True)
        font.setWeight(75)
        self.subscribtion_topic_view.setFont(font)
        self.subscribtion_topic_view.setObjectName("subscribtion_topic_view")
        self.cost_of_water_view = QtWidgets.QLabel(self.Dialog)
        self.cost_of_water_view.setGeometry(QtCore.QRect(10, 190, 110, 25))
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Fixed, QtWidgets.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.cost_of_water_view.sizePolicy().hasHeightForWidth())
        self.cost_of_water_view.setSizePolicy(sizePolicy)
        self.cost_of_water_view.setMinimumSize(QtCore.QSize(60, 25))
        self.cost_of_water_view.setMaximumSize(QtCore.QSize(110, 25))
        font = QtGui.QFont()
        font.setPointSize(12)
        font.setBold(True)
        font.setWeight(75)
        self.cost_of_water_view.setFont(font)
        self.cost_of_water_view.setObjectName("cost_of_water_view")
        self.broker = QtWidgets.QLineEdit(self.Dialog)
        self.broker.setGeometry(QtCore.QRect(120, 10, 270, 25))
        self.broker.setObjectName("broker")
        self.broker_port = QtWidgets.QLineEdit(self.Dialog)
        self.broker_port.setGeometry(QtCore.QRect(120, 40, 270, 25))
        self.broker_port.setObjectName("broker_port")
        self.password = QtWidgets.QLineEdit(self.Dialog)
        self.password.setGeometry(QtCore.QRect(120, 70, 270, 25))
        self.password.setObjectName("password")
        self.device_id = QtWidgets.QComboBox(self.Dialog)
        self.device_id.setGeometry(QtCore.QRect(120, 100, 270, 25))
        self.device_id.setObjectName("device_id")
        self.publish_topic = QtWidgets.QLineEdit(Dialog)
        self.publish_topic.setGeometry(QtCore.QRect(120, 130, 270, 25))
        self.publish_topic.setObjectName("publish_topic")
        self.subcribtion_topic = QtWidgets.QLineEdit(self.Dialog)
        self.subcribtion_topic.setGeometry(QtCore.QRect(120, 160, 270, 25))
        self.subcribtion_topic.setObjectName("subcribtion_topic")
        self.cost_of_water = QtWidgets.QLineEdit(self.Dialog)
        self.cost_of_water.setGeometry(QtCore.QRect(120, 190, 270, 25))
        self.cost_of_water.setObjectName("cost_of_water")
        self.groupBox = QtWidgets.QGroupBox(Dialog)
        self.groupBox.setGeometry(QtCore.QRect(10, 220, 295, 60))
        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.Fixed, QtWidgets.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.groupBox.sizePolicy().hasHeightForWidth())
        self.groupBox.setSizePolicy(sizePolicy)
        self.groupBox.setMinimumSize(QtCore.QSize(295, 60))
        self.groupBox.setMaximumSize(QtCore.QSize(295, 60))
        self.groupBox.setObjectName("groupBox")
        self.deployed_devices_selected = QtWidgets.QRadioButton(self.groupBox)
        self.deployed_devices_selected.setGeometry(QtCore.QRect(10, 30, 150, 25))
        self.deployed_devices_selected.setChecked(True)
        self.deployed_devices_selected.setObjectName("deployed_devices_selected")
        self.new_devices_selected = QtWidgets.QRadioButton(self.groupBox)
        self.new_devices_selected.setGeometry(QtCore.QRect(180, 30, 110, 25))
        self.new_devices_selected.setObjectName("new_devices_selected")
        
        self.retranslateUi()
        
        self.dialog_box.accepted.connect(self.on_accept)
        self.dialog_box.rejected.connect(self.on_reject)
        self.broker.editingFinished.connect(self.make_temp_change_on_record)
        self.broker_port.editingFinished.connect(self.make_temp_change_on_record)
        self.password.editingFinished.connect(self.make_temp_change_on_record)
        self.device_id.currentIndexChanged['QString'].connect(self.change_device_details)
        self.publish_topic.editingFinished.connect(self.make_temp_change_on_record)
        self.subcribtion_topic.editingFinished.connect(self.make_temp_change_on_record)
        self.cost_of_water.editingFinished.connect(self.make_temp_change_on_record)
        self.Dialog.closeEvent = self.closeEvent
        self.deployed_devices_selected.toggled.connect(self.toggole_devices)
        self.new_devices_selected.toggled.connect(self.toggole_devices)
        
        QtCore.QMetaObject.connectSlotsByName(self.Dialog)
        
        self.load_device_record()
        
    def retranslateUi(self):
        _translate = QtCore.QCoreApplication.translate
        self.Dialog.setWindowTitle(_translate("Dialog", "Device_Configuration"))
        self.broker_view.setText(_translate("Dialog", "Broker:"))
        self.broker_port_view.setText(_translate("Dialog", "Port:"))
        self.password_view.setText(_translate("Dialog", "Password:"))
        self.device_id_view.setText(_translate("Dialog", "Device ID:"))
        self.publish_topic_view.setText(_translate("Dialog", "Pub_Topic:"))
        self.subscribtion_topic_view.setText(_translate("Dialog", "Sub_Topic:"))
        self.cost_of_water_view.setText(_translate("Dialog", "Cost of Water:"))
        self.groupBox.setTitle(_translate("Dialog", " Device Type"))
        self.deployed_devices_selected.setText(_translate("Dialog", "Deployed Devices"))
        self.new_devices_selected.setText(_translate("Dialog", "New Device"))

    def toggole_devices(self):
        if self.deployed_devices_selected.isChecked():
            self.show_deployed_devices_config = True
        elif self.new_devices_selected.isChecked():
            self.show_deployed_devices_config = False
        self.load_device_record()
    
    def load_device_record(self):
        Devices_Index = self.devices_dataframe_base["deployed"]==self.show_deployed_devices_config
        self.devices_dataframe = self.devices_dataframe_base[Devices_Index]
        self.devices_index = self.devices_dataframe.index
        self.device_id.clear()
        if self.new_devices_selected.isChecked():
            for last_index in (self.devices_index):#,-1,-1):
                self.device_id.addItem(self.devices_dataframe["device_id"][last_index])
                break
        else:
            for last_index in (self.devices_index):#,-1,-1):
                self.device_id.addItem(self.devices_dataframe["device_id"][last_index])
        self.change_device_details()
        self.password.setEnabled(False)

    def change_device_details(self):
        try:
            mask = self.devices_dataframe["device_id"] == self.device_id.currentText()
            temp_k = self.devices_dataframe[mask]
            self.current_device_index = temp_k.index[0]#len(self.devices_dataframe)-1-self.device_id.currentIndex()
        except Exception as eValueSet:
            #print(eValueSet)
            return
        #print("current device index: "+str(self.current_device_index))
        self.broker.setText(str(self.devices_dataframe["broker"][self.current_device_index]))
        self.broker_port.setText(str(self.devices_dataframe["port"][self.current_device_index]))
        self.password.setText(str(self.devices_dataframe["password"][self.current_device_index]))
        self.publish_topic.setText(str(self.devices_dataframe["publish_topic"][self.current_device_index]))
        self.subcribtion_topic.setText(str(self.devices_dataframe["subscribtion_topic"][self.current_device_index]))
        self.cost_of_water.setText(str(self.devices_dataframe["cost_of_water"][self.current_device_index]))
        self.make_temp_change_on_record()
        
    def save_device_record(self):
        return
        #if not self.discard_change:
        #    print("saving to : "+self.devices_conf_database)
        #    try:
        #        self.devices_dataframe.to_csv(self.devices_conf_database)
        #    except Exception as eDevicesDataFrameWrite:
        #        print("unable to write to devices databasse")
        #        print(eDevicesDataFrameWrite)
        #        raise Exception

    def make_temp_change_on_record(self):
        #print("hello temp change")
        self.config["device_id"] = self.device_id.currentText()
        self.config["broker"] = self.broker.text()
        self.config["port"] = self.broker_port.text()
        self.config["password"] = self.password.text()
        self.config["publish_topic"] = self.publish_topic.text()
        self.config["subscribtion_topic"] =self.subcribtion_topic.text()
        self.config["costOfWater"] = self.cost_of_water.text()
    
    def generate_files(self):
        print("Generating firmware files")
        if self.discard_change:
            self.discard_changes()
        else:
            self.save_changes()
    
    def closeEvent(self, event):
        self.generate_files()
    
    def on_accept(self):
        print("accepted")
        self.discard_change = False
        self.Dialog.close()
        return 0
    
    def discard_changes(self):
        print("changes_discarded")
        self.discard_change = True
        
    def save_changes(self):
        print("saving changes")
        if not self.discard_change:
            try:
                Net_Conf_Gen(self.config["device_id"])
                Pretty_Json_String = json.dumps(self.config,indent=1)
                with open(self.individual_device_conf,"w") as fp:
                    fp.write(Pretty_Json_String)
                self.save_device_record()
            except Exception as eConfGen:
                print("Unable to generate new configurtion files")
                print(eConfGen)
                exit(0)
            
    def on_reject(self):
        print("rejected")
        self.discard_change = True
        self.Dialog.close()
        return -1

#if __name__ == "__main__":
#    import sys
#    app = QtWidgets.QApplication(sys.argv)
#    Dialog = QtWidgets.QDialog()
#    ui = Ui_Device_Configuration_Dialog()
#    ui.setupUi(Dialog)
#    k = Dialog.show()
#    ar = app.exec_()
#    sys.exit()
