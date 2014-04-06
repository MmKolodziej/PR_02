#include "painter.h"
#include <unistd.h>
#import "global.h"

using namespace std;

Painter::Painter(int i, int paint_ind, int brush_ind) {
    number = i;
    _paint_ind = paint_ind;
    _brush_ind = brush_ind;
    wine_mutex_locked = false;
    brush_mutex_locked = false;
    paint_mutex_locked = false;
    _painted_lines = 0;

    _brush_side = (number % 2 == 0) ? LEFT : RIGHT;
}

void Painter::work() {
    printf(ANSI_COLOR_YELLOW"[INFO] [%i] Painter starts working\n" ANSI_COLOR_RESET, number);

    while(_painted_lines < 10) {
        srand(number);

        int sleep_time = rand()%5+1;

        printf("[INFO] [%i] Painter needs to think for %i seconds\n", number, sleep_time);
        usleep(sleep_time);
        printf("[INFO] [%i] Painter wants to try his luck with painting a line\n", number);
        fflush(stdout);

        paintLine();
    }

    printf(ANSI_COLOR_GREEN "****[FINISH] [%i] Painter finishes his work.\n", number);
    fflush(stdout);
}

void Painter::paintLine() {
    // locks paint at _paint_ind position, 0 <= i <= painters_count/2
    lockPaint();

    // locks paint at _brush_ind position, 0 <= i <= painters_count/2
    lockBrush();

    // locks wine
    lockWine();

    // unlocks 3 mutexes
    releaseStuff();
}

void Painter::lockPaint() {
    printf("[INFO] [%i] Painter tries to obtain paint...\n", number);
    fflush(stdout);

    // Block paints
    if (!paint_mutex_locked)
      pthread_mutex_lock(&paint_mutex[_paint_ind]);

    printf(ANSI_COLOR_YELLOW "[INFO] [PAINT] Left: %i\n", paint_count[_paint_ind]);
    if(paint_count[_paint_ind] > 0)
    {
        paint_count[_paint_ind]--;

        printf(ANSI_COLOR_GREEN "[SUCC] [%i] Painter found paint in his bucket\n" ANSI_COLOR_RESET, number);
        fflush(stdout);
    }
    else {
        printf(ANSI_COLOR_RED "[FAIL] [%i] Painter found no paint in his bucket. Waiting for the filling helper...\n" ANSI_COLOR_RESET, number);
        fflush(stdout);

        // if resources are not available, wait until Helper notifies it's available
        pthread_cond_wait(&paint_cond[_paint_ind], &paint_mutex[_paint_ind]);

        // We have a mutex locked, we go checking again
        paint_mutex_locked = true;
        lockPaint();
    }
}

void Painter::lockBrush() {
    printf("[INFO] [%i] Painter tries to obtain brush\n", number);
    fflush(stdout);

    if (!brush_mutex_locked)
      pthread_mutex_lock(&brush_mutex[_brush_ind]);

    // Brush is cleared or was used by this painter
    if(brush_usage_side[_brush_ind] == CLEAR || brush_usage_side[_brush_ind] == _brush_side)
    {
        // Set brush usage to the painter's side
        brush_usage_side[_brush_ind] = _brush_side;

        printf(ANSI_COLOR_GREEN "[SUCC] [%i] Painter found his brush clear\n" ANSI_COLOR_RESET, number);
        fflush(stdout);
    } else {
        printf(ANSI_COLOR_RED "[FAIL] [%i] Painter could not use brush as it is dirty\n" ANSI_COLOR_RESET, number);
        fflush(stdout);

        // Wait untill Helper clears brush as it's unusable for this Painter
        pthread_cond_wait(&brush_cond[_brush_ind], &brush_mutex[_brush_ind]);

        // We have a mutex locked, we go checking again
        brush_mutex_locked = true;
        lockBrush();
    }
}

void Painter::lockWine() {
  if (!wine_mutex_locked)
    pthread_mutex_lock(&wine_mutex);

  if(wine_count>0)
  {
      wine_count--;

      printf(ANSI_COLOR_YELLOW "[INFO] [Wine] Left: %i\n", wine_count);
      printf(ANSI_COLOR_GREEN "[SUCC] [%i] Painter poured wine into glass\n" ANSI_COLOR_RESET, number);
      fflush(stdout);
      wine_mutex_locked = false;
  }
  else {
      printf(ANSI_COLOR_YELLOW "[INFO] [WINE] Left: %i\n", wine_count);
      printf(ANSI_COLOR_RED "[FAIL] [%i] Painter waiting for wine...\n" ANSI_COLOR_RESET, number);
      fflush(stdout);

      // if wine is not available, wait until Helper notifies it's available
      pthread_cond_wait(&wine_cond, &wine_mutex);

      // if we get a signal that wine has been refilled, we check again if there's enough
      wine_mutex_locked = true;
      lockWine();
  }
}

void Painter::releaseStuff() {
    _painted_lines++;

    printf(ANSI_COLOR_GREEN "[SUCC] [%i] Painter painted a line. Total lines produced: %i\n" ANSI_COLOR_RESET, number, _painted_lines);
    fflush(stdout);

    // Release all mutex after job is finished
    pthread_mutex_unlock(&paint_mutex[_paint_ind]);
    pthread_mutex_unlock(&brush_mutex[_brush_ind]);
    pthread_mutex_unlock(&wine_mutex);

    printf("[INFO] [%i] Painter released bucket, brush and wine.\n", number);
    fflush(stdout);
}