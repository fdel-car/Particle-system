__kernel void initSphere(__global float3 *particles) {
    global float3 *pos = &(particles[get_global_id(0)]);
    pos->x = 0.75f;
    pos->y = 0.75f;
    pos->z = 0.0f;
}
