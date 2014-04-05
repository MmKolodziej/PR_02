#include <iostream>
#include <pthread.h>

#include <stdio.h>
#include <stdlib.h>

#include "painter.h"
#include "clearing_helper.h"
#include "filling_helper.h"

#include <vector>

#include "global.h"

using namespace std;

void *painterStart(void *p);
void *fillingHelperStart(void *p);
void *clearingHelperStart(void *p);


int main(int argc, const char * argv[])
{
    int i;
    int painters_count = atoi(argv[1]);

    if (painters_count < 4) {
        printf(ANSI_COLOR_RED "Number of painters must be >= 4!\n" ANSI_COLOR_RESET);
        exit(2);
    }

    if (painters_count % 2 != 0) {
        printf(ANSI_COLOR_RED "Number of painters is not even!\n" ANSI_COLOR_RESET);
        exit(2);
    }

    brush_count = painters_count / 2;

    std::vector<pthread_t> t_painters;
    std::vector<pthread_t> t_helpers;

    // Reserve memory for global variables
    t_painters.reserve(painters_count);
    t_helpers.reserve(2);
    pthread_attr_t attr;

    ingredients_mutex.reserve(brush_count);
    ingredients_cond.reserve(brush_count);
    brush_mutex.reserve(brush_count);
    brush_cond.reserve(brush_count);

    ingredients_count.reserve(brush_count);
    brush_usage_side.reserve(brush_count);


    for (i = 0; i < brush_count; i++) {
        pthread_mutex_init(&ingredients_mutex[i], NULL);
        pthread_mutex_init(&brush_mutex[i], NULL);
        pthread_cond_init(&ingredients_cond[i], NULL);
        pthread_cond_init(&brush_cond[i], NULL);
        ingredients_count[i] = 5;
        brush_usage_side[i] = 0;
    }

    std::vector<Painter> painters;
    painters.reserve(painters_count);

    painters.push_back(Painter(0, 0, brush_count-1));


    int p_ind;
    int i_ind;

    for (i = 1; i < painters_count; i++) {
        i_ind = i/2;

        if (i % 2 == 0) {
            p_ind = i/2 - 1;
        } else {
            p_ind = i/2;
        }
        painters.push_back(Painter(i, i_ind, p_ind));
    }

    pthread_attr_init(&attr);

    // Start helpers
    pthread_create(t_helpers.data(),     &attr, &clearingHelperStart, NULL);
    pthread_create(t_helpers.data() + 1, &attr, &fillingHelperStart, NULL);

    // Start painters
    for(i = 0; i < painters_count; i++)
    {
        pthread_create(t_painters.data() + i, &attr, &painterStart, painters.data() +i);
    }

    // Join painters threads so that program stops
    for (i = 0; i < painters_count; i++)
    {
        pthread_join(t_painters[i], NULL);
    }

    pthread_attr_destroy(&attr);


    return 0;
}

void *painterStart(void* p)
{
    Painter *obj = (Painter*)p;

    obj->work();

    pthread_exit(NULL);
}

void *fillingHelperStart(void *p) {
    FillingHelper *fillingObj = new FillingHelper(brush_count);

    printf(ANSI_COLOR_YELLOW "[Helper] Filling Helper starts working\n" ANSI_COLOR_RESET);

    fillingObj->work();

    printf(ANSI_COLOR_YELLOW "Finished!" ANSI_COLOR_YELLOW);

    pthread_exit(NULL);
}

void *clearingHelperStart(void *p) {
    ClearingHelper *clearingObj = new ClearingHelper(brush_count);

    printf(ANSI_COLOR_YELLOW "[Helper] Clearing Helper starts working\n" ANSI_COLOR_RESET);

    clearingObj->work();

    printf(ANSI_COLOR_YELLOW "Finished!" ANSI_COLOR_RESET);

    pthread_exit(NULL);
}
