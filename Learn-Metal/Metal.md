# Notes about Metal

Metal is OOP so everything is a class. MTLDevice creates EVERYTHING. For example, MTL4CommandQueue is made using a factory method in MTLDevice
- Pipeline
    - Choose between Compute and Render to process GPU kernel code. You load a kernel into a respective pipeline depending on what the kernel does
- Command Buffers 
    - Essentially Jobs submitted to Command Queue
    - Stores all commands that are encoded
    - Equivalent to CUDA stream
- Command Allocator 
    - provides memory for Command Buffer
- Command Encoder
    - Is an interface
    - Encodes commands into command buffers
    - It allows for implementations for ML, Rendering, and Compute
- Argument tables
    - Store bindings to resources like data buffers, textures, samplers, etc.
    - Before MTL4, you used to have to setBuffer, setTexture for every encoder. Argument Table bypasses that by binding a table with buffer encodings
- Barriers
    - synchronization primitives that ensure correct ordering of resource access across pipeline stages—like compute, vertex, or fragment

## Steps to Create and Run Metal Kernel:
- Write Kernel Function in .metal file. This will basically take in parameter buffers from shared memory. Do some operation on the buffers and return it back into a different buffer. This kernel function will then be called within a different function. The .metal file then needs to be compiled into a metallib. Think of .metal as a .cpp file and .metallib as a .o file
- Set Device to Metal GPU
- Make default library and make function with string name of whatever gpu kernel function name is. This library contains all the GPU kernel functions created
- Create a Command Queue
- Sends commands into the GPU FIFO Command Queue
- Compute Pipeline State
- Create MTL buffers in shared memory
- Create command buffer and command encoder. Command encoder in charge of setting pipeline state to be the GPU kernel function
- Figure out how many threads need to be used for the function. Then dispatch threads which basically says how many threads will be used
- Stop command encoder from encoding these preliminary information
- commandBuffer.commit() is in charge of actually sending kernel function to queue

## Keywords:
Device:  device address space to declare persistent memory that the GPU can read from and write to  
MPS (Metal Performance Shader): Essentially same as CuBlas. Collection of shaders for popular operations  

