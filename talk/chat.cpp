/*
 * This program demonstrates how to programatically interact with OpenAI GPT using C++.
 * The program uses the curl library for making HTTP requests, and the nlohmann json
 * library for handling JSON data.
 */
#include <iostream>
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <cstdlib> // For system()
#include <sstream>

#include "voice.h"

using std::string;
using std::cout;
using std::cin;
using std::cerr;
using std::endl;
using nlohmann::json;

using namespace std;


// Function prototypes
size_t WriteCallback(void*, size_t, size_t, string*);

std::vector<std::string> splitString(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}


// Handle data received from the server
size_t WriteCallback(void* contents, size_t size, size_t nmemb, string* response) {
    size_t totalSize = size * nmemb;
    response->append((char*)contents, totalSize);
    return totalSize;
}

// Construct a POST request to the chat model endpoint and process the response.
string getCompletion(const string& prompt) {
    //string apiKey = ""; // add your API key, the app will not work without it
    string apiKey = "lm-studio"; // add your API key, the app will not work without it
    //string baseUrl = "https://api.openai.com/v1/chat/completions";
    string baseUrl = "http://192.168.2.249:1234/v1/chat/completions";
    string response;
    CURL* curl = curl_easy_init();

    if (curl) {
        json requestData;
        requestData["model"] = "local-model";
        requestData["messages"][0]["role"] = "system";
        requestData["messages"][0]["content"] = "You are an intelligent assistant.";
        requestData["messages"][1]["role"] = "user";
        requestData["messages"][1]["content"] = prompt;
        requestData["temperature"] = 0.7;
        requestData["max_tokens"] = -1;
        requestData["stream"] = true;

        string requestDataStr = requestData.dump().c_str();

        struct curl_slist* headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, ("Authorization: Bearer " + apiKey).c_str());
        curl_easy_setopt(curl, CURLOPT_URL, baseUrl.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, requestDataStr.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, requestDataStr.length());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
        CURLcode res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            cerr << "Curl request failed: " << curl_easy_strerror(res) << endl;
        }

        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }

    //cout << response << endl;

    std::vector<std::string> responses = splitString(response, '\n');

    std::string answer = "";
    for (const std::string& response : responses) {
        if (response.empty()) {
            continue; // Skip empty lines
        }
        // Remove the "data:" prefix
        std::string jsonStr = response.substr(6); // Remove the first 6 characters (data: )
        if (jsonStr.find("[DONE]") == 0)
             break;

        try {
            // Parse the JSON response
            json jresponse = json::parse(jsonStr);


            // Check if the "delta" object is empty
            if (jresponse["choices"][0]["delta"].empty()) {
                continue; // Skip this response
            }

            // Extract data from the response
            std::string content = jresponse["choices"][0]["delta"]["content"];
            answer += content;
        } catch (const std::exception& e) {
            std::cerr << "Error parsing JSON: " << e.what() << std::endl;
            break;
        }
     }
    
  //  json jresponse = json::parse(response);

/*
    for (const auto& choice : jresponse["choices"]) {
        // Print the content of each choice
        cout << choice << endl;
        //std::cout << "Content: " << choice["message"]["delta"]["content"].get<std::string>() << std::endl;
    }


    return jresponse["choices"][0]["message"]["delta"]["content"].get<string>();
*/
   return answer;
}
