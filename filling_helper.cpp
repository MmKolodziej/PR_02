#include "filling_helper.h"
#include <unistd.h>

FillingHelper::FillingHelper(int brush_count) {
    _brush_count = brush_count;
}

void FillingHelper::work() {
    while (true) {
        int time = rand()% HELPER_TIMEOUT +1;

        printf(ANSI_COLOR_CYAN "[Ingredients] Helper will sleep for %i seconds\n" ANSI_COLOR_RESET, time);
        fflush(stdout);

        sleep(time);

        printf(ANSI_COLOR_CYAN "[Ingredients] Helper will now fill bowls\n" ANSI_COLOR_RESET);
        fflush(stdout);

        for(int i = 0; i < _brush_count; i++)
        {
            // Try to lock ingredient at i index, where 0 <= i <= total_painters/2;
            pthread_mutex_lock(&ingredients_mutex[i]);

            ingredients_count[i] = 5;

            // Notify that the resource was updated
            pthread_cond_broadcast(&ingredients_cond[i]);

            // Unlock resource
            pthread_mutex_unlock(&ingredients_mutex[i]);
        }

        printf(ANSI_COLOR_CYAN "[Ingredients] Helper filled bowls\n" ANSI_COLOR_RESET);
        fflush(stdout);
    }

}