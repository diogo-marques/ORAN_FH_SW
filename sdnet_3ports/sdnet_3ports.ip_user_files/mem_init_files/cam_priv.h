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
-- Revision       : $Revision: #1 $
-- Date           : $DateTime: 2021/01/07 04:30:57 $
-- Last Author    : $Author: fmartin $
--
--------------------------------------------------------------------------------
-- Description : private header file to store cam top common declarations
--
--------------------------------------------------------------------------------
*/

#ifndef CAM_PRIV_H
#define CAM_PRIV_H
/****************************************************************************************************************************************************/
/* SECTION: Header includes */
/****************************************************************************************************************************************************/
#include "cam.h"
#include "cam_top.h"
#include "sdnet_common.h"
#include "tiny_cam.h"

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************************************************************************************/
/* SECTION: Constants */
/****************************************************************************************************************************************************/

/****************************************************************************************************************************************************/
/* SECTION: Type definitions */
/****************************************************************************************************************************************************/
typedef enum XilSdnetSwapCmd
{
    SDNET_CAM_SWAP_KEY,
    SDNET_CAM_SWAP_KEY_MASK,
    SDNET_CAM_SWAP_RESPONSE,
    SDNET_CAM_SWAP_RESPONSE_MASK
} XilSdnetSwapCmd;

typedef union XilSdnetCamUnion
{
    struct bcam *BcamPtr;
    struct tcam *TcamPtr;
    struct stcam *StcamPtr;
} XilSdnetCamUnion;

struct XilSdnetCamCtx
{
    XilSdnetEnvIf EnvIf;
    XilSdnetAddressType BaseAddr;
    XilSdnetEndian Endian;
    uint32_t NumEntries; /* Number of entries that are in the CAM */

    uint32_t ResponseSizeBits;
    uint32_t ResponsePaddingSizeBits; /* number of extra bits beyond 8 bit boundary in response*/
    uint32_t ResponseBufSizeBytes;
    uint8_t ResponsePaddingMask; /* Mask for the last response byte. Bits that are allowed are 'hi' */

    uint32_t KeySizeBits;
    uint32_t KeyPaddingSizeBits; /* number of extra bits beyond 8 bit boundary in response*/
    uint32_t KeyBufSizeBytes;
    uint8_t KeyPaddingMask; /* Mask for the last key byte. Bits that are allowed are 'hi' */

    uint8_t *KeyBufPtr;
    uint8_t *MaskBufPtr;
    uint8_t *ResponseBufPtr;
    uint8_t *ResponseMaskBufPtr;

    XilSdnetCamUnion CamUnion;
    cam_arg_t *CamArgPtr;
};

typedef struct XilSdnetTinyCamTableStats
{
    uint32_t TotalSizeBits;   /* Total table size in bits. This includes all padding i.e.*/
    uint32_t EntrySizeBytes;  /* Size (in bytes) of a single table entry with key-mask-response data. This does not include extra padding bytes for
                                 32-bit alignment */
    uint32_t EntrySizeWord32; /* Size (in 32-bit words) of a single table entry. This will include padding so that each table entry ends on a 32-bit
                                 boundary */
    uint32_t NumPaddingBytes; /* Number of padding bytes added to the end of the buffer to ensure that the table entry is boundary aligned to a 32-bit register */
    /* For Bcam Mode emulations to ensure used to reduce number register writes to a minimum */
    uint32_t NumKeyRegisters;
    uint32_t NumResponseRegisters;
} XilSdndetTinyCamTableStats;

/* Manages single entry in Entry Array*/
typedef struct XilSdnetTinyCamEntry
{
    bool InUse;
    uint32_t Priority;
    uint8_t EntryId;
    uint8_t *KeyPtr;
    uint8_t *MaskPtr;
    uint8_t *ResponsePtr;
    struct XilSdnetTinyCamEntry *NextEntryPtr;
    struct XilSdnetTinyCamEntry *PreviousEntryPtr;
} XilSdnetTinyCamEntry;

typedef struct XilSdnetTinyCamShadowTable
{
    uint32_t NumEntriesMax;
    uint32_t NumEntriesInUse;
    XilSdnetTinyCamEntry *EntryListHeadPtr; /* Track as the head will move when re-ordering the list */
    uint8_t *EntryListMemPtr;
    uint8_t *EntryArrayMemPtr;
} XilSdnetTinyCamShadowTable;

struct XilSdnetTinyCamPrvCtx
{
    XilSdnetCamCtx *PrivateSdnetCamCtxPtr; /* Internal Tiny Cam context data used by driver */
    XilSdndetTinyCamTableStats TableInfo;  /* Description of the table entry */
    XilSdnetTinyCamShadowTable ShadowTable;

    XilSdnetTinyCamEmulationMode EmulationMode;

    uint8_t *WriteBufTableEntryPtr;
    uint8_t *ReadBufTableEntryPtr;
};

/****************************************************************************************************************************************************/
/* SECTION: CAM common function prototypes */
/****************************************************************************************************************************************************/

XilSdnetReturnType XilSdnetCamByteSwap(XilSdnetCamCtx *CtxPtr, uint8_t *SrcDataPtr, uint8_t *DstDataPtr, XilSdnetSwapCmd Cmd);

XilSdnetReturnType XilSdnetCamValidateCtx(XilSdnetCamCtx *CtxPtr);

XilSdnetReturnType XilSdnetCamCtxCreate(XilSdnetCamCtx **CtxPtr, XilSdnetEnvIf *EnvIfPtr, XilSdnetCamConfig *ConfigPtr);

XilSdnetReturnType XilSdnetCamCtxDestroy(XilSdnetCamCtx *CtxPtr);

/****************************************************************************************************************************************************/
/* SECTION: Internal TinyCam Function prototypes to facilate grey box testing */
/****************************************************************************************************************************************************/
XilSdnetReturnType XilSdnetTinyCamPrvShadowTableCreate(XilSdnetTinyCamPrvCtx *TinyCamPrvCtxPtr, XilSdnetTinyCamShadowTable *ShadowTablePtr);

XilSdnetReturnType XilSdnetTinyCamPrvShadowTableDestroy(XilSdnetTinyCamShadowTable *ShadowTablePtr);

XilSdnetReturnType XilSdnetTinyCamPrvShadowTableNextEmptyEntry(XilSdnetTinyCamShadowTable *ShadowTablePtr, XilSdnetTinyCamEntry **EmptyEntryPtrPtr);

XilSdnetReturnType XilSdnetTinyCamPrvShadowTableGetEntryById(XilSdnetTinyCamShadowTable *ShadowTablePtr,
                                                             uint32_t EntryId,
                                                             XilSdnetTinyCamEntry **EmptyEntryPtrPtr);

/* Null for Current indicates at the HEAD, will also remove the Insert Entry from the list as the list will not add new entries only rearranged
 * existing entries */
XilSdnetReturnType XilSdnetTinyCamPrvShadowTableInsertEntryAfter(XilSdnetTinyCamShadowTable *ShadowTablePtr,
                                                                 XilSdnetTinyCamEntry *CurrentEntryPtr,
                                                                 XilSdnetTinyCamEntry *InsertEntryPtr);

XilSdnetReturnType XilSdnetTinyCamPrvShadowTableMoveEmptyEntryToTail(XilSdnetTinyCamShadowTable *ShadowTablePtr,
                                                                      XilSdnetTinyCamEntry *CurrentEntryPtr);

XilSdnetReturnType XilSdnetTinyCamPrvShadowTableFindEntry(XilSdnetTinyCamPrvCtx *TinyCamPrvCtxPtr,
                                                          XilSdnetTinyCamShadowTable *ShadowTablePtr,
                                                          uint8_t *KeyPtr,
                                                          uint8_t *MaskPtr,
                                                          XilSdnetTinyCamEntry **FoundEntryPtrPtr);

XilSdnetReturnType XilSdnetTinyCamPrvIpShadowTableFindEntryByResponse(XilSdnetTinyCamPrvCtx *TinyCamPrvCtxPtr,
                                                                      XilSdnetTinyCamShadowTable *ShadowTablePtr,
                                                                      uint8_t *ResponsePtr,
                                                                      uint8_t *MaskPtr,
                                                                      XilSdnetTinyCamEntry **FoundEntryPtrPtr);

/* Finds the entry in shadow table where all entires after the found entry will have a lower priority. For case
 * where no lower priority entries exist the Found entry will be assigned NULL, i.e. need to insert higher priority
 * at the head of the shadow tail
 */
XilSdnetReturnType XilSdnetTinyCamPrvShadowTableFindPivotEntryByPriority(XilSdnetTinyCamShadowTable *ShadowTablePtr,
                                                                          uint32_t Priority,
                                                                          XilSdnetTinyCamEntry **FoundEntryPtrPtr);

/* Should not attempt to fill an entry that is InUse, will return an error of XIL_SDNET_GENERAL_ERR_INTERNAL_ASSERTION,
 * would be an internal error from a user point of view
 */
XilSdnetReturnType XilSdnetTinyCamPrvShadowTableFillEntry(XilSdnetTinyCamPrvCtx *TinyCamPrvCtxPtr,
                                                          XilSdnetTinyCamEntry *EntryPtr,
                                                          uint8_t *KeyPtr,
                                                          uint8_t *MaskPtr,
                                                          uint32_t Priority,
                                                          uint8_t *ResponsePtr);

XilSdnetReturnType XilSdnetTinyCamPrvShadowTableUpdateEntry(XilSdnetTinyCamPrvCtx *TinyCamPrvCtxPtr,
                                                            XilSdnetTinyCamEntry *EntryPtr,
                                                            uint8_t *ResponsePtr);

XilSdnetReturnType XilSdnetTinyCamPrvShadowTableEmptyEntry(XilSdnetTinyCamPrvCtx *TinyCamPrvCtxPtr, XilSdnetTinyCamEntry *EntryPtr);

/****************************************************************************************************************************************************/
/* SECTION: TinyCAM  functions specific to TCAM Emulation Mode*/
/****************************************************************************************************************************************************/

/****************************************************************************************************************************************************/
/* SECTION: TinyCAM  functions specific to BCAM Emulation Mode*/
/****************************************************************************************************************************************************/

/****************************************************************************************************************************************************/
/* SECTION: TinyCam used to Entries to the CAM IP Table*/
/****************************************************************************************************************************************************/
XilSdnetReturnType XilSdnetTinyCamPrvIpTableDeleteEntry(XilSdnetTinyCamPrvCtx *TinyCamPrvCtxPtr, XilSdnetTinyCamEntry *EntryPtr);

XilSdnetReturnType XilSdnetTinyCamPrvIpTableWriteEntry(XilSdnetTinyCamPrvCtx *TinyCamPrvCtxPtr, XilSdnetTinyCamEntry *EntryPtr);

XilSdnetReturnType XilSdnetTinyCamPrvIpTableRewriteEntry(XilSdnetTinyCamPrvCtx *TinyCamPrvCtxPtr, XilSdnetTinyCamEntry *EntryPtr);

XilSdnetReturnType XilSdnetTinyCamPrvIpTableReadEntry(XilSdnetTinyCamPrvCtx *TinyCamPrvCtxPtr, XilSdnetTinyCamEntry *EntryPtr);

XilSdnetReturnType XilSdnetTinyCamPrvIpTableCheckEntryMatches(XilSdnetTinyCamPrvCtx *TinyCamPrvCtxPtr,
                                                              XilSdnetTinyCamEntry *EntryPtr,
                                                              bool *EntryMatchesPtr);

XilSdnetReturnType XilSdnetTinyCamPrvIpTableCheckEntryEmpty(XilSdnetTinyCamPrvCtx *TinyCamPrvCtxPtr,
                                                            XilSdnetTinyCamEntry *EntryPtr,
                                                            bool *EntryEmptyPtr);

#ifdef __cplusplus
}
#endif

#endif // CAM_PRIV_H