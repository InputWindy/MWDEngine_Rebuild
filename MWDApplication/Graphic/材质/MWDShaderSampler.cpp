#include "pch.h"
#include "MWDShaderSampler.h"
namespace MWDEngine {
	INITIAL_WITH_INIT_TERMINAL_BEGIN(MWDShaderSampler2D)
    ADD_PRIORITY(MWDObject)
    ADD_INITIAL_FUNCTION(InitialClassFactory)
    INITIAL_WITH_INIT_TERMINAL_END(MWDShaderSampler2D)

    BEGIN_ADD_PROPERTY(MWDShaderSampler2D)
    REGISTER_PROPERTY(m_uiTexType,TexType,MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(m_Name,Name, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(m_uiRegisterIndex,RegisterIndex, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(m_uiRegisterNum,RegisterNum, MWDProperty::F_SAVE_LOAD_CLONE)
    END_ADD_PROPERTY

    MWDShaderSampler2D::MWDShaderSampler2D(const MWDName& Name, unsigned int uiTexType, unsigned int uiRegisterIndex, unsigned int uiRegisterNum) {
        m_Name = Name;
        m_uiTexType = uiTexType;
        m_uiRegisterIndex = uiRegisterIndex;
        m_uiRegisterNum = uiRegisterNum;
        m_pTextureArray.ResetBufferNum(uiRegisterNum);
    };

    MWDShaderSampler2D::~MWDShaderSampler2D() {
        m_pTextureArray.Clear();
    }
    MWDShaderSampler2D::MWDShaderSampler2D()
    {
        m_uiTexType = MWDTexture::TT_2D;
        m_pTextureArray.Clear();
        m_uiRegisterNum = 1;
    }
    ;
}