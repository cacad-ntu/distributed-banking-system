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
g++ -o server main.cpp udp_client_server.cpp
./server
```