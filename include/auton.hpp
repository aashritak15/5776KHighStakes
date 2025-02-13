#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "piston.hpp"
#include "intake.hpp"
#include "globals.hpp"
#include "ladybrown.hpp"
#include "magic.hpp"
#include <cmath>

#include "pros/apix.h"

extern int auton_selection;

static void auton_btn_event_handler(lv_event_t * e);
void auton_selector();
void initializeAutonSelector();
void autonFromSelector();