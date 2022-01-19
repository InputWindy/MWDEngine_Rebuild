#pragma once
#include "../MWDBind.h"
namespace MWDEngine
{
	//ά��һ�׶������ݽ�����ʽ
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

		//������������
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

		//ά��һ�ֶ������ݣ������������ͣ�offset�����壬����idx��
		struct VERTEXFORMAT_TYPE
		{
		public:
			VERTEXFORMAT_TYPE()
			{
				OffSet = 0;
				DataType = 0;
				Semantics = 0;//����
				SemanticsIndex = 0;
			}
			~VERTEXFORMAT_TYPE()
			{

			}
			UINT OffSet;
			UINT DataType;			//�������ͣ�int��float��double......��
			UINT Semantics;			//�������ͣ�position��normal��texcoord......��
			UINT SemanticsIndex;	//��index�׶�������
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
		//MWDRenderδʵ��
		virtual bool OnLoadResource(MWDOpenglResource*& pID) {
			if (!m_pUser)
				return 0;

			/*if (!m_pUser->OnLoadVBufferFormat(this, pID))
				return 0;*/

			return 1;
		};
		//MWDRenderδʵ��
		virtual bool OnReleaseResource(MWDOpenglResource* pID) {
			if (!m_pUser)
				return 0;
			/*if (!m_pUser->OnReleaseVBufferFormat(pID))
				return 0;*/
			return 1;
		};
		//CRC32У����
		unsigned int m_uiVertexFormatCode;
	public:
		//ά���������ݵ�����
		MWDArray<MWDVertexFormat::VERTEXFORMAT_TYPE> m_FormatArray;
		FORCEINLINE unsigned int GetVertexFormatCode()const
		{
			return m_uiVertexFormatCode;
		}

	};
	DECLARE_Ptr(MWDVertexFormat);
	MWDTYPE_MARCO(MWDVertexFormat);

}


