# Notes about Metal
Metal is OOP so everything is a class. MTLDevice creates EVERYTHING. For example, MTL4CommandQueue is made using a factory method in MTLDevice.

- Command Queue
    - Jobs sent to the GPU to be executed
    - Current process is not the only process sending work to the Command Queue. Apple will often submit work to be done on the GPU concurrently
- Command Buffers 
    - The work being submitted to Command Queue
    - Created by Command Queue method
    - Stores all commands that are encoded
    - Equivalent to CUDA stream
- Pipeline
    - GPU state that is needed for the current command to be executed. 
    - Choose between Compute, Render, and Blit to process GPU kernel code. You load a kernel into a respective pipeline depending on what the kernel does
- Command Allocator 
    - provides memory for Command Buffer
- Command Encoder
    - Encodes commands into command buffers
    - Dedicated encoders for types of work (Render, Compute, Blit)
    - It allows for implementations for ML, Rendering, and Compute
- Argument tables
    - Store bindings to resources like data buffers, textures, samplers, etc.
    - Before MTL4, you used to have to setBuffer, setTexture for every encoder. Argument Table bypasses that by binding a table with buffer encodings
- Barriers
    - synchronization primitives that ensure correct ordering of resource access across pipeline stages—like compute, vertex, or fragment

## Metal Encoder Types:
- Render
    - running shaders, rasterizing triangles, shading pixels
    - uses dedicated hardware on GPU to render
- Compute
    - general computation that user defines
- Blit
    - copy buffer/texture info to each other. Filling in bytes in a buffer and generating mipmaps
- Mipmap
    - maximize and minimize images i.e. 256x256 to 128x128.

### Threads & Thread Groups:
- threads = single unit of work done by GPU
- threads can be organized into at most 3-D matrices

### Types of Shaders
- Vertex = takes vertex data and transforms it. Basically does transformations one vertex at a time
- Fragment = shades each vertex by applying lighting, texture, and color. Runs once per pixel 
- Kernel = general purpose parallelized programming

## Steps to Create and Run Metal Kernel:
- Write Kernel Function in .metal file. This will basically take in parameter buffers from shared memory. Do some operation on the buffers and return it back into a different buffer. This kernel function will then be called within a different function. The .metal file then needs to be compiled into a metallib. Think of .metal as a .cpp file and .metallib as a .o file
- Initialize Metal Context
    - Set Device to Metal GPU by MTLCreateSystemDefaultDevice(). Not a singleton.
    - Create Command Queue from device method
- Create Command Buffer
    - Set pipeline state
    - Set description of workload. For example, if the job is rendering, then the geometry buffer, fragment buffer, and etc should be set.
    - Call Dispatch thrads to say how many threads will be used by the shader. The shader itself usually has no for loops, but it can if the necessary amount of work is larger than the number of threads that exist
    - Each buffer can have more than one command. Once you finish a command, you can queue another command over it to be done by the GPU all in the same job
    - End encoding is called to finalize the job
- Commit to send buffer to command qeue
- Wait until job is completed by either making the CPU wait, or subscribe to completion event so that CPU can do other work in meantime
- Make default library and make function with string name of whatever gpu kernel function name is. This library contains all the GPU kernel functions created

## Keywords:
Device:  device address space to declare persistent memory that the GPU can read from and write to  
MPS (Metal Performance Shader): Essentially same as CuBlas. Collection of shaders for popular operations  

