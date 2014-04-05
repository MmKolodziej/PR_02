#include "painter.h"
#include <unistd.h>
#import "global.h"

using namespace std;

Painter::Painter(int i, int ingredient_ind, int pot_ind) {
    number = i;
    _ingredient_ind = ingredient_ind;
    _pot_ind        = pot_ind;

    _produced_gold = 0;

    _pot_side = (number % 2 == 0) ? 1 : -1;
}

void Painter::work() {
    printf(ANSI_COLOR_YELLOW"[INFO] [%i] Painter starts working\n" ANSI_COLOR_RESET, number);

    while(_produced_gold < 10) {
        srand(number);

        int sleep_time = rand()%5+1;

        printf("[INFO] [%i] Painter needs to think for %i seconds\n", number, sleep_time);
        sleep(sleep_time);
        printf("[INFO] [%i] Painter wants to try his luck with making gold\n", number);
        fflush(stdout);

        produceGold();
    }

    printf(ANSI_COLOR_GREEN "****[FINISH] [%i] Painter goes to lunch and finishes his work.\n", number);
    fflush(stdout);
}

void Painter::produceGold() {
    // locks ingredient at _ingredient_ind position, 0 <= i <= alch_count/2
    lockIngredient();

    // locks pot at _pot_ind position, 0 <= i <= alch_count/2
    lockPot();

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

void Painter::lockPot() {
    printf("[INFO] [%i] Painter tries to obtain pot\n", number);
    fflush(stdout);

    pthread_mutex_lock(&pots_mutex[_pot_ind]);

    // Pot is cleared or was used by this painter
    if(pots_usage_side[_pot_ind] == 0 || pots_usage_side[_pot_ind] == _pot_side)
    {
        // Set pot usage to the painter's side
        pots_usage_side[_pot_ind] = _pot_side;

        printf("[INFO] [%i] Painter locked pot\n", number);
        fflush(stdout);
    } else {
        printf(ANSI_COLOR_RED "[FAIL] [%i] Painter could not use pot as it is dirty\n" ANSI_COLOR_RESET, number);
        fflush(stdout);

        // Wait untill Helper clears pot as it's unusable for this Painter
        pthread_cond_wait(&pots_cond[_pot_ind], &pots_mutex[_pot_ind]);

        printf(ANSI_COLOR_GREEN "[SUCC] [%i] Painter found his pot clear\n" ANSI_COLOR_RESET, number);
        fflush(stdout);
    }
}

void Painter::releaseStuff() {
    _produced_gold++;

    printf(ANSI_COLOR_GREEN "[SUCC] [%i] Painter produced gold. Total gold produced: %i\n" ANSI_COLOR_RESET, number, _produced_gold);
    fflush(stdout);

    // Release all mutex after job is finished
    pthread_mutex_unlock(&ingredients_mutex[_ingredient_ind]);
    pthread_mutex_unlock(&pots_mutex[_pot_ind]);

    printf("[INFO] [%i] Painter released bowl & pot.\n", number);
    fflush(stdout);
}