/**
 * evss_cntx_track_module.cpp
 * An implementation of a User Extension to OpenVX for context-aware
 * tracking kernel.
 * Hamid Bazargani <hamidb@synopsys.com>
 * \brief Context-aware tracking kernel using FAST9 feature detection and
 * BRIEF descriptor extraction. Matching based on hamming distance is
 * performed. Given an array of candidate rectangles and a model abstracting
 * target contextual information, the index to the array elemnt with maximum
 * support is computed.
 */

#include <VX/vx.h>
#include "vx_lib_snps.h"
#include "evss_cntx_track_kernel.h"
#include <VX/lower/vx_lower.h>
#include <VX/vx_lib_debug.h>
#include <stdarg.h>

/**
 * An internal definition of the order of the parameters to the function.
 */
typedef enum _cntx_track_params_e {
    CNTX_TRACK_PARAM_INPUT	= 0,
    CNTX_TRACK_PARAM_CANDIDATES,
    CNTX_TRACK_PARAM_TRACK_RECT,
} evss_cntx_track_params_e;

/**
 * Declares the parameter types for vxSnpsContextTrackKernel.
 */
static vx_param_description_t cntx_track_kernel_params[] = {
    {VX_INPUT, VX_TYPE_IMAGE, VX_PARAMETER_STATE_REQUIRED},
    {VX_INPUT, VX_TYPE_ARRAY, VX_PARAMETER_STATE_REQUIRED},
    {VX_OUTPUT, VX_TYPE_ARRAY, VX_PARAMETER_STATE_REQUIRED},
};

/**
 * The private kernel function for context tracking.
 */
static vx_status vxSnpsContextTrackKernel(vx_node node, vx_reference *parameters, vx_uint32 num)
{
    EVTH_TRACE_MSG("START_CONTEXT_KERNEL", 0, 0);
    if (num == dimof(cntx_track_kernel_params)){

	vx_image image	    = (vx_image)parameters[CNTX_TRACK_PARAM_INPUT];
	vx_array candidates = (vx_array)parameters[CNTX_TRACK_PARAM_CANDIDATES];
	vx_array trackRect  = (vx_array)parameters[CNTX_TRACK_PARAM_TRACK_RECT];

	/**
	 *  check weather the FRAME_LEVEL flag is set
	 */
	 
	// REMOVED... This is a sample code only
	
	/**
	 *  check weather the context model is initialized.
	 */
	 
	// REMOVED... This is a sample code only

	/**
	 * create trgtCntx for the lifetime of S_descriptor.
	 */
	krnlDataPtr->update.trgtCntx	= &S_descriptor;

	status |= vxSnpsContextTrack(krnlDataPtr);

	// REMOVED... This is a sample code only
	
    return VX_ERROR_INVALID_PARAMETERS;
}

/**
 * Input parameter validator.
 */
static vx_status vxSnpsContextTrackInputValidator(vx_node node, vx_uint32 index)
{
    // REMOVED... This is a sample code only
    return status;
}

/**
 * Output parameter validator.
 */
static vx_status vxSnpsContextTrackOutputValidator(vx_node node, vx_uint32 index, vx_meta_format_t* meta)
{
	// REMOVED... This is a sample code only
    return status;
}

/**
 * An initializer function.
 */
static vx_status vxSnpsContextTrackInitializer(vx_node node, vx_reference *parameters, vx_uint32 num)
{
 
	/**
	 * One time initialization.
	 */
	krnlDataPtr->match.FAST9Thrsh	    = vxCreateScalar(context, VX_TYPE_FLOAT32, &fastThrsh);
	krnlDataPtr->match.nonMax	    = vxCreateScalar(context, VX_TYPE_BOOL, &nonM);
	krnlDataPtr->internal.imgWidth	    = width;
	krnlDataPtr->internal.imgHeight	    = height;
	// REMOVED... This is a sample code only
	
	/**
	 * allocate memory on heap
	 * (used to be on the stack inside vxSnpsContextTrack)
	 * using new[] to call the constructor
	 */
	krnlDataPtr->internal.currCntx	    = new CNTX_MODEL();
	krnlDataPtr->internal.bestCntx	    = new CNTX_MODEL();

	status  = vxSetNodeAttribute(node, VX_NODE_ATTRIBUTE_LOCAL_DATA_SIZE, &sizeData, sizeof(sizeData));
	status |= vxSetNodeAttribute(node, VX_NODE_ATTRIBUTE_LOCAL_DATA_PTR, &krnlDataPtr,sizeof(krnlDataPtr));
    }
    return status;
}

/**
 * A deinitializer function.
 */
static vx_status vxSnpsContextTrackDeInitializer(vx_node node, vx_reference *parameters, vx_uint32 num)
{
    // REMOVED... This is a sample code only
}

vx_kernel_description_t cntx_track_kernels = {
    VX_KERNEL_USER_CNTX_TRACK,
    // REMOVED... This is a sample code only
    vxSnpsContextTrackKernel,
    cntx_track_kernel_params, dimof(cntx_track_kernel_params),
    vxSnpsContextTrackInputValidator,
    vxSnpsContextTrackOutputValidator,
    vxSnpsContextTrackInitializer,
    vxSnpsContextTrackDeInitializer,
};

static vx_kernel_description_t *kernelss[] = {
    &cntx_track_kernels,
};
//**********************************************************************
//  PUBLIC FUNCTION
//**********************************************************************

/**
 * The entry point into this module to add the extensions to OpenVX.
 * \param [in] context The handle to the implementation context.
 * \return A \ref vx_status_e enumeration. Returns errors if some or all kernels were not added
 * correctly.
 */
vx_status vxPublishSnpsContextTrackKernels(vx_context context) {
	// REMOVED... This is a sample code only
	
}
