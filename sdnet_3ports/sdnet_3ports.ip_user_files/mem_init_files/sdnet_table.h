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
-- Revision       : $Revision: #4 $
-- Date           : $DateTime: 2021/01/07 04:30:57 $
-- Last Author    : $Author: fmartin $
--
--------------------------------------------------------------------------------
-- Description : Header file for the SDNet table driver
--
--------------------------------------------------------------------------------
*/

/**
 * Public API of the SDNet table driver.
 *
 * @file sdnet_table.h
 * @addtogroup table SDNet table public API
 * @{
 * @details
 *
 * \section Introduction
 *
 * In SDNet, the term <b>table</b> refers to an entity which implements
 * match-action lookup behaviour.  In brief, this means that it is an entity to
 * which an input (called a <b>key</b>) can be provided, which will result in a
 * lookup operation taking place.  The lookup produces an output (called a
 * <b>response</b>) which is used to trigger a behaviour (called an
 * <b>action</b>).  Part of the response from the lookup operation (called the
 * <b>action ID</b>) is used to select which behaviour to trigger, and the
 * remainder of the response (called the <b>action parameters</b>) is provided
 * as input data to the action that was triggered by the lookup.
 *
 * It is the responsibility of a control plane application to program the
 * contents of such tables at runtime with a set of entries that are chosen to
 * cause desired behaviour of the SDNet design.  The term <b>entry</b> refers to
 * the combination of a key and a response as defined previously.
 *
 * In SDNet, the lookup portion of the match-action lookup behaviour is
 * implemented using with a variety of CAM IP.  There are different versions of
 * this IP depending on the lookup behaviour specified by the user in their P4
 * program.  Each variant has an associated driver - see @ref cam_top for
 * further details.
 *
 * The SDNet table driver provides a set of control plane management functions
 * which can be used by customer applications to configure tables present in a
 * given SDNet design.  The driver offers a level of abstraction above that
 * provided by the drivers in @ref cam_top.  The primary features of the table
 * driver are:
 *  - Common API regardless of the CAM used to implement the table in the SDNet
 *    design
 *  - API allows CAM response data to be defined in terms of action parameters
 *    and action ID
 *  - API provides functions to look up the action ID based on the action's name
 *
 * These features aim to provide an increased ease of use for the development of
 * control plane applications by abstracting away these implementation-specific
 * details.
 *
 * \section overview Overview of functions
 *
 * The table driver provides the following set of functions for managing the
 * content of a table in an SDNet design:
 *  - setup: @ref XilSdnetTableInit()
 *  - insert: @ref XilSdnetTableInsert()
 *  - update: @ref XilSdnetTableUpdate()
 *  - get: @ref XilSdnetTableGetByKey() and @ref XilSdnetTableGetByResponse()
 *  - delete: @ref XilSdnetTableDelete()
 *  - reset: @ref XilSdnetTableReset()
 *  - teardown: @ref XilSdnetTableExit()
 *
 * Several of these functions accept a parameter called ActionId.  The driver
 * provides a utility function named @ref XilSdnetTableGetActionId() which
 * accepts a string containing the name of an action supported by the given
 * table and retrieves the corresponding ID.
 *
 * The table API also provides the following functions which allow querying ECC
 * error information associated with the table's memory:
 *  - @ref XilSdnetTableGetEccCountersClearOnRead()
 *  - @ref XilSdnetTableGetEccAddressesClearOnRead()
 *
 * \section tbl_mode_dep Table API dependency on implementation mode
 *
 * The functionality offered by the table driver is dependent on the underlying
 * CAM IP used to implement the table.  If a function is not supported by the
 * underlying implementation, the function shall return
 * <b>XIL_SDNET_TABLE_ERR_FUNCTION_NOT_SUPPORTED</b>.
 *
 * \section Endianness
 *
 * Several of the functions offered by the table driver accept pointers to byte
 * arrays for specifying the data for keys and action parameters.  The table
 * driver supports two different layouts - these are:
 * - <b>XIL_SDNET_LITTLE_ENDIAN</b>: Least significant byte is in offset 0
 * - <b>XIL_SDNET_BIG_ENDIAN</b>: Most significant byte is in offset 0
 *
 * A detailed discussion of packing data into byte arrays is provided in
 * \ref cam_top
 *
 */

#ifndef SDNET_TABLE_H
#define SDNET_TABLE_H

/****************************************************************************************************************************************************/
/* SECTION: Header includes */
/****************************************************************************************************************************************************/

#include "cam_top.h"
#include "tiny_cam.h"

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************************************************************************************/
/* SECTION: Type definitions */
/****************************************************************************************************************************************************/

typedef enum XilSdnetTableMode
{
    XIL_SDNET_TABLE_MODE_BCAM,
    XIL_SDNET_TABLE_MODE_STCAM,
    XIL_SDNET_TABLE_MODE_TCAM,
    XIL_SDNET_TABLE_MODE_DCAM,
    XIL_SDNET_TABLE_MODE_TINY_BCAM,
    XIL_SDNET_TABLE_MODE_TINY_TCAM,
    XIL_SDNET_NUM_TABLE_MODES
} XilSdnetTableMode;

typedef struct XilSdnetAction
{
    const char          *NameStringPtr;
    uint32_t            ParamListSize;
    XilSdnetAttribute   *ParamListPtr;
} XilSdnetAction;

/** Populated by the user to define the table configuration */
typedef struct XilSdnetTableConfig
{
    XilSdnetEndian      Endian;             /**< Format of key, mask and action parameter byte arrays */
    XilSdnetTableMode   Mode;               /**< Specfies which variety of CAM implements this table */
    uint32_t            KeySizeBits;        /**< Key size in bits */
    XilSdnetCamConfig   CamConfig;          /**< Configuration structure for the CAM that implements this table */
    uint32_t            ActionIdWidthBits;  /**< Width of the action ID in bits */
    uint32_t            ActionListSize;     /**< Number of actions present in this table */
    XilSdnetAction      **ActionListPtr;    /**< Structure describing the actions present in the design */
} XilSdnetTableConfig;

/** Forward declaration to support context structure declaration */
typedef struct XilSdnetTablePrivateCtx XilSdnetTablePrivateCtx;

/** Holds context information needed by the table driver's API */
typedef struct XilSdnetTableCtx
{
    XilSdnetTablePrivateCtx *PrivateCtxPtr; /**< Internal context data used by driver implementation */
} XilSdnetTableCtx;

/****************************************************************************************************************************************************/
/* SECTION: Table function declarations */
/****************************************************************************************************************************************************/

/**
 * Initialization function for the table driver API
 *
 * @param[in,out] CtxPtr        Pointer to an uninitialized table context structure instance.
 *                              Is populated by function execution.
 *
 * @param[in] EnvIfPtr          Pointer to environment interface definition
 *
 * @param[in] ConfigPtr         Pointer to driver configuration definition
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code.
 */

XilSdnetReturnType XilSdnetTableInit(XilSdnetTableCtx *CtxPtr, XilSdnetEnvIf *EnvIfPtr, XilSdnetTableConfig *ConfigPtr);

/**
 * Gets the implementation mode of the table.
 *
 * @param[in] CtxPtr    Pointer to the table driver instance context
 *
 * @param[out] ModePtr  Pointer to the implementation mode of table.
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code.
 */

XilSdnetReturnType XilSdnetTableGetMode(XilSdnetTableCtx *CtxPtr, XilSdnetTableMode *ModePtr);

/**
 * Get the number of actions present in a table.
 *
 * @param[in] CtxPtr            Pointer to the table driver instance context
 *
 * @param[out] NumActionsPtr    Pointer to integer holding the number of actions present in the table.
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code.
 */

XilSdnetReturnType XilSdnetTableGetNumActions(XilSdnetTableCtx *CtxPtr, uint32_t *NumActionsPtr);

/**
 * Get the size of the key for this table.
 *
 * @param[in] CtxPtr            Pointer to the table driver instance context
 *
 * @param [out] KeySizeBitsPtr  Pointer to integer holding the size of the key measured in bits.
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code.
 */

XilSdnetReturnType XilSdnetTableGetKeySizeBits(XilSdnetTableCtx *CtxPtr, uint32_t *KeySizeBitsPtr);

/**
 * Get the size of the action parameters for this table.
 *
 * @param[in] CtxPtr                    Pointer to the table driver instance context
 *
 * @param [out] ActionParamSizeBitsPtr  Pointer to integer holding the size of the packed action parameters measured in bits.
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code.
 */

XilSdnetReturnType XilSdnetTableGetActionParamsSizeBits(XilSdnetTableCtx *CtxPtr, uint32_t *ActionParamsSizeBitsPtr);

/**
 * Get the action identifer width in bits for this table.
 *
 * @param[in] CtxPtr                    Pointer to the table driver instance context
 *
 * @param [out] ActionIdWidthBitsPtr    Pointer to integer holding the size of the action id width measured in bits.
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code.
 */

XilSdnetReturnType XilSdnetTableGetActionIdWidthBits(XilSdnetTableCtx *CtxPtr, uint32_t *ActionIdWidthBitsPtr);

/**
 * Gets an action identifier for an action in a table.
 *
 * @param[in] CtxPtr            Pointer to the table driver instance context
 *
 * @param[in] ActionNamePtr     Pointer to string that holds the action name.
 *
 * @param[out] ActionIdPtr      Pointer to integer holding the action identifier.
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code.
 */

XilSdnetReturnType XilSdnetTableGetActionId(XilSdnetTableCtx *CtxPtr, char *ActionNamePtr, uint32_t *ActionIdPtr);

/**
 * Gets the name of an action for a given identifier if present in the table.
 *
 * @param[in] CtxPtr                Pointer to the table driver instance context
 *
 * @param[in] ActionId              The action identifier.
 *
 * @param[out] ActionNamePtr        Pointer to char buffer to hold the action name.
 *
 * @param[in] ActionNameNumBytes    The number of characters the action name buffer can hold including the null terminating character.
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code.
 */

XilSdnetReturnType XilSdnetTableGetActionNameById(XilSdnetTableCtx *CtxPtr,
                                                  uint32_t ActionId,
                                                  char *ActionNamePtr,
                                                  uint32_t ActionNameNumBytes);

/**
 * Inserts an entry into the table instance.
 *
 * If an existing matching entry is found, the function fails by returning error
 * code XIL_SDNET_CAM_ERR_DUPLICATE_FOUND. If an entry is not found, then it is
 * inserted in the table instance.
 *
 * When the table has an implementation mode of DCAM or BCAM the following parameters are not used:
 *   - /p MaskPtr, which must be set to NULL
 *   - /p Priority, which is ignored by the function
 * see @ref tbl_mode_dep
 *
 * @param[in] CtxPtr            Pointer to the table driver instance context
 *
 * @param[in] KeyPtr            Pointer to byte array holding the key for the entry.
 *
 * @param[in] MaskPtr           Pointer to byte array holding the mask for the entry.
 *
 * @param[in] Priority          The priority of the entry. If multiple matches occur, the winning matching
 *                              entry is determined based on the priority value. The entry with the lowest priority
 *                              wins. If there are multiple matches with the same lowest priority, any of them becomes
 *                              the winner.
 *
 * @param[in] ActionId          A action identifier specifies the action executed when a hit occurs for this entry.
 *
 * @param[in] ActionParamsPtr   Pointer to byte array holding the action parameters of the entry.
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code.
 */

XilSdnetReturnType XilSdnetTableInsert(XilSdnetTableCtx *CtxPtr,
                                       uint8_t *KeyPtr,
                                       uint8_t *MaskPtr,
                                       uint32_t Priority,
                                       uint32_t ActionId,
                                       uint8_t *ActionParamsPtr);

/**
 * Updates an entry in the table instance.
 *
 * If an existing matching entry is found, the response is updated. If an entry
 * is not found, the function fails with error code
 * XIL_SDNET_CAM_ERR_KEY_NOT_FOUND.
 *
 * * When the table has an implementation mode of DCAM or BCAM the following parameters are not used:
 *   - @p MaskPtr, which must be set to NULL
 * see @ref tbl_mode_dep
 *
 * @param[in] CtxPtr            Pointer to the table driver instance context
 *
 * @param[in] KeyPtr            Pointer to byte array holding the key for the entry.
 *
 * @param[in] MaskPtr           Pointer to byte array holding the mask for the entry.
 *
 * @param[in] ActionId          A action identifier specifies the action executed when a hit occurs for this entry.
 *
 * @param[in] ActionParamsPtr   Pointer to byte array holding the action parameters of the entry.
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code.
 */

XilSdnetReturnType XilSdnetTableUpdate(XilSdnetTableCtx *CtxPtr,
                                       uint8_t *KeyPtr,
                                       uint8_t *MaskPtr,
                                       uint32_t ActionId,
                                       uint8_t *ActionParamsPtr);

/**
 * Gets an entry from the table instance with the specified response.
 *
 * If a matching entry is not found, the function fails with error code XIL_SDNET_CAM_ERR_KEY_NOT_FOUND.
 *
 * When the table has an implementation mode of DCAM or BCAM the following parameters are not used:
 *   - @p MaskPtr, which must be set to NULL
 * see @ref tbl_mode_dep
 *
 * @param[in] CtxPtr            Pointer to the table driver instance context
 *
 * @param[in] ActionId          An action identifier specifies the action executed when a hit occurs for this entry.
 *
 * @param[in] ActionParamsPtr   Pointer to byte array holding the action parameteres to search for.
 *
 * @param[in] ActionParamsMaskPtr   The buffers pointed to by ActionParamsPtr and ActionParamsMaskPtr are ANDed and then
 *                                  searched for.
 *
 * @param[in,out] PositionPtr   Position in the database. Used for consecutive get operations, set to 0 for
 *                              first get.
 *
 * @param[out] KeyPtr           Pointer to byte array with storage for the key that is read.
 *
 * @param[out] MaskPtr          Pointer to byte array with storage for the mask that is read.
 *
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code.
 *
 */

XilSdnetReturnType XilSdnetTableGetByResponse(XilSdnetTableCtx *CtxPtr,
                                              uint32_t ActionId,
                                              uint8_t *ActionParamsPtr,
                                              uint8_t *ActionParamsMaskPtr,
                                              uint32_t *PositionPtr,
                                              uint8_t *KeyPtr,
                                              uint8_t *MaskPtr);

/**
 * Gets an entry from the table instance with the specified key.
 *
 * If an entry is not found, the function fails with error code XIL_SDNET_CAM_ERR_KEY_NOT_FOUND.
 * The function does not read from the hardware, it reads from the shadow copy in system memory.
 * The function uses a hash table and is fast.
 *
 * When the table has an implementation mode of DCAM this function is not supported.
 * When the table has an implementation mode of BCAM the following parameters are not used:
 *   - @p MaskPtr, which must be set to NULL
 *   - @p PriorityPtr, which must be set to NULL
 * see @ref tbl_mode_dep
 *
 * @param[in] CtxPtr            Pointer to the table driver instance context
 *
 * @param[in] KeyPtr            Pointer to byte array holding the key to search for.
 *
 * @param[in] MaskPtr           Pointer to byte array holding the mask to search for.
 *
 * @param[out] PriorityPtr      Pointer to integer with storage for the priority of the entry that is read.
 *
 * @param[out] ActionIdPtr      Pointer to the action identifier assoicated with this table entry.
 *
 * @param[out] ActionParamsPtr      Pointer to byte array with storage for the action parameters that are read.
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code.
 */

XilSdnetReturnType XilSdnetTableGetByKey(XilSdnetTableCtx *CtxPtr,
                                         uint8_t *KeyPtr,
                                         uint8_t *MaskPtr,
                                         uint32_t *PriorityPtr,
                                         uint32_t *ActionIdPtr,
                                         uint8_t *ActionParamsPtr);

/**
 * Looks up an entry in the table instance.
 *
 * This function provides the same response as if a lookup had been performed in hardware.
 * If no match is found in the database, the response value associated with the catch-all entry
 * is returned.
 *
 * If an entry is not found, the function returns XIL_SDNET_CAM_ERR_KEY_NOT_FOUND
 *
 *
 * @param[in] CtxPtr            Pointer to the table driver instance context
 *
 * @param[in] KeyPtr            Pointer to byte array holding the key to lookup.
 *
 * @param[out] ActionIdPtr      Pointer to the action identifier assoicated with this table entry.
 *
 * @param[out] ActionParamsPtr  Pointer to byte array with storage for the action parameters that are read.
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code.
 */

XilSdnetReturnType XilSdnetTableLookup(XilSdnetTableCtx *CtxPtr,
                                       uint8_t *KeyPtr,
                                       uint32_t *ActionIdPtr,
                                       uint8_t *ActionParamsPtr);

/**
 * Delete an entry with the specified key from the table instance.
 *
 * When the table has an implementation mode of DCAM orBCAM the following parameters are not used:
 *   - @p MaskPtr, which must be set to NULL
 * see @ref tbl_mode_dep
 *
 * If an entry is not found, the function fails with error code XIL_SDNET_CAM_ERR_KEY_NOT_FOUND.
 *
 * @param[in] CtxPtr            Pointer to the table driver instance context
 *
 * @param[in] KeyPtr            Pointer to byte array holding the key to be deleted.
 *
 * @param[in] MaskPtr           Pointer to byte array holding the mask to be deleted.
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code.
 */

XilSdnetReturnType XilSdnetTableDelete(XilSdnetTableCtx *CtxPtr, uint8_t *KeyPtr, uint8_t *MaskPtr);

/**
 * Reads and clears the ECC counters of the table instance.
 *
 * When the table has an implementation mode of DCAM this function is not supported.
 * see @ref tbl_mode_dep
 *
 * @param[in] CtxPtr            Pointer to the table driver instance context
 *
 * @param[out] CorrectedSingleBitErrorsPtr  The number of single bit errors the hardware scrubbing
 *                                          process has detected and corrected. Errors might
 *                                          have been detected by a hardware lookup and then corrected
 *                                          dynamically for the lookups, but still not counted.
 *                                          The scrubbing process runs in the background and corrects
 *                                          errors permanently. The counter is only incremented after
 *                                          the error is corrected permanently.
 *
 * @param[out] DetectedDoubleBitErrorsPtr   The number of detected double-bit errors. Double-bit
 *                                          errors are fatal errors and the data base is corrupt. The hardware
 *                                          cannot correct the error without help from software. The instance
 *                                          has generated an interrupt and this counter can be used to find
 *                                          the instance which caused the interrupt if multiple instances are
 *                                          in use. Lookups detecting double-bit errors will not discard the lookup,
 *                                          the lookup result might be corrupt. Once the double-bit error is
 *                                          detected the interrupt pin is asserted and it is advised to use this
 *                                          signal to discard packets until the data base is corrected.
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code.
 */

XilSdnetReturnType XilSdnetTableGetEccCountersClearOnRead(XilSdnetTableCtx *CtxPtr,
                                                          uint32_t *CorrectedSingleBitErrorsPtr,
                                                          uint32_t *DetectedDoubleBitErrorsPtr);

/**
 * Reads and clears the ECC adress registers of the table instance.
 *
 * When the table has an implementation mode of DCAM this function is not supported.
 * see @ref tbl_mode_dep
 *
 * @param[in] CtxPtr                            Pointer to the table driver instance context
 *
 * @param[out] FailingAddressSingleBitErrorPtr  The address of the first single bit error detected and corrected
 *                                              by the hardware scrubbing process. Additional errors might
 *                                              have been detected during a hardware lookup and then corrected
 *                                              dynamically, but this register only reflect the
 *                                              errrors detected by hardware scrubbing.
 *
 * @param[out] FailingAddressDoubleBitErrorPtr The address of the first double-bit error detected by the
 *                                              hardware scrubbing process.
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code.
 */

XilSdnetReturnType XilSdnetTableGetEccAddressesClearOnRead(XilSdnetTableCtx *CtxPtr,
                                                           uint32_t *FailingAddressSingleBitErrorPtr,
                                                           uint32_t *FailingAddressDoubleBitErrorPtr);

/**
 * Reset a table.
 *
 * @param[in] CtxPtr Pointer to the table driver instance context.
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code.
 */
XilSdnetReturnType XilSdnetTableReset(XilSdnetTableCtx *CtxPtr);


/**
 * Destroy the table driver instance
 *
 * All memory allocated by \ref XilSdnetTableInit is released.
 *
 * @param[in] CtxPtr    Pointer to the table driver instance context
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code.
 */
XilSdnetReturnType XilSdnetTableExit(XilSdnetTableCtx *CtxPtr);

#ifdef __cplusplus
}
#endif

#endif

/** @} */
