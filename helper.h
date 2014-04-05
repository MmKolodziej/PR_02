#ifndef __parallel_2__helper__
#define __parallel_2__helper__

#include <iostream>
#include <vector>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

class Helper {
protected:
    int _brush_count;

public:
    virtual void work() {};
};

#endif /* defined(__parallel_2__helper__) */
