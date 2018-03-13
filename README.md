# distributed-banking-system
CZ4013 - Distributed System Assignment

# How to compile

## Client (Java)
```
cd client
javac -d . *.java
java client.UDPClient <IP address> <port>
```

## Server (C++)
```
cd server
g++ -o server main.cpp udp_client_server.cpp utils.cpp Handler.cpp AccountManager.cpp Account.cpp
./server
```