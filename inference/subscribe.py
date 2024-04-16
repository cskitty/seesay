import os
import io
import numpy as np
import paho.mqtt.client as mqtt
import cv2

# MQTT Broker Info
broker_address = "localhost"
topic = "new_image"

# Callback function to handle incoming messages
def on_message(client, userdata, message):
    if message.topic == topic:
        # Convert the received message payload to a numpy array
        nparr = np.frombuffer(message.payload, np.uint8)
        
        # Decode the numpy array into an OpenCV image
        img = cv2.imdecode(nparr, cv2.IMREAD_COLOR)

        # Save the image to a file
        filename = "received_image.jpg"
        cv2.imwrite(filename, img)
        print("Image saved as:", filename)

# Create a MQTT client instance
client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION1)

# Set up callback functions
client.on_message = on_message

# Connect to the MQTT broker
client.connect(broker_address)

# Subscribe to the "new_image" topic
client.subscribe(topic)

# Loop to listen for incoming messages
client.loop_forever()

