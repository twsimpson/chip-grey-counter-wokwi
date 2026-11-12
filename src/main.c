// For information and examples see:
// https://link.wokwi.com/custom-chips-alpha

#include "wokwi-api.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  pin_t arst_pin;
  pin_t clk_pin;
  pin_t ce_pin;
  pin_t we_pin;
  pin_t input_bus[8];
  pin_t output_bus[8];
  uint8_t value;
} chip_state_t;

static inline void bus_init(pin_t* pins, char prefix, uint32_t mode)
{
  char pin_name[3] = { prefix, '0', '\0' };
  for (size_t index = 0; index < 8; ++index)
  {
    pin_name[1] = (char)((uint8_t)'0' + index);
    pins[index] = pin_init(pin_name, mode);
  }
}

static void chip_pin_change(void* user_data, pin_t pin, uint32_t value);

void chip_init(void) {
  chip_state_t* chip = malloc(sizeof(chip_state_t));

  chip->arst_pin = pin_init("ARST", INPUT);
  chip->clk_pin = pin_init("CLK", INPUT);
  chip->ce_pin = pin_init("CE", INPUT);
  chip->we_pin = pin_init("WE", INPUT);
  bus_init(chip->input_bus, 'D', INPUT);
  bus_init(chip->output_bus, 'Q', OUTPUT_LOW);
  chip->value = 0;

  const pin_watch_config_t pin_watch_config = {
    .user_data = chip,
    .edge = BOTH,
    .pin_change = chip_pin_change
  };

  pin_watch(chip->arst_pin, &pin_watch_config);
  pin_watch(chip->clk_pin, &pin_watch_config);

  puts("Online!");
}

static inline uint8_t bus_read(const pin_t* pins)
{
  uint8_t value = 0;
  for (size_t index = 0; index < 8; ++index)
  {
    if (pin_read(pins[index]) != LOW)
    {
      value |= (1 << index);
    }
  }
  return value;
}

static inline void bus_write(const pin_t* pins, uint8_t value)
{
  value = value ^ (value >> 1);
  for (size_t index = 0; index < 8; ++index)
  {
    pin_write(pins[index], ((value >> index) & 1) == 0 ? LOW : HIGH);
  }
}

void chip_pin_change(void* user_data, pin_t pin, uint32_t value)
{
  chip_state_t* chip = (chip_state_t*)user_data;

  if (pin == chip->arst_pin)
  {
    if (value != LOW)
    {
      chip->value = 0;
    }
  }
  else if (pin == chip->clk_pin)
  {
    if (value != LOW)
    {
      if (pin_read(chip->ce_pin) != LOW)
      {
        if (pin_read(chip->we_pin) != LOW)
        {
          chip->value = bus_read(chip->input_bus);
        }
        else
        {
          chip->value = chip->value + 1;
        }
      }
    }
  }
  bus_write(chip->output_bus, chip->value);
}
