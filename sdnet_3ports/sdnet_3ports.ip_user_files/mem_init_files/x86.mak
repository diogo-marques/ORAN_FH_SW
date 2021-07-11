# (c) Copyright 2018 Xilinx, Inc. All rights reserved.
#
# This file contains confidential and proprietary information
# of Xilinx, Inc. and is protected under U.S. and
# international copyright and other intellectual property
# laws.
#
# DISCLAIMER
# This disclaimer is not a license and does not grant any
# rights to the materials distributed herewith. Except as
# otherwise provided in a valid license issued to you by
# Xilinx, and to the maximum extent permitted by applicable
# law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND
# WITH ALL FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES
# AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY, INCLUDING
# BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-
# INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE; and
# (2) Xilinx shall not be liable (whether in contract or tort,
# including negligence, or under any other theory of
# liability) for any loss or damage of any kind or nature
# related to, arising under or in connection with these
# materials, including for any direct, or any indirect,
# special, incidental, or consequential loss or damage
# (including loss of data, profits, goodwill, or any type of
# loss or damage suffered as a result of any action brought
# by a third party) even if such damage or loss was
# reasonably foreseeable or Xilinx had been advised of the
# possibility of the same.
#
# CRITICAL APPLICATIONS
# Xilinx products are not designed or intended to be fail-
# safe, or for use in any application requiring fail-safe
# performance, such as life-support or safety devices or
# systems, Class III medical devices, nuclear facilities,
# applications related to the deployment of airbags, or any
# other applications that could lead to death, personal
# injury, or severe property or environmental damage
# (individually and collectively, "Critical
# Applications"). Customer assumes the sole risk and
# liability of any use of Xilinx products in Critical
# Applications, subject only to applicable laws and
# regulations governing limitations on product liability.
#
# THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS
# PART OF THIS FILE AT ALL TIMES. 
################################################################################
#
# Vendor         : Xilinx
# Revision       : $Revision: #2 $
# Date           : $DateTime: 2019/06/13 14:46:53 $
# Last Author    : $Author: fmartin $
#
################################################################################
# Description : Configuration settings to adapt the Xilinx SDNet control plane
# driver library build to run on an X86 platform
#
################################################################################

# Targets
LIBNAME=libsdnetdrv

# Directories
SRC_ROOT=.
BUILD_ROOT=./build
INSTALL_ROOT?=./install
LIB_INSTALL_DIR=$(INSTALL_ROOT)/lib
LIB_HEADER_INSTALL_DIR=$(INSTALL_ROOT)/include

# Commands
COMPILE=gcc
STATIC_LINK=ar
DYNAMIC_LINK=gcc
DEPLOY=

# File extensions
TEMP_DEP_FILE_EXT=Td
DEP_FILE_EXT=d
OBJ_FILE_EXT=o
STATIC_LIB_EXT=a
DYNAMIC_LIB_EXT=so

# Command options/flags
EARLY_COMPILE_FLAGS=-fPIC -Wall -Wextra -std=c99
LATE_COMPILE_FLAGS=-MT $$@ -MMD -MP -MF $($1_DEP_DIR)/$$*.$(TEMP_DEP_FILE_EXT) -c $$< -o $$@
EXTRA_COMPILE_FLAGS=
INC_SWITCH=-I
STATIC_LINK_FLAGS=rcs $@ $^
DYNAMIC_LINK_FLAGS=-shared -o $@ $^
DEPLOY_FLAGS=

# Build variants
VARIANT?=debug
ifeq ($(VARIANT),debug)
EARLY_COMPILE_FLAGS+=-g -O0
endif

ifeq ($(VARIANT),release)
EARLY_COMPILE_FLAGS+=-O3
endif

ifeq ($(VARIANT),coverage)
EARLY_COMPILE_FLAGS+=-g -O0 --coverage
DYNAMIC_LINK_FLAGS+=--coverage -lgcov
endif
