# i2c_check: Bare-Metal I2C Driver & Chip Identification

This is a diagnostic utility for the **STM32F407G-DISC1** to verify I2C communication with the onboard **CS43L22 Audio DAC**. The project bypasses all HAL/LL libraries to implement a hardware-level driver using direct register manipulation.

## Core Objective
The primary goal of this sub-project was to architect a reliable I2C control plane capable of identifying the DAC revision ID (`0xE3`). This serves as the foundation for the upcoming I2S audio streaming pipeline.

## Technical Implementation
The driver manages the full I2C state machine, specifically handling the "Dummy Write" sequence required for register targeting:

1.  **Master Transmitter Mode:** Send Device Address (`0x94`) + Target Register (`0x01`).
2.  **Repeated Start:** Re-issue Start condition without a Stop to maintain bus ownership.
3.  **Master Receiver Mode:** Send Device Address with Read bit (`0x95`).
4.  **1-Byte Read Sequence:** Specialized timing to disable ACK and generate STOP before the final NACK to prevent bus wedging.



## Hardware & Register Configuration
* **Peripheral:** I2C1 (APB1 @ 42MHz).
* **Pins:** PB6 (SCL), PB9 (SDA) configured in **Alternate Function (AF4)**.
* **Logic:** Open-Drain output with internal pull-up resistors enabled (`PUPDR`).
* **Clock:** 100kHz Standard Mode.

## Documented Hardware Challenges
This repository contains specific implementations to handle the following STM32-specific hardware quirks:

* **The Latched BUSY Flag:** If the bus is initialized while lines are floating, the `SR2->BUSY` bit latches high. This driver implements a **Software Reset (`SWRST`)** sequence at the start of the `init` function to flush the internal logic block.
* **Floating Bus Errors:** Initial testing showed the I2C peripheral refusing to generate a START bit. Diagnosis via the **Input Data Register (`IDR`)** confirmed the lines were at 0V; the fix involved enabling internal pull-ups to ensure a 3.3V idle state.
* **RXNE Timing:** Implemented the strict manufacturer-recommended sequence for single-byte reads to ensure the `STOP` condition is queued before the `ADDR` flag is cleared, preventing the master from requesting a second, unwanted byte.

## Results
Successful execution returns the raw hex value `0xE3` from register `0x01`. 
* **Part ID:** `11100` (CS43L22 confirmed).
* **Revision:** `011` (Silicon Revision 3).

## Usage
1.  Connect STM32F407 Discovery via USB.
2.  Run `make` and flash using OpenOCD or ST-Link.
3.  Monitor the `chip_id` variable in the debugger or observe the LED blink patterns (Green = Success).

---

Since you've got the `0xE3` and the LinkedIn post is ready, are you planning to keep this `i2c_check` as a separate "lab" folder in your main repository, or will you integrate this driver directly into the speaker project files?