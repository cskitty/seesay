# SeeSay - Smart Glasses 

SeeSay is a smart glasses designed to assist the visually impaired through various functions including continuous voice recognition, natural text-to-speech (TTS) conversion, and token generation using a large language model (LLM). It is specifically designed to run on an ESP32 S3 Sense board for camera and voice sensing. It uses Raspberry Pi as a computation unit,  communicating through BLE and the Mosquitto MQTT protocol for dispatching messages.

## Demo

Check out the demo of SeeSay in action [here](https://www.youtube.com/watch?v=nybifJqqwr0).

## Features

1. **Continuous Voice Recognition**: SeeSay is capable of take photos and continuously recognizing voice inputs, allowing for seamless interaction with users.

2. **Natural Text-to-Speech (TTS)**: With SeeSay, users can enjoy natural and human-like speech synthesis, converting text inputs into spoken words.

3. **Large Language Model (LLM)**: SeeSay leverages a large language model for token generation, enabling it to understand and generate contextually relevant responses. Currently, LLM is running on my laptop using LM Studio with Dolphin mistral 7B.
 

4. **More features TBD**: SeeSay is continuously evolving, with more features and functionalities planned for future releases.

## Requirements

- Raspberry Pi 5
- Mosquitto MQTT
- C++ Compiler (for Whisper.cpp)
- Python 3
- Piper (for TTS)
- Mistral (LLM), runs as a service on my laptop

## Installation

1. Install Mosquitto MQTT on your Raspberry Pi.
2. Compile and install Whisper.cpp. [GitHub Repository](https://github.com/ggerganov/whisper.cpp)
3. Install Piper for TTS. [GitHub Repository](https://github.com/rhasspy/piper)
4. Clone the repo code , compile and run

## Usage

1. Start SeeSay by running the provided scripts.
2. SeeSay will continuously listen for voice inputs and respond accordingly.

## Contributing

Contributions to SeeSay are welcome! If you have any ideas, feature requests, or bug reports, feel free to open an issue or submit a pull request.

## License

SeeSay is licensed under the [MIT License](LICENSE).

## Acknowledgments

- Whisper.cpp by ggerganov
- Piper by rhasspy
- LLM by mistral 


