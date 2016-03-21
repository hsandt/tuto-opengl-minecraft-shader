varying vec3 normal;
varying vec3 vertex_to_light_vector;
varying vec4 color;

uniform float elapsed;
uniform mat4 invertView;

uniform float wave_amplitude;
uniform float normalized_wavelength;

varying float wave_factor;  // 0 for no wave

void main()
{
	// Transforming The Vertex
	mat4 modelMatrix = invertView * gl_ModelViewMatrix;
	mat4 viewProjectionMatrix = gl_ModelViewProjectionMatrix * inverse(modelMatrix);
	vec4 worldVertex = modelMatrix * gl_Vertex;
	worldVertex.z += wave_factor * wave_amplitude * sin(2 * 3.14f * (worldVertex.x / 10.f) / normalized_wavelength);
	gl_Position = viewProjectionMatrix * worldVertex;

	// Transforming The Normal To ModelView-Space
	normal = gl_NormalMatrix * gl_Normal;

	//Direction lumiere
	vertex_to_light_vector = vec3(gl_LightSource[0].position);

	//Couleur
	color = gl_Color;
}
