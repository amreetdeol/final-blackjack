#pragma once

class Server {
public:
    Server(int port);
    void start();

private:
    int port;
    int server_fd;
    void handleConnection(int client_socket);
};