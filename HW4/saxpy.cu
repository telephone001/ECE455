#include <stdio.h>
#include <cuda_runtime.h>

// CUDA kernel for SAXPY: y[i] = a * x[i] + y[i]
__global__ void saxpy(int n, float a, float *x, float *y) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < n) {
        y[i] = a * x[i] + y[i];
    }
}

int main() {
    int N = 1000000;
    size_t size = N * sizeof(float);

    // Allocate host memory
    float *x, *y;
    x = (float *)malloc(size);
    y = (float *)malloc(size);

    // Initialize host arrays
    for (int i = 0; i < N; i++) {
        x[i] = 1.0f;
        y[i] = 2.0f;
    }

    // Allocate device memory
    float *d_x, *d_y;
    cudaMalloc(&d_x, size);
    cudaMalloc(&d_y, size);

    // Copy host data to device
    cudaMemcpy(d_x, x, size, cudaMemcpyHostToDevice);
    cudaMemcpy(d_y, y, size, cudaMemcpyHostToDevice);

    // Kernel launch configuration
    int threadsPerBlock = 256;
    int blocksPerGrid = (N + threadsPerBlock - 1) / threadsPerBlock;

    // Launch SAXPY kernel
    saxpy<<<blocksPerGrid, threadsPerBlock>>>(N, 2.0f, d_x, d_y);

    // Copy result back to host
    cudaMemcpy(y, d_y, size, cudaMemcpyDeviceToHost);

    // Print first result
    printf("y[0] = %f\n", y[0]);

    // Free device memory
    cudaFree(d_x);
    cudaFree(d_y);

    // Free host memory
    free(x);
    free(y);

    return 0;
}
