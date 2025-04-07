# ft_irc

**ft_irc** is a project developed at 42 Barcelona that consists of building an IRC (Internet Relay Chat) server in C++98. This project is part of the core curriculum at 42 and represents one of the final and most challenging steps before completing the program.

The goal is to implement a real-time chat server that supports multiple clients, handling connections, commands, and communication following the IRC protocol specifications.

## 🧠 Key Learnings

### 🔌 Sockets and Networking
The project involved low-level socket programming, managing client connections over TCP/IP, and handling data transmission between the server and multiple clients.

### 🧵 Multiplexing Connections
To support multiple clients simultaneously without using threads, the server uses **multiplexing techniques** such as `select` or `poll`, allowing for scalable real-time communication.

### 🗣️ IRC Protocol Implementation
Commands such as `NICK`, `JOIN`, `PRIVMSG`, `MODE`, and others were implemented according to the IRC RFCs. Responses follow the protocol structure using numeric status codes and formatted messages.

### 🧮 Memory Management in C++98
Due to the use of the C++98 standard, dynamic memory was manually managed, avoiding memory leaks and ensuring optimal performance.

### 🧱 Object-Oriented Design
The codebase follows a modular and maintainable object-oriented architecture. Key components include:
- `Client` class: represents connected users.
- `Channel` class: manages chat rooms.
- `Command` classes: each IRC command is handled by a separate class for clarity and modularity.
- `Server` class: manages the overall logic and lifecycle of the server.

## 🚀 Project Highlights
- Real-time communication between multiple clients
- Full support for IRC basic commands and user/channel management
- Error handling and feedback with proper IRC numeric codes
- Secure and efficient handling of connections using non-blocking IO

## ⚙️ Usage
To run the server:
```bash
./ircserv <port> <password>
