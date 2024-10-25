#ifndef EVENT_H
#define EVENT_H

#include "types.h"

void add_event(EventQueue* eq, int time_ms, Process* proc, int virtual_page);
Event* get_next_event(EventQueue* eq);

#endif // eVENT_H
