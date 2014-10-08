// Not used at present...

#include <systemlib/param/param.h>

/**
 * Minimum thrust
 *
 * Minimum vertical thrust. It's recommended to set it > 0 to avoid free fall with zero thrust.
 *
 * @min 0
 * @max 1
 * @group Transition State Controller
 */
PARAM_DEFINE_INT(TS_STATE, 0);
