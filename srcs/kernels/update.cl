typedef struct {
  uchar gravityEnabled;
  float4 color;
} Settings;

__kernel void updateParticles(__global Particle *particles, float4 const gravityCenter, float const deltaTime, Settings const settings) {
	__private int const idx = get_global_id(0);
	__global float4 *vel = &(particles[idx].velocity);

	float3 vec = gravityCenter.xyz - particles[idx].position.xyz;
	float dist = length(vec);
	if (settings.gravityEnabled != 0) {
		float G = 6.67E-11;
		// The mass of a particle is one, so this is equal to the acceleration
		float Fg = (G * 1.0E9f) / (dist * dist);
		vel->xyz += vec * Fg * deltaTime;
	}
	particles[idx].position += *vel;

	float maxSatDist = 2.6f;
	float satCoeff = 1.0f - (min(dist, maxSatDist) / maxSatDist);
	float L = 0.299f * settings.color.x + 0.587f * settings.color.y + 0.114f * settings.color.z;
	particles[idx].color.x = settings.color.x + satCoeff * (L - settings.color.x);
	particles[idx].color.y = settings.color.y + satCoeff * (L - settings.color.y);
	particles[idx].color.z = settings.color.z + satCoeff * (L - settings.color.z);
}
