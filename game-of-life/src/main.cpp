#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <iostream>

const int COLOR_ALIVE[4] = {255, 255, 255, 0};
const int COLOR_DEAD[4] = {0, 0, 0, 0};

const unsigned int WINDOW_WIDTH = 400;
const unsigned int WINDOW_HEIGHT = 400;
const unsigned int CELL_SIZE = 4;

const float LIKELIHOOD_STARTS_DEAD = 0.6;

const unsigned int DELAY_REFRESH_MILLIS = 60;

float randf() {
    return (float)(std::rand() / (float)RAND_MAX);
}

class Cell {
  private:
    bool alive;
    SDL_Rect rect;

  public:
    Cell() : alive{false} {}

    Cell(unsigned int x_pos, unsigned int y_pos, bool alive) : alive{alive} {
        rect.x = x_pos;
        rect.y = y_pos;
        rect.h = CELL_SIZE;
        rect.w = CELL_SIZE;
    }

    bool isAlive() const {
        return alive;
    }

    void setLife(bool life) {
        alive = life;
    }

    void draw(SDL_Renderer* renderer) const {
        if (alive) {
            SDL_SetRenderDrawColor(
                renderer, COLOR_ALIVE[0], COLOR_ALIVE[1], COLOR_ALIVE[2], COLOR_ALIVE[3]);
        } else {
            SDL_SetRenderDrawColor(
                renderer, COLOR_DEAD[0], COLOR_DEAD[1], COLOR_DEAD[2], COLOR_DEAD[3]);
        }
        SDL_RenderFillRect(renderer, &rect);
    }
};

template <unsigned int cols, unsigned int rows>
class GameState {
  private:
    unsigned int gameSize;
    Cell state[cols * rows];

  public:
    GameState() : gameSize{cols * rows} {
        for (size_t i = 0; i < gameSize; i++) {
            bool alive = randf() >= LIKELIHOOD_STARTS_DEAD;
            state[i] =
                Cell((i * CELL_SIZE) % WINDOW_WIDTH, (i / cols) * CELL_SIZE, alive);
        }
    };

    void draw(SDL_Renderer* renderer) {
        for (Cell& cell : state) {
            cell.draw(renderer);
        }
    }

    // Count the number of direct neighbours that are alive.
    unsigned int neighbourCount(unsigned int cellIdx) {
        int xIdx = cellIdx % cols;
        int yIdx = cellIdx / cols;

        unsigned int count{};
        for (int xShift = -1; xShift <= 1; xShift++) {
            int xNbhd = xIdx + xShift;

            // Check if we got outside the grid.
            if (xNbhd < 0 || (int)cols <= xNbhd) {
                continue;
            }

            for (int yShift = -1; yShift <= 1; yShift++) {
                if (xShift == 0 && yShift == 0) {
                    continue;
                }
                int yNbhd = yIdx + yShift;

                // Check if we got outside the grid.
                if (yNbhd < 0 || (int)rows <= yNbhd) {
                    continue;
                }

                count += (unsigned int)state[xNbhd + yNbhd * cols].isAlive();
            }
        }
        return count;
    }

    void nextIteration() {
        for (unsigned int cellIdx = 0; cellIdx < gameSize; cellIdx++) {
            unsigned int numNeighbours = neighbourCount(cellIdx);
            if (state[cellIdx].isAlive()) {
                // Death by underpopulation or overpopulation.
                if (numNeighbours < 2 || numNeighbours > 3) {
                    state[cellIdx].setLife(false);
                }
            } else {
                // Reproduction.
                if (numNeighbours == 3) {
                    state[cellIdx].setLife(true);
                }
            }
        }
    }
};

int main() {
    if (SDL_VideoInit(nullptr) != 0) {
        std::cout << "Couldn't initialise SDL video subsystem: " << SDL_GetError()
                  << std::endl;
        return -1;
    }

    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_CreateWindowAndRenderer(WINDOW_WIDTH, WINDOW_HEIGHT, 0, &window, &renderer);

    if (window == nullptr) {
        std::cout << "Couldn't create a new window: " << SDL_GetError() << std::endl;
        return -1;
    }
    if (renderer == nullptr) {
        std::cout << "Couldn't create a new renderer: " << SDL_GetError() << std::endl;
        return -1;
    }

    GameState<WINDOW_HEIGHT / CELL_SIZE, WINDOW_WIDTH / CELL_SIZE> game{};

    SDL_Event event{};
    bool running{true};
    while (running) {
        if (SDL_PollEvent(&event) != 0 && event.type == SDL_QUIT) {
            running = false;
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);

        game.nextIteration();
        game.draw(renderer);

        SDL_RenderPresent(renderer);
        SDL_Delay(DELAY_REFRESH_MILLIS);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
