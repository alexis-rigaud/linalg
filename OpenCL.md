OpenCL 
===========================================

Introduction
------------------

OpenCL is a standard for multi-platform heterogeneous parallel programming. It is standardized and maintained by [Khronos Group](https://registry.khronos.org/OpenCL/).

It allows to compile and run *kernels* on GPU for acceleration purpose of routines that are organized with the same operation over multiples dimensions.

OpenCL is the abbreviation of Open Compute language. It is a dialect of C99, and had been extended to C++.

It features a Host / Kernel separation paradigm, with a detailed memory hierarchy (__global, __local, __constant, and __private).

GPUs & iGPUs
------------------

GPUs are hardware accelerator designed according parallelism paradigm, especially suited to handle graphics (eg. X/Y grid of pixels).
Modern CPUs have iGPUs (internal GPU) hardware accelerators that can be used by OpenCL.

For instance :
- Apple M1 has an iGPU supporting FP32. 
- AMD Ryzen 7 5700U has an iGPU (Vega 8) supporting FP64.

OpenCL wording
------------------

- OpenCL *headers* : The C/C++ Headers needed to compile. (eg. the API)
- OpenCL *runtime* : A Vendor implementation of the OCL on a specific hardware (core drivers).
- OpenCL *library* : Implementation of the API according to OCL specification, based on runtime.
- OpenCL *ICD*     : An Installable Client Driver (ICD) to manage several OCL implementations.
- OpenCL *SDK*     : a vendor packaged combination of libraries, ICD and runtime for SW dev.

OpenCL drivers installation
------------------

You can check if OpenCL is already installed with command line tool `clinfo`.

With `Number of devices` info, you should see if your hardware have an GPU/iGPU.     

For installation, you can :
- Run the PhysX's OpenCL [benchmark](https://github.com/ProjectPhysX/OpenCL-Benchmark) scripts and follow instruction to install OpenCL drivers for your platform.
- Follow the PhysX's [OpenCL wrapper](https://github.com/ProjectPhysX/OpenCL-Wrapper) installation steps *Install GPU Drivers and OpenCL Runtime (click to expand section)*
- (Ubuntu) follow the Zivid tutorial [here](https://support.zivid.com/en/latest/getting-started/software-installation/gpu/install-opencl-drivers-ubuntu.html)

As a side note :
- Intel CPU  [runtime](https://www.intel.com/content/www/us/en/developer/articles/technical/intel-cpu-runtime-for-opencl-applications-with-sycl-support.html) also works for AMD CPU (but not for Intel GPU or AMD GPU) 
- Portable OpenCL [pocl]( https://github.com/pocl/pocl) project supports many CPUs and GPUs
- Khronos [OpenCL SDK](https://github.com/KhronosGroup/OpenCL-SDK/releases) supports Windows and Linux 

OpenCL Resources
--------------------

Here are some curated resources for OpenCL :


| Vendor | Name & Link | Usage |
| :---          | :---  | :---- |
| Khronos Group | [OpenCL guide](https://github.com/KhronosGroup/OpenCL-Guide) |  Introduction guide (for developers)     |
| Khronos Group | [OpenCL conformant implementation](https://www.khronos.org/conformance/adopters/conformant-products/opencl) | A list of OCL conformant implementation to the standard |
| Khronos Group | [ICD-Loader](https://github.com/KhronosGroup/OpenCL-ICD-Loader) | The reference ICD loader library |
| Khronos Group | [Conformance Tests](https://github.com/KhronosGroup/OpenCL-CTS) | OpenCL-CTS : OpenCL Conformance Test Suite  |
| Khronos Group | [OpenCL SDK](https://github.com/KhronosGroup/OpenCL-SDK) | A reference OpenCL SDK (Windows/Linux) |
| Intel         | [Compute Runtime](https://github.com/intel/compute-runtime) | Intel CPU open source for OpenCL runtime |
| ARM           | [ComputeLibrary](https://github.com/ARM-software/ComputeLibrary) | ARM CPU and GPU open source for OpenCL runtime |
| AMD           | [ROCm ](https://github.com/ROCm/ROCm) | AMD CPU and GPU open source for OpenCL runtime |
| AMD           | [SDK-Lite](https://github.com/GPUOpen-LibrariesAndSDKs/OCL-SDK/releases) | AMD OpenCL lite SDK |
| AMD           | [amdgpu-install](https://amdgpu-install.readthedocs.io/en/latest/index.html) | AMD Radeon linux installation script  |
| AMD           | [Adrenalin](https://www.amd.com/en/support/download/drivers.html)| AMD Adrenalin Windows/linux drivers (including OpenCL)  |
| NVIDIA        | [NVIDIA](https://developer.nvidia.com/opencl) | NVIDIA OpenCL landing page |
| TI            | [support](https://downloads.ti.com/mctools/esd/docs/opencl/index.html) | TI OpenCL support landing page |
| FreeBSD       | [FreeBSD](https://wiki.freebsd.org/Graphics/OpenCL) | FreeBSD OpenCL Support |
| LLVM          | [Clang OpenCL](https://clang.llvm.org/docs/OpenCLSupport.html) | LLVM Clang is the reference compiler for offline opencl kernels |
| PhysX         | [OpenCL wrapper](https://github.com/ProjectPhysX/OpenCL-Wrapper) | A (C++) wrapper for ease of OpenCL usage |
| PhysX         | [benchmark suite](https://github.com/ProjectPhysX/OpenCL-Benchmark) | A benchmark program to test OpenCL running speed |
| POCL          | [POCL](https://github.com/pocl/pocl) | The Portable OpenCL project |
| Einstein Toolkit | [guide](https://einsteintoolkit.org/thornguide/ExternalLibraries/OpenCL/documentation.html)  | An OCL ref guide |
| `clinfo`      | [repo](https://github.com/Oblomov/clinfo) | the awsome clinfo command line program source |
| N/A           | [libOpenCL-loader](https://github.com/robertwgh/libOpenCL-loader) | A dynamic loading of OpenCL example | 
| N/A | [clvk](https://github.com/kpet/clvk) | OpenCL running over Vulkan engine |
| Google | [clspv](https://github.com/google/clspv) | OpenCL to Vulkan/SPIR compilation |
| QEMU | [virtio-gpu](https://www.qemu.org/docs/master/system/devices/virtio-gpu.html) | virtual GPU reference |