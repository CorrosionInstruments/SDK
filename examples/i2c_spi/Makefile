# Copyright (c) 2016-2019, Myriota Pty Ltd, All Rights Reserved
# SPDX-License-Identifier: BSD-3-Clause-Attribution
#
# This file is licensed under the BSD with attribution  (the "License"); you
# may not use these files except in compliance with the License.
#
# You may obtain a copy of the License here:
# LICENSE-BSD-3-Clause-Attribution.txt and at
# https://spdx.org/licenses/BSD-3-Clause-Attribution.html
#
# See the License for the specific language governing permissions and
# limitations under the License.


USING_I2C ?= 1 # Change to 0 to use SPI

ifeq ($(USING_I2C), 1)
PROGRAM_NAME = LIS3DH_i2c
else
PROGRAM_NAME = LIS3DH_spi
endif

ROOTDIR ?= $(abspath ../..)

OBJ_LIST = main.o

ifeq (sim, $(notdir $(PLATFORM)))
	OBJ_LIST+=sim.o
endif

include $(ROOTDIR)/terminal/app.mk
ifeq ($(USING_I2C), 1)
CFLAGS+=-DUSING_I2C
endif
