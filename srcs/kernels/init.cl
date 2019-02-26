typedef struct {
	float4 position;
	float4 velocity;
}	Particle;

void initValues(__global Particle *particles, int const idx) {
	// Init with full opacity
	particles[idx].position.w = 1.0f;

	// Init with no initial velocity, this could be changed to produce nice effects
	particles[idx].velocity = (float4)(0);
}

__kernel void initLatitudes(__global Particle *particles, ulong const numParticles) {
	__private int const idx = get_global_id(0);
	__global float4 *pos = &(particles[idx].position);

	float cubeRoot = cbrt((float)numParticles);
	int largeIdx = idx / cubeRoot;
	int smallIdx = fmod((float)idx, cubeRoot);
	float lTheta = (largeIdx / (cubeRoot * cubeRoot)) * (M_PI * 2);
	float sTheta = (smallIdx / cubeRoot) * (M_PI * 2);
	pos->x = 0.5f * cos(lTheta);
	pos->y = 0.5f * sin(lTheta);
	pos->z = 0.0f;

	float tmpX = pos->x;
	pos->x = cos(sTheta) * pos->x;
	pos->z = -sin(sTheta) * tmpX;

	initValues(particles, idx);
}

__kernel void initLongitudes(__global Particle *particles, ulong const numParticles) {
	__private int const idx = get_global_id(0);
	__global float4 *pos = &(particles[idx].position);

	float cubeRoot = cbrt((float)numParticles);
	int largeIdx = idx / cubeRoot;
	int smallIdx = fmod((float)idx, cubeRoot);
	float lTheta = (largeIdx / (cubeRoot * cubeRoot)) * (M_PI * 2);
	float sTheta = (smallIdx / cubeRoot) * (M_PI * 2);
	pos->x = 0.5f * cos(sTheta);
	pos->y = 0.5f * sin(sTheta);
	pos->z = 0.0f;

	float tmpX = pos->x;
	pos->x = cos(lTheta) * pos->x;
	pos->z = -sin(lTheta) * tmpX;

	initValues(particles, idx);
}

__kernel void initFilledCube(__global Particle *particles, ulong const numParticles) {
	__private int const idx = get_global_id(0);
	__global float4 *pos = &(particles[idx].position);

	float cubeRoot = cbrt((float)numParticles);

	int	x = fmod((float)idx, cubeRoot);
	int	y = fmod((float)idx / cubeRoot, cubeRoot);
	int	z = idx / (cubeRoot * cubeRoot);

	float	invCubeRoot = 1.0f / cubeRoot;
	float	centerCorrection = -0.5f + invCubeRoot / 2.0f;

	pos->x = x * invCubeRoot + centerCorrection;
	pos->y = y * invCubeRoot + centerCorrection;
	pos->z = z * invCubeRoot + centerCorrection;

	initValues(particles, idx);
}

