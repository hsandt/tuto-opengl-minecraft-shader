varying vec3 normal;
varying vec3 vertex_to_light_vector;
varying vec4 color;

uniform float elapsed;
uniform mat4 invertView;

uniform float wave_amplitude;

void main()
{
	// Transforming The Vertex
	vec4 vertex = gl_Vertex;
	vertex.z += wave_amplitude * sin(1.f * vertex.x);
	gl_Position = gl_ModelViewProjectionMatrix * vertex;

	// Transforming The Normal To ModelView-Space
	normal = gl_NormalMatrix * gl_Normal;

	//Direction lumiere
	vertex_to_light_vector = vec3(gl_LightSource[0].position);

	//Couleur
	color = gl_Color;
}