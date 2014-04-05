#ifndef __parallel_2__painter__
#define __parallel_2__painter__

#include <iostream>
#include <stdio.h>
#include <stdlib.h>


#include "global.h"

class Painter {
    int _produced_gold;

    // -1 - right, 1 - left, 0 - clear
    int _pot_side;

    int _pot_ind, _ingredient_ind;

public:
    int number;
    Painter() {};
    Painter(int i, int ingredient_ind, int pot_ind);

    void work();

private:
    void produceGold();

    void lockIngredient();
    void lockPot();
    void releaseStuff();
};

#endif /* defined(__parallel_2__painter__) */
