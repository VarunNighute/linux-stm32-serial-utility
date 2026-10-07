# Linux–STM32 Serial Communication Utility

A Linux-based UART communication utility for controlling and communicating with an STM32F103C8T6 using a simple command interface.

## Overview

This project demonstrates host-to-microcontroller communication between a Raspberry Pi running Linux and an STM32F103C8T6 over UART.

The STM32 firmware is written in bare-metal C using direct register access. The Linux host programs use POSIX serial APIs and `termios` to configure and communicate with the UART interface.

## Features

- Bare-metal STM32 UART communication
- Register-level USART1 configuration
- 115200 baud UART communication
- Command-based host-to-microcontroller interface
- LED ON/OFF control
- Device status reporting
- Invalid-command handling
- Command-length protection
- Linux POSIX `termios` serial communication
- ARM GCC and Makefile build system
- ST-Link and OpenOCD firmware flashing

## Hardware

- Raspberry Pi 3B+
- STM32F103C8T6 Blue Pill
- ST-Link V2

## UART Connections

| Raspberry Pi | STM32F103 |
|---|---|
| GPIO14 (TX) | PA10 (RX) |
| GPIO15 (RX) | PA9 (TX) |
| GND | GND |

## Communication Flow

```text
+-------------------+       UART        +-------------------+
| Raspberry Pi 3B+  | <---------------> | STM32F103C8T6    |
| Linux             |                   | Bare-metal C     |
| POSIX / termios   |                   | USART1           |
+-------------------+                   +-------------------+
                                               |
                                               v
                                         On-board LED
