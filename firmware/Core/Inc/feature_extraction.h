/*
 * feature_extraction.h
 *
 *  Created on: Mar 7, 2025
 *      Author: lilmo
 */

#ifndef SRC_FEATURE_EXTRACTION_H_
#define SRC_FEATURE_EXTRACTION_H_



#include <stdint.h>

#define FRAME_SIZE 400

#ifdef __cplusplus
extern "C" {
#endif

void convert_adc_data(const int32_t *adc_raw, int16_t *adc_processed);
void calculate_features(const int16_t *data, int16_t *features);

#ifdef __cplusplus
}
#endif



#endif /* SRC_FEATURE_EXTRACTION_H_ */
