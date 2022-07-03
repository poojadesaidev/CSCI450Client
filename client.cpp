#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>

#define TCPSERVERPORT 25112
#define IPADDR "127.0.0.1"
#define DOMAIN PF_INET // TODO change domain to AF_INET for Unix

using namespace std;

int main()
{
  // Create a socket
  int stream_client_socket; // socket id of client socket

  // Domain = AF_INET  Type = SOCK_STREAM Protocol = 0
  // We let the transport layer decide the protocol based on 'Type'
  if ((stream_client_socket = socket(DOMAIN, SOCK_STREAM, 0)) == -1)
  {
    cerr << "Stream client socket could not be created for client";
    return -1;
  }

  string ipAddress = IPADDR; // using loopback address as local IP for client

  // Create a addr hint for the Main/Stream Server
  sockaddr_in stream_server_hint;
  stream_server_hint.sin_family = DOMAIN;
  stream_server_hint.sin_port = htons(TCPSERVERPORT);                 // host to network short
  inet_pton(DOMAIN, ipAddress.c_str(), &stream_server_hint.sin_addr); // inet_pton to convert a number in our IP to array of integers

  // Connect to server
  if (connect(stream_client_socket, (sockaddr *)&stream_server_hint, sizeof(stream_server_hint)) == -1)
  {
    cerr << "Stream client socket could not be connect to stream serverM";
    return -2;
  }

  // Send and Recieve
  char buf[4096];
  string userInput;
  do
  {
    // Get user input
    cout << "> ";
    getline(cin, userInput);

    // Send to the server
    if (send(stream_client_socket, userInput.c_str(), userInput.size() + 1, 0) == -1)
    {
      cerr << "Stream client socket could not send to stream serverM. Please try again." << endl;
      continue;
    }

    // Wait for response and recieve it
    memset(buf, 0, 4096);
    int bytesRecieved = recv(stream_client_socket, buf, 4096, 0);

    if (bytesRecieved == -1)
    {
      cerr << "Stream client socket could not recieve stream from serverM. Please try again." << endl;
      continue;
    }

    // Display response
    cout << "SERVER>" << string(buf, bytesRecieved) << endl;

  } while (true);

  close(stream_client_socket);
  return 0;
}