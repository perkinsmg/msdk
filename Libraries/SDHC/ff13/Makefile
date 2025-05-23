###############################################################################
 #
 # Copyright (C) 2022-2023 Maxim Integrated Products, Inc. (now owned by
 # Analog Devices, Inc.),
 # Copyright (C) 2023-2024 Analog Devices, Inc.
 #
 # Licensed under the Apache License, Version 2.0 (the "License");
 # you may not use this file except in compliance with the License.
 # You may obtain a copy of the License at
 #
 #     http://www.apache.org/licenses/LICENSE-2.0
 #
 # Unless required by applicable law or agreed to in writing, software
 # distributed under the License is distributed on an "AS IS" BASIS,
 # WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 # See the License for the specific language governing permissions and
 # limitations under the License.
 #
 ##############################################################################

# This is the name of the build output file
override PROJECT=FAT32

ifeq "$(TARGET)" ""
$(error TARGET must be specified)
endif
TARGET_UC := $(subst m,M,$(subst a,A,$(subst x,X,$(TARGET))))
TARGET_LC := $(subst M,m,$(subst A,a,$(subst X,x,$(TARGET))))

ifeq "$(COMPILER)" ""
$(error COMPILER must be specified)
endif

ifeq "$(BUILD_DIR)" ""
BUILD_DIR=./Build
endif

# This is the path to the peripheral drivers
ifeq "$(PERIPH_DRIVER_DIR)" ""
$(error PeriphDriver must be specified)
endif

# This is the path to the CMSIS root directory
ifeq "$(CMSIS_ROOT)" ""
$(error CMSIS_ROOT must be specified)
endif

ifeq "$(BOARD_DIR)" ""
$(error BOARD_DIR must be specified)
endif

ifeq "$(SDHC_DRIVER_DIR)" ""
SDHC_DRIVER_DIR=../
endif

# Source files for this test (add path to VPATH below)
SRCS += diskio.c
SRCS += ff.c
SRCS += ffsystem.c
SRCS += ffunicode.c
ifdef EXTERNAL_FLASH
SRCS += mscmem.c
SRCS += mx25_64MB.c
endif

ifdef EXTERNAL_FLASH
PROJ_CFLAGS += -DEXTERNAL_FLASH
endif

ifdef USE_NATIVE_SDHC
PROJ_CFLAGS += -DNATIVE_SDHC
endif

# Where to find source files for this project
VPATH  = Source
VPATH += $(MISC_DRIVERS_DIR)/ExtMemory

# Where to find header files for this project
IPATH  = Source
IPATH += $(SDHC_DRIVER_DIR)/Include
IPATH += $(CMSIS_ROOT)/Device/Maxim/$(TARGET)/Include
IPATH += $(CMSIS_ROOT)/Include
IPATH += $(PERIPH_DRIVER_DIR)/Include/$(TARGET_UC)
IPATH += $(BOARD_DIR)/Include
IPATH += $(MISC_DRIVERS_DIR)/ExtMemory

# Since the diskio.c implementation file has some dependencies
# on board files, we must set up the board build system as well
include $(BOARD_DIR)/board.mk

# Include the rules for building for this target
include $(CMSIS_ROOT)/Device/Maxim/$(TARGET_UC)/Source/$(COMPILER)/$(TARGET_LC).mk

# Build this as a library
.DEFAULT_GOAL ?= lib
