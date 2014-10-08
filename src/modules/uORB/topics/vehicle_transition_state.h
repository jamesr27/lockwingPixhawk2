// James' vehicle_transition_state uORB.
//	The transition_state_controller sets this value depending on commanded vehicle state.
//	The controllers will subscribe to this uORB and behave appropriately, depending on its value.

#ifndef VEHICLE_TRANSITION_STATE_H_
#define VEHICLE_TRANSITION_STATE_H_

#include <stdint.h>
#include <stdbool.h>
#include "../uORB.h"

/**
 * @addtogroup topics
 * @{
 */

struct vehicle_transition_state_s {
	int vehicle_state;
};

/**
 * @}
 */

/* register this as object request broker structure */
ORB_DECLARE(vehicle_transition_state);

#endif
