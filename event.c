#include "event.h"
#include <stdlib.h>
#include <stdio.h>

// adds an event to the event queue in sorted order
void add_event(EventQueue* eq, int time_ms, Process* proc, int virtual_page) {
    Event* new_event = (Event*)malloc(sizeof(Event));
    if(!new_event) {
        fprintf(stderr, "Memory allocation failed for Event\n");
        exit(EXIT_FAILURE);
    }
    new_event->time_ms = time_ms;
    new_event->proc = proc;
    new_event->virtual_page = virtual_page;
    new_event->next = NULL;

    if(eq->head == NULL || time_ms < eq->head->time_ms) {
        new_event->next = eq->head;
        eq->head = new_event;
    } else {
        Event* current = eq->head;
        while(current->next != NULL && current->next->time_ms <= time_ms) {
            current = current->next;
        }
        new_event->next = current->next;
        current->next = new_event;
    }
}

// retrieves and removes the next event from the event queue
Event* get_next_event(EventQueue* eq) {
    if(eq->head == NULL) return NULL;
    Event* evt = eq->head;
    eq->head = eq->head->next;
    return evt;
}
