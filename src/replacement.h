#ifndef REPLACEMENT_H
#define REPLACEMENT_H

#include "types.h"

int select_victim_page_fifo();
int select_victim_page_lru();
int select_victim_page_lfu();
int select_victim_page_mfu();
int select_victim_page_random();
int select_victim_page();

#endif // rEPLACEMENT_H
