#include "filling_helper.h"
#include <unistd.h>

FillingHelper::FillingHelper(int brush_count) {
    _brush_count = brush_count;
}

void FillingHelper::work() {
    while (true) {
        int time = rand()% HELPER_TIMEOUT + 500000;

        printf(ANSI_COLOR_CYAN "[Paints] Helper will sleep for %i seconds\n" ANSI_COLOR_RESET, time);
        fflush(stdout);

        usleep(time);

        printf(ANSI_COLOR_CYAN "[Paints] Helper will now fill buckets\n" ANSI_COLOR_RESET);
        fflush(stdout);

        for(int i = 0; i < _brush_count; i++)
        {
            // Try to lock paint at i index, where 0 <= i <= total_painters/2;
            pthread_mutex_lock(&paint_mutex[i]);

            paint_count[i] = 3;

            // Notify that the resource was updated
            pthread_cond_broadcast(&paint_cond[i]);

            // Unlock resource
            pthread_mutex_unlock(&paint_mutex[i]);
        }

        printf(ANSI_COLOR_CYAN "[Paints] Helper filled buckets\n" ANSI_COLOR_RESET);
        fflush(stdout);
    }

}