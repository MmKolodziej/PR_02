#include "global.h"

std::vector<pthread_mutex_t> ingredients_mutex;
std::vector<pthread_mutex_t> brush_mutex;
std::vector<pthread_cond_t>  ingredients_cond;
std::vector<pthread_cond_t>  brush_cond;

std::vector<int> ingredients_count;
std::vector<int> brush_usage_side;

int brush_count;