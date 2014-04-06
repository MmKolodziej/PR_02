#include "wine_helper.h"
#include <unistd.h>

WineHelper::WineHelper() {}

void WineHelper::work() {
    while (true) {
        int time = rand() % HELPER_TIMEOUT + 500000;

        printf(ANSI_COLOR_CYAN "[Wine] Helper will sleep for %.3f seconds\n" ANSI_COLOR_RESET, (float)(time)/1000000);
        fflush(stdout);

        usleep(time);

        printf(ANSI_COLOR_CYAN "[Wine] Helper will now refill wine\n" ANSI_COLOR_RESET);
        fflush(stdout);

        // Lock wine
        pthread_mutex_lock(&wine_mutex);

        wine_count = 4;

        // Notify that the has been refilled
        pthread_cond_broadcast(&wine_cond);

        // Unlock wine
        pthread_mutex_unlock(&wine_mutex);

        printf(ANSI_COLOR_CYAN "[Wine] Helper refilled wine\n" ANSI_COLOR_RESET);
        fflush(stdout);
    }

}