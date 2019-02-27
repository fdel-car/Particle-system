// https://web.archive.org/web/20120219141616/http://www.cgafaq.info/wiki/Evenly_distributed_points_on_sphere
__kernel void initSphere(__global float4 *pos, __global float4 *vel) {
    uint const idx = get_global_id(0);

	float dlong = 2.39996322973f; // M_PI_F * (3.0f - sqrtf(5.0f))
	float dz = 2.0f / get_global_size(0);

	float z = (1.0f - dz / 2.0f) - (dz * idx);
	float l = dlong * idx;
	float r = sqrt(1.0f - z * z) * 0.5f; // 0.5f is the expected sphere radius

	pos[idx].x = cos(l) * r;
	pos[idx].y = sin(l) * r;
	pos[idx].z = z;

	pos[idx].w = 1.0f;
	vel[idx] = (float4)(0);
}

__kernel void initFilledCube(__global float4 *pos, __global float4 *vel) {
	uint const idx = get_global_id(0);

	float cubeRoot = cbrt((float)get_global_size(0));

	uint x = fmod((float)idx, cubeRoot);
	uint y = fmod((float)idx / cubeRoot, cubeRoot);
	uint z = idx / (cubeRoot * cubeRoot);

	float invCubeRoot = 1.0f / cubeRoot;
	float centerCorrection = -0.5f + invCubeRoot / 1.0f;

	pos[idx].x = x * invCubeRoot + centerCorrection;
	pos[idx].y = y * invCubeRoot + centerCorrection;
	pos[idx].z = z * invCubeRoot + centerCorrection;
	pos[idx].w = 1.0f;
	vel[idx] = (float4)(0);
}

__kernel void initLatitudes(__global float4 *pos, __global float4 *vel) {
	uint const idx = get_global_id(0);

	float cubeRoot = cbrt((float)get_global_size(0));
	uint largeIdx = idx / cubeRoot;
	uint smallIdx = fmod((float)idx, cubeRoot);
	float lTheta = (largeIdx / (cubeRoot * cubeRoot)) * (M_PI_F * 2);
	float sTheta = (smallIdx / cubeRoot) * (M_PI_F * 2);

	float tmpX = 0.5f * cos(lTheta);
	pos[idx].y = 0.5f * sin(lTheta);

	pos[idx].x = cos(sTheta) * tmpX;
	pos[idx].z = -sin(sTheta) * tmpX;
	pos[idx].w = 1.0f;
	vel[idx] = (float4)(0);
}

__kernel void initLongitudes(__global float4 *pos, __global float4 *vel) {
	uint const idx = get_global_id(0);

	float cubeRoot = cbrt((float)get_global_size(0));
	uint largeIdx = idx / cubeRoot;
	uint smallIdx = fmod((float)idx, cubeRoot);
	float lTheta = (largeIdx / (cubeRoot * cubeRoot)) * (M_PI_F * 2);
	float sTheta = (smallIdx / cubeRoot) * (M_PI_F * 2);

	float tmpX = 0.5f * cos(sTheta);
	pos[idx].y = 0.5f * sin(sTheta);

	pos[idx].x = cos(lTheta) * tmpX;
	pos[idx].z = -sin(lTheta) * tmpX;
	pos[idx].w = 1.0f;
	vel[idx] = (float4)(0);
}
