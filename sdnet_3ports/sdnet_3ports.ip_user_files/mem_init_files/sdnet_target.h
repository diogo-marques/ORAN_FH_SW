/*
-- (c) Copyright 2019 Xilinx, Inc. All rights reserved.
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
-- Revision       : $Revision: #6 $
-- Date           : $DateTime: 2021/01/07 04:30:57 $
-- Last Author    : $Author: fmartin $
--
--------------------------------------------------------------------------------
-- Description : Header file for the SDNet target driver
--
--------------------------------------------------------------------------------
*/

/**
 * Public API of the SDNet Target driver.
 *
 * @file sdnet_target.h
 * @addtogroup target SDNet target public API
 * @{
 * @details
 *
 * \section Introduction
 *
 * The target driver is the top level of the SDNet control plane management
 * stack.  It implements two key functions designed to improve ease of use for
 * application development:
 *  - It provides functions to setup and teardown all control plane components
 *    in an SDNet design
 *  - It provides an API which resolves the names of control plane components
 *    to an associated context structure, for use with the corresponding control plane driver
 *
 * To perform this functionality, the target driver consumes a configuration
 * structure that is automatically generated when SDNet is run. This
 * configuration structure describes all control plane manageable components
 * present in the SDNet design.
 *
 * The control plane components can be either SDnet directly managed P4 elements or
 * IP specific elements
 *
 * The SDnet P4 elements available  in SDNet are:
 *  - Tables
 *
 * The IP specific elements in SDNet are:
 *  - Build information reader
 *  - Interrupt controller
 *
 *
 * \section overview_op Overview of Operation
 *
 * The driver provides the following functions for setup and teardown
 * respectively:
 *  - @ref XilSdnetTargetInit()
 *  - @ref XilSdnetTargetExit()
 *
 * Upon successful execution of @ref XilSdnetTargetInit(), the application is
 * free to call any of the remaining functions.  The primary usecase is to
 * fetch a context structure for a given control plane management component by
 * specifying its name.  In the case of a table, the user would call
 * @ref XilSdnetTargetGetTableByName(), which upon successful execution provides
 * the caller with a pointer to an @ref XilSdnetTableCtx structure, which may
 * then be used with the functions offered by the @ref table driver.
 */

#ifndef SDNET_TARGET_H
#define SDNET_TARGET_H

/****************************************************************************************************************************************************/
/* SECTION: Header includes */
/****************************************************************************************************************************************************/
#include <stdbool.h>

#include "sdnet_table.h"
#include "sdnet_target_mgmt.h"

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************************************************************************************/
/* SECTION: Type definitions */
/****************************************************************************************************************************************************/

/** Wrapper structure to group table name with table configuration */
typedef struct XilSdnetTargetTableConfig
{
    const char          *NameStringPtr; /**< Table control plane name */
    XilSdnetTableConfig Config;         /**< Table configuration */
} XilSdnetTargetTableConfig;


/** Top level configuration data structure describing all control plane manageable elements in an SDNet design */
typedef struct XilSdnetTargetConfig
{
    /****************************************************************************************************************************************************/
    /* Managed P4 Elements Configurations*/
    /****************************************************************************************************************************************************/
    XilSdnetEndian              Endian;         /**< Global endianness setting - applies to all P4 element drivers instantiated by the target */
    uint32_t                    TableListSize;  /**< Number of tables present in the design */
    XilSdnetTargetTableConfig   **TableListPtr; /**< Pointer to list holding the configuration of each table present */

    /****************************************************************************************************************************************************/
    /* IP specific elements Configurations*/
    /****************************************************************************************************************************************************/
    XilSdnetTargetBuildInfoConfig   *BuildInfoPtr;      /**< Pointer to the configuration for the build information reader*/
    XilSdnetTargetInterruptConfig   *InterruptPtr;      /**< Pointer to the configuration for the interrupt manager*/
    XilSdnetTargetCtrlConfig        *CtrlConfigPtr;     /**< Pointer to the configuration for the packet control manager */
} XilSdnetTargetConfig;

/** Forward declaration to support context structure declaration */
typedef struct XilSdnetTargetPrivateCtx XilSdnetTargetPrivateCtx;

/** Holds context information needed by the target driver's API */
typedef struct XilSdnetTargetCtx
{
    XilSdnetTargetPrivateCtx *PrivateCtxPtr;    /**< Internal context data used by driver implementation */
} XilSdnetTargetCtx;

/****************************************************************************************************************************************************/
/* SECTION: Target function prototypes */
/****************************************************************************************************************************************************/

/**
 * Initialization function for the SDNet Target driver.
 *
 * @param[in,out] CtxPtr        Pointer to an uninitialized SDNet Target context structure instance.
 *                              Is populated by function execution.
 *
 * @param[in] EnvIfPtr          Pointer to environment interface definition
 *
 * @param[in] ConfigPtr         Pointer to driver configuration definition
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code.
 */

XilSdnetReturnType XilSdnetTargetInit(XilSdnetTargetCtx *CtxPtr, XilSdnetEnvIf *EnvIfPtr, XilSdnetTargetConfig *ConfigPtr);

/**
 * Destroy the SDNet Target driver instance.
 *
 * @param[in] CtxPtr            Pointer to the SDNet Target instance context.
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code.
 */

XilSdnetReturnType XilSdnetTargetExit(XilSdnetTargetCtx *CtxPtr);

/**
 * Get the Software version
 *
 * @param[in] CtxPtr            Pointer to the SDNet Target instance context.
 *
 * @param[out] SwVersionPtr     Pointer to the structure that holds the software version values
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code.
 */
XilSdnetReturnType XilSdnetTargetGetSwVersion(XilSdnetTargetCtx *CtxPtr, XilSdnetVersion *SwVersionPtr);

/****************************************************************************************************************************************************/
/* SECTION: Table helper function prototypes */
/****************************************************************************************************************************************************/

/**
 * @addtogroup target_hlp  SDNet target: table helper API
 * @{
 * A collection of helper functions for managing the tables present in the target.
 */

/**
 * Gets the context structure for the named table, if present in the target.
 *
 * @param[in] CtxPtr            Pointer to the SDNet Target instance context.
 *
 * @param[in] TableNamePtr      Pointer to string holding table name.
 *
 * @param[out] TableCtxPtrPtr   Pointer to a context structure for the table instance.
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code.
 */

XilSdnetReturnType XilSdnetTargetGetTableByName(XilSdnetTargetCtx *CtxPtr, char *TableNamePtr, XilSdnetTableCtx **TableCtxPtrPtr);

/**
 * Gets the context structure for the indexed table, if present in the target.
 *
 * @param[in] CtxPtr            Pointer to the SDNet Target instance context.
 *
 * @param[in] Index             Integer specifying the table instance to be retrieved.
 *
 * @param[out] TableCtxPtrPtr   Pointer to a context structure for the table instance.
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code.
 */

XilSdnetReturnType XilSdnetTargetGetTableByIndex(XilSdnetTargetCtx *CtxPtr, uint32_t Index, XilSdnetTableCtx **TableCtxPtrPtr);

/**
 * Gets the number of tables present in the target.
 *
 * @param[in] CtxPtr            Pointer to the SDNet Target instance context.
 *
 * @param[out] NumTablesPtr     The number of tables present in the target.
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code.
 */

XilSdnetReturnType XilSdnetTargetGetTableCount(XilSdnetTargetCtx *CtxPtr, uint32_t *NumTablesPtr);

/**
 * Gets the name of the table from its context structure
 *
 * @param[in] CtxPtr                Pointer to the SDNet Target instance context.
 *
 * @param[in] TableCtxPtr           Pointer to a table's context structure.
 *
 * @param[out] TableNamePtr         Pointer to char buffer to hold the table name.
 *
 * @param[in] TableNameNumBytes     The number of characters the table name buffer can hold including the null terminating character.
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code.
 */

XilSdnetReturnType XilSdnetTargetGetTableName(XilSdnetTargetCtx *CtxPtr,
                                              XilSdnetTableCtx *TableCtxPtr,
                                              char *TableNamePtr,
                                              uint32_t TableNameNumBytes);

/**
 * Gets the element identifier for the named table, if present in the target.
 *
 * @param[in] CtxPtr                Pointer to the SDNet Target instance context.
 *
 * @param[in] TableNamePtr          Pointer to char buffer that holds the table name.
 *
 * @param[out] ElementIdPtr         Pointer to integer that stores the table identifier.
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code
 */
XilSdnetReturnType XilSdnetTargetGetTableElementIdByName(XilSdnetTargetCtx *CtxPtr, char *TableNamePtr, uint32_t *ElementIdPtr);


/****************************************************************************************************************************************************/
/* SECTION: SDNet Instance Management helper function prototypes */
/****************************************************************************************************************************************************/


/**
 * Gets the context structure for the build information driver.
 *
 * If the build information driver is not present, the function fails by returning error code XIL_SDNET_TARGET_ERR_MGMT_DRV_NOT_AVAILABLE.
 *
 * @param[in] CtxPtr                Pointer to the SDNet Target instance context.
 *
 * @param[out] BuildInfoCtxPtrPtr    Pointer to a context structure for the build information driver instance.
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code.
 */
XilSdnetReturnType XilSdnetTargetGetBuildInfoDrv(XilSdnetTargetCtx *CtxPtr, XilSdnetTargetBuildInfoCtx **BuildInfoCtxPtrPtr);

/**
 * Gets the context structure for the management interrupt driver, if present in the target.
 *
 * If the interrupt driver is not present, the function fails by returning error code XIL_SDNET_TARGET_ERR_MGMT_DRV_NOT_AVAILABLE.
 *
 * @param[in] CtxPtr                Pointer to the SDNet Target instance context.
 *
 * @param[out] InterruptCtxPtrPtr   Pointer to a context structure for the management interrupt driver instance.
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code.
 */
XilSdnetReturnType XilSdnetTargetGetInterruptDrv(XilSdnetTargetCtx *CtxPtr, XilSdnetTargetInterruptCtx **InterruptCntlrCtxPtrPtr);

/**
 * Gets the context structure for the management control driver, if present in the target.
 *
 * If the interrupt driver is not present, the function fails by returning error code XIL_SDNET_TARGET_ERR_MGMT_DRV_NOT_AVAILABLE.
 *
 * @param[in] CtxPtr                Pointer to the SDNet Target instance context.
 *
 * @param[out] InterruptCtxPtrPtr   Pointer to a context structure for the management interrupt driver instance.
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code.
 */
XilSdnetReturnType XilSdnetTargetGetCtrlDrv(XilSdnetTargetCtx* CtxPtr, XilSdnetTargetCtrlCtx** CtrlCtxPtrPtr);


/** @} */

#ifdef __cplusplus
}
#endif

#endif

/** @} */
