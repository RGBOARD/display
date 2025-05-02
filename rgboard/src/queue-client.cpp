//
// Created by evalentin on 01/05/25.
//

#include "../include/queue-client.h"
#include <curl/curl.h>
#include <jsoncpp/json/json.h>
#include <cstdlib>

#define LOGIN_URL "http://localhost:5000/login"


QueueClient::QueueClient(std::string email, std::string password)
{
    this->email = std::move(email);
    this->password = std::move(password);
    jwt = GetJWT();
}


std::string QueueClient::GetJWT()
{
    if (this->email.empty())
    {
        std::printf("Email field is empty.\n");
        return "";
    }
    else if (password.empty())
    {
        std::printf("Password field is empty.\n");
        return "";
    }

    CURL* curl = curl_easy_init();

    if (!curl)
    {
        printf("Curl init failed\n");
        return "";
    }

    std::string response;
    Json::Value body;

    // Doing this in C++ is crazy, even the IDE is confused.
    body["email"] = this->email;
    body["password"] = this->password;

    Json::StreamWriterBuilder writer;

    std::string json_body = Json::writeString(writer, body);

    struct curl_slist* headers = nullptr;

    headers = curl_slist_append(headers, "Content-Type: application/json");

    // Set options
    curl_easy_setopt(curl, CURLOPT_URL, LOGIN_URL);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_body.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CurlWriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    // Send request
    CURLcode result = curl_easy_perform(curl);
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    if (result != CURLE_OK)
    {
        fprintf(stderr, "problem: %s\n", curl_easy_strerror(result));
        return "";
    }

    Json::CharReaderBuilder reader;
    Json::Value json_response;
    std::string errs;

    std::istringstream s(response);

    if (!Json::parseFromStream(reader, s, &json_response, &errs))
    {
        printf("Failed to parse response JSON:  %s\n", errs.c_str());
        return "";
    }

    if (json_response.isMember("access_token"))
    {
        std::string token = json_response["access_token"].asString();
        return token;
    }

        std::string error_message = json_response["error"].asString();
        printf("Login failed: no token in response.\nError: %s\n", error_message.c_str());
        return "";
}

std::string QueueClient::GetDesign()
{

    if (this->jwt.empty())
    {
        return "";
    }
}

size_t QueueClient::CurlWriteCallback(void* contents, size_t size, size_t nmemb, std::string* output)
{
    size_t total = size * nmemb;
    output->append((char*)contents, total);
    return total;
}
