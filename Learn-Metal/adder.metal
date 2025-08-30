//
//  metal.metal
//  Learn-Metal
//
//  Created by Philip Yi on 8/30/25.
//

#include <metal_stdlib>
using namespace metal;

kernel void double_it(const device float* in  [[buffer(0)]],
                      device float*       out [[buffer(1)]],
                      uint gid [[thread_position_in_grid]]) {
    out[gid] = in[gid] * 2.0;
}

