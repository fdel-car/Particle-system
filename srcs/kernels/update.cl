__kernel void updateParticles(__global Particle *particles, float4 gravityCenter, float deltaTime) {
	__private int idx = get_global_id(0);
	__global float4 *vel = &(particles[idx].velocity);

	float3 vec = gravityCenter.xyz - particles[idx].position.xyz;
	float G = 6.67E-11;
	float dist = length(vec);
	// The mass of a particle is one, so this is equal to the acceleration
	float Fg = (G * 1.0E9f) / (dist * dist);

	vel->xyz += vec * Fg * deltaTime;
	particles[idx].position += *vel;

	// Yellow
	float4 color = (float4)(1.0f, 0.8, 0.0f, 1.0f);

	float maxSatDist = 2.6f;
	float satCoeff = 1.0f - (min(dist, maxSatDist) / maxSatDist);
	float L = 0.299f * color.x + 0.587f * color.y + 0.114f * color.z;
	particles[idx].color.x = color.x + satCoeff * (L - color.x);
	particles[idx].color.y = color.y + satCoeff * (L - color.y);
	particles[idx].color.z = color.z + satCoeff * (L - color.z);
}
