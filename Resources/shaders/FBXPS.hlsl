#include "FBX.hlsli"

Texture2D<float4> tex : register(t0);	// 0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0);		// 0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

// �G���g���[�|�C���g
float4 main(VSOutput input) : SV_TARGET
{
	// �e�N�X�`���}�b�s���O
	float4 texColor = tex.Sample(smp,input.uv);
	// Lambert����
	float3 light = normalize(float3(1, -1, 1));	// �E���������̃��C�g
	float diffuse = saturate(dot(-light, input.normal));
	float brightness = diffuse + 0.3f;
	float4 shaderColor = float4(brightness, brightness, brightness, 1.0f);

	// �A�e�ƃe�N�X�`���̐F������
	return shaderColor * texColor;
}