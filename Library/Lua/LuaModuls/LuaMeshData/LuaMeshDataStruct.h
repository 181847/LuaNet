#pragma once


// my help tools
#include "../../../MyTools/LuaTools.h"
#include "../../../MyTools/MyTools.h"
#include "../../../MyTools/Cleaner.h"

// necessary directX modules
#include "../../../DirectX12/Common/GeometryGenerator.h"
#include "../../../DirectX12/Common/d3dUtil.h"

// Here define the geometry container for store the mesh data in lua userData.
namespace Lua
{
	using uint16 = std::uint16_t;
	using uint32 = std::uint32_t;
	struct Vertex
	{
		DirectX::XMFLOAT3 Position;
		DirectX::XMFLOAT3 Normal;
		DirectX::XMFLOAT3 TangentU;
		DirectX::XMFLOAT2 TexC;

		Vertex(
			DirectX::XMFLOAT3 pos,
			DirectX::XMFLOAT3 nml,
			DirectX::XMFLOAT3 tgu,
			DirectX::XMFLOAT2 tec)
			:Position(pos), Normal(nml), TangentU(tgu), TexC(tec) {}
	};

	struct MeshData
	{
		std::vector<Vertex> Vertices;
		std::vector<uint32> Indices32;

		// for read object,
		// here we save the component seperately.
		std::vector<DirectX::XMFLOAT3> Positions;
		std::vector<DirectX::XMFLOAT3> Normals;
		std::vector<DirectX::XMFLOAT3> TangentUs;
		std::vector<DirectX::XMFLOAT2> Texcoords;

	private:
		std::vector<uint16> mIndices16;

	public:
		std::vector<uint16>& GetIndices16()
		{
			if (mIndices16.empty())
			{
				mIndices16.resize(Indices32.size());
				for (size_t i = 0; i < Indices32.size(); ++i)
				{
					mIndices16[i] = static_cast<uint16>(Indices32[i]);
				}
			}

			return mIndices16;
		}
	};
}
