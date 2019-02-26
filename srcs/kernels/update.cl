__kernel void updateParticles(__global Particle *particles, float3 const gravityCenter, float const deltaTime, uchar const gravityEnabled) {
	__private int const idx = get_global_id(0);
	__global float4 *vel = &(particles[idx].velocity);

	float3 vec = gravityCenter - particles[idx].position.xyz;
	float dist = length(vec);
	if (gravityEnabled != 0) {
		float Gm = 6.67E-2f; // 6.67E-11f * 1.0E9f
		// The mass of a particle is one, so this is equal to the acceleration
		float Fg = Gm / pown(dist, 2);
		vel->xyz += vec * Fg * deltaTime;
	}
	particles[idx].position += *vel;
}
