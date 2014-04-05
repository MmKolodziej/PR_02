#include "painter.h"
#include <unistd.h>
#import "global.h"

using namespace std;

Painter::Painter(int i, int ingredient_ind, int brush_ind) {
    number = i;
    _ingredient_ind = ingredient_ind;
    _brush_ind        = brush_ind;

    _painted_lines = 0;

    _brush_side = (number % 2 == 0) ? 1 : -1;
}

void Painter::work() {
    printf(ANSI_COLOR_YELLOW"[INFO] [%i] Painter starts working\n" ANSI_COLOR_RESET, number);

    while(_painted_lines < 10) {
        srand(number);

        int sleep_time = rand()%5+1;

        printf("[INFO] [%i] Painter needs to think for %i seconds\n", number, sleep_time);
        sleep(sleep_time);
        printf("[INFO] [%i] Painter wants to try his luck with painting a line\n", number);
        fflush(stdout);

        paintLine();
    }

    printf(ANSI_COLOR_GREEN "****[FINISH] [%i] Painter goes to lunch and finishes his work.\n", number);
    fflush(stdout);
}

void Painter::paintLine() {
    // locks ingredient at _ingredient_ind position, 0 <= i <= alch_count/2
    lockIngredient();

    // locks brush at _brush_ind position, 0 <= i <= alch_count/2
    lockBrush();

    // unlocks 2 mutexes
    releaseStuff();
}

void Painter::lockIngredient() {
    printf("[INFO] [%i] Painter tries to obtain ingredient...\n", number);
    fflush(stdout);

    // Block ingredients
    pthread_mutex_lock(&ingredients_mutex[_ingredient_ind]);

    if(ingredients_count[_ingredient_ind] >= 1)
    {
        ingredients_count[_ingredient_ind]--;

        printf(ANSI_COLOR_YELLOW "[INFO] [POT] Left: %i\n", ingredients_count[_ingredient_ind]);
        printf(ANSI_COLOR_GREEN "[SUCC] [%i] Painter found an ingredient in his bowl\n" ANSI_COLOR_RESET, number);
        fflush(stdout);
    }
    else {
        printf(ANSI_COLOR_YELLOW "[INFO] [POT] Left: %i\n", ingredients_count[_ingredient_ind]);
        printf(ANSI_COLOR_RED "[FAIL] [%i] Painter found the bowl empty. Waiting for the filling helper...\n" ANSI_COLOR_RESET, number);
        fflush(stdout);

        // if resources are not available, wait until Helper notifies it's available
        pthread_cond_wait(&ingredients_cond[_ingredient_ind], &ingredients_mutex[_ingredient_ind]);
    }
}

void Painter::lockBrush() {
    printf("[INFO] [%i] Painter tries to obtain brush\n", number);
    fflush(stdout);

    pthread_mutex_lock(&brush_mutex[_brush_ind]);

    // Pot is cleared or was used by this painter
    if(brush_usage_side[_brush_ind] == 0 || brush_usage_side[_brush_ind] == _brush_side)
    {
        // Set brush usage to the painter's side
        brush_usage_side[_brush_ind] = _brush_side;

        printf("[INFO] [%i] Painter locked brush\n", number);
        fflush(stdout);
    } else {
        printf(ANSI_COLOR_RED "[FAIL] [%i] Painter could not use brush as it is dirty\n" ANSI_COLOR_RESET, number);
        fflush(stdout);

        // Wait untill Helper clears brush as it's unusable for this Painter
        pthread_cond_wait(&brush_cond[_brush_ind], &brush_mutex[_brush_ind]);

        printf(ANSI_COLOR_GREEN "[SUCC] [%i] Painter found his brush clear\n" ANSI_COLOR_RESET, number);
        fflush(stdout);
    }
}

void Painter::releaseStuff() {
    _painted_lines++;

    printf(ANSI_COLOR_GREEN "[SUCC] [%i] Painter painted a line. Total lines produced: %i\n" ANSI_COLOR_RESET, number, _painted_lines);
    fflush(stdout);

    // Release all mutex after job is finished
    pthread_mutex_unlock(&ingredients_mutex[_ingredient_ind]);
    pthread_mutex_unlock(&brush_mutex[_brush_ind]);

    printf("[INFO] [%i] Painter released bowl & brush.\n", number);
    fflush(stdout);
}