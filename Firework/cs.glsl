uniform float dt;

layout(local_size_x = WORK_GROUP_SIZE, local_size_y = 1, local_size_z = 1) in;

void main()
{
	uint i = gl_GlobalInvocationID.x;
	if (i >= numParticles) return;

	update(i, dt);
}