#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>

bool is_running = false;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* color_buffer_texture = NULL;

uint32_t* color_buffer = NULL;
int window_width = 800;
int window_height = 600;

bool initialize_window(void) {
  if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
    fprintf(stderr, "error initializing sdl.\n");
    return false;
  }
  // create sdl window
  window = SDL_CreateWindow(
    NULL,
    SDL_WINDOWPOS_CENTERED,
    SDL_WINDOWPOS_CENTERED,
    window_width,
    window_height,
    SDL_WINDOW_BORDERLESS
  );

  if (!window) {
    fprintf(stderr, "error creating window.\n");
    return false;
  }

  // create sdl renderer
  renderer = SDL_CreateRenderer(window, -1, 0);

  if (!renderer) {
    fprintf(stderr, "error creating renderer.\n");
    return false;
  }

  return true;
}

void setup(void) {
  color_buffer = (uint32_t*) malloc(sizeof(uint32_t) * window_width * window_height);
  color_buffer_texture = SDL_CreateTexture(
    renderer,
    SDL_PIXELFORMAT_ARGB8888,
    SDL_TEXTUREACCESS_STREAMING,
    window_width,
    window_height
  );
}

void process_input(void) {
  SDL_Event event;
  while(SDL_PollEvent(&event)) {
    switch (event.type) {
     case SDL_QUIT:
       is_running = false;
       break;
     case SDL_KEYDOWN:
       if (event.key.keysym.sym == SDLK_ESCAPE) {
         is_running = false;
       }
       break;
    }
  }
}

void update(void) {

}

void clear_color_buffer(uint32_t color) {
  for (int row = 0; row < window_height; row++) {
    for (int col = 0; col < window_width; col++) {
      color_buffer[(window_width * row) + col] = color;
    }
  }
}

void render_color_buffer(void) {
  SDL_UpdateTexture(
    color_buffer_texture,
    NULL,
    color_buffer,
    (int)(window_width * sizeof(uint32_t))
  );
  SDL_RenderCopy(
    renderer,
    color_buffer_texture,
    NULL,
    NULL
  );
}

void render(void) {
  //SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
  //SDL_RenderClear(renderer);

  render_color_buffer();
  clear_color_buffer(0xFFFFFF00);

  SDL_RenderPresent(renderer);
}

void destroy_window(void) {
  free(color_buffer);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_DestroyTexture(color_buffer_texture);
  SDL_Quit();
}

int main(void) {
  is_running = initialize_window();

    setup();

    while (is_running) {
      process_input();
      update();
      render();
    }

  destroy_window();

  return 0;
}
