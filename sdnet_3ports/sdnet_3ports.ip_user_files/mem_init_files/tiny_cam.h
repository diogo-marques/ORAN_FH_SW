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
-- Revision       : $Revision: #2 $
-- Date           : $DateTime: 2021/01/27 13:43:20 $
-- Last Author    : $Author: fmartin $
--
--------------------------------------------------------------------------------
-- Description : Header file for top level of SDNet CAM driver
--
--------------------------------------------------------------------------------
*/
/**
 * Public API of the CAM drivers
 *
 * @file tiny_cam.h
 * @addtogroup cam_top SDNet CAM public API
 * @{
 *
 *
 *
 * @addtogroup tiny_cam Tiny CAM Public API
 * @{
 *
 * Please refer to the following sections to understand key and response data layout:
 *  - \ref fmt
 *  - \ref endian
 *  - \ref Priority
 * \section Priority
 * The Priority specifies the entry number of a particular key-mask-response entry
 * Its value will vary between 0 and the maximum number of entries minus one
 *
 */
#ifndef TINY_CAM_H
#define TINY_CAM_H

/****************************************************************************************************************************************************/
/* SECTION: Header includes */
/****************************************************************************************************************************************************/

#include "cam_top.h"
#include "sdnet_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************************************************************************************/
/* SECTION: Constants */
/****************************************************************************************************************************************************/

/****************************************************************************************************************************************************/
/* SECTION: Type definitions */
/****************************************************************************************************************************************************/
typedef enum XilSdnetTinyCamEmulationMode
{
    XIL_SDNET_TINY_CAM_BCAM_EMULATION,
    XIL_SDNET_TINY_CAM_TCAM_EMULATION,
    XIL_SDNET_TINY_CAM_NUM_EMULATION_MODES
} XilSdnetTinyCamEmulationMode;


/** Forward declaration to support Tiny Cam private context structure declaration */
typedef struct XilSdnetTinyCamPrvCtx XilSdnetTinyCamPrvCtx;

typedef struct XilSdnetTinyCamCtx
{
    XilSdnetTinyCamPrvCtx* PrivateCtxPtr;
} XilSdnetTinyCamCtx;


typedef struct XilSdnetTinyTcamCtx
{
    XilSdnetTinyCamPrvCtx* PrivateCtxPtr;
} XilSdnetTinyTcamCtx;

typedef struct XilSdnetTinyBcamCtx
{
    XilSdnetTinyCamPrvCtx* PrivateCtxPtr;
} XilSdnetTinyBcamCtx;


/** Union of all low level CAM driver contexts
 *
 * This is used by a high level drivers for different platforms, e.g. SDNet Table and the SmartNIC Plugin high level drivers.
 * TBD should move a common high level driver
 */
typedef union XilSdnetCamDriverUnion
{
    XilSdnetDcamCtx DcamCtx;
    XilSdnetBcamCtx BcamCtx;
    XilSdnetTcamCtx TcamCtx;
    XilSdnetStcamCtx StcamCtx;
    XilSdnetTinyTcamCtx TinyTcamCtx;
    XilSdnetTinyBcamCtx TinyBcamCtx;
} XilSdnetCamDriverUnion;

/****************************************************************************************************************************************************/
/* SECTION: Tiny Cam for BCAM Emulation */
/****************************************************************************************************************************************************/

/**
 * Initialization function for the Tiny BCAM driver API
 *
 * @param[in,out] CtxPtr        Pointer to an uninitialized Tiny CAM context structure instance.
 *                              Is populated by function execution.
 *
 * @param[in] EnvIfPtr          Pointer to environment interface definition
 *
 * @param[in] ConfigPtr         Pointer to driver configuration definition
 *
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code.
 */
XilSdnetReturnType XilSdnetTinyBcamInit(XilSdnetTinyBcamCtx *CtxPtr, XilSdnetEnvIf *EnvIfPtr, XilSdnetCamConfig *ConfigPtr);

/**
 * Destroy the Tiny CAM instance
 *
 * All memory allocated by \ref XilSdnetTinyBcamInit is released.
 *
 * @param[in] CtxPtr    Pointer to the Tiny BCAM instance context
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code.
 */
XilSdnetReturnType XilSdnetTinyBcamExit(XilSdnetTinyBcamCtx *CtxPtr);

/**
 * Inserts an entry into the Tiny CAM instance.for BCAM Emulation
 *
 * If an existing matching entry is found, the function fails by returning error
 * code XIL_SDNET_CAM_ERR_DUPLICATE_FOUND. If an existing matching entry is not
 * found, then it is inserted in the Tiny CAM instance.
 *
 * @param[in] CtxPtr        Pointer to the Tiny BCAM instance context
 *
 * @param[in] KeyPtr        Pointer to byte array holding the key for the entry
 *
 * @param[in] ResponsePtr   Pointer to byte array holding the response of the entry
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code.
 */

XilSdnetReturnType XilSdnetTinyBcamInsert(XilSdnetTinyBcamCtx *CtxPtr, uint8_t *KeyPtr, uint8_t *ResponsePtr);

/**
 * Updates an entry in the Tiny CAM instance.for BCAM Emulation
 *
 * If the entry is found, the response is updated. If the entry is not found,
 * the function fails with error code XIL_SDNET_CAM_ERR_KEY_NOT_FOUND.
 *
 * @param[in] CtxPtr        Pointer to the Tiny BCAM instance context
 *
 * @param[in] KeyPtr        Pointer to byte array holding the key for the entry
 *
 * @param[in] ResponsePtr   Pointer to byte array holding the response of the entry
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code.
 */

XilSdnetReturnType XilSdnetTinyBcamUpdate(XilSdnetTinyBcamCtx *CtxPtr, uint8_t *KeyPtr, uint8_t *ResponsePtr);

/**
 * Gets an entry from the Tiny Cam instance in BCAM Emulation mode with the matching response value.
 *
 * If the entry is not found, the function fails with error code XIL_SDNET_CAM_ERR_KEY_NOT_FOUND.
 *
 * The PositionPtr argument enables multiple matching entries to be found. The first time the function is
 * called the integer at PositionPtr should be set to zero. To find all entries matching the response, the function is
 * called multiple times, each call returns one matching entry.
 *
 * For every consecutive call the returned PositionPtr value must be used again as input argument.
 * When XIL_SDNET_CAM_ERR_KEY_NOT_FOUND is returned no more entries matching the response could be found.
 *
 * The ResponseMaskPtr is used to limit the search to subfields of the response. If the complete,
 * exact response value is searched, then all bits in the byte array at ResponseMaskPtr should be set to one.
 * By setting the bits to all zeros, any response value will match.
 * This can be used for getting all entries without prior knowledge of the response values.
 *
 * The function does not read from the hardware, it reads from the shadow copy in system memory.
 * The function uses a linear search.
 *
 * @param[in] CtxPtr            Pointer to the Tiny BCAM instance context
 *
 * @param[in] ResponsePtr       Pointer to byte array holding the response to search for
 *
 * @param[in] ResponseMaskPtr   The buffers pointed to by ResponsePtr and ResponseMaskPtr are ANDed and then
 *                              searched for. The stored response is also ANDed
 *                              with the ResponseMaskPtr before compare takes place.
 *
 * @param[in,out] PositionPtr   Position in the database. Used for
 *                              consecutive get operations, set to 0 for
 *                              first get.
 *
 * @param[out] KeyPtr           Pointer to byte array with storage for the key that is read
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code.
 */

XilSdnetReturnType XilSdnetTinyBcamGetByResponse(XilSdnetTinyBcamCtx *CtxPtr,
                                                 uint8_t *ResponsePtr,
                                                 uint8_t *ResponseMaskPtr,
                                                 uint32_t *PositionPtr,
                                                 uint8_t *KeyPtr);

/**
 * Gets an entry from the Tiny Cam instance in BCAM Emulation mode with the specified key

 * If an entry is not found, the function fails with error code XIL_SDNET_CAM_ERR_KEY_NOT_FOUND.
 * The function does not read from the hardware, it reads from the shadow copy in system memory.
 * The function uses a hash table and is fast.
 *
 * @param[in] CtxPtr        Pointer to the Tiny BCAM instance context
 *
 * @param[in] KeyPtr        Pointer to byte array holding the key to search for
 *
 * @param[out] ResponsePtr  Pointer to byte array with storage for the response that is read
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code.
 */

XilSdnetReturnType XilSdnetTinyBcamGetByKey(XilSdnetTinyBcamCtx *CtxPtr, uint8_t *KeyPtr, uint8_t *ResponsePtr);

/**
 * Delete an entry with the specified key from the Tiny Cam instance in BCAM Emulation mode.
 *
 * If an entry is not found, the function fails with error code XIL_SDNET_CAM_ERR_KEY_NOT_FOUND.
 *
 * @param[in] CtxPtr    Pointer to the Tiny BCAM instance context
 *
 * @param[in] KeyPtr    Pointer to byte array holding the key to be deleted
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code.
 */

XilSdnetReturnType XilSdnetTinyBcamDelete(XilSdnetTinyBcamCtx *CtxPtr, uint8_t *KeyPtr);

/**
 * Resets a Tiny BCAM instance.
 *
 * All existing entries in the table (if any) are deleted
 *
 * @param[in]  CtxPtr           Pointer to the Tiny CAM instance context
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code.
 */
XilSdnetReturnType XilSdnetTinyBcamReset(XilSdnetTinyBcamCtx *CtxPtr);

/**
 * Currently not supported
 *
 * @param[in] CtxPtr        Pointer to the Tiny BCAM instance context
 *
 * @return XIL_SDNET_TABLE_ERR_FUNCTION_NOT_SUPPORTED
 */
XilSdnetReturnType XilSdnetTinyBcamGetEccCountersClearOnRead(XilSdnetTinyBcamCtx *CtxPtr,
                                                            uint32_t *CorrectedSingleBitErrorsPtr,
                                                            uint32_t *DetectedDoubleBitErrorsPtr);

/**
 * Currently not supported
 *
 * @param[in] CtxPtr        Pointer to the Tiny BCAM instance context
 *
 * @return XIL_SDNET_TABLE_ERR_FUNCTION_NOT_SUPPORTED
 */
XilSdnetReturnType XilSdnetTinyBcamGetEccAddressesClearOnRead(XilSdnetTinyBcamCtx *CtxPtr,
                                                             uint32_t *FailingAddressSingleBitErrorPtr,
                                                             uint32_t *FailingAddressDoubleBitErrorPtr);
/**
 * Currently not supported
 *
 * @param[in] CtxPtr        Pointer to the Tiny BCAM instance context
 *
 * @return XIL_SDNET_TABLE_ERR_FUNCTION_NOT_SUPPORTED
 */
XilSdnetReturnType XilSdnetTinyBcamRewrite(XilSdnetTinyBcamCtx *CtxPtr);


/****************************************************************************************************************************************************/
/* SECTION: Tiny Cam for TCAM Emulation*/
/****************************************************************************************************************************************************/

/**
 * Initialization function for the Tiny Tcam driver API
 *
 * @param[in,out] CtxPtr        Pointer to an uninitialized Tiny TCAM context structure instance.
 *                              Is populated by function execution.
 *
 * @param[in] EnvIfPtr          Pointer to environment interface definition
 *
 * @param[in] ConfigPtr         Pointer to driver configuration definition
 *
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code.
 */
XilSdnetReturnType XilSdnetTinyTcamInit(XilSdnetTinyTcamCtx *CtxPtr, XilSdnetEnvIf *EnvIfPtr, XilSdnetCamConfig *ConfigPtr);


/**
 * Destroy the Tiny TCAM instance
 *
 * All memory allocated by \ref XilSdnetTinyTcamInit is released.
 *
 * @param[in] CtxPtr    Pointer to the TinyTcam instance context
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code.
 */
XilSdnetReturnType XilSdnetTinyTcamExit(XilSdnetTinyTcamCtx *CtxPtr);


/**
 * Inserts an entry into the Tiny CAM instance in TCAM Emulation mode.
 *
 * If a entry already exists at the entry location specified by priority, then the function fails by returning error
 * code XIL_SDNET_CAM_ENTRY_OCCUPIED.
 *
 * @param[in] CtxPtr        Pointer to the Tiny TCAM instance context
 *
 * @param[in] KeyPtr        Pointer to byte array holding the Key
 *
 * @param[in] MaskPtr       Pointer to byte array holding the Mask
 *
 * @param[in] Priority      Specifies priority number of the entry
 *
 * @param[in] ResponsePtr   Pointer to byte array holding the Response
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code.
 */
XilSdnetReturnType XilSdnetTinyTcamInsert(XilSdnetTinyTcamCtx *CtxPtr, uint8_t *KeyPtr, uint8_t *MaskPtr, uint32_t Priority, uint8_t *ResponsePtr);

/**
 * Updates an entry in the Tiny CAM instance in TCAM Emulation mode.
 *
 * If the entry is found, the response is updated. If the entry is not found,
 * the function fails with error code XIL_SDNET_CAM_ERR_KEY_NOT_FOUND.
 *
 * @param[in] CtxPtr        Pointer to the Tiny TCAM instance context
 *
 * @param[in] KeyPtr        Pointer to byte array holding the Key
 *
 * @param[in] MaskPtr       Pointer to byte array holding the Mask
 *
 * @param[in] ResponsePtr   Pointer to byte array holding the Response
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code.
 */
XilSdnetReturnType XilSdnetTinyTcamUpdate(XilSdnetTinyTcamCtx *CtxPtr, uint8_t *KeyPtr, uint8_t *MaskPtr, uint8_t *ResponsePtr);

/**
 * Gets an entry from the Tiny CAM instance in TCAM Emulation mode with the matching response value.
 *
 * If the entry is not found, the function fails with error code XIL_SDNET_CAM_ERR_KEY_NOT_FOUND.
 *
 * The PositionPtr argument enables multiple matching entries to be found. The first time the function is
 * called the integer at PositionPtr should be set to zero. To find all entries matching the response, the function is
 * called multiple times, each call returns one matching entry.
 *
 * For every consecutive call the returned PositionPtr value must be used again as input argument.
 * When XIL_SDNET_CAM_ERR_KEY_NOT_FOUND is returned no more entries matching the response could be found.
 *
 * The ResponseMaskPtr is used to limit the search to subfields of the response. If the complete,
 * exact response value is searched, then all bits in the byte array at ResponseMaskPtr should be set to one.
 * By setting the bits to all zeros, any response value will match.
 * This can be used for getting all entries without prior knowledge of the response values.
 *
 * The function uses a linear search to read the entries from hardware.
 *
 * @param[in] CtxPtr            Pointer to the Tiny TCAM instance context
 *
 * @param[in] ResponsePtr       Pointer to byte array holding the response to search for
 *
 * @param[in] ResponseMaskPtr   The buffers pointed to by ResponsePtr and ResponseMaskPtr are ANDed and then
 *                              searched for. The stored response is also ANDed
 *                              with the ResponseMaskPtr before the comparison takes place.
 *
 * @param[in,out] PositionPtr   Position in the database. Used for
 *                              consecutive get operations, set to 0 for
 *                              first get.
 *
 * @param[out] KeyPtr           Pointer to byte array which is updated with the Key entry when response matches are found
 *
 * @param[out] MaskPtr          Pointer to byte array which is updated with the Mask entry when response matches are found
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code.
 */
XilSdnetReturnType XilSdnetTinyTcamGetByResponse(XilSdnetTinyTcamCtx *CtxPtr,
                                                uint8_t *ResponsePtr,
                                                uint8_t *ResponseMaskPtr,
                                                uint32_t *PositionPtr,
                                                uint8_t *KeyPtr,
                                                uint8_t *MaskPtr);
/**
 * Gets an response entry and priority location from the Tiny CAM instance in TCAM Emulation mode with the specified key mask pair.

 * If an entry is not found, the function fails with error code XIL_SDNET_CAM_ERR_KEY_NOT_FOUND.
 *
 * @param[in] CtxPtr        Pointer to the Tiny TCAM instance context
 *
 * @param[in] KeyPtr        Pointer to byte array holding the key to search for
 *
 * @param[in] MaskPtr       Pointer to byte array holding the Mask to search for
 *
 * @param[in,out] PriorityPtr   Position in the database. Used for
 *                              consecutive get operations, set to 0 for
 *                              first get. This is updated with position of any matching key
 *
 * @param[out] ResponsePtr  Poi6nter to byte array with storage for the response that is read
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code.
 */
XilSdnetReturnType XilSdnetTinyTcamGetByKey(XilSdnetTinyTcamCtx *CtxPtr,
                                           uint8_t *KeyPtr,
                                           uint8_t *MaskPtr,
                                           uint32_t *PriorityPtr,
                                           uint8_t *ResponsePtr);

/**
 * Parses the Tiny CAM in TCAM Emulation mode entry database until a matching key-mask pair is found. It then deletes this entry.
 * If a match is not found, then XIL_SDNET_CAM_ERR_KEY_NOT_FOUND is returned.
 *
 * @param[in] CtxPtr        Pointer to the Tiny TCAM instance context
 *
 * @param[in] KeyPtr        Pointer to byte array holding the key to search for
 *
 * @param[in] MaskPtr       Pointer to byte array holding the Mask to search for
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code.
 */
XilSdnetReturnType XilSdnetTinyTcamDelete(XilSdnetTinyTcamCtx *CtxPtr, uint8_t *KeyPtr, uint8_t *MaskPtr);

/**
 * Resets a Tiny TCAM instance.
 *
 * All existing entries in the table (if any) are deleted
 *
 * @param[in]  CtxPtr           Pointer to the Tiny CAM instance context
 *
 * @return XIL_SDNET_SUCCESS if successful, otherwise an error code.
 */
XilSdnetReturnType XilSdnetTinyTcamReset(XilSdnetTinyTcamCtx *CtxPtr);

/**
 * Currently not supported
 *
 * @param[in] CtxPtr        Pointer to the Tiny TCAM instance context
 *
 * @return XIL_SDNET_TABLE_ERR_FUNCTION_NOT_SUPPORTED
 */
XilSdnetReturnType XilSdnetTinyTcamGetEccCountersClearOnRead(XilSdnetTinyTcamCtx *CtxPtr,
                                                            uint32_t *CorrectedSingleBitErrorsPtr,
                                                            uint32_t *DetectedDoubleBitErrorsPtr);

/**
 * Currently not supported
 *
 * @param[in] CtxPtr        Pointer to the Tiny TCAM instance context
 *
 * @return XIL_SDNET_TABLE_ERR_FUNCTION_NOT_SUPPORTED
 */
XilSdnetReturnType XilSdnetTinyTcamGetEccAddressesClearOnRead(XilSdnetTinyTcamCtx *CtxPtr,
                                                             uint32_t *FailingAddressSingleBitErrorPtr,
                                                             uint32_t *FailingAddressDoubleBitErrorPtr);
/**
 * Currently not supported
 *
 * @param[in] CtxPtr        Pointer to the Tiny TCAM instance context
 *
 * @return XIL_SDNET_TABLE_ERR_FUNCTION_NOT_SUPPORTED
 */
XilSdnetReturnType XilSdnetTinyTcamRewrite(XilSdnetTinyTcamCtx *CtxPtr);


#ifdef __cplusplus
}
#endif

#endif
/** @} */
