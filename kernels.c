/* kernels.c

  (c) A. Rigaud, 2024

  openCl kernel wrapper
  to use :

  struct opencl opencl;
  struct params params;
  init_opencl(&opencl);
  void* kernel = init_kernel(&opencl, &params); // inits
  render_kernel(&opencl, kernel, &params); // loop i,j here
  free_opencl(&opencl, kernel); // free mem

*/

#include <stdlib.h>
#include <stdio.h>

#include <OpenCL/opencl.h>

const char *kernelSource = \
"__kernel void mandelbrot_kernel(__global int *output, float center_re, float center_im, float scale, int max_iterations, int width, int height) {\n" \
"    int x = get_global_id(0);\n"\
"    int y = get_global_id(1);\n"\
"    float c_re = center_re + (x - width / 2) * scale;\n"\
"    float c_im = center_im + (y - height / 2) * scale;\n"\
"    float z_re = 0;\n"\
"    float z_im = 0;\n"\
"    float z2_re = 0;\n"\
"    float z2_im = 0;\n"\
"    int iterations = 0;\n"\
"    while ((z2_re + z2_im < 4) && (iterations++ < max_iterations)) {\n"\
"        z_im = 2 * z_re * z_im + c_im;\n"\
"        z_re = z2_re - z2_im + c_re;\n"\
"        z2_re = z_re * z_re;\n"\
"        z2_im = z_im * z_im;\n"\
"    }\n"\
"    float brightness = 24 ;\n"\
"    float color = 255 * iterations / max_iterations;\n"\
"    float srgb_color = color <= 0.0031308 ? color * 12.92 : 1.055 * pow(color, 1.0 / 2.4) - 0.055;\n"\
"    output[x + y * width] = brightness * srgb_color;\n"\
"}\n";

struct opencl {
    cl_kernel kernel;
    cl_context context;
    cl_command_queue queue;
    cl_program program;
    cl_platform_id platform;
    cl_device_id device;
    cl_mem output;
};

struct params {
    cl_int max_iterations;
    cl_float center_re;
    cl_float center_im;
    cl_float scale;
    cl_int width;
    cl_int height;
};

// print opencl informations
void debug_opencl(struct opencl* opencl)
{
    char buffer[4096];
    clGetProgramBuildInfo(opencl->program, opencl->device, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, NULL);
    printf("%s\n", buffer);
    exit(EXIT_FAILURE);
}

// compile kernel
void init_opencl(struct opencl* opencl)
{
    cl_int err;
    err = clGetPlatformIDs(1, &opencl->platform, NULL);
    int gpu = 1;
    err = clGetDeviceIDs(opencl->platform, gpu ? CL_DEVICE_TYPE_GPU : CL_DEVICE_TYPE_CPU, 1, &opencl->device, NULL);
    opencl->context = clCreateContext(NULL, 1, &opencl->device, NULL, NULL, &err);
    opencl->queue = clCreateCommandQueue(opencl->context, opencl->device, 0, &err);
    opencl->program = clCreateProgramWithSource(opencl->context, 1, (const char **) &kernelSource, NULL, &err);
    err = clBuildProgram(opencl->program, 1, &opencl->device, NULL, NULL, NULL);
    //load_kernel_bin(opencl);
    if (err) debug_opencl(opencl);
    opencl->kernel = clCreateKernel(opencl->program, "mandelbrot_kernel", &err);
    if (err) printf("-> Error: Failed to create compute kernel! ERR = %d \n", err);
}

// updage kernel input
void update_kernel_inputs_args(struct opencl* opencl, struct params* params) {
    cl_int err = 0;
    err |= clSetKernelArg(opencl->kernel, 1, sizeof(cl_float), &params->center_re);
    err |= clSetKernelArg(opencl->kernel, 2, sizeof(cl_float), &params->center_im);
    err |= clSetKernelArg(opencl->kernel, 3, sizeof(cl_float), &params->scale);
    err |= clSetKernelArg(opencl->kernel, 4, sizeof(cl_int), &params->max_iterations);
    err |= clSetKernelArg(opencl->kernel, 5, sizeof(cl_int), &params->width);
    err |= clSetKernelArg(opencl->kernel, 6, sizeof(cl_int), &params->height);
    if (err) debug_opencl(opencl);
}

// kernel struct init
void* init_kernel(struct opencl* opencl, struct params* params)
{
    cl_int err = 0;
    void *kernel = malloc(params->width * params->height * sizeof(int));
    if (kernel == NULL) printf("mem allocation failed\n");
    opencl->output = clCreateBuffer(opencl->context, CL_MEM_WRITE_ONLY, params->width * params->height * sizeof(int), NULL,  &err);
    err |= clSetKernelArg(opencl->kernel, 0, sizeof(cl_mem), &opencl->output);
    update_kernel_inputs_args(opencl, params);
    if (err) debug_opencl(opencl);
    return kernel;
}

// call GPU kernel
void render_kernel(struct opencl* opencl, int *kernel, struct params* params)
{
    cl_int err;
    update_kernel_inputs_args(opencl, params);
    size_t globalWorkSize[2] = {params->width, params->height};
    err = clEnqueueNDRangeKernel(opencl->queue, opencl->kernel, 2, NULL, globalWorkSize, NULL, 0, NULL, NULL);
    err = clFinish(opencl->queue);
    err = clEnqueueReadBuffer(opencl->queue, opencl->output, CL_TRUE, 0, params->width * params->height * sizeof(int), kernel, 0, NULL, NULL);
    //if (err) debug_opencl(opencl);
}

// free allocated mem
void free_opencl(struct opencl* opencl, int *kernel)
{
    clReleaseMemObject(opencl->output);
    clReleaseKernel(opencl->kernel);
    free(kernel);
}

// example of kernel usage
void test_kernels(void)
{
  struct opencl opencl;
  struct params params  = {
        .max_iterations = 10,
        .center_re = -0.5,
        .center_im = 0,
        .scale = 0.005,
        .width = 10,
        .height = 10
    };
  init_opencl(&opencl);                         // init
  void* kernel = init_kernel(&opencl, &params); // more init
  render_kernel(&opencl, kernel, &params);      // loop i,j here
  free_opencl(&opencl, kernel);                 // free mem
}
