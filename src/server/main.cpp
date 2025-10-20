// SPDX-License-Identifier: MIT
// Copyright (c) Colin Melican 2025
// Based off of code from https://think-async.com

#include <stddef.h>
#include <algorithm>

#include <array>
#include <ctime>
#include <iostream>
#include <string>
#include <string_view>
#include <system_error>

#include <asio.hpp>

using asio::ip::tcp;

std::array<std::string, 30> messages{};

int main() {
  std::cout << "Starting server\n";
  try {
    asio::io_context io_context;

    tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 63101));

    // main loop
    while (true) {
      tcp::socket socket{io_context};
      acceptor.accept(socket);

      // read client message
      std::string client_message{};
      std::cout << "reading... ";
      while (true) {
        std::array<char, 51> buf;
        std::error_code error;

        if (socket.available() == 0) break;  // no data

        size_t len{socket.read_some(asio::buffer(buf), error)};

        if (error == asio::error::eof)
          break;  // Connection closed cleanly by peer.
        else if (error)
          throw std::system_error(error);  // Some other error

        client_message.append(buf.data(), len);
      }
      std::cout << "done" << std::endl;

      if (!client_message.empty()) {
        std::cout << "assuming send operation" << std::endl;
        std::cout << "received: " << client_message << '\n';
        std::shift_left(messages.begin(), messages.end(), 1);
        messages[messages.size() - 1] = client_message;
      } else {
        std::cout << "assuming get operation" << std::endl;
        std::string ret{};
        for (std::string_view i : messages) {
          ret.append(i);
          ret += '\n';
        }

        std::error_code ignored_error;
        asio::write(socket, asio::buffer(ret), ignored_error);
        std::cout << "Returned messages\n";
      }
    }
  } catch (std::exception& e) {
    std::cerr << e.what() << '\n';
  }

  return 0;
}
