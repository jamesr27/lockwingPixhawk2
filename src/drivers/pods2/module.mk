# James' pods driver
# This is a copy of px4io, modified to send pod can messages.
#
# Interface driver for the PX4IO board.
#

MODULE_COMMAND		= px4io

SRCS			= pods2.cpp \
			  pods2_uploader.cpp \
			  pods2_serial.cpp \
			  pods2_i2c.cpp

# XXX prune to just get UART registers
INCLUDE_DIRS    += $(NUTTX_SRC)/arch/arm/src/stm32 $(NUTTX_SRC)/arch/arm/src/common

MODULE_STACKSIZE = 1200

EXTRACXXFLAGS	= -Weffc++
