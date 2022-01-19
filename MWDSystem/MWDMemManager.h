#pragma once
#include <iostream>
#include <Windows.h>
#include "MWDSystem.h"
#include "MWDSynchronize.h"
#define USE_STL_TYPE_TRAIT
#ifdef USE_STL_TYPE_TRAIT
#include <type_traits>//静态类型判断
#endif // USE_STL_TYPE_TRAIT


//typedef unsigned long       DWORD;//32bit
//typedef int                 BOOL; //32bit
//typedef unsigned char       BYTE; //8bit 
//typedef unsigned short      WORD; //16bit
//typedef float               FLOAT;//64bit 
//1字节=8bit
namespace MWDEngine {
#pragma region type_traits
#ifdef USE_STL_TYPE_TRAIT
	//静态类型判断
#define HAS_TRIVIAL_CONSTRUCTOR(T) std::is_trivially_constructible<T>::value
#define HAS_TRIVIAL_DESTRUCTOR(T) std::is_trivially_destructible<T>::value
#define HAS_TRIVIAL_ASSIGN(T) std::is_trivially_assignable<T>::value
#define HAS_TRIVIAL_COPY(T) std::is_trivially_copyable<T>::value
#define IS_POD(T) std::is_pod<T>::value
#define IS_ENUM(T) std::is_enum<T>::value
#define IS_EMPTY(T) std::is_empty<T>::value

//自定义静态类型判断
/**
* TIsFloatType
*/
	template<typename T> struct TIsFloatType { enum { Value = std::is_floating_point<T>::value }; };


	/**
	* TIsIntegralType
	*/
	template<typename T> struct TIsIntegralType { enum { Value = std::is_integral<T>::value }; };


	/**
	* TIsArithmeticType
	*/
	template<typename T> struct TIsArithmeticType
	{
		enum { Value = std::is_arithmetic<T>::value };
	};

	/**
	* TIsPointerType
	* @todo - exclude member pointers
	*/
	template<typename T> struct TIsPointerType { enum { Value = std::is_pointer<T>::value }; };


	/**
	* TIsVoidType
	*/
	template<typename T> struct TIsVoidType { enum { Value = std::is_void<T>::value }; };


	/**
	* TIsPODType
	*/
	template<typename T> struct TIsPODType { enum { Value = IS_POD(T) }; };

	/**
	* TIsFundamentalType
	*/
	template<typename T> struct TIsFundamentalType { enum { Value = std::is_fundamental<T>::Value }; };

	template<typename T> struct ValueBase
	{
		enum { NeedsConstructor = !HAS_TRIVIAL_CONSTRUCTOR(T) && !TIsPODType<T>::Value };
		enum { NeedsDestructor = !HAS_TRIVIAL_DESTRUCTOR(T) && !TIsPODType<T>::Value };
	};
#else
#if _MSC_VER >= 1400
#define HAS_TRIVIAL_CONSTRUCTOR(T) __has_trivial_constructor(T)
#define HAS_TRIVIAL_DESTRUCTOR(T) __has_trivial_destructor(T)
#define HAS_TRIVIAL_ASSIGN(T) __has_trivial_assign(T)
#define HAS_TRIVIAL_COPY(T) __has_trivial_copy(T)
#define IS_POD(T) __is_pod(T)
#define IS_ENUM(T) __is_enum(T)
#define IS_EMPTY(T) __is_empty(T)
#else
#define HAS_TRIVIAL_CONSTRUCTOR(T) false
#define HAS_TRIVIAL_DESTRUCTOR(T) false
#define HAS_TRIVIAL_ASSIGN(T) false
#define HAS_TRIVIAL_COPY(T) false
#define IS_POD(T) false
#define IS_ENUM(T) false
#define IS_EMPTY(T) false
#endif

	template<typename T> struct TIsFloatType { enum { Value = false }; };

	template<> struct TIsFloatType<float> { enum { Value = true }; };
	template<> struct TIsFloatType<double> { enum { Value = true }; };
	template<> struct TIsFloatType<long double> { enum { Value = true }; };

	/**
	* TIsIntegralType
	*/
	template<typename T> struct TIsIntegralType { enum { Value = false }; };

	template<> struct TIsIntegralType<unsigned char> { enum { Value = true }; };
	template<> struct TIsIntegralType<unsigned short> { enum { Value = true }; };
	template<> struct TIsIntegralType<unsigned int> { enum { Value = true }; };
	template<> struct TIsIntegralType<unsigned long> { enum { Value = true }; };

	template<> struct TIsIntegralType<signed char> { enum { Value = true }; };
	template<> struct TIsIntegralType<signed short> { enum { Value = true }; };
	template<> struct TIsIntegralType<signed int> { enum { Value = true }; };
	template<> struct TIsIntegralType<signed long> { enum { Value = true }; };

	template<> struct TIsIntegralType<bool> { enum { Value = true }; };
	template<> struct TIsIntegralType<char> { enum { Value = true }; };

	// compilers we support define wchar_t as a native type
#if !_MSC_VER || defined(_NATIVE_WCHAR_T_DEFINED)
	template<> struct TIsIntegralType<wchar_t> { enum { Value = true }; };
#endif

	// C99, but all compilers we use support it
	template<> struct TIsIntegralType<unsigned long long> { enum { Value = true }; };
	template<> struct TIsIntegralType<signed long long> { enum { Value = true }; };
	/**
	* TIsArithmeticType
	*/
	template<typename T> struct TIsArithmeticType
	{
		enum { Value = TIsIntegralType<T>::Value || TIsFloatType<T>::Value };
	};

	/**
	* TIsPointerType
	* @todo - exclude member pointers
	*/
	template<typename T> struct TIsPointerType { enum { Value = false }; };
	template<typename T> struct TIsPointerType<T*> { enum { Value = true }; };
	template<typename T> struct TIsPointerType<const T*> { enum { Value = true }; };
	template<typename T> struct TIsPointerType<const T* const> { enum { Value = true }; };
	template<typename T> struct TIsPointerType<T* volatile> { enum { Value = true }; };
	template<typename T> struct TIsPointerType<T* const volatile> { enum { Value = true }; };

	/**
	* TIsVoidType
	*/
	template<typename T> struct TIsVoidType { enum { Value = false }; };
	template<> struct TIsVoidType<void> { enum { Value = true }; };
	template<> struct TIsVoidType<void const> { enum { Value = true }; };
	template<> struct TIsVoidType<void volatile> { enum { Value = true }; };
	template<> struct TIsVoidType<void const volatile> { enum { Value = true }; };

	/**
	* TIsPODType
	* @todo - POD array and member pointer detection
	*/
	template<typename T> struct TIsPODType
	{
		enum { Value = IS_POD(T) || IS_ENUM(T) || TIsArithmeticType<T>::Value || TIsPointerType<T>::Value };
	};

	/**
	* TIsFundamentalType
	*/
	template<typename T>
	struct TIsFundamentalType
	{
		enum { Value = TIsArithmeticType<T>::Value || TIsVoidType<T>::Value };
	};


	template<typename T> struct ValueBase
	{
		enum { NeedsConstructor = !HAS_TRIVIAL_CONSTRUCTOR(T) && !TIsPODType<T>::Value };
		enum { NeedsDestructor = !HAS_TRIVIAL_DESTRUCTOR(T) && !TIsPODType<T>::Value };
	};
#endif
#pragma endregion

	#pragma region Align
	template< class T > FORCEINLINE T Align(const T Ptr, unsigned int Alignment)
	{
		return (T)(((unsigned int)Ptr + Alignment - 1) & ~(Alignment - 1));
	}
	template< class T > FORCEINLINE T Align1(const T Ptr, unsigned int Alignment)
	{
		return (T)((unsigned int)Ptr + Alignment - (Ptr & (Alignment - 1)));
	}
#pragma endregion

class MWDSYSTEM_API MWDHeapMemManager {
	public:
		MWDHeapMemManager();
		virtual ~MWDHeapMemManager();
		virtual void* Allocate(unsigned int const uiSize, unsigned int uiAlignment, bool bIsArray);
		virtual void Deallocate(void* addr, unsigned int uiAlignment, bool bIsArray);

		void Print();
	private:
		static MWDCriticalSection ms_MemLock;

		struct FPool;
		struct FMem;
		struct FPoolTable;

		enum { POOL_COUNT = 42 };
		enum { POOL_MAX = 32768 + 1 };

		struct FMem {
			FMem*		m_Next;
			DWORD		m_Blocks;

			FORCEINLINE FPool* GetPool() {
				//高16位地址是对应的FPoolInfo*，每个FPoolInfo管理64KB内存
				return (FPool*)((INT)this & 0xffff0000);
			};
		};


		//PoolState大小1字节
		struct FPoolTable
		{
			FPool*		m_EmptyPool;//这下面永远只会挂着一个或0个pool,永远指向链表第一个节点
			FPool*		m_ExhaustedPool;//这下面会挂很多pool,永远指向链表第一个节点
			DWORD		m_BlockSize;
			INT			m_Index;
		};

		//FPool的开头得有一个Block装填基本信息，外加8字节装指针
		struct FPool
		{
			DWORD 		m_Taken;	
			DWORD		m_OsBytes;	// Aligned_uiSize
			DWORD	    m_Bytes;		// uiSize
			FPoolTable* m_Parent;			//由哪个table管理

			void*		m_Mem;			//自己空间的首地址
			FMem*		m_FirstMem;		//可分配空间的首地址

			FPool**		m_Pre;	//前指针是两颗*，是为了在UnLink能处理链头,m_Pre是指向自己的那根指针，m_Next是下一个pool，整个链表其实是个单链表
			FPool*		m_Next;

			//由于链表结构为单链表且pool Table只有一个链头指针，所以整个链表的插入操作只能发生在链头
			void Link(FPool*& firstPool){
				if (!firstPool)
				{
					m_Next = NULL;
					m_Pre = &firstPool;
					firstPool = this;
				}
				else {
					firstPool->m_Pre = &m_Next;
					this->m_Next = firstPool;
					firstPool = this;
				}
			};
			void UnLink() {
				if (m_Next)
				{
					m_Next->m_Pre = m_Pre;
				}
				*m_Pre = m_Next;
			};
		};

		FPoolTable		poolTable[POOL_COUNT], bigMemPoolTable;//bigMemPoolTable挂载东西，整个链表都是空的
		//这个数组收集了所有没释放的FPool指针
		FPool*			m_IndirectPool[32];
		//这个对照表占用空间131076字节，空间换时间
		FPoolTable*		mem2poolTable[POOL_MAX];
		INT				m_PageSize;
		//开一个数组，存放2048个FPool
		FPool* CreateIndirect()
		{
			FPool* Indirect = (FPool*)VirtualAlloc(NULL, 2048 * sizeof(FPool), MEM_COMMIT, PAGE_READWRITE);
			if (!Indirect)
			{
				return NULL;
			}
			return Indirect;
		}

		FPool* GetPool(void* pcAddr) {
			//高16位地址是对应的FPoolInfo*，每个FPoolInfo管理64KB内存
			return (FPool*)((INT)pcAddr & 0xffff0000);
		};
	};
class MWDSYSTEM_API MWDStackMem
{
public:
	MWDStackMem(unsigned int uiDefaultChunkSize = 65536) {
		Top = NULL;
		End = NULL;
		DefaultChunkSize = uiDefaultChunkSize;
		TopChunk = NULL;
		UnusedChunks = NULL;
		NumMarks = 0;
	};
	~MWDStackMem() {
		FreeChunks(NULL);
		while (UnusedChunks)
		{
			void* Old = UnusedChunks;
			UnusedChunks = UnusedChunks->Next;
			MWDHeapMemManager().Deallocate((char*)Old, 0, true);
		}
	};
	void* Allocate(unsigned int uiSize, unsigned int uiAlignment, bool bIsArray) {
		MWDMAC_ASSERT(uiSize >= 0);
		if (uiAlignment > 0)
		{
			MWDMAC_ASSERT((uiAlignment & (uiAlignment - 1)) == 0);
		}
		MWDMAC_ASSERT(Top <= End);
		MWDMAC_ASSERT(NumMarks > 0);

		// Try to get memory from the current chunk.
		BYTE* Result = Top;
		if (uiAlignment > 0)
		{
			Result = (BYTE*)(((unsigned int)Top + (uiAlignment - 1)) & ~(uiAlignment - 1));
		}
		Top = Result + uiSize;

		// Make sure we didn't overflow.
		if (Top > End)
		{
			// We'd pass the end of the current chunk, so allocate a new one.
			AllocateNewChunk(uiSize + uiAlignment);
			Result = Top;
			if (uiAlignment > 0)
			{
				Result = (BYTE*)(((unsigned int)Top + (uiAlignment - 1)) & ~(uiAlignment - 1));
			}
			Top = Result + uiSize;
		}
		return Result;
	};
	void Deallocate(char* pcAddr, unsigned int uiAlignment, bool bIsArray)
	{
		return;
	}
	template<class T>
	friend class MWDStackMemAlloc;


	void Clear() {
		FreeChunks(NULL);
	};
private:

	// Types.
	struct FTaggedMemory
	{
		FTaggedMemory* Next;
		INT DataSize;
		BYTE Data[1];
	};

	BYTE* Top;				// Top of current chunk (Top<=End).
	BYTE* End;				// End of current chunk.
	unsigned int	DefaultChunkSize;	// Maximum chunk size to allocate.
	FTaggedMemory* TopChunk;			// Only chunks 0..ActiveChunks-1 are valid.

	FTaggedMemory* UnusedChunks;

	INT NumMarks;

	BYTE* AllocateNewChunk(INT MinSize) {
		FTaggedMemory* Chunk = NULL;
		for (FTaggedMemory** Link = &UnusedChunks; *Link; Link = &(*Link)->Next)
		{
			// Find existing chunk.
			if ((*Link)->DataSize >= MinSize)
			{
				Chunk = *Link;
				*Link = (*Link)->Next;
				break;
			}
		}
		if (!Chunk)
		{
			// Create new chunk.
			INT DataSize = Max(MinSize, (INT)DefaultChunkSize - (INT)sizeof(FTaggedMemory));
			Chunk = (FTaggedMemory*)MWDHeapMemManager().Allocate(DataSize + sizeof(FTaggedMemory), 0, true);
			Chunk->DataSize = DataSize;
		}

		Chunk->Next = TopChunk;
		TopChunk = Chunk;
		Top = Chunk->Data;
		End = Top + Chunk->DataSize;
		return Top;
	};

	void FreeChunks(FTaggedMemory* NewTopChunk) {
		while (TopChunk != NewTopChunk)
		{
			FTaggedMemory* RemoveChunk = TopChunk;
			TopChunk = TopChunk->Next;
			RemoveChunk->Next = UnusedChunks;
			UnusedChunks = RemoveChunk;
		}
		Top = NULL;
		End = NULL;
		if (TopChunk)
		{
			Top = TopChunk->Data;
			End = Top + TopChunk->DataSize;
		}
	};
};

class MWDSYSTEM_API MWDMemObject
{
public:
	MWDMemObject();
	~MWDMemObject();
	static MWDStackMem& GetStackMemManager() {
		static MWDStackMem g_StackMemManager;
		return g_StackMemManager;
	};
	static MWDHeapMemManager& GetMemManager();
};

//==========================栈内存管理器还没有自己实现==============================//

typedef MWDHeapMemManager& (*MWDMemManagerFun)();

template<class T>
class MWDStackMemAlloc
{
public:
	// Constructors.

	MWDStackMemAlloc(unsigned int uiNum = 0, unsigned int uiAlignment = 0)
	{
		m_uiNum = uiNum;
		Top = MWDMemObject::GetStackMemManager().Top;
		SavedChunk = MWDMemObject::GetStackMemManager().TopChunk;
		// Track the number of outstanding marks on the stack.
		MWDMemObject::GetStackMemManager().NumMarks++;
		if (m_uiNum > 0)
		{
			m_pPtr = (T*)MWDMemObject::GetStackMemManager().Allocate(uiNum * sizeof(T), uiAlignment, 0);
			MWDMAC_ASSERT(m_pPtr);
			if (ValueBase<T>::NeedsConstructor)
			{
				for (unsigned int i = 0; i < uiNum; i++)
				{
					new(m_pPtr + i)T();
				}
			}
		}
	}

	/** Destructor. */
	~MWDStackMemAlloc()
	{

		if (m_uiNum > 0)
		{
			if (ValueBase<T>::NeedsDestructor)
			{
				for (unsigned int i = 0; i < m_uiNum; i++)
				{
					(m_pPtr + i)->~T();
				}
			}
		}
		// Track the number of outstanding marks on the stack.
		--MWDMemObject::GetStackMemManager().NumMarks;

		// Unlock any new chunks that were allocated.
		if (SavedChunk != MWDMemObject::GetStackMemManager().TopChunk)
			MWDMemObject::GetStackMemManager().FreeChunks(SavedChunk);

		// Restore the memory stack's state.
		MWDMemObject::GetStackMemManager().Top = Top;

		// Ensure that the mark is only popped once by clearing the top pointer.
		Top = NULL;
	}

	FORCEINLINE T* GetPtr()const
	{
		return m_pPtr;
	}
	FORCEINLINE unsigned int GetNum() const
	{
		return m_uiNum;
	}
private:
	BYTE* Top;
	MWDStackMem::FTaggedMemory* SavedChunk;
	T* m_pPtr;
	unsigned int m_uiNum;
};
//==================================================================================//
}
//#define USE_CUSTOM_NEW


#ifdef USE_CUSTOM_NEW

#define HEAP_MEM_INFO_LOG \
cout<<"=====================内存管理器状态======================"<<endl;\
MWDEngine::MWDMemObject::GetMemManager().Print();\
cout<<"=====================内存管理器状态======================"<<endl;

FORCEINLINE void* operator new(size_t uiSize)
{
	return MWDEngine::MWDMemObject::GetMemManager().Allocate((unsigned int)uiSize, 0, false);
}
FORCEINLINE void* operator new[](size_t uiSize)
{
	return MWDEngine::MWDMemObject::GetMemManager().Allocate((unsigned int)uiSize, 0, true);
}
//delete用完之后还要手动把地址改为NULL，要不会出大问题
FORCEINLINE void operator delete (void* pvAddr)
{
	return MWDEngine::MWDMemObject::GetMemManager().Deallocate((char*)pvAddr, 0, false);
}
//delete用完之后还要手动把地址改为NULL，要不会出大问题
FORCEINLINE void operator delete[](void* pvAddr)
{
	return MWDEngine::MWDMemObject::GetMemManager().Deallocate((char*)pvAddr, 0, true);
}
#endif
#define MWDMAC_DELETE(p) if(p){delete p; p = 0;}
#define MWDMAC_DELETEA(p) if(p){delete []p; p = 0;}
#define MWDMAC_DELETEAB(p,num) if(p){ for(int i = 0 ; i < num ; i++) MWDMAC_DELETEA(p[i]); MWDMAC_DELETEA(p);}
	// use by inner mac
	template<typename T>
FORCEINLINE void MWDDelete(T* p)
{
	if (p) { delete p; p = 0; }
}
template<typename T>
FORCEINLINE void MWDDeleteA(T* p)
{
	if (p) { delete[]p; p = 0; }
}
template<typename T, typename N>
FORCEINLINE void MWDDeleteAB(T* p, N num)
{
	if (p) { for (int i = 0; i < num; i++) MWDMAC_DELETEA(p[i]); MWDMAC_DELETEA(p); }
}