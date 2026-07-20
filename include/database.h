#pragma once

#include "event.h"

#include <vector>


void initializeDatabase();

void addEvent(Event event);

std::vector<Event> getEvents();

void deleteEvent(int id);

void updateEvent(int id, Event event);