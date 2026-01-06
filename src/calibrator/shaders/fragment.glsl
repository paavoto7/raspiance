#version 330 core
out vec4 FragColor;

in vec2 vUv;

uniform sampler2D yuyvTex;

// Reference: Jack, Keith. Video Demystified : A Handbook for the Digital Engineer, Elsevier Science & Technology, 2007
// Coefficients taken from Keith's book.
vec3 ConvertToRGB(vec4 yuv) {

        // In column-major order
        mat3 coefs = mat3(
            1.0, 1.0, 1.0,
            0.0, -0.34414, 1.772,
            1.402, -0.71414, 0.0
        );
        
        // Subtracting 0.5 instead of 128 as they are normalized
        return coefs * vec3(yuv.r, yuv.g - 0.5, yuv.a - 0.5);
    }

void main() {
    // !! Currently ignores the second pixel of each YUYV pixel
    
    vec4 yuv = texture(yuyvTex, vec2(vUv.r, 1 - vUv.g));
    FragColor = vec4(ConvertToRGB(yuv), 1.0);
}
