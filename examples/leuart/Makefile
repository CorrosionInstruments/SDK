# Copyright (c) 2019, Myriota Pty Ltd, All Rights Reserved
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


PROGRAM_NAME = leuart

ROOTDIR ?= $(abspath ../..)

OBJ_LIST = main.o

ifeq (sim, $(notdir $(PLATFORM)))
	OBJ_LIST+=sim.o
endif

include $(ROOTDIR)/terminal/app.mk
