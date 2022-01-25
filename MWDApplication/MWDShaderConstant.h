#pragma once
#include "../Graphic/Core/MWDObject.h"
#include "../Graphic/Core/MWDName.h"
namespace MWDEngine {
	//ά��shader������buffer��shader���������:һ��MWDShaderConstant����һ��shader�ڵı���
	class  MWDShaderConstant : public MWDObject
	{
		DECLARE_CLASS_FUNCTION(MWDShaderConstant)
		DECLARE_RTTI(MWDShaderConstant, MWDObject)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDShaderConstant)
	public:
		enum //Value Type
		{
			VT_FLOAT,
			VT_BOOL,
			VT_INT,
			VT_VEC2,
			VT_VEC3,
			VT_VEC4,
			VT_MAT3,
			VT_MAT4,
			VT_STRUCT,
			VT_MAX
		};
	public:
		//vec2��vec3��vec4��mat3��mat4��ͨ��ͳһ��floatֵ
		MWDShaderConstant(const MWDName& Name, const void* pData, unsigned int uiSize,
			unsigned int uiRegisterIndex, unsigned int uiRegisterNum, unsigned int uiValueType = VT_FLOAT);
		//vec2��vec3��vec4��mat3��mat4��ͨ��ͳһ��floatֵ
		MWDShaderConstant(const MWDName& Name, const MWDName& NameInShader, const void* pData, unsigned int uiSize,
			unsigned int uiRegisterIndex, unsigned int uiRegisterNum, unsigned int uiValueType = VT_FLOAT);
		virtual ~MWDShaderConstant();

		bool SetData(void* pElementData) {
			if (!pElementData)
				return 0;
			void* pDest = (unsigned char*)m_pData;
			MWDMemcpy(pDest, pElementData, m_uiSize);

			return 1;
		};
		FORCEINLINE void* GetData()const {
			return m_pData;
		};

		FORCEINLINE unsigned int GetValueType()const {
			return m_uiValueType;
		};
		FORCEINLINE unsigned int GetSize()const {
			return m_uiSize;
		};
		FORCEINLINE unsigned int GetRegisterNum()const {
			return m_uiRegisterNum;
		};
		FORCEINLINE unsigned int GetRegisterIndex()const {
			return m_uiRegisterIndex;
		};

		FORCEINLINE const MWDName& GetShowName()const
		{
			return m_showName;
		}
		FORCEINLINE const MWDName& GetNameInShader()const
		{
			return m_nameInShader;
		}

		unsigned int m_uiSize;
		unsigned int m_uiValueType;
		unsigned int m_uiRegisterIndex;
		unsigned int m_uiRegisterNum;

	protected:
		friend class MWDRenderer;
		MWDShaderConstant();

	private:

		unsigned char* m_pData;

		MWDName m_showName;
		MWDName m_nameInShader;
	};
	MWDTYPE_MARCO(MWDShaderConstant);
	DECLARE_Ptr(MWDShaderConstant);
}


