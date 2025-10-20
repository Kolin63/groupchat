// SPDX-License-Identifier: MIT
// Copyright (c) Colin Melican 2025
// Based off of code from https://think-async.com

#include <stddef.h>

#include <ctime>
#include <iostream>
#include <string>
#include <system_error>

#include <asio.hpp>

using asio::ip::tcp;

std::array<char[51], 50> messages;

int main() {
  std::cout << "Starting server\n";
  try {
    asio::io_context io_context;

    tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 63101));

    // main loop
    while (true) {
      tcp::socket socket(io_context);
      acceptor.accept(socket);

      // read client message
      while (true) {
        std::array<char, 51> buf;
        std::error_code error;

        size_t len{socket.read_some(asio::buffer(buf), error)};

        if (error == asio::error::eof)
          break;  // Connection closed cleanly by peer.
        else if (error)
          throw std::system_error(error);  // Some other error

        std::string message{};
        for (size_t i{}; i < len; ++i) {
          message += buf[i];
        }

        if (!message.empty()) std::cout << "received: " << message << '\n';
      }

      std::string message{"Hello, Client!"};

      std::error_code ignored_error;
      asio::write(socket, asio::buffer(message), ignored_error);
      std::cout << "Returned " << message << '\n';
    }
  } catch (std::exception& e) {
    std::cerr << e.what() << '\n';
  }

  return 0;
}
