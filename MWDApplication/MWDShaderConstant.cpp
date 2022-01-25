#include "MWDShaderConstant.h"
namespace MWDEngine {
    INITIAL_WITH_INIT_TERMINAL_BEGIN(MWDShaderConstant)
    ADD_PRIORITY(MWDObject)
    ADD_INITIAL_FUNCTION(InitialClassFactory)
    INITIAL_WITH_INIT_TERMINAL_END(MWDShaderConstant)

    BEGIN_ADD_PROPERTY(MWDShaderConstant)
    END_ADD_PROPERTY

    MWDShaderConstant::MWDShaderConstant() {
        m_pData = NULL;
        m_uiRegisterIndex = 0;
        m_uiValueType = MWDMAX_INTEGER;
        m_uiRegisterNum = 0;
        m_uiSize = 0;
    };

    //shader内外名字不一样
    MWDShaderConstant::MWDShaderConstant(const MWDName& Name, const MWDName& NameInShader, const void* pData, unsigned int uiSize,
        unsigned int uiRegisterIndex, unsigned int uiRegisterNum, unsigned int uiValueType) {
        MWDMAC_ASSERT(uiValueType < VT_MAX&& uiSize > 0 && uiRegisterNum > 0);

        m_showName = Name;
        m_uiValueType = uiValueType;
        m_uiSize = uiSize;
        m_pData = new unsigned char[uiSize];
        m_uiRegisterIndex = uiRegisterIndex;
        m_uiRegisterNum = uiRegisterNum;
        MWDMAC_ASSERT(m_pData);

        if (pData)
        {
            MWDMemcpy(m_pData, pData, uiSize);
        }
        else
        {
            MWDMemset(m_pData, 0, uiSize);
        }
        m_nameInShader = NameInShader;
    };

    //shader内外名字一样
    MWDShaderConstant::MWDShaderConstant(const MWDName& Name, const void* pData, unsigned int uiSize,
        unsigned int uiRegisterIndex, unsigned int uiRegisterNum, unsigned int uiValueType) {
        MWDMAC_ASSERT(uiValueType < VT_MAX&& uiSize > 0 && uiRegisterNum > 0);

        m_showName = Name;
        m_uiValueType = uiValueType;
        m_uiSize = uiSize;
        m_pData = new unsigned char[uiSize];
        m_uiRegisterIndex = uiRegisterIndex;
        m_uiRegisterNum = uiRegisterNum;
        MWDMAC_ASSERT(m_pData);

        if (pData)
        {
            MWDMemcpy(m_pData, pData, uiSize);
        }
        else
        {
            MWDMemset(m_pData, 0, uiSize);
        }
        m_nameInShader = Name;
    };

    MWDShaderConstant::~MWDShaderConstant() {
        MWDMAC_DELETEA(m_pData);
    }


}