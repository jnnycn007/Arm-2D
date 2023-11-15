/*
 * Copyright (c) 2009-2020 Arm Limited. All rights reserved.
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
 * Title:        #include "arm_2d_helper_pfb.c"
 * Description:  the pfb helper service source code
 *
 * $Date:        15. Nov 2023
 * $Revision:    V.1.7.0-dev
 *
 * Target Processor:  Cortex-M cores
 * -------------------------------------------------------------------- */

/*============================ INCLUDES ======================================*/
#define __ARM_2D_IMPL__

#include <stdbool.h>
#include <stdint.h>
#include <assert.h>
#include "arm_2d_helper.h"

#if defined(__PERF_COUNTER__)
#   include "perf_counter.h"
#else
#   include <time.h>
#endif

#if defined(__clang__)
#   pragma clang diagnostic push
#   pragma clang diagnostic ignored "-Wunknown-warning-option"
#   pragma clang diagnostic ignored "-Wreserved-identifier"
#   pragma clang diagnostic ignored "-Wdeclaration-after-statement"
#   pragma clang diagnostic ignored "-Wcast-qual"
#   pragma clang diagnostic ignored "-Wsign-conversion"
#   pragma clang diagnostic ignored "-Wpadded"
#   pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#   pragma clang diagnostic ignored "-Wmissing-prototypes"
#   pragma clang diagnostic ignored "-Wimplicit-fallthrough"
#   pragma clang diagnostic ignored "-Wundef"
#   pragma clang diagnostic ignored "-Wgnu-statement-expression"
#   pragma clang diagnostic ignored "-Wcast-align"
#   pragma clang diagnostic ignored "-Wconditional-uninitialized"
#   pragma clang diagnostic ignored "-Wunreachable-code-return"
#   pragma clang diagnostic ignored "-Wimplicit-int-conversion"
#   pragma clang diagnostic ignored "-Wsign-compare"
#   pragma clang diagnostic ignored "-Wunused-function"
#elif defined(__IS_COMPILER_GCC__)
#   pragma GCC diagnostic push
#   pragma GCC diagnostic ignored "-Wpedantic"
#   pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif

/*============================ MACROS ========================================*/

#undef  this
#define this            (*ptThis)

/*============================ MACROFIED FUNCTIONS ===========================*/
/*============================ TYPES =========================================*/
/*============================ GLOBAL VARIABLES ==============================*/
/*============================ LOCAL VARIABLES ===============================*/
/*============================ PROTOTYPES ====================================*/

ARM_NONNULL(1)
static void __arm_2d_helper_dirty_region_pool_free(
                                            arm_2d_helper_pfb_t *ptThis,
                                            arm_2d_region_list_item_t *ptItem);

/*============================ IMPLEMENTATION ================================*/

/*----------------------------------------------------------------------------*
 * PFB Helper                                                                 *
 *----------------------------------------------------------------------------*/

ARM_NONNULL(1)
arm_2d_pfb_t *__arm_2d_helper_pfb_new(arm_2d_helper_pfb_t *ptThis)
{
    assert(NULL != ptThis);
    
    arm_2d_pfb_t *ptPFB = NULL;

    arm_irq_safe {
        if (this.Adapter.chFreePFBCount) {
            this.Adapter.chFreePFBCount--;
            ARM_LIST_STACK_POP(this.Adapter.ptFreeList, ptPFB);
        }
    }

    if (NULL != ptPFB) {
        ptPFB->ptPFBHelper = ptThis;
    }

    return ptPFB;
}

ARM_NONNULL(1)
void __arm_2d_helper_pfb_free(arm_2d_helper_pfb_t *ptThis, arm_2d_pfb_t *ptPFB)
{
    assert(NULL != ptThis);
    
    if (NULL == ptPFB) {
        return ;
    }

    arm_irq_safe {
        ARM_LIST_STACK_PUSH(this.Adapter.ptFreeList, ptPFB);
        this.Adapter.chFreePFBCount++;
    }
    
    /* set event */
    if  (   (NULL == this.tCFG.Dependency.evtOnLowLevelSyncUp.fnHandler)
        &&  ((uintptr_t)NULL != this.Adapter.pFPBPoolAvailable)) {
        arm_2d_port_set_semaphore(this.Adapter.pFPBPoolAvailable);
    }
}

ARM_NONNULL(1,2)
arm_2d_err_t arm_2d_helper_pfb_init(arm_2d_helper_pfb_t *ptThis, 
                                    arm_2d_helper_pfb_cfg_t *ptCFG)
{
    assert(NULL != ptThis);
    assert(NULL != ptCFG);
    
    arm_2d_helper_init();
    
    memset(ptThis, 0, sizeof(this));
    this.tCFG = *ptCFG;
    
    if (NULL == this.tCFG.Dependency.evtOnLowLevelRendering.fnHandler) {
        return ARM_2D_ERR_MISSING_PARAM;
    }
    
    this.Adapter.wPFBPixelCount 
        = this.tCFG.FrameBuffer.tFrameSize.iWidth
        * this.tCFG.FrameBuffer.tFrameSize.iHeight;

    /* make sure the width of the pfb fulfill the pixel-width-alignment 
     * requirement 
     */
    do {
        int_fast8_t chPixelWidthAlignMask 
                = (1 << this.tCFG.FrameBuffer.u3PixelWidthAlign) - 1;
        uint32_t wTotalPixelCount = this.Adapter.wPFBPixelCount;
        int_fast8_t chPixelHeightAlignMask 
                = (1 << this.tCFG.FrameBuffer.u3PixelHeightAlign) - 1;

        if ((0 == chPixelWidthAlignMask) && (0 == chPixelHeightAlignMask)) {
            break;
        }

        /* Align both Width and Height */
        if (chPixelWidthAlignMask && chPixelHeightAlignMask) {

            int_fast16_t iWidth 
                = this.tCFG.FrameBuffer.tFrameSize.iWidth & ~chPixelWidthAlignMask;
            int_fast16_t iHeight 
                = this.tCFG.FrameBuffer.tFrameSize.iHeight & ~chPixelHeightAlignMask;

            if (0 == iWidth) {
                int_fast16_t iWidthAlign = (chPixelWidthAlignMask + 1);
                if (wTotalPixelCount < iWidthAlign) {
                    return ARM_2D_ERR_INSUFFICIENT_RESOURCE;
                }
                iWidth = iWidthAlign;
            }

            if (0 == iHeight) {
                int_fast16_t iHeightAlign = (chPixelHeightAlignMask + 1);
                if (wTotalPixelCount < iHeightAlign) {
                    return ARM_2D_ERR_INSUFFICIENT_RESOURCE;
                }
                iHeight = iHeightAlign;
            }

            if (iWidth * iHeight > wTotalPixelCount) {
                return ARM_2D_ERR_INSUFFICIENT_RESOURCE;
            }

            /* update frame size */
            this.tCFG.FrameBuffer.tFrameSize.iWidth = iWidth;
            this.tCFG.FrameBuffer.tFrameSize.iHeight = iHeight;

        /* Align Width Only */
        } else if (chPixelWidthAlignMask) {

            int_fast16_t iWidth 
                = this.tCFG.FrameBuffer.tFrameSize.iWidth & ~chPixelWidthAlignMask;
            
            if (0 == iWidth) {
                int_fast16_t iWidthAlign = (chPixelWidthAlignMask + 1);
                if (wTotalPixelCount < iWidthAlign) {
                    return ARM_2D_ERR_INSUFFICIENT_RESOURCE;
                }
                iWidth = iWidthAlign;
            }

            /* update frame size */
            this.tCFG.FrameBuffer.tFrameSize.iWidth = iWidth;
            this.tCFG.FrameBuffer.tFrameSize.iHeight 
                = wTotalPixelCount 
                / this.tCFG.FrameBuffer.tFrameSize.iWidth;

        /* Align Height Only */
        } else if (chPixelHeightAlignMask) {
            
            int_fast16_t iHeight 
                = this.tCFG.FrameBuffer.tFrameSize.iHeight & ~chPixelHeightAlignMask;

            if (0 == iHeight) {
                int_fast16_t iHeightAlign = (chPixelHeightAlignMask + 1);
                if (wTotalPixelCount < iHeightAlign) {
                    return ARM_2D_ERR_INSUFFICIENT_RESOURCE;
                }
                iHeight = iHeightAlign;
            }

            /* update frame size */
            this.tCFG.FrameBuffer.tFrameSize.iHeight = iHeight;
            this.tCFG.FrameBuffer.tFrameSize.iWidth 
                = wTotalPixelCount 
                / this.tCFG.FrameBuffer.tFrameSize.iHeight;
        }
    } while(0);

    if (NULL == this.tCFG.Dependency.evtOnLowLevelSyncUp.fnHandler) {
        /* use default semaphore */
        this.Adapter.pFPBPoolAvailable = arm_2d_port_new_semaphore();
    }

    /* initialize internal dirty region pool*/
    do {
        uint_fast8_t chCount = this.tCFG.DirtyRegion.chCount;
        arm_2d_region_list_item_t *ptRegion = this.tCFG.DirtyRegion.ptRegions;
        if (    (0 == chCount) ||  (NULL == ptRegion)) {
            this.Adapter.bIsDirtyRegionOptimizationEnabled = false;
            break;
        }

        do {
            memset(ptRegion, 0, sizeof(arm_2d_region_list_item_t) );
            ptRegion->bFromInternalPool = true;

            __arm_2d_helper_dirty_region_pool_free(ptThis, ptRegion);
            ptRegion++;
        } while(--chCount);

        this.Adapter.bIsDirtyRegionOptimizationEnabled = true;
    } while(0);

    // perform validation
    do {
        int_fast16_t n = this.tCFG.FrameBuffer.hwPFBNum;
        arm_2d_pfb_t *ptItem = this.tCFG.FrameBuffer.ptPFBs;
        uint32_t wBufferSize = this.tCFG.FrameBuffer.wBufferSize;
        
        // handle alignments
        wBufferSize += __alignof__(arm_2d_pfb_t) - 1;
        wBufferSize &= ~(__alignof__(arm_2d_pfb_t) - 1);
        
        if (0 == n || NULL == ptItem) {
            return ARM_2D_ERR_MISSING_PARAM;
        } else if ( (0 == this.tCFG.FrameBuffer.tFrameSize.iHeight)
                 || (0 == this.tCFG.FrameBuffer.tFrameSize.iWidth)
                 || (0 == this.tCFG.FrameBuffer.wBufferSize)) {
            return ARM_2D_ERR_INVALID_PARAM;
        }
        
        this.Adapter.chFreePFBCount = 0;
        // add PFBs to pool
        do {
            ptItem->tTile = (arm_2d_tile_t) {
                .tRegion = {
                    .tSize = this.tCFG.FrameBuffer.tFrameSize,
                },
                .tInfo.bIsRoot = true,
                .pchBuffer = (uint8_t *)((uintptr_t)ptItem + sizeof(arm_2d_pfb_t)),
            };
            
            // update buffer size
            ptItem->u24Size = wBufferSize;

            __arm_2d_helper_pfb_free(ptThis, ptItem);
            
            // update pointer
            ptItem = (arm_2d_pfb_t *)(  (uintptr_t)ptItem 
                                     +  wBufferSize
                                     +  sizeof(arm_2d_pfb_t));
        } while(--n);
    
    } while(0);

    this.Adapter.bFirstIteration = true;
    
    return ARM_2D_ERR_NONE;
}


ARM_NONNULL(1)
void arm_2d_helper_pfb_deinit(arm_2d_helper_pfb_t *ptThis) 
{
    assert(NULL != ptThis);
    arm_2d_port_free_semaphore(this.Adapter.pFPBPoolAvailable);
}

ARM_NONNULL(1)
arm_2d_region_t arm_2d_helper_pfb_get_display_area(arm_2d_helper_pfb_t *ptThis)
{
    assert(NULL != ptThis);
    
    return this.tCFG.tDisplayArea;
}

ARM_NONNULL(1)
arm_2d_size_t arm_2d_helper_pfb_get_pfb_size(arm_2d_helper_pfb_t *ptThis)
{
    assert(NULL != ptThis);
    
    return this.tCFG.FrameBuffer.tFrameSize;
}

ARM_NONNULL(1)
arm_2d_location_t arm_2d_helper_pfb_get_absolute_location(  
                                                    arm_2d_tile_t *ptTile, 
                                                    arm_2d_location_t tLocation)
{
    assert(NULL != ptTile);
    
    while( !ptTile->tInfo.bIsRoot && !ptTile->tInfo.bVirtualScreen ) {
        tLocation.iX += ptTile->tRegion.tLocation.iX;
        tLocation.iY += ptTile->tRegion.tLocation.iY;
        
        ptTile = ptTile->ptParent;

        assert(NULL != ptTile);
    }
    
    return tLocation;
}

__WEAK 
void arm_2d_helper_swap_rgb16(uint16_t *phwBuffer, uint32_t wCount)
{
    assert(NULL != phwBuffer);

    if (0 == wCount) {
        return ;
    }

    // aligned (2)
    assert((((uintptr_t) phwBuffer) & 0x01) == 0);

    // it is not aligned to 4
    if ((((uintptr_t) phwBuffer) & 0x03) == 0x02) {
        // handle the leading pixel
        uint32_t wTemp = *phwBuffer;
        *phwBuffer++ = (uint16_t)__REV16(wTemp);
        wCount--;
    }


    uint32_t wWords = wCount >> 1;
    uint32_t *pwBuffer = (uint32_t *)phwBuffer;
    wCount &= 0x01;

    if (wWords > 0) {
        do {
            uint32_t wTemp = *pwBuffer;
            *pwBuffer++ = __REV16(wTemp);
        } while(--wWords);
    }

    if (wCount) {
        uint32_t wTemp = *pwBuffer;
        (*(uint16_t *)pwBuffer) = (uint16_t)__REV16(wTemp);
    }
}

ARM_NONNULL(1)
void arm_2d_helper_ignore_low_level_flush(arm_2d_helper_pfb_t *ptThis)
{
    assert(NULL != ptThis);
    this.Adapter.bIgnoreLowLevelFlush = true;
}

ARM_NONNULL(1)
void arm_2d_helper_resume_low_level_flush(arm_2d_helper_pfb_t *ptThis)
{
    assert(NULL != ptThis);
    this.Adapter.bIgnoreLowLevelFlush = false;
}


ARM_NONNULL(1)
void arm_2d_helper_hide_navigation_layer(arm_2d_helper_pfb_t *ptThis)
{
    assert(NULL != ptThis);
    this.Adapter.bHideNavigationLayer = true;
}

ARM_NONNULL(1)
void arm_2d_helper_show_navigation_layer(arm_2d_helper_pfb_t *ptThis)
{
    assert(NULL != ptThis);
    this.Adapter.bHideNavigationLayer = false;
}

ARM_NONNULL(1)
void arm_2d_helper_pfb_flush(arm_2d_helper_pfb_t *ptThis)
{
    assert(NULL != ptThis);

    arm_2d_pfb_t *ptPFB = NULL;
    
    /* atomic access of this.Adapter.pfFlushing */
    arm_irq_safe {
        if (NULL != this.Adapter.ptFlushing) {
            arm_exit_irq_safe;
        }

        ARM_LIST_QUEUE_DEQUEUE( this.Adapter.FlushFIFO.ptHead, 
                                this.Adapter.FlushFIFO.ptTail, 
                                ptPFB);

        if (NULL != ptPFB) {
            this.Adapter.ptFlushing = ptPFB;
            ptPFB->ptPFBHelper = ptThis;
        }
    }

    if (NULL != ptPFB) {
        if (    (NULL == this.tCFG.Dependency.evtOnLowLevelRendering.fnHandler)
           ||   this.Adapter.bIgnoreLowLevelFlush) {
           __arm_2d_helper_pfb_report_rendering_complete(ptThis, ptPFB);
        } else {
            // call handler
            (*this.tCFG.Dependency.evtOnLowLevelRendering.fnHandler)(
                            this.tCFG.Dependency.evtOnLowLevelRendering.pTarget,
                            ptPFB,
                            ptPFB->bIsNewFrame);
        }
    }
}

static 
void __arm_2d_helper_enqueue_pfb(arm_2d_helper_pfb_t *ptThis)
{
    arm_irq_safe {
        ARM_LIST_QUEUE_ENQUEUE( this.Adapter.FlushFIFO.ptHead, 
                                this.Adapter.FlushFIFO.ptTail, 
                                this.Adapter.ptCurrent);
    }
    
    arm_2d_helper_pfb_flush(ptThis);
}

ARM_NONNULL(1)
void __arm_2d_helper_pfb_report_rendering_complete( arm_2d_helper_pfb_t *ptThis,
                                                    arm_2d_pfb_t *ptPFB)
{
    assert(NULL != ptThis);

    /* note: in fact, user can only pass either NULL or this.Adapter.ptFlushing
     *       to the ptPFB. This makes ptPFB useless. We only keep it for 
     *       backward compatible.
     */

    arm_irq_safe {
        if (NULL == ptPFB) {
            ptPFB = this.Adapter.ptFlushing;
            this.Adapter.ptFlushing = NULL;
        } else if (ptPFB == this.Adapter.ptFlushing) {
            this.Adapter.ptFlushing = NULL;
        } else {
            /* this should not happen */
            assert(false);
        }
    }

    if (NULL == ptPFB) {
        assert(false);  /* this should not happen */
        return ;
    }

    ptPFB->tTile.tRegion.tLocation = (arm_2d_location_t) {0,0};
    
    __arm_2d_helper_pfb_free(ptThis, ptPFB);
    
    arm_2d_helper_pfb_flush(ptThis);
}

static
void __arm_2d_helper_low_level_rendering(arm_2d_helper_pfb_t *ptThis)
{

    assert(NULL != this.tCFG.Dependency.evtOnLowLevelRendering.fnHandler);
    assert(NULL != this.Adapter.ptCurrent);
    
    // update location info
    this.Adapter.ptCurrent->tTile.tRegion.tLocation = (arm_2d_location_t) {
        .iX = this.Adapter.tDrawRegion.tLocation.iX
            + this.Adapter.tTargetRegion.tLocation.iX,
        .iY = this.Adapter.tDrawRegion.tLocation.iY
            + this.Adapter.tTargetRegion.tLocation.iY,
    };

    if (this.tCFG.FrameBuffer.bSwapRGB16) {
        arm_2d_helper_swap_rgb16( this.Adapter.ptCurrent->tTile.phwBuffer, 
                                    get_tile_buffer_pixel_count(
                                        this.Adapter.ptCurrent->tTile));
    }

    __arm_2d_helper_enqueue_pfb(ptThis);

    this.Adapter.bFirstIteration = false;

}


__STATIC_INLINE 
bool __when_dirty_region_list_is_empty(arm_2d_helper_pfb_t *ptThis)
{
    /* check whether has already been switched to the navigation dirty 
     * region list 
     */
    if (this.Adapter.bNoAdditionalDirtyRegionList) {
        // no dirty region is available
        this.Adapter.bFirstIteration = true;
        
        return false;
    } else if ( (NULL != this.tCFG.Dependency.Navigation.evtOnDrawing.fnHandler)
           &&   (NULL != this.tCFG.Dependency.Navigation.ptDirtyRegion)
           &&   (!this.Adapter.bHideNavigationLayer)) {
        
        /* switch to navigation dirty region list */
        this.Adapter.ptDirtyRegion = this.tCFG.Dependency.Navigation.ptDirtyRegion;
        this.Adapter.bNoAdditionalDirtyRegionList = true;
        
        this.Adapter.bIsRegionChanged = true;
        return true;
    } else {
        // no dirty region is available
        this.Adapter.bFirstIteration = true;
        
        return false;
    }
}

static bool __arm_2d_helper_pfb_get_next_dirty_region(arm_2d_helper_pfb_t *ptThis)
{
#if 1
    if (this.Adapter.bIsUsingOptimizedDirtyRegionList) {
        if (NULL == this.Adapter.OptimizedDirtyRegions.ptWorkingList) {
            /* finished or empty */
            this.Adapter.bIsUsingOptimizedDirtyRegionList = false;
            this.Adapter.ptDirtyRegion = this.Adapter.OptimizedDirtyRegions.ptOriginalList;
        } else {
            this.Adapter.ptDirtyRegion
                = this.Adapter.OptimizedDirtyRegions.ptWorkingList;
            

            this.Adapter.bIsRegionChanged = true;
            return true;
        }
    }
#endif

    if (NULL == this.Adapter.ptDirtyRegion) {
        return __when_dirty_region_list_is_empty(ptThis);
    } 
    
    if (this.Adapter.ptDirtyRegion->bUpdated) {
        /* refresh this dirty region item again */
        this.Adapter.bIsRegionChanged = true;
    } else {
        this.Adapter.ptDirtyRegion = this.Adapter.ptDirtyRegion->ptNext;
        
        if (NULL == this.Adapter.ptDirtyRegion) {
            // reach last item in a chain
            return __when_dirty_region_list_is_empty(ptThis);
        } else {
            this.Adapter.bIsRegionChanged = true;
        }
    }
    
    return true;
}

/*----------------------------------------------------------------------------*
 * Optimized Dirty Regions                                                    *
 *----------------------------------------------------------------------------*/
ARM_NONNULL(1)
static void __arm_2d_helper_dirty_region_pool_free(
                                            arm_2d_helper_pfb_t *ptThis,
                                            arm_2d_region_list_item_t *ptItem)
{
    assert(NULL != ptThis);
    if (NULL == ptItem) {
        return ;
    } else if (!ptItem->bFromInternalPool) {
        return ;
    }

    arm_irq_safe {
        /* PUSH item to the pool in STACK-style */
        ptItem->ptInternalNext = this.Adapter.OptimizedDirtyRegions.ptFreeList;
        this.Adapter.OptimizedDirtyRegions.ptFreeList = ptItem;
    }
}

ARM_NONNULL(1)
static 
arm_2d_region_list_item_t *__arm_2d_helper_dirty_region_pool_new(
                                            arm_2d_helper_pfb_t *ptThis)
{
    assert(NULL != ptThis);

    arm_2d_region_list_item_t *ptItem = NULL;

    arm_irq_safe {
        ptItem = this.Adapter.OptimizedDirtyRegions.ptFreeList;
        if (NULL != ptItem) {
            /* POP a dirty region item from the free list in STACK-style */
            this.Adapter.OptimizedDirtyRegions.ptFreeList = ptItem->ptInternalNext;
        }
    }

    if (NULL != ptItem) {
        memset(ptItem, 0, sizeof(arm_2d_region_list_item_t));
        ptItem->bFromInternalPool = true;
    }

    return ptItem;
}

ARM_NONNULL(1)
static 
void __arm_2d_helper_free_dirty_region_working_list(arm_2d_helper_pfb_t *ptThis, 
                                                    arm_2d_region_list_item_t *ptList)
{
    assert(NULL != ptThis);

    /* free the optimized dirty region list*/
    if (NULL != ptList) {
        
        do {
            arm_2d_region_list_item_t *ptNextItem = ptList->ptInternalNext;
            __arm_2d_helper_dirty_region_pool_free(ptThis, ptList);

            ptList = ptNextItem;
        } while(NULL != ptList);

        this.Adapter.OptimizedDirtyRegions.ptWorkingList = NULL;
    }
}

ARM_NONNULL(1,2)
static 
void __arm_2d_helper_add_item_to_weighted_dirty_region_list(  
                                                arm_2d_region_list_item_t **pptList,
                                                arm_2d_region_list_item_t *ptItem)
{
    assert(NULL != pptList);
    assert(NULL != ptItem);

    if (NULL != *pptList) {
        uint32_t wItemPixelCount = ptItem->tRegion.tSize.iHeight 
                                 * ptItem->tRegion.tSize.iWidth;

        do {
            arm_2d_region_list_item_t *ptItemInList = (*pptList);
            uint32_t wPixelCount = ptItemInList->tRegion.tSize.iHeight 
                                * ptItemInList->tRegion.tSize.iWidth;
            
            if (wPixelCount <= wItemPixelCount) {
                /* add item to the list */
                ptItem->ptInternalNext = (*pptList);
                *pptList = ptItem;
                return;
            }

            pptList = &(ptItemInList->ptInternalNext);
        } while(NULL != (*pptList));
    }

    /* add item to the list */
    ptItem->ptInternalNext = (*pptList);
    *pptList = ptItem;
}

ARM_NONNULL(1,2)
static 
arm_2d_region_list_item_t * __arm_2d_helper_remove_item_from_weighted_dirty_region_list(  
                                                arm_2d_region_list_item_t **pptList,
                                                arm_2d_region_list_item_t *ptItem)
{
    assert(NULL != pptList);
    assert(NULL != ptItem);

    while(NULL != (*pptList)) {

        /* find item */
        if ((*pptList) == ptItem) {
            (*pptList) = ptItem->ptInternalNext;
            ptItem->ptInternalNext = NULL;
            break ;
        }

        pptList = &((*pptList)->ptInternalNext);
    }

    return ptItem;
}


ARM_NONNULL(1,2)
static 
void __arm_2d_helper_update_dirty_region_working_list(  
                                                arm_2d_helper_pfb_t *ptThis, 
                                                arm_2d_region_list_item_t *ptItem,
                                                bool bEncounterDynamicDirtyRegion)
{
    assert(NULL != ptThis);
    assert(NULL != ptItem);

    do {
        arm_2d_region_list_item_t *ptDirtyRegion = ptItem;

        if (bEncounterDynamicDirtyRegion) {
            ptDirtyRegion = __arm_2d_helper_dirty_region_pool_new(ptThis);
            if (NULL == ptDirtyRegion) {
                this.Adapter.bFailedToOptimizeDirtyRegion = true;
            } else {
                ptDirtyRegion->tRegion = ptItem->tRegion;
            }
        }

        if (this.Adapter.bFailedToOptimizeDirtyRegion) {
            __arm_2d_helper_dirty_region_pool_free(ptThis, ptDirtyRegion);
            return ;
        }

        /* add dirty region to the candidate list */
        __arm_2d_helper_add_item_to_weighted_dirty_region_list(
            &this.Adapter.OptimizedDirtyRegions.ptCandidateList,
            ptDirtyRegion
        );
    } while(0);

label_start_process_candidate:
    /* the process loop */
    do {
        arm_2d_region_list_item_t *ptCandidate 
            = this.Adapter.OptimizedDirtyRegions.ptCandidateList;

        while(NULL != ptCandidate) {
            arm_2d_region_list_item_t *ptNextCandiate = ptCandidate->ptInternalNext;
            arm_2d_region_list_item_t *ptWorking
                = this.Adapter.OptimizedDirtyRegions.ptWorkingList;

            uint32_t wCandidatePixelCount = ptCandidate->tRegion.tSize.iHeight
                                            * ptCandidate->tRegion.tSize.iWidth;
            while(NULL != ptWorking) {
                arm_2d_region_list_item_t *ptNextWorking = ptWorking->ptInternalNext;
                arm_2d_region_t tOverlapArea, tEnclosureArea;

                int_fast8_t chResult = arm_2d_is_region_inside_target(&ptCandidate->tRegion,
                                                                      &ptWorking->tRegion);

                if (1 == chResult) {
                    /* the candidate is inside the working region,
                     * remove the candidate from the list 
                     */
                    __arm_2d_helper_remove_item_from_weighted_dirty_region_list(
                        &this.Adapter.OptimizedDirtyRegions.ptCandidateList,
                        ptCandidate
                    );
                    /* free the working item */
                    __arm_2d_helper_dirty_region_pool_free(ptThis, ptCandidate);

                    goto label_move_to_next_candidate;
                } else if (-1 == chResult) {
                    /* remove the working item from the list */
                    __arm_2d_helper_remove_item_from_weighted_dirty_region_list(
                        &this.Adapter.OptimizedDirtyRegions.ptWorkingList,
                        ptWorking
                    );

                    /* free the working item */
                    __arm_2d_helper_dirty_region_pool_free(ptThis, ptWorking);
                    
                } else if (arm_2d_region_intersect(&ptWorking->tRegion, 
                                            &ptCandidate->tRegion,
                                            &tOverlapArea)) {
                    /* has overlap */
                    arm_2d_region_get_minimal_enclosure(&ptWorking->tRegion, 
                                                        &ptCandidate->tRegion,
                                                        &tEnclosureArea);
                    uint32_t wWorkingItemPixelCount 
                                        = ptWorking->tRegion.tSize.iWidth
                                        * ptWorking->tRegion.tSize.iHeight;

                    uint32_t wPixelsRegionEnclosure = tEnclosureArea.tSize.iHeight
                                                    * tEnclosureArea.tSize.iWidth;

                    if (wPixelsRegionEnclosure < wCandidatePixelCount + wWorkingItemPixelCount) {
                        /* we only refresh the enclosure region to save time */
                        
                        /* remove the working item from the list */
                        __arm_2d_helper_remove_item_from_weighted_dirty_region_list(
                            &this.Adapter.OptimizedDirtyRegions.ptWorkingList,
                            ptWorking
                        );
                        /* free the working item */
                        __arm_2d_helper_dirty_region_pool_free(ptThis, ptWorking);

                        /* remove the candidate from the list */
                        __arm_2d_helper_remove_item_from_weighted_dirty_region_list(
                            &this.Adapter.OptimizedDirtyRegions.ptCandidateList,
                            ptCandidate
                        );
                        /* free the working item */
                        __arm_2d_helper_dirty_region_pool_free(ptThis, ptCandidate);

                        arm_2d_region_list_item_t *ptDirtyRegion = __arm_2d_helper_dirty_region_pool_new(ptThis);
                        if (NULL == ptDirtyRegion) {
                            this.Adapter.bFailedToOptimizeDirtyRegion = true;
                            return ;
                        }
                        /* initialize the newly allocated dirty region item */
                        ptDirtyRegion->tRegion = tEnclosureArea;

                        /* update candidate list */
                        __arm_2d_helper_add_item_to_weighted_dirty_region_list(
                            &this.Adapter.OptimizedDirtyRegions.ptCandidateList,
                            ptDirtyRegion
                        );

                        /* re-start processing candidate list from the beginning */
                        goto label_start_process_candidate;
                    }

                    /* todo: try to get residual */
                    
                }
                /* no overlap */
                ptWorking = ptNextWorking;  /* get the next dirty region in the working list */
            }

            /* default: no way to optimize the candidate,
             *          add the candidate to the working list
             */

            /* remove the candidate from the list */
            __arm_2d_helper_remove_item_from_weighted_dirty_region_list(
                &this.Adapter.OptimizedDirtyRegions.ptCandidateList,
                ptCandidate
            );

            /* add the candidate to the working list */
            __arm_2d_helper_add_item_to_weighted_dirty_region_list(
                &this.Adapter.OptimizedDirtyRegions.ptWorkingList,
                ptCandidate
            );

label_move_to_next_candidate:
            /* process next candidate */
            ptCandidate = ptNextCandiate;
        }

        if (NULL == ptCandidate) {
            break;  /* all candidates are processed */
        }
    } while(true);

}


/*! \brief begin a iteration of drawing and request a frame buffer from 
 *         low level display driver.
 *  \param[in] ptThis the PFB helper control block
 *  \param[in] ptTargetRegion the address of the target region in the LCD
 *         passing NULL means we want to draw the whole LCD.
 *  \retval NULL the display driver is not ready
 *  \retval (intptr_t)-1 the display driver want to ignore this drawing 
 *          (maybe because the target area is out of the LCD)
 *  \retval non-null a tile which contains the (partial) frame buffer
 */
static 
arm_2d_tile_t * __arm_2d_helper_pfb_drawing_iteration_begin(
                                    arm_2d_helper_pfb_t *ptThis,
                                    arm_2d_region_list_item_t *ptDirtyRegions)
{ 
    if (this.Adapter.bIsDryRun) {
        /* 
         * NOTE: If this is a dry run, no need to allocate PFB again.
         */
        
        if (!this.Adapter.bIsDirtyRegionOptimizationEnabled) {

            this.Adapter.bIsDryRun = false;
            /* refresh the first dirty region as people might update dirty region list 
            * in the dry run
            */
            if (NULL == this.Adapter.ptDirtyRegion) {
                this.Adapter.bIsRegionChanged = true;   
            } else if (this.Adapter.ptDirtyRegion->bUpdated) {
                this.Adapter.bIsRegionChanged = true;
            }

        } else {
            if (NULL == this.Adapter.ptDirtyRegion) {
                /* dry run is finished */
                this.Adapter.bIsRegionChanged = true; 
            }
        }

    } else {
        this.Adapter.ptCurrent = NULL;
        arm_irq_safe {
            /* allocating pfb only when the number of free PFB blocks is larger than
            * the reserved threashold
            */
            if (this.Adapter.chFreePFBCount > this.tCFG.FrameBuffer.u4PoolReserve) {
                this.Adapter.ptCurrent = __arm_2d_helper_pfb_new(ptThis);
            }
        }
        
        if (NULL == this.Adapter.ptCurrent) {
            // no resource left
            return NULL;
        }
        this.Adapter.ptCurrent->bIsNewFrame = this.Adapter.bFirstIteration;
    }

    arm_2d_tile_t *ptPartialFrameBuffer = &(this.Adapter.ptCurrent->tTile);

    if (this.Adapter.bFirstIteration) {

        /* NOTE: this only works for the optimized-dirty-region services */
        if (this.Adapter.bIsDryRun && this.tCFG.FrameBuffer.bDebugDirtyRegions) {
            /* in dirty region debug mode, we will refresh the whole screen
             * instead of the dirty regions to show the optimized dirty regions
             * correctly.
             */
            this.Adapter.bIsDryRun = false;             /* clear the DryRun flag */

            /* NOTE: due to the dry run, it is not the first iteration of a frame 
             *       but this flag is set to true when all dirty regions are visited, 
             *       so we have to clear it manually.
             * */
            this.Adapter.bFirstIteration = false;       
            this.Adapter.ptDirtyRegion = NULL;          /* refresh the whole screen */
        } else {
            this.Adapter.ptDirtyRegion = ptDirtyRegions;
        }
        
        if (NULL == this.Adapter.ptDirtyRegion) {
            /* since we draw the whole frame, no need to take the additional 
             * dirty region list into consideration 
             */
            this.Adapter.bNoAdditionalDirtyRegionList = true;
        } else {
            /* reset flag */
            this.Adapter.bNoAdditionalDirtyRegionList = false;
        }

        this.Adapter.bIsRegionChanged = true;
    }

    do {
        if (this.Adapter.bIsRegionChanged) {
            this.Adapter.bIsRegionChanged = false;

            if (NULL != this.Adapter.ptDirtyRegion) {
                this.Adapter.bEncounterDynamicDirtyRegion = false;

                if (this.Adapter.ptDirtyRegion->bUpdated) {
                    /* clear the region updated flag
                     * NOTE: Please never set bUpdate of a const dirty region
                     *       item to true
                     */
                    this.Adapter.ptDirtyRegion->bUpdated = false;
                    this.Adapter.bEncounterDynamicDirtyRegion = true;
                }

                // calculate the valid region
                if (    (this.Adapter.ptDirtyRegion->bIgnore)
                    ||  (!arm_2d_region_intersect(
                                &this.tCFG.tDisplayArea, 
                                &(this.Adapter.ptDirtyRegion->tRegion), 
                                &this.Adapter.tTargetRegion))) {

                    /*----------------------------------------------------------------*
                     * the code segment for try next dirty region or end early: BEGIN *
                     *----------------------------------------------------------------*/
                    if (__arm_2d_helper_pfb_get_next_dirty_region(ptThis)) {
                        /* note, this function updates the 
                         * this.Adapter.bIsRegionChanged flag
                         */

                        continue;    //!< try next region
                    }

                    /* free pfb */
                    arm_irq_safe {
                        __arm_2d_helper_pfb_free(ptThis, this.Adapter.ptCurrent);
                        this.Adapter.ptCurrent = NULL;
                    }

                    // out of lcd 
                    return (arm_2d_tile_t *)-1;
                    /*--------------------------------------------------------------*
                     * the code segment for try next dirty region or end early: END *
                     *--------------------------------------------------------------*/
                }

            } else {
                this.Adapter.tTargetRegion = this.tCFG.tDisplayArea;
            }

            /* update this.Adapter.tTargetRegion to fulfill the pixel width
             * alignment request 
             */
            if (this.tCFG.FrameBuffer.u3PixelWidthAlign) {
                uint_fast8_t chPixelWidthAlignMask 
                    = (1 << this.tCFG.FrameBuffer.u3PixelWidthAlign)-1;
                
                int_fast16_t iX = this.Adapter.tTargetRegion.tLocation.iX;
                this.Adapter.tTargetRegion.tLocation.iX 
                    &= ~chPixelWidthAlignMask;
                
                this.Adapter.tTargetRegion.tSize.iWidth 
                    += iX - this.Adapter.tTargetRegion.tLocation.iX;
                this.Adapter.tTargetRegion.tSize.iWidth 
                    = ( this.Adapter.tTargetRegion.tSize.iWidth 
                      + chPixelWidthAlignMask) 
                    & ~chPixelWidthAlignMask;
            }

            /* update this.Adapter.tTargetRegion to fulfill the pixel height
             * alignment request 
             */
            if (this.tCFG.FrameBuffer.u3PixelHeightAlign) {
                uint_fast8_t chPixelHeightAlignMask 
                    = (1 << this.tCFG.FrameBuffer.u3PixelHeightAlign)-1;
                
                int_fast16_t iY = this.Adapter.tTargetRegion.tLocation.iY;
                this.Adapter.tTargetRegion.tLocation.iY 
                    &= ~chPixelHeightAlignMask;
                
                this.Adapter.tTargetRegion.tSize.iHeight 
                    += iY - this.Adapter.tTargetRegion.tLocation.iY;
                this.Adapter.tTargetRegion.tSize.iHeight
                    = ( this.Adapter.tTargetRegion.tSize.iHeight 
                      + chPixelHeightAlignMask) 
                    & ~chPixelHeightAlignMask;
            }

            if (this.tCFG.FrameBuffer.bDisableDynamicFPBSize) {
                // reset adapter frame size
                this.Adapter.tFrameSize = this.tCFG.FrameBuffer.tFrameSize;

            } else {                                                            //!< update PFB size
                uint32_t wTargetPixelCount 
                    = this.Adapter.tTargetRegion.tSize.iWidth
                    * this.Adapter.tTargetRegion.tSize.iHeight;
                
                uint32_t wPFBPixelCount = this.Adapter.wPFBPixelCount;
                        
                if (    (wTargetPixelCount <= wPFBPixelCount)
                   ||   (   this.Adapter.tTargetRegion.tSize.iWidth 
                        <   wPFBPixelCount)) {
                    // redefine the shape of PFB
                    
                    this.Adapter.tFrameSize.iWidth 
                        = this.Adapter.tTargetRegion.tSize.iWidth;
                    
                    int16_t iHeight = (int16_t)
                        (   wPFBPixelCount 
                        /   (uint32_t)this.Adapter.tTargetRegion.tSize.iWidth);
                
                    iHeight = MIN(iHeight, this.Adapter.tTargetRegion.tSize.iHeight);

                    if (this.tCFG.FrameBuffer.u3PixelHeightAlign) {
                        uint_fast8_t chPixelHeightAlignMask 
                            = (1 << this.tCFG.FrameBuffer.u3PixelHeightAlign)-1;

                        
                        iHeight &= ~chPixelHeightAlignMask;

                        if (0 == iHeight) {
                            // reset adapter frame size
                            this.Adapter.tFrameSize = this.tCFG.FrameBuffer.tFrameSize;
                        } else {
                            this.Adapter.tFrameSize.iHeight = iHeight;
                        } 

                    } else {
                        this.Adapter.tFrameSize.iHeight = iHeight;
                    }

                } else {
                    // reset adapter frame size
                    this.Adapter.tFrameSize = this.tCFG.FrameBuffer.tFrameSize;
                }
            } 

        }
        break;
    } while(true);
    
    
    arm_2d_region_t tTempRegion = {
        .tSize = this.tCFG.tDisplayArea.tSize,
        .tLocation = {
            .iX = - (   this.Adapter.tTargetRegion.tLocation.iX 
                    +   this.Adapter.tDrawRegion.tLocation.iX),
            .iY = - (   this.Adapter.tTargetRegion.tLocation.iY 
                    +   this.Adapter.tDrawRegion.tLocation.iY),
        },
    };
    
    
    ptPartialFrameBuffer->tRegion.tSize.iWidth 
        = MIN(  this.Adapter.tFrameSize.iWidth, 
                this.Adapter.tTargetRegion.tSize.iWidth 
            -   this.Adapter.tDrawRegion.tLocation.iX);
    ptPartialFrameBuffer->tRegion.tSize.iHeight 
        = MIN(  this.Adapter.tFrameSize.iHeight, 
                this.Adapter.tTargetRegion.tSize.iHeight 
            -   this.Adapter.tDrawRegion.tLocation.iY);
            
    arm_2d_tile_generate_child( ptPartialFrameBuffer, 
                                &tTempRegion, 
                                &this.Adapter.tPFBTile, 
                                false);

    if (this.Adapter.bIsDirtyRegionOptimizationEnabled) {

        /* check whether we need a dry run */
        if (this.Adapter.bFirstIteration && NULL != this.Adapter.ptDirtyRegion) {
            if (!this.Adapter.bIsDryRun) {
                this.Adapter.bIsDryRun = true;
            } else {
                /* starting the real drawing after the dry run */
                this.Adapter.bIsDryRun = false;

                /* NOTE: due to the dry run, it is not the first iteration of a frame 
                *       but this flag is set to true when all dirty regions are visited, 
                *       so we have to clear it manually.
                */
                this.Adapter.bFirstIteration = false;
            }
        }

        if (this.Adapter.bIsDryRun) {
            /* if the dirty region list isn't empty, to support the services that
            * need a dry run to update dirty region (e.g. dynamic dirty regions
            * and transform helper etc), we will do a dry run for the first 
            * iteration. 
            * 
            * To achieve that, we need to modify the this.Adapter.tPFBTile to  
            * ensure that there is no valid region between the root tile and
            * the child tile, i.e draw nothing. 
            */
            this.Adapter.tPFBTile.tRegion.tLocation.iX
                = ptPartialFrameBuffer->tRegion.tSize.iWidth;
            
            assert(NULL != this.Adapter.ptDirtyRegion);
            if (!this.Adapter.bFailedToOptimizeDirtyRegion) {
                __arm_2d_helper_update_dirty_region_working_list(
                                        ptThis, 
                                        this.Adapter.ptDirtyRegion,
                                        this.Adapter.bEncounterDynamicDirtyRegion);
            }

        }
    } else {
        /* check whether we need a dry run */
        if (this.Adapter.bFirstIteration && NULL != this.Adapter.ptDirtyRegion) {
            this.Adapter.bIsDryRun = true;

            /* if the dirty region list isn't empty, to support the services that
            * need a dry run to update dirty region (e.g. dynamic dirty regions
            * and transform helper etc), we will do a dry run for the first 
            * iteration. 
            * 
            * To achieve that, we need to modify the this.Adapter.tPFBTile to  
            * ensure that there is no valid region between the root tile and
            * the child tile, i.e draw nothing. 
            */
            this.Adapter.tPFBTile.tRegion.tLocation.iX
                = ptPartialFrameBuffer->tRegion.tSize.iWidth;
        }
    }

    if (!this.tCFG.FrameBuffer.bDoNOTUpdateDefaultFrameBuffer) {
        // update default frame buffer
        arm_2d_set_default_frame_buffer(&this.Adapter.tPFBTile);
    }

    this.Adapter.tPFBTile.tInfo.bVirtualScreen = true;

    return (arm_2d_tile_t *)&(this.Adapter.tPFBTile);
}



/*! \brief end a drawing iteration and decide wether a new iteration is required
 *         or not based on the return value
 *  \param[in] ptThis the PFB control block
 *  \retval true a new iteration is required
 *  \retval false no more iteration is required
 */
static 
bool __arm_2d_helper_pfb_drawing_iteration_end(arm_2d_helper_pfb_t *ptThis)
{
    if (this.Adapter.bIsDryRun) {
        /* NOTE: if this is a dry run, we don't have to invoke low level rendering
         *       process.
         */

        this.Adapter.bFirstIteration = false;

        if (this.Adapter.bIsDirtyRegionOptimizationEnabled) {
            __arm_2d_helper_pfb_get_next_dirty_region(ptThis);
        }
        return true;
    }

    __arm_2d_helper_low_level_rendering(ptThis);
    
    arm_2d_tile_t *ptPartialFrameBuffer = &(this.Adapter.ptCurrent->tTile);
    
    if (!this.tCFG.FrameBuffer.bDoNOTUpdateDefaultFrameBuffer) {
        // update default frame buffer
        arm_2d_set_default_frame_buffer(NULL);
    }

    this.Adapter.tDrawRegion.tLocation.iX 
        += ptPartialFrameBuffer->tRegion.tSize.iWidth;
    if (    this.Adapter.tDrawRegion.tLocation.iX 
        >=  this.Adapter.tTargetRegion.tSize.iWidth) {
        this.Adapter.tDrawRegion.tLocation.iY 
            += ptPartialFrameBuffer->tRegion.tSize.iHeight;
        this.Adapter.tDrawRegion.tLocation.iX = 0;
        
        if (    this.Adapter.tDrawRegion.tLocation.iY 
            >=  this.Adapter.tTargetRegion.tSize.iHeight) {
            // finished
            this.Adapter.tDrawRegion.tLocation.iY = 0;

            return __arm_2d_helper_pfb_get_next_dirty_region(ptThis);
            
        }
    }

    return true;
}

#if     defined(__PERF_COUNTER__)                                               \
    &&  defined(__PERF_CNT_USE_RTOS__)                                          \
    &&  __PER_COUNTER_VER__ >= 20204ul
static task_cycle_info_t s_tPerfCounters[__ARM_2D_PERFC_COUNT - 1];

__WEAK
void *__arm_2d_helper_perf_counter_get(arm_2d_perfc_type_t tType)
{
    if (ARM_2D_PERFC_RENDER == tType) {
        return &s_tPerfCounters[tType];
    }
    
    return NULL;
}

__WEAK void __arm_2d_helper_perf_counter_start( int64_t *plTimestamp, 
                                                arm_2d_perfc_type_t tType)
{
    assert(NULL != plTimestamp);
    if (ARM_2D_PERFC_RENDER == tType) {
        start_task_cycle_counter(&s_tPerfCounters[tType]);
    } else {
        *plTimestamp = arm_2d_helper_get_system_timestamp();
    }
}

__WEAK int32_t __arm_2d_helper_perf_counter_stop(int64_t *plTimestamp,
                                                 arm_2d_perfc_type_t tType)
{
    ARM_2D_UNUSED(plTimestamp);
    
    assert(NULL != plTimestamp);
    
    if (ARM_2D_PERFC_RENDER == tType) {
        return (int32_t)stop_task_cycle_counter(&s_tPerfCounters[tType]);
    }

    return (int32_t)(   (int64_t)arm_2d_helper_get_system_timestamp() 
                    -   (int64_t)*plTimestamp);
}

#else

__WEAK
void *__arm_2d_helper_perf_counter_get(arm_2d_perfc_type_t tType)
{
    ARM_2D_UNUSED(tType);
    return NULL;
}

__WEAK void __arm_2d_helper_perf_counter_start( int64_t *plTimestamp, 
                                                arm_2d_perfc_type_t tType)
{
    ARM_2D_UNUSED(tType);

    assert(NULL != plTimestamp);
    *plTimestamp = arm_2d_helper_get_system_timestamp();
}

__WEAK int32_t __arm_2d_helper_perf_counter_stop(int64_t *plTimestamp,
                                                 arm_2d_perfc_type_t tType)
{
    ARM_2D_UNUSED(tType);
    
    assert(NULL != plTimestamp);
    return (int32_t)(   (int64_t)arm_2d_helper_get_system_timestamp() 
                    -   (int64_t)*plTimestamp);
}

#endif



ARM_NONNULL(1,3)
arm_2d_err_t arm_2d_helper_pfb_update_dependency(
                            arm_2d_helper_pfb_t *ptThis, 
                            uint_fast8_t chMask,
                            const arm_2d_helper_pfb_dependency_t *ptDependency)
{
    assert(NULL != ptThis);
    assert(NULL != ptDependency);
    
    arm_irq_safe {
        if (chMask & ARM_2D_PFB_DEPEND_ON_LOW_LEVEL_RENDERING) {
            this.tCFG.Dependency.evtOnLowLevelRendering 
                = ptDependency->evtOnLowLevelRendering;
        }

        if (chMask & ARM_2D_PFB_DEPEND_ON_DRAWING) {
            this.tCFG.Dependency.evtOnDrawing 
                = ptDependency->evtOnDrawing;
        }

        if (chMask & ARM_2D_PFB_DEPEND_ON_LOW_LEVEL_SYNC_UP) {
            this.tCFG.Dependency.evtOnLowLevelSyncUp 
                = ptDependency->evtOnLowLevelSyncUp;
        }
        
        if (chMask & ARM_2D_PFB_DEPEND_ON_EACH_FRAME_CPL) {
            this.tCFG.Dependency.evtOnEachFrameCPL
                = ptDependency->evtOnEachFrameCPL;
        }

        if (chMask & ARM_2D_PFB_DEPEND_ON_NAVIGATION) {
            this.tCFG.Dependency.Navigation = ptDependency->Navigation;
        }
    }
    
    return ARM_2D_ERR_NONE;
}



arm_fsm_rt_t arm_2d_helper_pfb_task(arm_2d_helper_pfb_t *ptThis, 
                                    arm_2d_region_list_item_t *ptDirtyRegions) 
{   
    assert(NULL != ptThis);
    assert(NULL != this.tCFG.Dependency.evtOnDrawing.fnHandler);
    arm_fsm_rt_t tResult;

ARM_PT_BEGIN(this.Adapter.chPT)

#if     defined(__PERF_COUNTER__)                                               \
    &&  defined(__PERF_CNT_USE_RTOS__)                                          \
    &&  __PER_COUNTER_VER__ >= 20204ul

    if (!perfc_check_task_stack_canary_safe()) {
        /* init the cycle counter for the current task */
        init_task_cycle_counter();
        
        /* initialize the cross tasks task-cycle-info object */
        init_task_cycle_info(&s_tPerfCounters[ARM_2D_PERFC_RENDER]);
        
        static task_cycle_info_agent_t s_tAgent;
        register_task_cycle_agent(&s_tPerfCounters[ARM_2D_PERFC_RENDER],
                                  &s_tAgent);
    }
#endif

    this.Statistics.nTotalCycle = 0;
    this.Statistics.nRenderingCycle = 0;
    this.Adapter.bIsNewFrame = true;

    /* initialize the OptimizedDirtyRegions service */
    if (this.Adapter.bIsDirtyRegionOptimizationEnabled) {
        this.Adapter.OptimizedDirtyRegions.ptOriginalList = ptDirtyRegions;
        this.Adapter.bIsUsingOptimizedDirtyRegionList = false;              /* this must be false here */
        this.Adapter.bFailedToOptimizeDirtyRegion = false;

        assert(NULL == this.Adapter.OptimizedDirtyRegions.ptWorkingList);
        assert(NULL == this.Adapter.OptimizedDirtyRegions.ptCandidateList);
    }

    __arm_2d_helper_perf_counter_start(&this.Statistics.lTimestamp,
                                       ARM_2D_PERFC_DRIVER); 
    do {
        /* begin of the drawing iteration, 
         * try to request the tile of frame buffer
         */
        do {
        
            /*! \note In deep embedded applications, a LCD usually is connected 
             *       via a serial interface to save pins, hence the bandwidth 
             *       is limited and the FPS is low due to the bandwidth.
             *       To overcome this issue, some partial-flushing schemes are 
             *       used, such as:
             *       - Dirty Region based partial-flushing
             *       - Flush the known and fixed small area that is updated 
             *         frequently based on the application scenarios. 
             *       
             *       It is worth emphasizing that as we are using partial 
             *       flushing scheme, which means for a given frame, we only 
             *       update those changed area(s) but not the complete frame,
             *       using the term frame per sec (FPS) might confuse people,
             *       hence, we decide to introduce a NEW term called update per
             *       sec (UPS) to avoid this confusion. It reflects what people
             *       feel when looking at the LCD but not necessarily means
             *       the rate that a complete frame is flushed into LCD.  
             *       
             *       In Arm-2D:
             *       - FPS is a sub-set of UPS. 
             *       - UPS forcus on how people feel and FPS is sticks to the 
             *         concept of (full) frame per sec.              
             */
        
            // request to draw the whole LCD
            this.Adapter.ptFrameBuffer = 
                __arm_2d_helper_pfb_drawing_iteration_begin( 
                                                        ptThis, 
                                                        ptDirtyRegions);
            if (NULL == this.Adapter.ptFrameBuffer) {
                if (NULL != this.tCFG.Dependency.evtOnLowLevelSyncUp.fnHandler){
                     // wait until lcd is ready
                    (*this.tCFG.Dependency.evtOnLowLevelSyncUp.fnHandler)(
                        this.tCFG.Dependency.evtOnLowLevelSyncUp.pTarget
                    );
                } else if ((uintptr_t)NULL != this.Adapter.pFPBPoolAvailable) {
                    arm_2d_port_wait_for_semaphore(
                                                this.Adapter.pFPBPoolAvailable);
                }
                continue;
            } else if (-1 == (intptr_t)this.Adapter.ptFrameBuffer) {
                /* display driver wants to end the drawing */
                goto label_pfb_task_rt_cpl;
            }
        } while(NULL == this.Adapter.ptFrameBuffer);

        /* LCD Latency includes the time of waiting for a PFB block */
        this.Statistics.nRenderingCycle 
            += __arm_2d_helper_perf_counter_stop(&this.Statistics.lTimestamp,
                                                 ARM_2D_PERFC_DRIVER);

    ARM_PT_ENTRY()
        
        __arm_2d_helper_perf_counter_start( &this.Statistics.lTimestamp,
                                            ARM_2D_PERFC_RENDER); 
        /* draw all the gui elements on target frame buffer */
        tResult = this.tCFG.Dependency.evtOnDrawing.fnHandler(
                                        this.tCFG.Dependency.evtOnDrawing.pTarget,
                                        this.Adapter.ptFrameBuffer,
                                        this.Adapter.bIsNewFrame);

        // just in case some one forgot to do this...
        arm_2d_op_wait_async(NULL);

        this.Statistics.nTotalCycle += 
            __arm_2d_helper_perf_counter_stop(  &this.Statistics.lTimestamp,
                                                ARM_2D_PERFC_RENDER); 

        if (arm_fsm_rt_on_going == tResult) {
    ARM_PT_GOTO_PREV_ENTRY(arm_fsm_rt_on_going)
        } else if (tResult < 0) {
            // error was reported
    ARM_PT_GOTO_PREV_ENTRY(tResult)
        } else if (arm_fsm_rt_wait_for_obj == tResult) {
    ARM_PT_GOTO_PREV_ENTRY(tResult)
        } else { 
    ARM_PT_YIELD(arm_fsm_rt_on_going)
        }

        /* draw navigation layer */
        if (    (NULL != this.tCFG.Dependency.Navigation.evtOnDrawing.fnHandler)
            &&  (!this.Adapter.bHideNavigationLayer)) {
    ARM_PT_ENTRY()
            __arm_2d_helper_perf_counter_start( &this.Statistics.lTimestamp,
                                                ARM_2D_PERFC_RENDER); 

            tResult = this.tCFG.Dependency.Navigation.evtOnDrawing.fnHandler(
                    this.tCFG.Dependency.Navigation.evtOnDrawing.pTarget,
                    this.Adapter.ptFrameBuffer,
                    this.Adapter.bIsNewFrame);
            arm_2d_op_wait_async(NULL);
            
            this.Statistics.nTotalCycle += 
                __arm_2d_helper_perf_counter_stop(
                                                &this.Statistics.lTimestamp,
                                                ARM_2D_PERFC_RENDER); 

        if (arm_fsm_rt_on_going == tResult) {
        ARM_PT_GOTO_PREV_ENTRY(arm_fsm_rt_on_going)
            } else if (tResult < 0) {
                // error was reported
        ARM_PT_GOTO_PREV_ENTRY(tResult)
            } else if (arm_fsm_rt_wait_for_obj == tResult) {
        ARM_PT_GOTO_PREV_ENTRY(tResult)
            } else { 
        ARM_PT_YIELD(arm_fsm_rt_on_going)
            }
        }
        
        /* draw dirty regions */
        if (this.tCFG.FrameBuffer.bDebugDirtyRegions) {
            arm_2d_region_list_item_t *ptRegionListItem = ptDirtyRegions;
            
            while(NULL != ptRegionListItem) {
                if (!ptRegionListItem->bIgnore) {
                    arm_2d_helper_draw_box( this.Adapter.ptFrameBuffer, 
                                            &ptRegionListItem->tRegion, 
                                            1,  
                                            GLCD_COLOR_GREEN, 255);
                }
                
                ptRegionListItem = ptRegionListItem->ptNext;
            }

            if (this.Adapter.bIsDirtyRegionOptimizationEnabled) {
                ptRegionListItem = this.Adapter.OptimizedDirtyRegions.ptWorkingList;

                while(NULL != ptRegionListItem) {
                    if (!ptRegionListItem->bIgnore) {
                        arm_2d_helper_draw_box( this.Adapter.ptFrameBuffer, 
                                                &ptRegionListItem->tRegion, 
                                                1,  
                                                GLCD_COLOR_RED, 255);
                    }
                    
                    ptRegionListItem = ptRegionListItem->ptInternalNext;
                }
            }

        }

        this.Adapter.bIsNewFrame = false;
        __arm_2d_helper_perf_counter_start( &this.Statistics.lTimestamp,
                                            ARM_2D_PERFC_DRIVER); 
    } while(__arm_2d_helper_pfb_drawing_iteration_end(ptThis));
    
label_pfb_task_rt_cpl:    
    this.Statistics.nRenderingCycle 
        += __arm_2d_helper_perf_counter_stop(   &this.Statistics.lTimestamp,
                                                ARM_2D_PERFC_DRIVER); 

ARM_PT_END()

    if (this.Adapter.bIsDirtyRegionOptimizationEnabled) {
        /* free working list */
        __arm_2d_helper_free_dirty_region_working_list(
                                ptThis, 
                                this.Adapter.OptimizedDirtyRegions.ptWorkingList);
        
        /* free candidate list */
        __arm_2d_helper_free_dirty_region_working_list(
                                ptThis, 
                                this.Adapter.OptimizedDirtyRegions.ptCandidateList);
    }

    /* invoke the On Each Frame Complete Event */
    ARM_2D_INVOKE(  this.tCFG.Dependency.evtOnEachFrameCPL.fnHandler,
                    this.tCFG.Dependency.evtOnEachFrameCPL.pTarget);

    return arm_fsm_rt_cpl;
}

/*----------------------------------------------------------------------------*
 * 3FB (Direct Mode) Helper                                                   *
 *----------------------------------------------------------------------------*/

ARM_NONNULL(1)
static
void __arm_2d_helper_3fb_dma_copy(  arm_2d_helper_3fb_t *ptThis,
                                    void *pObj,
                                    uintptr_t pnSource,
                                    uintptr_t pnTarget,
                                    uint32_t nDataItemCount,
                                    uint_fast8_t chDataItemSize)
{
    ARM_2D_UNUSED(pObj);
    
    memcpy((void *)pnTarget, (const void *)pnSource, nDataItemCount * chDataItemSize);

    arm_2d_helper_3fb_report_dma_copy_complete(ptThis);
}

static
bool __arm_2d_helper_3fb_dma_2d_copy(   arm_2d_helper_3fb_t *ptThis, 
                                        void *pObj,
                                        uintptr_t pnSource,
                                        uint32_t wSourceStride,
                                        uintptr_t pnTarget,
                                        uint32_t wTargetStride,
                                        int16_t iWidth,
                                        int16_t iHeight,
                                        uint_fast8_t chBytePerPixel ) 
{
    ARM_2D_UNUSED(ptThis);
    ARM_2D_UNUSED(pObj);
    assert(iWidth * chBytePerPixel <= wSourceStride);
    assert(iWidth * chBytePerPixel <= wTargetStride);

    /* 2D copy */
    for (int_fast16_t i = 0; i < iHeight; i++) {
        memcpy( (void *)pnTarget, (void *)pnSource, iWidth * chBytePerPixel );

        pnSource += wSourceStride;
        pnTarget += wTargetStride;
    }

    return true;
}

ARM_NONNULL(1,2)
void arm_2d_helper_3fb_init(arm_2d_helper_3fb_t *ptThis, 
                            const arm_2d_helper_3fb_cfg_t *ptCFG)
{
    assert(NULL != ptThis);
    assert(NULL != ptCFG);
    assert((uintptr_t)NULL != ptCFG->pnAddress[0]);
    assert((uintptr_t)NULL != ptCFG->pnAddress[1]);
    assert((uintptr_t)NULL != ptCFG->pnAddress[2]);
    assert(0 == (ptCFG->chPixelBits & 0x7));

    memset(ptThis, 0, sizeof(arm_2d_helper_3fb_t));

    this.tCFG = *ptCFG;
    uint32_t wFrameBufferSize = this.tCFG.tScreenSize.iHeight 
                                    * this.tCFG.tScreenSize.iWidth 
                                    * (this.tCFG.chPixelBits >> 3);


    memset((void *)this.tCFG.pnAddress[0], 0, wFrameBufferSize);
    memset((void *)this.tCFG.pnAddress[1], 0, wFrameBufferSize);
    memset((void *)this.tCFG.pnAddress[2], 0, wFrameBufferSize);

    this.Runtime.tState[0] = ARM_3FB_STATE_READY_TO_DRAW;
    this.Runtime.u2ReadyToDraw = 0;

    this.Runtime.tState[1] = ARM_3FB_STATE_DRAWING;
    this.Runtime.u2Drawing = 1;

    this.Runtime.tState[2] = ARM_3FB_STATE_FLUSHING;
    this.Runtime.u2Flushing = 2;

    this.Runtime.u2ReadyToFlush = ARM_2D_3FB_INVALID_IDX;

    this.Runtime.tSemaphore = arm_2d_port_new_semaphore();

    if (NULL == this.tCFG.evtOn2DCopy.fnHandler) {
        this.tCFG.evtOn2DCopy.fnHandler = __arm_2d_helper_3fb_dma_2d_copy;
    }
    if (NULL == this.tCFG.evtOnDMACopy.fnHandler) {
        this.tCFG.evtOnDMACopy.fnHandler = __arm_2d_helper_3fb_dma_copy;
    }
}

ARM_NONNULL(1)
void arm_2d_helper_3fb_report_dma_copy_complete(arm_2d_helper_3fb_t *ptThis)
{
    this.Runtime.bFBCopyComplete = true;
    arm_2d_port_set_semaphore(this.Runtime.tSemaphore);
}



ARM_NONNULL(1)
void * arm_2d_helper_3fb_get_flush_pointer(arm_2d_helper_3fb_t *ptThis)
{
    assert(NULL != ptThis);

    uint_fast8_t chFlushingIndex;
    uint_fast8_t chReadyToFlushIndex;
    uint_fast8_t chReadyToDrawIndex;

    arm_irq_safe {
        chFlushingIndex = this.Runtime.u2Flushing;
        chReadyToFlushIndex = this.Runtime.u2ReadyToFlush;
        chReadyToDrawIndex = this.Runtime.u2ReadyToDraw;
    }

    if (    ARM_2D_3FB_INVALID_IDX != chReadyToFlushIndex
        &&  ARM_2D_3FB_INVALID_IDX == chReadyToDrawIndex) {
        /* a new fb is ready to flush and the ready-to-draw slot is available */
        
        arm_irq_safe {
            this.Runtime.tState[chFlushingIndex]        = ARM_3FB_STATE_READY_TO_DRAW;
            this.Runtime.u2ReadyToDraw = chFlushingIndex;

            this.Runtime.tState[chReadyToFlushIndex]    = ARM_3FB_STATE_FLUSHING;
            this.Runtime.u2Flushing = chReadyToFlushIndex;

            this.Runtime.u2ReadyToFlush = ARM_2D_3FB_INVALID_IDX;
        }
    }

    return (void *)this.tCFG.pnAddress[this.Runtime.u2Flushing];
}

ARM_NONNULL(1)
static uintptr_t 
__arm_2d_helper_3fb_get_drawing_pointer(arm_2d_helper_3fb_t *ptThis, bool bIsNewFrame)
{
    assert(NULL != ptThis);

    uint_fast8_t chDrawingIndex;
    uint_fast8_t chReadyToDrawIndex;


    arm_irq_safe {
        chDrawingIndex = this.Runtime.u2Drawing;
        chReadyToDrawIndex = this.Runtime.u2ReadyToDraw;
    }

    uintptr_t pnAddress = this.tCFG.pnAddress[chDrawingIndex];
    bool bPrepareForCopy = false;

    if (bIsNewFrame) {
        arm_irq_safe {

            /* drawing: no */
            if (ARM_2D_3FB_INVALID_IDX == this.Runtime.u2Drawing) {
                /* ensure ready-to-draw: available */
                assert(ARM_2D_3FB_INVALID_IDX != chReadyToDrawIndex);

                uint_fast8_t chIndex = chReadyToDrawIndex;

                /* update FB status */
                this.Runtime.tState[chIndex] = ARM_3FB_STATE_DRAWING;

                /* update pointer */
                this.Runtime.u2ReadyToDraw = ARM_2D_3FB_INVALID_IDX;
                this.Runtime.u2Drawing = chIndex;

                pnAddress = this.tCFG.pnAddress[chIndex];
                arm_exit_irq_safe;   /* jump out */
            } /* drawing: yes */

            /* check whether Ready-To-Flush is empty */
            if (this.Runtime.u2ReadyToFlush == ARM_2D_3FB_INVALID_IDX) {
                
                /* ensure ready-to-draw: available */
                assert(ARM_2D_3FB_INVALID_IDX != chReadyToDrawIndex);
                this.Runtime.tState[chDrawingIndex] = ARM_3FB_STATE_COPYING_AS_SOURCE;
                this.Runtime.tState[chReadyToDrawIndex] = ARM_3FB_STATE_COPYING_AS_TARGET;
                this.Runtime.u2ReadyToDraw = ARM_2D_3FB_INVALID_IDX;
                this.Runtime.u2Drawing = ARM_2D_3FB_INVALID_IDX;

                bPrepareForCopy = true;
            } 
        }

        if (bPrepareForCopy) {

            this.Runtime.bFBCopyComplete = false;
            __arm_2d_helper_3fb_dma_copy(ptThis, 
                                        this.tCFG.evtOnDMACopy.pObj,
                                        this.tCFG.pnAddress[chDrawingIndex],
                                        this.tCFG.pnAddress[chReadyToDrawIndex],
                                        this.tCFG.tScreenSize.iWidth * this.tCFG.tScreenSize.iHeight,
                                        this.tCFG.chPixelBits >> 3);

            while(true) {
                if (this.Runtime.bFBCopyComplete) {
                    break;
                }
                while(!arm_2d_port_wait_for_semaphore(this.Runtime.tSemaphore));
            }

            /* update state and pointers */
            arm_irq_safe {

                this.Runtime.tState[chDrawingIndex] = ARM_3FB_STATE_READY_TO_FLUSH;
                this.Runtime.u2ReadyToFlush = chDrawingIndex;

                this.Runtime.tState[chReadyToDrawIndex] = ARM_3FB_STATE_DRAWING;
                this.Runtime.u2Drawing = chReadyToDrawIndex;

                pnAddress = this.tCFG.pnAddress[chReadyToDrawIndex];
            }
        }
    }

    return pnAddress;
}



ARM_NONNULL(1,2)
bool __arm_2d_helper_3fb_draw_bitmap( arm_2d_helper_3fb_t *ptThis, 
                                      const arm_2d_pfb_t *ptPFB)
{

    assert(NULL != ptThis);
    assert(NULL != ptPFB);
    assert(0 == (this.tCFG.chPixelBits & 0x07));

    //uint_fast8_t chBytesPerPixel = this.tCFG.chPixelBits >> 3;
    int16_t iLCDWidth = this.tCFG.tScreenSize.iWidth;
    int16_t iLCDHeight = this.tCFG.tScreenSize.iHeight;
    uintptr_t pnAddress = __arm_2d_helper_3fb_get_drawing_pointer(ptThis, ptPFB->bIsNewFrame);

    uint_fast8_t chBytePerPixel = this.tCFG.chPixelBits >> 3;
    uint32_t wLCDStrideInByte = chBytePerPixel * iLCDWidth;

    int16_t iX = ptPFB->tTile.tRegion.tLocation.iX;
    int16_t iY = ptPFB->tTile.tRegion.tLocation.iY;
    int16_t iPFBWidth = ptPFB->tTile.tRegion.tSize.iWidth;
    int16_t iPFBHeight = ptPFB->tTile.tRegion.tSize.iHeight;
    uint32_t wPFBStrideInByte = chBytePerPixel * iPFBWidth;

    /* calculate source and target address */
    uintptr_t pnTarget = pnAddress + (iY * iLCDWidth + iX) * chBytePerPixel;
    uintptr_t pnSource = ptPFB->tTile.nAddress;

    /* region clipping */
    int16_t iHeight = MIN(iPFBHeight, iLCDHeight);
    int16_t iWidth = MIN(iPFBWidth, iLCDWidth);

    return this.tCFG.evtOn2DCopy.fnHandler(ptThis,
                                            this.tCFG.evtOnDMACopy.pObj,
                                            pnSource,
                                            wPFBStrideInByte,
                                            pnTarget,
                                            wLCDStrideInByte,
                                            iWidth,
                                            iHeight,
                                            chBytePerPixel
                                        );
}



/*----------------------------------------------------------------------------*
 * Transform Helper                                                           *
 *----------------------------------------------------------------------------*/


ARM_NONNULL(1,2,5)
void arm_2d_helper_transform_init(arm_2d_helper_transform_t *ptThis,
                                  arm_2d_op_t *ptTransformOP,
                                  float fAngleStep,
                                  float fScaleStep,
                                  arm_2d_region_list_item_t **ppDirtyRegionList)
{
    assert(NULL != ptThis);
    assert(NULL != ptTransformOP);
    assert(NULL != ppDirtyRegionList);
    
    memset(ptThis, 0, sizeof(arm_2d_helper_transform_t));
    this.ppDirtyRegionList = ppDirtyRegionList;

    this.ptTransformOP = ptTransformOP;
    this.Angle.fStep = fAngleStep;
    this.Scale.fStep = fScaleStep;
    this.Scale.fValue = 1.0f;
    this.fScale = 1.0f;

    this.bNeedUpdate = true;
    
    this.tDirtyRegions[0].bIgnore = true;
    this.tDirtyRegions[0].ptNext = &this.tDirtyRegions[1];
    this.tDirtyRegions[1].bIgnore = true;

    while(NULL != (*ppDirtyRegionList)) {
        ppDirtyRegionList = &((*ppDirtyRegionList)->ptNext);
    }

    /* add dirty region item to the list */
    (*ppDirtyRegionList) = &this.tDirtyRegions[0];
    this.tDirtyRegions[1].ptNext = NULL;
}

ARM_NONNULL(1)
void arm_2d_helper_transform_depose(arm_2d_helper_transform_t *ptThis)
{
    assert(NULL != ptThis);
    arm_2d_region_list_item_t **ppDirtyRegionList = this.ppDirtyRegionList;
    
    if (NULL == ppDirtyRegionList) {
        return ;
    }

    while(NULL != (*ppDirtyRegionList)) {

        /* remove the dirty region from the user dirty region list */
        if ((*ppDirtyRegionList) == &this.tDirtyRegions[0]) {
            (*ppDirtyRegionList) = this.tDirtyRegions[1].ptNext;
            this.tDirtyRegions[1].ptNext = NULL;
            break;
        }

        ppDirtyRegionList = &((*ppDirtyRegionList)->ptNext);
    }

}

ARM_NONNULL(1,2)
void arm_2d_helper_transform_update_dirty_regions(
                                    arm_2d_helper_transform_t *ptThis,
                                    const arm_2d_region_t *ptCanvas,
                                    bool bIsNewFrame)
{
    assert(NULL != ptThis);
    assert(NULL != ptCanvas);
    ARM_2D_UNUSED(ptCanvas);

    arm_2d_tile_t *ptTarget = (arm_2d_tile_t *)this.ptTransformOP->Target.ptTile;
    
    if (!bIsNewFrame) {
        return ;
    }
    
    if (this.bNeedUpdate) {
        this.bNeedUpdate = false;
        /* keep the old region */
        this.tDirtyRegions[1].tRegion = this.tDirtyRegions[0].tRegion;

        /* update the new region */
        this.tDirtyRegions[0].tRegion = *(this.ptTransformOP->Target.ptRegion);
        this.tDirtyRegions[0].tRegion.tLocation 
            = arm_2d_helper_pfb_get_absolute_location(
                                        ptTarget, 
                                        this.tDirtyRegions[0].tRegion.tLocation);

        arm_2d_region_t tOverlapArea, tEnclosureArea;
        if (arm_2d_region_intersect(&this.tDirtyRegions[1].tRegion, 
                                    &this.tDirtyRegions[0].tRegion,
                                    &tOverlapArea)) {
            /* the new region overlaps with the old region */
            uint32_t wPixelsRegion0 = this.tDirtyRegions[0].tRegion.tSize.iHeight
                                    * this.tDirtyRegions[0].tRegion.tSize.iWidth;
            uint32_t wPixelsRegion1 = this.tDirtyRegions[1].tRegion.tSize.iHeight
                                    * this.tDirtyRegions[1].tRegion.tSize.iWidth;


            arm_2d_region_get_minimal_enclosure(&this.tDirtyRegions[1].tRegion, 
                                                &this.tDirtyRegions[0].tRegion,
                                                &tEnclosureArea);
            
            uint32_t wPixelsRegionEnclosure = tEnclosureArea.tSize.iHeight
                                            * tEnclosureArea.tSize.iWidth;

            if (wPixelsRegionEnclosure < wPixelsRegion0 + wPixelsRegion1) {
                /* we only refresh the enclosure region to save time */
                this.tDirtyRegions[1].tRegion = tEnclosureArea;

                this.tDirtyRegions[0].bIgnore = true;
                this.tDirtyRegions[1].bIgnore = false;

                return ;
            }
        }

        /* the new region has no overlapping with the old region */
        this.tDirtyRegions[0].bIgnore = false;
        this.tDirtyRegions[1].bIgnore = false;

    }
}

ARM_NONNULL(1)
void arm_2d_helper_transform_on_frame_begin(arm_2d_helper_transform_t *ptThis)
{
    assert(NULL != ptThis);

    /* make it thread safe */
    arm_irq_safe {
        if (!this.bNeedUpdate) {
            this.tDirtyRegions[0].bIgnore = true;
            this.tDirtyRegions[1].bIgnore = true;
        } else {
            this.fAngle = this.Angle.fValue;
            this.fScale = this.Scale.fValue;
        }
    }
}

ARM_NONNULL(1)
void arm_2d_helper_transform_update_value(  arm_2d_helper_transform_t *ptThis,
                                            float fAngle,
                                            float fScale)
{
    assert(NULL != ptThis);
    
    float fDelta = 0.0f;

    arm_irq_safe {
        float fDelta = this.Angle.fValue - fAngle;
        fDelta = ABS(fDelta);

        if (fDelta >= this.Angle.fStep) {
            this.Angle.fValue = fAngle;
            this.bNeedUpdate = true;
        }
    }

    arm_irq_safe {
        fDelta = this.Scale.fValue - fScale;
        fDelta = ABS(fDelta);

        if (fDelta >= this.Scale.fStep) {
            this.Scale.fValue = fAngle;
            this.bNeedUpdate = true;
        }
    }
}

