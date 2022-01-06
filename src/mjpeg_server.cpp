#include <fmt/core.h>
#include <fstream>
#include <iostream>
#include <iomanip>
#include "mjpeg_server.h"
#include "app.h"
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#endif

int listener;
int client_conn;
int address_length;
struct sockaddr_in address;
std::ofstream file;

std::string string_to_hex(const std::string &input)
{
    static const char hex_digits[] = "0123456789ABCDEF";

    std::string output;
    output.reserve(input.length() * 2);
    for (unsigned char c : input)
    {
        output.push_back(hex_digits[c >> 4]);
        output.push_back(hex_digits[c & 15]);
    }
    return output;
}

void handle_connection()
{
    // while (true)
    // {
    client_conn = accept(listener, (struct sockaddr *)&address, (socklen_t *)&address_length);
    if ((client_conn) < 0)
    {
        app::logger("[ERROR] : Accept\n");
        exit(EXIT_FAILURE);
    }
    app::logger("[LOG] : Connection from {}:{}\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));
    file.open("raw", std::ios_base::out);
    if (!file.is_open())
    {
        app::logger("[ERROR] : Error writing file\n");
        exit(EXIT_FAILURE);
    }

    char buffer[1024];
    std::string data;
    while (true)
    {
        auto len = read(client_conn, buffer, sizeof(buffer));
        if (len == 0)
        {
            break;
        }
        data.append(buffer, len);
    }

    app::logger("[LOG] : Total length: {} bytes\n", data.size());
    file.write(data.c_str(), data.size());
    file.close();
    close(client_conn);
    app::logger("[LOG] : Done\n");
    // }
}

void mjpeg_server(int port)
{
#ifdef _WIN32
    //----------------------
    // Initialize Winsock.
    WSADATA wsaData;
    auto iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != NO_ERROR)
    {
        app::logger(L"WSAStartup failed with error: {}\n", iResult);
        return 1;
    }
#endif
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    address_length = sizeof(address);

    if ((listener = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        app::logger("[ERROR] : Socket failed\n");
        exit(EXIT_FAILURE);
    }

    if (bind(listener, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        app::logger("[ERROR] : Bind failed\n");
        exit(EXIT_FAILURE);
    }

    app::logger("[LOG] : Server started on {}\n", port);

    if (listen(listener, 3) < 0)
    {
        app::logger("[ERROR] : Listen\n");
        exit(EXIT_FAILURE);
    }

    handle_connection();
}
