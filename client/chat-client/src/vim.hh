#ifndef CHAT_CLIENT_VIM_HH
#define CHAT_CLIENT_VIM_HH

#include "window.hh"
#include "program_state.hh"

void useVimMode(State& programState, Window& window);
void useVimMode(int trigger, State& programState, Window& window);

#endif
