import paho.mqtt.server as mqtt

# Create an MQTT broker
broker = mqtt.MQTTServer()

# Define the callback functions
def on_connect(client, userdata, flags, rc):
    print(f"Connected with result code {rc}")
    client.subscribe("test/topic")

def on_message(client, userdata, msg):
    print(f"Message received on {msg.topic}: {msg.payload.decode()}")

# Set up the broker and callbacks
broker.on_connect = on_connect
broker.on_message = on_message

# Start the broker
broker.listen()
