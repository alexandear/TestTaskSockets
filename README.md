TestTaskSockets
===============

A client-server program ready for MS Windows 7/8 64-bit that works with XML files.

The application be divided on two parts, server and client:

1. Server (Windows Service) work with XML file that should contain pair tag-value.
Server read the pairs, add new pairs and update the existing values.
Each console command reply its status into console window.

2. Client should be a console application. It should connect to Server using Sockets,
send requests and receive answers from Server. User should have possibility to use console
commands to get access to XML file. You need to implement at least three commands:
add <tag name> <tag value>, get <tag name>, set <tag name> <tag value>.
