/////////////////////////////////////////////////////////////////////////////
//
// Copyright 2018 Xilinx, Inc. All rights reserved.
// This file contains confidential and proprietary information of Xilinx, Inc.
// and is protected under U.S. and international copyright and other
// intellectual property laws.
//
// DISCLAIMER
// This disclaimer is not a license and does not grant any
// rights to the materials distributed herewith. Except as
// otherwise provided in a valid license issued to you by
// Xilinx, and to the maximum extent permitted by applicable
// law: (1) THESE MATERIALS ARE MADE AVAILABLE "AS IS" AND
// WITH ALL FAULTS, AND XILINX HEREBY DISCLAIMS ALL WARRANTIES
// AND CONDITIONS, EXPRESS, IMPLIED, OR STATUTORY, INCLUDING
// BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, NON-
// INFRINGEMENT, OR FITNESS FOR ANY PARTICULAR PURPOSE; and
// (2) Xilinx shall not be liable (whether in contract or tort,
// including negligence, or under any other theory of
// liability) for any loss or damage of any kind or nature
// related to, arising under or in connection with these
// materials, including for any direct, or any indirect,
// special, incidental, or consequential loss or damage
// (including loss of data, profits, goodwill, or any type of
// loss or damage suffered as a result of any action brought
// by a third party) even if such damage or loss was
// reasonably foreseeable or Xilinx had been advised of the
// possibility of the same.
//
// CRITICAL APPLICATIONS
// Xilinx products are not designed or intended to be fail-
// safe, or for use in any application requiring fail-safe
// performance, such as life-support or safety devices or
// systems, Class III medical devices, nuclear facilities,
// applications related to the deployment of airbags, or any
// other applications that could lead to death, personal
// injury, or severe property or environmental damage
// (individually and collectively, "Critical
// Applications"). Customer assumes the sole risk and
// liability of any use of Xilinx products in Critical
// Applications, subject only to applicable laws and
// regulations governing limitations on product liability.
//
// THIS COPYRIGHT NOTICE AND DISCLAIMER MUST BE RETAINED AS
// PART OF THIS FILE AT ALL TIMES.
//


/**
 * Public API of the STCAM.
 *
 * @file stcam.h
 *
 *
 * @addtogroup stcam STCAM Public API
 * @{
 */


#ifndef _STCAM_H_
#define _STCAM_H_

#include "cam.h"

#ifdef __cplusplus
extern "C" {
#endif

struct stcam;

/**
 * Creates a stcam instance. Returns 0 if successful, otherwise an error code. A shadow of the HW instance
 * is created.
 *
 * @param[in] cam_arg       Arguments used to create the stcam instance. cam_arg is copied to the
 *                          stcam instance, and can be destroyed immediately after this call.
 *
 * @param[out] out_stcam    Returns the newly created stcam instance, memory is allocated using malloc.
 *                          If stcam_create returns an error the instance is not created.
 *                          In this case no memory is allocated, and out_stcam is left unchanged.
 *
 * @return                  Returns 0 for success, otherwise the error code.
 *
 */
int
stcam_create(const cam_arg_t *cam_arg, struct stcam **out_stcam);


/**
 * Inserts an entry to the stcam instance. If the entry is found, the function fails by returning
 * error code CAM_ERROR_DUPLICATE_FOUND.
 * If the entry is not found, then it is inserted to the stcam instance.
 * Returns 0 if successful, otherwise an error code.
 *
 * @param[in] stcam     Pointer to the stcam instance
 *
 * @param[in] key       The key. The key width (in bits) of the stcam instance specifies the number of bits
 *                      which will be copied from the argument. Any content outside the scope of the key width is ignored.
 *                      The key is assumed to be in little-endian format.
 *                      key[0] bit 0 is the least significant bit of the key.
 *                      key[1] bit 0 is bit 8 of the key and so on.
 *
 *@param[in] mask       The ternary mask. The key width (in bits) of the stcam instance specifies the number of bits
 *                      which will be copied from the argument.
 *                      Any content outside the scope of the defined key width is ignored.
 *                      The ternary mask is assumed to be in little-endian format.
 *                      mask[0] bit 0 is the least significant bit of the ternary mask.
 *                      mask[1] bit 0 is bit 8 of the ternary mask and so on.
 *                      If a mask bit is cleared, the corresponding key bit must also be cleared, otherwise
 *                      an error code is returned.
 *
 * @param[in] priority  The priority of the entry. If multiple matches occur, the winning matching
 *                      entry is determined based on the priority value. The entry with the lowest priority
 *                      wins. If there are multiple matches with the same lowest priority, any of them becomes
 *                      the winner.
 *                      Priority values greater than what can be represented by the defined priority
 *                      width generates an error code.
 *
 * @param[in] response  The response of the entry.
 *                      The response width of the instance specifies how many bits are copied and stored as response.
 *                      The response is in little-endian format.
 *
 * @return              Returns 0 for success, otherwise the error code.
 *
 */
int stcam_insert(struct stcam *stcam, const uint8_t key[], const uint8_t mask[], uint32_t priority,
        const uint8_t response[]);


/**
 * Updates an entry in the stcam instance. If the entry is found, the response is updated. For an entry to be found,
 * both the key and mask must match the "old entry".
 * If the entry is not found, the function fails with error code CAM_ERROR_NO_ENTRY.
 * Returns 0 if successful, otherwise an error code.
 *
 * @param[in] stcam      Pointer to the stcam instance
 *
 * @param[in] key       The key. The key width (in bits) of the stcam instance specifies the number of bits
 *                      which will be used from the argument. Any content outside the scope of the key width is ignored.
 *                      The key is assumed to be in little-endian format.
 *                      key[0] bit 0 is the least significant bit of the key.
 *                      key[1] bit 0 is bit 8 of the key and so on.
 *
 *@param[in] mask       The ternary mask. The key width (in bits) of the stcam instance specifies the number of bits
 *                      which will be used from the argument.
 *                      Any content outside the scope of the defined key width is ignored.
 *                      The ternary mask is assumed to be in little-endian format.
 *                      mask[0] bit 0 is the least significant bit of the ternary mask.
 *                      mask[1] bit 0 is bit 8 of the ternary mask and so on.
 *                      If a mask bit is cleared, the corresponding key bit must also be cleared, otherwise
 *                      an error code is returned.
 *
 * @param[in] response  The response of the entry.
 *                      The response width of the instance specifies the number of bits copied from the argument.
 *                      Any content outside the scope of the response width is ignored.
 *                      The response is in little-endian format.
 *
 * @return              Returns 0 for success, otherwise the error code.
 *
 */
int stcam_update(struct stcam *stcam, const uint8_t key[], const uint8_t mask[], const uint8_t response[]);


/**
 * Gets an entry with matching response value. If an entry is found, the function returns 0.
 * If the entry is not found, the function fails with error code CAM_ERROR_KEY_NOT_FOUND.
 *
 * The pos argument enables multiple matching entries to be found. The first time the function is
 * called pos should be set to zero. To find all entries matching the response, the function is
 * called multiple times, each call returns one matching entry.
 * For every consecutive call the returned pos value must be used again as input argument.
 * When CAM_ERROR_KEY_NOT_FOUND is returned no more entries matching the response could be found.
 *
 * The response_mask is used to limit the search to subfields of the response. If the complete,
 * exact response value is searched, then all response_mask bits should be set to one.
 * By setting the response_mask bits to all zeros, any response value will match.
 * This can be used for getting all entries without prior knowledge of the response values.
 *
 * If the function fails, the out arguments might have been modified (but are not valid).
 * The get function does not read from the hardware. It reads from the shadow.
 * tcam_get_by_response uses linear search.
 *
 * @param[in] stcam              Pointer to stcam instance
 *
 * @param[in] response          The response to search for.
 *                              The response is in little-endian format.
 *
 * @param[in] response_mask     The response and response_mask are ANDed and then
 *                              searched for. The stored response is also ANDed
 *                              with the response_mask before compare takes place.
 *                              The response_mask is in little-endian format.
 *
 * @param[in,out] pos           Position in the database. Used for
 *                              consecutive get operations, set to 0 for
 *                              first get.
 *
 * @param[out] out_key          The read key. Key width bits are copied to the key.
 *                              The key is in little-endian format.
 *
 * @param[out] out_mask         The read ternary mask. Key width bits are copied to the ternary mask.
 *                              The ternary mask is in little-endian format.
 *
 * @return                      Returns 0 for success, otherwise the error code.
 *
 */
int stcam_get_by_response(struct stcam *stcam, const uint8_t response[], const uint8_t response_mask[], uint32_t *pos,
        uint8_t out_key[], uint8_t out_mask[]);

/**
 * Gets an entry with matching key and mask. If an entry with both a matching key and mask is found,
 * the function returns 0. If an entry is not found, the function fails with error code CAM_ERROR_KEY_NOT_FOUND.
 * The function does not read from the hardware, it reads from the shadow.
 * tcam_get_by_key uses a hash table and is fast.
 *
 * @param[in] stcam     Pointer to stcam instance
 *
 * @param[in] key       The key to search for. The key. The key width (in bits) of the stcam instance specifies the number of bits
 *                      which will be used from the argument. Any content outside the scope of the key width is ignored.
 *                      The key is assumed to be in little-endian format.
 *                      key[0] bit 0 is the least significant bit of the key.
 *                      key[1] bit 0 is bit 8 of the key and so on.
 *
 * @param[in] mask      The ternary mask to search for. The key width (in bits) of the stcam instance specifies the number of bits
 *                      which will be used from the argument.
 *                      Any content outside the scope of the defined key width is ignored.
 *                      The ternary mask is assumed to be in little-endian format.
 *                      mask[0] bit 0 is the least significant bit of the ternary mask.
 *                      mask[1] bit 0 is bit 8 of the ternary mask and so on.
 *
 * @param[out] priority The priority of the entry.
 *
 * @param[out] response The response of the key. The response is in little-endian format.
 *
 * @return              Returns 0 for success, otherwise the error code.
 *
 */
int stcam_get_by_key(struct stcam *stcam, const uint8_t key[], const uint8_t mask[], uint32_t *priority,
        uint8_t response[]);


/**
 * Delete an entry from the stcam instance. If the entry is found, the function returns 0.
 * For an entry to be found both the key and mask must match an "old" entry".
 * If an entry is not found, the function fails with error code CAM_ERROR_KEY_NOT_FOUND.
 *
 * @param[in] stcam     Pointer to stcam instance
 *
 * @param[in] key       The key to delete. The key. The key width (in bits) of the stcam instance specifies the number of bits
 *                      which will be copied from the argument. Any content outside the scope of the key width is ignored.
 *                      The key is assumed to be in little-endian format.
 *                      key[0] bit 0 is the least significant bit of the key.
 *                      key[1] bit 0 is bit 8 of the key and so on.
 *
 * @param[in] mask      The ternary mask to delete. The key width (in bits) of the stcam instance specifies the number of bits
 *                      which will be used from the argument.
 *                      Any content outside the scope of the defined key width is ignored.
 *                      The ternary mask is assumed to be in little-endian format.
 *                      mask[0] bit 0 is the least significant bit of the ternary mask.
 *                      mask[1] bit 0 is bit 8 of the ternary mask and so on.
 *                      If a mask bit is cleared, the corresponding key bit must also be cleared, otherwise
 *                      an error code is returned.
 *
 * @return              Returns 0 for success, otherwise the error code.
 *
 */
int stcam_delete(struct stcam *stcam, const uint8_t key[], const uint8_t mask[]);

/**
 * Looks up an entry in the stcam instance.
 * This function provides the same response as if a lookup had been performed in hardware.
 * If no match is found in the database, the response value associated with the default response entry
 * is returned and the return code is CAM_ERROR_KEY_NOT_FOUND.
 * If an entry is found in the data base, the function returns 0.
 *
 * @param[in] stcam     Pointer to stcam instance
 *
 * @param[in] key       The key to lookup. The key. The key width (in bits) of the stcam instance specifies the number of bits
 *                      in the key. Any content outside the scope of the key width is ignored.
 *                      The key is assumed to be in little-endian format.
 *                      key[0] bit 0 is the least significant bit of the key.
 *                      key[1] bit 0 is bit 8 of the key and so on.
 *
 * @param[out] response The response from the lookup. The response is in little-endian format.
 *
 * @return              Returns 0 for success, otherwise the error code.
 *
 */
int stcam_lookup(struct stcam *stcam, const uint8_t key[], uint8_t response[]);

/**
 * Reads and clears the ECC counters of the stcam instance. When the instance is created the ECC counters
 * are automatically cleared. This function always returns zero.
 *
 * @param[in] stcam                         Pointer to stcam instance
 *
 * @param[out] corrected_single_bit_errors  The number of single bit errors the hardware scrubbing
 *                                          process has detected and corrected permanently. Errors might
 *                                          have been detected by hardware lookups and then corrected
 *                                          dynamically for the lookups, these errors are not counted.
 *                                          The scrubbing process runs in the background and corrects
 *                                          errors permanently. The counter is only incremented after
 *                                          the error is corrected permanently.The counter is cleared
 *                                          when read.
 *
 * @param[out] detected_double_bit_errors   The number of detected double-bit errors. Double-bit
 *                                          errors are fatal errors and the database is corrupt. The hardware
 *                                          cannot correct the error without help from software. When a
 *                                          double bit error is detected either by scrubbing or a lookup, an
 *                                          interrupt signal is asserted. Double bit errors detected by scrubbing
 *                                          are counted by this counter. The counter is cleared when read.
 *                                          Lookups detecting double-bit errors will mark the lookup response with
 *                                          a double-bit-error indication. It is advised to use this indication to
 *                                          discard packets. Double bit errors detected by lookups are not counted.
 *
 * @return                                  Returns 0 for success, otherwise the error code.
 *
 */
int stcam_read_and_clear_ecc_counters(struct stcam *stcam, uint32_t *corrected_single_bit_errors,
        uint32_t *detected_double_bit_errors);

/**
 * Reads and clears the ECC address registers of the stcam instance. The ECC adresses registers are
 * automatically cleared when the stcam instance is created. This function always returns zero.
 *
 * @param[in] stcam                              Pointer to stcam instance
 *
 * @param[out] failing_address_single_bit_error  The address of the first single bit error detected and corrected
 *                                               by the hardware scrubbing process. Additional errors might
 *                                               have been detected during a hardware lookup and then corrected
 *                                               dynamically, but this register only reflect the
 *                                               errors detected by hardware scrubbing.
 *
 * @param[out] failing_address_double_bit_error  The address of the first double-bit error detected by the
 *                                               hardware scrubbing process.
 *
 * @return                                       Returns 0 for success, otherwise the error code.
 *
 */
int stcam_read_and_clear_ecc_addresses(struct stcam *stcam, uint32_t *failing_address_single_bit_error,
                                      uint32_t *failing_address_double_bit_error);

/**
 * Set error injection for ECC test. Error injection makes subsequently inserted entries to be stored in memory with errors.
 *
 * @param[in] stcam                              Pointer to stcam instance
 *
 * @param[in] inject_single_bit_errors           Enable insertion of single-bit errors (correctable).
 *
 * @param[in] inject_double_bit_errors           Enable insertion of double-bit errors (uncorrectable).
 *
 * @return                                       Returns 0 for success, otherwise the error code.
 *
 */
int stcam_set_ecc_test(struct stcam *stcam, bool inject_single_bit_errors, bool inject_double_bit_errors);



/**
 * Destroy stcam instance.
 *
 * @param[in]   stcam    Pointer to stcam instance. All memory previously allocated by stcam_create is released.
 */
void stcam_destroy(struct stcam *stcam);


/** @}*/

#ifdef __cplusplus
}
#endif

#endif
