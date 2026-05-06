# Car Climate Control Controller

Embedded automotive climate control system developed on an **STM32F429Zi NUCLEO** board using **CMSIS-RTOS2 (RTX)**.

The project simulates the behaviour of a car climate controller by measuring cabin and outside temperature, adjusting a simulated fan speed through PWM, displaying system information on an LCD, and allowing user interaction through a joystick and UART command protocol.

## Overview


The main objective was to design and implement a complete embedded application based on a real-time operating system, integrating multiple peripherals, sensors, user interfaces and communication mechanisms.

The system controls the simulated climate behaviour of a vehicle by comparing the interior temperature with a configurable target temperature. Depending on the temperature difference, the controller activates heating or cooling states using an RGB LED and modifies the fan speed using a PWM signal.

In addition, the system monitors the outside temperature and warns the user when it falls below a configurable alarm threshold.

## Main Features

- Real-time embedded application using **CMSIS-RTOS2 / RTX**
- Modular firmware architecture
- Interior and exterior temperature monitoring using **LM75 sensors**
- Temperature readings every **500 ms**
- System clock with **1-second resolution**
- Four operating modes:
  - Standby mode
  - Active climate control mode
  - Test mode
  - Programming/debugging mode
- LCD-based user interface
- Joystick control with:
  - Interrupt-based input handling
  - Debouncing
  - Short and long press detection
- RGB LED state indication for heating/cooling/neutral states
- PWM output at **1 kHz** to simulate fan speed control
- Circular buffer for storing recent measurements
- UART/RS232 command protocol for configuration and debugging
- External temperature alarm using LM75 hardware alert signal and interrupt handling

## Hardware Used

- **NUCLEO STM32F429Zi**
- **mbed Application Board**
- **LM75 temperature sensor** for interior temperature
- **External LM75 temperature sensor** for outside temperature
- LCD display
- Joystick
- RGB LED
- LD1 LED from the NUCLEO board
- Potentiometers POT_1 and POT_2
- UART connection to PC
- Oscilloscope or logic analyser for PWM verification

## Software and Tools

- **C**
- **Keil µVision**
- **STM32 HAL**
- **CMSIS-RTOS2 / RTX**
- **UART / RS232**
- **I2C**
- **SPI**
- **PWM**
- **GPIO interrupts**
- **Tera Term** for UART command testing

## System Modes

### Standby Mode

After reset, the system starts in standby mode.

The LCD displays the system title and the current time, which starts from `00:00:00`.

The system remains in this mode until a long press on the joystick center button changes the state to active mode.

### Active Mode

In active mode, the system continuously reads the interior temperature and compares it with the target temperature.

The control variable is:

```text
x = Tc - Ti
```

Where:

- `Tc` = target cabin temperature
- `Ti` = measured interior temperature

Depending on this difference, the system adjusts:

- RGB LED colour
- PWM duty cycle
- Simulated fan speed

The outside temperature is also monitored and displayed.

If the outside temperature falls below the alarm temperature, the system activates a visual warning through the LD1 LED.

### Test Mode

Test mode allows manual simulation of the system variables without relying entirely on sensor readings.

The following variables can be modified manually:

- Interior temperature
- Target temperature
- Exterior temperature
- Alarm temperature

This mode is useful for validating the control logic, LED behaviour, PWM output and LCD display.

### Programming and Debugging Mode

This mode allows the user to configure system parameters such as:

- System time
- Target temperature
- Alarm temperature

Configuration can be performed using the joystick or through UART commands.

## Climate Control Logic

The system uses the difference between the target temperature and the measured interior temperature to determine the heating/cooling state and the PWM duty cycle.

| Temperature difference `x = Tc - Ti` | RGB LED | PWM Duty Cycle |
|---|---|---|
| `+5 ºC <= x` | Red | 100% |
| `+2 ºC <= x < +5 ºC` | Red | 70% |
| `+0.5 ºC <= x < +2 ºC` | Red | 40% |
| `-0.5 ºC <= x < +0.5 ºC` | Green | 10% |
| `-2 ºC <= x < -0.5 ºC` | Blue | 40% |
| `-5 ºC <= x < -2 ºC` | Blue | 70% |
| `x < -5 ºC` | Blue | 100% |

## UART Communication Protocol

The system can be controlled from a PC using a UART/RS232 communication channel.

UART configuration:

```text
Baud rate: 115200
Data bits: 8
Stop bits: 1
Parity: None
```

All frames follow this format:

```text
SOH CMD LEN Payload EOT
```

Where:

| Field | Description |
|---|---|
| SOH | Start of frame, `0x01` |
| CMD | Command byte |
| LEN | Total frame length, including SOH and EOT |
| Payload | ASCII-encoded command data |
| EOT | End of frame, `0xFE` |

Malformed frames are ignored by the system.

## Supported UART Commands

| Command | CMD | Description |
|---|---:|---|
| Set system time | `0x20` | Sets the internal clock |
| Set target temperature | `0x25` | Updates the desired cabin temperature |
| Set alarm temperature | `0x26` | Updates the exterior temperature alarm threshold |
| Request all measurements | `0x55` | Sends all stored measurements from the circular buffer |
| Clear measurements | `0x60` | Clears the circular measurement buffer |

## Measurement Buffer

In active mode, the system stores the last 10 measurements in a circular buffer.

Each stored entry follows this format:

```text
HH:MM:SS--Ti:XX.Xº--Tc:YY.Yº--D:ZZ%
```

Where:

- `HH:MM:SS` = timestamp
- `Ti` = interior temperature
- `Tc` = target temperature
- `D` = PWM duty cycle

## Project Structure

The firmware was designed following a modular approach.

Each module is responsible for one specific peripheral, sensor or system functionality.


## Skills Demonstrated

This project demonstrates practical experience in:

- Embedded C programming
- Real-time operating systems
- STM32 firmware development
- Peripheral configuration
- Sensor integration
- I2C communication
- SPI display control
- UART protocol design
- Interrupt-driven programming
- PWM generation
- Modular firmware architecture
- Embedded systems debugging
- Hardware/software integration

## Author

**Marcos Indiano**  
Electronic Engineering Student  
Universidad Politécnica de Madrid
