// SPDX-License-Identifier: MIT
// Copyright (c) Colin Melican 2025
// Based off of code from https://think-async.com

#include <stddef.h>

#include <ctime>
#include <iostream>
#include <string>

#include <asio.hpp>

using asio::ip::tcp;

int main() {
  std::cout << "Starting server\n";
  try {
    asio::io_context io_context;

    tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 63101));

    while (true) {
      tcp::socket socket(io_context);
      acceptor.accept(socket);

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
