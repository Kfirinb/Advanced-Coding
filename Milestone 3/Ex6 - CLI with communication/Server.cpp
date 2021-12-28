// g++ -pthread -std=c++11 serverTest.cpp
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#include "Server.h"

#include <thread>

#include <sstream>
#include <iomanip>

#define READING_BUFF_SIZE 1

//implementation of SocketIO class constructor
SocketIO::SocketIO(int aClient) {
    this->aClient = aClient;
}
//implementation of SocketIO class functions:
string SocketIO::read(){
    char buffer[READING_BUFF_SIZE];
    int bytesRead;
    string s ="";
    //bzero function zeros all the bits in the buffer.
    bzero(buffer, READING_BUFF_SIZE);
    if ((bytesRead = recv(aClient, buffer, READING_BUFF_SIZE,0)) < 0)
        throw std::system_error(errno, std::generic_category(), "Error reading from connection socket");
    else if (bytesRead == 0)
        return "";
    while(*buffer!='\n'){
        s += *buffer;
        if ((bytesRead = recv(aClient, buffer, READING_BUFF_SIZE,0)) < 0)
            throw std::system_error(errno, std::generic_category(), "Error reading from connection socket");
        else if (bytesRead == 0)
            return "";
    }
    return s;
}
void SocketIO::read(float* f){
    string s = read();
    *f=std::stof(s);
}
void SocketIO::write(string text){
    send(aClient,text.c_str(),strlen(text.c_str()),0);
}

void SocketIO::write(float f){
    string s = std::to_string(f);
    s.erase( s.find_last_not_of('0') + 1,string::npos );
    send(aClient,s.c_str(),s.length(),0);

}

void SocketIO::closeSocketIO(){
    close(aClient);
}
SocketIO::~SocketIO(){
    closeSocketIO();
}

//This is the constructor of the server which initializes the server by port parameter.
Server::Server(int port) throw (const char*) {
    fileDescriptor = socket(AF_INET,SOCK_STREAM,0);
    if(fileDescriptor < 0)
        throw "socket failed";
    //IPV4:
    server.sin_family = AF_INET;
    //the IP of the server:
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);
    if(bind(fileDescriptor,(struct sockaddr*)&server,sizeof(server))<0)
        throw "binf failure";
    if(listen(fileDescriptor,3)<0)
        throw "listen failure";

}

//This function gets a clienthandler and start accepting and handling clients in threads.
void Server::start(ClientHandler& ch) throw(const char*){
   t = new thread([&ch,this](){
           while(!isStop) {
                //std::cout << "waiting for a client" << std::endl;
                socklen_t clientSize = sizeof(client);
                int aClient = accept(fileDescriptor, (struct sockaddr *) &client, &clientSize);
                if (aClient < 0) {
                    throw "accept failure";
                }
                //std::cout << "Client connected from server" << std::endl;
                ch.handle(aClient);
                close(aClient);
                this_thread::sleep_for (chrono::seconds(1));
           }
        close(fileDescriptor);
   });

}

//This function stops the server running - it stops the start loop and joins the threads.
void Server::stop(){
    isStop = true;
    t->join(); // do not delete this!
}

Server::~Server() {
}

