// SPDX-License-Identifier: MIT
// Copyright (c) Colin Melican 2025

#include "message.hpp"

#include <iostream>
#include <string>
#include <string_view>

Message::Message() {}
Message::Message(std::string_view raw) {
  if (raw.length() >= 1) {
    if (raw[0] == 's')
      action_ = Action::send;
    else
      action_ = Action::get;
  }
  if (raw.length() > 1) {
    message_ = raw.substr(1, raw.length() - 1);
  }
}
Message::Message(Action action) : action_{action} {}
Message::Message(Action action, std::string_view message)
    : action_{action}, message_{message} {}

void Message::operator=(std::string_view raw) {
  if (raw.length() >= 1) {
    if (raw[0] == 'p')
      action_ = Action::send;
    else
      action_ = Action::get;
  }
  if (raw.length() > 1) {
    message_ = raw.substr(1, raw.length() - 2);
  }
}

std::string Message::raw() const {
  const char a{(action_ == Action::send) ? 's' : 'g'};
  return a + message_;
}

std::ostream& operator<<(std::ostream& os, Message::Action a) {
  if (a == Message::Action::get) {
    os << "get";
  } else {
    os << "send";
  }
  return os;
}

Message::Action Message::get_action() const { return action_; }
void Message::set_action(Action action) { action_ = action; }
std::string_view Message::get_message() const { return message_; }
void Message::set_message(std::string_view message) { message_ = message; }
