/* kernels.c
  (c) A. Rigaud, 2024

  OpenCl kernel wrapper
*/

#include <stdlib.h>
#include <stdio.h>

#include <OpenCL/opencl.h>

struct opencl {
    cl_kernel        kernel;
    cl_context       context;
    cl_command_queue queue;
    cl_program       program;
    cl_platform_id   platform;
    cl_device_id     device;
    cl_mem           output;
};

struct params {
    cl_double* r;
    cl_double* q;
    cl_double* current_column;
    cl_int     size;
};

/* OpenCL 1.2 error handler */
void opencl_err_hander(cl_int err)
{
    char* str;
    #define return_err(x) case x: str = #x; break;
    switch (err)
    {
        return_err(CL_DEVICE_NOT_FOUND                         )
        return_err(CL_DEVICE_NOT_AVAILABLE                     )
        return_err(CL_COMPILER_NOT_AVAILABLE                   )
        return_err(CL_MEM_OBJECT_ALLOCATION_FAILURE            )
        return_err(CL_OUT_OF_RESOURCES                         )
        return_err(CL_OUT_OF_HOST_MEMORY                       )
        return_err(CL_PROFILING_INFO_NOT_AVAILABLE             )
        return_err(CL_MEM_COPY_OVERLAP                         )
        return_err(CL_IMAGE_FORMAT_MISMATCH                    )
        return_err(CL_IMAGE_FORMAT_NOT_SUPPORTED               )
        return_err(CL_BUILD_PROGRAM_FAILURE                    )
        return_err(CL_MAP_FAILURE                              )
        return_err(CL_MISALIGNED_SUB_BUFFER_OFFSET             )
        return_err(CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST)
        return_err(CL_COMPILE_PROGRAM_FAILURE                  )
        return_err(CL_LINKER_NOT_AVAILABLE                     )
        return_err(CL_LINK_PROGRAM_FAILURE                     )
        return_err(CL_DEVICE_PARTITION_FAILED                  )
        return_err(CL_KERNEL_ARG_INFO_NOT_AVAILABLE            )
        return_err(CL_INVALID_VALUE                            )
        return_err(CL_INVALID_DEVICE_TYPE                      )
        return_err(CL_INVALID_PLATFORM                         )
        return_err(CL_INVALID_DEVICE                           )
        return_err(CL_INVALID_CONTEXT                          )
        return_err(CL_INVALID_QUEUE_PROPERTIES                 )
        return_err(CL_INVALID_COMMAND_QUEUE                    )
        return_err(CL_INVALID_HOST_PTR                         )
        return_err(CL_INVALID_MEM_OBJECT                       )
        return_err(CL_INVALID_IMAGE_FORMAT_DESCRIPTOR          )
        return_err(CL_INVALID_IMAGE_SIZE                       )
        return_err(CL_INVALID_SAMPLER                          )
        return_err(CL_INVALID_BINARY                           )
        return_err(CL_INVALID_BUILD_OPTIONS                    )
        return_err(CL_INVALID_PROGRAM                          )
        return_err(CL_INVALID_PROGRAM_EXECUTABLE               )
        return_err(CL_INVALID_KERNEL_NAME                      )
        return_err(CL_INVALID_KERNEL_DEFINITION                )
        return_err(CL_INVALID_KERNEL                           )
        return_err(CL_INVALID_ARG_INDEX                        )
        return_err(CL_INVALID_ARG_VALUE                        )
        return_err(CL_INVALID_ARG_SIZE                         )
        return_err(CL_INVALID_KERNEL_ARGS                      )
        return_err(CL_INVALID_WORK_DIMENSION                   )
        return_err(CL_INVALID_WORK_GROUP_SIZE                  )
        return_err(CL_INVALID_WORK_ITEM_SIZE                   )
        return_err(CL_INVALID_GLOBAL_OFFSET                    )
        return_err(CL_INVALID_EVENT_WAIT_LIST                  )
        return_err(CL_INVALID_EVENT                            )
        return_err(CL_INVALID_OPERATION                        )
        return_err(CL_INVALID_GL_OBJECT                        )
        return_err(CL_INVALID_BUFFER_SIZE                      )
        return_err(CL_INVALID_MIP_LEVEL                        )
        return_err(CL_INVALID_GLOBAL_WORK_SIZE                 )
        return_err(CL_INVALID_PROPERTY                         )
        return_err(CL_INVALID_IMAGE_DESCRIPTOR                 )
        return_err(CL_INVALID_COMPILER_OPTIONS                 )
        return_err(CL_INVALID_LINKER_OPTIONS                   )
        return_err(CL_INVALID_DEVICE_PARTITION_COUNT           )
        default: str = "Unknown OpenCL error code";
    }
    printf("%s\n", str);
    exit(EXIT_FAILURE);
}

/* print opencl kernel.cl compilation messages */
void debug_opencl(struct opencl* opencl)
{
    #define BUF_MAX_SIZE 4096
    char buffer[BUF_MAX_SIZE];
    clGetProgramBuildInfo(opencl->program, opencl->device, CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, NULL);
    buffer[BUF_MAX_SIZE-1] = '\0';
    printf("%s\n", buffer);
    exit(EXIT_FAILURE);
}

/* read a .cl file and place content into buffer */
char* kernel_read(void)
{
    FILE* kernels_file = fopen("../kernels.cl", "r");
    if(kernels_file == NULL) {
        printf("Couldn't find the program file");
        exit(EXIT_FAILURE);
    }
    fseek(kernels_file, 0, SEEK_END);
    size_t kernels_size = ftell(kernels_file);
    rewind(kernels_file);
    char* kernels_buffer = (char*)malloc(kernels_size + 1);
    kernels_buffer[kernels_size] = '\0';
    fread(kernels_buffer, sizeof(char), kernels_size, kernels_file);
    fclose(kernels_file);
    return kernels_buffer;
}

/* compile kernels */
void init_opencl(struct opencl* opencl)
{
    cl_int err = 0;
    err = clGetPlatformIDs(1, &opencl->platform, NULL);
    if (err) opencl_err_hander(err);
    err = clGetDeviceIDs(opencl->platform, CL_DEVICE_TYPE_GPU, 1, &opencl->device, NULL);
    if (err) opencl_err_hander(err);
    opencl->context = clCreateContext(NULL, 1, &opencl->device, NULL, NULL, &err);
    if (err) opencl_err_hander(err);
    opencl->queue = clCreateCommandQueue(opencl->context, opencl->device, 0, &err);
    if (err) opencl_err_hander(err);
    char *kernelSource = kernel_read();
    opencl->program = clCreateProgramWithSource(opencl->context, 1, (const char **) &kernelSource, NULL, &err);
    if (err) opencl_err_hander(err);
    free(kernelSource);
    //"-I/opt/homebrew/Cellar/llvm/19.1.3/lib/clang/19/include -I.."
    err = clBuildProgram(opencl->program, 1, &opencl->device, NULL, NULL, NULL);
    if (err) debug_opencl(opencl);
    opencl->kernel = clCreateKernel(opencl->program, "qr_kernel", &err); // TODO compile all the kernels
    if (err) opencl_err_hander(err);
}

// updage kernel input
void kernel_inputs_update(struct opencl* opencl, struct params* params) {
    clSetKernelArg(opencl->kernel, 1, sizeof(cl_float*), &params->q);
    clSetKernelArg(opencl->kernel, 2, sizeof(cl_float*), &params->r);
    clSetKernelArg(opencl->kernel, 3, sizeof(cl_float*), &params->current_column);
    clSetKernelArg(opencl->kernel, 4, sizeof(cl_int),    &params->size);
}

/* kernel memory init. */
void* kernel_init(struct opencl* opencl, struct params* params)
{
    cl_int err = 0;
    void *kernel = malloc(params->size * sizeof(double));
    if (kernel == NULL) {
        printf("mem allocation failed\n");
        exit(EXIT_FAILURE);
    }
    opencl->output = clCreateBuffer(opencl->context, CL_MEM_WRITE_ONLY, params->size * sizeof(double), NULL,  &err);
    if (err) opencl_err_hander(err);
    err = clSetKernelArg(opencl->kernel, 0, sizeof(cl_mem), &opencl->output);
    if (err) opencl_err_hander(err);
    kernel_inputs_update(opencl, params); // TODO : check if really needed
    if (err) opencl_err_hander(err);
    return kernel;
}

/* (simple) call to GPU kernel (update input, queue, run, read back) */
void kernel_run(struct opencl* opencl, int *kernel, struct params* params)
{
    size_t globalWorkSize = params->size;
    kernel_inputs_update(opencl, params);
    clEnqueueNDRangeKernel(opencl->queue, opencl->kernel, 1, NULL, &globalWorkSize, NULL, 0, NULL, NULL);
    clFinish(opencl->queue);
    clEnqueueReadBuffer(opencl->queue, opencl->output, CL_TRUE, 0, params->size * sizeof(double), kernel, 0, NULL, NULL);
}

/* free allocated mem */
void kernel_free(struct opencl* opencl, int *kernel)
{
    clReleaseMemObject(opencl->output);
    clReleaseKernel(opencl->kernel);
    free(kernel);
}

/* example */
void kernel_test(void)
{
  struct opencl opencl;
  struct params params;
  init_opencl(&opencl);                         // init
  void* kernel = kernel_init(&opencl, &params); // memory init


  params.r;
  params.q;
  params.current_column;
  params.size;

  kernel_run(&opencl, kernel, &params);         // loop i,j here
  kernel_free(&opencl, kernel);                 // free mem
}
