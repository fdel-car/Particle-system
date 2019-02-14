__kernel void updateParticles(__global float4 *particles, float4 gravityCenter) {
    __private int idx = get_global_id(0);
    __global float4 *pos = &(particles[idx]);

    // float3 center = (float3)(0.2f, 0.0f, 0.0f);
    // float3 tmpPos = (float3)(pos->x, pos->y, pos->z);

    float4 dir = gravityCenter - *pos;
    pos->x += dir.x * 0.001f;
    pos->y += dir.y * 0.001f;
    pos->z += dir.z * 0.001f;
    // *pos += dir // This does not work, why?
}
