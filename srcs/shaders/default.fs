#version 410 core
in vec3 wPos;

uniform vec3 gravityCenter;
uniform vec3 satColor;

out vec4 fragColor;

void main() {
	vec3 vec = gravityCenter - wPos;
	float dist = length(vec);

	float maxSatDist = 2.6f;
	float satCoeff = 1.0f - (min(dist, maxSatDist) / maxSatDist);
	float L = 0.299f * satColor.x + 0.587f * satColor.y + 0.114f * satColor.z;
	fragColor.x = satColor.x + satCoeff * (L - satColor.x);
	fragColor.y = satColor.y + satCoeff * (L - satColor.y);
	fragColor.z = satColor.z + satCoeff * (L - satColor.z);
}
