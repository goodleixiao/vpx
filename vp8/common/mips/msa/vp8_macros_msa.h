/*
 *  Copyright (c) 2015 The WebM project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef VP8_COMMON_MIPS_MSA_VP8_MACROS_MSA_H_
#define VP8_COMMON_MIPS_MSA_VP8_MACROS_MSA_H_

#include <msa.h>

#include "./vpx_config.h"
#include "vpx/vpx_integer.h"

#define LD_B(RTYPE, psrc) *((const RTYPE *)(psrc))
#define LD_UB(...) LD_B(v16u8, __VA_ARGS__)
#define LD_SB(...) LD_B(v16i8, __VA_ARGS__)

#define LD_H(RTYPE, psrc) *((const RTYPE *)(psrc))
#define LD_UH(...) LD_H(v8u16, __VA_ARGS__)
#define LD_SH(...) LD_H(v8i16, __VA_ARGS__)

#define ST_B(RTYPE, in, pdst) *((RTYPE *)(pdst)) = (in)
#define ST_UB(...) ST_B(v16u8, __VA_ARGS__)
#define ST_SB(...) ST_B(v16i8, __VA_ARGS__)

#define ST_H(RTYPE, in, pdst) *((RTYPE *)(pdst)) = (in)
#define ST_UH(...) ST_H(v8u16, __VA_ARGS__)
#define ST_SH(...) ST_H(v8i16, __VA_ARGS__)

#if (__mips_isa_rev >= 6)
#define LW(psrc)                                      \
({                                                    \
    const uint8_t *psrc_m = (const uint8_t *)(psrc);  \
    uint32_t val_m;                                   \
                                                      \
    asm volatile (                                    \
        "lw  %[val_m],  %[psrc_m]  \n\t"              \
                                                      \
        : [val_m] "=r" (val_m)                        \
        : [psrc_m] "m" (*psrc_m)                      \
    );                                                \
                                                      \
    val_m;                                            \
})

#if (__mips == 64)
#define LD(psrc)                                      \
({                                                    \
    const uint8_t *psrc_m = (const uint8_t *)(psrc);  \
    uint64_t val_m = 0;                               \
                                                      \
    asm volatile (                                    \
        "ld  %[val_m],  %[psrc_m]  \n\t"              \
                                                      \
        : [val_m] "=r" (val_m)                        \
        : [psrc_m] "m" (*psrc_m)                      \
    );                                                \
                                                      \
    val_m;                                            \
})
#else  // !(__mips == 64)
#define LD(psrc)                                             \
({                                                           \
    const uint8_t *psrc_m = (const uint8_t *)(psrc);         \
    uint32_t val0_m, val1_m;                                 \
    uint64_t val_m = 0;                                      \
                                                             \
    val0_m = LW(psrc_m);                                     \
    val1_m = LW(psrc_m + 4);                                 \
                                                             \
    val_m = (uint64_t)(val1_m);                              \
    val_m = (uint64_t)((val_m << 32) & 0xFFFFFFFF00000000);  \
    val_m = (uint64_t)(val_m | (uint64_t)val0_m);            \
                                                             \
    val_m;                                                   \
})
#endif  // (__mips == 64)

#define SH(val, pdst)                     \
{                                         \
    uint8_t *pdst_m = (uint8_t *)(pdst);  \
    const uint16_t val_m = (val);         \
                                          \
    asm volatile (                        \
        "sh  %[val_m],  %[pdst_m]  \n\t"  \
                                          \
        : [pdst_m] "=m" (*pdst_m)         \
        : [val_m] "r" (val_m)             \
    );                                    \
}

#define SW(val, pdst)                     \
{                                         \
    uint8_t *pdst_m = (uint8_t *)(pdst);  \
    const uint32_t val_m = (val);         \
                                          \
    asm volatile (                        \
        "sw  %[val_m],  %[pdst_m]  \n\t"  \
                                          \
        : [pdst_m] "=m" (*pdst_m)         \
        : [val_m] "r" (val_m)             \
    );                                    \
}

#define SD(val, pdst)                     \
{                                         \
    uint8_t *pdst_m = (uint8_t *)(pdst);  \
    const uint64_t val_m = (val);         \
                                          \
    asm volatile (                        \
        "sd  %[val_m],  %[pdst_m]  \n\t"  \
                                          \
        : [pdst_m] "=m" (*pdst_m)         \
        : [val_m] "r" (val_m)             \
    );                                    \
}
#else  // !(__mips_isa_rev >= 6)
#define LW(psrc)                                      \
({                                                    \
    const uint8_t *psrc_m = (const uint8_t *)(psrc);  \
    uint32_t val_m;                                   \
                                                      \
    asm volatile (                                    \
        "ulw  %[val_m],  %[psrc_m]  \n\t"             \
                                                      \
        : [val_m] "=r" (val_m)                        \
        : [psrc_m] "m" (*psrc_m)                      \
    );                                                \
                                                      \
    val_m;                                            \
})

#if (__mips == 64)
#define LD(psrc)                                      \
({                                                    \
    const uint8_t *psrc_m = (const uint8_t *)(psrc);  \
    uint64_t val_m = 0;                               \
                                                      \
    asm volatile (                                    \
        "uld  %[val_m],  %[psrc_m]  \n\t"             \
                                                      \
        : [val_m] "=r" (val_m)                        \
        : [psrc_m] "m" (*psrc_m)                      \
    );                                                \
                                                      \
    val_m;                                            \
})
#else  // !(__mips == 64)
#define LD(psrc)                                             \
({                                                           \
    const uint8_t *psrc_m1 = (const uint8_t *)(psrc);        \
    uint32_t val0_m, val1_m;                                 \
    uint64_t val_m = 0;                                      \
                                                             \
    val0_m = LW(psrc_m1);                                    \
    val1_m = LW(psrc_m1 + 4);                                \
                                                             \
    val_m = (uint64_t)(val1_m);                              \
    val_m = (uint64_t)((val_m << 32) & 0xFFFFFFFF00000000);  \
    val_m = (uint64_t)(val_m | (uint64_t)val0_m);            \
                                                             \
    val_m;                                                   \
})
#endif  // (__mips == 64)
#define SH(val, pdst)                      \
{                                          \
    uint8_t *pdst_m = (uint8_t *)(pdst);   \
    const uint16_t val_m = (val);          \
                                           \
    asm volatile (                         \
        "ush  %[val_m],  %[pdst_m]  \n\t"  \
                                           \
        : [pdst_m] "=m" (*pdst_m)          \
        : [val_m] "r" (val_m)              \
    );                                     \
}

#define SW(val, pdst)                      \
{                                          \
    uint8_t *pdst_m = (uint8_t *)(pdst);   \
    const uint32_t val_m = (val);          \
                                           \
    asm volatile (                         \
        "usw  %[val_m],  %[pdst_m]  \n\t"  \
                                           \
        : [pdst_m] "=m" (*pdst_m)          \
        : [val_m] "r" (val_m)              \
    );                                     \
}

#define SD(val, pdst)                                         \
{                                                             \
    uint8_t *pdst_m1 = (uint8_t *)(pdst);                     \
    uint32_t val0_m, val1_m;                                  \
                                                              \
    val0_m = (uint32_t)((val) & 0x00000000FFFFFFFF);          \
    val1_m = (uint32_t)(((val) >> 32) & 0x00000000FFFFFFFF);  \
                                                              \
    SW(val0_m, pdst_m1);                                      \
    SW(val1_m, pdst_m1 + 4);                                  \
}
#endif  // (__mips_isa_rev >= 6)

/* Description : Store 4 words with stride
   Arguments   : Inputs - in0, in1, in2, in3, pdst, stride
   Details     : Store word from 'in0' to (pdst)
                 Store word from 'in1' to (pdst + stride)
                 Store word from 'in2' to (pdst + 2 * stride)
                 Store word from 'in3' to (pdst + 3 * stride)
*/
#define SW4(in0, in1, in2, in3, pdst, stride)  \
{                                              \
    SW(in0, (pdst));                           \
    SW(in1, (pdst) + stride);                  \
    SW(in2, (pdst) + 2 * stride);              \
    SW(in3, (pdst) + 3 * stride);              \
}

/* Description : Store 4 double words with stride
   Arguments   : Inputs - in0, in1, in2, in3, pdst, stride
   Details     : Store double word from 'in0' to (pdst)
                 Store double word from 'in1' to (pdst + stride)
                 Store double word from 'in2' to (pdst + 2 * stride)
                 Store double word from 'in3' to (pdst + 3 * stride)
*/
#define SD4(in0, in1, in2, in3, pdst, stride)  \
{                                              \
    SD(in0, (pdst));                           \
    SD(in1, (pdst) + stride);                  \
    SD(in2, (pdst) + 2 * stride);              \
    SD(in3, (pdst) + 3 * stride);              \
}

/* Description : Load vectors with 16 byte elements with stride
   Arguments   : Inputs  - psrc, stride
                 Outputs - out0, out1
                 Return Type - as per RTYPE
   Details     : Load 16 byte elements in 'out0' from (psrc)
                 Load 16 byte elements in 'out1' from (psrc + stride)
*/
#define LD_B2(RTYPE, psrc, stride, out0, out1)  \
{                                               \
    out0 = LD_B(RTYPE, (psrc));                 \
    out1 = LD_B(RTYPE, (psrc) + stride);        \
}
#define LD_UB2(...) LD_B2(v16u8, __VA_ARGS__)
#define LD_SB2(...) LD_B2(v16i8, __VA_ARGS__)

#define LD_B4(RTYPE, psrc, stride, out0, out1, out2, out3)   \
{                                                            \
    LD_B2(RTYPE, (psrc), stride, out0, out1);                \
    LD_B2(RTYPE, (psrc) + 2 * stride , stride, out2, out3);  \
}
#define LD_UB4(...) LD_B4(v16u8, __VA_ARGS__)
#define LD_SB4(...) LD_B4(v16i8, __VA_ARGS__)

#define LD_B8(RTYPE, psrc, stride,                                      \
              out0, out1, out2, out3, out4, out5, out6, out7)           \
{                                                                       \
    LD_B4(RTYPE, (psrc), stride, out0, out1, out2, out3);               \
    LD_B4(RTYPE, (psrc) + 4 * stride, stride, out4, out5, out6, out7);  \
}
#define LD_UB8(...) LD_B8(v16u8, __VA_ARGS__)

/* Description : Load vectors with 8 halfword elements with stride
   Arguments   : Inputs  - psrc, stride
                 Outputs - out0, out1
   Details     : Load 8 halfword elements in 'out0' from (psrc)
                 Load 8 halfword elements in 'out1' from (psrc + stride)
*/
#define LD_H2(RTYPE, psrc, stride, out0, out1)  \
{                                               \
    out0 = LD_H(RTYPE, (psrc));                 \
    out1 = LD_H(RTYPE, (psrc) + (stride));      \
}
#define LD_SH2(...) LD_H2(v8i16, __VA_ARGS__)

#define LD_H4(RTYPE, psrc, stride, out0, out1, out2, out3)  \
{                                                           \
    LD_H2(RTYPE, (psrc), stride, out0, out1);               \
    LD_H2(RTYPE, (psrc) + 2 * stride, stride, out2, out3);  \
}
#define LD_SH4(...) LD_H4(v8i16, __VA_ARGS__)

/* Description : Store vectors of 16 byte elements with stride
   Arguments   : Inputs - in0, in1, pdst, stride
   Details     : Store 16 byte elements from 'in0' to (pdst)
                 Store 16 byte elements from 'in1' to (pdst + stride)
*/
#define ST_B2(RTYPE, in0, in1, pdst, stride)  \
{                                             \
    ST_B(RTYPE, in0, (pdst));                 \
    ST_B(RTYPE, in1, (pdst) + stride);        \
}
#define ST_UB2(...) ST_B2(v16u8, __VA_ARGS__)

#define ST_B4(RTYPE, in0, in1, in2, in3, pdst, stride)    \
{                                                         \
    ST_B2(RTYPE, in0, in1, (pdst), stride);               \
    ST_B2(RTYPE, in2, in3, (pdst) + 2 * stride, stride);  \
}
#define ST_UB4(...) ST_B4(v16u8, __VA_ARGS__)
#define ST_SB4(...) ST_B4(v16i8, __VA_ARGS__)

/* Description : Store vectors of 8 halfword elements with stride
   Arguments   : Inputs - in0, in1, pdst, stride
   Details     : Store 8 halfword elements from 'in0' to (pdst)
                 Store 8 halfword elements from 'in1' to (pdst + stride)
*/
#define ST_H2(RTYPE, in0, in1, pdst, stride)  \
{                                             \
    ST_H(RTYPE, in0, (pdst));                 \
    ST_H(RTYPE, in1, (pdst) + stride);        \
}
#define ST_SH2(...) ST_H2(v8i16, __VA_ARGS__)

/* Description : Store 2x4 byte block to destination memory from input vector
   Arguments   : Inputs - in, stidx, pdst, stride
   Details     : Index 'stidx' halfword element from 'in' vector is copied to
                 the GP register and stored to (pdst)
                 Index 'stidx+1' halfword element from 'in' vector is copied to
                 the GP register and stored to (pdst + stride)
                 Index 'stidx+2' halfword element from 'in' vector is copied to
                 the GP register and stored to (pdst + 2 * stride)
                 Index 'stidx+3' halfword element from 'in' vector is copied to
                 the GP register and stored to (pdst + 3 * stride)
*/
#define ST2x4_UB(in, stidx, pdst, stride)             \
{                                                     \
    uint16_t out0_m, out1_m, out2_m, out3_m;          \
    uint8_t *pblk_2x4_m = (uint8_t *)(pdst);          \
                                                      \
    out0_m = __msa_copy_u_h((v8i16)in, (stidx));      \
    out1_m = __msa_copy_u_h((v8i16)in, (stidx + 1));  \
    out2_m = __msa_copy_u_h((v8i16)in, (stidx + 2));  \
    out3_m = __msa_copy_u_h((v8i16)in, (stidx + 3));  \
                                                      \
    SH(out0_m, pblk_2x4_m);                           \
    SH(out1_m, pblk_2x4_m + stride);                  \
    SH(out2_m, pblk_2x4_m + 2 * stride);              \
    SH(out3_m, pblk_2x4_m + 3 * stride);              \
}

/* Description : Store 4x4 byte block to destination memory from input vector
   Arguments   : Inputs - in0, in1, pdst, stride
   Details     : 'Idx0' word element from input vector 'in0' is copied to the
                 GP register and stored to (pdst)
                 'Idx1' word element from input vector 'in0' is copied to the
                 GP register and stored to (pdst + stride)
                 'Idx2' word element from input vector 'in0' is copied to the
                 GP register and stored to (pdst + 2 * stride)
                 'Idx3' word element from input vector 'in0' is copied to the
                 GP register and stored to (pdst + 3 * stride)
*/
#define ST4x4_UB(in0, in1, idx0, idx1, idx2, idx3, pdst, stride)  \
{                                                                 \
    uint32_t out0_m, out1_m, out2_m, out3_m;                      \
    uint8_t *pblk_4x4_m = (uint8_t *)(pdst);                      \
                                                                  \
    out0_m = __msa_copy_u_w((v4i32)in0, idx0);                    \
    out1_m = __msa_copy_u_w((v4i32)in0, idx1);                    \
    out2_m = __msa_copy_u_w((v4i32)in1, idx2);                    \
    out3_m = __msa_copy_u_w((v4i32)in1, idx3);                    \
                                                                  \
    SW4(out0_m, out1_m, out2_m, out3_m, pblk_4x4_m, stride);      \
}
#define ST4x8_UB(in0, in1, pdst, stride)                            \
{                                                                   \
    uint8_t *pblk_4x8 = (uint8_t *)(pdst);                          \
                                                                    \
    ST4x4_UB(in0, in0, 0, 1, 2, 3, pblk_4x8, stride);               \
    ST4x4_UB(in1, in1, 0, 1, 2, 3, pblk_4x8 + 4 * stride, stride);  \
}

/* Description : Shuffle byte vector elements as per mask vector
   Arguments   : Inputs  - in0, in1, in2, in3, mask0, mask1
                 Outputs - out0, out1
                 Return Type - as per RTYPE
   Details     : Byte elements from 'in0' & 'in1' are copied selectively to
                 'out0' as per control vector 'mask0'
*/
#define VSHF_B2(RTYPE, in0, in1, in2, in3, mask0, mask1, out0, out1)   \
{                                                                      \
    out0 = (RTYPE)__msa_vshf_b((v16i8)mask0, (v16i8)in1, (v16i8)in0);  \
    out1 = (RTYPE)__msa_vshf_b((v16i8)mask1, (v16i8)in3, (v16i8)in2);  \
}
#define VSHF_B2_SB(...) VSHF_B2(v16i8, __VA_ARGS__)

/* Description : Clips all signed halfword elements of input vector
                 between 0 & 255
   Arguments   : Input  - in
                 Output - out_m
                 Return Type - signed halfword
*/
#define CLIP_SH_0_255(in)                               \
({                                                      \
    v8i16 max_m = __msa_ldi_h(255);                     \
    v8i16 out_m;                                        \
                                                        \
    out_m = __msa_maxi_s_h((v8i16)in, 0);               \
    out_m = __msa_min_s_h((v8i16)max_m, (v8i16)out_m);  \
    out_m;                                              \
})
#define CLIP_SH2_0_255(in0, in1)  \
{                                 \
    in0 = CLIP_SH_0_255(in0);     \
    in1 = CLIP_SH_0_255(in1);     \
}
#define CLIP_SH4_0_255(in0, in1, in2, in3)  \
{                                           \
    CLIP_SH2_0_255(in0, in1);               \
    CLIP_SH2_0_255(in2, in3);               \
}

/* Description : Clips all signed word elements of input vector
                 between 0 & 255
   Arguments   : Input  - in
                 Output - out_m
                 Return Type - signed word
*/
#define CLIP_SW_0_255(in)                               \
({                                                      \
    v4i32 max_m = __msa_ldi_w(255);                     \
    v4i32 out_m;                                        \
                                                        \
    out_m = __msa_maxi_s_w((v4i32)in, 0);               \
    out_m = __msa_min_s_w((v4i32)max_m, (v4i32)out_m);  \
    out_m;                                              \
})

/* Description : Interleave even byte elements from vectors
   Arguments   : Inputs  - in0, in1, in2, in3
                 Outputs - out0, out1
                 Return Type - as per RTYPE
   Details     : Even byte elements of 'in0' and 'in1' are interleaved
                 and written to 'out0'
*/
#define ILVEV_B2(RTYPE, in0, in1, in2, in3, out0, out1)   \
{                                                         \
    out0 = (RTYPE)__msa_ilvev_b((v16i8)in1, (v16i8)in0);  \
    out1 = (RTYPE)__msa_ilvev_b((v16i8)in3, (v16i8)in2);  \
}
#define ILVEV_B2_SD(...) ILVEV_B2(v2i64, __VA_ARGS__)

/* Description : Interleave even halfword elements from vectors
   Arguments   : Inputs  - in0, in1, in2, in3
                 Outputs - out0, out1
                 Return Type - as per RTYPE
   Details     : Even halfword elements of 'in0' and 'in1' are interleaved
                 and written to 'out0'
*/
#define ILVEV_H2(RTYPE, in0, in1, in2, in3, out0, out1)   \
{                                                         \
    out0 = (RTYPE)__msa_ilvev_h((v8i16)in1, (v8i16)in0);  \
    out1 = (RTYPE)__msa_ilvev_h((v8i16)in3, (v8i16)in2);  \
}
#define ILVEV_H2_UB(...) ILVEV_H2(v16u8, __VA_ARGS__)

/* Description : Interleave even word elements from vectors
   Arguments   : Inputs  - in0, in1, in2, in3
                 Outputs - out0, out1
                 Return Type - as per RTYPE
   Details     : Even word elements of 'in0' and 'in1' are interleaved
                 and written to 'out0'
*/
#define ILVEV_W2(RTYPE, in0, in1, in2, in3, out0, out1)   \
{                                                         \
    out0 = (RTYPE)__msa_ilvev_w((v4i32)in1, (v4i32)in0);  \
    out1 = (RTYPE)__msa_ilvev_w((v4i32)in3, (v4i32)in2);  \
}
#define ILVEV_W2_SD(...) ILVEV_W2(v2i64, __VA_ARGS__)

/* Description : Interleave even double word elements from vectors
   Arguments   : Inputs  - in0, in1, in2, in3
                 Outputs - out0, out1
                 Return Type - as per RTYPE
   Details     : Even double word elements of 'in0' and 'in1' are interleaved
                 and written to 'out0'
*/
#define ILVEV_D2(RTYPE, in0, in1, in2, in3, out0, out1)   \
{                                                         \
    out0 = (RTYPE)__msa_ilvev_d((v2i64)in1, (v2i64)in0);  \
    out1 = (RTYPE)__msa_ilvev_d((v2i64)in3, (v2i64)in2);  \
}
#define ILVEV_D2_UB(...) ILVEV_D2(v16u8, __VA_ARGS__)

/* Description : Interleave left half of byte elements from vectors
   Arguments   : Inputs  - in0, in1, in2, in3
                 Outputs - out0, out1
                 Return Type - as per RTYPE
   Details     : Left half of byte elements of 'in0' and 'in1' are interleaved
                 and written to 'out0'.
*/
#define ILVL_B2(RTYPE, in0, in1, in2, in3, out0, out1)   \
{                                                        \
    out0 = (RTYPE)__msa_ilvl_b((v16i8)in0, (v16i8)in1);  \
    out1 = (RTYPE)__msa_ilvl_b((v16i8)in2, (v16i8)in3);  \
}
#define ILVL_B2_SH(...) ILVL_B2(v8i16, __VA_ARGS__)

/* Description : Interleave left half of halfword elements from vectors
   Arguments   : Inputs  - in0, in1, in2, in3
                 Outputs - out0, out1
                 Return Type - as per RTYPE
   Details     : Left half of halfword elements of 'in0' and 'in1' are
                 interleaved and written to 'out0'.
*/
#define ILVL_H2(RTYPE, in0, in1, in2, in3, out0, out1)   \
{                                                        \
    out0 = (RTYPE)__msa_ilvl_h((v8i16)in0, (v8i16)in1);  \
    out1 = (RTYPE)__msa_ilvl_h((v8i16)in2, (v8i16)in3);  \
}
#define ILVL_H2_SH(...) ILVL_H2(v8i16, __VA_ARGS__)
#define ILVL_H2_SW(...) ILVL_H2(v4i32, __VA_ARGS__)

/* Description : Interleave left half of word elements from vectors
   Arguments   : Inputs  - in0, in1, in2, in3
                 Outputs - out0, out1
                 Return Type - as per RTYPE
   Details     : Left half of word elements of 'in0' and 'in1' are interleaved
                 and written to 'out0'.
*/
#define ILVL_W2(RTYPE, in0, in1, in2, in3, out0, out1)   \
{                                                        \
    out0 = (RTYPE)__msa_ilvl_w((v4i32)in0, (v4i32)in1);  \
    out1 = (RTYPE)__msa_ilvl_w((v4i32)in2, (v4i32)in3);  \
}
#define ILVL_W2_SH(...) ILVL_W2(v8i16, __VA_ARGS__)

/* Description : Interleave right half of byte elements from vectors
   Arguments   : Inputs  - in0, in1, in2, in3
                 Outputs - out0, out1
                 Return Type - as per RTYPE
   Details     : Right half of byte elements of 'in0' and 'in1' are interleaved
                 and written to out0.
*/
#define ILVR_B2(RTYPE, in0, in1, in2, in3, out0, out1)   \
{                                                        \
    out0 = (RTYPE)__msa_ilvr_b((v16i8)in0, (v16i8)in1);  \
    out1 = (RTYPE)__msa_ilvr_b((v16i8)in2, (v16i8)in3);  \
}
#define ILVR_B2_SH(...) ILVR_B2(v8i16, __VA_ARGS__)
#define ILVR_B2_SW(...) ILVR_B2(v4i32, __VA_ARGS__)

#define ILVR_B4(RTYPE, in0, in1, in2, in3, in4, in5, in6, in7,  \
                out0, out1, out2, out3)                         \
{                                                               \
    ILVR_B2(RTYPE, in0, in1, in2, in3, out0, out1);             \
    ILVR_B2(RTYPE, in4, in5, in6, in7, out2, out3);             \
}
#define ILVR_B4_SH(...) ILVR_B4(v8i16, __VA_ARGS__)
#define ILVR_B4_SW(...) ILVR_B4(v4i32, __VA_ARGS__)

/* Description : Interleave right half of halfword elements from vectors
   Arguments   : Inputs  - in0, in1, in2, in3
                 Outputs - out0, out1
                 Return Type - as per RTYPE
   Details     : Right half of halfword elements of 'in0' and 'in1' are
                 interleaved and written to 'out0'.
*/
#define ILVR_H2(RTYPE, in0, in1, in2, in3, out0, out1)   \
{                                                        \
    out0 = (RTYPE)__msa_ilvr_h((v8i16)in0, (v8i16)in1);  \
    out1 = (RTYPE)__msa_ilvr_h((v8i16)in2, (v8i16)in3);  \
}
#define ILVR_H2_SH(...) ILVR_H2(v8i16, __VA_ARGS__)
#define ILVR_H2_SW(...) ILVR_H2(v4i32, __VA_ARGS__)

#define ILVR_H4(RTYPE, in0, in1, in2, in3, in4, in5, in6, in7,  \
                out0, out1, out2, out3)                         \
{                                                               \
    ILVR_H2(RTYPE, in0, in1, in2, in3, out0, out1);             \
    ILVR_H2(RTYPE, in4, in5, in6, in7, out2, out3);             \
}
#define ILVR_H4_SH(...) ILVR_H4(v8i16, __VA_ARGS__)
#define ILVR_H4_SW(...) ILVR_H4(v4i32, __VA_ARGS__)

#define ILVR_W2(RTYPE, in0, in1, in2, in3, out0, out1)   \
{                                                        \
    out0 = (RTYPE)__msa_ilvr_w((v4i32)in0, (v4i32)in1);  \
    out1 = (RTYPE)__msa_ilvr_w((v4i32)in2, (v4i32)in3);  \
}
#define ILVR_W2_SH(...) ILVR_W2(v8i16, __VA_ARGS__)

/* Description : Interleave right half of double word elements from vectors
   Arguments   : Inputs  - in0, in1, in2, in3
                 Outputs - out0, out1
                 Return Type - as per RTYPE
   Details     : Right half of double word elements of 'in0' and 'in1' are
                 interleaved and written to 'out0'.
*/
#define ILVR_D2(RTYPE, in0, in1, in2, in3, out0, out1)       \
{                                                            \
    out0 = (RTYPE)__msa_ilvr_d((v2i64)(in0), (v2i64)(in1));  \
    out1 = (RTYPE)__msa_ilvr_d((v2i64)(in2), (v2i64)(in3));  \
}
#define ILVR_D2_UB(...) ILVR_D2(v16u8, __VA_ARGS__)
#define ILVR_D2_SB(...) ILVR_D2(v16i8, __VA_ARGS__)
#define ILVR_D2_SH(...) ILVR_D2(v8i16, __VA_ARGS__)

#define ILVR_D4(RTYPE, in0, in1, in2, in3, in4, in5, in6, in7,  \
                out0, out1, out2, out3)                         \
{                                                               \
    ILVR_D2(RTYPE, in0, in1, in2, in3, out0, out1);             \
    ILVR_D2(RTYPE, in4, in5, in6, in7, out2, out3);             \
}
#define ILVR_D4_UB(...) ILVR_D4(v16u8, __VA_ARGS__)

/* Description : Interleave both left and right half of input vectors
   Arguments   : Inputs  - in0, in1
                 Outputs - out0, out1
                 Return Type - as per RTYPE
   Details     : Right half of byte elements from 'in0' and 'in1' are
                 interleaved and written to 'out0'
*/
#define ILVRL_B2(RTYPE, in0, in1, out0, out1)            \
{                                                        \
    out0 = (RTYPE)__msa_ilvr_b((v16i8)in0, (v16i8)in1);  \
    out1 = (RTYPE)__msa_ilvl_b((v16i8)in0, (v16i8)in1);  \
}
#define ILVRL_B2_SH(...) ILVRL_B2(v8i16, __VA_ARGS__)

#define ILVRL_H2(RTYPE, in0, in1, out0, out1)            \
{                                                        \
    out0 = (RTYPE)__msa_ilvr_h((v8i16)in0, (v8i16)in1);  \
    out1 = (RTYPE)__msa_ilvl_h((v8i16)in0, (v8i16)in1);  \
}
#define ILVRL_H2_SH(...) ILVRL_H2(v8i16, __VA_ARGS__)
#define ILVRL_H2_SW(...) ILVRL_H2(v4i32, __VA_ARGS__)

#define ILVRL_W2(RTYPE, in0, in1, out0, out1)            \
{                                                        \
    out0 = (RTYPE)__msa_ilvr_w((v4i32)in0, (v4i32)in1);  \
    out1 = (RTYPE)__msa_ilvl_w((v4i32)in0, (v4i32)in1);  \
}
#define ILVRL_W2_UB(...) ILVRL_W2(v16u8, __VA_ARGS__)
#define ILVRL_W2_SH(...) ILVRL_W2(v8i16, __VA_ARGS__)
#define ILVRL_W2_SW(...) ILVRL_W2(v4i32, __VA_ARGS__)

/* Description : Pack even byte elements of vector pairs
   Arguments   : Inputs  - in0, in1, in2, in3
                 Outputs - out0, out1
                 Return Type - as per RTYPE
   Details     : Even byte elements of 'in0' are copied to the left half of
                 'out0' & even byte elements of 'in1' are copied to the right
                 half of 'out0'.
*/
#define PCKEV_B2(RTYPE, in0, in1, in2, in3, out0, out1)   \
{                                                         \
    out0 = (RTYPE)__msa_pckev_b((v16i8)in0, (v16i8)in1);  \
    out1 = (RTYPE)__msa_pckev_b((v16i8)in2, (v16i8)in3);  \
}
#define PCKEV_B2_SB(...) PCKEV_B2(v16i8, __VA_ARGS__)
#define PCKEV_B2_UB(...) PCKEV_B2(v16u8, __VA_ARGS__)

#define PCKEV_B4(RTYPE, in0, in1, in2, in3, in4, in5, in6, in7,  \
                 out0, out1, out2, out3)                         \
{                                                                \
    PCKEV_B2(RTYPE, in0, in1, in2, in3, out0, out1);             \
    PCKEV_B2(RTYPE, in4, in5, in6, in7, out2, out3);             \
}
#define PCKEV_B4_SB(...) PCKEV_B4(v16i8, __VA_ARGS__)
#define PCKEV_B4_UB(...) PCKEV_B4(v16u8, __VA_ARGS__)
#define PCKEV_B4_SH(...) PCKEV_B4(v8i16, __VA_ARGS__)

/* Description : Pack even halfword elements of vector pairs
   Arguments   : Inputs  - in0, in1, in2, in3
                 Outputs - out0, out1
                 Return Type - as per RTYPE
   Details     : Even halfword elements of 'in0' are copied to the left half of
                 'out0' & even halfword elements of 'in1' are copied to the
                 right half of 'out0'.
*/
#define PCKEV_H2(RTYPE, in0, in1, in2, in3, out0, out1)   \
{                                                         \
    out0 = (RTYPE)__msa_pckev_h((v8i16)in0, (v8i16)in1);  \
    out1 = (RTYPE)__msa_pckev_h((v8i16)in2, (v8i16)in3);  \
}
#define PCKEV_H2_SH(...) PCKEV_H2(v8i16, __VA_ARGS__)

#define PCKEV_H4(RTYPE, in0, in1, in2, in3, in4, in5, in6, in7,  \
                 out0, out1, out2, out3)                         \
{                                                                \
    PCKEV_H2(RTYPE, in0, in1, in2, in3, out0, out1);             \
    PCKEV_H2(RTYPE, in4, in5, in6, in7, out2, out3);             \
}
#define PCKEV_H4_SH(...) PCKEV_H4(v8i16, __VA_ARGS__)

/* Description : Pack even double word elements of vector pairs
   Arguments   : Inputs  - in0, in1, in2, in3
                 Outputs - out0, out1
                 Return Type - as per RTYPE
   Details     : Even double elements of 'in0' are copied to the left half of
                 'out0' & even double elements of 'in1' are copied to the right
                 half of 'out0'.
*/
#define PCKEV_D2(RTYPE, in0, in1, in2, in3, out0, out1)   \
{                                                         \
    out0 = (RTYPE)__msa_pckev_d((v2i64)in0, (v2i64)in1);  \
    out1 = (RTYPE)__msa_pckev_d((v2i64)in2, (v2i64)in3);  \
}
#define PCKEV_D2_UB(...) PCKEV_D2(v16u8, __VA_ARGS__)
#define PCKEV_D2_SH(...) PCKEV_D2(v8i16, __VA_ARGS__)

/* Description : Pack odd double word elements of vector pairs
   Arguments   : Inputs  - in0, in1, in2, in3
                 Outputs - out0, out1
                 Return Type - as per RTYPE
   Details     : Odd double word elements of 'in0' are copied to the left half
                 of 'out0' & odd double word elements of 'in1' are copied to
                 the right half of 'out0'.
*/
#define PCKOD_D2(RTYPE, in0, in1, in2, in3, out0, out1)   \
{                                                         \
    out0 = (RTYPE)__msa_pckod_d((v2i64)in0, (v2i64)in1);  \
    out1 = (RTYPE)__msa_pckod_d((v2i64)in2, (v2i64)in3);  \
}
#define PCKOD_D2_UB(...) PCKOD_D2(v16u8, __VA_ARGS__)
#define PCKOD_D2_SH(...) PCKOD_D2(v8i16, __VA_ARGS__)

/* Description : Arithmetic shift right all elements of vector
                 (generic for all data types)
   Arguments   : Inputs  - in0, in1, in2, in3, shift
                 Outputs - in place operation
                 Return Type - as per input vector RTYPE
   Details     : Each element of vector 'in0' is right shifted by 'shift' and
                 the result is written in-place. 'shift' is a GP variable.
*/
#define SRA_4V(in0, in1, in2, in3, shift)  \
{                                          \
    in0 = in0 >> shift;                    \
    in1 = in1 >> shift;                    \
    in2 = in2 >> shift;                    \
    in3 = in3 >> shift;                    \
}

/* Description : Shift right arithmetic rounded (immediate)
   Arguments   : Inputs  - in0, in1, shift
                 Outputs - in place operation
                 Return Type - as per RTYPE
   Details     : Each element of vector 'in0' is shifted right arithmetically by
                 the value in 'shift'. The last discarded bit is added to the
                 shifted value for rounding and the result is written in-place.
                 'shift' is an immediate value.
*/
#define SRARI_H2(RTYPE, in0, in1, shift)            \
{                                                   \
    in0 = (RTYPE)__msa_srari_h((v8i16)in0, shift);  \
    in1 = (RTYPE)__msa_srari_h((v8i16)in1, shift);  \
}
#define SRARI_H2_UH(...) SRARI_H2(v8u16, __VA_ARGS__)
#define SRARI_H2_SH(...) SRARI_H2(v8i16, __VA_ARGS__)

#define SRARI_W2(RTYPE, in0, in1, shift)            \
{                                                   \
    in0 = (RTYPE)__msa_srari_w((v4i32)in0, shift);  \
    in1 = (RTYPE)__msa_srari_w((v4i32)in1, shift);  \
}

#define SRARI_W4(RTYPE, in0, in1, in2, in3, shift)  \
{                                                   \
    SRARI_W2(RTYPE, in0, in1, shift);               \
    SRARI_W2(RTYPE, in2, in3, shift);               \
}
#define SRARI_W4_SW(...) SRARI_W4(v4i32, __VA_ARGS__)

/* Description : Multiplication of pairs of vectors
   Arguments   : Inputs  - in0, in1, in2, in3
                 Outputs - out0, out1
   Details     : Each element from 'in0' is multiplied with elements from 'in1'
                 and the result is written to 'out0'
*/
#define MUL2(in0, in1, in2, in3, out0, out1)  \
{                                             \
    out0 = in0 * in1;                         \
    out1 = in2 * in3;                         \
}
#define MUL4(in0, in1, in2, in3, in4, in5, in6, in7,  \
             out0, out1, out2, out3)                  \
{                                                     \
    MUL2(in0, in1, in2, in3, out0, out1);             \
    MUL2(in4, in5, in6, in7, out2, out3);             \
}

/* Description : Addition of 2 pairs of vectors
   Arguments   : Inputs  - in0, in1, in2, in3
                 Outputs - out0, out1
   Details     : Each element in 'in0' is added to 'in1' and result is written
                 to 'out0'.
*/
#define ADD2(in0, in1, in2, in3, out0, out1)  \
{                                             \
    out0 = in0 + in1;                         \
    out1 = in2 + in3;                         \
}
#define ADD4(in0, in1, in2, in3, in4, in5, in6, in7,  \
             out0, out1, out2, out3)                  \
{                                                     \
    ADD2(in0, in1, in2, in3, out0, out1);             \
    ADD2(in4, in5, in6, in7, out2, out3);             \
}

/* Description : Sign extend halfword elements from input vector and return
                 the result in pair of vectors
   Arguments   : Input   - in            (halfword vector)
                 Outputs - out0, out1   (sign extended word vectors)
                 Return Type - signed word
   Details     : Sign bit of halfword elements from input vector 'in' is
                 extracted and interleaved right with same vector 'in0' to
                 generate 4 signed word elements in 'out0'
                 Then interleaved left with same vector 'in0' to
                 generate 4 signed word elements in 'out1'
*/
#define UNPCK_SH_SW(in, out0, out1)        \
{                                          \
    v8i16 tmp_m;                           \
                                           \
    tmp_m = __msa_clti_s_h((v8i16)in, 0);  \
    ILVRL_H2_SW(tmp_m, in, out0, out1);    \
}

/* Description : Butterfly of 4 input vectors
   Arguments   : Inputs  - in0, in1, in2, in3
                 Outputs - out0, out1, out2, out3
   Details     : Butterfly operation
*/
#define BUTTERFLY_4(in0, in1, in2, in3, out0, out1, out2, out3)  \
{                                                                \
    out0 = in0 + in3;                                            \
    out1 = in1 + in2;                                            \
                                                                 \
    out2 = in1 - in2;                                            \
    out3 = in0 - in3;                                            \
}

/* Description : Transpose 16x4 block into 4x16 with byte elements in vectors
   Arguments   : Inputs  - in0, in1, in2, in3, in4, in5, in6, in7,
                           in8, in9, in10, in11, in12, in13, in14, in15
                 Outputs - out0, out1, out2, out3
                 Return Type - unsigned byte
*/
#define TRANSPOSE16x4_UB_UB(in0, in1, in2, in3, in4, in5, in6, in7,        \
                            in8, in9, in10, in11, in12, in13, in14, in15,  \
                            out0, out1, out2, out3)                        \
{                                                                          \
    v2i64 tmp0_m, tmp1_m, tmp2_m, tmp3_m;                                  \
                                                                           \
    ILVEV_W2_SD(in0, in4, in8, in12, tmp0_m, tmp1_m);                      \
    out1 = (v16u8)__msa_ilvev_d(tmp1_m, tmp0_m);                           \
                                                                           \
    ILVEV_W2_SD(in1, in5, in9, in13, tmp0_m, tmp1_m);                      \
    out3 = (v16u8)__msa_ilvev_d(tmp1_m, tmp0_m);                           \
                                                                           \
    ILVEV_W2_SD(in2, in6, in10, in14, tmp0_m, tmp1_m);                     \
                                                                           \
    tmp2_m = __msa_ilvev_d(tmp1_m, tmp0_m);                                \
    ILVEV_W2_SD(in3, in7, in11, in15, tmp0_m, tmp1_m);                     \
                                                                           \
    tmp3_m = __msa_ilvev_d(tmp1_m, tmp0_m);                                \
    ILVEV_B2_SD(out1, out3, tmp2_m, tmp3_m, tmp0_m, tmp1_m);               \
    out0 = (v16u8)__msa_ilvev_h((v8i16)tmp1_m, (v8i16)tmp0_m);             \
    out2 = (v16u8)__msa_ilvod_h((v8i16)tmp1_m, (v8i16)tmp0_m);             \
                                                                           \
    tmp0_m = (v2i64)__msa_ilvod_b((v16i8)out3, (v16i8)out1);               \
    tmp1_m = (v2i64)__msa_ilvod_b((v16i8)tmp3_m, (v16i8)tmp2_m);           \
    out1 = (v16u8)__msa_ilvev_h((v8i16)tmp1_m, (v8i16)tmp0_m);             \
    out3 = (v16u8)__msa_ilvod_h((v8i16)tmp1_m, (v8i16)tmp0_m);             \
}

/* Description : Transpose 16x8 block into 8x16 with byte elements in vectors
   Arguments   : Inputs  - in0, in1, in2, in3, in4, in5, in6, in7,
                           in8, in9, in10, in11, in12, in13, in14, in15
                 Outputs - out0, out1, out2, out3, out4, out5, out6, out7
                 Return Type - unsigned byte
*/
#define TRANSPOSE16x8_UB_UB(in0, in1, in2, in3, in4, in5, in6, in7,          \
                            in8, in9, in10, in11, in12, in13, in14, in15,    \
                            out0, out1, out2, out3, out4, out5, out6, out7)  \
{                                                                            \
    v16u8 tmp0_m, tmp1_m, tmp2_m, tmp3_m;                                    \
    v16u8 tmp4_m, tmp5_m, tmp6_m, tmp7_m;                                    \
                                                                             \
    ILVEV_D2_UB(in0, in8, in1, in9, out7, out6);                             \
    ILVEV_D2_UB(in2, in10, in3, in11, out5, out4);                           \
    ILVEV_D2_UB(in4, in12, in5, in13, out3, out2);                           \
    ILVEV_D2_UB(in6, in14, in7, in15, out1, out0);                           \
                                                                             \
    tmp0_m = (v16u8)__msa_ilvev_b((v16i8)out6, (v16i8)out7);                 \
    tmp4_m = (v16u8)__msa_ilvod_b((v16i8)out6, (v16i8)out7);                 \
    tmp1_m = (v16u8)__msa_ilvev_b((v16i8)out4, (v16i8)out5);                 \
    tmp5_m = (v16u8)__msa_ilvod_b((v16i8)out4, (v16i8)out5);                 \
    out5 = (v16u8)__msa_ilvev_b((v16i8)out2, (v16i8)out3);                   \
    tmp6_m = (v16u8)__msa_ilvod_b((v16i8)out2, (v16i8)out3);                 \
    out7 = (v16u8)__msa_ilvev_b((v16i8)out0, (v16i8)out1);                   \
    tmp7_m = (v16u8)__msa_ilvod_b((v16i8)out0, (v16i8)out1);                 \
                                                                             \
    ILVEV_H2_UB(tmp0_m, tmp1_m, out5, out7, tmp2_m, tmp3_m);                 \
    out0 = (v16u8)__msa_ilvev_w((v4i32)tmp3_m, (v4i32)tmp2_m);               \
    out4 = (v16u8)__msa_ilvod_w((v4i32)tmp3_m, (v4i32)tmp2_m);               \
                                                                             \
    tmp2_m = (v16u8)__msa_ilvod_h((v8i16)tmp1_m, (v8i16)tmp0_m);             \
    tmp3_m = (v16u8)__msa_ilvod_h((v8i16)out7, (v8i16)out5);                 \
    out2 = (v16u8)__msa_ilvev_w((v4i32)tmp3_m, (v4i32)tmp2_m);               \
    out6 = (v16u8)__msa_ilvod_w((v4i32)tmp3_m, (v4i32)tmp2_m);               \
                                                                             \
    ILVEV_H2_UB(tmp4_m, tmp5_m, tmp6_m, tmp7_m, tmp2_m, tmp3_m);             \
    out1 = (v16u8)__msa_ilvev_w((v4i32)tmp3_m, (v4i32)tmp2_m);               \
    out5 = (v16u8)__msa_ilvod_w((v4i32)tmp3_m, (v4i32)tmp2_m);               \
                                                                             \
    tmp2_m = (v16u8)__msa_ilvod_h((v8i16)tmp5_m, (v8i16)tmp4_m);             \
    tmp2_m = (v16u8)__msa_ilvod_h((v8i16)tmp5_m, (v8i16)tmp4_m);             \
    tmp3_m = (v16u8)__msa_ilvod_h((v8i16)tmp7_m, (v8i16)tmp6_m);             \
    tmp3_m = (v16u8)__msa_ilvod_h((v8i16)tmp7_m, (v8i16)tmp6_m);             \
    out3 = (v16u8)__msa_ilvev_w((v4i32)tmp3_m, (v4i32)tmp2_m);               \
    out7 = (v16u8)__msa_ilvod_w((v4i32)tmp3_m, (v4i32)tmp2_m);               \
}

/* Description : Transpose 8x4 block with half word elements in vectors
   Arguments   : Inputs  - in0, in1, in2, in3, in4, in5, in6, in7
                 Outputs - out0, out1, out2, out3, out4, out5, out6, out7
                 Return Type - signed halfword
*/
#define TRANSPOSE8X4_SH_SH(in0, in1, in2, in3, out0, out1, out2, out3)  \
{                                                                       \
    v8i16 tmp0_m, tmp1_m, tmp2_m, tmp3_m;                               \
                                                                        \
    ILVR_H2_SH(in1, in0, in3, in2, tmp0_m, tmp1_m);                     \
    ILVL_H2_SH(in1, in0, in3, in2, tmp2_m, tmp3_m);                     \
    ILVR_W2_SH(tmp1_m, tmp0_m, tmp3_m, tmp2_m, out0, out2);             \
    ILVL_W2_SH(tmp1_m, tmp0_m, tmp3_m, tmp2_m, out1, out3);             \
}

/* Description : Transpose 4x4 block with word elements in vectors
   Arguments   : Inputs  - in0, in1, in2, in3
                 Outputs - out0, out1, out2, out3
                 Return Type - signed word
*/
#define TRANSPOSE4x4_SW_SW(in0, in1, in2, in3, out0, out1, out2, out3)  \
{                                                                       \
    v4i32 s0_m, s1_m, s2_m, s3_m;                                       \
                                                                        \
    ILVRL_W2_SW(in1, in0, s0_m, s1_m);                                  \
    ILVRL_W2_SW(in3, in2, s2_m, s3_m);                                  \
                                                                        \
    out0 = (v4i32)__msa_ilvr_d((v2i64)s2_m, (v2i64)s0_m);               \
    out1 = (v4i32)__msa_ilvl_d((v2i64)s2_m, (v2i64)s0_m);               \
    out2 = (v4i32)__msa_ilvr_d((v2i64)s3_m, (v2i64)s1_m);               \
    out3 = (v4i32)__msa_ilvl_d((v2i64)s3_m, (v2i64)s1_m);               \
}
#endif  /* VP8_COMMON_MIPS_MSA_VP8_MACROS_MSA_H_ */
