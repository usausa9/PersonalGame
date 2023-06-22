// �}�e���A��
cbuffer ConstBufferDataMaterial : register(b0)
{
    float3 hAmbient : packoffset(c0); // �A���r�G���g�W��
									
    float3 hDiffuse : packoffset(c1); // �f�B�t���[�Y�W��
									
    float3 hSpecular : packoffset(c2); // �X�y�L�����[�W��
    float alpha : packoffset(c2.w); // �A���t�@
	
	// // �F(RGBA)
	float4 color : packoffset(c3);
};

// 3D�ϊ��s��
cbuffer ConstBufferDataTransform : register(b1)
{
	// 3D�ϊ��s��
	matrix world;
};

// �J����
cbuffer ConstBufferDataCamera : register(b2)
{ 
    matrix view;
    matrix projection;
    matrix billboard;
    float3 cameraPos; 
};

// ���_�V�F�[�_�[�̏o�͍\����
// (���_�V�F�[�_�[����s�N�Z���V�F�[�_�[�ւ̂����Ɏg�p����)
struct VSOutput
{ 
	float4 svpos : SV_POSITION;	// �V�X�e���p���_���W
    float4 worldpos : POSITION; // ���[���h���W
	float3 normal : NORMAL;		// �@���x�N�g��
	float2 uv : TEXCOORD;		// uv�l
};