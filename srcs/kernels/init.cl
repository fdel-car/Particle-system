__kernel void initLatitudes(__global float4 *particles, ulong numParticles) {
    __private int idx = get_global_id(0);
    __global float4 *pos = &(particles[idx]);

    float cubeRoot = cbrt((float)numParticles);
    int largeIdx = idx / cubeRoot;
    int smallIdx = fmod((float)idx, cubeRoot);
    float lTheta =  (largeIdx / (cubeRoot * cubeRoot)) * (M_PI * 2);
    float sTheta =  (smallIdx / cubeRoot) * (M_PI * 2);
    pos->x = 0.5f * cos(lTheta);
    pos->y = 0.5f * sin(lTheta);
    pos->z = 0.0f;

    float tmpX = pos->x;
    pos->x = cos(sTheta) * pos->x;
    pos->z = -sin(sTheta) * tmpX;

    // Init with full opacity
    pos->w = 1.0f;
}

__kernel void initLongitudes(__global float4 *particles, ulong numParticles) {
    __private int idx = get_global_id(0);
    __global float4 *pos = &(particles[idx]);

    float cubeRoot = cbrt((float)numParticles);
    int largeIdx = idx / cubeRoot;
    int smallIdx = fmod((float)idx, cubeRoot);
    float lTheta =  (largeIdx / (cubeRoot * cubeRoot)) * (M_PI * 2);
    float sTheta =  (smallIdx / cubeRoot) * (M_PI * 2);
    pos->x = 0.5f * cos(sTheta);
    pos->y = 0.5f * sin(sTheta);
    pos->z = 0.0f;

    float tmpX = pos->x;
    pos->x = cos(lTheta) * pos->x;
    pos->z = -sin(lTheta) * tmpX;

    // Init with full opacity
    pos->w = 1.0f;
}

__kernel void initFilledCube(__global float4 *particles, ulong numParticles) {
    __private int idx = get_global_id(0);
    __global float4 *pos = &(particles[idx]);

	float cubeRoot = cbrt((float)numParticles);

	int	x = fmod((float)idx, cubeRoot);
	int	y = fmod((float)idx / cubeRoot, cubeRoot);
	int	z = idx / (cubeRoot * cubeRoot);

	float	invCubeRoot = 1.0f / cubeRoot;
	float	centerCorrection = -0.5f + invCubeRoot / 2.0f;

	pos->x = x * invCubeRoot + centerCorrection;
	pos->y = y * invCubeRoot + centerCorrection;
	pos->z = z * invCubeRoot + centerCorrection;

    // Init with full opacity
    pos->w = 1.0f;
}

