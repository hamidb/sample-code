
#include "evss_integral_kernel.h"
#include "iostream"
// nodeless version of the SquaredIntegralImage kernel
vx_status vxSnpsSQRIntegralImage(vx_image src, vx_array intImg, vx_array sqrIntImg)
{
    vx_uint32 y, x;
    void *src_base = NULL;
    void *intImg_base = NULL;
    void *sqrIntImg_base = NULL;
    vx_imagepatch_addressing_t src_addr;
    vx_rectangle_t rect;
    vx_size num_items = 0;
    vx_size stride = 0;


    vx_status status = VX_SUCCESS;
    status = vxGetValidRegionImage(src, &rect);
    status |= vxAccessImagePatch(src, &rect, 0, &src_addr, &src_base, VX_READ_ONLY);

    status |= vxQueryArray(intImg, VX_ARRAY_ATTRIBUTE_NUMITEMS, &num_items, sizeof(num_items));
    status |= vxAccessArrayRange(intImg,0, num_items, &stride, &intImg_base, VX_READ_AND_WRITE);
    status |= vxAccessArrayRange(sqrIntImg,0, num_items, &stride, &sqrIntImg_base, VX_READ_AND_WRITE);

    if(status){
        return status;
    }

    vx_uint32 step= src_addr.dim_x;

    for (y = 0; y < src_addr.dim_y; y++){
        vx_uint8 *pixels = (vx_uint8*)(src_base);
        pixels += y*step;
        vx_uint32 *sums = (vx_uint32*) intImg_base;
        vx_uint64 *sqr_sums = (vx_uint64*) sqrIntImg_base;

        if (y == 0){
            sums[0] = pixels[0];
            sqr_sums[0] = pixels[0]*pixels[0];
            for (x = 1; x < src_addr.dim_x; x++){
                sums[x] = sums[x-1] + pixels[x];
                sqr_sums[x] = sqr_sums[x-1] + pixels[x]*pixels[x];
            }
        }
        else{
            vx_uint32 *prev_sums = sums + (y-1)*step;
            vx_uint64 *sqr_prev_sums = sqr_sums + (y-1)*step;
            sums += y*step;
            sums[0] = prev_sums[0] + pixels[0];

            sqr_sums += y*step;
            sqr_sums[0] = sqr_prev_sums[0] + pixels[0]*pixels[0];

            for (x = 1; x < src_addr.dim_x; x++){
                sums[x] = pixels[x] + sums[x-1] + prev_sums[x] - prev_sums[x-1];
                sqr_sums[x] = pixels[x]*pixels[x] + sqr_sums[x-1] + sqr_prev_sums[x] - sqr_prev_sums[x-1];
            }
        }
    }
    status |= vxCommitImagePatch(src, NULL, 0, &src_addr, src_base);
    status |= vxCommitArrayRange(intImg, 0, num_items, intImg_base);
    status |= vxCommitArrayRange(sqrIntImg, 0, num_items, sqrIntImg_base);
    return status;
}
