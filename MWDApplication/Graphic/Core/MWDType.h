#pragma once
#include "MWDPointer.h"
#include <MWDString.h>
namespace MWDEngine {
	//自定义类型静态判断
	template<typename T> struct TIsCustomType { enum { Value = false }; };
	template<typename T> struct TIsCustomPointerType { enum { Value = false }; };

	//添加静态类型判断（在类外使用）
#define CUSTOMTYPE_MARCO(ClassName) \
	template<>	struct TIsCustomType<ClassName>	{ enum { Value = true }; }; \
	template<>	struct TIsCustomPointerType<ClassName *>	{ enum { Value = true }; };

	// MWD type
	template<typename T> struct TIsMWDType { enum { Value = false }; };

	//native pointer
	template<typename T> struct TIsNativePointerType { enum { Value = false }; };
	template<typename T> struct TIsNativePointerType<T*> { enum { Value = true }; };
	template<typename T> struct TIsNativePointerType<const T*> { enum { Value = true }; };
	template<typename T> struct TIsNativePointerType<const T* const> { enum { Value = true }; };
	template<typename T> struct TIsNativePointerType<T* volatile> { enum { Value = true }; };
	template<typename T> struct TIsNativePointerType<T* const volatile> { enum { Value = true }; };


	//MWD pointer
	template<typename T> struct TIsMWDPointerType { enum { Value = false }; };
	template<typename T> struct TIsMWDSmartPointerType { enum { Value = false }; };

	//====================================================================
	//添加静态类型判断（在类外使用）
	//TIsMWDType<ClassName>::Value = true;
	//TIsMWDPointerType<ClassName *>::Value = true;
	//TIsMWDSmartPointerType<MWDPointer<ClassName>>	::Value = true;
	//TIsMWDSmartPointerType<const MWDPointer<ClassName>>::Value = true;
	//====================================================================
#define MWDTYPE_MARCO(ClassName) \
	template<>	struct TIsMWDType<ClassName>										{ enum { Value = true }; }; \
	template<>	struct TIsMWDPointerType<ClassName *>							{ enum { Value = true }; }; \
	template<> struct TIsMWDSmartPointerType<MWDPointer<ClassName>>				{ enum { Value = true }; }; \
	template<> struct TIsMWDSmartPointerType<const MWDPointer<ClassName>>			{ enum { Value = true }; };

	template<typename T> struct TIsMWDEnumType { enum { Value = false }; };

	//添加静态类型判断（在类外使用）
	//TIsMWDEnumType<ClassName>::Value = true;
#define MWDENUMTYPE_MARCO(ClassName) \
	template<>	struct TIsMWDEnumType<ClassName>									{ enum { Value = true }; }; 

	//resource
	class MWDFSM;
	class MWDAnimSet;
	class MWDMaterial;
	class MWDMWDhader;
	class MWDPShader;
	class MWDTexAllState;
	class MWDSkelectonMeshNode;
	class MWDStaticMeshNode;
	class MWDResource;
	template<class T>
	class MWDResourceProxy;
	template<typename T> struct TIsMWDResourceType { enum { Value = false }; };
	template<typename T> struct TIsMWDResourcePointerType { enum { Value = false }; };
	template<typename T> struct TIsMWDResourceProxyType { enum { Value = false }; };
	template<typename T> struct TIsMWDResourceProxyPointType { enum { Value = false }; };

	template<typename T> struct TIsMWDResourceProxyType<MWDResourceProxy<T>> { enum { Value = true }; };
	template<typename T> struct TIsMWDResourceProxyPointType<MWDPointer<MWDResourceProxy<T>>> { enum { Value = true }; };

	// 	template<>	struct TIsMWDResourceType<MWDResource *>							{ enum { Value = true }; };
	// 	template<>	struct TIsMWDResourcePointerType<MWDPointer<MWDResource>>					{ enum { Value = true }; };
	// 	template<>	struct TIsMWDResourceType<MWDFSM *>								{ enum { Value = true }; };
	// 	template<>	struct TIsMWDResourcePointerType<MWDPointer<MWDFSM>>						{ enum { Value = true }; };
	// 	template<>	struct TIsMWDResourceType<MWDMaterial *>							{ enum { Value = true }; };
	// 	template<>	struct TIsMWDResourcePointerType<MWDPointer<MWDMaterial>>					{ enum { Value = true }; };
	// 	template<>	struct TIsMWDResourceType<MWDAnimSet *>							{ enum { Value = true }; };
	// 	template<>	struct TIsMWDResourcePointerType<MWDPointer<MWDAnimSet>>					{ enum { Value = true }; };
	// 	template<>	struct TIsMWDResourceType<MWDMWDhader *>							{ enum { Value = true }; };
	// 	template<>	struct TIsMWDResourcePointerType<MWDPointer<MWDMWDhader>>					{ enum { Value = true }; };
	// 	template<>	struct TIsMWDResourceType<MWDPShader *>							{ enum { Value = true }; };
	// 	template<>	struct TIsMWDResourcePointerType<MWDPointer<MWDPShader>>					{ enum { Value = true }; };
	// 	template<>	struct TIsMWDResourceType<MWDTexAllState *>						{ enum { Value = true }; };
	// 	template<>	struct TIsMWDResourcePointerType<MWDPointer<MWDTexAllState>>				{ enum { Value = true }; };
	// 	template<>	struct TIsMWDResourceType<MWDSkelectonMeshNode *>					{ enum { Value = true }; };
	// 	template<>	struct TIsMWDResourcePointerType<MWDPointer<MWDSkelectonMeshNode>>		{ enum { Value = true }; };
	// 	template<>	struct TIsMWDResourceType<MWDStaticMeshNode *>					{ enum { Value = true }; };
	// 	template<>	struct TIsMWDResourcePointerType<MWDPointer<MWDStaticMeshNode>>			{ enum { Value = true }; };

		// string
	template<typename T> struct TIsMWDStringType { enum { Value = false }; };
	template<>	struct TIsMWDStringType<MWDString> { enum { Value = true }; };

}