#include "clearing_helper.h"
#include <unistd.h>

ClearingHelper::ClearingHelper(int brush_count) {
    _brush_count = brush_count;
}

void ClearingHelper::work() {
    while (true) {
        int time = rand() % HELPER_TIMEOUT +1;

        printf(ANSI_COLOR_CYAN "[Clearing] Helper will sleep for %i seconds\n" ANSI_COLOR_RESET, time);
        fflush(stdout);

        sleep(time);

        printf(ANSI_COLOR_CYAN "[Clearing] Helper will now clear brushes\n" ANSI_COLOR_RESET);
        fflush(stdout);

        for(int i = 0; i < _brush_count; i++)
        {
            // Lock brush
            pthread_mutex_lock(&brush_mutex[i]);

            brush_usage_side[i] = 0;

            // Notify that the brush's status is set back to 0
            pthread_cond_broadcast(&brush_cond[i]);

            // Unlock brush
            pthread_mutex_unlock(&brush_mutex[i]);
        }

        printf(ANSI_COLOR_CYAN "[Clearing] Helper cleared brushes\n" ANSI_COLOR_RESET);
        fflush(stdout);
    }

}