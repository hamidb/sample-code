/*
 * Copyright (c) 2013-2014 The Khronos Group Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and/or associated documentation files (the
 * "Materials"), to deal in the Materials without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Materials, and to
 * permit persons to whom the Materials are furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Materials.
 *
 * THE MATERIALS ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * MATERIALS OR THE USE OR OTHER DEALINGS IN THE MATERIALS.
 */

#include <VX/lower/vx_lower.h>
#include "vx_lib_snps.h"

/*!
 * \file vx_demo_lib.c
 * \example vx_demo_lib.c
 * \brief An example of how to write a user mode extension to OpenVX.
 * \author Hamid Bazargani <hamidb@synopsys.com>
 */
//! [node]
vx_node vxSnpsLearningNode(vx_graph graph, vx_image input, vx_image CNNMask, vx_array integralImg, vx_array sqrIntegralImg,
                           vx_array trckRect, vx_array detectRect, vx_array learnRect, vx_scalar confidence)
{

    vx_reference params[] = {
        (vx_reference)input,
        (vx_reference)CNNMask,
        (vx_reference)integralImg,
        (vx_reference)sqrIntegralImg,
        (vx_reference)trckRect,
        (vx_reference)detectRect,
        (vx_reference)learnRect,
        (vx_reference)confidence,
    };

    return vxCreateNodeByStructure(graph, VX_KERNEL_USER_LEARNING, params, dimof(params));
}
//! [node]

//! [vxu]
/* IMMEDIATE INTERFACES */

vx_status vxuSnpsLearning(vx_context context, vx_image input, vx_image CNNMask, vx_array integralImg, vx_array sqrIntegralImg,
                          vx_array trckRect, vx_array detectRect, vx_array learnRect, vx_scalar confidence)
{
    vx_status status = VX_FAILURE;
    vx_graph graph = vxCreateGraph(context);
    if (graph)
    {
        vx_node node = vxSnpsLearningNode(graph, input, CNNMask, integralImg, sqrIntegralImg,
                                          trckRect, detectRect, learnRect, confidence);

        if (node)
        {
            status = vxVerifyGraph(graph);
            if (status == VX_SUCCESS)
            {
                status = vxProcessGraph(graph);
            }
            vxReleaseNode(&node);
        }
        vxReleaseGraph(&graph);
    }
    return status;
}
//! [vxu]
