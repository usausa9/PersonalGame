#pragma once

/**
 * @file	Common.h
 * @brief	雑多な.hの読み込み等
**/

// ComPtr
#include <wrl.h>
using namespace Microsoft::WRL;

#include <DirectXMath.h>
#include "UsaMath.h"
using namespace DirectX;
using namespace std;

#include <d3d12.h>
#include <d3dx12.h>
#include <dxgi1_6.h>

#include <cassert>
#include <vector>
#include <string>
#include <array>
#include <list>
#include <forward_list>

#pragma comment (lib,"d3d12.lib")
#pragma comment (lib,"dxgi.lib")

#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")