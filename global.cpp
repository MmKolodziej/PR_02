#include "global.h"

std::vector<pthread_mutex_t> ingredients_mutex;
std::vector<pthread_mutex_t> pots_mutex;
std::vector<pthread_cond_t>  ingredients_cond;
std::vector<pthread_cond_t>  pots_cond;

std::vector<int> ingredients_count;
std::vector<int> pots_usage_side;

int pots_count;