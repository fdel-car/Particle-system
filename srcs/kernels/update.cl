__kernel void updateParticles(__global float4 *pos, __global float4 *vel, float3 const gravityCenter, float const deltaTime, uchar const gravityEnabled) {
	uint const idx = get_global_id(0);

	float3 vec = gravityCenter - pos[idx].xyz;
	float dist = length(vec);
	if (gravityEnabled != 0) {
		// float Gm = 6.67E-2f; // 6.67E-11f * 1.0E9f
		// The mass of a particle is one, so this is equal to the acceleration
		// float Fg = Gm / pown(dist, 2);
		vel[idx].xyz += vec * (6.67E-2f / pown(dist, 2)) * deltaTime;
	}
	pos[idx].xyz += vel[idx].xyz;
}
