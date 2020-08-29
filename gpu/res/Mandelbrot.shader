#shader vertex
#version 400 core

layout(location = 0) in vec3 aPos;

out vec4 uv;

uniform mat4 u_MVP;

void main()
{
    gl_Position = vec4(aPos, 1.0);
    uv = inverse(u_MVP) * gl_Position;
};

#shader fragment
#version 400 core

out vec4 FragColor;
in vec4 uv;

uniform dvec4 u_bounds;

double map(double value, double min1, double max1, double min2, double max2) {
    return min2 + (value - min1) * (max2 - min2) / (max1 - min1);
}

void main()
{
    dvec2 c = dvec2(
        map(uv.x, -1.0, 1.0, u_bounds.x, u_bounds.y),
        map(uv.y, -1.0, 1.0, u_bounds.z, u_bounds.w)
    );

    dvec2 z;
    float iter;
    for (iter = 0.0; iter < 255; iter++) {
        z = dvec2(z.x * z.x - z.y * z.y, 2 * z.x * z.y) + c;
        if (length(z) > 2) break;
    }

    FragColor = vec4(
        iter / 255,
        iter * iter / 255 / 255,
        sqrt(iter) / sqrt(255),
        1.0);
};