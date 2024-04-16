import cv2
import numpy as np
import paho.mqtt.client as mqtt
import time
import datetime

# MQTT Broker Info
broker_address = "localhost"
create_image_topic = "capture_image"
new_image_topic = "new_image"
exit_capture_topic = "exit_capture"

# Function to handle MQTT messages
def on_message(client, userdata, message):
    global capturing
    global camera
    print("received from topic ", message.topic)
    if message.topic == create_image_topic:
        capture_image()
    elif message.topic == exit_capture_topic:
        capturing = False
        camera.release()

# Capture image using OpenCV and publish it to MQTT
def capture_image():
    # Initialize the camera
    global camera

    # Check if camera opened successfully
    if not camera.isOpened():
        print("Error: Could not open camera")
        return

    # Capture a frame from the camera
    ret, frame = camera.read()

    # Check if the frame was captured successfully
    if ret:
        # Convert image to byte array
        _, img_encoded = cv2.imencode('.jpg', frame)
        img_bytes = img_encoded.tobytes()

        # Publish the image to MQTT
        client.publish(new_image_topic, img_bytes)
        print("Image published to MQTT topic:", new_image_topic)


# Create MQTT client instance
client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION1)

# Set up MQTT callbacks
client.on_message = on_message

# Connect to MQTT broker
client.connect(broker_address)

# Subscribe to MQTT topics
client.subscribe([(create_image_topic, 0), (exit_capture_topic, 0)])

# Start the MQTT loop in a non-blocking way
client.loop_start()

# Flag to control image capturing
capturing = True

# Main loop
camera = cv2.VideoCapture(0)
while capturing:
    time.sleep(1)

# Disconnect from MQTT broker
client.loop_stop()
client.disconnect()

