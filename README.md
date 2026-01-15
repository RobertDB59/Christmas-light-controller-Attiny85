# ATtiny85 Christmas Lights — Natural Breathing Effect

A minimalist firmware for creating smooth, organic LED fading on an ATtiny85 microcontroller. Uses a Gaussian brightness curve to mimic natural light behavior — perfect for holiday decorations, mood lighting, or atmospheric projects.

## Features
- **Natural fading**: Gaussian curve creates gentle, breathing-like light transitions
- **Dual modes**: 
  - **Standard mode**: Consistent 5-second cycles
  - **Random mode**: Variable 1-20 second cycles for organic variation
- **Pause glow**: LEDs rest at a warm ember glow between cycles
- **Minimal hardware**: Just ATtiny85, MOSFET, and LEDs
- **Energy efficient**: Optimized for battery-powered decorations
- **Safe operation**: Built-in PWM limits prevent overcurrent

## Hardware Requirements
- ATtiny85 microcontroller
- N-channel MOSFET (AO3400 recommended)
- 22Ω gate resistor + 10kΩ pull-down resistor
- LED string (20 parallel LEDs maximum with proper current limiting)
- Mode selection button (optional)

## Safety Note: PWM Current Limiting
The default `MAX_PWM = 200` prevents excessive current through parallel LEDs. In the original setup, 3x AA batteries provided natural current limiting through their internal resistance and voltage drop. 

**For safe operation:**
1. Start with `MAX_PWM = 180` in the code
2. **Always measure actual current** with a multimeter
3. Adjust `MAX_PWM` downward if current exceeds LED specifications
4. Consider adding series resistors for additional protection

Without proper current measurement, LEDs can overheat and fail prematurely.

## Project Structure
```
attiny85-christmas-lights/
├── firmware/
│   └── christmas_lights.ino  # Main Arduino sketch
└── README.md                 # This file
```

## Installation
1. Upload sketch using Arduino IDE with [ATTinyCore](https://github.com/SpenceKonde/ATTinyCore)
2. Connect hardware per schematic
3. Use MODE_PIN (Pin 1) to toggle between standard/random modes
4. **Measure current** at full brightness before extended use

## License
Personal/educational use only. See code header for details.

## Why It Works
Human eyes perceive brightness logarithmically. The Gaussian curve's smooth acceleration/deceleration matches this natural perception, creating light that feels organic rather than mechanical.

---

*Perfect for adding warm, breathing light to holiday decorations without the robotic look of linear fades — but always measure your current!*
