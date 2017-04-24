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

#define MAX_DEVICES 2
static int deviceIndex = 0;

static struct mpsse_context *devices[MAX_DEVICES];

void spi_write(int deviceNum, u8 *tx, u32 len) {
  if(devices[deviceNum] == NULL) return;

  Start(devices[deviceNum]);
  FastWrite(devices[deviceNum], tx, len);
  Stop(devices[deviceNum]);
}

int init_ftdi(char dev[], u32 spi_speed_hz) {
  int retVal = -1;
  // struct mpsse_context *device = MPSSE(SPI0, spi_speed_hz, MSB);
  struct mpsse_context *device = Open(0x0403, 0x6014, SPI0, spi_speed_hz, MSB, IFACE_A, NULL, dev);

  if(device == NULL) {
    fprintf(stderr, "Error initializing device: %s\n", dev);
    retVal = -1;
  }

  if(device->open) {
    printf("Device: %s initialized at %dHz (SPI mode 0)\n", dev, GetClock(device));
    retVal = deviceIndex;
  } else {
    fprintf(stderr, "Error opening device: %s\n", dev);
    retVal = -1;
  }

  devices[deviceIndex] = device;
  
  if(retVal == -1) {
    close_ftdi(deviceIndex);
  }

  deviceIndex++;

  return retVal;
}

int close_ftdi(int deviceIndex) {
  Close(devices[deviceIndex]);
  devices[deviceIndex] = NULL;
  return 0;
}
