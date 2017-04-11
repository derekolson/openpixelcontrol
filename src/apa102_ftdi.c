/* Copyright 2016 Ka-Ping Yee

Licensed under the Apache License, Version 2.0 (the "License"); you may not
use this file except in compliance with the License.  You may obtain a copy
of the License at: http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied.  See the License for the
specific language governing permissions and limitations under the License. */

#include "cli.h"
#include "opc.h"
#include "spi_ftdi.h"
#include <math.h>

#define APA102_BRIGHTNESS 31  /* overall brightness level, 0 to 31 */

static u8 buffer[4 + OPC_MAX_PIXELS_PER_MESSAGE * 4];
static int spi_fd;

static u8 gamma_table_red[256];
static u8 gamma_table_green[256];
static u8 gamma_table_blue[256];

void apa102_put_pixels(u8* buffer, u16 count, pixel* pixels) {
  int i;
  pixel* p;
  u8* d;
  u8 flag;
  u8 r, g, b;

  d = buffer;
  *d++ = 0;
  *d++ = 0;
  *d++ = 0;
  *d++ = 0;
  for (i = 0, p = pixels; i < count; i++, p++) {
    r = gamma_table_red[p->r];
    g = gamma_table_green[p->g];
    b = gamma_table_blue[p->b];

    *d++ = 0xe0 + APA102_BRIGHTNESS;
    *d++ = b;
    *d++ = g;
    *d++ = r;
  }
  spi_write(buffer, d - buffer);
}

void set_gamma(double gamma_red, double gamma_green, double gamma_blue) {
  int i;
  for(i=0; i<256; i++) {
    gamma_table_red[i] = (uint8_t)(pow(i/255.0,gamma_red)*255.0+0.5);
    gamma_table_green[i] = (uint8_t)(pow(i/255.0,gamma_green)*255.0+0.5);
    gamma_table_blue[i] = (uint8_t)(pow(i/255.0,gamma_blue)*255.0+0.5);
  }
}

int main(int argc, char** argv) {
  u16 port = OPC_DEFAULT_PORT;
  u32 spi_speed_hz = SPI_DEFAULT_SPEED_HZ;
  char* spi_device_path = "fooserial";

  set_gamma(2.2, 2.2, 2.2);
  get_speed_and_port(&spi_speed_hz, &port, argc, argv);
  if (argc > 3) {
    spi_device_path = argv[3];
  }
  
  if(init_ftdi(spi_device_path, spi_speed_hz) > -1) {
    opc_serve_main(port, apa102_put_pixels, buffer);
  }

  close_ftdi();
  return 0;
}
