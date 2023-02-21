cbuffer constant_buffer : register(b0) 
{
    matrix world;           // ������� ����
    matrix view;            // ������� ������
    matrix projection;      // ������� ��������
    float4 light_dir[2];    // �������� ������� �����(�����, ����-�� ��� ������� �����)
    float4 light_color[2];  // ���� ������� �����
    float4 output_color;    // ���� ����(��� ���� ����������)
}

struct vertex_shader_data
{
    float4 pos : POSITION;  // ������� �� xyz
    float3 norm : NORMAL;   // ������� �� xyz
};

struct pixel_shader_data
{
    float4 pos : SV_POSITION;   // ������� ������ � ��������(�������)
    float3 norm : TEXCOORD0;    // ³������ ������� ������ �� tu, tv
};

// ��������� ������
pixel_shader_data vertex_shader_func(vertex_shader_data data)
{
    pixel_shader_data to_ret = (pixel_shader_data) 0;
    to_ret.pos = mul(data.pos, world);
    to_ret.pos = mul(to_ret.pos, view);
    to_ret.pos = mul(to_ret.pos, projection);
    to_ret.norm = mul(data.norm, world);

    return to_ret;
}

// ϳ�������� ������ ��� ����
float4 pixel_shader_func(pixel_shader_data data) : SV_Target
{
    float4 final_color = 0;

    // ������ ���������� ������ �� ��� ������
    for (int i = 0; i < 2; ++i)
    {
        final_color += saturate(dot((float3) light_dir[i], data.norm) * light_color[i]);
    }
    final_color.a = 1;
    return final_color;
}

// ϳ�������� ������ ��� ������� �����
float4 pixel_shader_solid_func(pixel_shader_data data) : SV_Target
{
    return output_color;
}

