#pragma once

#define PerShader PerspectiveShader
#define perShader perspectiveShader

class PerspectiveShader {
public:
    bool made = 0;
    Shader shader;
    
    PerspectiveShader() {};

    bool initializeWithStrings() {
        
        const std::string vertexShaderCode = R"(
varying vec2 v_texCoord;
varying float v_invW;

void main() {
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

    // Pass through the perspective-corrected UV coordinates
    v_texCoord = gl_MultiTexCoord0.xy;

    // Extract 1/w from the red color channel
    v_invW= (uint(gl_Color.r * 255.0)         |
            (uint(gl_Color.g * 255.0) << 8)   |
            (uint(gl_Color.b * 255.0) << 16)  |
            (uint(gl_Color.a * 255.0) << 24)) / 1000000.0; 
}
        )";
        
        const std::string fragmentShaderCode = R"(
uniform sampler2D tex;

varying vec2 v_texCoord;
varying float v_invW;

void main() {
    // Perform perspective correction: divide (uv/w) by (1/w) to get correct uv
    vec2 correctedUV = v_texCoord / v_invW;

    gl_FragColor = texture(tex, correctedUV);
}
        )";
        
        if (!shader.loadFromMemory(vertexShaderCode, fragmentShaderCode)) {
            return false;
        }
        
        return true;
    };

    Shader* get(void) {
        if (made)
            return &shader;
        else {
            made = true;
            if (!initializeWithStrings()) exit(-1);
            return &shader;
        }
    };
};

PerspectiveShader perspectiveShader;
