varying vec3 normal;
varying vec3 vertex_to_light_vector;
varying vec4 color;

uniform float elapsed;
uniform mat4 invertView;

attribute float wave_amplitude;

void main()
{
	// Transforming The Vertex
	mat4 modelMatrix = invertView * gl_ModelViewMatrix;
	mat4 viewProjectionMatrix = gl_ModelViewProjectionMatrix * inverse(modelMatrix);
	vec4 worldVertex = modelMatrix * gl_Vertex;
	worldVertex.z += wave_amplitude * sin(1.f * worldVertex.x);
	gl_Position = viewProjectionMatrix * worldVertex;

	// Transforming The Normal To ModelView-Space
	normal = gl_NormalMatrix * gl_Normal;

	//Direction lumiere
	vertex_to_light_vector = vec3(gl_LightSource[0].position);

	//Couleur
	color = gl_Color;
}