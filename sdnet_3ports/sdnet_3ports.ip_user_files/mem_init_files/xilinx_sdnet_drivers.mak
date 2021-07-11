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
# Revision       : $Revision: #4 $
# Date           : $DateTime: 2021/01/07 04:30:57 $
# Last Author    : $Author: fmartin $
#
################################################################################
# Description : This file contains definitions which describe how build Xilinx
# SDNet control plane drivers.  This is accomplished by defining makefile
# variables to reference individual elements within the code-base and variables
# which define relationships (DEPS) between a given build element and other
# elements.  Compiler options specific to a given build element may also be
# specified here. A master list of the defined build elements is generated,
# which is then operated on by a set of macros in the top level makefile, which
# generate build targets for compilation of each build entity.
#
################################################################################



######################################################################################################################################################
# BUILD ENTITY DEFINITION: COMMON
######################################################################################################################################################

COMMON = common
BUILD_ENTITY_LIST += COMMON
COMMON_COMPILE_FLAGS += -pedantic

######################################################################################################################################################
# BUILD ENTITY DEFINITION: TABLE
######################################################################################################################################################

TARGET_MGMT = target_mgmt
BUILD_ENTITY_LIST += TARGET_MGMT
TARGET_MGMT_COMPILE_FLAGS += -pedantic
TARGET_MGMT_DEPS += COMMON

######################################################################################################################################################
# BUILD ENTITY DEFINITION: CAM_OBF
######################################################################################################################################################

CAM_OBF = cam_obf
BUILD_ENTITY_LIST += CAM_OBF
CAM_OBF_COMPILE_FLAGS +=-std=gnu99 -Wno-unused-parameter -Wno-sign-compare -Wno-missing-braces -fgnu89-inline

######################################################################################################################################################
# BUILD ENTITY DEFINITION: CAM_TOP
######################################################################################################################################################

CAM_TOP = cam_top
BUILD_ENTITY_LIST += CAM_TOP
CAM_TOP_COMPILE_FLAGS += -pedantic
CAM_TOP_DEPS += COMMON CAM_OBF

######################################################################################################################################################
# BUILD ENTITY DEFINITION: TABLE
######################################################################################################################################################

TABLE = table
BUILD_ENTITY_LIST += TABLE
TABLE_COMPILE_FLAGS += -pedantic
TABLE_DEPS += COMMON CAM_TOP

######################################################################################################################################################
# BUILD ENTITY DEFINITION: TARGET
######################################################################################################################################################

TARGET = target
BUILD_ENTITY_LIST += TARGET
TARGET_COMPILE_FLAGS += -pedantic
TARGET_DEPS += COMMON \
               TARGET_MGMT \
               CAM_TOP \
               TABLE
