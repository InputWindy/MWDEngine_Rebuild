#pragma once
#include "MWDBind.h"
namespace MWDEngine
{
	//维护一套顶点数据解析格式
	class  MWDVertexFormat : public MWDObject
	{
		DECLARE_CLASS_FUNCTION(MWDVertexFormat)
		DECLARE_RTTI(MWDVertexFormat, MWDObject)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDVertexFormat)
	public:
		//顶点数据类型
		enum
		{
			VF_POSITION,		//Location=0
			VF_TEXCOORD,		//Location=1
			VF_NORMAL,			//Location=2
			VF_TANGENT,			//Location=3
			VF_BINORMAL,		//Location=4
			VF_PSIZE,			//Location=5
			VF_COLOR,			//Location=6
			VF_FOG,				//Location=7
			VF_DEPTH,			//Location=8
			VF_BLENDWEIGHT,		//Location=9
			VF_BLENDINDICES,	//Location=10
			VF_MAX
		};
		//维护一种顶点数据（包括数据类型，offset，语义，语义idx）
		struct VERTEXFORMAT_TYPE
		{
		public:
			VERTEXFORMAT_TYPE()
			{
				OffSet = 0;
				DataType = 0;
				Semantics = 0;//语义
				SemanticsIndex = 0;
			}
			~VERTEXFORMAT_TYPE()
			{

			}
			UINT OffSet;
			UINT DataType;			//数据类型（int，float，double......）
			UINT Semantics;			//属性类型（position，normal，texcoord......）
			UINT SemanticsIndex;	//第index套顶点属性
		};

	public:
		//维护顶点数据的数组
		MWDArray<MWDVertexFormat::VERTEXFORMAT_TYPE> m_FormatArray;

	};
	DECLARE_Ptr(MWDVertexFormat);
	MWDTYPE_MARCO(MWDVertexFormat);

}


