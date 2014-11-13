#version 150	

in vec2 vertexTexCoordsOut;
out vec4 FragColor;

uniform	sampler2D texture0; 

void	main()
{
	FragColor = texture(texture0, vertexTexCoordsOut);
	
	FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}