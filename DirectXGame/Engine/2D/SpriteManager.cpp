#include "SpriteManager.h"
#include "DirectXBase.h"

#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

Matrix4 SpriteManager::SpriteProjection;

void SpriteManager::Init()
{
	HRESULT result = S_FALSE;

	SpriteManager::SpriteProjection = Matrix4
	{
		2.f / WinAPI::GetInstance()->width, 0, 0, 0,
		0, -2.f / WinAPI::GetInstance()->height, 0, 0,
		0, 0, 1, 0,
		-1, 1, 0, 1
	};

#pragma region ���_�V�F�[�_
	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/SpriteVS.hlsl", // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "vs_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&vsBlob_, &errorBlob_);

	// �G���[�Ȃ�
	if (FAILED(result))
	{
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob_->GetBufferSize());
		std::copy_n((char*)errorBlob_->GetBufferPointer(),
			errorBlob_->GetBufferSize(),
			error.begin());
		error += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}
#pragma endregion

#pragma region �s�N�Z���V�F�[�_
	// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/SpritePS.hlsl", // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "ps_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&psBlob_, &errorBlob_);

	// �G���[�Ȃ�
	if (FAILED(result))
	{
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob_->GetBufferSize());
		std::copy_n((char*)errorBlob_->GetBufferPointer(),
			errorBlob_->GetBufferSize(),
			error.begin());
		error += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}
#pragma endregion

	// ���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[] =
	{
		{
			"POSITION",									// �Z�}���e�B�b�N��
			0,											// �����Z�}���e�B�b�N������������Ƃ��Ɏg���C���f�b�N�X (0�ł������[)
			DXGI_FORMAT_R32G32B32_FLOAT,				// �v�f���ƃr�b�g����\�� (XYZ��3��float�^�Ȃ̂�R32G32B32_FLOAT)
			0,											// ���̓X���b�g�C���f�b�N�X (0�ł������[)
			D3D12_APPEND_ALIGNED_ELEMENT,				// �f�[�^�̃I�t�Z�b�g�l (D3D12_APPEND_ALIGNED_ELEMENT���Ǝ����ݒ�
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, // ���̓f�[�^��� (�W����D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DAT)
			0											// ��x�ɕ`�悷��C���X�^���X�� (0�ł������[)
		},
		{ // uv���W
			"TEXCOORD",									// �Z�}���e�B�b�N��
			0,											// �����Z�}���e�B�b�N������������Ƃ��Ɏg���C���f�b�N�X (0�ł������[)
			DXGI_FORMAT_R32G32_FLOAT,				    // �v�f���ƃr�b�g����\�� (uv��2��float�^�Ȃ̂�R32G32_FLOAT)
			0,											// ���̓X���b�g�C���f�b�N�X (0�ł������[)
			D3D12_APPEND_ALIGNED_ELEMENT,				// �f�[�^�̃I�t�Z�b�g�l (D3D12_APPEND_ALIGNED_ELEMENT���Ǝ����ݒ�
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, // ���̓f�[�^��� (�W����D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DAT)
			0											// ��x�ɕ`�悷��C���X�^���X�� (0�ł������[)
		},
	};

	// �V�F�[�_�[�̐ݒ�
	pipelineDesc_.VS.pShaderBytecode = vsBlob_->GetBufferPointer();
	pipelineDesc_.VS.BytecodeLength = vsBlob_->GetBufferSize();
	pipelineDesc_.PS.pShaderBytecode = psBlob_->GetBufferPointer();
	pipelineDesc_.PS.BytecodeLength = psBlob_->GetBufferSize();

	// �T���v���}�X�N�̐ݒ�
	pipelineDesc_.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�

	// ���X�^���C�U�̐ݒ�
	pipelineDesc_.RasterizerState.CullMode = D3D12_CULL_MODE_NONE; // �J�����O���Ȃ�
	pipelineDesc_.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // �|���S�����h��Ԃ�
	pipelineDesc_.RasterizerState.DepthClipEnable = true; // �[�x�N���b�s���O��L����

	// �u�����h�X�e�[�g
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc_.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; // RBGA�S�Ẵ`�����l����`��

	blenddesc.BlendEnable = true;
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	// ��������K�p
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	//// ���Z����
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;	// ���Z
	//blenddesc.SrcBlend = D3D12_BLEND_ONE; 	// �\�[�X�̒l��100% �g��
	//blenddesc.DestBlend = D3D12_BLEND_ONE;	// �f�X�g�̒l��100% �g��

	//// ���Z����
	//blenddesc.BlendOp = D3D12_BLEND_OP_SUBTRACT;	// ���Z
	//blenddesc.SrcBlend = D3D12_BLEND_ONE;			// �\�[�X�̒l��100% �g��
	//blenddesc.DestBlend = D3D12_BLEND_ONE;		// �f�X�g�̒l��100% �g��

	//// �F���]
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;			// ���Z
	//blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;	// 1.0f - �f�X�g�J���[�̒l
	//blenddesc.DestBlend = D3D12_BLEND_ZERO;			// �f�X�g�̒l��  0% �g��

	//// ����������
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;			// ���Z
	//blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;		// �\�[�X�̃A���t�@�l
	//blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;	// 1.0f - �\�[�X�̃A���t�@�l

	// ���_���C�A�E�g�̐ݒ�
	pipelineDesc_.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc_.InputLayout.NumElements = _countof(inputLayout);

	// �}�`�̌`��ݒ�
	pipelineDesc_.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	// ���̑��̐ݒ�
	pipelineDesc_.NumRenderTargets = 1; // �`��Ώۂ�1��
	pipelineDesc_.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255�w���RGBA
	pipelineDesc_.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

	// �e�N�X�`���T���v���[�̐ݒ�
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;	//   ���J��Ԃ�(�^�C�����O)
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;	//   �c�J��Ԃ�(�^�C�����O)
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;	// ���s�J��Ԃ�(�^�C�����O)
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;	// �{�[�_�[�̎��͍�
	samplerDesc.Filter = D3D12_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;		// �S�ă��j�A���
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;	// �~�b�v�}�b�v�ő�l
	samplerDesc.MinLOD = 0.0f;				// �~�b�v�}�b�v�ŏ��l
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	// �s�N�Z���V�F�[�_�[����̂ݎg�p�\

	// �f�X�N���v�^�����W�̐ݒ�
	D3D12_DESCRIPTOR_RANGE descriptorRange{};
	descriptorRange.NumDescriptors = 1;		// ��x�̕`��Ɏg���e�N�X�`����1���Ȃ̂�1
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;	// �e�N�X�`�����W�X�^0��
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	// ���[�g�p�����[�^�̐ݒ�
	D3D12_ROOT_PARAMETER rootParams[2] = {};

	// �萔�o�b�t�@0�� 
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;		// ���
	rootParams[0].Descriptor.ShaderRegister = 0;						// �萔�o�b�t�@�̔ԍ�
	rootParams[0].Descriptor.RegisterSpace = 0;							// �f�t�H���g�l
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;		// �S�ẴV�F�[�_���猩����

	// �e�N�X�`�����W�X�^0��
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	// ���
	rootParams[1].DescriptorTable.pDescriptorRanges = &descriptorRange;			// �f�X�N���v�^�����W
	rootParams[1].DescriptorTable.NumDescriptorRanges = 1;						// �f�t�H���g�l
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;				// �S�ẴV�F�[�_���猩����

	// ���[�g�V�O�l�`���̐ݒ�
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams;
	rootSignatureDesc.NumParameters = _countof(rootParams);
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	// ���[�g�V�O�l�`���̃V���A���C�Y
	ComPtr<ID3DBlob> rootSigBlob = nullptr;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob_);
	assert(SUCCEEDED(result));
	result = DirectXBase::GetInstance()->device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature_));
	assert(SUCCEEDED(result));

	// �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	pipelineDesc_.pRootSignature = rootSignature_.Get();

	result = DirectXBase::GetInstance()->device->CreateGraphicsPipelineState(&pipelineDesc_, IID_PPV_ARGS(&pipelineState_));
	assert(SUCCEEDED(result));
}

void SpriteManager::PreDraw()
{
	ID3D12GraphicsCommandList* commandList = DirectXBase::GetInstance()->commandList.Get();
	
	// �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
	commandList->SetPipelineState(pipelineState_.Get());
	commandList->SetGraphicsRootSignature(rootSignature_.Get());

	// �O�p�`�̃��X�g
	commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);	 

	// SRV�q�[�v�̐ݒ�R�}���h
	commandList->SetDescriptorHeaps(1, TextureManager::srvHeap.GetAddressOf());
}