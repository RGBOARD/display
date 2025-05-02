//
// Created by evalentin on 01/05/25.
//

#ifndef QUEUE_CLIENT_H
#define QUEUE_CLIENT_H
#include <string>

class QueueClient
{
public:
    QueueClient(std::string email, std::string password);
    std::string GetDesign();

private:
    std::string jwt;
    std::string email;
    std::string password;

    std::string GetJWT();
    static size_t CurlWriteCallback(void* contents, size_t size, size_t nmemb, std::string* output);
};
#endif //QUEUE_CLIENT_H
