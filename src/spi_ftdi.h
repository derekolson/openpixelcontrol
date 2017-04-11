/* Copyright 2013 Ka-Ping Yee

Licensed under the Apache License, Version 2.0 (the "License"); you may not
use this file except in compliance with the License.  You may obtain a copy
of the License at: http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied.  See the License for the
specific language governing permissions and limitations under the License. */

#ifndef SPI_FTDI
#define SPI_FTDI

#include <stdio.h>
#include <stdlib.h>
#include "types.h"

#define BYTES_PER_BIT 3
#define SPI_BITS_PER_WORD 8
#define SPI_MAX_WRITE 4096
#define SPI_DEFAULT_SPEED_HZ 3000000
#define FTDI_BAUDRATE_MULTIPLIER 4

#define CLK 0x08  /* CTS (brown wire on FTDI cable) */
#define MOSI 0x14 /* DTR */

// void digitalWrite(uint8_t pin, uint8_t value);
// void spi_byte(uint8_t byte);
void spi_write(u8 *tx, u32 len);

int init_ftdi(char dev[], u32 spi_speed_hz);
int close_ftdi();

#endif /* SPI_H */
