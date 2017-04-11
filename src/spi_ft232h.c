/* Copyright 2013 Ka-Ping Yee

Licensed under the Apache License, Version 2.0 (the "License"); you may not
use this file except in compliance with the License.  You may obtain a copy
of the License at: http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied.  See the License for the
specific language governing permissions and limitations under the License. */

#include "spi_ftdi.h"
#include <mpsse.h>
#include <inttypes.h>

static struct mpsse_context *ftdi = NULL;

void spi_write(u8 *tx, u32 len) {
  //spi_buf_w(tx, len);
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

  Start(ftdi);
  Write(ftdi, tx, len-1);
  Stop(ftdi);

  // for(int i=0; i<30; i++) {
  //   printf("%d ", (unsigned char)tx[i]);
  // }
  // printf("\n"); 
  
}

int init_ftdi(char dev[], u32 spi_speed_hz) {
  if((ftdi = MPSSE(SPI0, TWELVE_MHZ, MSB)) == NULL) {
    fprintf(stderr, "Error initializing ftdi\n");
    return -1;
  }
  
  if(ftdi->open) {
    printf("%s initialized at %dHz (SPI mode 0)\n", GetDescription(ftdi), GetClock(ftdi));
  }

  // if(ftdi_usb_open(ftdi, 0x0403, 0x6001) < 0) {
  //   fprintf(stderr, "Can't open ftdi usb device\n");
  //   return -1;
  // }

  // if(ftdi_set_bitmode(ftdi, CLK | MOSI, BITMODE_BITBANG) < 0) {
  //   fprintf(stderr, "Can't set ftdi bitmode\n");
  //   return -1;
  // }
  
  // if(ftdi_set_baudrate(ftdi, spi_speed_hz / FTDI_BAUDRATE_MULTIPLIER) < 0) {
  //   fprintf(stderr, "Can't set ftdi baudrate: %dHz\n", spi_speed_hz);
  //   return -1;
  // }

  // fprintf(stderr, "Initialized FTDI - Baudrate: %dHz\n", ftdi->baudrate);
  //ftdi_out_buf = calloc(1, SPI_MAX_WRITE * 8 * BYTES_PER_BIT);

  return 0;
}

int close_ftdi() {
  Close(ftdi);
  return 0;
}
