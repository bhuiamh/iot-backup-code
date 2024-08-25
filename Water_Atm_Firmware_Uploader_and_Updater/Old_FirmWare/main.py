import serial
import time
import paho.mqtt.publish as publish
import paho.mqtt.subscribe as subscribe
import paho.mqtt.client as mqtt
import json
import os
import socket
from threading import Thread

Pid=str(os.getpid())
print(Pid)

def load_config(file_name="/root/WaterATM_config.json"):
    with open(file_name,"r") as fp:
        config = json.load(fp)
    return config

config = load_config()

debug_mode = config["debug_mode"]
old_topic = config["old_topic"]

serialReceiveFlag = False
serialPort = "/dev/ttyATH0"

# Variable to store broker response message
brokerMessage = ""

# Broker Timestamp
broker_timestamp = 0


# ATM ID
device_id = str(config["device_id"])
if(old_topic):
	config["publish_topic_verify"] = "request"
	config["publish_topic_dispense"] = "request"
	subscription_topic = device_id
else:
	subscription_topic = str(config["subscribtion_topic"]) + device_id

# Water cost per litre
costOfWater = float(str(config["costOfWater"]))

# For test
mqttDisconnectionMsgShown = False
connectedToBroker = False
messageReceivedFromBroker = False
mqttTimeoutPeriod = 20
mqRequestSent = False

# List to store card UID
serialBuffer = []
serialReceived = []
serialReceived = "\0"
cardValidity = False

# Dictionary for sending card status
cardInfoToSend = {
    "UID": "\0",
    "Dispensed": 0,
    "Water_Balance": 0,
    "DeviceID": device_id,
    "Time": 1569995427,
    "Message": 0,
    "Balance": 0,
    "MessageType": 1,
    "StatusCode": 0
}

serialReceiveFlag = False
serialPort = "/dev/ttyATH0"

ser = serial.Serial(serialPort, 115200)
ser.flushInput()

last_time_update = 0

mqHost = str(config["broker"])
mqPort = int(str(config["port"]))

ethernet_cable_plugged_in = False

# Method for checking internet connectivity
def internet(host=mqHost, port=mqPort, timeout=5):
    try:
        socket.setdefaulttimeout(timeout)
        socket.socket(socket.AF_INET, socket.SOCK_STREAM).connect((host, port))
        print("Internet available")
        return True
    except socket.error as ex:
        print(ex)
        return False


# The callback for getting connected to broker
def on_connect(client, userdata, flags, rc):
    global connectedToBroker
    print("Connected to broker")
    connectedToBroker = True
    
def on_disconnect(client, userdata, rc):
    global connectedToBroker
    print("Disconnected from broker")
    connectedToBroker = False

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    print(msg.topic+" "+str(msg.payload))
    if str(msg.payload)=="FU":
        #Thread(target=FU,args=()).start()
        #global alive
        #alive=False
        os.system("kill "+Pid)
    global wait_time
    global wait_start
    global wait_now
    wait_start = 0
    wait_now = 0
    global get_data
    global messageReceivedFromBroker
    global serialReceiveFlag
    
    try:
            get_data = json.loads(msg.payload)
            messageReceivedFromBroker = True
            serialReceiveFlag = True
    except:
        get_data = {}

def mq_message_sent(received_dic, requestType):
    received_dic["Time"] = int(time.time())
    if requestType == "verify":
    	print(str(config["publish_topic_verify"]))
    	publish.single(str(config["publish_topic_verify"]), json.dumps(received_dic), hostname=mqHost, port=mqPort)
    elif requestType == "dispense":
    	print(str(config["publish_topic_dispense"]))
    	publish.single(str(config["publish_topic_dispense"]), json.dumps(received_dic), hostname=mqHost, port=mqPort)
        
# Message to microcontroller to ensure system boot
if(debug_mode):
    ser.write("DE\n")
    time.sleep(1)
else:
    ser.write("DD\n")
    time.sleep(1)


ser.write("CIN")
ser.write(device_id)
ser.write("\n")
ser.write("CIA")
ser.write("\"")
ser.write(mqHost)
ser.write("\"")
ser.write("\n")

if(old_topic):
	ser.write("CIT\n")

time.sleep(1)
ser.write("O\n")

stream = os.popen('cat /sys/class/net/eth0/carrier')
output = stream.read()
if(output == '1\n'):
    ethernet_cable_plugged_in = True
elif(output == '0\n'):
    ethernet_cable_plugged_in = False

# Create an MQTT client and attach our routines to it.
client = mqtt.Client()
client.on_connect = on_connect
client.on_disconnect = on_disconnect
client.on_message = on_message


if(ethernet_cable_plugged_in):
    if(internet()):
        try:
            client.connect(mqHost, mqPort, 30)
            client.loop_start()
            client.subscribe(subscription_topic)
        except:
            if(not mqttDisconnectionMsgShown):
                ser.write("CIM")
                ser.write("Cannot Connect|to network")
                ser.write("\n")
                connectedToBroker = False
                mqttDisconnectionMsgShown = True
previousTime = int(time.time())
while(True):

    time.sleep(0.5)

    if(ser.inWaiting() > 0):
        
        stringFromSerial = ser.readline()
        serialReceiveFlag = True
        mqRequest = True
        print("Received:")
        print(stringFromSerial)
        serialReceived = list(stringFromSerial)

    if not serialReceiveFlag:
        
        if not messageReceivedFromBroker:
            if mqRequestSent:
                   print("Not received response yet")
                   currentTime = int(time.time())

                   if ((currentTime - previousTime) > mqttTimeoutPeriod):
                       print("Broker is not responding")
                       ser.write("CRT\n")
                       mqRequestSent = False
        
    if serialReceiveFlag:

        serialReceiveFlag = False            

        if(not debug_mode and serialReceived[0] == 'C'):

            print("Card ID: ")
            sID = stringFromSerial[1:stringFromSerial.index('\r')]
            decID = int(sID, 16)
            print("Decimal: ")
            print(decID)
            cardInfoToSend["UID"] = decID
            cardInfoToSend["MessageType"] = 1
            cardInfoToSend["Dispensed"] = 0
            cardInfoToSend["Time"] = int(time.time())
            print(cardInfoToSend["UID"])
            if mqRequest:
                mq_message_sent(cardInfoToSend, "verify")
                print(cardInfoToSend)
                previousTime = int(time.time())
                mqRequest = False
                mqRequestSent = True

            if messageReceivedFromBroker:

                messageReceivedFromBroker = False
                mqRequestSent = False

                if (get_data['StatusCode'] == 1):
                    cardValidity = True
                    ser.write("CIC")
                    ser.write(str(get_data["Calibration"]))
                    ser.write("\n")
                    print("Calibration sent")
                    print(get_data["Calibration"])
                    print("Card is Valid")
                    print("Card balance: ")
                    # cardBalance = float("{0:.2f}".format(get_data['Balance']))
                    cardBalance = float("{0:.2f}".format(get_data["Water_Balance"]))
                    broker_timestamp = get_data["Time"]
                    print(cardBalance)
                    print("Remaining water amount: ")
                    print(get_data["Water_Balance"])

                if (get_data['StatusCode'] == 2):
                    cardValidity = False
                    #print("Card is invalid")
                    brokerMessage = get_data["Message"]
                    print(brokerMessage)
                    ser.write("CIM")
                    ser.write(str(brokerMessage))
                    ser.write("\n")
                    ser.write("CRN\n")

            if(cardValidity):
                print("sending balance")
                ser.write("CIB" + str(cardBalance) + '\n')
                print(str(cardBalance))
                print("Sending card acknowledgement")
                ser.write("CRY\n")
                cardValidity = False

        elif(not debug_mode and serialReceived[0] == 'B'):

            sID = stringFromSerial[1:stringFromSerial.index('\r')]
            # send new account balance to broker
            print("New Balance: ")
            print(sID)
            #waterDispensed = (float(get_data["Balance"]) - float(sID)) / costOfWater
            waterDispensed = float(sID)
            waterDispensed = float("{0:.2f}".format(waterDispensed))
            print("Amount of water dispensed:")
            print(waterDispensed)
            cardInfoToSend["MessageType"] = 2
            cardInfoToSend["Dispensed"] = waterDispensed
            cardInfoToSend["Time"] = broker_timestamp
            mq_message_sent(cardInfoToSend, "dispense")
            print(cardInfoToSend)
            waterDispensed = 0.0
            serialReceived[0] = '\0'

        elif(not debug_mode and serialReceived[0] == 'T'):
            # get date and time from broker
            # send new account balance to broker
            pass

        elif(serialReceived[0] == 'E'):
            serialReceived[0] = '\0'
            print(connectedToBroker)
            
            stream = os.popen('cat /sys/class/net/eth0/carrier')
            output = stream.read()
            if(output == '1\n'):
                ethernet_cable_plugged_in = True
            elif(output == '0\n'):
                ethernet_cable_plugged_in = False
            
            if(not ethernet_cable_plugged_in):
                ser.write("EN\n")
            else:
                if(internet()):
                    if(connectedToBroker):
                        ser.write("EY\n")
                    else:
                        try:
                            client.connect(mqHost, mqPort, 30)
                            client.loop_start()
                            client.subscribe(subscription_topic)
                            ser.write("EY\n")
                        except:
                            ser.write("EN\n")
                else:
                    ser.write("EN\n")               
                    
        elif(not debug_mode and serialReceived[0] == 'A'):
            serialReceived[0] = '\0'
            debug_mode = True;

