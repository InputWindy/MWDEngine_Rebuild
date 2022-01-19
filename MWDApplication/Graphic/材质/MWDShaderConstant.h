#pragma once
#include "../MWDApplication/Graphic/Core/MWDGraphic.h"
#include "../MWDApplication/Graphic/Core/MWDObject.h"
#include "../MWDApplication/Graphic/Core/MWDName.h"
namespace MWDEngine {
	class MWDStream;
	//维护shader常量的buffer和shader内外的名字:一个MWDShaderConstant就是一个shader内的变量
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
			VT_STRUCT,
			VT_MAX
		};
	public:
		MWDShaderConstant(const MWDName& Name, const void* pData, unsigned int uiSize,
			unsigned int uiRegisterIndex, unsigned int uiRegisterNum, unsigned int uiValueType = VT_FLOAT);
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
			return m_Name;
		}
		FORCEINLINE const MWDName& GetNameInShader()const
		{
			return m_NameInShader;
		}

		unsigned int m_uiSize;
		unsigned int m_uiValueType;
		unsigned int m_uiRegisterIndex;
		unsigned int m_uiRegisterNum;

	protected:
		friend class MWDRenderer;
		MWDShaderConstant() ;

	private:

		unsigned char* m_pData;

		MWDName m_Name;
		MWDName m_NameInShader;
	};
	MWDTYPE_MARCO(MWDShaderConstant);
	DECLARE_Ptr(MWDShaderConstant);
}

