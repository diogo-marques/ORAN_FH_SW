/*
-- (c) Copyright 2020 Xilinx, Inc. All rights reserved.
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
-- Revision       : $Revision: #3 $
-- Date           : $DateTime: 2021/01/07 04:30:57 $
-- Last Author    : $Author: fmartin $
--
--------------------------------------------------------------------------------
-- Description : Header file for the SDNet target management drivers
--
--
--------------------------------------------------------------------------------
*/
/**
 * Public API for a set of drivers that provide access to SDNet IP specific elements
 *
 * @file
 * @addtogroup sdnet_ip_specific_elements
 * @{
 * @details
 *
 * \section Introduction
 *
 * The SDNet target management is a collection of IP specific drivers that
 * - collect IP Build Information include the IP Version
 * - control the triggering ECC Error Interrupts to inform user when ECC Error
 *   information is available
 *
 * The build information driver provides the following functions to retrieve
 * build time information:
 *  - @ref XilSdnetTargetBuildInfoInit()
 *  - @ref XilSdnetTargetBuildInfoGetIpVersion()
 *  - @ref XilSdnetTargetBuildInfoGetIpSettings()
 *  - @ref XilSdnetTargetBuildInfoExit()
 *
 * The interrupt driver provides function to control the interrupts for single
 * or double bit ECC error detection for SDNet target components. The components
 * that support ECC error detection are as follows:
 *  - P4 elements supported by SDNet, see @ref target for further details
 *  - Internal Component's used in the SDNet target
 *
 * The drivers interfaces provided provide the capability to enable, disable and
 * read plus clear the interrupt status for an individual element or Component.
 *
 * The P4 Element interrupt related functions accept a parameter called P4ElementId.
 * @ref target driver provides APIs that accepts a string containing the name of the
 * P4 element retrieves the corresponding ID.
 *
 * Note: Some configuration , e.g. type of memory, used for P4 elements result
 *       in P4 elements that do not support ECC error interrupts. For these cases
 *       driver will indicate the successful enabling of interrupts but no interrupts
 *       will be generated for these elements.
 *
 */

#ifndef SDNET_TARGET_MGMT_H
#define SDNET_TARGET_MGMT_H

/****************************************************************************************************************************************************/
/* SECTION: Header includes */
/****************************************************************************************************************************************************/
#include <stdbool.h>
#include "sdnet_common.h"

#ifdef __cplusplus
extern "C" {
#endif


/****************************************************************************************************************************************************/
/* SECTION: Constants */
/****************************************************************************************************************************************************/
typedef enum XilSdnetTargetInterruptEccErrorType
{
    XIL_SDNET_INTERRUPT_ECC_ERROR_SINGLE_BIT,
    XIL_SDNET_INTERRUPT_ECC_ERROR_DOUBLE_BIT,
    XIL_SDNET_INTERRUPT_ECC_ERROR_ALL,
    XIL_SDNET_INTERRUPT_ECC_ERROR_TYPE_MAX  /**< For validation by driver - do not use */
} XilSdnetTargetInterruptEccErrorType;

/****************************************************************************************************************************************************/
/* SECTION: Type definitions  */
/****************************************************************************************************************************************************/

typedef enum XilSdnetTargetCtrlEngineId
{
    XIL_SDNET_TARGET_CTRL_ALL_ENGINES,
    XIL_SDNET_TARGET_CTRL_DEPARSER_ENGINE,
    XIL_SDNET_TARGET_CTRL_FIFO_ENGINE,
    XIL_SDNET_TARGET_CTRL_MA_ENGINE,
    XIL_SDNET_TARGET_CTRL_PARSER_ENGINE,
} XilSdnetTargetCtrlEngineId;

/*
 * Context typedef structures
 */

/** Forward declaration to support context structure declaration */
typedef struct XilSdnetTargetBuildInfoPrivateCtx XilSdnetTargetBuildInfoPrivateCtx;

/** Holds context information needed by the SDNet instance information driver's API */
typedef struct XilSdnetTargetBuildInfoCtx
{
    XilSdnetTargetBuildInfoPrivateCtx *PrivateCtxPtr;  /**< Internal context data used by driver implementation */
} XilSdnetTargetBuildInfoCtx;

/** Forward declaration to support context structure declaration */
typedef struct XilSdnetTargetInterruptPrivateCtx XilSdnetTargetInterruptPrivateCtx;

/** Holds context information needed by the SDNet instance information driver's API */
typedef struct XilSdnetTargetInterruptCtx
{
    XilSdnetTargetInterruptPrivateCtx *PrivateCtxPtr;  /**< Internal context data used by driver implementation */
} XilSdnetTargetInterruptCtx;

/*
 * Configuration typedef structures
 */

/** Configuration that describes the block information of the SDNet instance */
typedef struct XilSdnetTargetBuildInfoConfig
{
    XilSdnetAddressType BaseAddr;   /**< Base address within the SDNet instance */
} XilSdnetTargetBuildInfoConfig;


typedef struct XilSdnetComponentName
{
    char *NameStringPtr;
} XilSdnetComponentName;

/** Configuration that describes the interrupt controller of the SDNet instance  */
typedef struct XilSdnetTargetInterruptConfig
{
    XilSdnetAddressType      BaseAddr;           /**< Base address within the SDNet instance */
    uint32_t                 NumP4Elements;        /**< Number of P4 Elements present in the SDNet instance */
    uint32_t                 NumComponents;           /**< Number of internal Components present in the SDNet instance */
    XilSdnetComponentName    ComponentNameList[];     /**< List of Components name that are present in the SDNet instance */
} XilSdnetTargetInterruptConfig;

/*
 * typedefs for API's
 */

typedef struct XilSdnetTargetBuildInfoIpSettings
{
    uint32_t PacketRateMps;      /**< Packet rate in Mp/s*/
    uint32_t CamMemoryClockMhz;  /**< CAM memory clock in MHz*/
    uint32_t AxiStreamClockMhz;   /**< AXI Stream clock in MHz*/
} XilSdnetTargetBuildInfoIpSettings;

typedef struct XilSdnetTargetInterruptEccErrorStatus
{
    bool SingleBitErrorStatus; /**< */
    bool DoubleBitErrorStatus; /**< */
} XilSdnetTargetInterruptEccErrorStatus;

/*** Control driver definitions****/

/** Forward declaration to support context structure declaration */
typedef struct XilSdnetTargetCtrlPrivateCtx XilSdnetTargetCtrlPrivateCtx;

/** Holds context information needed by the SDNet instance information driver's API */
typedef struct XilSdnetTargetCtrlCtx
{
    XilSdnetTargetCtrlPrivateCtx* PrivateCtxPtr;  /**< Internal context data used by driver implementation */
} XilSdnetTargetCtrlCtx;

/*
 * Configuration typedef structures
 */

/** Configuration that describes the interrupt controller of the SDNet instance  */
typedef struct XilSdnetTargetCtrlConfig
{
    XilSdnetAddressType BaseAddr;           /**< Base address within the SDNet instance */
    uint32_t            NumP4Elements;        /**< Number of P4 Elements present in the SDNet instance */
    uint32_t            NumComponents;           /**< Number of internal FIFOs present in the SDNet instance */
    uint32_t            ClkInHz;            /* Clock in Hz */
    uint32_t            PktRatePerSec;      /* Packet rate in packets per second */
} XilSdnetTargetCtrlConfig;

/****************************************************************************************************************************************************/
/* SECTION: Global Function prototypes */
/****************************************************************************************************************************************************/

/**
 * @addtogroup sdnet_build_info SDNet Build Information driver public API
 * @{
 */

/**
 * Initialization function for the Build Information driver API
 *
 * @param[in] CtxPtr            Pointer to an uninitialized SDNet instance information context structure instance.
 *                              Is populated by function execution.
 *
 * @param[in] EnvIfPtr          Pointer to environment interface definition
 *
 * @param[in] ConfigPtr         Pointer to driver configuration definition
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code
 */
XilSdnetReturnType XilSdnetTargetBuildInfoInit(XilSdnetTargetBuildInfoCtx *CtxPtr, XilSdnetEnvIf *EnvIfPtr, XilSdnetTargetBuildInfoConfig *ConfigPtr);

/**
 * Get the IP Version for the SDNet Instance
 *
 * @param[in]  CtxPtr           Pointer to the SDNet instance information context.
 *
 * @param[out] IpVersionPtr     Pointer to a structure that stores the IP version.
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code.
 */
XilSdnetReturnType XilSdnetTargetBuildInfoGetIpVersion(XilSdnetTargetBuildInfoCtx *CtxPtr, XilSdnetVersion *IpVersionPtr);

/**
 * Get the setting parameters for the SDNet Instance
 *
 * @param[in]  CtxPtr           Pointer to the SDNet instance information context.
 *
 * @param[out] SettingsPtr    Pointer to a structure that stores the setting parameters.
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code.
 */
XilSdnetReturnType XilSdnetTargetBuildInfoGetIpSettings(XilSdnetTargetBuildInfoCtx *CtxPtr, XilSdnetTargetBuildInfoIpSettings *IpSettingsPtr);

/**
 * Destroy the Build Information instance
 *
 * All memory allocated by \ref XilSdnetTargetBuildInfoInit is released.
 *
 * @param[in]  CtxPtr           Pointer to the SDNet instance information context.
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code.
 */
XilSdnetReturnType XilSdnetTargetBuildInfoExit(XilSdnetTargetBuildInfoCtx *CtxPtr);

/** @} */

/**
 * @addtogroup sdnet_interrupt SDNet Interrupt Driver public API
 * @{
    Manages the triggering of ECC Interrupts for P4 Elements and internal Components present in the SDNet Instance.
 */

/**
 * Initialization function for the Interrupt driver API
 *
 * @param[in] CtxPtr            Pointer to an uninitialized interrupt controller context structure instance.
 *                              Is populated by function execution.
 *
 * @param[in] EnvIfPtr          Pointer to environment interface definition.
 *
 * @param[in] ConfigPtr         Pointer to driver configuration definition.
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code.
 */
XilSdnetReturnType XilSdnetTargetInterruptInit(XilSdnetTargetInterruptCtx *CtxPtr, XilSdnetEnvIf *EnvIfPtr, XilSdnetTargetInterruptConfig *ConfigPtr);

/**
 * Gets the Number of P4 elements that have ECC interrupt controls
 *
 * @param[in]  CtxPtr           Pointer to the interrupt controller context.
 *
 * @param[out] NumP4ElementsPtr   Pointer to variable used to store the number of elements value.
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code.
 */
XilSdnetReturnType XilSdnetTargetInterruptGetP4ElementCount(XilSdnetTargetInterruptCtx *CtxPtr, uint32_t *NumP4ElementsPtr);

/**
 * Gets the number of internal Components that have ECC interrupt controls
 *
 * @param[in]  CtxPtr           Pointer to the interrupt controller context.
 *
 * @param[out] NumFifosPtr      Pointer to variable used to store the number of Components.
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code.
 */
XilSdnetReturnType XilSdnetTargetInterruptGetComponentCount(XilSdnetTargetInterruptCtx *CtxPtr, uint32_t * NumComponentPtr);

/**
 * Gets the index for the named Component, if present
 *
 * If the Component is not present, the function fails by returning error code XIL_SDNET_INTERRUPT_ERR_COMPONENT_NOT_FOUND.
 *
 * @param[in] CtxPtr            Pointer to the interrupt controller context.
 *
 * @param[in] ComponentNamePtr       Pointer to char buffer that holds the Component name.
 *
 * @param[out] IndexPtr         Pointer to variable used to store the number of Components.
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code.
 */
XilSdnetReturnType XilSdnetTargetInterruptGetComponentIndexByName(XilSdnetTargetInterruptCtx *CtxPtr, char * ComponentNamePtr, uint32_t *IndexPtr);

/**
 * Get the Component name for a specific index, supports iterative liner search for all Component names present
 *
 * If the entry is not found, the function fails with error code XIL_SDNET_INTERRUPT_ERR_COMPONENT_NOT_FOUND.
 *
 * The IndexPtr argument enables iterative search for Component names. The first time the function is
 * called the integer at IndexPtr should be set to zero. To find all Component names, the function is
 * called multiple times, each call returns a single Component name.
 *
 * For every consecutive call the returned IndexPtr value must be used again as input argument.
 * When XIL_SDNET_INTERRUPT_ERR_COMPONENT_NOT_FOUND is returned no more FIFOs are present.
 *
 * @param[in] CtxPtr            Pointer to the interrupt controller context.
 *
 * @param[in,out] IndexPtr      Index of the Component. Used for consecutive get operations, set to 0 for
 *                              first get.
 *
 * @param[out] ComponentNamePtr      Pointer to char buffer to hold the table name.
 *
 * @param[in] ComponentNameNumBytes  The number of characters the Component name buffer can hold including the null terminating character.
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code.
 */
XilSdnetReturnType XilSdnetTargetInterruptGetComponentNameByIndex(XilSdnetTargetInterruptCtx *CtxPtr, uint32_t *IndexPtr, char *ComponentNamePtr, uint32_t ComponentNameNumBytes);

/**
 * Enable ECC Error interrupts for a specified P4 element
 *
 * If the element identifier does not exist, the function fails by returning error code XIL_SDNET_INTERRUPT_ERR_INVALID_ELEMENT_ID.
 *
 * @param[in] CtxPtr            Pointer to the interrupt controller context.
 *
 * @param[in] P4ElementId         Stores the P4 element identifier.
 *
 * @param[in] EccErrorType      Selects which ECC interrupts are enabled for this element.
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code.
 */
XilSdnetReturnType XilSdnetTargetInterruptEnableP4ElementEccErrorById(XilSdnetTargetInterruptCtx *CtxPtr, uint32_t ElementId, XilSdnetTargetInterruptEccErrorType EccErrorType);

/**
 * Disable ECC Error interrupts for a specified P4 element
 *
 * If the element identifier does not exist, the function fails by returning error code XIL_SDNET_INTERRUPT_ERR_INVALID_ELEMENT_ID.
 *
 * @param[in]   CtxPtr           Pointer to the interrupt controller context.
 *
 * @param[in]   P4ElementId       Stores the P4 element identifier.
 *
 * @param[in] EccErrorType      Selects which ECC interrupts are disabled for this element.
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code.
 */
XilSdnetReturnType XilSdnetTargetInterruptDisableP4ElementEccErrorById(XilSdnetTargetInterruptCtx *CtxPtr, uint32_t ElementId, XilSdnetTargetInterruptEccErrorType EccErrorType);

/**
 * Get the ECC Error interrupt status for a specified P4 element
 *
 * If the element identifier does not exist, the function fails by returning error code XIL_SDNET_INTERRUPT_ERR_INVALID_ELEMENT_ID.
 *
 * @param[in]   CtxPtr          Pointer to the interrupt controller context.
 *
 * @param[in]   P4ElementId       Stores the P4 element identifier.
 *
 * @param[out]  StatusPtr       Pointer to structure that stores the ECC interrupt status.
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code.
 */
XilSdnetReturnType XilSdnetTargetInterruptGetP4ElementEccErrorStatusById(XilSdnetTargetInterruptCtx *CtxPtr, uint32_t ElementId, XilSdnetTargetInterruptEccErrorStatus *StatusPtr);

/**
 * Clear the ECC Error interrupt status for a specified P4 element
 *
 * If the element identifier does not exist, the function fails by returning error code XIL_SDNET_INTERRUPT_ERR_INVALID_ELEMENT_ID.
 *
 * @param[in]  CtxPtr           Pointer to the interrupt controller context.
 *
 * @param[in] P4ElementId         Stores the P4 element identifier.
 *
 * @param[in] EccErrorType      Selects which ECC interrupts are cleared for this element.
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code.
 */
XilSdnetReturnType XilSdnetTargetInterruptClearP4ElementEccErrorStatusById(XilSdnetTargetInterruptCtx *CtxPtr, uint32_t ElementId, XilSdnetTargetInterruptEccErrorType EccErrorType);

/**
 * Enable ECC Error interrupts for a specified Component
 *
 * If a Component does not exist at the specified index, the function fails by returning error code XIL_SDNET_INTERRUPT_ERR_INVALID_COMPONENT_INDEX.
 *
 * @param[in] CtxPtr        Pointer to the interrupt controller context.
 *
 * @param[in] ComponentIndex     Stores the Component index.
 *
 * @param[in] EccErrorType  Selects which ECC interrupts are enabled for this Component.
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code.
 */
XilSdnetReturnType XilSdnetTargetInterruptEnableComponentEccErrorByIndex(XilSdnetTargetInterruptCtx *CtxPtr, uint32_t ComponentIndex, XilSdnetTargetInterruptEccErrorType EccErrorType);

/**
 * Disable ECC Error interrupts for a specified Component
 *
 * If the element identifier does not exist, the function fails by returning error code XIL_SDNET_INTERRUPT_ERR_INVALID_COMPONENT_INDEX.
 *
 * @param[in] CtxPtr        Pointer to the interrupt controller context.
 *
 * @param[in] ComponentIndex     Stores the Component index.
 *
 * @param[in] EccErrorType  Selects which ECC interrupts are disabled for this Component.
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code.
 */
XilSdnetReturnType XilSdnetTargetInterruptDisableComponentEccErrorByIndex(XilSdnetTargetInterruptCtx *CtxPtr, uint32_t ComponentIndex, XilSdnetTargetInterruptEccErrorType EccErrorType);

/**
 * Get the ECC Error interrupt status for a specified Component
 *
 * If the element identifier does not exist, the function fails by returning error code XIL_SDNET_INTERRUPT_ERR_INVALID_COMPONENT_INDEX.
 *
 * @param[in]  CtxPtr       Pointer to the interrupt controller context.
 *
 * @param[in]  ComponentIndex    Stores the Component index.
 *
 * @param[out] StatusPtr    Pointer to structure that stores the ECC interrupt status.
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code.
 */
XilSdnetReturnType XilSdnetTargetInterruptGetComponentEccErrorStatusByIndex(XilSdnetTargetInterruptCtx *CtxPtr, uint32_t ComponentIndex, XilSdnetTargetInterruptEccErrorStatus *StatusPtr);

/**
 * Clear the ECC Error interrupt status for a specified Component
 *
 * If the element identifier does not exist, the function fails by returning error code XIL_SDNET_INTERRUPT_ERR_INVALID_COMPONENT_INDEX.
 *
 * @param[in] CtxPtr        Pointer to the interrupt controller context.
 *
 * @param[in] ComponentIndex     Stores the Component index.
 *
 * @param[in] EccErrorType  Selects which ECC interrupts are cleared for this element.
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code.
 */
XilSdnetReturnType XilSdnetTargetInterruptClearComponentEccErrorStatusByIndex(XilSdnetTargetInterruptCtx *CtxPtr, uint32_t ComponentIndex, XilSdnetTargetInterruptEccErrorType EccErrorType);

/**
 * Destroy the Interrupt instance
 *
 * All memory allocated by \ref XilSdnetTargetInterruptInit is released.
 *
 * @param[in]  CtxPtr           Pointer to the SDNet instance information context
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code
 */
XilSdnetReturnType XilSdnetTargetInterruptExit(XilSdnetTargetInterruptCtx *CtxPtr);

/**
 * Initialization function for the Target Control driver API
 *
 * @param[in,out] CtxPtr        Pointer to an uninitialized Control Register context structure instance.
 *                              Is populated by function execution.
 *
 * @param[in] EnvIfPtr          Pointer to environment interface definition
 *
 * @param[in] ConfigPtr         Pointer to Target Control driver configuration definition
 *
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code.
 */
XilSdnetReturnType XilSdnetTargetCtrlInit(XilSdnetTargetCtrlCtx* CtxPtr,
                                          XilSdnetEnvIf* EnvIfPtr,
                                          XilSdnetTargetCtrlConfig* ConfigPtr);

/**
 * Destroy the Target Control instance
 *
 * All memory allocated by \ref XilSdnetTargetCtrlInit is released.
 *
 * @param[in] CtxPtr    Pointer to the BCAM instance context
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code.
 */
XilSdnetReturnType XilSdnetTargetCtrlExit(XilSdnetTargetCtrlCtx* CtxPtr);


/**
 * Retrieve the element count
 *
 * @param[in]   CtxPtr         Pointer to the control register context.
 *
 * @param[in]   NumP4ElementsPtr  Pointer to the number of elements
 *
 * @ret
 */
XilSdnetReturnType XilSdnetTargetCtrlGetP4ElementCount(XilSdnetTargetCtrlCtx* CtxPtr, uint32_t* NumElementsPtr);

/**
 * Retrieve the clock in Hz
 *
 * If the engine type identifier does not exist, the function fails by returning error code.
 *
 * @param[in]   CtxPtr         Pointer to the control register context.
 *
 * @param[in]   ClkInHzPtr  Pointer to the clock in Hz
 *
 * @ret
 */
XilSdnetReturnType XilSdnetTargetCtrlGetClkInHz(XilSdnetTargetCtrlCtx* CtxPtr, uint32_t* ClkInHzPtr);

/**
 * Retrieve the number of components
 *
 *
 * @param[in]   CtxPtr         Pointer to the control register context.
 *
 * @param[in]   NumComponentsPtr  Pointer to the number of components
 *
 * @ret
 */
XilSdnetReturnType XilSdnetTargetCtrlGetNumComponents(XilSdnetTargetCtrlCtx* CtxPtr, uint32_t* NumComponentsPtr);

/**
 * Retrieve the packet rate per second
 *
 *
 * @param[in]   CtxPtr         Pointer to the control register context.
 *
 * @param[in]   PktRatePerSecPtr  Pointer to the packet rate per second
 *
 * @ret
 */
XilSdnetReturnType XilSdnetTargetCtrlGetPktRatePerSec(XilSdnetTargetCtrlCtx* CtxPtr, uint32_t* PktRatePerSecPtr);

/**
 * Retrieve the packet rate to clock ratio
 *
 *
 * @param[in]   CtxPtr         Pointer to the control register context.
 *
 * @param[in]   Pr2ClkRatioPtr  Pointer to the packet rate to clock ratio
 *
 * @ret
 */
XilSdnetReturnType XilSdnetTargetCtrlGetPr2ClkRatio(XilSdnetTargetCtrlCtx* CtxPtr, float* Pr2ClkRatioPtr);

/**
 * Resets an SDNET engine
 *
 * If the engine type identifier does not exist, the function fails by returning error code.
 *
 * @param[in]   CtxPtr         Pointer to the control register context.
 *
 * @param[in]   EngineId      type of reset
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code.
 */
XilSdnetReturnType XilSdnetTargetCtrlSoftResetEngine(XilSdnetTargetCtrlCtx* CtxPtr, XilSdnetTargetCtrlEngineId EngineId);

/**
 * Set the maximum Start of Frame (SOF) counter per interval
 *
 * @param[in]   CtxPtr  Pointer to the control register context.
 *
 * @param[in]   PacketRateMargin  Packet rate Margin. Integer value for the number of hundreds of a percentage e.g. 512 is 5.12% 
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code.
 */
XilSdnetReturnType XilSdnetTargetCtrlSetPacketRateLimitMargin(XilSdnetTargetCtrlCtx* CtxPtr, uint32_t PacketRateMargin);

/**
 * Get the maximum Start of Frame (SOF) counter per interval
 *
 * @param[in]   CtxPtr         Pointer to the control register context.
 *
 * @param[in]   MaxSofCounterPerInvlPtr      Retrieved 16 bit maximum SOF counter per interval value
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code.
 */
XilSdnetReturnType XilSdnetTargetCtrlGetPacketRateLimitMargin(XilSdnetTargetCtrlCtx* CtxPtr, uint32_t* PacketRateMarginPtr);

/**
 * Start injecting ECC bit errors for the IP Component
 *
 * If the ComponentIndex does not exist and XIL_SDNET_INTERRUPT_ERR_INVALID_COMPONENT_INDEX is returned
 *
 * @param[in] CtxPtr         Pointer to the control register context.
 *
 * @param[in] ComponentIndex     Stores the FIFO index.
 *
 * @param[in] EccErrorType  Selects which ECC interrupts are disabled for this FIFO.
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code.
 */
XilSdnetReturnType XilSdnetTargetCtrlIpComponentEnableInjectEccError(XilSdnetTargetCtrlCtx* CtxPtr,
                                                                     uint32_t ComponentIndex,
                                                                     XilSdnetTargetInterruptEccErrorType EccErrorType);


/**
 * Stop injecting ECC bit errors for the IP Component
 *
 * If the ComponentIndex does not exist and XIL_SDNET_INTERRUPT_ERR_INVALID_COMPONENT_INDEX is returned
 *
 * @param[in] CtxPtr         Pointer to the control register context.
 *
 * @param[in] ComponentIndex     Stores the FIFO index.
 *
 * @param[in] EccErrorType  Selects which ECC interrupts are disabled for this FIFO.
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code.
 */
XilSdnetReturnType XilSdnetTargetCtrlIpComponentDisableInjectEccError(XilSdnetTargetCtrlCtx* CtxPtr,
                                                                      uint32_t ComponentIndex,
                                                                      XilSdnetTargetInterruptEccErrorType EccErrorType);


/**
 * Start injecting ECC bit errors for the P4 specific P4ElementId
 *
 * If the P4ElementId does not exist and error code is returned
 *
 * @param[in] CtxPtr         Pointer to the control register context.
 *
 * @param[in] P4ElementId         Stores the P4 element identifier.
 *
 * @param[in] EccErrorType      Selects which ECC interrupts are enabled for this element.
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code.
 */
XilSdnetReturnType XilSdnetTargetCtrlP4ElementEnableInjectEccError(XilSdnetTargetCtrlCtx* CtxPtr,
                                                                   uint32_t P4ElementId,
                                                                   XilSdnetTargetInterruptEccErrorType EccErrorType);

/**
 * Stop injecting ECC bit errors for the P4 specific P4ElementId
 *
 * If the P4ElementId does not exist and error code is returned
 *
 * @param[in] CtxPtr         Pointer to the control register context.
 *
 * @param[in] P4ElementId         Stores the P4 element identifier.
 *
 * @param[in] EccErrorType      Selects which ECC interrupts are enabled for this element.
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code.
 */
XilSdnetReturnType XilSdnetTargetCtrlP4ElementDisableInjectEccError(XilSdnetTargetCtrlCtx* CtxPtr,
                                                                    uint32_t P4ElementId,
                                                                    XilSdnetTargetInterruptEccErrorType EccErrorType);


/** @} */


#ifdef __cplusplus
}
#endif

#endif

/** @} */
