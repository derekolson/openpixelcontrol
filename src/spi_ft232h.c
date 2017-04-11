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
  Start(ftdi);
  Write(ftdi, tx, len-1);
  Stop(ftdi);
}

int init_ftdi(char dev[], u32 spi_speed_hz) {
  if((ftdi = MPSSE(SPI0, TWELVE_MHZ, MSB)) == NULL) {
    fprintf(stderr, "Error initializing ftdi\n");
    return -1;
  }
  
  if(ftdi->open) {
    printf("%s initialized at %dHz (SPI mode 0)\n", GetDescription(ftdi), GetClock(ftdi));
  }

  return 0;
}

int close_ftdi() {
  Close(ftdi);
  return 0;
}
