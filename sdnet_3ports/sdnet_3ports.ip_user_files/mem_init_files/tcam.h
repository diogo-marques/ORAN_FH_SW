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
 * Public API of the TCAM.
 *
 * @file tcam.h
 *
 *
 * @addtogroup tcam TCAM Public API
 * @{
 */


#ifndef _TCAM_H_
#define _TCAM_H_

#include "cam.h"

#ifdef __cplusplus
extern "C" {
#endif

struct tcam;




/**
 * Creates a tcam instance. Returns 0 if successful, otherwise an error code. A shadow of the HW instance
 * is created.
 *
 * @param[in] cam_arg   Arguments used to create the tcam instance. cam_arg is copied to the
 *                      tcam instance, and can be destroyed immediately after this call.
 *
 * @param[out] out_tcam Returns the newly created tcam instance, memory is allocated using malloc.
 *                      If tcam_create returns an error the instance is not created.
 *                      In this case no memory is allocated, and out_tcam is left unchanged.
 *
 * @return              Returns 0 for success, otherwise the error code.
 *
 */
int
tcam_create(const cam_arg_t *cam_arg, struct tcam **out_tcam);


/**
 * Inserts an entry to the tcam instance. If the entry is found, the function fails by returning
 * error code CAM_ERROR_DUPLICATE_FOUND.
 * If the entry is not found, then it is inserted in the tcam instance.
 * Returns 0 if successful, otherwise an error code.
 *
 * @param[in] tcam      Pointer to the tcam instance
 *
 * @param[in] key       The key. The key width (in bits) is implicitly specified by the format string.
 *                      The key width specifies how many bits are copied and stored.
 *                      Any contents outside the scope of the defined key width is ignored.
 *                      The key is assumed to be in little-endian format.
 *                      key[0] bit 0 is the least significant bit of the key.
 *                      key[1] bit 0 is bit 8 of the key and so on.
 *
 *@param[in] mask       The ternary mask. The key width (in bits) is implicitly specified by the format string.
 *                      The key width specifies how many bits are copied and stored.
 *                      Any contents outside the scope of the defined key width is ignored.
 *                      The ternary mask is assumed to be in little-endian format.
 *                      mask[0] bit 0 is the least significant bit of the ternary mask.
 *                      mask[1] bit 0 is bit 8 of the ternary mask and so on.
 *
 * @param[in] prio      The priority of the entry. If multiple matches occur, the winning matching
 *                      entry is determined based on the priority value. The entry with the lowest priority
 *                      wins. If there are multiple matches with the same lowest priority, any of them becomes
 *                      the winner.
 *                      Prio values greater than what can be represented by the defined prio
 *                      width generates an error code.
 *
 * @param[in] response  The response of the entry.
 *                      The defined response width specifies how many bits are copied and stored.
 *                      The response is in little-endian format.
 *
 * @return              Returns 0 for success, otherwise the error code.
 *
 */
int tcam_insert(struct tcam *tcam, const uint8_t key[], const uint8_t mask[], uint32_t prio, const uint8_t response[]);


/**
 * Updates an entry in the tcam instance. If the entry is found, the response is updated.
 * If the entry is not found, the function fails with error code CAM_ERROR_NO_ENTRY.
 * Returns 0 if successful, otherwise an error code.
 *
 * @param[in] tcam      Pointer to the tcam instance
 *
 * @param[in] key       The key. The key width (in bits) is implicitly specified by the format string.
 *                      The key width specifies how many bits of the key to use.
 *                      Any contents outside the scope of the defined key width is ignored.
 *                      The key is assumed to be in little-endian format.
 *                      key[0] bit 0 is the least significant bit of the key.
 *                      key[1] bit 0 is bit 8 of the key and so on.
 *
 *@param[in] mask       The ternary mask. The key width (in bits) is implicitly specified by the format string.
 *                      The key width specifies how many bits of the mask to use.
 *                      Any contents outside the scope of the defined key width is ignored.
 *                      The ternary mask is assumed to be in little-endian format.
 *                      mask[0] bit 0 is the least significant bit of the ternary mask.
 *                      mask[1] bit 0 is bit 8 of the ternary mask and so on.
 *
 * @param[in] response  The response of the entry.
 *                      The defined response width specifies how many bits are copied and stored.
 *                      The response is in little-endian format.
 *
 * @return              Returns 0 for success, otherwise the error code.
 *
 */
int tcam_update(struct tcam *tcam, const uint8_t key[], const uint8_t mask[], const uint8_t response[]);


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
 * The get function does not read from the hardware. It reads from the shadow.
 * tcam_get_by_response uses linear search.
 *
 * @param[in] tcam              Pointer to tcam instance
 *
 * @param[in] response          The response to search for. The response is in little-endian format.
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
int tcam_get_by_response(struct tcam *tcam, const uint8_t response[], const uint8_t response_mask[], uint32_t *pos,
        uint8_t out_key[],
        uint8_t out_mask[]);

/**
 * Gets an entry with matching key and mask. If an entry with both a matching key and mask is found,
 * the function returns 0. If an entry is not found, the function fails with error code CAM_ERROR_KEY_NOT_FOUND.
 * The function does not read from the hardware, it reads from the shadow.
 * tcam_get_by_key uses a hash table and is fast.
 *
 * @param[in] tcam      Pointer to tcam instance
 *
 * @param[in] key       The key to search for. The key width (in bits) is implicitly specified by the
 *                      format string. The key width specifies the number of bits in the key to use.
 *                      Any contents outside the scope of the defined key width is ignored.
 *                      The key is assumed to be in little-endian format.
 *                      key[0] bit 0 is the least significant bit of the key.
 *                      key[1] bit 0 is bit 8 of the key and so on.
 *
 * @param[in] mask      The ternary mask to search for. The key width (in bits) is implicitly specified by the
 *                      format string. The key width specifies how many bits of the mask to use.
 *                      Any contents outside the scope of the defined key width is ignored.
 *                      The mask is assumed to be in little-endian format.
 *                      mask[0] bit 0 is the least significant bit of the mask.
 *                      mask[1] bit 0 is bit 8 of the mask and so on.
 *
 * @param[out] prio     The prio of the key
 *
 * @param[out] response The response of the key. The response is in little-endian format.
 *
 * @return              Returns 0 for success, otherwise the error code.
 *
 */
int tcam_get_by_key(struct tcam *tcam, const uint8_t key[], const uint8_t mask[], uint32_t *prio, uint8_t response[]);

/**
 * Looks up an entry in the tcam instance.
 * This function provides the same response as if a lookup had been performed in hardware.
 * If no match is found in the database, the response value associated with the default response entry
 * is returned and the return code is CAM_ERROR_KEY_NOT_FOUND.
 * If an entry is found in the data base, the function returns 0.
 *
 * @param[in] tcam      Pointer to tcam instance
 *
 * @param[in] key       The key to lookup. The key width (in bits) is specified by the
 *                      key_width. The key width specifies how many bits of the key to use.
 *                      Any contents outside the scope of the defined key width is ignored.
 *                      The key is assumed to be in little-endian format.
 *                      key[0] bit 0 is the least significant bit of the key.
 *                      key[1] bit 0 is bit 8 of the key and so on.
 *
 * @param[out] response The response from the lookup. The response is in little-endian format.
 *
 * @return              Returns 0 for success, otherwise the error code.
 *
 */
int tcam_lookup(struct tcam *tcam, const uint8_t key[], uint8_t response[]);


/**
 * Delete an entry from the tcam instance. If the entry is found, the function returns 0.
 * If an entry is not found, the function fails with error code CAM_ERROR_KEY_NOT_FOUND.
 *
 * @param[in] tcam      Pointer to tcam instance
 *
 * @param[in] key       The key to delete. The key width (in bits) is implicitly specified by the
 *                      format string. The key width specifies how many bits of the key to use.
 *                      Any contents outside the scope of the defined key width is ignored.
 *                      The key is assumed to be in little-endian format.
 *                      key[0] bit 0 is the least significant bit of the key.
 *                      key[1] bit 0 is bit 8 of the key and so on.
 * @param[in] mask      The ternary mask to delete.
 *
 * @return              Returns 0 for success, otherwise the error code.
 *
 */
int tcam_delete(struct tcam *tcam, const uint8_t key[], const uint8_t mask[]);

/**
 * Reads and clears the ECC counters of the tcam instance. When the instance is created the ECC counters
 * are automatically cleared. This function always returns zero.
 *
 * @param[in] tcam                          Pointer to tcam instance
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
int tcam_read_and_clear_ecc_counters(struct tcam *tcam, uint32_t *corrected_single_bit_errors,
        uint32_t *detected_double_bit_errors);

/**
 * Reads and clears the ECC address registers of the tcam instance. The ECC adresses registers are
 * automatically cleared when the tcam instance is created. This function always returns zero.
 *
 * @param[in] tcam                               Pointer to tcam instance
 *
 * @param[out] failing_address_single_bit_error  The address of the first single bit error detected and corrected
 *                                               by the hardware scrubbing process. Additional errors might
 *                                               have been detected during a hardware lookup and then corrected
 *                                               dynamically, but this register only reflects the
 *                                               errors detected by hardware scrubbing.
 *
 * @param[out] failing_address_double_bit_error  The address of the first double-bit error detected by the
 *                                               hardware scrubbing process.
 *
 * @return                                       Returns 0 for success, otherwise the error code.
 *
 */
int tcam_read_and_clear_ecc_addresses(struct tcam *tcam, uint32_t *failing_address_single_bit_error,
                                      uint32_t *failing_address_double_bit_error);


/**
 * Set error injection for ECC test. Error injection makes subsequently inserted entries to be stored in memory with errors.
 *
 * @param[in] tcam                               Pointer to tcam instance
 *
 * @param[in] inject_single_bit_errors           Enable insertion of single-bit errors (correctable).
 *
 * @param[in] inject_double_bit_errors           Enable insertion of double-bit errors (uncorrectable).
 *
 * @return                                       Returns 0 for success, otherwise the error code.
 *
 */
int tcam_set_ecc_test(struct tcam *tcam,
                      bool inject_single_bit_errors,
                      bool inject_double_bit_errors);


/**
 * Destroy tcam instance.
 *
 * @param[in]   tcam    Pointer to tcam instance. All memory previously allocated by tcam_create is released.
 *
 * @return              Returns 0 for success, otherwise the error code.
 *
 */
int tcam_destroy(struct tcam *tcam);


/** @}*/

#ifdef __cplusplus
}
#endif

#endif
