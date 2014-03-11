/*
 * SimpleGPIO.cpp
 *
 * Modifications by Derek Molloy, School of Electronic Engineering, DCU
 * www.eeng.dcu.ie/~molloyd/
 * Almost entirely based on Software by RidgeRun:
 *
 * Copyright (c) 2011, RidgeRun
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *    This product includes software developed by the RidgeRun.
 * 4. Neither the name of the RidgeRun nor the
 *    names of its contributors may be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY RIDGERUN ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL RIDGERUN BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "SimpleGPIO.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>

/****************************************************************
 * gpio_export
 ****************************************************************/
int gpio_export(gpio_t* gpio, char ngpio)
{
	int fd, len;
	char buf[MAX_BUF];

	fd = open(SYSFS_GPIO_DIR "/export", O_WRONLY);
	if (fd < 0) {
		perror("gpio/export");
		return fd;
	}

	len = snprintf(buf, sizeof(buf), "%d", ngpio);
	write(fd, buf, len);
	close(fd);

        // init gpio_t struct
        memset (gpio, 0, sizeof(gpio_t));
        gpio->num = ngpio;
        // open value file
        snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", ngpio);
        gpio->fd_value = open(buf, O_RDWR);
//         gpio->fd_value = open(buf, O_RDONLY);
        if (gpio->fd_value < 0) {
                perror("gpio/open-value");
                return fd;
        }
        // open direction file
        snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/direction", ngpio);
        gpio->fd_direction = open(buf, O_WRONLY);
        if (gpio->fd_direction < 0) {
                perror("gpio/open-direction");
                return fd;
        }

        // open edge file(damien)
        snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/edge", ngpio);
        gpio->fd_edge = open(buf, O_WRONLY);
        if (gpio->fd_edge < 0) {
                perror("gpio/open-edge");
                return fd;
        }


	return 0;
}

/****************************************************************
 * gpio_unexport
 ****************************************************************/
int gpio_unexport(gpio_t* gpio)
{
	int fd, len;
	char buf[MAX_BUF];

        close (gpio->fd_value);
        close (gpio->fd_direction);
        close (gpio->fd_edge);//damien

	fd = open(SYSFS_GPIO_DIR "/unexport", O_WRONLY);
	if (fd < 0) {
		perror("gpio/export");
		return fd;
	}

	len = snprintf(buf, sizeof(buf), "%d", gpio->num);
	write(fd, buf, len);
	close(fd);
	return 0;
}

/****************************************************************
 * gpio_set_dir
 ****************************************************************/
int gpio_set_dir(gpio_t* gpio, PIN_DIRECTION out_flag)
{
	if (out_flag == OUTPUT_PIN)
		write(gpio->fd_direction, "out", 4);
	else
		write(gpio->fd_direction, "in", 3);

	return 0;
}

/****************************************************************
 * gpio_set_value
 ****************************************************************/
int gpio_set_value(gpio_t* gpio, PIN_VALUE value)
{
        lseek (gpio->fd_value, 0, SEEK_SET);
	if (value==LOW)
		write(gpio->fd_value, "0", 2);
	else
		write(gpio->fd_value, "1", 2);

	return 0;
}

/****************************************************************
 * gpio_get_value
 ****************************************************************/
int gpio_get_value(gpio_t* gpio, unsigned int *value)
{
	char ch;

        lseek (gpio->fd_value, 0, SEEK_SET);
	read(gpio->fd_value, &ch, 1);

	if (ch != '0') {
		*value = 1;
	} else {
		*value = 0;
	}
	return 0;
}


/****************************************************************
 * gpio_set_edge
 ****************************************************************/
/*
int gpio_set_edge(unsigned int gpio, char *edge)
{
	int fd;
	char buf[MAX_BUF];

	snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/edge", gpio);

	fd = open(buf, O_WRONLY);
	if (fd < 0) {
		perror("gpio/set-edge");
		return fd;
	}

	write(fd, edge, strlen(edge) + 1);
	close(fd);
	return 0;
}
*/

int gpio_set_edge(gpio_t* gpio, char *edge)//damien
{

   lseek (gpio->fd_edge, 0, SEEK_SET);
   write(gpio->fd_edge, edge, strlen(edge) + 1);

	return 0;
}


/****************************************************************
 * gpio_fd_open
 ****************************************************************/

int gpio_fd_open(unsigned int gpio)
{
	int fd;
	char buf[MAX_BUF];

	snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", gpio);

	fd = open(buf, O_RDONLY | O_NONBLOCK );
	if (fd < 0) {
		perror("gpio/fd_open");
	}
	return fd;
}

int gpio_fd_open_g(gpio_t* gpio)//damien
{
	int fd;
	char buf[MAX_BUF];
	unsigned int num_gpio;

	num_gpio = atoi(&gpio->num);

	snprintf(buf, sizeof(buf), SYSFS_GPIO_DIR "/gpio%d/value", num_gpio);

	fd = open(buf, O_RDONLY | O_NONBLOCK );
	if (fd < 0) {
		perror("gpio/fd_open");
	}
	return fd;
}

/****************************************************************
 * gpio_fd_close
 ****************************************************************/

int gpio_fd_close(int fd)
{
	return close(fd);
}


/****************************************************************
 * gpio_omap_mux_setup - Allow us to setup the omap mux mode for a pin
 ****************************************************************/
int gpio_omap_mux_setup(const char *omap_pin0_name, const char *mode)
{
	int fd;
	char buf[MAX_BUF];
	snprintf(buf, sizeof(buf), SYSFS_OMAP_MUX_DIR "%s", omap_pin0_name);
	fd = open(buf, O_WRONLY);
	if (fd < 0) {
		perror("failed to open OMAP_MUX");
		return fd;
	}
	write(fd, mode, strlen(mode) + 1);
	close(fd);
	return 0;
}
