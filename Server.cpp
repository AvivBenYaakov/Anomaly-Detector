
#include "Server.h"

Server::Server(int port) throw(const char *) {
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        /**
         * remember to free all used objects before.
         */
        throw "socket failed";
    }
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);
    if (bind(fd, (struct sockaddr *) &server, sizeof(server)) < 0) {
        throw "bind failed";
    }
    if (listen(fd, 3) < 0) {
        throw "listen failed";
    }

}

void Server::start(ClientHandler &ch) throw(const char *) {
    running = true;
    t = new thread([&ch, this]() {
        while (running) {
            // cout << "waiting for a client" << endl;
            socklen_t clientSize = sizeof(client);
            alarm(1);
            int aClient = accept(fd, (struct sockaddr *) &client, &clientSize);
            alarm(0);
            if (aClient < 0)
                throw "accept failed";
            // cout << "client connected" << endl;
            ch.handle(aClient);
            close(aClient);
        }
        close(fd);
    });



}

void Server::stop() {
//    t->join(); // do not delete this!
    running = false;

}

Server::~Server() {
}

void AnomalyDetectionHandler::handle(int clientID) {
    SocketIO sio(clientID);
    CLI cli(&sio);
    cli.start();
}