cmake_minimum_required(VERSION 3.10)
project(DNS_client)

set(CMAKE_CXX_STANDARD 11)

add_executable(DNS_client
        src/main.c src/dns.c inc/dns.h src/socket.c inc/socket.h)
