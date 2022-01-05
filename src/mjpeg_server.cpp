#include <fmt/core.h>
#include <fstream>
#include <unistd.h>
#include <arpa/inet.h>
#include "mjpeg_server.h"

int general_socket;
int client_socket;
int address_length;
struct sockaddr_in address;
std::ofstream file;
void close_fd()
{
    fmt::print("[LOG] : Exiting.\n");
    close(client_socket);
    close(general_socket);
}
Mjpeg_server::Mjpeg_server()
{
    int PORT = 1337;
    if ((general_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        fmt::print("[ERROR] : Socket failed\n");
        exit(EXIT_FAILURE);
    }
    atexit(close_fd);

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    address_length = sizeof(address);

    if (bind(general_socket, (sockaddr *)&address, sizeof(address)) < 0)
    {
        fmt::print("[ERROR] : Bind failed\n");
        exit(EXIT_FAILURE);
    }
    fmt::print("[LOG] : Server started on {}.\n", PORT);

    if (listen(general_socket, 3) < 0)
    {
        fmt::print("[ERROR] : Listen\n");
        exit(EXIT_FAILURE);
    }
    handle_connection();
}

void Mjpeg_server::handle_connection()
{

    if ((client_socket = accept(general_socket, (sockaddr *)&address, (socklen_t *)&address_length)) < 0)
    {
        fmt::print("[ERROR] : Accept\n");
        exit(EXIT_FAILURE);
    }
    fmt::print("[LOG] : Connection from {}:{}\n", inet_ntoa(address.sin_addr), ntohs(address.sin_port));

    file.open("tes.jpg");
    if (!file.is_open())
    {
        fmt::print("[ERROR] : Error writing file.\n");
        exit(EXIT_FAILURE);
    }
    char buffer[1024];
    while (true)
    {
        int valread = recv(client_socket, buffer, 1024, 0);
        if (valread == 0)
        {
            break;
        }
        fmt::print("{}", buffer);
        file << buffer;
    }

    file.close();
    close(client_socket);
}
