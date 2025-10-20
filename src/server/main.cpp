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

#include "../common/message.hpp"

using asio::ip::tcp;

std::array<std::string, 15> messages{};

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
      std::string raw{};
      std::array<char, 128> buf;
      std::error_code error;

      size_t len{socket.read_some(asio::buffer(buf), error)};

      if (error == asio::error::eof)
        break;  // Connection closed cleanly by peer.
      else if (error)
        throw std::system_error(error);  // Some other error

      raw.append(buf.data(), len);

      Message msg{raw};

      if (msg.get_action() == Message::Action::send) {
        std::cout << "Received: " << raw << '\n';
        std::shift_left(messages.begin(), messages.end(), 1);
        messages[messages.size() - 1] = msg.get_message();
      } else {
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
