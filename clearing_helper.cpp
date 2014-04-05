#include "clearing_helper.h"
#include <unistd.h>

ClearingHelper::ClearingHelper(int pots_count) {
    _pots_count = pots_count;
}

void ClearingHelper::work() {
    while (true) {
        int time = rand() % HELPER_TIMEOUT +1;

        printf(ANSI_COLOR_CYAN "[Clearing] Helper will sleep for %i seconds\n" ANSI_COLOR_RESET, time);
        fflush(stdout);

        sleep(time);

        printf(ANSI_COLOR_CYAN "[Clearing] Helper will now clear pots\n" ANSI_COLOR_RESET);
        fflush(stdout);

        for(int i = 0; i < _pots_count; i++)
        {
            // Lock pot
            pthread_mutex_lock(&pots_mutex[i]);

            pots_usage_side[i] = 0;

            // Notify that the pot's status is set back to 0
            pthread_cond_broadcast(&pots_cond[i]);

            // Unlock pot
            pthread_mutex_unlock(&pots_mutex[i]);
        }

        printf(ANSI_COLOR_CYAN "[Clearing] Helper cleared pots\n" ANSI_COLOR_RESET);
        fflush(stdout);
    }

}