// SPDX-License-Identifier: MIT
// Copyright (c) Colin Melican 2025
// Based off of code from https://think-async.com

#include <cstring>
#include <iostream>
#include <ostream>
#include <string>
#include <system_error>

#include <asio.hpp>

#include "../common/message.hpp"

using asio::ip::tcp;

int main(int argc, char* argv[]) {
  Message msg;
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
      using enum Message::Action;
      msg.set_action(std::strcmp(argv[1], "send") == 0 ? send : get);
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

    if (msg.get_action() == Message::Action::send) {
      std::string text{};
      std::cout << "Please enter your message:\n> ";
      std::getline(std::cin, text, '\n');

      if (text.length() > 50) text = text.substr(0, 50);

      msg.set_message(text);

      std::cout << "raw: " << msg.raw() << '\n';
      asio::write(socket, asio::buffer(msg.raw()));
    } else if (msg.get_action() == Message::Action::get) {
      std::cout << "raw: " << msg.raw() << '\n';
      asio::write(socket, asio::buffer(msg.raw()));
      socket.shutdown(tcp::socket::shutdown_send);
      while (true) {
        std::array<char, 128> buf;
        std::error_code error;

        std::cout << "reading... " << std::endl;
        size_t len{socket.read_some(asio::buffer(buf), error)};
        std::cout << "done\n";

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
