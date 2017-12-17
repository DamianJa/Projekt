#version 330
// A texture is expected as program attribute
uniform sampler2D Texture;

// Direction of light
uniform vec3 LightDirection;
uniform vec3 LightDirection2;

uniform float intenzita=1.0f;
uniform float intenzita2=1.0f;
// (optional) Transparency
uniform float Transparency;

// (optional) Texture offset
uniform vec2 TextureOffset;

uniform float dt;

// The vertex shader will feed this input
in vec2 texCoord;

// Wordspace normal passed from vertex shader
in vec4 normal;

// The final color
out vec4 FragmentColor;



void main() {
  // Compute diffuse lighting
  float diffuse = max(dot(normal, vec4(normalize(LightDirection), 1.0f + dt)), 0.0f) ;
   float diffuse2 = max(dot(normal, vec4(normalize(LightDirection2), 1.0f + dt)), 0.0f) ;

 // if(diffuse <0.5f) diffuse = 0.5f;
  // Lookup the color in Texture on coordinates given by texCoord
  // NOTE: Texture coordinate is inverted vertically for compatibility with OBJ
  FragmentColor = texture(Texture, vec2(texCoord.x, 1.0 - texCoord.y) + TextureOffset) * diffuse * intenzita +  texture(Texture, vec2(texCoord.x, 1.0 - texCoord.y) + TextureOffset) * diffuse2 * intenzita2 ;


}
