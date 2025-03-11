/*
 * feature_extraction.c
 *
 *  Created on: Mar 3, 2025
 *      Author: Noah Marosok
 *      		Blake Pearson
 *
 *
 *   C function meant to calculate all of the features within the data frame
 *
 *   we should have 400 samples at 16 bits, floating point is fine dont worry about fixedpoint
 *
 *   features to calculate:
 *
 *   	IEMG - integrated EMG
 *
 *   	MAV - mean absolute value
 *
 *   	SSI - simple square integrated
 *
 *   	RMS - root mean square
 *
 *   	VAR - variance
 *
 *   	MYOP - myopulse percentage rate
 *
 *   	WL - waveform length
 *
 *   	DAMV - difference absolute mean value
 *
 *   	M2 - second order moment
 *
 *   	DVARV - difference variance version
 *
 *   	DASDV - difference absolute standard deviation value
 *
 *   	WAMP - willison amplitude
 *
 *
 */


#include <stdint.h>
#include <math.h>
#include <stdio.h>

#define FRAME_SIZE 400

//convert 24bit ADC (stored in int32_t) to int16_t
void convert_adc_data(const int32_t *adc_raw, int16_t *adc_processed) {
    for (int i = 0; i < FRAME_SIZE; i++) {
        adc_processed[i] = (int16_t)(adc_raw[i] >> 8);  // Truncate 24-bit to 16-bit
    }
}

// feature extraction
void calculate_features(const int16_t *data, int16_t *features) {
    float iemg = 0.0f, mav = 0.0f, ssi = 0.0f, rms = 0.0f;
    float variance = 0.0f, myop = 0.0f, wl = 0.0f, damv = 0.0f;
    float m2 = 0.0f, dvarv = 0.0f, dasdv = 0.0f, wamp = 0.0f;

    float mean = 0.0f;
    float prev_sample = (float)data[0];


    for (int i = 0; i < FRAME_SIZE; i++) {
        mean += (float)data[i];
    }
    mean /= FRAME_SIZE;


    float sum_sq_diff = 0.0f;
    for (int i = 0; i < FRAME_SIZE; i++) {
        float diff = (float)data[i] - mean;
        sum_sq_diff += diff * diff;
    }
    float std_dev = sqrtf(sum_sq_diff / FRAME_SIZE);
    float threshold = 0.3f * std_dev;

    for (int i = 0; i < FRAME_SIZE; i++) {
        float sample = (float)data[i];

        iemg += fabsf(sample);
        mav += fabsf(sample);
        ssi += sample * sample;
        wl += fabsf(sample - prev_sample);
        m2 += sample * sample;

        if (fabsf(sample) > threshold) {
            myop += 1.0f;
        }

        if (i > 0) {
            float diff = sample - prev_sample;
            damv += fabsf(diff);
            dvarv += diff * diff;
            dasdv += diff * diff;
            if (fabsf(diff) > threshold) {
                wamp += 1.0f;
            }
        }

        prev_sample = sample;
    }

    mav /= FRAME_SIZE;
    rms = sqrtf(ssi / FRAME_SIZE);
    variance = sum_sq_diff / FRAME_SIZE;
    myop = (myop / FRAME_SIZE) * 100.0f;
    damv /= (FRAME_SIZE - 1);
    dvarv /= (FRAME_SIZE - 1);
    dasdv = sqrtf(dvarv);

    // in16_t scaling for tflite cmopatibility
    float scale_factor = 32767.0f;
    features[0] = (int16_t)(iemg / scale_factor);
    features[1] = (int16_t)(mav * scale_factor);
    features[2] = (int16_t)(ssi / scale_factor);
    features[3] = (int16_t)(rms * scale_factor);
    features[4] = (int16_t)(variance * scale_factor);
    features[5] = (int16_t)(myop * scale_factor);
    features[6] = (int16_t)(wl / scale_factor);
    features[7] = (int16_t)(damv * scale_factor);
    features[8] = (int16_t)(m2 / scale_factor);
    features[9] = (int16_t)(dvarv * scale_factor);
    features[10] = (int16_t)(dasdv * scale_factor);
    features[11] = (int16_t)(wamp * scale_factor);
}



//for testing!!
int main() {
    int32_t adc_raw[FRAME_SIZE];
    int16_t adc_processed[FRAME_SIZE];
    int16_t features[12];

    FILE *file = fopen("emg_data_24bit_bin", "rb");
        if (!file) {
            printf("Error opening file\n");
            return;
        }
    fread(adc_raw, sizeof(int32_t), FRAME_SIZE, file);
    fclose(file);


    convert_adc_data(adc_raw, adc_processed);


    calculate_features(adc_processed, features);

    printf("Computed Features:\n");
    for (int i = 0; i < 12; i++) {
        printf("Feature %d: %d\n", i, features[i]);
    }

    return 0;
}

