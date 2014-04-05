#ifndef __parallel_2__painter__
#define __parallel_2__painter__

#include <iostream>
#include <stdio.h>
#include <stdlib.h>


#include "global.h"

class Painter {
    int _painted_lines;

    // -1 - right, 1 - left, 0 - clear
    int _brush_side;

    int _brush_ind, _paint_ind;

public:
    int number;
    Painter() {};
    Painter(int i, int paint_ind, int brush_ind);

    void work();

private:
    void paintLine();

    void lockPaint();
    void lockBrush();
    void lockWine();
    void releaseStuff();
};

#endif /* defined(__parallel_2__painter__) */
