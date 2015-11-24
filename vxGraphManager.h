#ifndef VX_GRAPH_MANAGER_H
#define VX_GRAPH_MANAGER_H

#include <iostream>
#include <VX/vx.h>
#include <VX/vx_helper.h>
#include <VX/vx_lib_debug.h>
#include <VX/vx_lib_snps.h>
#include <evss/cnn_hal.h>

#define GET_REF(vxType, delayObj, slot) \
	(vxType)vxGetReferenceFromDelay(delayObj, slot)
vx_status vxEvssScaleImage(vx_image src_image, vx_image dst_image, vx_scalar stype, vx_border_mode_t *bordermode, vx_float64 *interm, vx_size size);

// REMOVED... This is a sample code only
// .
// .
// .

// look-up table for exponential function used by softmax classification
static int8_t CNN_EXP_LUT[CNN_EXPLUT_SZ] __attribute__((aligned(128)));

#ifdef __arc__
#define UNCACHED _Uncached
#define SYNC() _sync()
#else
#define UNCACHED
#define SYNC()
#endif

// various stuff for host/ARC synchronization
static UNCACHED volatile uint64_t *gsa_ctrl = (UNCACHED volatile uint64_t *)EVSS_GSA_BASE_ADDR;
static UNCACHED volatile host_ctrl_t host_ctrl __attribute__((aligned(8)));
static UNCACHED volatile uint64_t host_synchro_flag __attribute__((aligned(8)));
static volatile int host_flag = 0;

// REMOVED... This is a sample code only
// .
// .
// .

static void wait_host_signal()
{
#if defined(NSIM)
    while(!host_flag){
	evYieldThread();
    }
#elif defined(HAPS)
    // disabling interrupts before checking host_flags to avoid race conditions
    _clri();
    while (!host_flag){
	// enable interrupts and go to sleep (this is done in an atomic way)
	_sleep(0x1F);
	_clri();
    }
    host_flag = 0;
    // re-enabling interrupts before leaving the function
    _seti(0);
#endif
}
#endif

class VXGraphManager{

public:
    VXGraphManager():
	context(NULL), width(inputWidth), height(inputHeight){}

    ~VXGraphManager(){}

    vx_status createGraph(){

	vx_status   status = VX_SUCCESS;
	context = vxCreateContext();
	if(!context){
	    error_print("vxgraph.createGraph() failed in creating context with code %d \n", status);
	    return VX_FAILURE;
	}
	// REMOVED... This is a sample code only
	// .
	// .
	// .

	graph = vxCreateGraph(context);
	
	/**
	 * creating scalars.
	 */
	vx_float32  conf	    = 0.0;
	vx_uint16   detectorSize    = CNN_DETECT_SIZE;	/* detect Box is 96x96 */
	vx_scalar   nonMaxEnabled   = vxCreateScalar(context, VX_TYPE_BOOL, &nonMax);
	vx_scalar   scale	    = vxCreateScalar(context, VX_TYPE_FLOAT32, &scale_f);
	
	// REMOVED... This is a sample code only
	// .
	// .
	// .

	/**
	 * creating arrays and images.
	 */
	size_t	    SIZE        = width * height;
	vx_enum	    detectType  = vxRegisterUserStruct(context, sizeof(ev_cnn_detection_t));
	vx_image    rgbImage	= vxCreateImage(context, width, height, VX_DF_IMAGE_RGB);	/* 0: rgb */
	vx_image    grayImage	= vxCreateImage(context, width, height, VX_DF_IMAGE_U8);	/* 2: gray */
	vx_array    intArray	= vxCreateArray(context,VX_TYPE_UINT32, SIZE);			/* integral image */
	vx_array    intSqArray	= vxCreateArray(context,VX_TYPE_UINT64, SIZE);			/* squared integral image */
	// REMOVED... This is a sample code only
	// .
	// .
	// .
	vx_pyramid  imgPyr	= vxCreatePyramid(context, 4, VX_SCALE_PYRAMID_ORB, width, height, VX_DF_IMAGE_U8);

	/**
	 * creating delay objects.
	 */
	input_d		= vxCreateDelay(context, (vx_reference)rgbImage,    2);
	output_d	= vxCreateDelay(context, (vx_reference)rectArray,   2);
	image_d		= vxCreateDelay(context, (vx_reference)grayImage,   5);
	CNNMask_d	= vxCreateDelay(context, (vx_reference)grayImage,   2);
	intArray_d	= vxCreateDelay(context, (vx_reference)intArray,    4);
	// REMOVED... This is a sample code only
	// .
	// .
	// .

	/**
	 * set CNN configuration
	 */
	// REMOVED... This is a sample code only
	// .
	// .
	// .

	i = 0;
	nodes = new vx_node[8];

	nodes[i++] = vxEvssGrayscaleNode(graph, GET_REF(vx_image, input_d, 0), GET_REF(vx_image, image_d, 0));
	nodes[i++] = vxSnpsSQRIntegralImageNode(graph, GET_REF(vx_image, image_d, 1), GET_REF(vx_array, intArray_d, 0),
						GET_REF(vx_array, SqIntArray_d, 0));
	nodes[i++] = vxEvssCnnPyramidNode(graph, GET_REF(vx_image, image_d, 1), GET_REF(vx_pyramid, imgPyr_d, 0), scale);

	nodes[i++] = vxEvssCnnNode(graph, GET_REF(vx_pyramid, imgPyr_d, 1), GET_REF(vx_array, detectArray_d, 0),
				   ctrlArray, scale, detector);

	nodes[i++] = vxSnpsNMSNode(graph, GET_REF(vx_array, detectArray_d, 0), GET_REF(vx_array, faceArray_d, 0));
	
	nodes[i++] = vxSnpsContextTrackNode(graph, GET_REF(vx_image, image_d, 3), GET_REF(vx_array, faceArray_d, 1),
					    GET_REF(vx_array, trackRect_d, 0));

	nodes[i++] = vxSnpsCascadeDetectNode(graph, GET_REF(vx_image, image_d, 3), GET_REF(vx_array, intArray_d, 2),
					     GET_REF(vx_array, SqIntArray_d, 2), GET_REF(vx_array, detectArray_d, 1),
					     GET_REF(vx_image, CNNMask_d, 0), GET_REF(vx_array, detectRect_d, 0));

	nodes[i++] = vxSnpsLearningNode(graph, GET_REF(vx_image, image_d, 4), GET_REF(vx_image, CNNMask_d, 1),
					GET_REF(vx_array, intArray_d, 3), GET_REF(vx_array, SqIntArray_d, 3),
					GET_REF(vx_array, trackRect_d, 1), GET_REF(vx_array, detectRect_d, 1),
					GET_REF(vx_array, output_d, 0), confidence);
	numNodes = i;

	status |= vxReleasePyramid(&imgPyr);
	status |= vxReleaseImage(&rgbImage);
	status |= vxReleaseImage(&grayImage);
	status |= vxReleaseArray(&faceArray);
	status |= vxReleaseArray(&detectArray);
	// REMOVED... This is a sample code only
	// .
	// .
	// .

	return status;
    }

    vx_status scheduleGraph()
    {
	vx_status   status = VX_SUCCESS;
	int cpu0 = 0, cpu1 = 1, cpu2 = 2, cpu3 = 3;

	/** CPU 0 **/
	status |= evHint(context, (vx_reference)nodes[NODE_NAME_CNN], EV_HINT_EXECUTE_CPU, &cpu0);		// CNN
	status |= evHint(context, (vx_reference)nodes[NODE_NAME_LEARN], EV_HINT_EXECUTE_CPU, &cpu0);		// learning
	/** CPU 1 **/
	status |= evHint(context, (vx_reference)nodes[NODE_NAME_GRAY_SCALE], EV_HINT_EXECUTE_CPU, &cpu1);	// color convert
	status |= evHint(context, (vx_reference)nodes[NODE_NAME_INT_IMAGE], EV_HINT_EXECUTE_CPU, &cpu1);	// integral image
	status |= evHint(context, (vx_reference)nodes[NODE_NAME_IMAGE_PYR], EV_HINT_EXECUTE_CPU, &cpu1);	// image pyramid
	/** CPU 2 **/
	status |= evHint(context, (vx_reference)nodes[NODE_NAME_CNTX_TRACK], EV_HINT_EXECUTE_CPU, &cpu2);	// context track
	/** CPU 3 **/
	status |= evHint(context, (vx_reference)nodes[NODE_NAME_CASCADE_DETECT], EV_HINT_EXECUTE_CPU, &cpu3);	// cascade detect

	return status;
    }

    vx_status handleGraphIO()
    {
	vx_status   status = VX_SUCCESS;
#ifdef USE_HOST_DRIVER
	// set pointer to host control structure such that the host can retrieve it
	*((volatile host_ctrl_t * UNCACHED *)HOST_CTRL_STRUCT_ADDR) = (volatile host_ctrl_t * UNCACHED)&host_ctrl;
	//evDataCacheFlushInvalidate();

	// host uses GSA to send interrupts to ARC cores
	host_ctrl.gsa_base_addr = EVSS_GSA_BASE_ADDR;

	// reset host synchro flag to tell host driver not to grab frames
	host_ctrl.wait_flag_addr = (uint32_t)&host_synchro_flag;

	//set target_loc_addr
	host_ctrl.target_loc_addr = (uint32_t)&target_loc;
	host_synchro_flag = 0;

	// setting up interrupts for main ARC HS core
	mcip_idu_disable(); // Disable IDU before reconfiguring it
	mcip_set_cirq_mode(2, MCIP_CIRQ_DISTR_ALL_DEST, MCIP_CIRQ_TRIG_LEVEL);
	mcip_set_cirq_dest(2, 0x1); // Send cIRQ 2 to core 0
	mcip_unmask_cirq(2); // Unmask cIRQ 2
	mcip_idu_enable();

	// set interrupt handler for GSA signal trigerred by Host
	_setvecti(26, host_handler);

	// enable all interrupts
	_seti(1<<4 | 0xf);

	void*				basePtr	= NULL;
	vx_rectangle_t			rect	= {0, 0, width, height};
	vx_imagepatch_addressing_t	addr;
	int i = 0;
	// filling host control structure before the host driver can be launched
	host_ctrl.exp_lut_addr = (uint32_t)cnn_ctrl.activation_lut;

	for(i = 0; i < 5; i++) {
	    host_ctrl.weights_addr[i] = (uint32_t)cnn_ctrl.weights[i];
	    host_ctrl.biases_addr[i] = (uint32_t)cnn_ctrl.biases[i];
	}
	for(i = 0; i < 2; i++){
	    basePtr = NULL;
	    vx_image inputImage = (vx_image)vxGetReferenceFromDelay(input_d, -i);
	    status |= vxAccessImagePatch(inputImage, &rect, 0, &addr, &basePtr, VX_READ_ONLY);
	    host_ctrl.input_addr[i] = (uint32_t)basePtr;
	    status |= vxCommitImagePatch(inputImage, &rect, 0, &addr, basePtr);

	    vx_array output_detection = (vx_array)vxGetReferenceFromDelay(output_d, -1+i);
	    vx_size stride;
	    basePtr = NULL;
		// REMOVED... This is a sample code only
		// .
		// .
		// .

	}
	
	for (i = 0; i < PE_CNN_CORES; i++) {
	    host_ctrl.cnn_engine_prog_addr[i] = (uint32_t)cnn_engine_prog[i];
	    host_ctrl.cnn_engine_data_addr[i] = (uint32_t)cnn_engine_data[i];
	}
	SYNC();
	
	wait_host_signal();
	info_print("Received signal from host driver \n");

	// boot CNN engine PEs
	ev_cnn_boot_t boot_struct;
	// assuming ev_1_0_big config
	boot_struct.data_size = 64*1024;
	boot_struct.prog_size = 16*1024;

	for (i = 0; i < PE_CNN_CORES; i++) {
	    boot_struct.data_addr = (uint32_t)cnn_engine_data[i];
	    boot_struct.prog_addr = (uint32_t)cnn_engine_prog[i];
	    evCnnBoot(i, &boot_struct);
	}
	
#endif
	return status;
    }
    vx_status verifyGraph()
    {
	// REMOVED... This is a sample code only
	// .
	// .
	// .
    }

    vx_status executeGraph()
    {
	vx_status   status;

	/**
	 * consequently activate blocks after each run
	 */
	// REMOVED... This is a sample code only
	// .
	// .
	// .

	status = vxProcessGraph(graph);

	/// performance measurement
	// REMOVED... This is a sample code only
	// .
	// .
	// .
	return status;
    }

    void disableParallelBlocks()
    {
		// clear values to dactivate exectution of all blocks (B0-B4)
		// REMOVED... This is a sample code only
    }

    vx_status ageGraph()
    {
	vx_status   status = VX_SUCCESS;
	status |= vxAgeDelay(input_d);
	status |= vxAgeDelay(output_d);
	status |= vxAgeDelay(image_d);
	status |= vxAgeDelay(imgPyr_d);
	status |= vxAgeDelay(intArray_d);
	status |= vxAgeDelay(SqIntArray_d);
	// REMOVED... This is a sample code only
	// .
	// .
	// .

	return status;
    }

    bool isGraphAgedEnough()
    {
		// REMOVED... This is a sample code only
    }


    Mat getRawFrame()
    {
	// REMOVED... This is a sample code only
	// .
	// .
	// .

    }

    Mat getGrayFrame()
    {
	void*			    basePtr = NULL;
	vx_rectangle_t		    rect    = {0, 0, width, height};
	vx_status		    status;
	vx_imagepatch_addressing_t  addr;
	Mat			    gray;

	vx_image input	= GET_REF(vx_image, image_d, 0);

	status = vxAccessImagePatch(input, &rect, 0, &addr, &basePtr, VX_READ_ONLY);
	if (!status){
	    gray = Mat(height, width, CV_8U, basePtr);
	}

	status |= vxCommitImagePatch(input, NULL, 0, &addr, basePtr);
	return gray;
    }

    vector<vx_rectangle_t> getCNNDetections()
    {
		// REMOVED... This is a sample code only
		// .
		// .
		// .
    }

    vx_status  vxGetIntegralImages(vx_uint32*& intImage, vx_uint64*& intSqImage)
    {
		// REMOVED... This is a sample code only
		// .
		// .
		// .

	vx_array intArray   = GET_REF(vx_array, intArray_d, 0);
	vx_array intSqArray = GET_REF(vx_array, SqIntArray_d, 0);

	vx_size	    numItems, stride32, stride64;
	vx_uint32*  tmp32   = NULL;
	vx_uint64*  tmp64   = NULL;
	vx_status   status;

	status = vxQueryArray(intArray, VX_ARRAY_ATTRIBUTE_NUMITEMS, &numItems, sizeof(numItems));

	if(numItems == 0){
	    return VX_FAILURE;
	}

	intImage    = (vx_uint32*)malloc(numItems * sizeof(vx_uint32));
	intSqImage  = (vx_uint64*)malloc(numItems * sizeof(vx_uint64));

	status |= vxAccessArrayRange(intArray,
				     0, numItems, &stride32, (void**)&tmp32, VX_READ_ONLY);
	status |= vxAccessArrayRange(intSqArray,
				     0, numItems, &stride64, (void**)&tmp64, VX_READ_ONLY);

	memcpy(intImage, tmp32, numItems * sizeof(vx_uint32));
	memcpy(intSqImage, tmp64, numItems * sizeof(vx_uint64));

	status |= vxCommitArrayRange(intArray, 0, numItems, tmp32);
	status |= vxCommitArrayRange(intSqArray, 0, numItems, tmp64);

	return status;
    }


    cv::Rect* getDetectRect()
    {
		// REMOVED... This is a sample code only
		// .
		// .
		// .
    }

    cv::Rect* getTrackRect()
    {
	// REMOVED... This is a sample code only
	// .
	// .
	// .
    }

    cv::Rect* getLearnRect()
    {
	// REMOVED... This is a sample code only
	// .
	// .
	// .
    }

    float getCurrConfidence()
    {
	// REMOVED... This is a sample code only
	// .
	// .
	// .
    }

    void release()
    {
	int i;
	for (i = 0; i < numNodes; i++){
	    vxReleaseNode(&nodes[i]);
	}

	vxReleaseGraph(&graph);
	vxReleaseContext(&context);

	vxReleaseDelay(&input_d);
	vxReleaseDelay(&output_d);
	vxReleaseDelay(&image_d);
	vxReleaseDelay(&imgPyr_d);
	vxReleaseDelay(&intArray_d);
	vxReleaseDelay(&SqIntArray_d);
	// REMOVED... This is a sample code only
	// .
	// .
	// .
	vxReleaseDelay(&CNNMask_d);

	vxReleaseScalar(&confidence);

#ifdef NSIM
	destroy_host_driver();
#endif
    }

    //Members
    vx_delay	input_d;
    vx_delay	output_d;

private:

    vx_uint32	width, height;
    vx_uint32	numNodes;
    vx_context	context;
    vx_graph	graph;
    vx_node*	nodes;

    /**
     * delay objects
     */
    vx_delay	image_d;
    vx_delay	imgPyr_d;
    vx_delay	intArray_d;
    // REMOVED... This is a sample code only
	// .
	// .
	// .

    vx_scalar	confidence;
};

#endif // VX_GRAPH_MANAGER_H
