// SPDX-License-Identifier: MIT
// Copyright (c) Colin Melican 2025
// Based off of code from https://think-async.com

#include <cstring>
#include <iostream>
#include <ostream>
#include <string>
#include <system_error>

#include <asio.hpp>

using asio::ip::tcp;

enum class Action {
  get,
  send,
};

std::ostream& operator<<(std::ostream& os, Action a) {
  if (a == Action::get) {
    os << "get";
  } else {
    os << "send";
  }
  return os;
}

int main(int argc, char* argv[]) {
  Action action{Action::get};
  std::string host{"kolin63.com"};
  std::string port{"63101"};
  switch (argc) {
    case 4:
      port = argv[3];
      [[fallthrough]];
    case 3:
      host = argv[2];
      [[fallthrough]];
    case 2:
      using enum Action;
      action = (std::strcmp(argv[1], "send") == 0 ? send : get);
      break;
    case 1:
    default:
      std::cout << "Usage: client <get|send> [host] [port]\n";
      std::cout << "Default host is kolin63.com\n";
      std::cout << "Default port is 63101\n";
      return 1;
  }

  try {
    asio::io_context io_context;

    tcp::resolver resolver{io_context};
    tcp::resolver::results_type endpoints{resolver.resolve(host, port)};

    tcp::socket socket{io_context};
    asio::connect(socket, endpoints);

    std::cout << "Connected to " << host << " on port " << port << '\n';

    if (action == Action::send) {
      std::string message{};
      std::cout << "Please enter your message:\n> ";
      std::getline(std::cin, message, '\n');
      if (message.length() > 50) message = message.substr(0, 50);
      asio::write(socket, asio::buffer(message));
    }

    if (action == Action::get) {
      while (true) {
        std::array<char, 128> buf;
        std::error_code error;

        size_t len{socket.read_some(asio::buffer(buf), error)};

        if (error == asio::error::eof)
          break;  // Connection closed cleanly by peer.
        else if (error)
          throw std::system_error(error);  // Some other error

        std::cout.write(buf.data(), len) << '\n';
      }
    }
  } catch (std::exception& e) {
    std::cerr << e.what() << '\n';
  }

  return 0;
}
