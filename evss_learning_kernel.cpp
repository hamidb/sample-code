
#include "log.h"
#include "param.h"
#include "TLDUtil.h"
#include "evss_learning_kernel.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <iostream>

void learnNN(learningLocalData* krnl, vector<patchNorm> &patches)
{
// REMOVED... This is a sample code only
}

void updatePosteriors(learningLocalData* krnl, vx_int32* featureVector, vx_bool positive)
{
// REMOVED... This is a sample code only
}

void learnFern(learningLocalData* krnl, vx_uint8* img, vx_int32 s, vx_uint32 x, vx_uint32 y,
	       vx_bool positive, vx_int32* featureVector)
{
    // REMOVED... This is a sample code only
}

vx_status setupKernel(learningLocalData* krnl)
{
    // REMOVED... This is a sample code only
	
    num = 0;
    status |= vxQueryArray(trackArr, VX_ARRAY_ATTRIBUTE_NUMITEMS, &num, sizeof(num));

    if(!status && num == 1){
	arrPtr  = NULL;
	status |= vxAccessArrayRange(trackArr, 0, 1, &stride, &arrPtr, VX_READ_ONLY);
	krnl->internal.trackBB_r  = vxArrayItem(vx_rectangle_t, arrPtr, 0, stride);
	krnl->internal.numTracks  = (vx_uint32)num;
	status |= vxCommitArrayRange(trackArr, 0, 1, arrPtr);

	/**
	 * preserve target aspect ratio
	 * and boundry check
	 */
	 // REMOVED... This is a sample code only
	vx_float32 ar     = krnl->constants.objHeight/(vx_float32)krnl->constants.objWidth;
	vx_float32 height = krnl->internal.trackBB_r.end_y - krnl->internal.trackBB_r.start_y;
	vx_int32   offset = (ar-1) * height / 2.f;
	krnl->internal.trackBB_r.start_y -= offset;
	krnl->internal.trackBB_r.end_y   += offset;
	}
	
    // REMOVED... This is a sample code only
	return status;
}

void fuseDetections(learningLocalData* krnl)
{
    // REMOVED... This is a sample code only
}

vx_status vxSnpsLearning(learningLocalData* krnl)
{
    vx_status status = setupKernel(krnl);
    if(status){
		error_print("vxSnpsLearning::setupKernel() returned with error code %d\n", status);
		return status;
    }

    fuseDetections(krnl);
    if(!krnl->internal.valid){
	// REMOVED... This is a sample code only
    }

    /**
     * learning starts from here.
     */
    vx_image			input	    = krnl->arg.input;
    vx_rectangle_t		fusedBB	    = krnl->internal.fusedBB;
    vx_uint32			imgWidth    = krnl->constants.imgWidth;
    void*			src_base    = NULL;
    void*			msk_base    = NULL;
    // REMOVED... This is a sample code only

    
    vx_uint8*	imgPtr		= (vx_uint8*) src_base;
    vx_uint8*	mskPtr		= (vx_uint8*) msk_base;
    vx_int32	featureVector[FERN_SIZE];
    vx_uint32	x, y, i = 0, sIdx = 0;
    vx_int32	s = 0;
    for(s = krnl->constants.minScale; s <= krnl->constants.maxScale; s++){

	vx_float32  scale   = powf(1.2f, (float)s);
	vx_uint32   w	    = krnl->constants.objWidth * scale;
	vx_uint32   h	    = krnl->constants.objHeight * scale;
	// REMOVED... This is a sample code only
	
	sIdx++;
	for( y = 1 ; (y + h) <= krnl->constants.imgHeight; y += sH){

	    int cntrY   = (y + hHalf) * imgWidth;
	    int quartY1 = cntrY + hQuart;
	    int quartY2 = cntrY - hQuart;
	    for( x = 1; (x + w) <= imgWidth; x += sW){

		int cntrX = x + wHalf;
		if( mskPtr[cntrY + cntrX] == 0){
		    continue;
		}
		int quartX = cntrX + wQuart;
		if( mskPtr[quartY1 + quartX] == 0){
		    continue;
		}
		// REMOVED... This is a sample code only
		
		vx_float32 overlap	= calcOverlapRectRect(&fusedBB, &tmp_r);

		if(overlap > 0.6f){
		    positiveIndices.push_back(pair<int, float>(i, overlap));
		}

		if(overlap < 0.2f){

		// REMOVED... This is a sample code only
		}
		i++;
	    }
	}
    }

	// REMOVED... This is a sample code only
    for( i = 0; i < negativeIndices.size() && !status ; i++){
	// REMOVED... This is a sample code only
	learnFern(krnl, imgPtr, scalesIdx[idx],xywh_r.start_x, xywh_r.start_y, vx_false_e, featureVector);
    }

    // REMOVED... This is a sample code only
	
    if(status == VX_SUCCESS){
	learnNN(krnl, patches);
    }
    /**
     * clear data
     */
    positiveIndices.clear();
    negativeIndices.clear();
    patches.clear();
    xywh.clear();
    scalesIdx.clear();
    /// TODO: commit range?
    status |= vxCommitArrayRange(krnl->arg.intArray, 0, 0, int_base);
    status |= vxCommitArrayRange(krnl->arg.intSqArray, 0, 0, intSq_base);
    status |= vxCommitImagePatch(input, NULL, 0, &src_addr, src_base);
    if(status){
	error_print("vxSnpsLearning::commit returned error code %d\n", status);
    }
    return status;
}

vx_status calcNormalizedRect(vx_image input, vx_rectangle_t *rect, vx_float32 *patch)
{
    // REMOVED... This is a sample code only
}
