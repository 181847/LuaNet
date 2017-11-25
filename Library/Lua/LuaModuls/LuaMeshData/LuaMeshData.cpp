#include "LuaMeshData.h"

#ifdef __cplusplus
extern "C"
{
#endif
int LUAMESHDATA_API luaopen_MeshData(lua_State * L)
{
	luaL_newmetatable(L, "LoadAssets.MeshData");
	lua_pushvalue(L, -1);
	lua_setfield(L, -2, "__index");
	luaL_setfuncs(L, MeshDataLib_m, 0);
	luaL_newlib(L, MeshDataLib);
	return 1;
}
#ifdef __cplusplus
}
#endif

int lua_newMeshData(lua_State * L)
{
	// convert the user data,
	// using a lambda of the struct to automatically free the object.
	auto *pContainer = reinterpret_cast<LuaPointerContainer<Lua::MeshData>*>
		(lua_newuserdata(L, sizeof(LuaPointerContainer<Lua::MeshData>)));

	auto * pMeshData = new Lua::MeshData();
	pContainer->setPointer(pMeshData);

	// the next few lines add empty data to the vector,
	// these data all have the index 0, 
	// which stand for the illegal data.
	pMeshData->Positions.push_back({ 0.0f, 0.0f, 0.0f });
	pMeshData->Normals.push_back({ 0.0f, 0.0f, 0.0f });
	pMeshData->TangentUs.push_back({ 0.0f, 0.0f, 0.0f });
	pMeshData->Texcoords.push_back({ 0.0f, 0.0f });
	pMeshData->Indices32.push_back(0);
	pMeshData->Vertices.push_back(Lua::Vertex(
		pMeshData->Positions[0],
		pMeshData->Normals[0],
		pMeshData->TangentUs[0],
		pMeshData->Texcoords[0]));

	luaL_getmetatable(L, "LoadAssets.MeshData");
	lua_setmetatable(L, -2);
	
	return 1;
}

int lua_showMeshData(lua_State * L)
{
	// check and get mesh data
	auto *pMeshData = checkMeshData(L);
	printf("Vertex count:\t\t%d\n", pMeshData->Vertices.size() - 1);
	printf("Index count:\t\t%d\n", pMeshData->Indices32.size() - 1);
	printf("Position count:\t\t%d\n", pMeshData->Positions.size() - 1);
	printf("Normal count:\t\t%d\n", pMeshData->Normals.size() - 1);
	printf("TangentU count:\t\t%d\n", pMeshData->TangentUs.size() - 1);
	printf("TextureCoord count:\t%d\n", pMeshData->Texcoords.size() - 1);
	return 0;
}

int lua_getDetail(lua_State * L)
{
	auto *pMeshData = checkMeshData(L);

	// create a new table
	lua_newtable(L);
	lua_pushinteger(L, pMeshData->Vertices.size() - 1);
	lua_setfield(L, -2, "VertexCount");
	lua_pushinteger(L, pMeshData->Indices32.size() - 1);
	lua_setfield(L, -2, "IndexCount");
	lua_pushinteger(L, pMeshData->Positions.size() - 1);
	lua_setfield(L, -2, "PositionCount");
	lua_pushinteger(L, pMeshData->Normals.size() - 1);
	lua_setfield(L, -2, "NormalCount");
	lua_pushinteger(L, pMeshData->TangentUs.size() - 1);
	lua_setfield(L, -2, "TangentUCount");
	lua_pushinteger(L, pMeshData->Texcoords.size() - 1);
	lua_setfield(L, -2, "TextureCoordCount");
	return 1;
}

int lua_addPosition(lua_State * L)
{
	auto * pMeshData = checkMeshData(L);
	auto px = static_cast<float>(luaL_checknumber(L, 2));
	auto py = static_cast<float>(luaL_checknumber(L, 3));
	auto pz = static_cast<float>(luaL_checknumber(L, 4));


	pMeshData->Positions.push_back(DirectX::XMFLOAT3(px, py, pz));
	DEBUG_MESSAGE("position added: %f, %f, %f\n", px, py, pz);
	return 0;
}

int lua_addNormal(lua_State * L)
{
	auto * pMeshData = checkMeshData(L);
	auto px = static_cast<float>(luaL_checknumber(L, 2));
	auto py = static_cast<float>(luaL_checknumber(L, 3));
	auto pz = static_cast<float>(luaL_checknumber(L, 4));


	pMeshData->Normals.push_back(DirectX::XMFLOAT3(px, py, pz));
	DEBUG_MESSAGE("normal added: %f, %f, %f\n", px, py, pz);
	return 0;
}

int lua_addTangentU(lua_State * L)
{
	auto * pMeshData = checkMeshData(L);
	auto px = static_cast<float>(luaL_checknumber(L, 2));
	auto py = static_cast<float>(luaL_checknumber(L, 3));
	auto pz = static_cast<float>(luaL_checknumber(L, 4));


	pMeshData->TangentUs.push_back(DirectX::XMFLOAT3(px, py, pz));
	DEBUG_MESSAGE("tangentU added: %f, %f, %f\n", px, py, pz);
	return 0;
}

int lua_addTexC(lua_State * L)
{
	auto * pMeshData = checkMeshData(L);
	auto pu = static_cast<float>(luaL_checknumber(L, 2));
	auto pv = static_cast<float>(luaL_checknumber(L, 3));


	pMeshData->Texcoords.push_back(DirectX::XMFLOAT2(pu, pv));
	DEBUG_MESSAGE("textureCoord added: %f, %f\n", pu, pv);
	return 0;
}

int lua_addVertex(lua_State * L)
{
	auto * pMeshData	= checkMeshData(L);
	auto positionIndex	= 
		static_cast<unsigned int>(luaL_checkinteger(L, 2));
	auto texCIndex =
		static_cast<unsigned int>(luaL_checkinteger(L, 3));
	auto normalIndex	= 
		static_cast<unsigned int>(luaL_checkinteger(L, 4));
	auto tangentUIndex	= 
		static_cast<unsigned int>(luaL_checkinteger(L, 5));

	auto posCount = pMeshData->Positions.size();
	auto texCount = pMeshData->Texcoords.size();
	auto nmlCount = pMeshData->Normals.size();
	auto tguCount = pMeshData->TangentUs.size();


	DEBUG_MESSAGE("add vertex: %d\t%d\t%d\t%d\n",
		positionIndex, normalIndex, tangentUIndex, texCIndex);
	DEBUG_MESSAGE("size comp : %d\t%d\t%d\t%d\n",
		posCount, nmlCount, tguCount, texCount);

	// check index range
	if (	positionIndex	>= posCount
		||  texCIndex		>= texCount
		||	normalIndex		>= nmlCount
		||	tangentUIndex	>= tguCount)
	{
		lua_pushboolean(L, false);
		lua_pushstring(L, "some compent index out of range.");
		lua_pushboolean(L, positionIndex >= posCount);
		lua_pushboolean(L, texCIndex >= texCount);
		lua_pushboolean(L, normalIndex >= nmlCount);
		lua_pushboolean(L, tangentUIndex >= tguCount);
		return 2;
	}

	pMeshData->Vertices.push_back(
		Lua::Vertex(
			pMeshData->Positions[positionIndex],
			pMeshData->Normals	[normalIndex],
			pMeshData->TangentUs[tangentUIndex],
			pMeshData->Texcoords[texCIndex]
		));

	// oppose to the index out of range,
	// when there is no error,
	// return true.
	lua_pushboolean(L, true);
	
	return 1;
}

int lua_addIndex(lua_State * L)
{
	auto * pMeshData = checkMeshData(L);
	auto index = luaL_checkinteger(L, 2);

	if (index >= pMeshData->Vertices.size())
	{
		lua_pushboolean(L, false);
		lua_pushstring(L, "the vertex index out of range.");
		return 2;
	}
	pMeshData->Indices32.push_back(static_cast<Lua::uint32>(index));
	
	// operation success
	lua_pushboolean(L, true);
	return 1;
}

int lua_help(lua_State * L)
{

	printf("\n****** HELP MESH DATA *****\n");
	fprintf(stderr, ":show:				show the statics of the mesh.\n");
	fprintf(stderr, ":addPosition:		add one position to the buffer, please pass 3 number.\n");
	fprintf(stderr, ":addNormal:		add one normal to the buffer, please pass 3 number.\n");
	fprintf(stderr, ":addTangentU:		add one tangentU to the buffer, please pass 3 number.\n");
	fprintf(stderr, ":addTextureCoord:	add one textureCoord to the buffer, please pass 2 number.\n");
	fprintf(stderr, ":addVertex:		add one Vertex to the Vertices, must pass 4 integer, and the index just start from 1(same as in the .obj file).\n");
	fprintf(stderr, "						index order: position normal textureCoord tangent");
	fprintf(stderr, ":help:				show this information.\n");
	fprintf(stderr, ":getDetail:		this is like the show method, but instead print the information, ");
	fprintf(stderr, ":						this function return a table contain the information:");
	fprintf(stderr, "		VertexCount/IndexCount/PositionCount/NormalCount/TangentUCount/TextureCoordCount");

	printf("\n****** HELP MESH DATA *****\n");

	return 0;
}

int lua_gc(lua_State * L)
{
	checkConainer(L)->freePointer();

	DEBUG_MESSAGE("MeshData has been deleted.\n");

	return 0;
}
