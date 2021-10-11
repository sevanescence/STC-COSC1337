#pragma once
#include "EventHandler.h"
#include "Database.h"

namespace Application {
	bool ShouldExit = false;
	EventHandler AppEventHandler;
	Database<Movie, 5, 2> MovieDatabase;


};
