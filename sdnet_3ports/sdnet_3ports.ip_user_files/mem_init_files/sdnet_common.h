/*
-- (c) Copyright 2018 Xilinx, Inc. All rights reserved.
--
-- This file contains confidential and proprietary information
-- of Xilinx, Inc. and is protected under U.S. and
-- international copyright and other intellectual property
-- laws.
--
-- DISCLAIMER
-- This disclaimer is not a license and does not grant any
-- rights to the materials distributed herewith. Except as
-- otherwise provided in a valid license issued to you by
-- Xilinx, and to the maximum extent permitted by applicable
-- law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND
-- WITH ALL FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES
-- AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY, INCLUDING
-- BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-
-- INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE; and
-- (2) Xilinx shall not be liable (whether in contract or tort,
-- including negligence, or under any other theory of
-- liability) for any loss or damage of any kind or nature
-- related to, arising under or in connection with these
-- materials, including for any direct, or any indirect,
-- special, incidental, or consequential loss or damage
-- (including loss of data, profits, goodwill, or any type of
-- loss or damage suffered as a result of any action brought
-- by a third party) even if such damage or loss was
-- reasonably foreseeable or Xilinx had been advised of the
-- possibility of the same.
--
-- CRITICAL APPLICATIONS
-- Xilinx products are not designed or intended to be fail-
-- safe, or for use in any application requiring fail-safe
-- performance, such as life-support or safety devices or
-- systems, Class III medical devices, nuclear facilities,
-- applications related to the deployment of airbags, or any
-- other applications that could lead to death, personal
-- injury, or severe property or environmental damage
-- (individually and collectively, "Critical
-- Applications"). Customer assumes the sole risk and
-- liability of any use of Xilinx products in Critical
-- Applications, subject only to applicable laws and
-- regulations governing limitations on product liability.
--
-- THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS
-- PART OF THIS FILE AT ALL TIMES.
--------------------------------------------------------------------------------
--
-- Vendor         : Xilinx
-- Revision       : $Revision: #8 $
-- Date           : $DateTime: 2021/01/27 13:43:20 $
-- Last Author    : $Author: fmartin $
--
--------------------------------------------------------------------------------
-- Description : Header file with definitions for device access interface used
-- by Xilinx SDNet extern SW drivers.
--
--------------------------------------------------------------------------------
*/

/*! \mainpage SDNet control plane drivers
 *
 * This collection of Doxygen generated HTML describes the API provided by the
 * low level control plane drivers included with SDNet.  The drivers enable the
 * development of application software for management of P4 designs implemented
 * on Xilinx devices using SDNet.
 *
 * The drivers provided are developed and tested on an x86 Linux platform using
 * the GCC toolchain.  Makefiles are provided to compile the code for this
 * platform and these may be adapted to target the software to alternative
 * platforms.
 *
 * All of the drivers provided are built upon a set of shared definitions
 * contained in \ref sdnet_common.h and it is recommended that users familiarize
 * themselves with the content of this file.  In particular it should be noted
 * that the drivers are designed such that the user application is responsible
 * for binding in callbacks for platform-specific functionality such as hardware
 * read/write access and logging.  This is accomplished via the
 * \ref XilSdnetEnvIf structure and associated definitions - the drivers require
 * a pointer to an instance of this structure which the application has
 * populated with suitable functions.  The drivers hold an internal copy of the
 * structure provided during the initialization.
 *
 * Please refer to the modules page for an overview of the individual drivers
 * present.
 *
 */

/**
 * Common defintions used to support the development of SDNet control plane drivers
 *
 * @file sdnet_common.h
 *
 * @addtogroup common SDNet common definitions
 * @{
 */

/****************************************************************************************************************************************************/
/* SECTION: Header guards */
/****************************************************************************************************************************************************/

#ifndef SDNET_COMMON_H
#define SDNET_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************************************************************************************/
/* SECTION: Header includes */
/****************************************************************************************************************************************************/

#include <stdint.h>
#include <stdlib.h>

/****************************************************************************************************************************************************/
/* SECTION: Constants/Macros */
/****************************************************************************************************************************************************/

/* Common definitions to avoid magic numbers / save redefining them in every single driver */
#define XIL_SDNET_BITS_PER_BYTE         (8)
#define XIL_SDNET_BITS_PER_REG          (32)
#define XIL_SDNET_BYTES_PER_REG         (4)
#define XIL_SDNET_LOG_BUF_SIZE_BYTES    (256)
#define XIL_SDNET_BYTE_MASK             (0xff)

/* Macro to gracefully remove warnings about unused parameters */
#define XIL_SDNET_UNUSED_ARG(x) (void)(x)

/****************************************************************************************************************************************************/
/* SECTION: Type definitions */
/****************************************************************************************************************************************************/

/* Generic declaration for SDNet attributes: an attribute is any named numeric value */
typedef struct XilSdnetAttribute
{
    const char *NameStringPtr;
    uint32_t Value;
} XilSdnetAttribute;

/**
 * Error code definitions for Xilinx SDNet library
 */

/* If making a change here, ensure to change the  table of strings (XilSdnetReturnTypeStrings) found in "sdnet_common.c" */
typedef enum XilSdnetReturnType
{
    XIL_SDNET_SUCCESS,                                       /**< General/common return code: Success*/
    XIL_SDNET_GENERAL_ERR_NULL_PARAM,                        /**< General/common return code: Error null parameter*/
    XIL_SDNET_GENERAL_ERR_INVALID_CONTEXT,                   /**< General/common return code: Error invalid context*/
    XIL_SDNET_GENERAL_ERR_INVALID_ENVIRONMENT_INTERFACE,     /**< General/common return code: Error invalid environment interface*/
    XIL_SDNET_GENERAL_ERR_INTERNAL_ASSERTION,                /**< General/common return code: Error internal assertion*/

    XIL_SDNET_BUILD_INFO_ERR_MALLOC_FAILED,                  /**< Build Info driver return code: Error malloc failed*/


    XIL_SDNET_CONTROL_ERR_INVALID_ENGINE_ID_ERROR_TYPE,           /**< Control register group return code: Invalid Engine Id error type*/
    XIL_SDNET_CONTROL_ERR_INVALID_PR2CLK_RATIO_ERROR_TYPE,        /**< Control register group return code: Invalid Packet rate to clock ratio error type*/
    XIL_SDNET_CONTROL_ERR_INVALID_PRM_VALUE_ERROR_TYPE,           /**< Control register group return code: Invalid Packet rate Margin value error type*/

    XIL_SDNET_INTERRUPT_ERR_MALLOC_FAILED,                        /**< Interrupt driver return code: Error malloc failed*/
    XIL_SDNET_INTERRUPT_ERR_CONFIG_MAX_NUM_ELEMENTS_EXCEED,       /**< Interrupt driver return code: Error number of elements in configuration exceeds maximum supported*/
    XIL_SDNET_INTERRUPT_ERR_CONFIG_MAX_NUM_COMPONENTS_EXCEED,          /**< Interrupt driver return code: Error number of Component exceeds in configuration maximum supported*/
    XIL_SDNET_INTERRUPT_ERR_CONFIG_INVALID_COMPONENT_NAME,             /**< Interrupt driver return code: Error invalid Component name used in configuration*/
    XIL_SDNET_INTERRUPT_ERR_COMPONENT_NOT_FOUND,                       /**< Interrupt driver return code: Error Component not found*/
    XIL_SDNET_INTERRUPT_ERR_NAME_BUFFER_TOO_SMALL,                /**< Interrupt driver return code: Error buffer for name string is to small to hold the full name*/
    XIL_SDNET_INTERRUPT_ERR_INVALID_ECC_ERROR_TYPE,               /**< Interrupt driver return code: Invalid ECC error type*/
    XIL_SDNET_INTERRUPT_ERR_INVALID_ELEMENT_ID,                   /**< Interrupt driver return code: Invalid Element Id*/
    XIL_SDNET_INTERRUPT_ERR_INVALID_COMPONENT_INDEX,                   /**< Interrupt driver return code: Invalid Fifo index*/

    XIL_SDNET_RANDOM_ERR_INVALID_MODE,                       /**< Random Extern driver return code: Error invalid mode*/
    XIL_SDNET_RANDOM_ERR_ARRAY_INVALID_SIZE,                 /**< Random Extern driver return code: Error array invalid size*/
    XIL_SDNET_RANDOM_ERR_INVALID_SEED,                       /**< Random Extern driver return code: Error invalid seed*/

    XIL_SDNET_REGISTER_ERR_INVALID_DATA_SIZE,                /**< Register Extern driver return code: Error invalid data size*/
    XIL_SDNET_REGISTER_ERR_INVALID_NUM_REGS,                 /**< Register Extern driver return code: Error invalid number of registers*/
    XIL_SDNET_REGISTER_ERR_INVALID_NUM_BITS,                 /**< Register Extern driver return code: Error invalid number of bits*/
    XIL_SDNET_REGISTER_ERR_BUFFER_SIZE_MISMATCH,             /**< Register Extern driver return code: Error buffer size mismatch*/
    XIL_SDNET_REGISTER_ERR_INVALID_INDEX,                    /**< Register Extern driver return code: Error invalid index*/

    XIL_SDNET_COUNTER_ERR_CONFIG_INVALID_MODE,               /**< Counter Extern driver return code: Error invalid mode*/
    XIL_SDNET_COUNTER_ERR_CONFIG_UNSUPPORTED_OPERATION,      /**< Counter Extern driver return code: Error unsupported operation*/
    XIL_SDNET_COUNTER_ERR_CONFIG_INVALID_NUM_COUNTERS,       /**< Counter Extern driver return code: Error invalid number of counters*/
    XIL_SDNET_COUNTER_ERR_INVALID_INDEX,                     /**< Counter Extern driver return code: Error invalid index*/
    XIL_SDNET_COUNTER_ERR_INVALID_PACKET_COUNT,              /**< Counter Extern driver return code: Error invalid packet count*/
    XIL_SDNET_COUNTER_ERR_INVALID_BYTE_COUNT,                /**< Counter Extern driver return code: Error invalid byte count*/

    XIL_SDNET_METER_ERR_CONFIG_INVALID_MODE,                 /**< Meter Extern driver return code: Error invalid mode*/
    XIL_SDNET_METER_ERR_CONFIG_UNSUPPORTED_OPERATION,        /**< Meter Extern driver return code: Error unsupported operation*/
    XIL_SDNET_METER_ERR_CONFIG_INVALID_NUM_METERS,           /**< Meter Extern driver return code: Error invalid number of meters*/
    XIL_SDNET_METER_ERR_CONFIG_INVALID_CLOCK_FREQ,           /**< Meter Extern driver return code: Error invalid clock frequency*/
    XIL_SDNET_METER_ERR_INVALID_INDEX,                       /**< Meter Extern driver return code: Error invalid index*/
    XIL_SDNET_METER_ERR_INVALID_PROFILE_COLOUR_AWARE,        /**< Meter Extern driver return code: Error invalid profile colour aware setting*/
    XIL_SDNET_METER_ERR_INVALID_PROFILE_DATA_RATE,           /**< Meter Extern driver return code: Error invalid profile data rate*/
    XIL_SDNET_METER_ERR_INVALID_PROFILE_BURST_SIZE,          /**< Meter Extern driver return code: Error invalid profile burst size*/
    XIL_SDNET_METER_ERR_INVALID_NUM_BITS,                    /**< Meter Extern driver return code: Error invalid number of bits */

    XIL_SDNET_TINY_CAM_ERR_INVALID_MODE,                     /**< Tiny TCAM/BCAM return code: Error invalid emulation mode*/

    XIL_SDNET_CAM_ERR_INVALID_NUM_ENTRIES,                   /**< CAM return code: Error invalid number of entries*/
    XIL_SDNET_CAM_ERR_LOOKUP_NOT_FOUND,                      /**< CAM return code: Error lookup not found*/
    XIL_SDNET_CAM_ERR_INVALID_KEY,                           /**< CAM return code: Error invalid key*/
    XIL_SDNET_CAM_ERR_INVALID_ENDIAN,                        /**< CAM return code: Error invalid endianness*/
    XIL_SDNET_CAM_ERR_FULL,                                  /**< CAM return code: Error full*/
    XIL_SDNET_CAM_ERR_NO_OPEN,                               /**< CAM return code: Error no open*/
    XIL_SDNET_CAM_ERR_INVALID_ARG,                           /**< CAM return code: Error invalid argument*/
    XIL_SDNET_CAM_ERR_WRONG_KEY_WIDTH,                       /**< CAM return code: Error wrong key width*/
    XIL_SDNET_CAM_ERR_TOO_MANY_INSTANCES,                    /**< CAM return code: Error too many instances*/
    XIL_SDNET_CAM_ERR_WRONG_BIT_FIELD_MASK,                  /**< CAM return code: Error wrong bit field mask*/
    XIL_SDNET_CAM_ERR_WRONG_CONST_FIELD_MASK,                /**< CAM return code: Error wrong constant field mask*/
    XIL_SDNET_CAM_ERR_WRONG_UNUSED_FIELD_MASK,               /**< CAM return code: Error wrong unused field mask*/
    XIL_SDNET_CAM_ERR_INVALID_TERNARY_FIELD_LEN,             /**< CAM return code: Error invalid ternary field length*/
    XIL_SDNET_CAM_ERR_WRONG_PRIO_WIDTH,                      /**< CAM return code: Error wrong priority width*/
    XIL_SDNET_CAM_ERR_WRONG_MAX,                             /**< CAM return code: Error wrong max*/
    XIL_SDNET_CAM_ERR_DUPLICATE_FOUND,                       /**< CAM return code: Error duplicate found*/
    XIL_SDNET_CAM_ERR_WRONG_PREFIX,                          /**< CAM return code: Error wrong prefix*/
    XIL_SDNET_CAM_ERR_WRONG_PREFIX_MASK,                     /**< CAM return code: Error wrong prefix mask*/
    XIL_SDNET_CAM_ERR_WRONG_RANGE,                           /**< CAM return code: Error wrong range*/
    XIL_SDNET_CAM_ERR_WRONG_RANGE_MASK,                      /**< CAM return code: Error wrong range mask*/
    XIL_SDNET_CAM_ERR_KEY_NOT_FOUND,                         /**< CAM return code: Error key not found*/
    XIL_SDNET_CAM_ERR_WRONG_MIN,                             /**< CAM return code: Error wrong minimum*/
    XIL_SDNET_CAM_ERR_WRONG_PRIO,                            /**< CAM return code: Error wrong priority*/
    XIL_SDNET_CAM_ERR_WRONG_LIST_LENGTH,                     /**< CAM return code: Error wrong list length*/
    XIL_SDNET_CAM_ERR_WRONG_NUMBER_OF_SLOTS,                 /**< CAM return code: Error wrong number of slots*/
    XIL_SDNET_CAM_ERR_INVALID_MEM_TYPE,                      /**< CAM return code: Error invalid memory type*/
    XIL_SDNET_CAM_ERR_TOO_HIGH_FREQUENCY,                    /**< CAM return code: Error frequency is too high*/
    XIL_SDNET_CAM_ERR_WRONG_TERNARY_MASK,                    /**< CAM return code: Error wrong ternary mask*/
    XIL_SDNET_CAM_ERR_MASKED_KEY_BIT_IS_SET,                 /**< CAM return code: Error masked key bit is set*/
    XIL_SDNET_CAM_ERR_INVALID_MODE,                          /**< CAM return code: Error invalid mode*/
    XIL_SDNET_CAM_ERR_WRONG_RESPONSE_WIDTH,                  /**< CAM return code: Error wrong response width*/
    XIL_SDNET_CAM_ERR_FORMAT_SYNTAX,                         /**< CAM return code: Error format syntax*/
    XIL_SDNET_CAM_ERR_TOO_MANY_FIELDS,                       /**< CAM return code: Error too many fields*/
    XIL_SDNET_CAM_ERR_TOO_MANY_RANGES,                       /**< CAM return code: Error too many ranges*/
    XIL_SDNET_CAM_ERR_INVALID_RANGE_LEN,                     /**< CAM return code: Error invalid range length*/
    XIL_SDNET_CAM_ERR_INVALID_RANGE_START,                   /**< CAM return code: Error invalid range start*/
    XIL_SDNET_CAM_ERR_INVALID_PREFIX_LEN,                    /**< CAM return code: Error invalid prefix length*/
    XIL_SDNET_CAM_ERR_INVALID_PREFIX_START,                  /**< CAM return code: Error invalid start*/
    XIL_SDNET_CAM_ERR_INVALID_PREFIX_KEY,                    /**< CAM return code: Error invalid prefix key*/
    XIL_SDNET_CAM_ERR_INVALID_BIT_FIELD_LEN,                 /**< CAM return code: Error invalid bit field length*/
    XIL_SDNET_CAM_ERR_INVALID_BIT_FIELD_START,               /**< CAM return code: Error invalid bit field start*/
    XIL_SDNET_CAM_ERR_INVALID_CONST_FIELD_LEN,               /**< CAM return code: Error invalid constant field length*/
    XIL_SDNET_CAM_ERR_INVALID_CONST_FIELD_START,             /**< CAM return code: Error invalid constant field start*/
    XIL_SDNET_CAM_ERR_INVALID_UNUSED_FIELD_LEN,              /**< CAM return code: Error invalid unused field length*/
    XIL_SDNET_CAM_ERR_INVALID_UNUSED_FIELD_START,            /**< CAM return code: Error invalid unused field start*/
    XIL_SDNET_CAM_ERR_MAX_KEY_LEN_EXCEED,                    /**< CAM return code: Error max key length exceeded*/
    XIL_SDNET_CAM_ERR_INVALID_PRIO_AND_INDEX_WIDTH,          /**< CAM return code: Error invalid priority and index width*/
    XIL_SDNET_CAM_ERR_TOO_MANY_UNITS,                        /**< CAM return code: Error too many units*/
    XIL_SDNET_CAM_ERR_NO_MASK,                               /**< CAM return code: Error no mask*/
    XIL_SDNET_CAM_ERR_INVALID_MEMORY_WIDTH,                  /**< CAM return code: Error invalid memory width*/
    XIL_SDNET_CAM_ERR_UNSUPPORTED_COMMAND,                   /**< CAM return code: Unsupported command*/
    XIL_SDNET_CAM_ERR_ENVIRONMENT,                           /**< CAM return code: CAM environment error or IP non-responsive*/
    XIL_SDNET_CAM_ERR_UNSUPPORTED_CAM_TYPE,                  /**< CAM return code: API request for an unsupported cam type*/
    XIL_SDNET_CAM_ERR_NULL_POINTER,                          /**< CAM return code: Null pointer*/
    XIL_SDNET_CAM_ERR_TOO_MANY_PCS,                          /**< CAM return code: Too many pseudo-channels (PCs) are used*/
    XIL_SDNET_CAM_ERR_CONFIGURATION,                         /**< CAM return code: Computational BCAM configurations do not match sw setttings*/
    XIL_SDNET_CAM_ERR_ENVIRONMENT_FSMBUSY,                   /**< CAM return code: Computational BCAM FSM reporting busy */
    XIL_SDNET_CAM_ERR_ENVIRONMENT_POLLED_OUT,                /**< CAM return code: Computational BCAM has polled status register beyond the max allowed */
    XIL_SDNET_CAM_ERR_SEGMENT_COUNT_ZERO,                    /**< CAM return code: Virtual BCAM has zero segments */
    XIL_SDNET_CAM_ERR_VC_UNSUPPORTED_COMMAND,                /**< CAM return code: Unsupported Virtual container command */
    XIL_SDNET_CAM_ERR_WRONG_VIRTUAL_CAM_ID,                  /**< CAM return code: Wrong virtual BCAM identifier */
    XIL_SDNET_CAM_ERR_WRONG_SEGMENT_NUMBER,                  /**< CAM return code: Wrong segment number */
    XIL_SDNET_CAM_ERR_TOO_MANY_RAMS,                         /**< CAM return code: Too many RAM instances are used */
    XIL_SDNET_CAM_ERR_TOO_SMALL_HEAP,                        /**< CAM return code: The max heap for tcam is not sufficiently large */
    XIL_SDNET_CAM_ERR_TOO_SMALL,                             /**< CAM return code: The RAM is not large enough to be divided into virtual segments*/
    XIL_SDNET_CAM_ERR_NOMEM,                                 /**< CAM return code: No memory available*/
    XIL_SDNET_CAM_ERR_MALLOC_FAILED,                         /**< CAM return code: Error malloc failed*/
    XIL_SDNET_CAM_ERR_OPTIMIZATION_TYPE_UNKNOWN,             /**< CAM return code: Error optimization type unknown*/
    XIL_SDNET_CAM_ERR_UNKNOWN,                               /**< CAM return code: Error unknown*/

    XIL_SDNET_TABLE_ERR_INVALID_TABLE_HANDLE_DRV,            /**< Table return code: Error invalid table handle driver, possibily uninitialized*/
    XIL_SDNET_TABLE_ERR_INVALID_TABLE_MODE,                  /**< Table return code: Error invalid table mode specified */
    XIL_SDNET_TABLE_ERR_INVALID_ACTION_ID,                   /**< Table return code: Error invalid action id */
    XIL_SDNET_TABLE_ERR_PARAM_NOT_REQUIRED,                  /**< Table return code: Error parameter not required for this mode */
    XIL_SDNET_TABLE_ERR_ACTION_NOT_FOUND,                    /**< Table return code: Error action not found*/
    XIL_SDNET_TABLE_ERR_FUNCTION_NOT_SUPPORTED,              /**< Table return code: Error function is not supported*/
    XIL_SDNET_TABLE_ERR_MALLOC_FAILED,                       /**< Table return code: Error malloc failed*/
    XIL_SDNET_TABLE_ERR_NAME_BUFFER_TOO_SMALL,               /**< Table return code: Error buffer for name string is to small to hold the full name*/

    XIL_SDNET_TARGET_ERR_INCOMPATIBLE_SW_HW,                 /**< Target return code: Software and Hardware are incompatible*/
    XIL_SDNET_TARGET_ERR_MALLOC_FAILED,                      /**< Target return code: Error malloc failed*/
    XIL_SDNET_TARGET_ERR_TABLE_NOT_FOUND,                    /**< Target return code: Error table not found*/
    XIL_SDNET_TARGET_ERR_MGMT_DRV_NOT_AVAILABLE,             /**< Target return code: Error management driver not available*/
    XIL_SDNET_TARGET_ERR_NAME_BUFFER_TOO_SMALL,              /**< Target return code: Error buffer for name string is to small to hold the full name*/
    XIL_SDNET_TINY_CAM_ENTRY_OCCUPIED,                       /**< Tiny CAM return code: Error Entry already exists */

    /* Always last - do not move */
    XIL_SDNET_NUM_RETURN_CODES                               /**< Number of return codes - Always last entry in enum*/
} XilSdnetReturnType;


/**
 * Specifies the endianness of byte arrays accepted by drivers (where configurable)
 */

typedef enum XilSdnetEndian
{
    XIL_SDNET_LITTLE_ENDIAN,    /**< Byte arrays use the little endian format */
    XIL_SDNET_BIG_ENDIAN,       /**< Byte arrays use the big endian format */
    XIL_SDNET_NUM_ENDIAN        /**< For validation by driver - do not use */
} XilSdnetEndian;

typedef void * XilSdnetUserCtxType;
typedef uintptr_t XilSdnetAddressType;

/* Forward declaration to facilitate callback definitions */
typedef struct XilSdnetEnvIf XilSdnetEnvIf;

typedef XilSdnetReturnType(*XilSdnetWordWrite32Fp)(XilSdnetEnvIf *EnvIfPtr, XilSdnetAddressType Address, uint32_t WriteValue);
typedef XilSdnetReturnType(*XilSdnetWordRead32Fp)(XilSdnetEnvIf *EnvIfPtr, XilSdnetAddressType Address, uint32_t *ReadValuePtr);
typedef XilSdnetReturnType(*XilSdnetLogFp)(XilSdnetEnvIf *EnvIfPtr, const char *MessagePtr);

/** SDNet type used to hold versions for both software and IP */
typedef struct XilSdnetVersion
{
    uint8_t Major;  /**< SDNet major version number*/
    uint8_t Minor;  /**< SDNet minor version number*/
    uint8_t Revision; /**< SDNet revision number*/
} XilSdnetVersion;

/** SDNet environment interface definition.
 *
 * This structure is used by all SDNet control plane drivers to obtain access to
 * platform-specific functionality such as FPGA register access.  Customer
 * applications are required to allocate an instance of this structure and
 * populate its contents with type-compatible callback functions to implement
 * the functionality described by the names of the members of this structure.
 * The instance is then passed by application software into the SDNet control
 * plane drivers during their initialization. During initialization, the drivers
 * make a copy of the structure provided, meaning that the instance allocated by
 * the application can be deallocated if desired.
 *
 * The structure also provides a UserCtx member, which can be used to share any
 * data structure that needed to support the implementation of the
 * platform-specific functions.  An example use-case for this facility would be
 * an application in which multiple instances of SDNet are in use, each residing
 * on a different FPGA.  In this scenario, the application could provide a
 * single implementation of the register access functions that is
 * shared by all SDNet instances and use the UserCtx member to provide
 * additional information (such as an FPGA device name or ID), to allow the
 * implementation of those functions to direct the register accesses to the
 * correct FPGA.
 *
 * The key caveat of this capability is that the structure pointed to by the
 * UserCtx field <b>must persist in memory for as long as the driver is in use
 * by the application</b>.  To put this in more explicit terms, the structure
 * generally shouldn't be a local variable in a function - it should instead
 * be either at global scope or allocated from the heap.
 */
struct XilSdnetEnvIf
{
    XilSdnetUserCtxType     UserCtx;        /**< Pointer to application-specific context needed by callbacks (set to NULL if not needed) */
    XilSdnetWordWrite32Fp   WordWrite32;    /**< Callback for function to perform a 32-bit write to a hardware register */
    XilSdnetWordRead32Fp    WordRead32;     /**< Callback for function to perform a 32-bit read from a hardware register */
    XilSdnetLogFp           LogError;       /**< Callback for function to emit error messages */
    XilSdnetLogFp           LogInfo;        /**< Callback for function to emit informational messages */
};



/****************************************************************************************************************************************************/
/* SECTION: Global function prototypes */
/****************************************************************************************************************************************************/

/**
 * Convert a return code to a C-style string
 *
 * This function accepts a return code as an input and returns a string representation of that error
 * code.  This function can be useful for printing human-readable error messages.
 *
 * @param[in] Value     The return code to look up
 * *
 * @return NULL is returned on error, otherwise a string representation of the code specifed by value is returned.
 */

const char *XilSdnetReturnTypeToString(XilSdnetReturnType Value);

/**
 * Create a stubbed environment interface
 *
 * This function populates an instance of \ref XilSdnetEnvIf with a set of well-behaved stub functions.
 * Well-behaved in this context means that the functions will execute gracefully without side-effects
 * when used correctly, but will return an error if invalid parameters are passed.
 *
 * All SDNet control plane drivers require a properly populated instance of this structure to be
 * provided during initialization.  In some cases, an application may not wish to provide all of
 * the functions required, so this utility function can be used to initialize the structure with
 * a set of defaults - the user can then optionally override the functions that they do wish to
 * provide with implementations suitable for the platform.
 *
 * @param[in,out] EnvIfPtr      Pointer to environment interface definition, populated by the call
 * *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code.
 */
XilSdnetReturnType XilSdnetStubEnvIf(XilSdnetEnvIf *EnvIfPtr);

/* Utilty validation function - intended for use by driver functions */
XilSdnetReturnType XilSdnetValidateEnvIf(XilSdnetEnvIf *EnvIfPtr);

/* Utility functions for printf-style logging - intended for use by the driver functions */
XilSdnetReturnType XilSdnetLogError(XilSdnetEnvIf *EnvIfPtr, const char *FormatStringPtr, ...);
XilSdnetReturnType XilSdnetLogInfo(XilSdnetEnvIf *EnvIfPtr, const char *FormatStringPtr, ...);

#ifdef __cplusplus
}
#endif

#endif

/** @} */
