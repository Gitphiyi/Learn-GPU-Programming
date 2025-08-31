//
//  main.cpp
//  Learn-Metal
//
//  Created by Philip Yi on 8/30/25.
//
#define NS_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION

#include <Foundation/Foundation.hpp>
#include <Metal/Metal.hpp>
#include <QuartzCore/QuartzCore.hpp>

#include <iostream>
#include <fstream>

int main(int argc, const char * argv[]) {
    //init Metal Context
    MTL::Device* device = MTL::CreateSystemDefaultDevice();
    if (!device) { std::cerr << "No Metal device.\n"; return 1; }
    MTL::CommandQueue* queue = device->newCommandQueue();
    
    //init GPU kernel
    NS::Error* err = nullptr;
    auto lib = device->newDefaultLibrary();
    if (!lib) {
        std::cerr << "Library load error: "
                  << (err ? err->localizedDescription()->utf8String() : "unknown")
                  << "\n";
        return 1;
    }
    
    // bring function and create compute pipeline
    MTL::Function* fn = lib->newFunction(NS::String::string("double_it", NS::UTF8StringEncoding));
    if (!fn) { std::cerr << "Missing function.\n"; return 1; }

    MTL::ComputePipelineState* pso = device->newComputePipelineState(fn, &err);
    if (!pso) {
        std::cerr << "PSO error: " << (err ? err->localizedDescription()->utf8String() : "unknown") << "\n";
        return 1;
    }
    
    //Create buffers
    const size_t N = 1024;
    const size_t bytes = N * sizeof(float);
    std::vector<float> hostIn(N), hostOut(N);

    for (size_t i = 0; i < N; ++i) hostIn[i] = float(i); // fill input

    MTL::Buffer* inBuf  = device->newBuffer(bytes, MTL::ResourceStorageModeShared);
    MTL::Buffer* outBuf = device->newBuffer(bytes, MTL::ResourceStorageModeShared);

    std::memcpy(inBuf->contents(), hostIn.data(), bytes); //fill the input buffer with info

    //Record commands
    MTL::CommandBuffer* cb = queue->commandBuffer();
    MTL::ComputeCommandEncoder* enc = cb->computeCommandEncoder();

    enc->setComputePipelineState(pso);
    enc->setBuffer(inBuf,  0, 0);
    enc->setBuffer(outBuf, 0, 1);
    
    // Choose a simple launch config
    const NS::UInteger threadsPerThreadgroup = std::min<NS::UInteger>(256, pso->maxTotalThreadsPerThreadgroup());
    const NS::UInteger threadsPerGrid        = N;

    // Use dispatchThreads for exact grid sizing
    enc->dispatchThreads(MTL::Size(threadsPerGrid, 1, 1),
                         MTL::Size(threadsPerThreadgroup, 1, 1));
    enc->endEncoding();

    cb->commit();
    cb->waitUntilCompleted();

    // 6) Read back
    std::memcpy(hostOut.data(), outBuf->contents(), bytes);
    
    for(int i = 0; i < hostOut.size(); i++) {
        std::cout << "out[" << i << "]= " << hostOut[i] << std::endl;
    }
    // 7) Cleanup
    outBuf->release();
    inBuf->release();
    pso->release();
    fn->release();
    lib->release();
    queue->release();
    device->release();
    return 0;
}
