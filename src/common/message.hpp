// SPDX-License-Identifier: MIT
// Copyright (c) Colin Melican 2025

#ifndef GROUPCHAT_COMMON_MESSAGE_HPP_
#define GROUPCHAT_COMMON_MESSAGE_HPP_

#include <ostream>
#include <string>
#include <string_view>

class Message {
 public:
  enum class Action { get, send };

  Message();
  explicit Message(std::string_view raw);
  explicit Message(Action action);
  Message(Action action, std::string_view message);

  std::string raw() const;

  friend std::ostream& operator<<(std::ostream& os, Action a);

  Action get_action() const;
  void set_action(Action action);
  std::string_view get_message() const;
  void set_message(std::string_view message);

 private:
  Action action_{Action::get};
  std::string message_{};
};

#endif  // GROUPCHAT_COMMON_MESSAGE_HPP_
