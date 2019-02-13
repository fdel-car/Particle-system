__kernel void initSphere(__global float4 *particles, unsigned long numParticles) {
    int idx = get_global_id(0);
    global float4 *pos = &(particles[idx]);

    float theta =  ((float)idx / numParticles) * (M_PI * 2);
    pos->x = 0.5f * cos(theta);
    pos->y = 0.5f * sin(theta);
    pos->z = 0.0f;

    // Init with full opacity
    pos->w = 1.0f;
}
