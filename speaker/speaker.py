import paho.mqtt.client as mqtt
import subprocess

# MQTT Broker Info
broker_address = "localhost"
speak_topic = "speak"

# Function to convert text to speech
def speak(input_text):
    sanitized_text = input_text.replace('"', '').replace("'", "")
    command = f"echo '{sanitized_text}' | /home/pi/work/piper/install/piper --model /home/pi/work/piper/models/amy-medium.onnx --output-raw | aplay -r 22050 -f S16_LE -t raw -"

    # Execute the command
    subprocess.run(command, shell=True)

# Callback function for MQTT message reception
def on_message(client, userdata, message):
    if message.topic == speak_topic:
        # Convert the received message to speech
        speak(message.payload.decode())

# Create MQTT client instance
client = mqtt.Client(mqtt.CallbackAPIVersion.VERSION1)

# Set up MQTT callbacks
client.on_message = on_message

# Connect to MQTT broker
client.connect(broker_address)

# Subscribe to MQTT topic
client.subscribe(speak_topic)

# Start the MQTT loop
client.loop_forever()

