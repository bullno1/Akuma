#ifndef _AKUMA_INPUT_H_
#define _AKUMA_INPUT_H_

#include <SDL.h>

void initInput();
void injectInput(const SDL_Event& ev);

#endif