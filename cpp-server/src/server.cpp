#include "server.hpp"
#include "thread_pool.hpp"
#include "request.hpp"
#include "response.hpp"
#include "router.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <thread>
#include <vector>
#include <cstring>

Server::Server(int port) : port(port) {}

void Server::start() {
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 10) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    std::cout << "🟢 Server listening on port " << port << std::endl;

    ThreadPool pool(4); // 4 threads for simplicity
    while (true) {
        int client_socket = accept(server_fd, nullptr, nullptr);
        if (client_socket >= 0) {
            pool.enqueue([this, client_socket]() {
                handleConnection(client_socket);
            });
        }
    }
}

void Server::handleConnection(int client_socket) {
    char buffer[4096] = {0};
    int valread = read(client_socket, buffer, sizeof(buffer));
    if (valread > 0) {
        std::string raw(buffer);
        Request req = Request::parse(raw);
        Response res = Router::route(req);
        std::string http = res.toString();
        send(client_socket, http.c_str(), http.length(), 0);
    }
    close(client_socket);
}