# General GPU Programming Notes

## SIMT (Single Instruction Multiple Threads)
Very similar to SIMD except that each thread has their own set of memory. This is useful because there is no need to constantly write temporary states to global memory. Instead, threads can write to a register or something and aggregate in shared memory. SIMT can do branches, but the branches end up being executed serially.

# Vocab
Shader: code that runs on GPU  
Buffer: GPU memory (can be shared with CPU in Metal as GPU and CPU are on same device)  
GPGPU: General Purpose computing on Graphics Processing Units