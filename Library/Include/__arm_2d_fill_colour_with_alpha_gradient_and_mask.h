/*
 * Copyright (C) 2024 Arm Limited or its affiliates. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* ----------------------------------------------------------------------
 * Project:      Arm-2D Library
 * Title:        __arm_2d_fill_colour_with_alpha_gradient_and_mask.c
 * Description:  The header file of APIs for colour-filling-with-alpha-gradient
 *               -and-mask
 *
 * $Date:        22. July 2024
 * $Revision:    V.0.5.0
 *
 * Target Processor:  Cortex-M cores
 *
 * -------------------------------------------------------------------- */

#ifndef __ARM_2D_FILL_COLOUR_WITH_ALPHA_GRADIENT_AND_MASK_H__
#define __ARM_2D_FILL_COLOUR_WITH_ALPHA_GRADIENT_AND_MASK_H__

/*============================ INCLUDES ======================================*/
#include "arm_2d_types.h"

#ifdef   __cplusplus
extern "C" {
#endif

#if defined(__clang__)
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wunknown-warning-option"
#   pragma clang diagnostic ignored "-Wreserved-identifier"
#   pragma clang diagnostic ignored "-Wdeclaration-after-statement"
#   pragma clang diagnostic ignored "-Wsign-conversion"
#   pragma clang diagnostic ignored "-Wpadded"
#   pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#   pragma clang diagnostic ignored "-Wmissing-declarations"
#elif defined(__IS_COMPILER_ARM_COMPILER_5__)
#   pragma diag_suppress 174,177,188,68,513,144,64
#endif

/*!
 * \addtogroup gAlpha 4 Alpha Blending Operations
 * @{
 */

/*============================ MACROS ========================================*/
/*============================ MACROFIED FUNCTIONS ===========================*/

#define arm_2d_gray8_fill_colour_with_4pts_alpha_gradient_and_mask(            \
                                    __TARGET_ADDR,  /*   target tile address*/  \
                                    __REGION_ADDR,  /*   target region address*/\
                                    __COLOUR,       /*   target colour     */   \
                                    ...)            /*   sample points     */   \
            arm_2dp_gray8_fill_colour_with_4pts_alpha_gradient_and_mask(       \
                                                        NULL,                   \
                                                        (__TARGET_ADDR),        \
                                                        (__REGION_ADDR),        \
                                                        (__COLOUR),             \
                                                        __VA_ARGS__)

#define arm_2d_gray8_fill_colour_with_4pts_alpha_gradient_mask_and_opacity(    \
                                    __TARGET_ADDR,  /*   target tile address*/  \
                                    __REGION_ADDR,  /*   target region address*/\
                                    __COLOUR,       /*   target colour     */   \
                                    __OPACITY,      /*   opacity           */   \
                                    ...)            /*   sample points     */   \
        arm_2dp_gray8_fill_colour_with_4pts_alpha_gradient_mask_and_opacity(   \
                                                        NULL,                   \
                                                        (__TARGET_ADDR),        \
                                                        (__REGION_ADDR),        \
                                                        (__COLOUR),             \
                                                        (__OPACITY),            \
                                                        __VA_ARGS__)

#define arm_2d_gray8_fill_colour_with_3pts_alpha_gradient_and_mask(            \
                                    __TARGET_ADDR,  /*   target tile address*/  \
                                    __REGION_ADDR,  /*   target region address*/\
                                    __COLOUR,       /*   target colour     */   \
                                    ...)            /*   sample points     */   \
            arm_2dp_gray8_fill_colour_with_3pts_alpha_gradient_and_mask(       \
                                                        NULL,                   \
                                                        (__TARGET_ADDR),        \
                                                        (__REGION_ADDR),        \
                                                        (__COLOUR),             \
                                                        __VA_ARGS__)

#define arm_2d_gray8_fill_colour_with_3pts_alpha_gradient_mask_and_opacity(    \
                                    __TARGET_ADDR,  /*   target tile address*/  \
                                    __REGION_ADDR,  /*   target region address*/\
                                    __COLOUR,       /*   target colour     */   \
                                    __OPACITY,      /*   opacity           */   \
                                    ...)            /*   sample points     */   \
        arm_2dp_gray8_fill_colour_with_3pts_alpha_gradient_mask_and_opacity(   \
                                                        NULL,                   \
                                                        (__TARGET_ADDR),        \
                                                        (__REGION_ADDR),        \
                                                        (__COLOUR),             \
                                                        (__OPACITY),            \
                                                        __VA_ARGS__)

#define arm_2d_gray8_fill_colour_with_vertical_alpha_gradient_and_mask(        \
                                    __TARGET_ADDR,  /*   target tile address*/  \
                                    __REGION_ADDR,  /*   target region address*/\
                                    __COLOUR,       /*   target colour     */   \
                                    ...)            /*   sample points     */   \
            arm_2dp_gray8_fill_colour_with_vertical_alpha_gradient_and_mask(   \
                                                        NULL,                   \
                                                        (__TARGET_ADDR),        \
                                                        (__REGION_ADDR),        \
                                                        (__COLOUR),             \
                                                        __VA_ARGS__)

#define arm_2d_gray8_fill_colour_with_vertical_alpha_gradient_mask_and_opacity(\
                                    __TARGET_ADDR,  /*   target tile address*/  \
                                    __REGION_ADDR,  /*   target region address*/\
                                    __COLOUR,       /*   target colour     */   \
                                    __OPACITY,      /*   opacity           */   \
                                    ...)            /*   sample points     */   \
    arm_2dp_gray8_fill_colour_with_vertical_alpha_gradient_mask_and_opacity(   \
                                                        NULL,                   \
                                                        (__TARGET_ADDR),        \
                                                        (__REGION_ADDR),        \
                                                        (__COLOUR),             \
                                                        (__OPACITY),            \
                                                        __VA_ARGS__)

#define arm_2d_gray8_fill_colour_with_horizontal_alpha_gradient_and_mask(      \
                                    __TARGET_ADDR,  /*   target tile address*/  \
                                    __REGION_ADDR,  /*   target region address*/\
                                    __COLOUR,       /*   target colour     */   \
                                    ...)            /*   sample points     */   \
            arm_2dp_gray8_fill_colour_with_horizontal_alpha_gradient_and_mask( \
                                                        NULL,                   \
                                                        (__TARGET_ADDR),        \
                                                        (__REGION_ADDR),        \
                                                        (__COLOUR),             \
                                                        __VA_ARGS__)

#define arm_2d_gray8_fill_colour_with_horizontal_alpha_gradient_mask_and_opacity(\
                                    __TARGET_ADDR,  /*   target tile address*/  \
                                    __REGION_ADDR,  /*   target region address*/\
                                    __COLOUR,       /*   target colour     */   \
                                    __OPACITY,      /*   opacity           */   \
                                    ...)            /*   sample points     */   \
    arm_2dp_gray8_fill_colour_with_horizontal_alpha_gradient_mask_and_opacity(  \
                                                        NULL,                   \
                                                        (__TARGET_ADDR),        \
                                                        (__REGION_ADDR),        \
                                                        (__COLOUR),             \
                                                        (__OPACITY),            \
                                                        __VA_ARGS__)

#define arm_2d_rgb565_fill_colour_with_4pts_alpha_gradient_and_mask(            \
                                    __TARGET_ADDR,  /*   target tile address*/  \
                                    __REGION_ADDR,  /*   target region address*/\
                                    __COLOUR,       /*   target colour     */   \
                                    ...)            /*   sample points     */   \
            arm_2dp_rgb565_fill_colour_with_4pts_alpha_gradient_and_mask(       \
                                                        NULL,                   \
                                                        (__TARGET_ADDR),        \
                                                        (__REGION_ADDR),        \
                                                        (__COLOUR),             \
                                                        __VA_ARGS__)

#define arm_2d_rgb565_fill_colour_with_4pts_alpha_gradient_mask_and_opacity(    \
                                    __TARGET_ADDR,  /*   target tile address*/  \
                                    __REGION_ADDR,  /*   target region address*/\
                                    __COLOUR,       /*   target colour     */   \
                                    __OPACITY,      /*   opacity           */   \
                                    ...)            /*   sample points     */   \
        arm_2dp_rgb565_fill_colour_with_4pts_alpha_gradient_mask_and_opacity(   \
                                                        NULL,                   \
                                                        (__TARGET_ADDR),        \
                                                        (__REGION_ADDR),        \
                                                        (__COLOUR),             \
                                                        (__OPACITY),            \
                                                        __VA_ARGS__)

#define arm_2d_rgb565_fill_colour_with_3pts_alpha_gradient_and_mask(            \
                                    __TARGET_ADDR,  /*   target tile address*/  \
                                    __REGION_ADDR,  /*   target region address*/\
                                    __COLOUR,       /*   target colour     */   \
                                    ...)            /*   sample points     */   \
            arm_2dp_rgb565_fill_colour_with_3pts_alpha_gradient_and_mask(       \
                                                        NULL,                   \
                                                        (__TARGET_ADDR),        \
                                                        (__REGION_ADDR),        \
                                                        (__COLOUR),             \
                                                        __VA_ARGS__)

#define arm_2d_rgb565_fill_colour_with_3pts_alpha_gradient_mask_and_opacity(    \
                                    __TARGET_ADDR,  /*   target tile address*/  \
                                    __REGION_ADDR,  /*   target region address*/\
                                    __COLOUR,       /*   target colour     */   \
                                    __OPACITY,      /*   opacity           */   \
                                    ...)            /*   sample points     */   \
        arm_2dp_rgb565_fill_colour_with_3pts_alpha_gradient_mask_and_opacity(   \
                                                        NULL,                   \
                                                        (__TARGET_ADDR),        \
                                                        (__REGION_ADDR),        \
                                                        (__COLOUR),             \
                                                        (__OPACITY),            \
                                                        __VA_ARGS__)

#define arm_2d_rgb565_fill_colour_with_vertical_alpha_gradient_and_mask(        \
                                    __TARGET_ADDR,  /*   target tile address*/  \
                                    __REGION_ADDR,  /*   target region address*/\
                                    __COLOUR,       /*   target colour     */   \
                                    ...)            /*   sample points     */   \
            arm_2dp_rgb565_fill_colour_with_vertical_alpha_gradient_and_mask(   \
                                                        NULL,                   \
                                                        (__TARGET_ADDR),        \
                                                        (__REGION_ADDR),        \
                                                        (__COLOUR),             \
                                                        __VA_ARGS__)

#define arm_2d_rgb565_fill_colour_with_vertical_alpha_gradient_mask_and_opacity(\
                                    __TARGET_ADDR,  /*   target tile address*/  \
                                    __REGION_ADDR,  /*   target region address*/\
                                    __COLOUR,       /*   target colour     */   \
                                    __OPACITY,      /*   opacity           */   \
                                    ...)            /*   sample points     */   \
    arm_2dp_rgb565_fill_colour_with_vertical_alpha_gradient_mask_and_opacity(   \
                                                        NULL,                   \
                                                        (__TARGET_ADDR),        \
                                                        (__REGION_ADDR),        \
                                                        (__COLOUR),             \
                                                        (__OPACITY),            \
                                                        __VA_ARGS__)

#define arm_2d_rgb565_fill_colour_with_horizontal_alpha_gradient_and_mask(      \
                                    __TARGET_ADDR,  /*   target tile address*/  \
                                    __REGION_ADDR,  /*   target region address*/\
                                    __COLOUR,       /*   target colour     */   \
                                    ...)            /*   sample points     */   \
            arm_2dp_rgb565_fill_colour_with_horizontal_alpha_gradient_and_mask( \
                                                        NULL,                   \
                                                        (__TARGET_ADDR),        \
                                                        (__REGION_ADDR),        \
                                                        (__COLOUR),             \
                                                        __VA_ARGS__)

#define arm_2d_rgb565_fill_colour_with_horizontal_alpha_gradient_mask_and_opacity(\
                                    __TARGET_ADDR,  /*   target tile address*/  \
                                    __REGION_ADDR,  /*   target region address*/\
                                    __COLOUR,       /*   target colour     */   \
                                    __OPACITY,      /*   opacity           */   \
                                    ...)            /*   sample points     */   \
    arm_2dp_rgb565_fill_colour_with_horizontal_alpha_gradient_mask_and_opacity(  \
                                                        NULL,                   \
                                                        (__TARGET_ADDR),        \
                                                        (__REGION_ADDR),        \
                                                        (__COLOUR),             \
                                                        (__OPACITY),            \
                                                        __VA_ARGS__)

#define arm_2d_cccn888_fill_colour_with_4pts_alpha_gradient_and_mask(            \
                                    __TARGET_ADDR,  /*   target tile address*/  \
                                    __REGION_ADDR,  /*   target region address*/\
                                    __COLOUR,       /*   target colour     */   \
                                    ...)            /*   sample points     */   \
            arm_2dp_cccn888_fill_colour_with_4pts_alpha_gradient_and_mask(       \
                                                        NULL,                   \
                                                        (__TARGET_ADDR),        \
                                                        (__REGION_ADDR),        \
                                                        (__COLOUR),             \
                                                        __VA_ARGS__)

#define arm_2d_cccn888_fill_colour_with_4pts_alpha_gradient_mask_and_opacity(    \
                                    __TARGET_ADDR,  /*   target tile address*/  \
                                    __REGION_ADDR,  /*   target region address*/\
                                    __COLOUR,       /*   target colour     */   \
                                    __OPACITY,      /*   opacity           */   \
                                    ...)            /*   sample points     */   \
        arm_2dp_cccn888_fill_colour_with_4pts_alpha_gradient_mask_and_opacity(   \
                                                        NULL,                   \
                                                        (__TARGET_ADDR),        \
                                                        (__REGION_ADDR),        \
                                                        (__COLOUR),             \
                                                        (__OPACITY),            \
                                                        __VA_ARGS__)

#define arm_2d_cccn888_fill_colour_with_3pts_alpha_gradient_and_mask(            \
                                    __TARGET_ADDR,  /*   target tile address*/  \
                                    __REGION_ADDR,  /*   target region address*/\
                                    __COLOUR,       /*   target colour     */   \
                                    ...)            /*   sample points     */   \
            arm_2dp_cccn888_fill_colour_with_3pts_alpha_gradient_and_mask(       \
                                                        NULL,                   \
                                                        (__TARGET_ADDR),        \
                                                        (__REGION_ADDR),        \
                                                        (__COLOUR),             \
                                                        __VA_ARGS__)

#define arm_2d_cccn888_fill_colour_with_3pts_alpha_gradient_mask_and_opacity(    \
                                    __TARGET_ADDR,  /*   target tile address*/  \
                                    __REGION_ADDR,  /*   target region address*/\
                                    __COLOUR,       /*   target colour     */   \
                                    __OPACITY,      /*   opacity           */   \
                                    ...)            /*   sample points     */   \
        arm_2dp_cccn888_fill_colour_with_3pts_alpha_gradient_mask_and_opacity(   \
                                                        NULL,                   \
                                                        (__TARGET_ADDR),        \
                                                        (__REGION_ADDR),        \
                                                        (__COLOUR),             \
                                                        (__OPACITY),            \
                                                        __VA_ARGS__)

#define arm_2d_cccn888_fill_colour_with_vertical_alpha_gradient_and_mask(        \
                                    __TARGET_ADDR,  /*   target tile address*/  \
                                    __REGION_ADDR,  /*   target region address*/\
                                    __COLOUR,       /*   target colour     */   \
                                    ...)            /*   sample points     */   \
            arm_2dp_cccn888_fill_colour_with_vertical_alpha_gradient_and_mask(   \
                                                        NULL,                   \
                                                        (__TARGET_ADDR),        \
                                                        (__REGION_ADDR),        \
                                                        (__COLOUR),             \
                                                        __VA_ARGS__)

#define arm_2d_cccn888_fill_colour_with_vertical_alpha_gradient_mask_and_opacity(\
                                    __TARGET_ADDR,  /*   target tile address*/  \
                                    __REGION_ADDR,  /*   target region address*/\
                                    __COLOUR,       /*   target colour     */   \
                                    __OPACITY,      /*   opacity           */   \
                                    ...)            /*   sample points     */   \
    arm_2dp_cccn888_fill_colour_with_vertical_alpha_gradient_mask_and_opacity(   \
                                                        NULL,                   \
                                                        (__TARGET_ADDR),        \
                                                        (__REGION_ADDR),        \
                                                        (__COLOUR),             \
                                                        (__OPACITY),            \
                                                        __VA_ARGS__)

#define arm_2d_cccn888_fill_colour_with_horizontal_alpha_gradient_and_mask(      \
                                    __TARGET_ADDR,  /*   target tile address*/  \
                                    __REGION_ADDR,  /*   target region address*/\
                                    __COLOUR,       /*   target colour     */   \
                                    ...)            /*   sample points     */   \
            arm_2dp_cccn888_fill_colour_with_horizontal_alpha_gradient_and_mask( \
                                                        NULL,                   \
                                                        (__TARGET_ADDR),        \
                                                        (__REGION_ADDR),        \
                                                        (__COLOUR),             \
                                                        __VA_ARGS__)

#define arm_2d_cccn888_fill_colour_with_horizontal_alpha_gradient_mask_and_opacity(\
                                    __TARGET_ADDR,  /*   target tile address*/  \
                                    __REGION_ADDR,  /*   target region address*/\
                                    __COLOUR,       /*   target colour     */   \
                                    __OPACITY,      /*   opacity           */   \
                                    ...)            /*   sample points     */   \
    arm_2dp_cccn888_fill_colour_with_horizontal_alpha_gradient_mask_and_opacity(  \
                                                        NULL,                   \
                                                        (__TARGET_ADDR),        \
                                                        (__REGION_ADDR),        \
                                                        (__COLOUR),             \
                                                        (__OPACITY),            \
                                                        __VA_ARGS__)

/*============================ TYPES =========================================*/

typedef struct arm_2d_fill_cl_4p_al_grd_msk_t arm_2d_fill_cl_4p_al_grd_msk_t;

struct arm_2d_fill_cl_4p_al_grd_msk_t {
ARM_PRIVATE(
    implement(arm_2d_op_src_t);                     /* inherit from base class arm_2d_op_src_t*/

    union {
        uint8_t  chColour;                      //!< 8bit colour
        uint16_t hwColour;                      //!< 16bit colour
        uint32_t wColour;                       //!< 32bit colour
    };
    
    arm_2d_alpha_samples_4pts_t tSamplePoints;
)
};

typedef struct arm_2d_fill_cl_3p_al_grd_msk_t arm_2d_fill_cl_3p_al_grd_msk_t;

struct arm_2d_fill_cl_3p_al_grd_msk_t {
ARM_PRIVATE(
    implement(arm_2d_op_src_t);                     /* inherit from base class arm_2d_op_src_t*/

    union {
        uint8_t  chColour;                      //!< 8bit colour
        uint16_t hwColour;                      //!< 16bit colour
        uint32_t wColour;                       //!< 32bit colour
    };
    
    arm_2d_alpha_samples_3pts_t tSamplePoints;
)
};

typedef struct arm_2d_fill_cl_2p_al_grd_msk_t arm_2d_fill_cl_2p_al_grd_msk_t;

struct arm_2d_fill_cl_2p_al_grd_msk_t {
ARM_PRIVATE(
    implement(arm_2d_op_src_t);                     /* inherit from base class arm_2d_op_src_t*/

    union {
        uint8_t  chColour;                      //!< 8bit colour
        uint16_t hwColour;                      //!< 16bit colour
        uint32_t wColour;                       //!< 32bit colour
    };
    
    arm_2d_alpha_samples_2pts_t tSamplePoints;
)
};


/*============================ GLOBAL VARIABLES ==============================*/
/*============================ PROTOTYPES ====================================*/

/*!
 * \brief fill the target region with a given colour and an alpha gradient that
 *        is defined by 4 corner sample points and an mask. 
 * \param[in] ptOP the control block, NULL means using the default control block
 * \param[in] ptTarget the target tile
 * \param[in] ptRegion the target region
 * \param[in] ptMask the target mask
 * \param[in] tColour the target colour
 * \param[in] tSamplePoints the 4 corner sample points.
 * \return arm_fsm_rt_t the operations result
 */
extern
ARM_NONNULL(2)
arm_fsm_rt_t arm_2dp_gray8_fill_colour_with_4pts_alpha_gradient_and_mask(  
                            arm_2d_fill_cl_4p_al_grd_msk_t *ptOP,
                            const arm_2d_tile_t *ptTarget,
                            const arm_2d_region_t *ptRegion,
                            const arm_2d_tile_t *ptMask,
                            arm_2d_color_gray8_t tColour,
                            arm_2d_alpha_samples_4pts_t tSamplePoints);

/*!
 * \brief fill the target region with a given colour, an alpha gradient (that
 *        is defined by 4 corner sample points), an mask and an opacity
 * \param[in] ptOP the control block, NULL means using the default control block
 * \param[in] ptTarget the target tile
 * \param[in] ptRegion the target region
 * \param[in] ptMask the target mask
 * \param[in] tColour the target colour
 * \param[in] chOpacity the opacity ratio
 * \param[in] tSamplePoints the 4 corner sample points.
 * \return arm_fsm_rt_t the operations result
 */
extern
ARM_NONNULL(2)
arm_fsm_rt_t arm_2dp_gray8_fill_colour_with_4pts_alpha_gradient_mask_and_opacity(  
                            arm_2d_fill_cl_4p_al_grd_msk_t *ptOP,
                            const arm_2d_tile_t *ptTarget,
                            const arm_2d_region_t *ptRegion,
                            const arm_2d_tile_t *ptMask,
                            arm_2d_color_gray8_t tColour,
                            uint8_t chOpacity,
                            arm_2d_alpha_samples_4pts_t tSamplePoints);

/*!
 * \brief fill the target region with a given colour and an alpha gradient that
 *        is defined by 3 corner sample points and an mask.
 * \param[in] ptOP the control block, NULL means using the default control block
 * \param[in] ptTarget the target tile
 * \param[in] ptRegion the target region
 * \param[in] ptMask the target mask
 * \param[in] tColour the target colour
 * \param[in] tSamplePoints the 3 corner sample points.
 * \return arm_fsm_rt_t the operations result
 */
extern
ARM_NONNULL(2)
arm_fsm_rt_t arm_2dp_gray8_fill_colour_with_3pts_alpha_gradient_and_mask(  
                            arm_2d_fill_cl_3p_al_grd_msk_t *ptOP,
                            const arm_2d_tile_t *ptTarget,
                            const arm_2d_region_t *ptRegion,
                            const arm_2d_tile_t *ptMask,
                            arm_2d_color_gray8_t tColour,
                            arm_2d_alpha_samples_3pts_t tSamplePoints);

/*!
 * \brief fill the target region with a given colour, an alpha gradient (that
 *        is defined by 3 corner sample points), an mask and an opacity
 * \param[in] ptOP the control block, NULL means using the default control block
 * \param[in] ptTarget the target tile
 * \param[in] ptRegion the target region
 * \param[in] ptMask the target mask
 * \param[in] tColour the target colour
 * \param[in] chOpacity the opacity ratio
 * \param[in] tSamplePoints the 3 corner sample points.
 * \return arm_fsm_rt_t the operations result
 */
extern
ARM_NONNULL(2)
arm_fsm_rt_t arm_2dp_gray8_fill_colour_with_3pts_alpha_gradient_mask_and_opacity(  
                            arm_2d_fill_cl_3p_al_grd_msk_t *ptOP,
                            const arm_2d_tile_t *ptTarget,
                            const arm_2d_region_t *ptRegion,
                            const arm_2d_tile_t *ptMask,
                            arm_2d_color_gray8_t tColour,
                            uint8_t chOpacity,
                            arm_2d_alpha_samples_3pts_t tSamplePoints);


/*!
 * \brief fill the target region with a given colour and a vertical alpha gradient
 *         (that is defined by 2 corner sample points) and an mask.
 * \param[in] ptOP the control block, NULL means using the default control block
 * \param[in] ptTarget the target tile
 * \param[in] ptRegion the target region
 * \param[in] ptMask the target mask
 * \param[in] tColour the target colour
 * \param[in] tSamplePoints the 2 corner sample points.
 * \return arm_fsm_rt_t the operations result
 */
extern
ARM_NONNULL(2)
arm_fsm_rt_t arm_2dp_gray8_fill_colour_with_vertical_alpha_gradient_and_mask(  
                            arm_2d_fill_cl_2p_al_grd_msk_t *ptOP,
                            const arm_2d_tile_t *ptTarget,
                            const arm_2d_region_t *ptRegion,
                            const arm_2d_tile_t *ptMask,
                            arm_2d_color_gray8_t tColour,
                            arm_2d_alpha_samples_2pts_t tSamplePoints);

/*!
 * \brief fill the target region with a given colour, a vertical alpha gradient
 *         (that is defined by 2 corner sample points), an mask and an opacity
 * \param[in] ptOP the control block, NULL means using the default control block
 * \param[in] ptTarget the target tile
 * \param[in] ptRegion the target region
 * \param[in] ptMask the target mask
 * \param[in] tColour the target colour
 * \param[in] chOpacity the opacity ratio
 * \param[in] tSamplePoints the 2 corner sample points.
 * \return arm_fsm_rt_t the operations result
 */
extern
ARM_NONNULL(2)
arm_fsm_rt_t arm_2dp_gray8_fill_colour_with_vertical_alpha_gradient_mask_and_opacity(  
                            arm_2d_fill_cl_2p_al_grd_msk_t *ptOP,
                            const arm_2d_tile_t *ptTarget,
                            const arm_2d_region_t *ptRegion,
                            const arm_2d_tile_t *ptMask,
                            arm_2d_color_gray8_t tColour,
                            uint8_t chOpacity,
                            arm_2d_alpha_samples_2pts_t tSamplePoints);

/*!
 * \brief fill the target region with a given colour and a horizontal alpha
 *        gradient (that is defined by 2 corner sample points) and an mask.
 * \param[in] ptOP the control block, NULL means using the default control block
 * \param[in] ptTarget the target tile
 * \param[in] ptRegion the target region
 * \param[in] ptMask the target mask
 * \param[in] tColour the target colour
 * \param[in] tSamplePoints the 2 corner sample points.
 * \return arm_fsm_rt_t the operations result
 */
extern
ARM_NONNULL(2)
arm_fsm_rt_t arm_2dp_gray8_fill_colour_with_horizontal_alpha_gradient_and_mask(
                            arm_2d_fill_cl_2p_al_grd_msk_t *ptOP,
                            const arm_2d_tile_t *ptTarget,
                            const arm_2d_region_t *ptRegion,
                            const arm_2d_tile_t *ptMask,
                            arm_2d_color_gray8_t tColour,
                            arm_2d_alpha_samples_2pts_t tSamplePoints);

/*!
 * \brief fill the target region with a given colour, a horizontal alpha
 *         gradient (that is defined by 2 corner sample points), an mask and an opacity
 * \param[in] ptOP the control block, NULL means using the default control block
 * \param[in] ptTarget the target tile
 * \param[in] ptRegion the target region
 * \param[in] ptMask the target mask
 * \param[in] tColour the target colour
 * \param[in] chOpacity the opacity ratio
 * \param[in] tSamplePoints the 2 corner sample points.
 * \return arm_fsm_rt_t the operations result
 */
extern
ARM_NONNULL(2)
arm_fsm_rt_t arm_2dp_gray8_fill_colour_with_horizontal_alpha_gradient_mask_and_opacity(  
                            arm_2d_fill_cl_2p_al_grd_msk_t *ptOP,
                            const arm_2d_tile_t *ptTarget,
                            const arm_2d_region_t *ptRegion,
                            const arm_2d_tile_t *ptMask,
                            arm_2d_color_gray8_t tColour,
                            uint8_t chOpacity,
                            arm_2d_alpha_samples_2pts_t tSamplePoints);

/*!
 * \brief fill the target region with a given colour and an alpha gradient that
 *        is defined by 4 corner sample points and an mask. 
 * \param[in] ptOP the control block, NULL means using the default control block
 * \param[in] ptTarget the target tile
 * \param[in] ptRegion the target region
 * \param[in] ptMask the target mask
 * \param[in] tColour the target colour
 * \param[in] tSamplePoints the 4 corner sample points.
 * \return arm_fsm_rt_t the operations result
 */
extern
ARM_NONNULL(2)
arm_fsm_rt_t arm_2dp_rgb565_fill_colour_with_4pts_alpha_gradient_and_mask(  
                            arm_2d_fill_cl_4p_al_grd_msk_t *ptOP,
                            const arm_2d_tile_t *ptTarget,
                            const arm_2d_region_t *ptRegion,
                            const arm_2d_tile_t *ptMask,
                            arm_2d_color_rgb565_t tColour,
                            arm_2d_alpha_samples_4pts_t tSamplePoints);

/*!
 * \brief fill the target region with a given colour, an alpha gradient (that
 *        is defined by 4 corner sample points), an mask and an opacity
 * \param[in] ptOP the control block, NULL means using the default control block
 * \param[in] ptTarget the target tile
 * \param[in] ptRegion the target region
 * \param[in] ptMask the target mask
 * \param[in] tColour the target colour
 * \param[in] chOpacity the opacity ratio
 * \param[in] tSamplePoints the 4 corner sample points.
 * \return arm_fsm_rt_t the operations result
 */
extern
ARM_NONNULL(2)
arm_fsm_rt_t arm_2dp_rgb565_fill_colour_with_4pts_alpha_gradient_mask_and_opacity(  
                            arm_2d_fill_cl_4p_al_grd_msk_t *ptOP,
                            const arm_2d_tile_t *ptTarget,
                            const arm_2d_region_t *ptRegion,
                            const arm_2d_tile_t *ptMask,
                            arm_2d_color_rgb565_t tColour,
                            uint8_t chOpacity,
                            arm_2d_alpha_samples_4pts_t tSamplePoints);

/*!
 * \brief fill the target region with a given colour and an alpha gradient that
 *        is defined by 3 corner sample points and an mask.
 * \param[in] ptOP the control block, NULL means using the default control block
 * \param[in] ptTarget the target tile
 * \param[in] ptRegion the target region
 * \param[in] ptMask the target mask
 * \param[in] tColour the target colour
 * \param[in] tSamplePoints the 3 corner sample points.
 * \return arm_fsm_rt_t the operations result
 */
extern
ARM_NONNULL(2)
arm_fsm_rt_t arm_2dp_rgb565_fill_colour_with_3pts_alpha_gradient_and_mask(  
                            arm_2d_fill_cl_3p_al_grd_msk_t *ptOP,
                            const arm_2d_tile_t *ptTarget,
                            const arm_2d_region_t *ptRegion,
                            const arm_2d_tile_t *ptMask,
                            arm_2d_color_rgb565_t tColour,
                            arm_2d_alpha_samples_3pts_t tSamplePoints);

/*!
 * \brief fill the target region with a given colour, an alpha gradient (that
 *        is defined by 3 corner sample points), an mask and an opacity
 * \param[in] ptOP the control block, NULL means using the default control block
 * \param[in] ptTarget the target tile
 * \param[in] ptRegion the target region
 * \param[in] ptMask the target mask
 * \param[in] tColour the target colour
 * \param[in] chOpacity the opacity ratio
 * \param[in] tSamplePoints the 3 corner sample points.
 * \return arm_fsm_rt_t the operations result
 */
extern
ARM_NONNULL(2)
arm_fsm_rt_t arm_2dp_rgb565_fill_colour_with_3pts_alpha_gradient_mask_and_opacity(  
                            arm_2d_fill_cl_3p_al_grd_msk_t *ptOP,
                            const arm_2d_tile_t *ptTarget,
                            const arm_2d_region_t *ptRegion,
                            const arm_2d_tile_t *ptMask,
                            arm_2d_color_rgb565_t tColour,
                            uint8_t chOpacity,
                            arm_2d_alpha_samples_3pts_t tSamplePoints);


/*!
 * \brief fill the target region with a given colour and a vertical alpha gradient
 *         (that is defined by 2 corner sample points) and an mask.
 * \param[in] ptOP the control block, NULL means using the default control block
 * \param[in] ptTarget the target tile
 * \param[in] ptRegion the target region
 * \param[in] ptMask the target mask
 * \param[in] tColour the target colour
 * \param[in] tSamplePoints the 2 corner sample points.
 * \return arm_fsm_rt_t the operations result
 */
extern
ARM_NONNULL(2)
arm_fsm_rt_t arm_2dp_rgb565_fill_colour_with_vertical_alpha_gradient_and_mask(  
                            arm_2d_fill_cl_2p_al_grd_msk_t *ptOP,
                            const arm_2d_tile_t *ptTarget,
                            const arm_2d_region_t *ptRegion,
                            const arm_2d_tile_t *ptMask,
                            arm_2d_color_rgb565_t tColour,
                            arm_2d_alpha_samples_2pts_t tSamplePoints);

/*!
 * \brief fill the target region with a given colour, a vertical alpha gradient
 *         (that is defined by 2 corner sample points), an mask and an opacity
 * \param[in] ptOP the control block, NULL means using the default control block
 * \param[in] ptTarget the target tile
 * \param[in] ptRegion the target region
 * \param[in] ptMask the target mask
 * \param[in] tColour the target colour
 * \param[in] chOpacity the opacity ratio
 * \param[in] tSamplePoints the 2 corner sample points.
 * \return arm_fsm_rt_t the operations result
 */
extern
ARM_NONNULL(2)
arm_fsm_rt_t arm_2dp_rgb565_fill_colour_with_vertical_alpha_gradient_mask_and_opacity(  
                            arm_2d_fill_cl_2p_al_grd_msk_t *ptOP,
                            const arm_2d_tile_t *ptTarget,
                            const arm_2d_region_t *ptRegion,
                            const arm_2d_tile_t *ptMask,
                            arm_2d_color_rgb565_t tColour,
                            uint8_t chOpacity,
                            arm_2d_alpha_samples_2pts_t tSamplePoints);

/*!
 * \brief fill the target region with a given colour and a horizontal alpha
 *        gradient (that is defined by 2 corner sample points) and an mask.
 * \param[in] ptOP the control block, NULL means using the default control block
 * \param[in] ptTarget the target tile
 * \param[in] ptRegion the target region
 * \param[in] ptMask the target mask
 * \param[in] tColour the target colour
 * \param[in] tSamplePoints the 2 corner sample points.
 * \return arm_fsm_rt_t the operations result
 */
extern
ARM_NONNULL(2)
arm_fsm_rt_t arm_2dp_rgb565_fill_colour_with_horizontal_alpha_gradient_and_mask(
                            arm_2d_fill_cl_2p_al_grd_msk_t *ptOP,
                            const arm_2d_tile_t *ptTarget,
                            const arm_2d_region_t *ptRegion,
                            const arm_2d_tile_t *ptMask,
                            arm_2d_color_rgb565_t tColour,
                            arm_2d_alpha_samples_2pts_t tSamplePoints);

/*!
 * \brief fill the target region with a given colour, a horizontal alpha
 *         gradient (that is defined by 2 corner sample points), an mask and an opacity
 * \param[in] ptOP the control block, NULL means using the default control block
 * \param[in] ptTarget the target tile
 * \param[in] ptRegion the target region
 * \param[in] ptMask the target mask
 * \param[in] tColour the target colour
 * \param[in] chOpacity the opacity ratio
 * \param[in] tSamplePoints the 2 corner sample points.
 * \return arm_fsm_rt_t the operations result
 */
extern
ARM_NONNULL(2)
arm_fsm_rt_t arm_2dp_rgb565_fill_colour_with_horizontal_alpha_gradient_mask_and_opacity(  
                            arm_2d_fill_cl_2p_al_grd_msk_t *ptOP,
                            const arm_2d_tile_t *ptTarget,
                            const arm_2d_region_t *ptRegion,
                            const arm_2d_tile_t *ptMask,
                            arm_2d_color_rgb565_t tColour,
                            uint8_t chOpacity,
                            arm_2d_alpha_samples_2pts_t tSamplePoints);

/*!
 * \brief fill the target region with a given colour and an alpha gradient that
 *        is defined by 4 corner sample points and an mask. 
 * \param[in] ptOP the control block, NULL means using the default control block
 * \param[in] ptTarget the target tile
 * \param[in] ptRegion the target region
 * \param[in] ptMask the target mask
 * \param[in] tColour the target colour
 * \param[in] tSamplePoints the 4 corner sample points.
 * \return arm_fsm_rt_t the operations result
 */
extern
ARM_NONNULL(2)
arm_fsm_rt_t arm_2dp_cccn888_fill_colour_with_4pts_alpha_gradient_and_mask(  
                            arm_2d_fill_cl_4p_al_grd_msk_t *ptOP,
                            const arm_2d_tile_t *ptTarget,
                            const arm_2d_region_t *ptRegion,
                            const arm_2d_tile_t *ptMask,
                            arm_2d_color_cccn888_t tColour,
                            arm_2d_alpha_samples_4pts_t tSamplePoints);

/*!
 * \brief fill the target region with a given colour, an alpha gradient (that
 *        is defined by 4 corner sample points), an mask and an opacity
 * \param[in] ptOP the control block, NULL means using the default control block
 * \param[in] ptTarget the target tile
 * \param[in] ptRegion the target region
 * \param[in] ptMask the target mask
 * \param[in] tColour the target colour
 * \param[in] chOpacity the opacity ratio
 * \param[in] tSamplePoints the 4 corner sample points.
 * \return arm_fsm_rt_t the operations result
 */
extern
ARM_NONNULL(2)
arm_fsm_rt_t arm_2dp_cccn888_fill_colour_with_4pts_alpha_gradient_mask_and_opacity(  
                            arm_2d_fill_cl_4p_al_grd_msk_t *ptOP,
                            const arm_2d_tile_t *ptTarget,
                            const arm_2d_region_t *ptRegion,
                            const arm_2d_tile_t *ptMask,
                            arm_2d_color_cccn888_t tColour,
                            uint8_t chOpacity,
                            arm_2d_alpha_samples_4pts_t tSamplePoints);

/*!
 * \brief fill the target region with a given colour and an alpha gradient that
 *        is defined by 3 corner sample points and an mask.
 * \param[in] ptOP the control block, NULL means using the default control block
 * \param[in] ptTarget the target tile
 * \param[in] ptRegion the target region
 * \param[in] ptMask the target mask
 * \param[in] tColour the target colour
 * \param[in] tSamplePoints the 3 corner sample points.
 * \return arm_fsm_rt_t the operations result
 */
extern
ARM_NONNULL(2)
arm_fsm_rt_t arm_2dp_cccn888_fill_colour_with_3pts_alpha_gradient_and_mask(  
                            arm_2d_fill_cl_3p_al_grd_msk_t *ptOP,
                            const arm_2d_tile_t *ptTarget,
                            const arm_2d_region_t *ptRegion,
                            const arm_2d_tile_t *ptMask,
                            arm_2d_color_cccn888_t tColour,
                            arm_2d_alpha_samples_3pts_t tSamplePoints);

/*!
 * \brief fill the target region with a given colour, an alpha gradient (that
 *        is defined by 3 corner sample points), an mask and an opacity
 * \param[in] ptOP the control block, NULL means using the default control block
 * \param[in] ptTarget the target tile
 * \param[in] ptRegion the target region
 * \param[in] ptMask the target mask
 * \param[in] tColour the target colour
 * \param[in] chOpacity the opacity ratio
 * \param[in] tSamplePoints the 3 corner sample points.
 * \return arm_fsm_rt_t the operations result
 */
extern
ARM_NONNULL(2)
arm_fsm_rt_t arm_2dp_cccn888_fill_colour_with_3pts_alpha_gradient_mask_and_opacity(  
                            arm_2d_fill_cl_3p_al_grd_msk_t *ptOP,
                            const arm_2d_tile_t *ptTarget,
                            const arm_2d_region_t *ptRegion,
                            const arm_2d_tile_t *ptMask,
                            arm_2d_color_cccn888_t tColour,
                            uint8_t chOpacity,
                            arm_2d_alpha_samples_3pts_t tSamplePoints);


/*!
 * \brief fill the target region with a given colour and a vertical alpha gradient
 *         (that is defined by 2 corner sample points) and an mask.
 * \param[in] ptOP the control block, NULL means using the default control block
 * \param[in] ptTarget the target tile
 * \param[in] ptRegion the target region
 * \param[in] ptMask the target mask
 * \param[in] tColour the target colour
 * \param[in] tSamplePoints the 2 corner sample points.
 * \return arm_fsm_rt_t the operations result
 */
extern
ARM_NONNULL(2)
arm_fsm_rt_t arm_2dp_cccn888_fill_colour_with_vertical_alpha_gradient_and_mask(  
                            arm_2d_fill_cl_2p_al_grd_msk_t *ptOP,
                            const arm_2d_tile_t *ptTarget,
                            const arm_2d_region_t *ptRegion,
                            const arm_2d_tile_t *ptMask,
                            arm_2d_color_cccn888_t tColour,
                            arm_2d_alpha_samples_2pts_t tSamplePoints);

/*!
 * \brief fill the target region with a given colour, a vertical alpha gradient
 *         (that is defined by 2 corner sample points), an mask and an opacity
 * \param[in] ptOP the control block, NULL means using the default control block
 * \param[in] ptTarget the target tile
 * \param[in] ptRegion the target region
 * \param[in] ptMask the target mask
 * \param[in] tColour the target colour
 * \param[in] chOpacity the opacity ratio
 * \param[in] tSamplePoints the 2 corner sample points.
 * \return arm_fsm_rt_t the operations result
 */
extern
ARM_NONNULL(2)
arm_fsm_rt_t arm_2dp_cccn888_fill_colour_with_vertical_alpha_gradient_mask_and_opacity(  
                            arm_2d_fill_cl_2p_al_grd_msk_t *ptOP,
                            const arm_2d_tile_t *ptTarget,
                            const arm_2d_region_t *ptRegion,
                            const arm_2d_tile_t *ptMask,
                            arm_2d_color_cccn888_t tColour,
                            uint8_t chOpacity,
                            arm_2d_alpha_samples_2pts_t tSamplePoints);

/*!
 * \brief fill the target region with a given colour and a horizontal alpha
 *        gradient (that is defined by 2 corner sample points) and an mask.
 * \param[in] ptOP the control block, NULL means using the default control block
 * \param[in] ptTarget the target tile
 * \param[in] ptRegion the target region
 * \param[in] ptMask the target mask
 * \param[in] tColour the target colour
 * \param[in] tSamplePoints the 2 corner sample points.
 * \return arm_fsm_rt_t the operations result
 */
extern
ARM_NONNULL(2)
arm_fsm_rt_t arm_2dp_cccn888_fill_colour_with_horizontal_alpha_gradient_and_mask(
                            arm_2d_fill_cl_2p_al_grd_msk_t *ptOP,
                            const arm_2d_tile_t *ptTarget,
                            const arm_2d_region_t *ptRegion,
                            const arm_2d_tile_t *ptMask,
                            arm_2d_color_cccn888_t tColour,
                            arm_2d_alpha_samples_2pts_t tSamplePoints);

/*!
 * \brief fill the target region with a given colour, a horizontal alpha
 *         gradient (that is defined by 2 corner sample points), an mask and an opacity
 * \param[in] ptOP the control block, NULL means using the default control block
 * \param[in] ptTarget the target tile
 * \param[in] ptRegion the target region
 * \param[in] ptMask the target mask
 * \param[in] tColour the target colour
 * \param[in] chOpacity the opacity ratio
 * \param[in] tSamplePoints the 2 corner sample points.
 * \return arm_fsm_rt_t the operations result
 */
extern
ARM_NONNULL(2)
arm_fsm_rt_t arm_2dp_cccn888_fill_colour_with_horizontal_alpha_gradient_mask_and_opacity(  
                            arm_2d_fill_cl_2p_al_grd_msk_t *ptOP,
                            const arm_2d_tile_t *ptTarget,
                            const arm_2d_region_t *ptRegion,
                            const arm_2d_tile_t *ptMask,
                            arm_2d_color_cccn888_t tColour,
                            uint8_t chOpacity,
                            arm_2d_alpha_samples_2pts_t tSamplePoints);

/*! @} */

#if defined(__clang__)
#   pragma clang diagnostic pop
#endif

#ifdef   __cplusplus
}
#endif


#endif /* __ARM_2D_FILL_COLOUR_WITH_ALPHA_GRADIENT_AND_MASK_H__ */
