#include "ParticleManager.h"
#include "DirectXBase.h"

ComPtr<ID3D12RootSignature> ParticleManager::rootSignature; 	// ���[�g�V�O�l�`��
ComPtr<ID3D12PipelineState> ParticleManager::pipelineState;	// �p�C�v���C��

// Float3���m�̉��Z����
const Float3 operator+(const Float3& Ihs, const Float3& rhs)
{
	Float3 result = { 0,0,0 };
	result.x = Ihs.x + rhs.x;
	result.y = Ihs.y + rhs.y;
	result.z = Ihs.z + rhs.z;
	return result;
}

void ParticleManager::CreatePipeline()
{
	HRESULT result = S_FALSE;

	// �f�X�N���v�^�����W�̐ݒ�
	D3D12_DESCRIPTOR_RANGE descriptorRange{};
	descriptorRange.NumDescriptors = 1;		// ��x�̕`��Ɏg���e�N�X�`����1���Ȃ̂�1
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;	// �e�N�X�`�����W�X�^0��
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	// ���[�g�p�����[�^�̐ݒ�
	D3D12_ROOT_PARAMETER rootParams[4] = {};

	// �萔�o�b�t�@0�� [�}�e���A��]
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;		// ���
	rootParams[0].Descriptor.ShaderRegister = 0;						// �萔�o�b�t�@�̔ԍ�
	rootParams[0].Descriptor.RegisterSpace = 0;							// �f�t�H���g�l
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;		// �S�ẴV�F�[�_���猩����

	// �e�N�X�`�����W�X�^0��
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;	// ���
	rootParams[1].DescriptorTable.pDescriptorRanges = &descriptorRange;			// �f�X�N���v�^�����W
	rootParams[1].DescriptorTable.NumDescriptorRanges = 1;						// �f�t�H���g�l
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;				// �S�ẴV�F�[�_���猩����

	// �萔�o�b�t�@1�� [�s��]
	rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;		// ���
	rootParams[2].Descriptor.ShaderRegister = 1;						// �萔�o�b�t�@�̔ԍ�
	rootParams[2].Descriptor.RegisterSpace = 0;							// �f�t�H���g�l
	rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;		// �S�ẴV�F�[�_���猩����

	// �萔�o�b�t�@2�� [�J����]
	rootParams[3].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;		// ���
	rootParams[3].Descriptor.ShaderRegister = 2;						// �萔�o�b�t�@�̔ԍ�
	rootParams[3].Descriptor.RegisterSpace = 0;							// �f�t�H���g�l
	rootParams[3].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;		// �S�ẴV�F�[�_���猩����

	ComPtr<ID3DBlob> vsBlob = nullptr;		 // ���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob = nullptr;		 // �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> gsBlob = nullptr;		 // �W�I���g���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob = nullptr;	 // �G���[�I�u�W�F�N�g
	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/ParticleVS.hlsl", // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "vs_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&vsBlob, &errorBlob);

	// �G���[�Ȃ�
	if (FAILED(result))
	{
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}

	// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/ParticlePS.hlsl", // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "ps_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&psBlob, &errorBlob);

	// �G���[�Ȃ�
	if (FAILED(result))
	{
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}

	// �W�I���g���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/ParticleGS.hlsl", // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "gs_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&gsBlob, &errorBlob);

	// �G���[�Ȃ�
	if (FAILED(result))
	{
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}

	// ���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[] =
	{
		{ // xyz���W
			"POSITION",									// �Z�}���e�B�b�N��
			0,											// �����Z�}���e�B�b�N������������Ƃ��Ɏg���C���f�b�N�X (0�ł������[)
			DXGI_FORMAT_R32G32B32_FLOAT,				// �v�f���ƃr�b�g����\�� (XYZ��3��float�^�Ȃ̂�R32G32B32_FLOAT)
			0,											// ���̓X���b�g�C���f�b�N�X (0�ł������[)
			D3D12_APPEND_ALIGNED_ELEMENT,				// �f�[�^�̃I�t�Z�b�g�l (D3D12_APPEND_ALIGNED_ELEMENT���Ǝ����ݒ�
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, // ���̓f�[�^��� (�W����D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DAT)
			0											// ��x�ɕ`�悷��C���X�^���X�� (0�ł������[)
		}, // ���W�ȊO�ɐF�A�e�N�X�`��UV�Ȃǂ�n���ꍇ�͍X�ɑ�����B

		{ // uv���W
			"TEXCOORD",									// �Z�}���e�B�b�N��
			0,											// �����Z�}���e�B�b�N������������Ƃ��Ɏg���C���f�b�N�X (0�ł������[)
			DXGI_FORMAT_R32G32_FLOAT,				    // �v�f���ƃr�b�g����\�� (uv��2��float�^�Ȃ̂�R32G32_FLOAT)
			0,											// ���̓X���b�g�C���f�b�N�X (0�ł������[)
			D3D12_APPEND_ALIGNED_ELEMENT,				// �f�[�^�̃I�t�Z�b�g�l (D3D12_APPEND_ALIGNED_ELEMENT���Ǝ����ݒ�
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, // ���̓f�[�^��� (�W����D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DAT)
			0											// ��x�ɕ`�悷��C���X�^���X�� (0�ł������[)
		}, // ���W�ȊO�ɐF�A�e�N�X�`��UV�Ȃǂ�n���ꍇ�͍X�ɑ�����B

		{ // �J���[
			"COLOR",									// �Z�}���e�B�b�N��
			0,											// �����Z�}���e�B�b�N������������Ƃ��Ɏg���C���f�b�N�X (0�ł������[)
			DXGI_FORMAT_R32G32B32A32_FLOAT,				// �v�f���ƃr�b�g����\�� (XYZ��3��float�^�Ȃ̂�R32G32B32_FLOAT)
			0,											// ���̓X���b�g�C���f�b�N�X (0�ł������[)
			D3D12_APPEND_ALIGNED_ELEMENT,				// �f�[�^�̃I�t�Z�b�g�l (D3D12_APPEND_ALIGNED_ELEMENT���Ǝ����ݒ�
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, // ���̓f�[�^��� (�W����D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DAT)
			0											// ��x�ɕ`�悷��C���X�^���X�� (0�ł������[)
		}, // ���W�ȊO�ɐF�A�e�N�X�`��UV�Ȃǂ�n���ꍇ�͍X�ɑ�����B
	};

	// �O���t�B�b�N�X�p�C�v���C���ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	// �V�F�[�_�[�̐ݒ�
	pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();
	pipelineDesc.GS.pShaderBytecode = gsBlob->GetBufferPointer();
	pipelineDesc.GS.BytecodeLength = gsBlob->GetBufferSize();

	// �T���v���}�X�N�̐ݒ�
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�

	// ���X�^���C�U�̐ݒ�
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK; // �w�ʂ��J�����O
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // �|���S�����h��Ԃ�
	//pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME; // �|���S������h��Ԃ��Ȃ��ꍇ�͂�����(���C���[�t���[��)
	pipelineDesc.RasterizerState.DepthClipEnable = true; // �[�x�N���b�s���O��L����

	// �u�����h�X�e�[�g
	//pipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; // RBGA�S�Ẵ`�����l����`��
	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; // RBGA�S�Ẵ`�����l����`��
	// ���Z
	blenddesc.BlendEnable = true;
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;

	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;
	blenddesc.SrcBlend = D3D12_BLEND_ONE;
	blenddesc.DestBlend = D3D12_BLEND_ONE;

	// ���_���C�A�E�g�̐ݒ�
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);

	// �}�`�̌`��ݒ�
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;

	// ���̑��̐ݒ�
	pipelineDesc.NumRenderTargets = 1; // �`��Ώۂ�1��
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255�w���RGBA
	pipelineDesc.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

	// �e�N�X�`���T���v���[�̐ݒ�
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;	// ���J��Ԃ�(�^�C�����O)
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;	// �c�J��Ԃ�(�^�C�����O)
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;	// ���s�J��Ԃ�(�^�C�����O)
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;	// �{�[�_�[�̎��͍�
	samplerDesc.Filter = D3D12_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;		// �S�ă��j�A���
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;	// �~�b�v�}�b�v�ő�l
	samplerDesc.MinLOD = 0.0f;				// �~�b�v�}�b�v�ŏ��l
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;	// �s�N�Z���V�F�[�_�[����̂ݎg�p�\

	// ���[�g�V�O�l�`���̐ݒ�
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams;	// ���[�g�p�����[�^�̐擪�A�h���X
	rootSignatureDesc.NumParameters = _countof(rootParams);		// ���[�g�p�����[�^��
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;

	// ���[�g�V�O�l�`���̃V���A���C�Y
	ComPtr<ID3DBlob> rootSigBlob = nullptr;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));
	result = DirectXBase::Get()->device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result));
	// rootSigBlob->Release();

	// �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	pipelineDesc.pRootSignature = rootSignature.Get();

	// �f�v�X�X�e���V���X�e�[�g�̐ݒ�
	pipelineDesc.DepthStencilState.DepthEnable = true;							// �[�x�e�X�g���s��
	pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;// �������ݕs�I�I�I�I�I�I�I�I�I�I
	pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;		// ��������΍��i
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;		// �[�x�l�t�H�[�}�b�g

	// �p�C�v�����X�e�[�g�̐���
	result = DirectXBase::Get()->device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result));
}

// �p�[�e�B�N��������
void ParticleManager::InitializeParticle()
{
	HRESULT result = S_FALSE;

	{
		// �萔�o�b�t�@�̃q�[�v�ݒ�
		D3D12_HEAP_PROPERTIES heapProp{};
		heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;

		// �萔�o�b�t�@�̃��\�[�X�ݒ�
		D3D12_RESOURCE_DESC resdesc{};
		resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resdesc.Width = (sizeof(constMapTransform) + 0xff) & ~0xff;
		resdesc.Height = 1;
		resdesc.DepthOrArraySize = 1;
		resdesc.MipLevels = 1;
		resdesc.SampleDesc.Count = 1;
		resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		// �萔�o�b�t�@�̐���
		result = DirectXBase::Get()->device->CreateCommittedResource(
			&heapProp,
			D3D12_HEAP_FLAG_NONE,
			&resdesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&constBuffTransform));
		assert(SUCCEEDED(result));

		// �萔�o�b�t�@�̃}�b�s���O
		result = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform);
		assert(SUCCEEDED(result));
	}

	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	UINT sizeVB = static_cast<UINT>(sizeof(Vertex) * vertexCount);

	// ���_�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES heapprop{};
	heapprop.Type = D3D12_HEAP_TYPE_UPLOAD;

	// ���_�f�[�^ ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = sizeVB;
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// ���_�o�b�t�@�̐���
	DirectXBase::Get()->device->CreateCommittedResource(
		&heapprop,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff)
	);

	// PIX�f�o�b�O�p�̖��O�t��
	vertBuff->SetName(L"PARTICLE VERT BUFF");

	// GPU��̃o�b�t�@�ɑΉ��������z���������擾
	vertBuff->Map(0, nullptr, (void**)&vertMap);

	// ���_�o�b�t�@�r���[�̍쐬
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeVB;
	vbView.StrideInBytes = sizeof(Vertex);
}

// �p�[�e�B�N���X�V����
void ParticleManager::UpdateParticle()
{
	// �������s�����p�[�e�B�N����S�폜
	particles.remove_if(
		[](Particle& x) {
			return x.frame >= x.num_frame;
		}
	);

	// �S�p�[�e�B�N���X�V
	for (forward_list<Particle>::iterator it = particles.begin(); it != particles.end(); it++) 
	{
		// �o�߃t���[�����J�E���g
		it->frame++;
		it->velocity = it->velocity + it->accel;
		it->position = it->position + it->velocity;

		float f = (float)it->frame / it->num_frame;

		it->scale = (it->e_scale - it->s_scale) * f;
		it->scale += it->s_scale;
	}

	// ��������0�ŏ�����
	activeCount = 0;

	// ���_�o�b�t�@�փf�[�^�]��
	// �p�[�e�B�N���̏���1�����f
	for (forward_list<Particle>::iterator it = particles.begin(); it != particles.end(); it++)
	{
		it->Update();

		vertMap[activeCount].pos = it->position;
		vertMap[activeCount].scale = Float2{it->scale, it->scale};
		vertMap[activeCount].color = it->color;

		activeCount++;
	}
}

// �p�[�e�B�N���`��
void ParticleManager::DrawParticle(TextureIndex index)
{
	// ���_�o�b�t�@�r���[�̐ݒ�
	DirectXBase::Get()->commandList->IASetVertexBuffers(0, 1, &vbView);

	// �萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
	DirectXBase::Get()->commandList->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());

	// �V�F�[�_���\�[�X�r���[���Z�b�g
	DirectXBase::Get()->commandList->
		SetGraphicsRootDescriptorTable
		(1, TextureManager::GetData(index)->gpuHandle);

	// �`��R�}���h
	DirectXBase::Get()->commandList->
		DrawInstanced((UINT)activeCount, 1, 0, 0);
}

void ParticleManager::Add(int life, Float3 position, Float3 velocity, Float3 accel, float start_scale, float end_scale)
{
	// ���X�g�ɗv�f�ǉ�
	particles.emplace_front();
	// �ǉ������v�f�̎Q��
	Particle& p = particles.front();
	// �l�̃Z�b�g
	p.position = position;
	p.velocity = velocity;
	p.accel = accel;
	p.num_frame = life;
	p.scale = start_scale;
	p.s_scale = start_scale;
	p.e_scale = end_scale;
}