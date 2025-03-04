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


#include <math.h>
#include <stdint.h>

#define FRAME_SIZE 400

void calculate_features(const int16_t *data, float *features) {
    float iemg = 0.0f, mav = 0.0f, ssi = 0.0f, rms = 0.0f;
    float variance = 0.0f, myop = 0.0f, wl = 0.0f, damv = 0.0f;
    float m2 = 0.0f, dvarv = 0.0f, dasdv = 0.0f, wamp = 0.0f;

    float mean = 0.0f;
    float prev_sample = (float)data[0];

    //calculate mean
    for (int i = 0; i < FRAME_SIZE; i++) {
        mean += (float)data[i];
    }
    mean /= FRAME_SIZE;

    // variance, and std
    float sum_sq_diff = 0.0f;
    for (int i = 0; i < FRAME_SIZE; i++) {
        float diff = (float)data[i] - mean;
        sum_sq_diff += diff * diff;
    }
    float std_dev = sqrtf(sum_sq_diff / FRAME_SIZE);

    // threshold for MYOP and WAMP. Should match whatever MATLAB script generates (2*std(data))
    float threshold = 0.3f * std_dev;

    // compute features
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
    myop = (myop / FRAME_SIZE);
    damv /= (FRAME_SIZE - 1);
    dvarv /= (FRAME_SIZE - 1);
    dasdv = sqrtf(dvarv);

    	features[3] = rms;
        features[4] = variance;
        features[5] = myop;
        features[6] = wl;
        features[7] = damv;
        features[8] = m2;
        features[9] = dvarv;
        features[10] = dasdv;
        features[11] = wamp;
    }
