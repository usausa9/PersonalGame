#include "Basic.hlsli"

VSOutput main(float4 pos : POSITION, float3 normal : NORMAL, float2 uv : TEXCOORD)
{
    float4 wnormal = normalize(mul(world, float4(normal, 0)));
    float4 wpos = mul(world, pos);
	
	VSOutput output;			 // �s�N�Z���V�F�[�_�[�ɓn���l
    output.svpos = mul(mul(projection, mul(view, world)), pos); //	���W�ɍs�����Z
    output.worldpos = wpos;
	output.normal = normal;
	output.uv = uv;
	return output;
}

//float4 main(float4 pos : POSITION) : SV_POSITION
//{
//	/*return pos + float4(0.5f,0.5f,0,0);    �E���0.5f���ړ� */ 
//	/*return pos * float4(1.5f, 1.5f, 1, 1); 1.5�{ �g�� */
//	return pos;
//}