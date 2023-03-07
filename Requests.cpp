#include <iostream>
#include <string>
#include <map>
#include "json.hpp"

#include "ClientSocket.h"

#define INCLUDE_NLOHMANN_JSON_HPP_

using json = nlohmann::json;

class Requests {
private:
    ClientSocket socket;
    const char* ip;
    const char* port;

public: 
    /*
        Создает новый объект ClientSocket
    */
    Requests(const char *ip, const char *port) {
        this->socket = ClientSocket();
        this->ip = ip;
        this->port = port;
    }
    /*
        Возвращает строку с запросом 
    */
    std::string get_request(std::map<const char*, const char*> params, const char* request) {
        json j_map(params);

        std::string json_info = j_map.dump();
        size_t json_info_length = json_info.length();
        std::string request_body;

        if (request == "get_serial_number")
            request_body = "POST /index.php?request=get_serial_number HTTP/1.1\nHost: test\nContent-Type: application/json\nContent-Length: ";
        else if (request == "send_testing_info") 
            request_body = "POST /index.php?request=send_testing_info HTTP/1.1\nHost: test\nContent-Type: application/json\nContent-Length: ";
        else if (request == "get_testing_info") 
            request_body = "POST /index.php?request=get_testing_info HTTP/1.1\nHost: test\nContent-Type: application/json\nContent-Length: ";

        std::string result_request = request_body + std::to_string(json_info_length) + "\n\n" + json_info;

        return result_request;
    }
    /*
        Возвращает серийный номер
    */
   json get_serial_number(std::map<const char*, const char*> params) {
        socket.socketConnect(ip, port);
        socket.sendRequest(get_request(params, "get_serial_number").c_str());
        std::string response = socket.recieveData();

        size_t index = response.find("{");
        std::string json_response = response.substr(index);

        return json::parse(json_response);
    }
    /*
        Отправляет запрос на запись данных
    */
    json send_testing_info(std::map<const char*, const char*> params) {
        socket.socketConnect(ip, port);
        socket.sendRequest(get_request(params, "send_testing_info").c_str());
        std::string response = socket.recieveData();

        size_t index = response.find("{");
        std::string json_response = response.substr(index);

        return json::parse(json_response);
    }   
    /*
        Отправляет запрос на получение данных
    */
    json get_testing_info(const char* serial_number) {
        std::map<const char*, const char*> params;
        params.insert(std::pair<const char*, const char*>("Serial number", serial_number));
        socket.socketConnect(ip, port);
        socket.sendRequest(get_request(params, "get_testing_info").c_str());
        std::string response = socket.recieveData();

        size_t index = response.find("{");
        std::string json_response = response.substr(index);

        return json::parse(json_response);
    }

};