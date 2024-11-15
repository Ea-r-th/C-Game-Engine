#version 420 core

out vec4 color;

in VertexData{
    vec2 pass_texCoords;
    flat uint pass_textureID;
    flat uint pass_Color;
    bool pass_skip;
}vertexData;

uniform sampler2DArray font;

vec3 decodeColor(uint encodedColor) {
    // Extract the bits for each component
    float r = float((encodedColor & uint(0x01)) * 0.25 + ((encodedColor >> 1) & uint(0x01)) * 0.75);
    float g = float(((encodedColor >> 2) & uint(0x01)) * 0.25 + ((encodedColor >> 3) & uint(0x01)) * 0.75);
    float b = float(((encodedColor >> 4) & uint(0x01)) * 0.25 + ((encodedColor >> 5) & uint(0x01)) * 0.75);

    // Return the decoded color
    return vec3(r, g, b);
}

void main() {

    if(vertexData.pass_skip){
    	discard;
	}
    vec3 decodedColor = decodeColor(vertexData.pass_Color);
    vec4 sampled = vec4(decodedColor, texture(font, vec3(vertexData.pass_texCoords.xy, vertexData.pass_textureID)).r);
    
    
    if(sampled.a < 0.5) {
        discard; // Discard the fragment if the alpha is less than 0.1
    } else {
        color = sampled;
    }
}