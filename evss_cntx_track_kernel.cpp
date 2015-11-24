
#include "evss_cntx_track_kernel.h"
#include "evss_fast9.h"
#include <VX/vx_lib_debug.h>
#include <VX/vx.h>
#include <stdio.h>
#include <c_model.h>
#include <iostream>

using namespace std;

vx_status vxContextTrack(CNTX_KERNEL_DATA* krnl)
{
    /**
     * setup kernel
     */
    vx_status state = setupKernel(krnl);
    if(state){
		// REMOVED... This is a sample code only
		// .
		// .
		// .
    }

    /**
     * should contain at least one candidate.
     */
    if(!krnl->internal.cndLen){
		goto EXIT;
    }

    /**
     * evaluate condidates with their supporters.
     */
    state = evalCandidates(krnl);
    if(state){	
		// REMOVED... This is a sample code only
		// .
		// .
		// .
    }

    /**
     * check if the best candidate is good enough.
     * the first frame does not need to be checked
     */
    if( isFirstFrame(krnl) || isTargetFound(krnl)){
		state |= outputTarget(krnl);
    }

EXIT:
    popModel(krnl);
    disposeKernel(krnl);

    return state;
}

vx_status evalCandidates(CNTX_KERNEL_DATA* krnl)
{
    vx_status   status	    = VX_SUCCESS;

    /**
     * go over all available candidates
     */
    krnl->internal.currId   = 0;
    for( ; krnl->internal.currId < krnl->internal.cndLen ; krnl->internal.currId++){

	/**
	 * access i-th element of candidates
	 */
		
		// REMOVED... This is a sample code only
		// .
		// .
		// .
		
	/**
	 * checks whether currCnd is inside the
	 * the searching Area.
	 */
	if(!isInside(krnl->internal.currCnd,
		     krnl->arg.globROI)){
	    continue;
	}

	/**
	 * expand candidate bounding rectangle
	 * within a radius of r = SEARCH_RADIUS
	 */
	setSearchArea(krnl);

	/**
	 * extract and describe features within search area
	 */
	!(status = extractFAST9(krnl))  && (status |= describeBRIEF(krnl) );

	/**
	 * match found features with context model
	 * vector vote and state are filled for
	 * currCntx
	 **/
	matchDescriptor(krnl);

    // REMOVED... This is a sample code only
	// .
	// .
	// .
	
	/**
	 * compute number of active matches found
	 */
	computeActiveMatches(krnl);

	/**
	 * find index with the maximum vote
	 */
	if(isBestCandidate(krnl)){
	    saveCandidate(krnl);
	}
	status |= vxReleaseImage(&krnl->internal.roiImg);
    }
    return status;
}


void findBRIEFMatches(const uint32_t src_descriptor[], const uint32_t dst_descriptor[], vector<D_MATCH> &matches, int size_src, int size_dst)
{
    matches.clear();
    int i, j, k;
    for (i = 0; i < size_src; i++ ){

	unsigned comp[DESCRIPTOR_STEP];
	    int	 minDist    = 0x7FFFFFFF;   //Minimum distance set at maximum value
	uint32_t minDistIdx = 0;	    //Minimum distance index

	int	 iStep	    = i * DESCRIPTOR_STEP;
	for (j = 0; j < size_dst; j++ ){
	    int jStep = j * DESCRIPTOR_STEP;
	    for (k = 0; k < DESCRIPTOR_STEP ; k++){
		comp[k] = ((unsigned)src_descriptor[iStep + k]
			   ^
			   (unsigned)dst_descriptor[jStep + k]);
	    }
	    int dist = descriptorBitCount(comp);
	    if (dist < minDist){
		minDist    = dist;
		minDistIdx = j;
	    }
	}
	if (minDist <= MIN_HAMMING_DIST){
	    D_MATCH mtch;
	    mtch.distance = minDist;
	    mtch.queryIdx = i;
	    mtch.trainIdx = minDistIdx;
	    matches.push_back(mtch);
	}
    }
}

int isActive(CNTX_KERNEL_DATA*   krnl, const CNTX_MODEL* desc, int index)
{
    // REMOVED... This is a sample code only
}

/**
 * @brief matchDescriptor
 * match features from searching area
 * with context features.
 * @param pointer to src descriptor, context descriptor, output matches
 */
void matchDescriptor(CNTX_KERNEL_DATA*   krnl)
{
    // REMOVED... This is a sample code only
}

/**
 * @brief countActiveMatches
 * count the number of activeMatches inside the given descriptor
 */
void computeActiveMatches(CNTX_KERNEL_DATA* krnl)
{
    // REMOVED... This is a sample code only
}

int isTargetFound(CNTX_KERNEL_DATA* krnl)
{
    // REMOVED... This is a sample code only
}
vx_status setupKernel(CNTX_KERNEL_DATA* krnl)
{
    vx_status   state		= VX_SUCCESS;

    /**
    * initialization of kernel parameters
    */
    state  =  vxQueryArray(krnl->arg.candidates,
			   VX_ARRAY_ATTRIBUTE_NUMITEMS,
			   &krnl->internal.cndLen,
			   sizeof(krnl->internal.cndLen)
			   );

    if (!state && krnl->internal.cndLen){
		// REMOVED... This is a sample code only
		// .
		// .
		// .
    }
    return state;
}

vx_status extractFAST9(CNTX_KERNEL_DATA* krnl)
{
    /**
     * extract FAST features (points)
     */
	// REMOVED... This is a sample code only
}

vx_status describeBRIEF(CNTX_KERNEL_DATA* krnl)
{
    // REMOVED... This is a sample code only
	// .
	// .
	// .

    status = vxGetValidRegionImage(input,
				   &rect);

    (!status) && ( status |= vxAccessImagePatch(input,
						&rect,
						0,
						&addr,
						(void**)&base_ptr,
						VX_READ_ONLY) );
    if(status){
		error_print("vxSnpsContextTrack::describeBRIEF returned error code %d \n",status);
		return status;
    }

    // REMOVED... This is a sample code only
	// .
	// .
	// .
	
    /**
     * reference commitment
     */
    status |= vxCommitImagePatch(input,
				 &rect,
				 0,
				 &addr,
				 base_ptr);
    return status;
}

void disposeKernel(CNTX_KERNEL_DATA* krnl)
{
    /**
     * commit array ranges to enable
     * the reference to be freed later.
     */
    // REMOVED... This is a sample code only
}

void popModel(CNTX_KERNEL_DATA* krnl)
{
    // REMOVED... This is a sample code only
	// .
	// .
	// .
}

void setSearchArea(CNTX_KERNEL_DATA* krnl)
{
    // REMOVED... This is a sample code only
}

int isFirstFrame(CNTX_KERNEL_DATA* krnl)
{
    return !(krnl->update.initCnt);
}

int compByScore(const void* k1, const void* k2)
{
    return (*(vx_keypoint_t*)k1).strength < (*(vx_keypoint_t*)k2).strength;
}

inline int int32BitCount( unsigned int v )
{
    // http://www-graphics.stanford.edu/~seander/bithacks.html#CountBitsSetParallel
    v = v - ((v >> 1) & 0x55555555);				// reuse input as temporary
    v = (v & 0x33333333) + ((v >> 2) & 0x33333333);		// temp
    return (((v + (v >> 4)) & 0xF0F0F0F) * 0x1010101) >> 24;	// count
}

int descriptorBitCount( unsigned int * desc)
{
    int dword_count = DESCRIPTOR_STEP; // number of UINT32's used to hold descriptor
    int i, bitCount = 0;
    for (i = 0; i < dword_count; i++){
	bitCount += int32BitCount(desc[i]);
    }
    return bitCount;
}

int isBestCandidate(CNTX_KERNEL_DATA* krnl)
{
    // REMOVED... This is a sample code only
}

void saveCandidate(CNTX_KERNEL_DATA* krnl)
{
    // REMOVED... This is a sample code only
}

vx_status outputTarget(CNTX_KERNEL_DATA* krnl)
{
    // REMOVED... This is a sample code only
}
