#include <iostream>
#include <mosquitto.h>

// MQTT Broker Info
const char *host = "localhost";
const char *topic = "speak";

// MQTT client instance
struct mosquitto *mosq = NULL;

// MQTT callback function for when the message is published
void on_publish(struct mosquitto *mosq, void *obj, int mid)
{
    std::cout << "Message published to topic: " << topic << std::endl;
}

// Function to initialize Mosquitto
void init_mosquitto()
{
    // Initialize mosquitto library
    mosquitto_lib_init();

    // Create a mosquitto client instance
    mosq = mosquitto_new(NULL, true, NULL);
    if (!mosq)
    {
        std::cerr << "Error: Unable to create mosquitto client" << std::endl;
        return;
    }

    // Connect to the MQTT broker
    int rc = mosquitto_connect(mosq, host, 1883, 60);
    if (rc != MOSQ_ERR_SUCCESS)
    {
        std::cerr << "Error: Unable to connect to MQTT broker" << std::endl;
        return;
    }

    // Set callback function for when the message is published
    mosquitto_publish_callback_set(mosq, on_publish);
}

// Function to cleanup Mosquitto
void cleanup_mosquitto()
{
    if (mosq)
    {
        // Cleanup
        mosquitto_destroy(mosq);
        mosquitto_lib_cleanup();
    }
}

// Function to publish a message to the MQTT broker
void publish_message(const std::string& message)
{
    // Publish the message to the topic
    int rc = mosquitto_publish(mosq, NULL, topic, message.size(), message.c_str(), 0, false);
    if (rc != MOSQ_ERR_SUCCESS)
    {
        std::cerr << "Error: Unable to publish message" << std::endl;
        return;
    }
}

int test_main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <message>" << std::endl;
        return 1;
    }

    // Initialize Mosquitto
    init_mosquitto();

    // Send the message to the MQTT broker
    publish_message(argv[1]);

    // Cleanup Mosquitto
    cleanup_mosquitto();

    return 0;
}

