#include "opencv2/opencv.hpp"
#include "opencv2/core/cuda.hpp"
#include <iostream>

using namespace std;
using namespace cv;

/* @ function main */
int main( int argc, char *argv[] )
{
    // CUDA device count
    int nDeviceCount = 0;
    nDeviceCount = cuda::getCudaEnabledDeviceCount();

    // CUDA device index
    int nCurrentDeviceIndex = 0;
    nCurrentDeviceIndex = cuda::getDevice();

    // CUDA device info
    cuda::DeviceInfo deviceInfo = cuda::DeviceInfo( nCurrentDeviceIndex );

    // CUDA device name
    const char* deviceName = deviceInfo.name();

    // global memory available on device in bytes
    const size_t deviceTotalGlobMem = deviceInfo.totalGlobalMem();

    // shared memory available per block in bytes
    const size_t deviceSharedMemPerBlock = deviceInfo.sharedMemPerBlock();

    // 32-bit registers available per block
    const int nDeviceegsPerBlock = deviceInfo.regsPerBlock();

    // warp size in threads
    const int nDeviceWarpSize = deviceInfo.warpSize();

    // maximum pitch in bytes allowed by memory copies
    const size_t deviceMemPitch = deviceInfo.memPitch();

    // maximum threads per block
    int nMaxThreadPerBlock = deviceInfo.maxThreadsPerBlock();

    // maximum size of each dimension of a block
    const Vec3i deviceMaxThreadsDim = deviceInfo.maxThreadsDim();

    // maximum size of each dimension of a grid
    const Vec3i deviceMaxGridSize = deviceInfo.maxGridSize();

    // clock frequency in kilohertz
    const int nDeviceClockRate = deviceInfo.clockRate();

    // constant memory available on device in bytes
    const size_t deviceTotalConstMem = deviceInfo.totalConstMem();

    // major compute capability
    const int nDeiveMajorVersion = deviceInfo.majorVersion();

    // minor compute capability
    const int nDeiveMinorVersion = deviceInfo.minorVersion();

    // pitch alignment requirement for texture references bound to pitched memory
    const size_t devicetexturePitchAlignment = deviceInfo.texturePitchAlignment();

    // number of multiprocessors on device
    const int nDeviceMultiProcessorCount = deviceInfo.multiProcessorCount();

    // specified whether there is a run time limit on kernels
    const bool bKernelExecTimeoutEnabled = deviceInfo.kernelExecTimeoutEnabled();
    string tmp1 = bKernelExecTimeoutEnabled ? "true" : "false";

    // device is integrated as opposed to discrete
    const bool bIDeviceIntegrated = deviceInfo.integrated();
    string tmp2 = bIDeviceIntegrated? "true" : "false";

    // device can map host memory with cudaHostAlloc/cudaHostGetDevicePointer
    const bool bCanMapHostMemory = deviceInfo.canMapHostMemory();
    string tmp3 = bCanMapHostMemory? "true" : "false";

    // maximum 1D texture size
    const int nMaxTex1D =  deviceInfo.maxTexture1D();

    // maximum 1D mipmapped texture size
    const int nMaxTexture1DMipmap = deviceInfo.maxTexture1DMipmap();

    // maximum size for 1D textures bound to linear memory
    const int nMaxTexture1DLinear = deviceInfo.maxTexture1DLinear();

    // maximum 2D texture dimensions
    const Vec2i deviceMaxTex2D = deviceInfo.maxTexture2D();

    // maximum 2D mipmapped texture dimensions
    const Vec2i deviceMaxTex2DMipmap = deviceInfo.maxTexture2DMipmap();

    // maximum dimensions (width, height, pitch) for 2D textures bound to pitched memory
    const Vec3i deviceMaxTex2DLinear = deviceInfo.maxTexture2DLinear();

    // maximum 2D texture dimensions if texture gather operations have to be performed
    const Vec2i deviceMaxTex2DGather = deviceInfo.maxTexture2DGather();

    // maximum 3D texture dimensions
    const Vec3i deviceMaxTex3D = deviceInfo.maxTexture3D();

    // maximum Cubemap texture dimensions
    const int deviceMaximumTexCubemap = deviceInfo.maxTextureCubemap();

    // maximum 1D layered texture dimensions
    const Vec2i deviceMaxTex1DLayered = deviceInfo.maxTexture1DLayered();

    // maximum 2D layered texture dimensions
    const Vec3i deviceMaxTex2DLayered = deviceInfo.maxTexture2DLayered();

    // maximum Cubemap layered texture dimensions
    const Vec2i deviceMaxTexCubemapLayered = deviceInfo.maxTextureCubemapLayered();

    // maximum 1D surface size
    const int deviceMaxSurface1D = deviceInfo.maxSurface1D();

    // maximum 2D surface dimensions
    const Vec2i deviceMaxSurface2D = deviceInfo.maxSurface2D();

    // maximum 3D surface dimensions
    const Vec3i deviceMaxSurface3D = deviceInfo.maxSurface3D();

    // maximum 1D layered surface dimensions
    const Vec2i deviceMaxSurf1DLayered = deviceInfo.maxSurface1DLayered();

    // maximum 2D layered surface dimensions
    const Vec3i deviceMaxSurf2DLayered = deviceInfo.maxSurface2DLayered();

    // maximum Cubemap surface dimensions
    const int deviceMaxSurfCubemap = deviceInfo.maxSurfaceCubemap();

    // maximum Cubemap layered surface dimensions
    const Vec2i deviceMaxSurfCubemapLayered = deviceInfo.maxSurfaceCubemapLayered();

    // alignment requirements for surfaces
    const size_t deviceSurfAlignment = deviceInfo.surfaceAlignment();


    // device can possibly execute multiple kernels concurrently
    const bool bConKernels = deviceInfo.concurrentKernels();
    string tmp4 = bConKernels? "true" : "false";

    // device has ECC support enabled
    const bool bECCEnabled = deviceInfo.ECCEnabled();
    string tmp5 = bECCEnabled? "true" : "false";

    // PCI bus ID of the device
    const int devicePciBusId = deviceInfo.pciBusID();

    // PCI device ID of the device
    const int devicePciDeviceId = deviceInfo.pciDeviceID();

    // PCI domain ID of the device
    const int devicePciDomainId = deviceInfo.pciDomainID();

    // true if device is a Tesla device using TCC driver, false otherwise
    const bool deviceTccDriver = deviceInfo.tccDriver();

    // number of asynchronous engines
    const int deviceAsyncEngineCount = deviceInfo.asyncEngineCount();

    // device shares a unified address space with the host
    const bool bUnifiedAddressing = deviceInfo.unifiedAddressing();
    string tmp6 = bUnifiedAddressing? "true" : "false";

    // peak memory clock frequency in kilohertz
    const int deviceMemClockRate = deviceInfo.memoryClockRate();

    // global memory bus width in bits
    const int deviceMemBusWidth = deviceInfo.memoryBusWidth();

    // size of L2 cache in bytes
    const int deviceL2CacheSize = deviceInfo.l2CacheSize();

    // maximum resident threads per multiprocessor
    const int deviceMaxThreadsPerMultiProcessor = deviceInfo.maxThreadsPerMultiProcessor();

    // checks whether the CUDA module can be run on the given device
    const bool bIsCompatible = deviceInfo.isCompatible();
    string tmp7 = bIsCompatible? "true" : "false";

    // print the results
    cout << "CUDA Enabled Device Count: " << nDeviceCount << endl;
    cout << "CUDA Device Index: " << nCurrentDeviceIndex << endl;
    cout << "Device Name: " << deviceName << endl;
    cout << "Device Total Global Memory: " << deviceTotalGlobMem << endl;
    cout << "Device Shared Memory Per Block: " << deviceSharedMemPerBlock << endl;
    cout << "Device Registry Per BlocK: " << nDeviceegsPerBlock << endl;
    cout << "Device Warp Size: " << nDeviceWarpSize << endl;
    cout << "Device Memory Pitch: " << deviceMemPitch << endl;
    cout << "Maximum Threads Per Block: " << nMaxThreadPerBlock << endl;
    cout << "Maximum Threads Dimension: " << deviceMaxThreadsDim << endl;
    cout << "Maximum Grid Size: " << deviceMaxGridSize << endl;
    cout << "Device Clock Rate: " << nDeviceClockRate << endl;
    cout << "Device Total Constant Memory: " << deviceTotalConstMem << endl;
    cout << "Device Major Version: " << nDeiveMajorVersion << endl;
    cout << "Device Minor Version: " << nDeiveMinorVersion << endl;
    cout << "Device Texture Pitch Alignment: " << devicetexturePitchAlignment << endl;
    cout << "Device Multi Processor Count: " << nDeviceMultiProcessorCount << endl;
    cout << "Device Kernel Execution Timeout Enabled?: " << tmp1 << endl;
    cout << "Device Integrated?: " << tmp2 << endl;
    cout << "Device Can Map Host Memory?: " << tmp3 << endl;
    cout << "Device Maximum Texture 1D: " << nMaxTex1D << endl;
    cout << "Device maximum Texture 1D Mipmap: " << nMaxTexture1DMipmap << endl;
    cout << "Device Maximum Texture 1D Linear: " << nMaxTexture1DLinear << endl;
    cout << "Device Maximum Texture 2D: " << deviceMaxTex2D << endl;
    cout << "Device Maximum Texture 2D Mipmap: " << deviceMaxTex2DMipmap << endl;
    cout << "Device Maximum Texture 2D Linear: " << deviceMaxTex2DLinear << endl;
    cout << "Device Maximum Texutre 2D Gather: " << deviceMaxTex2DGather << endl;
    cout << "Device Maximum Texture 3D: " << deviceMaxTex3D << endl;
    cout << "Device Maximum Texture Cubemap: " << deviceMaximumTexCubemap << endl;
    cout << "Device Maximum Texture 1D Layered: " << deviceMaxTex1DLayered << endl;
    cout << "Device Maximum Texture 2D Layered: " << deviceMaxTex2DLayered << endl;
    cout << "Device Maximum Texture Cubemap Layered: " << deviceMaxTexCubemapLayered << endl;
    cout << "Device Maximum Surface 1D: " << deviceMaxSurface1D << endl;
    cout << "Device Maximum Surface 2D: " << deviceMaxSurface2D << endl;
    cout << "Device Maximum Surface 3D: " << deviceMaxSurface3D << endl;
    cout << "Device Maximum Surface 1D Layered: " << deviceMaxSurf1DLayered << endl;
    cout << "Device Maximum Surface 2D Layered: " << deviceMaxSurf2DLayered << endl;
    cout << "Device Maximum Surface Cubemap: " << deviceMaxSurfCubemap << endl;
    cout << "Device Maximum Surface Cubemap Layered: " << deviceMaxSurfCubemapLayered << endl;
    cout << "Device Surfce Alignment: " << deviceSurfAlignment << endl;
    cout << "Concurrent Kernels?: "  << tmp4 << endl;
    cout << "ECC Enabled?: " << tmp5 << endl;
    cout << "Device PCI Bus ID: " << devicePciBusId << endl;
    cout << "Device PCI Device ID: " << devicePciDeviceId << endl;
    cout << "Device PCI Domain ID: " << devicePciDomainId << endl;
    cout << "Device TCC Driver: " << deviceTccDriver << endl;
    cout << "Device Async Engine Count: " << deviceAsyncEngineCount << endl;
    cout << "Device Unified Addressing?: " << tmp6 << endl;
    cout << "Device Memory Clock Rate: " << deviceMemClockRate << endl;
    cout << "Device Memory Bus Width: " << deviceMemBusWidth << endl;
    cout << "Device L2 Cache Size: " << deviceL2CacheSize << endl;
    cout << "Device Maximum Threads Per Multi Processor: " << deviceMaxThreadsPerMultiProcessor << endl;
    cout << "Is Compatible?: " << tmp7 << endl;

    return 0;

}
