#include <iostream>
#include <string>

void init_mosquitto();
void cleanup_mosquitto();
void publish_message(const std::string& message);
std::string getCompletion(const std::string&);
void wait_finish_speaking();
