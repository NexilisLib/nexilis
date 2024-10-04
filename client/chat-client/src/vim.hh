#ifndef CHAT_CLIENT_VIM_HH
#define CHAT_CLIENT_VIM_HH

#include "program_state.hh"
#include "window.hh"

#include <cstdint>
#include <functional>

void useVimMode(State& programState, Window& window, const std::function<void(const std::vector<uint8_t>&)>& sendTCPMessage);
void useVimMode(int trigger, State& programState, Window& window, const std::function<void(const std::vector<uint8_t>&)>& sendTCPMessage);

#endif
