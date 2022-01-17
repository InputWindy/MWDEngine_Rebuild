#pragma once
#include "MWDGraphic.h"
#include "MWDObject.h"
#include "MWDPointer.h"
#include "MWDType.h"
#include <MWDMap.h>
#include <MWDFile.h>
#include "MWDRtti.h"
#include <MWDArray.h>
#include <MWDArray.h>
#include <MWDMap.h>
#include <MWDString.h>
#include "MWDCustomArchiveObject.h"
#include "MWDResource.h"
#include "MWDResourceManager.h"
namespace MWDEngine {
	DECLARE_Ptr(MWDObject);

	template<typename T>
	void Copy(T& Dest, T& Src, MWDMap<MWDObject*, MWDObject*>& CloneMap)
	{
		if (TIsMWDResourceType<T>::Value)
		{
			Dest = Src;
		}
		else if (TIsMWDPointerType<T>::Value)
		{
			MWDObject*& TempSrc = *(MWDObject**)(void*)&Src;
			MWDObject*& TempDest = *(MWDObject**)(void*)&Dest;
			TempDest = MWDObject::_CloneCreateObject(TempSrc, CloneMap);
		}
		else if (TIsMWDType<T>::Value)
		{
			MWDObject* TempSrc = (MWDObject*)&Src;
			MWDObject* TempDest = (MWDObject*)&Dest;
			MWDObject::_CloneObject(TempSrc, TempDest, CloneMap);
		}
		else if (TIsMWDSmartPointerType<T>::Value)
		{
			MWDObjectPtr& TempSrc = *(MWDObjectPtr*)(void*)&Src;
			MWDObjectPtr& TempDest = *(MWDObjectPtr*)(void*)&Dest;
			TempDest = MWDObject::_CloneCreateObject(TempSrc, CloneMap);
		}
		else if (TIsCustomType<T>::Value)
		{
			MWDCustomArchiveObject* TempSrc = (MWDCustomArchiveObject*)(void*)&Src;
			MWDCustomArchiveObject* TempDest = (MWDCustomArchiveObject*)(void*)&Dest;
			TempDest->CopyFrom(TempSrc, CloneMap);
		}
		else
		{
			Dest = Src;
		}
	}
	template<typename T, MWDMemManagerFun MMFun>
	void Copy(MWDArray<T, MMFun>& Dest, MWDArray<T, MMFun>& Src, MWDMap<MWDObject*, MWDObject*>& CloneMap)
	{
		Dest.Clear();
		Dest.SetBufferNum(Src.GetNum());
		for (unsigned int i = 0; i < Src.GetNum(); i++)
		{
			Copy(Dest[i], Src[i], CloneMap);
		}
	}
	template<class Key, class Value, MWDMemManagerFun MMFun>
	void Copy(MWDMap<Key, Value, MMFun>& Dest, MWDMap<Key, Value, MMFun>& Src, MWDMap<MWDObject*, MWDObject*>& CloneMap)
	{
		Dest.Clear();
		Dest.SetBufferNum(Src.GetNum());
		for (unsigned int i = 0; i < Src.GetNum(); i++)
		{
			MWDMapElement<Key, Value>& ElementSrc = Src[i];
			MWDMapElement<Key, Value>& ElementDest = Dest[i];
			Copy(ElementDest.Key, ElementSrc.Key, CloneMap);
			Copy(ElementDest.Value, ElementSrc.Value, CloneMap);
		}
	}

	class  MWDStream
	{
	public:
		enum // Archive Type
		{
			AT_SAVE,
			AT_LOAD,
			AT_LINK,
			AT_REGISTER,
			AT_SIZE,
			AT_POSTLOAD,
			AT_OBJECT_COLLECT_GC,
			AT_CLEAR_OBJECT_PROPERTY_GC,
			AT_LOAD_OBJECT_COLLECT_GC,
		};
		template<class T>
		void Archive(T& Io)
		{
			if (m_uiStreamFlag == AT_LOAD)
			{

				/*if (TIsMWDResourceProxyPointType<T>::Value)
				{
					MWDResourceProxyBasePtr& Temp = *(MWDResourceProxyBasePtr*)(void*)&Io;
					ReadResource(Temp);
				}*/
				if (TIsMWDPointerType<T>::Value)
				{
					MWDObject*& Temp = *(MWDObject**)(void*)&Io;
					ReadObjectPtr(Temp);
				}
				else if (TIsMWDType<T>::Value)
				{
					MWDObject* Key = (MWDObject*)&Io;
					MWDObject* Value = NULL;
					ReadObjectPtr(Value);
					m_pmMWDTypeLoadMap.AddElement(Key, Value);
				}
				else if (TIsMWDSmartPointerType<T>::Value)
				{
					MWDObjectPtr& Temp = *(MWDObjectPtr*)(void*)&Io;
					ReadObjectPtr(Temp);
				}
				else if (TIsMWDStringType<T>::Value)
				{
					MWDString& Temp = *(MWDString*)(void*)&Io;
					ReadString(Temp);
				}
				else if (TIsCustomType<T>::Value)
				{
					MWDCustomArchiveObject* Temp = (MWDCustomArchiveObject*)(void*)&Io;
					if (Temp)
					{
						Temp->Archive(*this);
					}

				}
				else
				{
					Read((void*)&Io, sizeof(T));
				}
			}
			else if (m_uiStreamFlag == AT_SAVE)
			{
				/*if (TIsMWDResourceProxyPointType<T>::Value)
				{
					MWDResourceProxyBasePtr& Temp = *(MWDResourceProxyBasePtr*)(void*)&Io;
					WriteResource(Temp);

				}*/
				if (TIsMWDPointerType<T>::Value)
				{
					MWDObject*& Temp = *(MWDObject**)(void*)&Io;
					WriteObjectPtr(Temp);
				}
				else if (TIsMWDSmartPointerType<T>::Value)
				{
					MWDObjectPtr& Temp = *(MWDObjectPtr*)(void*)&Io;
					WriteObjectPtr(Temp);
				}
				else if (TIsMWDStringType<T>::Value)
				{
					MWDString& Temp = *(MWDString*)(void*)&Io;
					WriteString(Temp);
				}
				else if (TIsCustomType<T>::Value)
				{
					MWDCustomArchiveObject* Temp = (MWDCustomArchiveObject*)(void*)&Io;
					if (Temp)
					{
						Temp->Archive(*this);
					}

				}
				else if (TIsMWDType<T>::Value)
				{
					MWDObject* Temp = (MWDObject*)&Io;
					WriteObjectPtr(Temp);
				}
				else
				{
					Write((void*)&Io, sizeof(T));
				}
			}
			else if (m_uiStreamFlag == AT_REGISTER || m_uiStreamFlag == AT_POSTLOAD)
			{
				if (TIsMWDPointerType<T>::Value)
				{
					MWDObject*& Temp = *(MWDObject**)(void*)&Io;
					ArchiveAll(Temp);
				}
				else if (TIsMWDSmartPointerType<T>::Value)
				{
					MWDObjectPtr& Temp = *(MWDObjectPtr*)(void*)&Io;
					ArchiveAll(Temp);
				}
				else if (TIsCustomType<T>::Value)
				{
					MWDCustomArchiveObject* Temp = (MWDCustomArchiveObject*)(void*)&Io;
					if (Temp)
					{
						Temp->Archive(*this);
					}
				}
				else if (TIsMWDType<T>::Value)
				{
					MWDObject* Temp = (MWDObject*)&Io;
					ArchiveAll(Temp);
				}
			}
			else if (m_uiStreamFlag == AT_SIZE)
			{
				/*if (TIsMWDResourceProxyPointType<T>::Value)
				{
					MWDResourceProxyBasePtr& Temp = *(MWDResourceProxyBasePtr*)(void*)&Io;
					m_uiArchivePropertySize += GetResourceDistUse(Temp) + sizeof(bool);
				}*/
				if (TIsMWDPointerType<T>::Value || TIsMWDSmartPointerType<T>::Value || TIsMWDType<T>::Value)
				{
					m_uiArchivePropertySize += 4;
				}
				else if (TIsMWDStringType<T>::Value)
				{
					MWDString& Temp = *(MWDString*)(void*)&Io;
					m_uiArchivePropertySize += GetStrDistUse(Temp);
				}
				else if (TIsCustomType<T>::Value)
				{
					MWDCustomArchiveObject* Temp = (MWDCustomArchiveObject*)(void*)&Io;
					if (Temp)
					{
						Temp->Archive(*this);
					}
				}

				else
				{
					m_uiArchivePropertySize += sizeof(T);
				}
			}
			else if (m_uiStreamFlag == AT_OBJECT_COLLECT_GC || m_uiStreamFlag == AT_LOAD_OBJECT_COLLECT_GC)
			{
				if (TIsMWDPointerType<T>::Value)
				{
					MWDObject*& Temp = *(MWDObject**)(void*)&Io;

					if (Temp)
					{
						if (Temp->IsHasFlag(MWDObject::OF_PendingKill))
						{
							Temp = NULL;
							return;
						}
						ArchiveAll(Temp);
					}

				}
				else if (TIsMWDSmartPointerType<T>::Value)
				{
					MWDObjectPtr& Temp = *(MWDObjectPtr*)(void*)&Io;
					if (Temp)
					{
						if (Temp->IsHasFlag(MWDObject::OF_PendingKill))
						{
							Temp = NULL;
							return;
						}
						ArchiveAll(Temp);
					}


				}
				else if (TIsCustomType<T>::Value)
				{
					MWDCustomArchiveObject* Temp = (MWDCustomArchiveObject*)(void*)&Io;
					if (Temp)
					{
						Temp->Archive(*this);
					}

				}
				else if (TIsMWDType<T>::Value)
				{
					MWDObject* Temp = (MWDObject*)&Io;
					ArchiveAll(Temp);
				}
			}
			else if (m_uiStreamFlag == AT_CLEAR_OBJECT_PROPERTY_GC)
			{
				if (TIsMWDPointerType<T>::Value)
				{
					MWDObject*& Temp = *(MWDObject**)(void*)&Io;
					Temp = NULL;
				}
				else if (TIsMWDSmartPointerType<T>::Value)
				{
					MWDObjectPtr& Temp = *(MWDObjectPtr*)(void*)&Io;
					MWDObject* LocalTemp = Temp;
					Temp = NULL;
					if (LocalTemp)
					{
						if (LocalTemp->IsHasFlag(MWDObject::OF_REACH))
						{
							LocalTemp->ClearFlag(MWDObject::OF_PendingKill);
						}
					}


				}
				else if (TIsCustomType<T>::Value)
				{
					MWDCustomArchiveObject* Temp = (MWDCustomArchiveObject*)(void*)&Io;
					if (Temp)
					{
						Temp->Archive(*this);
					}

				}
				else if (TIsMWDType<T>::Value)
				{
					MWDObject* Temp = (MWDObject*)&Io;
					ArchiveAll(Temp);
				}
			}
			else if (m_uiStreamFlag == AT_LINK)
			{
				if (TIsMWDPointerType<T>::Value)
				{
					MWDObject*& Temp = *(MWDObject**)(void*)&Io;
					LinkObjectPtr(Temp);
				}
				else if (TIsMWDSmartPointerType<T>::Value)
				{
					MWDObjectPtr& Temp = *(MWDObjectPtr*)(void*)&Io;
					LinkObjectPtr(Temp);
				}
				else if (TIsMWDType<T>::Value)
				{
					MWDObject* Key = (MWDObject*)&Io;
					MWDObject* Value = NULL;
					Value = GetMWDTypeMapValue(Key);
					LinkObjectPtr(Value);
					MWDMAC_ASSERT(Value);
					MWDObject::CloneObject(Value, Key);
					// 				unsigned int uiIndex = m_pVObjectArray.FindElement(Value);
					// 				MWDMAC_ASSERT(uiIndex < m_CopyUsed.GetNum());
					// 				m_CopyUsed[uiIndex] = true;
				}
				else if (TIsCustomType<T>::Value)
				{
					MWDCustomArchiveObject* Temp = (MWDCustomArchiveObject*)(void*)&Io;
					if (Temp)
					{
						Temp->Archive(*this);
					}

				}
			}
		}
		template<class T, MWDMemManagerFun MMFun = MWDMemObject::GetMemManager>
		void Archive(MWDArray<T, MMFun>& Io)
		{
			if (m_uiStreamFlag == AT_LOAD)
			{
				unsigned int uiNum = 0;
				Archive(uiNum);
				Io.ResetBufferNum(uiNum);
				for (unsigned int i = 0; i < uiNum; i++)
				{
					Archive(Io[i]);
				}
			}
			else if (m_uiStreamFlag == AT_SAVE)
			{
				unsigned int uiNum = Io.GetNum();
				Archive(uiNum);
				for (unsigned int i = 0; i < uiNum; i++)
				{
					Archive(Io[i]);
				}
			}
			else if (m_uiStreamFlag == AT_REGISTER || m_uiStreamFlag == AT_LINK
				|| m_uiStreamFlag == AT_POSTLOAD || m_uiStreamFlag == AT_OBJECT_COLLECT_GC || m_uiStreamFlag == AT_LOAD_OBJECT_COLLECT_GC
				|| m_uiStreamFlag == AT_CLEAR_OBJECT_PROPERTY_GC)
			{
				unsigned int uiNum = Io.GetNum();
				for (unsigned int i = 0; i < uiNum; i++)
				{
					Archive(Io[i]);
				}
			}
			else if (m_uiStreamFlag == AT_SIZE)
			{
				unsigned int uiNum = Io.GetNum();
				Archive(uiNum);
				for (unsigned int i = 0; i < uiNum; i++)
				{
					Archive(Io[i]);
				}
			}

		}

		template<class T, MWDMemManagerFun MMFun>
		void Archive(MWDArrayOrder<T, MMFun>& Io)
		{
			if (m_uiStreamFlag == AT_LOAD)
			{
				unsigned int uiNum = 0;
				Archive(uiNum);
				Io.ResetBufferNum(uiNum);
				for (unsigned int i = 0; i < uiNum; i++)
				{
					Archive(Io[i]);
				}
			}
			else if (m_uiStreamFlag == AT_SAVE)
			{
				unsigned int uiNum = Io.GetNum();
				Archive(uiNum);
				for (unsigned int i = 0; i < uiNum; i++)
				{
					Archive(Io[i]);
				}
			}
			else if (m_uiStreamFlag == AT_REGISTER || m_uiStreamFlag == AT_LINK
				|| m_uiStreamFlag == AT_POSTLOAD || m_uiStreamFlag == AT_OBJECT_COLLECT_GC || m_uiStreamFlag == AT_LOAD_OBJECT_COLLECT_GC
				|| m_uiStreamFlag == AT_CLEAR_OBJECT_PROPERTY_GC)
			{
				unsigned int uiNum = Io.GetNum();
				for (unsigned int i = 0; i < uiNum; i++)
				{
					Archive(Io[i]);
				}
			}
			else if (m_uiStreamFlag == AT_SIZE)
			{
				unsigned int uiNum = Io.GetNum();
				Archive(uiNum);
				for (unsigned int i = 0; i < uiNum; i++)
				{
					Archive(Io[i]);
				}
			}

		}

		template<class Key, class Value, MWDMemManagerFun MMFun>
		void Archive(MWDMap<Key, Value, MMFun>& Io)
		{
			if (m_uiStreamFlag == AT_LOAD)
			{
				unsigned int uiNum = 0;
				Archive(uiNum);
				Io.ResetBufferNum(uiNum);
				for (unsigned int i = 0; i < uiNum; i++)
				{
					MWDMapElement<Key, Value>& Element = Io[i];
					Archive(Element.Key);
					Archive(Element.Value);
				}
			}
			else if (m_uiStreamFlag == AT_SAVE)
			{
				unsigned int uiNum = Io.GetNum();
				Archive(uiNum);
				for (unsigned int i = 0; i < uiNum; i++)
				{
					MWDMapElement<Key, Value>& Element = Io[i];
					Archive(Element.Key);
					Archive(Element.Value);
				}
			}
			else if (m_uiStreamFlag == AT_REGISTER || m_uiStreamFlag == AT_LINK || m_uiStreamFlag == AT_POSTLOAD
				|| m_uiStreamFlag == AT_OBJECT_COLLECT_GC || m_uiStreamFlag == AT_LOAD_OBJECT_COLLECT_GC
				|| m_uiStreamFlag == AT_CLEAR_OBJECT_PROPERTY_GC)
			{
				unsigned int uiNum = Io.GetNum();
				for (unsigned int i = 0; i < uiNum; i++)
				{
					MWDMapElement<Key, Value>& Element = Io[i];
					Archive(Element.Key);
					Archive(Element.Value);
				}
			}
			else if (m_uiStreamFlag == AT_SIZE)
			{
				unsigned int uiNum = Io.GetNum();
				Archive(uiNum);
				for (unsigned int i = 0; i < uiNum; i++)
				{
					MWDMapElement<Key, Value>& Element = Io[i];
					Archive(Element.Key);
					Archive(Element.Value);
				}
			}
		}

		template<class Key, class Value, MWDMemManagerFun MMFun>
		void Archive(MWDMapOrder<Key, Value, MMFun>& Io)
		{
			if (m_uiStreamFlag == AT_LOAD)
			{
				unsigned int uiNum = 0;
				Archive(uiNum);
				Io.ResetBufferNum(uiNum);
				for (unsigned int i = 0; i < uiNum; i++)
				{
					MWDMapElement<Key, Value>& Element = Io[i];
					Archive(Element.Key);
					Archive(Element.Value);
				}
			}
			else if (m_uiStreamFlag == AT_SAVE)
			{
				unsigned int uiNum = Io.GetNum();
				Archive(uiNum);
				for (unsigned int i = 0; i < uiNum; i++)
				{
					MWDMapElement<Key, Value>& Element = Io[i];
					Archive(Element.Key);
					Archive(Element.Value);
				}
			}
			else if (m_uiStreamFlag == AT_REGISTER || m_uiStreamFlag == AT_LINK || m_uiStreamFlag == AT_POSTLOAD
				|| m_uiStreamFlag == AT_OBJECT_COLLECT_GC || m_uiStreamFlag == AT_LOAD_OBJECT_COLLECT_GC
				|| m_uiStreamFlag == AT_CLEAR_OBJECT_PROPERTY_GC)
			{
				unsigned int uiNum = Io.GetNum();
				for (unsigned int i = 0; i < uiNum; i++)
				{
					MWDMapElement<Key, Value>& Element = Io[i];
					Archive(Element.Key);
					Archive(Element.Value);
				}
			}
			else if (m_uiStreamFlag == AT_SIZE)
			{
				unsigned int uiNum = Io.GetNum();
				Archive(uiNum);
				for (unsigned int i = 0; i < uiNum; i++)
				{
					MWDMapElement<Key, Value>& Element = Io[i];
					Archive(Element.Key);
					Archive(Element.Value);
				}
			}
		}

		//这两个API需要include resource.h
		//bool WriteResource(MWDResourceProxyBasePtr& Resource);
		//bool ReadResource(MWDResourceProxyBasePtr& Resource);

		FORCEINLINE unsigned int GetStreamFlag()const
		{
			return m_uiStreamFlag;
		}
		FORCEINLINE void SetStreamFlag(unsigned int uiStreamFlag)
		{
			m_uiStreamFlag = uiStreamFlag;
		}
		FORCEINLINE void AddBufferSize(unsigned int uiSize)
		{
			m_uiArchivePropertySize += uiSize;
		}

		struct ObjectPropertyTable
		{
			ObjectPropertyTable()
			{
				m_uiOffSet = 0;
				m_uiSize = 0;
				m_uiNameID = 0;
			}
			MWDString m_PropertyName;
			unsigned int m_uiOffSet;
			unsigned int m_uiSize;
			unsigned int m_uiNameID;
		};

		struct ObjectTable
		{
			ObjectTable()
			{
				m_pAddr = NULL;
				m_uiOffSet = 0;
				m_uiObjectPropertySize = 0;
				m_uiObjectPropertyTableSize = 0;
				m_uiObjectPropertyNum = 0;
			}
			MWDObject* m_pAddr;
			MWDString m_RttiName;
			unsigned int m_uiOffSet;
			unsigned int m_uiObjectPropertySize;
			unsigned int m_uiObjectPropertyTableSize;
			unsigned int m_uiObjectPropertyNum;
			MWDArray<ObjectPropertyTable> m_ObjectPropertyTable;
		};
		bool m_bLoadUseGC;
	protected:
		unsigned int m_uiStreamFlag;
		unsigned int m_uiArchivePropertySize;
	public:
		virtual bool NewLoad(const TCHAR* const pcFileName);
		virtual bool NewSave(const TCHAR* const pcFileName);

		virtual bool NewLoadFromBuffer(unsigned char* pBuffer, unsigned int uiSize);

		bool ArchiveAll(MWDObject* pObject);
	public:
		bool RegisterObject(MWDObject* pObject);

		bool RegisterPostLoadObject(MWDObject* pObject);

		bool RegisterReachableObject(MWDObject* pObject);

		MWDStream() {
			m_pVObjectArray.Clear();
			m_pmLoadMap.Clear();
			m_pmMWDTypeLoadMap.Clear();
			m_pcCurBufPtr = NULL;
			m_pcBuffer = NULL;
			m_uiBufferSize = 0;
			m_dwFlag = 0;
			m_bLoadUseGC = false;
		};
		~MWDStream() {
			m_pVObjectArray.Clear();
			m_pcCurBufPtr = NULL;
		};
		bool Read(void* pvBuffer, unsigned int uiSize) {
			if (!pvBuffer || !m_pcBuffer)
				return 0;

			if (unsigned int(m_pcCurBufPtr + uiSize - m_pcBuffer) > m_uiBufferSize)
				return 0;


			MWDMemcpy(pvBuffer, m_pcCurBufPtr, uiSize);

			m_pcCurBufPtr += uiSize;
			return 1;
		};
		bool Write(const void* pvBuffer, unsigned int uiSize) {
			if (!pvBuffer || !m_pcBuffer)
				return 0;

			if (unsigned int(m_pcCurBufPtr + uiSize - m_pcBuffer) > m_uiBufferSize)
				return 0;

			MWDMemcpy(m_pcCurBufPtr, pvBuffer, uiSize);

			m_pcCurBufPtr += uiSize;
			return 1;
		};
		bool ReadString(MWDString& String) {
			unsigned int uiBufferSize = 0;
			unsigned int uiStrLen = 0;
			if (!Read(&uiBufferSize, sizeof(unsigned int)))
				return 0;
			if (!uiBufferSize)
			{
				return 1;
			}
			char* pCh = 0;
			pCh = new char[uiBufferSize];
			if (!Read(pCh, uiBufferSize))
			{
				MWDMAC_DELETEA(pCh);
				return 0;
			}
			if (HasAnyFlag(UNICODE_FLAG))
			{
				uiStrLen = uiBufferSize >> 1;
				String.SetTCHARBufferNum(uiStrLen);
#ifdef	_UNICODE		
				MWDMemcpy(String.GetBuffer(), pCh, uiBufferSize);
#else
				MWDWcsToMbs(String.GetBuffer(), uiStrLen, (wchar_t*)pCh, uiStrLen);
#endif
			}
			else
			{
				uiStrLen = uiBufferSize;
				String.SetTCHARBufferNum(uiStrLen);
#ifdef	_UNICODE
				MWDMbsToWcs((wchar_t*)String.GetBuffer(), uiStrLen, pCh, uiStrLen);
#else
				MWDMemcpy(String.GetBuffer(), pCh, uiBufferSize);
#endif
			}
			MWDMAC_DELETEA(pCh);

			return 1;
		};
		bool WriteString(const MWDString& String) {
			unsigned int uiBufferLen = String.GetLength() * sizeof(TCHAR);
			if (!Write(&uiBufferLen, sizeof(unsigned int)))
				return 0;
			if (!Write(String.GetBuffer(), uiBufferLen))
				return 0;
			return 1;
		};
		//需要引用resource.h
		//static int GetResourceDistUse(MWDResourceProxyBasePtr& Resource);
		static int GetStrDistUse(const MWDString& Str) {
			return sizeof(int) + (unsigned int)Str.GetLength() * sizeof(TCHAR);
		};
		static int GetStrDistUse(const TCHAR* pCh) {
			return sizeof(int) + (unsigned int)MWDStrLen(pCh) * sizeof(TCHAR);
		};
		const MWDObject* GetMapValue(MWDObject* pKey)const {
			unsigned int i = m_pmLoadMap.Find(pKey);
			if (i == m_pmLoadMap.GetNum())
				return NULL;

			return m_pmLoadMap[i].Value;
		};
		MWDObject* GetMWDTypeMapValue(MWDObject* pKey)const {
			unsigned int i = m_pmMWDTypeLoadMap.Find(pKey);
			if (i == m_pmMWDTypeLoadMap.GetNum())
				return NULL;

			return m_pmMWDTypeLoadMap[i].Value;
		};

		//这个API要用资源管理器（但是暂时关掉了）
		const MWDObject* GetObjectByRtti(const MWDRtti& Rtti);
		bool GetObjectArrayByRtti(const MWDRtti& Rtti, MWDArray<MWDObject*>& ObjectArray, bool IsDerivedFrom = false);
		bool GetAllResourceObject(MWDArray<MWDObject*>& ObjectArray) { return true; };
		FORCEINLINE unsigned int GetVerson() const
		{
			return m_uiVerson;
		}

		FORCEINLINE static unsigned int GetCurVerson()
		{
			return ms_uiCurVerson;
		}
		enum
		{
			UNICODE_FLAG = BIT(0)
		};
		FORCEINLINE bool HasAnyFlag(DWORD dwFlagsToCheck)const
		{
			return (m_dwFlag & dwFlagsToCheck) != 0;
		}
		template<class T>
		bool ReadObjectPtr(MWDPointer<T>& Pointer) {
			T* pP = NULL;
			if (!Read(&pP, sizeof(T*)))
				return 0;
			Pointer.SetObject(pP);
			return 1;
		};

		template<class T>
		bool WriteObjectPtr(const MWDPointer<T>& Pointer) {
			T* pP = Pointer;
			if (!Write(&pP, sizeof(T*)))
				return 0;
			return 1;
		};

		template<class T>
		bool LinkObjectPtr(MWDPointer<T>& Pointer) {
			MWDObject* pP = (MWDObject*)Pointer.GetObject();

			Pointer.SetObject(NULL);

			Pointer = (T*)GetMapValue(pP);

			if (!Pointer)
			{
				return 0;
			}
			return 1;
		};

		template<class T>
		bool ReadObjectPtr(T*& pObject) {
			if (!Read(&pObject, sizeof(T*)))
				return 0;
			return 1;
		};

		template<class T>
		bool WriteObjectPtr(T* const& pObject) {
			if (!Write(&pObject, sizeof(T*)))
				return 0;
			return 1;
		};

		template<class T>
		bool LinkObjectPtr(T*& pObject) {
			pObject = (T*)GetMapValue(pObject);

			if (!pObject)
			{
				return 0;
			}
			return 1;
		};
	private:
		unsigned char* m_pcCurBufPtr;
		unsigned char* m_pcBuffer;
		unsigned int m_uiBufferSize;
		MWDArray<MWDObject*> m_pVObjectArray;
		MWDArray<MWDObject*> m_pPostLoadObject;

		MWDMap<MWDObject*, MWDObject*> m_pmLoadMap;
		MWDMap<MWDObject*, MWDObject*> m_pmMWDTypeLoadMap;

		static inline unsigned int ms_uiCurVerson;
		unsigned int m_uiVerson;
		DWORD	m_dwFlag;
	};


}

