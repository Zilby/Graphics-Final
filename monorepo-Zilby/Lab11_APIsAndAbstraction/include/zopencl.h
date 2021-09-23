#include <stdio.h>
#include <stdlib.h>
#include <iostream>
 
#ifdef __APPLE__
#include <OpenCL/opencl.h>
#else
#include <CL/cl.h>
#endif

class zopencl{

public:
	void runProgram();
};