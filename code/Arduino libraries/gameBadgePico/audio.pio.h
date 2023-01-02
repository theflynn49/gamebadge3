// -------------------------------------------------- //
// This file is autogenerated by pioasm; do not edit! //
// -------------------------------------------------- //

#if !PICO_NO_HARDWARE
#include "hardware/pio.h"
#endif

// ----- //
// audio //
// ----- //

#define audio_wrap_target 0
#define audio_wrap 3

static const uint16_t audio_program_instructions[] = {
            //     .wrap_target
    0x80a0, //  0: pull   block                      
    0x6024, //  1: out    x, 4                       
    0x6004, //  2: out    pins, 4                    
    0x0000, //  3: jmp    0                          
            //     .wrap
};

#if !PICO_NO_HARDWARE
static const struct pio_program audio_program = {
    .instructions = audio_program_instructions,
    .length = 4,
    .origin = -1,
};

static inline pio_sm_config audio_program_get_default_config(uint offset) {
    pio_sm_config c = pio_get_default_sm_config();
    sm_config_set_wrap(&c, offset + audio_wrap_target, offset + audio_wrap);
    return c;
}

	//								0 or 1,     0-3,   code pointer,  GPIO pin #
static inline void audio_program_init(PIO pio, uint sm, uint offset, uint pin) {
    pio_sm_config c = audio_program_get_default_config(offset);	//Get PIO structure
    sm_config_set_out_pins(&c, pin, 4);// Map the state machine's OUT pin group to one pin, namely the `pin` parameter to this function.
    for (uint i = 0; i < 4 ; i++) {
		pio_gpio_init(pio, pin + i);	
	}	
    pio_sm_set_consecutive_pindirs(pio, sm, pin, 4, true);		// Set the pin direction to output at the PIO https://raspberrypi.github.io/pico-sdk-doxygen/group__hardware__pio.html
    pio_sm_init(pio, sm, offset, &c);					// Load our configuration, and jump to the start of the program
    pio_sm_set_enabled(pio, sm, true);					// Set the state machine running
}

#endif

