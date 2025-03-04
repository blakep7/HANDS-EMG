/**
  ******************************************************************************
  * @file    emgclassifier_data_params.h
  * @author  AST Embedded Analytics Research Platform
  * @date    2025-03-03T17:56:44-0800
  * @brief   AI Tool Automatic Code Generator for Embedded NN computing
  ******************************************************************************
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  ******************************************************************************
  */

#ifndef EMGCLASSIFIER_DATA_PARAMS_H
#define EMGCLASSIFIER_DATA_PARAMS_H

#include "ai_platform.h"

/*
#define AI_EMGCLASSIFIER_DATA_WEIGHTS_PARAMS \
  (AI_HANDLE_PTR(&ai_emgclassifier_data_weights_params[1]))
*/

#define AI_EMGCLASSIFIER_DATA_CONFIG               (NULL)


#define AI_EMGCLASSIFIER_DATA_ACTIVATIONS_SIZES \
  { 464, }
#define AI_EMGCLASSIFIER_DATA_ACTIVATIONS_SIZE     (464)
#define AI_EMGCLASSIFIER_DATA_ACTIVATIONS_COUNT    (1)
#define AI_EMGCLASSIFIER_DATA_ACTIVATION_1_SIZE    (464)



#define AI_EMGCLASSIFIER_DATA_WEIGHTS_SIZES \
  { 26772, }
#define AI_EMGCLASSIFIER_DATA_WEIGHTS_SIZE         (26772)
#define AI_EMGCLASSIFIER_DATA_WEIGHTS_COUNT        (1)
#define AI_EMGCLASSIFIER_DATA_WEIGHT_1_SIZE        (26772)



#define AI_EMGCLASSIFIER_DATA_ACTIVATIONS_TABLE_GET() \
  (&g_emgclassifier_activations_table[1])

extern ai_handle g_emgclassifier_activations_table[1 + 2];



#define AI_EMGCLASSIFIER_DATA_WEIGHTS_TABLE_GET() \
  (&g_emgclassifier_weights_table[1])

extern ai_handle g_emgclassifier_weights_table[1 + 2];


#endif    /* EMGCLASSIFIER_DATA_PARAMS_H */
