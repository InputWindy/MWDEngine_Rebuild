#pragma once
#include "../MWDBind.h"
namespace MWDEngine
{
	//维护一套顶点数据解析格式
	class  MWDVertexFormat : public MWDBind
	{
		DECLARE_CLASS_FUNCTION(MWDVertexFormat)
		DECLARE_RTTI(MWDVertexFormat, MWDBind)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDVertexFormat)
	public:
		MWDVertexFormat() {
			m_bIsStatic = 1;
		};
		virtual ~MWDVertexFormat() {
			ReleaseResource();
		};

		//顶点数据类型
		enum
		{
			VF_POSITION,
			VF_TEXCOORD,
			VF_NORMAL,
			VF_TANGENT,
			VF_BINORMAL,
			VF_PSIZE,
			VF_COLOR,
			VF_FOG,
			VF_DEPTH,
			VF_BLENDWEIGHT,
			VF_BLENDINDICES,
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
		friend class MWDRenderer;
		friend class MWDDX9Renderer;
		friend class MWDOpenGlRenderer;
		friend class MWDMWDhaderKey;
		friend class MWDPShaderKey;
		friend class MWDResourceManager;
		friend class MWDShaderKey;

	protected:
		//MWDRender未实现
		virtual bool OnLoadResource(MWDOpenglResource*& pID) {
			if (!m_pUser)
				return 0;

			/*if (!m_pUser->OnLoadVBufferFormat(this, pID))
				return 0;*/

			return 1;
		};
		//MWDRender未实现
		virtual bool OnReleaseResource(MWDOpenglResource* pID) {
			if (!m_pUser)
				return 0;
			/*if (!m_pUser->OnReleaseVBufferFormat(pID))
				return 0;*/
			return 1;
		};
		//CRC32校验码
		unsigned int m_uiVertexFormatCode;
	public:
		//维护顶点数据的数组
		MWDArray<MWDVertexFormat::VERTEXFORMAT_TYPE> m_FormatArray;
		FORCEINLINE unsigned int GetVertexFormatCode()const
		{
			return m_uiVertexFormatCode;
		}

	};
	DECLARE_Ptr(MWDVertexFormat);
	MWDTYPE_MARCO(MWDVertexFormat);

}


