#version 460 core

layout(quads, equal_spacing, ccw) in;

out vec2 TexCoord;

void main()
{
    vec4 p[5][5];
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            p[j][i] = gl_in[i * 5 + j].gl_Position;
        }
    }

	float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;
    TexCoord = vec2(u, v);

    float bu[5];
    bu[0] = pow((1.0f-u), 4);
    bu[1] = 4.0f * u * pow((1.0f-u), 3);
    bu[2] = 6.0f * pow(u, 2) * pow((1.0f-u), 2);
    bu[3] = 4.0f * pow(u, 3) * (1.0f-u);
    bu[4] = pow(u, 4);

    float bv[5];
    bv[0] = pow((1.0f-v), 4);
    bv[1] = 4.0f * v * pow((1.0f-v), 3);
    bv[2] = 6.0f * pow(v, 2) * pow((1.0f-v), 2);
    bv[3] = 4.0f * pow(v, 3) * (1.0f-v);
    bv[4] = pow(v, 4);

    vec4 res = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    for (int i = 0; i < 5; i++)
    {
        for (int j = 0; j < 5; j++)
        {
            res += bu[i] * bv[j] * p[i][j];
        }
    }
    gl_Position = res;	
}
