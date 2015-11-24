/**
 * evss_learning_module.cpp
 * An implementation of a User Extension to OpenVX for fuse and learning as
 * parts of TLD algorithm.
 * Hamid Bazargani <hamidb@synopsys.com>
 */

#include <VX/vx.h>
#include "vx_lib_snps.h"
#include "evss_learning_kernel.h"
#include "evss_cascade_kernel.h"
#include <VX/lower/vx_lower.h>
#include <stdarg.h>

// REMOVED... This is a sample code only