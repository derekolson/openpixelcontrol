/* Copyright 2013 Ka-Ping Yee

Licensed under the Apache License, Version 2.0 (the "License"); you may not
use this file except in compliance with the License.  You may obtain a copy
of the License at: http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied.  See the License for the
specific language governing permissions and limitations under the License. */

#include "spi_ftdi.h"
#include <ftdi.h>

static struct ftdi_context *ftdi;
static uint8_t *ftdi_out_buf = NULL;
static uint8_t pin_state = 0;


void digitalWrite(uint8_t pin, uint8_t value) {
    // store current value
    static uint8_t r = 0;

    if (value)
        r |= pin;
    else
        r &= ~pin;

    ftdi_write_data(ftdi, &r, sizeof(r));
}

void spi_byte(uint8_t byte) {
    int i;

    for (i = 7; i >= 0; i--) { // most significant bit first
        if (byte & (1 << i))
            digitalWrite(MOSI, 1);
        else
            digitalWrite(MOSI, 0);

        // do a clock
        digitalWrite(CLK, 1);
        digitalWrite(CLK, 0);
    }
}

int spi_buf_w(const uint8_t *b, size_t s) {
  int j = 0, pos;

  for (pos = 0; pos < s; pos++) {
    uint8_t bit;

    // most significant bit first
    for (bit = (1 << 7); bit > 0; bit >>= 1) {
      if (b[pos] & bit) {
        pin_state |= MOSI;
        ftdi_out_buf[j++] = pin_state;
      } else {
        pin_state &= ~MOSI;
        ftdi_out_buf[j++] = pin_state;
      }
      pin_state |= CLK;
      ftdi_out_buf[j++] = pin_state;

      pin_state &= ~CLK;
      ftdi_out_buf[j++] = pin_state;
    }
  }
  j = ftdi_write_data(ftdi, ftdi_out_buf, j);

  return j;
}

void spi_write(u8 *tx, u32 len) {
  spi_buf_w(tx, len);
  // int block;

  // while (len) {
  //   block = len > SPI_MAX_WRITE ? SPI_MAX_WRITE : len;
  //   // if (spi_buf_w(tx, block) < block) {
  //   //   fprintf(stderr, "Write failed\n");
  //   // }
  //   spi_buf_w(tx, block);
  //   tx += block;
  //   len -= block;
  // }
}

int init_ftdi(char dev[], u32 spi_speed_hz) {
  if((ftdi = ftdi_new()) == 0) {
    fprintf(stderr, "Error initializing ftdi\n");
    return -1;
  }

  if(ftdi_usb_open(ftdi, 0x0403, 0x6001) < 0) {
    fprintf(stderr, "Can't open ftdi usb device\n");
    return -1;
  }

  if(ftdi_set_bitmode(ftdi, CLK | MOSI, BITMODE_BITBANG) < 0) {
    fprintf(stderr, "Can't set ftdi bitmode\n");
    return -1;
  }
  
  if(ftdi_set_baudrate(ftdi, spi_speed_hz / FTDI_BAUDRATE_MULTIPLIER) < 0) {
    fprintf(stderr, "Can't set ftdi baudrate: %dHz\n", spi_speed_hz);
    return -1;
  }

  fprintf(stderr, "Initialized FTDI - Baudrate: %dHz\n", ftdi->baudrate);
  ftdi_out_buf = calloc(1, SPI_MAX_WRITE * 8 * BYTES_PER_BIT);

  return 0;
}

int close_ftdi() {
  ftdi_disable_bitbang(ftdi);
  ftdi_usb_close(ftdi);
  ftdi_free(ftdi);

  free(ftdi_out_buf);
  ftdi_out_buf = NULL;
  return 0;
}
