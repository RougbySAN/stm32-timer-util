# stm32-timer-util
Timer util for stm32 microcontrollers that uses a single timer with overflow protection.

## Features
- High-precision timing with support for 16-bit or 32-bit hardware timers.
- Overflow handling for extended timing using a `uint64_t` variable.
- Configurable timeout intervals in microseconds or milliseconds.
- Easy integration into STM32 projects.

## Requirements
- STM32 microcontroller or similar hardware with 16-bit or 32-bit timers.
- STM32CubeIDE or other STM32 development environments.

## Setup

1. Clone this repository in your STM32 project workspace:
   ```sh
   git clone https://github.com/RougbySAN/stm32-timer-util
2. Add stm32-timer-util folder to you include files from Project-> Properties -> Settings -> Include Paths
3. Open a timer and configure the prescaler so that every timer tick is a microsecond. (Or make you own configuration and update some functions in the source code)
4. Configure the Timer to be used as LL from Project Manager-> Advanced Settings-> TIM-> Selected timer
5. Include the header file in you project files where you want to use it.
6. Call timer_util_init(TIM2); in you initialization sequence.
7. Select the size of the timer from timer_util.h (Ref@ TIMER_UTIL_TIMER_SIZE)
8. Ready to Go!

## Options

Interrupt:
You can place timer_util_isr() in Timer interrupt handler. The isr will add the ARR amount to the
global timer variable. However, for very fast timers with quick interrupt triggers, frequent interrupts
could overwhelm the rest of the system. Also, Interrupts introduces a system that is not deterministic
which could trigger in between any two lines of codes.

Polling:
I prefer to use the polling method due to the fact that i can determine when to handle the overflow case.
For polling there is one important consideration which is that you have to call timer_util_get_tick() function
which checks for the overflow flag and handles the overflow of the timer. The timer_util_get_tick() function must be called at least once before 
another overflow happens. In short prevent two overflows from happening. This is not difficult since even for a 16-bit timer with 1us tick period
overflows in ~65 milliseconds.

## Performance Measurement
There are performence measurement utility structure and functions in the library that simplifys the debugging process and provide
useful information for external systems and telemetry.

## Elapsed Time Measurement
One of the main uses of the timer is to measure elapsed time for main loops and message timers.

## Contribution
Feel free to fork and share your real-life experiance on you project and suggest improvements.
