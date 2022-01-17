#pragma once
#include "MWDGraphic.h"
#include <MWDMemManager.h>
#include <MWDSystem.h>
#include <MWDArray.h>
#include <MWDMap.h>
#include <MWDString.h>
#include <MWDMath.h>
#include "MWDType.h"
#define ConsoleLogPropertyName(classname)\
cout << endl;\
for (int i = 0; i < classname::ms_Type->GetPropertyNum(); i++) {\
	for (int j = 0; j < (*classname::ms_Type->GetProperty(i)).GetName().GetLength(); j++) {\
		cout << (char)(*classname::ms_Type->GetProperty(i)).GetName().GetBuffer()[j];\
	}\
	cout << endl;\
}

#define ReadPropertyData(classname,property_name,obj_instance_address) \
MWDProperty::readPropertyData<classname>(property_name,obj_instance_address);

namespace MWDEngine {
	class MWDObject;
	class MWDRtti;
	class MWDFunction;
	//MWDProperty维护：属于哪个Rtti（MWDProperty是类内属性），属于哪个Function（MWDProperty还是Function的传参类型），属性flag，属性name,当前属性offset
	class  MWDProperty
	{
	public:
		enum	//Property Type
		{
			PT_VALUE,
			PT_ENUM,
			PT_DATA,
			PT_ARRAY,
			PT_MAP,
			PT_MAX
		};
		enum	// Flag 
		{
			F_NONE = 0X00,
			F_SAVE_LOAD = 0X01,
			F_CLONE = 0X02,
			F_COPY = 0X04,
			F_SAVE_LOAD_CLONE = 0X03,
			F_SAVE_LOAD_COPY = 0X05,
			F_REFLECT_NAME = 0X08,
			F_NO_USE_GC = 0X10,
			F_MAX
		};
		MWDProperty()
		{
			m_pRttiOwner = NULL;
			m_pFuntionOwner = NULL;
			m_uiFlag = 0;
			m_uiElementOffset = 0;
		}

		//生成类属性
		MWDProperty(MWDRtti& Owner, const MWDString& Name, unsigned int uiElementOffset, unsigned int uiFlag)
			:m_pRttiOwner(&Owner)
		{
			m_Name = Name;
			m_uiElementOffset = uiElementOffset;
			m_uiFlag = uiFlag;
			m_pFuntionOwner = NULL;
		}

		//生成函数参数
		MWDProperty(MWDFunction& Owner, const MWDString& Name, unsigned int uiElementOffset, unsigned int uiFlag)
			:m_pFuntionOwner(&Owner)
		{
			m_Name = Name;
			m_uiElementOffset = uiElementOffset;
			m_uiFlag = uiFlag;
			m_pRttiOwner = NULL;
		}
		virtual ~MWDProperty()
		{

		}
		virtual unsigned int GetPropertyType()const = 0;
		virtual MWDProperty* GetInstance() = 0;
		virtual void* GetValue(void* pObj)const
		{
			return (void*)(((const char*)pObj) + m_uiElementOffset);
		}
		virtual void* GetValueAddress(void* pObj)const
		{
			return (void*)(((unsigned char*)pObj) + m_uiElementOffset);
		}
		//浅拷贝
		virtual bool Clone(MWDProperty* p)
		{
			if ((p->GetRtti() == MWDProperty::GetRtti() && MWDProperty::GetRtti()) || (p->GetFunction() == MWDProperty::GetFunction() && MWDProperty::GetFunction()))
			{
				m_Name = p->m_Name;
				m_uiFlag = p->m_uiFlag;
				m_uiElementOffset = p->m_uiElementOffset;
				return true;
			}
			return false;
		}
		FORCEINLINE void SetOwner(MWDRtti& Owner)
		{
			m_pRttiOwner = &Owner;
		}
		FORCEINLINE void SetOwner(MWDFunction& Function)
		{
			m_pFuntionOwner = &Function;
		}
		FORCEINLINE unsigned int GetFlag()const
		{
			return m_uiFlag;
		}
		FORCEINLINE void SetFlag(unsigned int uiFlag)
		{
			m_uiFlag = uiFlag;
		}
		FORCEINLINE MWDRtti* GetRtti()const
		{
			return m_pRttiOwner;
		}
		FORCEINLINE MWDFunction* GetFunction()const
		{
			return m_pFuntionOwner;
		}
		FORCEINLINE const MWDString& GetName()const
		{
			return m_Name;
		}

		template<typename T>
		static void* readPropertyData(const TCHAR* property_name,void *pObj) {
			for (int i = 0; i < T::ms_Type->GetPropertyNum(); i++) {
				if((*T::ms_Type->GetProperty(i)).GetName()== property_name){
					return GetValue(pObj);
				}
			}
			return NULL;
		}
	protected:
		//所属Rtti
		MWDRtti* m_pRttiOwner;
		//所属Function
		MWDFunction* m_pFuntionOwner;
		//属性名字
		MWDString m_Name;
		unsigned int m_uiFlag;
		unsigned int m_uiElementOffset;
	};

	template<typename T>
	class MWDEnumProperty : public MWDProperty
	{
	public:
		MWDEnumProperty()
		{
			MWDMAC_ASSERT(sizeof(T) == 4);
		}
		MWDEnumProperty(MWDRtti& Owner, const MWDString& Name, const MWDString& EnumName, unsigned int uiElementOffset, unsigned int uiFlag)
			:MWDProperty(Owner, Name, uiElementOffset, uiFlag)
		{
			MWDMAC_ASSERT(sizeof(T) == 4);
			m_EnumName = EnumName;
		}
		MWDEnumProperty(MWDFunction& Owner, const MWDString& Name, const MWDString& EnumName, unsigned int uiElementOffset, unsigned int uiFlag)
			:MWDProperty(Owner, Name, uiElementOffset, uiFlag)
		{
			MWDMAC_ASSERT(sizeof(T) == 4);
			m_EnumName = EnumName;
		}
		virtual ~MWDEnumProperty()
		{

		}
		//浅拷贝
		virtual bool Clone(MWDProperty* p)
		{
			MWDEnumProperty* Temp = (MWDEnumProperty*)p;
			if (!MWDProperty::Clone(Temp))
				return false;
			m_EnumName = Temp->m_EnumName;
			return true;

		}
		virtual bool SetValue(void* pObj, unsigned int& pDataSrc) const
		{

			*(unsigned int*)(((unsigned char*)pObj) + m_uiElementOffset) = pDataSrc;
			return true;

		}
		virtual bool GetValue(void* pObj, unsigned int& pDataDest) const
		{
			pDataDest = *(unsigned int*)(((unsigned char*)pObj) + m_uiElementOffset);
			return true;
		}
		virtual bool GetValue(const void* pObj, unsigned int& pDataDest) const
		{
			pDataDest = *(const unsigned int*)(((const char*)pObj) + m_uiElementOffset);
			return true;
		}
		//这里传进去的是一个obj的地址：先通过rtti找到当前类里的该属性，然后传入一个实例，读取实例里该属性的值
		virtual unsigned int& Value(void* pObj)const
		{
			return *(unsigned int*)(((const char*)pObj) + m_uiElementOffset);
		}
		virtual void CloneData(void* pSrcObj, void* pDestObj, MWDMap<MWDObject*, MWDObject*>& CloneMap)
		{
			Value(pDestObj) = Value(pSrcObj);
		}
		virtual void CopyData(void* pSrcObj, void* pDestObj)
		{
			Value(pDestObj) = Value(pSrcObj);
		}
		/*virtual void AddEditorElement(void* pSrcObj, MWDECollection* pParent, MWDString& Name)
		{
			MWDEEnumProperty* pEp = new MWDEEnumProperty((unsigned int*)(((const char*)pSrcObj) + m_uiElementOffset), Name, (MWDObject*)pSrcObj);
			pParent->AddElement(pEp);
			MWDArray<MWDString> AS;
			MWDString EnumName = m_EnumName.GetString();
			GET_ENUMARRAY(EnumName, AS);
			pEp->AddEnumString(AS);
		}*/
		virtual MWDProperty* GetInstance()
		{
			return new MWDEnumProperty<T>();
		}
		MWDString& GetEnumName()
		{
			return m_EnumName;
		}
		virtual unsigned int GetPropertyType()const
		{
			return PT_ENUM;
		}
	protected:
		MWDString m_EnumName;
	};

	template<typename T, typename NumType>
	class MWDDataProperty : public MWDProperty
	{
	public:
		MWDDataProperty(MWDRtti& Owner, const MWDString& Name, unsigned int uiElementOffset, unsigned int uiDataNum, bool bDynamicCreate)
			:MWDProperty(Owner, Name, uiElementOffset, F_SAVE_LOAD_CLONE | F_NO_USE_GC)
		{
			m_uiDataNum = uiDataNum;
			m_bDynamicCreate = bDynamicCreate;
		}
		MWDDataProperty(MWDFunction& Owner, const MWDString& Name, unsigned int uiElementOffset, unsigned int uiDataNum, bool bDynamicCreate)
			:MWDProperty(Owner, Name, uiElementOffset, F_SAVE_LOAD_CLONE | F_NO_USE_GC)
		{
			m_uiDataNum = uiDataNum;
			m_bDynamicCreate = bDynamicCreate;
		}
		MWDDataProperty()
		{

		}
		MWDDataProperty(MWDRtti& Owner, const MWDString& Name, unsigned int uiElementOffset, unsigned int uiNumElementOffset)
			:MWDProperty(Owner, Name, uiElementOffset, F_SAVE_LOAD_CLONE | F_NO_USE_GC)
		{
			m_uiDataNum = 0;
			m_bDynamicCreate = true;
			m_uiNumElementOffset = uiNumElementOffset;
		}
		virtual unsigned int GetPropertyType()const
		{
			return PT_DATA;
		}
		/*virtual void AddEditorElement(void* pSrcObj, MWDECollection* pParent, MWDString& Name)
		{
			return;
		}*/
		//未实现api
		virtual void CopyData(void* pSrcObj, void* pDestObj)
		{
			MWDMAC_ASSERT(0);
		}
		virtual void CloneData(void* pSrcObj, void* pDestObj, MWDMap<MWDObject*, MWDObject*>& CloneMap)
		{
			T* SrcValueAddres = *(T**)GetValue(pSrcObj);

			if (m_uiDataNum > 0)
			{
				T** Temp = (T**)GetValue(pDestObj);
				if (m_bDynamicCreate)
				{
					*Temp = new T[m_uiDataNum];
					MWDMemcpy((void*)(*Temp), (void*)SrcValueAddres, m_uiDataNum * sizeof(T));
				}
				else
				{
					MWDMemcpy((void*)(*Temp), (void*)SrcValueAddres, m_uiDataNum * sizeof(T));
				}
			}
			else
			{
				T** Temp = (T**)GetValue(pDestObj);


				void* SrcNumOffSet = (void*)(((unsigned char*)pSrcObj) + m_uiNumElementOffset);
				void* DestNumOffSet = (void*)(((unsigned char*)pDestObj) + m_uiNumElementOffset);
				*(NumType*)DestNumOffSet = *(NumType*)SrcNumOffSet;
				NumType uiNum = *(NumType*)SrcNumOffSet;

				*Temp = new T[uiNum];
				MWDMemcpy((void*)(*Temp), (void*)SrcValueAddres, uiNum * sizeof(T));
			}
		}
		virtual bool Archive(MWDStream* Stream, void* pObj) { return true; };
		virtual MWDProperty* GetInstance()
		{
			return new MWDDataProperty<T, NumType>();
		}
		virtual bool Clone(MWDProperty* p)
		{
			MWDDataProperty<T, NumType>* Temp = (MWDDataProperty<T, NumType> *)p;
			if (!MWDProperty::Clone(Temp))
				return false;
			m_bDynamicCreate = Temp->m_bDynamicCreate;
			m_uiDataNum = Temp->m_uiDataNum;
			m_uiNumElementOffset = Temp->m_uiNumElementOffset;
			return true;
		}
	protected:
		bool m_bDynamicCreate;
		unsigned int m_uiDataNum;
		unsigned int m_uiNumElementOffset;
	};

	//所有有范围的数值属性的基类
	template<typename T>
	class  MWDValueBaseProperty : public MWDProperty
	{
	public:
		MWDValueBaseProperty()
		{

		}
		MWDValueBaseProperty(MWDRtti& Owner, const MWDString& Name, unsigned int uiElementOffset, unsigned int uiFlag, bool Range = false, T HighValue = T(), T LowValue = T(), T fStep = T())
			:MWDProperty(Owner, Name, uiElementOffset, uiFlag)
		{
			m_LowValue = LowValue;
			m_HightValue = HighValue;
			m_fStep = fStep;
			m_bRange = Range;
		}
		MWDValueBaseProperty(MWDFunction& Owner, const MWDString& Name, unsigned int uiElementOffset, unsigned int uiFlag, bool Range = false, T HighValue = T(), T LowValue = T(), T fStep = T())
			:MWDProperty(Owner, Name, uiElementOffset, uiFlag)
		{
			m_LowValue = LowValue;
			m_HightValue = HighValue;
			m_fStep = fStep;
			m_bRange = Range;
		}
		virtual ~MWDValueBaseProperty()
		{

		}
		virtual bool Clone(MWDProperty* p)
		{
			MWDValueBaseProperty<T>* Temp = (MWDValueBaseProperty<T> *)p;
			if (!MWDProperty::Clone(Temp))
				return false;
			m_LowValue = Temp->m_LowValue;
			m_HightValue = Temp->m_HightValue;
			return true;
		}
	protected:
		T m_LowValue;
		T m_HightValue;
		T m_fStep;
		bool m_bRange;
	};

	template<typename T>
	class  MWDValueProperty : public MWDValueBaseProperty<T>
	{
	public:
		MWDValueProperty()
		{

		}
		MWDValueProperty(MWDRtti& Owner, const MWDString& Name, unsigned int uiElementOffset, unsigned int uiFlag, bool Range = false, T HighValue = T(), T LowValue = T(), T fStep = T())
			:MWDValueBaseProperty<T>(Owner, Name, uiElementOffset, uiFlag, Range, HighValue, LowValue, fStep)
		{

		}
		MWDValueProperty(MWDFunction& Owner, const MWDString& Name, unsigned int uiElementOffset, unsigned int uiFlag, bool Range = false, T HighValue = T(), T LowValue = T(), T fStep = T())
			:MWDValueBaseProperty<T>(Owner, Name, uiElementOffset, uiFlag, Range, HighValue, LowValue, fStep)
		{

		}
		virtual ~MWDValueProperty()
		{

		}
		virtual unsigned int GetPropertyType()const
		{
			return MWDProperty::PT_VALUE;
		}
		//pObj是属性地址，pDataSrc是属性值
		virtual bool SetValue(void* pObj, T& pDataSrc) const
		{
			//如果输入值超出范围就false
			if (pDataSrc > MWDValueBaseProperty<T>::m_HightValue || pDataSrc < MWDValueBaseProperty<T>::m_LowValue)
			{
				return false;
			}
			//给这段空间赋值
			*(T*)(((unsigned char*)pObj) + MWDProperty::m_uiElementOffset) = pDataSrc;
			return true;
		}
		//pObj是属性地址，pDataSrc是属性值
		virtual void* GetValue(void* pObj, T& pDataDest) const
		{
			pDataDest = *(T*)(((unsigned char*)pObj) + MWDProperty::m_uiElementOffset);
			return NULL;
		}
		//pObj是属性地址，pDataSrc是属性值
		virtual void* GetValue(const void* pObj, T& pDataDest) const
		{
			pDataDest = *(const T*)(((const char*)pObj) + MWDProperty::m_uiElementOffset);
			return NULL;
		}
		//传入属性地址，读取属性值
		virtual void* GetValue(void* pObj)const
		{
			return (void*)(((const char*)pObj) + MWDProperty::m_uiElementOffset);
		}
		//返回一个MWDValueProperty<T>()实例
		virtual MWDProperty* GetInstance()
		{
			return new MWDValueProperty<T>();
		}

	};

	template<typename ArrayType, typename T>
	class MWDArrayProperty : public MWDValueBaseProperty<T>
	{
	public:
		MWDArrayProperty()
		{

		}
		MWDArrayProperty(MWDRtti& Owner, const MWDString& Name, unsigned int uiElementOffset, unsigned int uiFlag, bool Range = false, T HighValue = T(), T LowValue = T(), T fStep = T())
			:MWDValueBaseProperty<T>(Owner, Name, uiElementOffset, uiFlag, Range, HighValue, LowValue, fStep)
		{

		}
		MWDArrayProperty(MWDFunction& Owner, const MWDString& Name, unsigned int uiElementOffset, unsigned int uiFlag, bool Range = false, T HighValue = T(), T LowValue = T(), T fStep = T())
			:MWDValueBaseProperty<T>(Owner, Name, uiElementOffset, uiFlag, Range, HighValue, LowValue, fStep)
		{

		}
		virtual ~MWDArrayProperty()
		{

		}
		virtual unsigned int GetPropertyType()const
		{
			return MWDValueBaseProperty<T>::PT_ARRAY;
		}
		FORCEINLINE ArrayType& GetContainer(void* pObj)const
		{
			return (*(ArrayType*)(((unsigned char*)pObj) + MWDValueBaseProperty<T>::m_uiElementOffset));
		}
		FORCEINLINE bool AddElement(void* pObj, unsigned int uiIndex, T& pDataSrc)
		{
			GetContainer(pObj).AddElement(pDataSrc);
			return true;
		}
		FORCEINLINE bool Erase(void* pObj, unsigned int i)
		{
			GetContainer(pObj).Erase(i);
		}
		virtual bool SetValue(void* pObj, unsigned int uiIndex, T& pDataSrc) const
		{

			if (pDataSrc > MWDValueBaseProperty<T>::m_HightValue || pDataSrc < MWDValueBaseProperty<T>::m_LowValue)
			{
				return false;
			}
			(GetContainer(pObj)[uiIndex]) = pDataSrc;
			return true;

		}
		virtual void* GetValue(void* pObj, unsigned int uiIndex, T& pDataDest) const
		{
			pDataDest = (GetContainer(pObj)[uiIndex]);
			return NULL;
		}
		/*virtual bool Archive(MWDStream& Stream, void* pObj)
		{
			Stream.Archive(GetContainer(pObj));
			return true;
		}*/
		virtual void CloneData(void* pSrcObj, void* pDestObj, MWDMap<MWDObject*, MWDObject*>& CloneMap)
		{
			MWDValueBaseProperty<T>::Copy(GetContainer(pDestObj), GetContainer(pSrcObj), CloneMap);
		}
		virtual void CopyData(void* pSrcObj, void* pDestObj)
		{
			GetContainer(pDestObj) = GetContainer(pSrcObj);
		}
		/*virtual void AddEditorElement(void* pSrcObj, MWDECollection* pParent, MWDString& Name)
		{
			CreateEditorElement(GetContainer(pSrcObj), (MWDObject*)pSrcObj, pParent, Name, m_bRange, m_HightValue, m_LowValue, m_fStep);
		}*/
		virtual MWDProperty* GetInstance()
		{
			return new MWDArrayProperty<ArrayType, T>();
		}
	};

	template<typename MapType, typename KEY, typename VALUE>
	class MWDMapProperty : public MWDValueBaseProperty<VALUE>
	{
	public:
		MWDMapProperty(MWDRtti& Owner, const MWDString& Name, unsigned int uiElementOffset, unsigned int uiFlag, bool Range = false, VALUE HighValue = VALUE(), VALUE LowValue = VALUE(), VALUE fStep = VALUE())
			:MWDValueBaseProperty<VALUE>(Owner, Name, uiElementOffset, uiFlag, Range, HighValue, LowValue, fStep)
		{

		}
		MWDMapProperty(MWDFunction& Owner, const MWDString& Name, unsigned int uiElementOffset, unsigned int uiFlag, bool Range = false, VALUE HighValue = VALUE(), VALUE LowValue = VALUE(), VALUE fStep = VALUE())
			:MWDValueBaseProperty<VALUE>(Owner, Name, uiElementOffset, uiFlag, Range, HighValue, LowValue, fStep)
		{

		}
		MWDMapProperty()
		{

		}
		virtual ~MWDMapProperty()
		{

		}
		virtual unsigned int GetPropertyType()const
		{
			return MWDValueBaseProperty<VALUE>::PT_MAP;
		}
		FORCEINLINE MapType& GetContaine(void* pObj)const
		{
			return (*(MapType*)(((unsigned char*)pObj) + MWDValueBaseProperty<VALUE>::m_uiElementOffset));
		}
		FORCEINLINE bool AddElement(void* pObj, unsigned int uiIndex, MWDMapElement<KEY, VALUE>& pDataSrc)
		{
			GetContaine(pObj).AddElement(pDataSrc);
			return true;
		}
		FORCEINLINE bool Erase(void* pObj, unsigned int i)
		{
			GetContaine(pObj).Erase(i);
		}
		virtual bool SetValue(void* pObj, unsigned int uiIndex, KEY& Key, VALUE& Value) const
		{

			if (Value > MWDValueBaseProperty<VALUE>::m_HightValue || Value < MWDValueBaseProperty<VALUE>::m_LowValue)
			{
				return false;
			}

			MWDMapElement<KEY, VALUE> Temp(Key, Value);
			(GetContaine(pObj)[uiIndex]) = Temp;
			return true;

		}
		virtual void* GetValue(void* pObj, unsigned int uiIndex, MWDMapElement<KEY, VALUE>& pDataDest) const
		{
			pDataDest = (GetContaine(pObj)[uiIndex]);
			return NULL;
		}
		virtual void CloneData(void* pSrcObj, void* pDestObj, MWDMap<MWDObject*, MWDObject*>& CloneMap)
		{
			Copy(GetContaine(pDestObj), GetContaine(pSrcObj), CloneMap);
		}
		virtual void CopyData(void* pSrcObj, void* pDestObj)
		{
			GetContaine(pDestObj) = GetContaine(pSrcObj);
		}
		/*virtual void AddEditorElement(void* pSrcObj, MWDECollection* pParent, MWDString& Name)
		{
			CreateEditorElement(GetContaine(pSrcObj), (MWDObject*)pSrcObj, pParent, Name, m_bRange, m_HightValue, m_LowValue, m_fStep);
		}*/
		virtual MWDProperty* GetInstance()
		{
			return new MWDMapProperty<MapType, KEY, VALUE>();
		}
	};

	//===============creator======================//
	template<class T, class NumType>
	struct DataPropertyCreator
	{
		MWDProperty* CreateProperty(const MWDString& Name, MWDRtti& Owner, unsigned int Offset, unsigned int NumOffset)
		{
			return new MWDDataProperty<T, NumType>(Owner, Name, Offset, NumOffset);
		}
		MWDProperty* CreateFunctionProperty(const MWDString& Name, MWDFunction& Owner, unsigned int Offset, unsigned int NumOffset)
		{
			return new MWDDataProperty<T, NumType>(Owner, Name, Offset, NumOffset);
		}
		MWDProperty* CreateProperty(const MWDString& Name, MWDRtti& Owner, unsigned int Offset, unsigned int uiDataNum, bool bDynamicCreate)
		{
			return new MWDDataProperty<T, NumType>(Owner, Name, Offset, uiDataNum, bDynamicCreate);
		}
	};

	template<class T>
	struct AutoPropertyCreator
	{
		//创建类属性(单值属性)
		MWDProperty* CreateProperty(const MWDString& Name, MWDRtti& Owner, unsigned int Offset, unsigned int uiFlag)
		{
			if (!(TIsMWDPointerType<T>::Value || TIsMWDSmartPointerType<T>::Value || TIsCustomType<T>::Value || TIsMWDType<T>::Value))
			{
				uiFlag |= MWDProperty::F_NO_USE_GC;
			}
			return new MWDValueProperty<T>(Owner, Name, Offset, uiFlag);
		}
		//创建类属性(范围属性)
		MWDProperty* CreateProperty(const MWDString& Name, MWDRtti& Owner, unsigned int Offset, T HighValue, T LowValue, MWDREAL fStep, unsigned int uiFlag)
		{
			if (!(TIsMWDPointerType<T>::Value || TIsMWDSmartPointerType<T>::Value || TIsCustomType<T>::Value || TIsMWDType<T>::Value))
			{
				uiFlag |= MWDProperty::F_NO_USE_GC;
			}
			return new MWDValueProperty<T>(Owner, Name, Offset, uiFlag, true, HighValue, LowValue, fStep);
		}
		//创建函数参数(单值参数)
		MWDProperty* CreateFunctionProperty(const MWDString& Name, MWDFunction& Owner, unsigned int Offset, unsigned int uiFlag)
		{
			return new MWDValueProperty<T>(Owner, Name, Offset, uiFlag);
		}
	};

	//属性生成器
	class  MWDPropertyCreator
	{
	public:
		template<class ValueType>
		static AutoPropertyCreator<ValueType>& GetAutoPropertyCreator(ValueType& valueTypeDummyRef)
		{
			static AutoPropertyCreator<ValueType> apc;
			return apc;
		}

		template<class ValueType, class NumType>
		static DataPropertyCreator<ValueType, NumType>& GetAutoPropertyCreator(ValueType*& valueTypeDummyRef, NumType& valueNumTypeDummyRef)
		{
			static DataPropertyCreator<ValueType, NumType> apc;
			return apc;
		}

		template<class ValueType>
		static MWDProperty* CreateEnumProperty(ValueType& valueTypeDummyRef, const MWDString& Name, const MWDString& EnumName, MWDRtti& Owner, unsigned int Offset, unsigned int uiFlag)
		{
			return new MWDEnumProperty<ValueType>(Owner, Name, EnumName, Offset, uiFlag | MWDProperty::F_NO_USE_GC);
		}
	};
	//模板函数：传入Obj，函数，参数列表，返回值

	typedef void(*FunctionTemplatePtr)(MWDObject* p, MWDFunction* pFun, void* para, void* ret);
	//MWDFunction维护：所属Rtti，函数名字，flag，返回类型，传参表，MWDFunction实例的内存占用大小
	class  MWDFunction
	{
	public:
		enum	// Flag 
		{
			F_DEFAULT = 0X00,

			//在客户端使用的函数
			F_CLIENT = 0X01,

			//在服务器端使用的函数
			F_SERVER = 0X02,

			//UI反射的函数
			F_REFLECT_UI = 0X04,
			F_MAX
		};
		MWDFunction()
		{
			m_pOwner = NULL;
			m_Name = NULL;
			m_pReturnProperty = NULL;
			ObjectFun = NULL;
			m_uiFlag = 0;
			m_uiParaSize = 0;
		}
		//清空参数列表和返回类型
		~MWDFunction()
		{
			for (unsigned int i = 0; i < m_PropertyArray.GetNum(); i++)
			{
				if (m_PropertyArray[i])
				{
					MWDMAC_DELETE(m_PropertyArray[i])
				}
			}
			MWDMAC_DELETE(m_pReturnProperty);
		}
		MWDFunction(MWDRtti& Owner, const MWDString& Name, unsigned int uiFlag)
			:m_pOwner(&Owner)
		{
			m_Name = Name;
			m_uiFlag = uiFlag;
			m_pReturnProperty = NULL;
			ObjectFun = NULL;
		}
		MWDRtti* GetRtti()const
		{
			return m_pOwner;
		}
		const MWDString& GetName()const
		{
			return m_Name;
		}
		
		//把另外一个Function的参数参数拷贝进来
		virtual bool Clone(MWDFunction* f)
		{
			if ((f->GetRtti() == GetRtti()) && GetRtti())
			{
				m_Name = f->m_Name;
				m_uiFlag = f->m_uiFlag;
				for (unsigned int i = 0; i < f->m_PropertyArray.GetNum(); i++)
				{
					MWDProperty* p = f->m_PropertyArray[i]->GetInstance();
					m_PropertyArray.AddElement(p);
					p->Clone(f->m_PropertyArray[i]);
				}
				return true;
			}
			return false;

		}
		virtual MWDFunction* GetInstance()
		{
			return new MWDFunction();
		}
		void SetOwner(MWDRtti& Owner)
		{
			m_pOwner = &Owner;
		}
		unsigned int GetFlag()const
		{
			return m_uiFlag;
		}
		void AddProperty(MWDProperty* P)
		{
			m_PropertyArray.AddElement(P);
		}
		MWDProperty* GetProperty(unsigned int i)
		{
			return m_PropertyArray[i];
		}
		unsigned int GetPropertyNum()const
		{
			return m_PropertyArray.GetNum();
		}
		void SetReturnType(MWDProperty* p)
		{
			m_pReturnProperty = p;
		}
		bool IsReturnVoid()
		{
			return !m_pReturnProperty;
		}

		FunctionTemplatePtr ObjectFun;

		void SetParaSize(unsigned int uiParaSize)
		{
			m_uiParaSize = uiParaSize;
		}
		//比较两个函数的信息
		bool IsSame(MWDFunction* p)
		{
			if (m_pOwner != p->m_pOwner)
			{
				return false;
			}
			else if (m_Name != p->m_Name)
			{
				return false;
			}
			else if (m_uiParaSize != p->m_uiParaSize)
			{
				return false;
			}
			else if (m_PropertyArray.GetNum() != p->m_PropertyArray.GetNum())
			{
				return false;
			}
			else
			{
				return true;
			}
		}
	protected:
		MWDRtti* m_pOwner;
		MWDString m_Name;
		unsigned int m_uiFlag;
		//函数参数
		MWDArray<MWDProperty*> m_PropertyArray;
		//函数返回值
		MWDProperty* m_pReturnProperty;
		unsigned int m_uiParaSize;
	};

}

