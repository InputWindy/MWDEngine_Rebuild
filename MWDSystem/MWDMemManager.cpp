#include "pch.h"
#include "MWDMemManager.h"
#include <iostream>
using namespace std;
MWDEngine::MWDCriticalSection MWDEngine::MWDHeapMemManager::ms_MemLock;
namespace MWDEngine {
    #pragma region Print
    void  MWDHeapMemManager::Print() {
        ms_MemLock.Lock();
            int cnt = 0;
            for (unsigned int i = 0; i < 32; i++)
            {
                if (m_IndirectPool[i])
                {
                    for (unsigned int j = 0; j < 2048; j++)
                    {
                        if (m_IndirectPool[i][j].m_Mem)
                        {
                            ++cnt;
                        }
                    }
                }
            }

            for (int j = 0; j < 42; j++) {
                if (poolTable[j].m_EmptyPool) {
                    cout <<"||" << "poolTable[" << j << "]=" << poolTable[j].m_EmptyPool->m_Parent->m_BlockSize << "||" << endl;
                }
                if (poolTable[j].m_ExhaustedPool) {
                    cout << "||" << "poolTable[" << j << "]=" << poolTable[j].m_ExhaustedPool->m_Parent->m_BlockSize << "||" << endl;
                }
            }
            if (bigMemPoolTable.m_ExhaustedPool) {
                cout << "||" << "bigMemPoolTable=" << bigMemPoolTable.m_ExhaustedPool->m_Parent->m_BlockSize << "||" << endl;
            }
            cout << "||" << "内存块个数为：" << cnt << "||" << endl;
            ms_MemLock.Unlock();
    }
    #pragma endregion

    #pragma region MWDHeapMemManager
        MWDHeapMemManager::MWDHeapMemManager() {
            m_PageSize = 0;
            SYSTEM_INFO SI;
            GetSystemInfo(&SI);
            m_PageSize = SI.dwPageSize;
            MWDMAC_ASSERT(!(m_PageSize&(m_PageSize-1)));

            //初始化43个PoolTable
            bigMemPoolTable.m_EmptyPool = NULL;
            bigMemPoolTable.m_ExhaustedPool= NULL;
            bigMemPoolTable.m_Index = 42;
            bigMemPoolTable.m_BlockSize = 0;

            poolTable[0].m_EmptyPool = NULL;
            poolTable[0].m_ExhaustedPool = NULL;
            poolTable[0].m_Index = 0;
            poolTable[0].m_BlockSize = 8;

            for (DWORD i = 1; i < 5; i++)
            {
                poolTable[i].m_EmptyPool = NULL;
                poolTable[i].m_ExhaustedPool = NULL;
                poolTable[i].m_Index = i;
                poolTable[i].m_BlockSize = (8 << ((i + 1) >> 2)) + (2 << i);
            }
            for (DWORD i = 5; i < POOL_COUNT; i++)
            {
                poolTable[i].m_EmptyPool = NULL;
                poolTable[i].m_ExhaustedPool = NULL;
                poolTable[i].m_Index = i;
                poolTable[i].m_BlockSize = (4 + ((i + 7) & 3)) << (1 + ((i + 7) >> 2));
            }
            //填写映射表
            for (DWORD i = 0; i < POOL_MAX; i++)
            {
                DWORD Index;
                for (Index = 0; poolTable[Index].m_BlockSize < i; Index++);
                MWDMAC_ASSERT(Index < POOL_COUNT);
                mem2poolTable[i] = &poolTable[Index];
            }

            for (DWORD i = 0; i < 32; i++)
            {
                m_IndirectPool[i] = NULL;
            }
            MWDMAC_ASSERT(POOL_MAX-1== poolTable[POOL_COUNT-1].m_BlockSize);
        };
        MWDHeapMemManager::~MWDHeapMemManager() {
            //释放内存管理器里所有空间
            for (unsigned int i = 0; i < 32; i++)
            {
                if (m_IndirectPool[i])
                {
                    for (unsigned int j = 0; j < 2048; j++)
                    {
                        if (m_IndirectPool[i][j].m_Mem)
                        {
                            VirtualFree(m_IndirectPool[i][j].m_Mem, 0, MEM_RELEASE);
                        }
                    }
                    VirtualFree(m_IndirectPool[i], 0, MEM_RELEASE);
                    m_IndirectPool[i] = NULL;
                }
            }
            for (unsigned int i = 0; i < 32; i++)
            {
                if (m_IndirectPool[i])
                {
                    for (unsigned int j = 0; j < 2048; j++)
                    {
                        if (m_IndirectPool[i][j].m_Mem)
                        {
                            cout << "内存销毁失败" << endl;
                        }
                    }
                }
            }
            cout << "内存销毁成功" << endl;
            return;
        };
        //这个函数返回的地址是可用地址，大小为uiSize，前面有8字节的数据包
        void* MWDHeapMemManager::Allocate(unsigned int const uiSize, unsigned int uiAlignment, bool bIsArray) {
            ms_MemLock.Lock();
            void* Mem = NULL;
            void* Free = NULL;
            if (uiSize < POOL_MAX) {
                //查表找到table
                FPoolTable* table = mem2poolTable[uiSize];
                MWDMAC_ASSERT(uiSize <= table->m_BlockSize);
                //看看table里有没有空闲的pool
                FPool* pool = table->m_EmptyPool;
                if (!pool) {
                    //cout << "申请了一次pool" << endl;
                    //申请一个pool，在表格中填写pool基本信息，然后挂载到table上
                    #pragma region 申请一块Mem
                    DWORD Blocks = 65536 /( table->m_BlockSize+sizeof(DWORD)+sizeof(FMem*));
                    DWORD Bytes = Blocks * (table->m_BlockSize + sizeof(DWORD) + sizeof(FMem*));
                    MWDMAC_ASSERT(Blocks >= 1);
                    MWDMAC_ASSERT(Blocks * table->m_BlockSize <= Bytes);
                    Mem = VirtualAlloc(NULL, Bytes, MEM_COMMIT, PAGE_READWRITE);
                    if (!Mem)
                    {
                        return NULL;
                    }
                    #pragma endregion

                    #pragma region 找到表格中的pool
                    //指针引用：FPool* &Indirect，当前格子里存的指针自己是空的
                    FPool* &Indirect = m_IndirectPool[((DWORD)Mem >> 27)];//找到一行
                    if (!Indirect) {
                    
                        Indirect = CreateIndirect();
                    }
                    //Indirect是一个长度为2048的pool数组
                    pool = &Indirect[((DWORD)Mem >> 16) & 0x07FF];
                    #pragma endregion

                    #pragma region 填写pool信息
                    //指针们
                    pool->m_Mem = Mem;
                    pool->m_Parent = table;

                    //这里这个是第一个的
                    pool->m_FirstMem = (FMem*)Mem;

                    pool->m_Next = NULL;
                    pool->m_Pre = NULL;
                
                    //不知道干嘛用的
                    pool->m_Bytes = Bytes;
                    pool->m_OsBytes = Align(Bytes, m_PageSize);
                    pool->m_Taken = 0;

                    //这里浪费了8字节
                    pool->m_FirstMem->m_Blocks = Blocks;
                    pool->m_FirstMem->m_Next = NULL;

                    #pragma endregion
                    pool->Link(table->m_EmptyPool);
                }
                //此时我们有了一个可以用的pool~,Mem指向的是实际使用的空间，pool指向的是内存管理器内的pool实例
                ++pool->m_Taken;
                MWDMAC_ASSERT(pool->m_FirstMem);
                MWDMAC_ASSERT(pool->m_FirstMem->m_Blocks > 0);
                //这里的Free指向的是带着前8字节数据包的地址
                Free = (FMem*)((char*)pool->m_FirstMem + --pool->m_FirstMem->m_Blocks *(table->m_BlockSize+sizeof(DWORD)+sizeof(FMem*)));
                if (pool->m_FirstMem->m_Blocks == 0)
                {
                    pool->m_FirstMem = pool->m_FirstMem->m_Next;
                    if (!pool->m_FirstMem)
                    {
                        // Move to exhausted list.
                        pool->UnLink();
                        pool->Link(table->m_ExhaustedPool);
                    }
                }
                Free = (void*)((char*)Free + sizeof(DWORD) + sizeof(FMem*));
            }
            else {
                //大内存没有FMem结构，所以不需要这8字节的数据包，直接返回地址
                INT AlignedSize = Align(uiSize, m_PageSize);
                Mem = VirtualAlloc(NULL, AlignedSize, MEM_COMMIT, PAGE_READWRITE);
                if (!Mem)
                {
                    return NULL;
                }
                MWDMAC_ASSERT(!((SIZE_T)Mem & 65535));

                FPool*& Indirect = m_IndirectPool[((DWORD)Mem >> 27)];
                if (!Indirect)
                {
                    Indirect = CreateIndirect();
                }
                FPool* pool = &Indirect[((DWORD)Mem >> 16) & 2047];

                #pragma region 填写pool信息
                //指针们
                pool->m_Mem = Mem;
                pool->m_Parent = &bigMemPoolTable;

                pool->m_FirstMem = (FMem*)Mem;

                pool->m_Next = NULL;
                pool->m_Pre = NULL;

                //不知道干嘛用的
                pool->m_Bytes = uiSize;
                pool->m_OsBytes = AlignedSize;
                #pragma endregion
                Free = Mem;
            }
            ms_MemLock.Unlock();
            
            return Free;
        };
        void MWDHeapMemManager::Deallocate(void* addr, unsigned int uiAlignment, bool bIsArray) {
            ms_MemLock.Lock();
            if (!addr)
            {
                return;
            }
            //先找到对应的pool
            FPool* pool = &m_IndirectPool[(DWORD)addr >> 27][((DWORD)addr >> 16) & 2047];
            //cout << &bigMemPoolTable << endl;
            MWDMAC_ASSERT(pool);
            if (pool->m_Parent == &bigMemPoolTable) {
                //cout << "测试" << endl;
                //如果这是一块大内存，就直接扔了
                VirtualFree(addr, 0, MEM_RELEASE);
                pool->m_Mem = NULL;
            }
            else {
                //如果这是一块小内存,销毁位置要往前移动8字节
                addr = (char*)addr - sizeof(DWORD) - sizeof(FMem*);
                //如果要清理的块在已用的链表里，就把表拿出来
                if (!pool->m_FirstMem)
                {
                    pool->UnLink();
                    pool->Link(pool->m_Parent->m_EmptyPool);
                }

                // 没有真的销毁空间，而是加标志允许重写
                FMem* Free = (FMem*)addr;

                Free->m_Blocks = 1;
                Free->m_Next = pool->m_FirstMem;
                
                pool->m_FirstMem = Free;
                // 如果这个pool清空了，就把整个pool销毁
                MWDMAC_ASSERT(pool->m_Taken >= 1);
                if (--pool->m_Taken == 0)
                {
                    // Free the OS memory.
                    pool->UnLink();
                    VirtualFree(pool->m_Mem, 0, MEM_RELEASE);
                    pool->m_Mem = NULL;
                }
            }
            ms_MemLock.Unlock();
        };
    #pragma endregion

    #pragma region MWDMemObject
        MWDMemObject::MWDMemObject() {
            GetMemManager();//创建一个静态堆管理器
        }
        MWDMemObject::~MWDMemObject() {

        }
        MWDHeapMemManager& MWDMemObject::GetMemManager()
        {
            static MWDHeapMemManager gs_HeapMemManager;
            return gs_HeapMemManager;
        }
    #pragma endregion

    


}
