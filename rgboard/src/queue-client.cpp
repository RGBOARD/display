//
// Created by evalentin on 01/05/25.
//

#include "../include/queue-client.h"
#include <curl/curl.h>
#include <cstdlib>

#define LOGIN_URL "http://localhost:5000/login"
#define CURRENT_URL "http://localhost:5000/rotation/current"


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

bool QueueClient::GetDesign()
{
    auto perform_request = [this](std::string& response) -> long
    {
        CURL* curl = curl_easy_init();
        if (!curl)
        {
            std::printf("Curl init failed\n");
            return -1;
        }

        struct curl_slist* headers = nullptr;
        std::string auth_header = "Authorization: Bearer " + this->jwt;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        headers = curl_slist_append(headers, auth_header.c_str());

        curl_easy_setopt(curl, CURLOPT_URL, CURRENT_URL);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L); // use GET
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CurlWriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

        CURLcode result = curl_easy_perform(curl);

        long http_code = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);

        if (result != CURLE_OK)
        {
            std::fprintf(stderr, "Request failed: %s\n", curl_easy_strerror(result));
            return -1;
        }

        return http_code;
    };

    // First attempt
    std::string response;
    long status_code = perform_request(response);

    // If unauthorized, refresh token and try again
    if (status_code == 401)
    {
        std::printf("JWT expired or invalid. Getting new token...\n");
        this->jwt = GetJWT();
        if (this->jwt.empty())
        {
            std::printf("Failed to refresh JWT.\n");
            return false;
        }

        response.clear();
        status_code = perform_request(response);
    }

    if (status_code != 200)
    {
        std::printf("Server responded with HTTP %ld\n", status_code);
        return false;
    }

    // Parse JSON response
    Json::CharReaderBuilder reader;
    Json::Value json_response;
    std::string errs;
    std::istringstream s(response);

    if (!Json::parseFromStream(reader, s, &json_response, &errs))
    {
        std::printf("Failed to parse response JSON: %s\n", errs.c_str());
        return false;
    }

    if (json_response.isMember("image") && json_response["image"].isMember("pixel_data") && json_response.
        isMember("time_left"))
    {
        std::string raw = json_response["image"]["pixel_data"].asString();

        // Strip "Design " prefix if present
        const std::string prefix = "Design ";
        if (raw.rfind(prefix, 0) == 0)
            raw = raw.substr(prefix.length());

        Json::CharReaderBuilder builder;
        Json::Value parsed;
        std::string errs;
        std::istringstream ss(raw);

        if (!Json::parseFromStream(builder, ss, &parsed, &errs))
        {
            std::fprintf(stderr, "Failed to parse pixel_data string: %s\n", errs.c_str());
            return false;
        }

        this->pixel_data = parsed;
        this->display_duration = json_response["time_left"].asInt();
        return true;
    }

    std::string error_message = json_response.get("error", "Unknown error").asString();
    std::printf("Failed to get design: %s\n", error_message.c_str());
    return false;
}


size_t QueueClient::CurlWriteCallback(void* contents, size_t size, size_t nmemb, std::string* output)
{
    size_t total = size * nmemb;
    output->append((char*)contents, total);
    return total;
}


// Getters

int QueueClient::GetDisplayDuration() const
{
    return this->display_duration;
}

Json::Value QueueClient::GetPixelData()
{
    return this->pixel_data;
}
