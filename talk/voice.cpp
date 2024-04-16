#include <iostream>
#include <mosquitto.h>
#include <unistd.h>

// MQTT Broker Info
const char *host = "localhost";
const char *topic = "speak";
const char *topic_finish_speaking = "finish_speaking";

// MQTT client instance
struct mosquitto *mosq = NULL;

void on_publish(struct mosquitto *mosq, void *obj, int mid)
{
    std::cout << "Message published to topic: " << topic << std::endl;
}

bool finished_talking;
void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg)
{
    printf("Message received on topic: %s\n", msg->topic);
    printf("Message payload: %s\n", (char *)msg->payload);
    
    finished_talking = true;
    //mosquitto_disconnect(mosq);
}

void init_mosquitto()
{
    mosquitto_lib_init();

    mosq = mosquitto_new(NULL, true, NULL);
    if (!mosq)
    {
        std::cerr << "Error: Unable to create mosquitto client" << std::endl;
        return;
    }

    int rc = mosquitto_connect(mosq, host, 1883, 60);
    if (rc != MOSQ_ERR_SUCCESS)
    {
        std::cerr << "Error: Unable to connect to MQTT broker" << std::endl;
        return;
    }

    mosquitto_publish_callback_set(mosq, on_publish);
    mosquitto_subscribe(mosq, NULL, topic_finish_speaking, 0);
    mosquitto_message_callback_set(mosq, on_message);
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

void wait_finish_speaking()
{
   finished_talking = false;

  while (!finished_talking) 
{
    mosquitto_loop(mosq, 0, 1);
    usleep(500000); 
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

