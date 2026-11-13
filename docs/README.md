# Wokwi Grey Counter Chip

This is a custom chip for [Wokwi](https://wokwi.com/).

## Pin names

| Name    | Description                 |
| ------- | --------------------------- |
| ARST    | Async reset signal (Rising) |
| CLK     | Clock signal (Rising)       |
| CE      | Clock Enable signal         |
| WE      | Write Enable signal         |
| D0..D7  | Data Input bus              |
| Q0..Q7  | Data Output bus             |
| VCC     | Power Supply                |
| GND     | Ground                      |

## Usage

To use this chip in your project, include it as a dependency in your `diagram.json` file:

```json
  "dependencies": {
    "chip-grey-counter": "github:twsimpson/chip-grey-counter-wokwi@1.0.6"
  }
```

Then, add the chip to your circuit by adding a `chip-template` item to the `parts` section of diagram.json:

```json
  "parts": {
    ...,
    { "type": "chip-grey-counter", "id": "chip1" }
  },
```

For a complete example, see [the example project](/diagram.json).
