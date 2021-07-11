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
# Revision       : $Revision: #1 $
# Date           : $DateTime: 2019/06/10 09:31:31 $
# Last Author    : $Author: fmartin $
#
################################################################################
# Description : Top level makefile for the Xilinx SDNet control plane driver
# library
#
################################################################################

######################################################################################################################################################
# PLATFORM CONFIGURATION
######################################################################################################################################################

# Specify default platform (can be overridden at the command line or using environment variable)
PLATFORM?=x86.mak

# Include the build configuration for the specified platform
include $(PLATFORM)

######################################################################################################################################################
# MASTER VARIABLES
######################################################################################################################################################

BUILD_ENTITY_LIST =
LIB_OBJS =
LIB_DEPS =

# Derive target name from LIBNAME specified in the platform file
STATIC_LIB=$(BUILD_ROOT)/$(LIBNAME).$(STATIC_LIB_EXT)
DYNAMIC_LIB=$(BUILD_ROOT)/$(LIBNAME).$(DYNAMIC_LIB_EXT)

# Definitions for pretty-printing output

# Colour codes
CMD_COLOR   = \033[0;34m
OBJ_COLOR   = \033[0;36m
OK_COLOR    = \033[0;32m
ERROR_COLOR = \033[0;31m
WARN_COLOR  = \033[0;33m
NO_COLOR    = \033[m

# Command strings
COM_STRING 		= "Compile"
LINK_STRING 	= "Link"
CLEAN_STRING 	= "Clean"
INSTALL_STRING	= "Install"

# Result strings
OK_STRING    = [ OK    ]
ERROR_STRING = [ ERROR ]
WARN_STRING  = [ WARN  ]

# Control output verbosity
VERBOSE ?= 0
ifeq ($(VERBOSE),0)
AT = @
else
AT =
endif

######################################################################################################################################################
# MACRO DEFINITIONS
######################################################################################################################################################

# The following macro generates a collection of entity-specific variables which describe each entity being built by this makefile
define gen-vars

# The following definitions are used to generate variables pointing to the src and inc directory for each entity.
# The contents of the src directory are used to generate the list of object files and dependency files which the build must produce
# The path to the inc directory is generated so that other entities depending on a given entity can receive it as a parameter
# The obj directory holds object files produced by compilation
# The dep directory holds dependency files produced by compilation
$1_SRC_DIR := $(SRC_ROOT)/$($1)/src
$1_INC_DIR := $(SRC_ROOT)/$($1)/inc
$1_OBJ_DIR := $(BUILD_ROOT)/$($1)/obj
$1_DEP_DIR := $(BUILD_ROOT)/$($1)/dep

$1_INC_LIST := $(wildcard $(SRC_ROOT)/$($1)/inc/*.h)
$1_SRC_LIST := $(wildcard $(SRC_ROOT)/$($1)/src/*.c)
$1_OBJ_LIST := $(patsubst $(SRC_ROOT)/$($1)/src/%.c,$(BUILD_ROOT)/$($1)/obj/%.$(OBJ_FILE_EXT),$(wildcard $(SRC_ROOT)/$($1)/src/*.c))
$1_DEP_LIST := $(patsubst $(SRC_ROOT)/$($1)/src/%.c,$(BUILD_ROOT)/$($1)/dep/%.$(DEP_FILE_EXT),$(wildcard $(SRC_ROOT)/$($1)/src/*.c))

# Generate a per-entity variable to hold the dependencies (DEPS)
# Populated manually by developers with name of entities which the current entity is dependent upon
# Each entity is dependent upon itself by default
$1_DEPS += $1

# Generate a per-entity variable to hold the compiler flags - leave initialised to empty
# Updated manually  by developers with any entity-specific flags
$1_COMPILE_FLAGS +=

# Generate a per-entity variable to hold the include paths - leave initialised to empty
# Generated based on the contents of the corresponding entity-specific "DEPS" variable
$1_INCLUDES +=
endef

# The following macro generates makefile targets to compile the code for each entity
# It relies on entity-specific definitions to generate makefile rules to compile source files into object files
# The compiler generates dependency files during compilation - these dependency files cause recompilation of sources if header files are modified
define gen-targets
# Create directories for object files and dependency files so that they can be cleaned up easily manually if necessary
$(shell mkdir -p $($1_OBJ_DIR) > /dev/null)
$(shell mkdir -p $($1_DEP_DIR) > /dev/null)

# Compile the sources into object files and emit "temporary" dependency files during the process
# Rename the "temporary" dependency files to their actual filename
# Temporary files are used so that the actual dependency file is only updated if the compilation step succeeds
# If compilation fails, then the actual dependency file is unchanged
$($1_OBJ_DIR)/%.$(OBJ_FILE_EXT): $($1_SRC_DIR)/%.c $($1_DEP_DIR)/%.$(DEP_FILE_EXT)
	$(AT)$(COMPILE) $($1_INCLUDES) $(EARLY_COMPILE_FLAGS) $(EXTRA_COMPILE_FLAGS) $($1_COMPILE_FLAGS) $(LATE_COMPILE_FLAGS)
	@$(COMPILE) $($1_INCLUDES) $(EARLY_COMPILE_FLAGS) $(EXTRA_COMPILE_FLAGS) $($1_COMPILE_FLAGS) $(LATE_COMPILE_FLAGS) 2> $$@.log; \
		RESULT=$$$$?; \
	    if [ $$$$RESULT -ne 0 ]; then \
	        printf "%-60b%b" "$(CMD_COLOR)$(COM_STRING) $(ERROR_COLOR)$(ERROR_STRING) $(OBJ_COLOR)$$@$(NO_COLOR)\n"; \
	    elif [ -s $$@.log ]; then \
	        printf "%-60b%b" "$(CMD_COLOR)$(COM_STRING) $(WARN_COLOR)$(WARN_STRING) $(OBJ_COLOR)$$@$(NO_COLOR)\n"; \
	    else  \
	        printf "%b%b" "$(CMD_COLOR)$(COM_STRING) $(OK_COLOR)$(OK_STRING) $(OBJ_COLOR)$$@$(NO_COLOR)\n"; \
	    fi; \
	    rm -f $$@.log; \
	    exit $$$$RESULT;
	$(AT)-mv -f $($1_DEP_DIR)/$$*.$(TEMP_DEP_FILE_EXT) $($1_DEP_DIR)/$$*.$(DEP_FILE_EXT)

# In the main compilation rule above, dependency files are prerequisites.  However, these files don't exist until the file is compiled at least once
# Since the files don't exist, make expects a rule to create them but complains that no such rule exists either and make will error out
# The following blank rule is provided to resolve that issue, which also handles the case where dependency files are deleted by the user
$($1_DEP_DIR)/%.$(DEP_FILE_EXT): ;

# Add some user friendly targets for each entity to allow them to be built and cleaned independently - useful for fast rebuild during development
build_$1: $($1_OBJ_LIST)

clean_$1:
	@printf "%b" "$(CMD_COLOR)$(CLEAN_STRING) $(OBJ_COLOR)$($1_OBJ_DIR)$(NO_COLOR)\n"
	$(AT)-rm -Rf $($1_OBJ_DIR)
	@printf "%b" "$(CMD_COLOR)$(CLEAN_STRING) $(OBJ_COLOR)$($1_DEP_DIR)$(NO_COLOR)\n"
	$(AT)-rm -Rf $($1_DEP_DIR)
endef

# The following macro is used to establish some useful lists:
# 	The list of all object files is created to be passed to the archiver
# 	The list of all dependency files is created so that they can be included in this makefile
# 	The list of all directories containing object files so that those directories can be removed during cleaning
# 	The list of all directories containing dependency files so that those directories can be removed during cleaning
define gen-master-lists
HDR_LIST += $($1_INC_LIST)
LIB_OBJS += $($1_OBJ_LIST)
LIB_DEPS += $($1_DEP_LIST)
OBJ_DIRS += $($1_OBJ_DIR)
DEP_DIRS += $($1_DEP_DIR)
endef

# The following macro is used to populate the entity-specific INCLUDES variable based on the manually configured entity-specific DEPS variable
define gen-entity-includes
$(foreach entry,$($1_DEPS),$(eval $1_INCLUDES += $(INC_SWITCH)$($(entry)_INC_DIR)))
endef

######################################################################################################################################################
# INCLUDE BUILD CONFIGURATION
######################################################################################################################################################

include xilinx_sdnet_drivers.mak

######################################################################################################################################################
# MAKEFILE BODY
######################################################################################################################################################

# Ensure that the default target is "all"
all: build install

# Create various directories targeted by the build if they don't exist already
$(shell mkdir -p $(BUILD_ROOT) > /dev/null)
$(shell mkdir -p $(INSTALL_ROOT) > /dev/null)
$(shell mkdir -p $(LIB_INSTALL_DIR) > /dev/null)
$(shell mkdir -p $(LIB_HEADER_INSTALL_DIR) > /dev/null)

# Generate entity-specific variables
$(foreach entity,$(BUILD_ENTITY_LIST),$(eval $(call gen-vars,$(entity))))

# Generate the INCLUDES variable for each entity based on the DEPS values from xilinx_sdnet_drivers.mak
$(foreach entity,$(BUILD_ENTITY_LIST),$(eval $(call gen-entity-includes,$(entity))))

# Generate targets for each entity
$(foreach entity,$(BUILD_ENTITY_LIST),$(eval $(call gen-targets,$(entity))))

# Generate all master list variables used to support the build and clean operations
$(foreach entity,$(BUILD_ENTITY_LIST),$(eval $(call gen-master-lists,$(entity))))

# Mark dependency files as precious - prevent make from deleting them
.PRECIOUS: $(LIB_DEPS)

# Include all generated dependency files (the '-' symbol prevents failure if the files don't exist i.e. during initial compilation)
-include $(LIB_DEPS)

# Makefile targets to generate the static and dynamic libraries from the above defined object generation
$(STATIC_LIB): $(LIB_OBJS)
	@printf "%b" "$(CMD_COLOR)$(LINK_STRING) $(OBJ_COLOR)$(STATIC_LIB)$(NO_COLOR)\n"
	$(AT)$(STATIC_LINK) $(STATIC_LINK_FLAGS)

$(DYNAMIC_LIB): $(LIB_OBJS)
	@printf "%b" "$(CMD_COLOR)$(LINK_STRING) $(OBJ_COLOR)$(DYNAMIC_LIB)$(NO_COLOR)\n"
	$(AT)$(DYNAMIC_LINK) $(DYNAMIC_LINK_FLAGS)

# User friendly targets for building the libraries
build_static_lib: $(STATIC_LIB)
build_dynamic_lib: $(DYNAMIC_LIB)

# User friendly targets for cleaning the libraries
clean_static_lib:
	@printf "%b" "$(CMD_COLOR)$(CLEAN_STRING) $(OBJ_COLOR)$(STATIC_LIB)$(NO_COLOR)\n"
	$(AT)-rm -f $(STATIC_LIB)
clean_dynamic_lib:
	@printf "%b" "$(CMD_COLOR)$(CLEAN_STRING) $(OBJ_COLOR)$(DYNAMIC_LIB)$(NO_COLOR)\n"
	$(AT)-rm -f $(DYNAMIC_LIB)

# User friendly targets for cleaning the build artefacts
clean_obj_dirs:
	@printf "%b" "$(CMD_COLOR)$(CLEAN_STRING) $(OBJ_COLOR)object directories$(NO_COLOR)\n"
	$(AT)-rm -Rf $(OBJ_DIRS)
clean_dep_dirs:
	@printf "%b" "$(CMD_COLOR)$(CLEAN_STRING) $(OBJ_COLOR)dependency directories$(NO_COLOR)\n"
	$(AT)-rm -Rf $(DEP_DIRS)

# User friendly targets to install libraries
install_static_lib: build_static_lib
	@printf "%b" "$(CMD_COLOR)$(INSTALL_STRING) $(OBJ_COLOR)$(STATIC_LIB)$(NO_COLOR)\n"
	$(AT)rsync -a $(STATIC_LIB) $(LIB_INSTALL_DIR)

install_dynamic_lib: build_dynamic_lib
	@printf "%b" "$(CMD_COLOR)$(INSTALL_STRING) $(OBJ_COLOR)$(DYNAMIC_LIB)$(NO_COLOR)\n"
	$(AT)rsync -a $(DYNAMIC_LIB) $(LIB_INSTALL_DIR)

install_lib_headers: $(HDR_LIST)
	@printf "%b" "$(CMD_COLOR)$(INSTALL_STRING) $(OBJ_COLOR)header files$(NO_COLOR)\n"
	$(AT)rsync -a $(HDR_LIST) $(LIB_HEADER_INSTALL_DIR)

deploy: $(DYNAMIC_LIB)
	$(DEPLOY) $(DEPLOY_FLAGS)
	
# Primary makefile targets
build: build_static_lib build_dynamic_lib install_static_lib install_dynamic_lib install_lib_headers
clean: clean_static_lib clean_dynamic_lib clean_dep_dirs clean_obj_dirs
install: deploy
.PHONY: build install

list_entities:
	@echo "Supported entities are:"
	@for i in $(BUILD_ENTITY_LIST) ; do \
		echo "	$$i"; \
	done

help:
	@echo ""
	@echo "Quick reference for various supported build targets for sdnetdrv"
	@echo "----------------------------------------------------"
	@echo "  clean                    clean out all build objects"
	@echo "  all                      build and install sdnetdrv"
	@echo "  build                    build subsystem"
	@echo "  install                  install built objects"
	@echo "Utility targets to facilitate development of sdnetdrv"
	@echo "----------------------------------------------------"
	@echo "  list_entities            print a list of all entities recognised by the build"
	@echo "  build_<ENTITY>           create build objects for specified <ENTITY>"
	@echo "                           e.g. build_COMMON"
	@echo "  clean_<ENTITY>           clean out build objects for specified <ENTITY>"
	@echo "                           e.g. clean_COMMON"
	@echo "  build_static_library     create a static library"
	@echo "  build_dynamic_library    create a dynamic library"
	@echo "  install_static_library   install the static library"
	@echo "  install_dynamic_library  install the dynamic library"
	@echo "  install_lib_headers      install the library header files"
	@echo "  clean_static_library     clean the static library"
	@echo "  clean_dynamic_library    clean the dynamic library"
	@echo "  clean_obj_dirs           clean out all generated object directories"
	@echo "  clean_dep_dirs           clean out all generated dependency directories"
