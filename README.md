# Linux–STM32 Serial Communication Utility

A bare-metal STM32F103 UART command interface controlled from a Linux system over a serial connection.

## Overview

This project implements a simple host-to-microcontroller command interface between a Linux system and an STM32F103C8T6.

The STM32 firmware is written in bare-metal C using direct register access, while the host communicates with the microcontroller through UART.

## Features

- Register-level USART1 configuration
- UART communication at 115200 baud
- Command-based communication
- LED control
- Device status reporting
- Invalid-command detection
- Command-length protection
- Bare-metal STM32 firmware
- ARM GCC + Makefile build system
- ST-Link + OpenOCD flashing

## Hardware

- Raspberry Pi 3B+
- STM32F103C8T6 Blue Pill
- ST-Link V2

## UART Connections

| Raspberry Pi | STM32F103 |
|---|---|
| GPIO14 TX | PA10 RX |
| GPIO15 RX | PA9 TX |
| GND | GND |

## Commands

```text
LED ON
LED OFF
STATUS
