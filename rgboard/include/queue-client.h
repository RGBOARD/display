//
// Created by evalentin on 01/05/25.
//

#ifndef QUEUE_CLIENT_H
#define QUEUE_CLIENT_H
#include <string>
#include <jsoncpp/json/json.h>

class QueueClient
{
public:
    QueueClient(std::string email, std::string password);
    bool GetDesign();
    [[nodiscard]] int GetDisplayDuration() const;
    Json::Value GetPixelData();

private:
    std::string jwt;
    std::string email;
    std::string password;

    // We store in these variables and get from here. Can't return a string and an int in the same function.
    int display_duration{};
    Json::Value pixel_data;

    std::string GetJWT();
    static size_t CurlWriteCallback(void* contents, size_t size, size_t nmemb, std::string* output);
};
#endif //QUEUE_CLIENT_H
