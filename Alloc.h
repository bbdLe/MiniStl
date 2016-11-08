#ifndef __MINISTL_ALLOC_H_
#define __MINISTL_ALLOC_H_

#include<cstdlib>
#include<cstring>
#include<iostream>

namespace MiniStl
{
    // 第一级配置器
    template <int inst>
    class _malloc_alloc_template
    {
        public:
            static void *allocate(size_t n)
            {
                void *result = std::malloc(n);
                if(!result)
                    result = oom_malloc(n);
                return result;
            }

            static void deallocate(void *p, size_t /* n */)
            {
                std::free(p);
            }
            
            static void *reallocate(void *p, size_t /* old_size */, size_t new_size)
            {
                void *result = std::realloc(p, new_size);       // 如realloc(3)所说, 内部内容不会改变, 所以不会自行处理内部内容
                if(!result)
                    result = oom_realloc(p, new_size);
                return result;
            }

            static void (*set_malloc_handler(void (*newHandler)()))()
            {
                void (*oldHandler)() = _malloc_alloc_oom_handler;
                _malloc_alloc_oom_handler = newHandler;
                return oldHandler;
            }
        
        private:
            static void *oom_malloc(size_t);
            static void *oom_realloc(void *, size_t);
            static void (* _malloc_alloc_oom_handler)();
    };

    template <int inst>
    void (* _malloc_alloc_template<inst>::_malloc_alloc_oom_handler)() = NULL;


    // 当allocate无法分配内存时, 调用该函数处理
    template <int inst>
    void * _malloc_alloc_template<inst>::oom_malloc(size_t n)
    {
        void (*mallocHandler)() = NULL;
        void *result = NULL;

        for(;;)
        {
            mallocHandler = _malloc_alloc_oom_handler;
            if(!mallocHandler)
            {
                // FixMe
                std::exit(-1);
            }
            (*mallocHandler)();
            result = malloc(n);
            if(result)
            {
                return result;
            }
        }
    }

    // 当调用reallocate无法重新分配内存时, 调用该函数处理
    template <int inst>
    void *_malloc_alloc_template<inst>::oom_realloc(void *ptr, size_t n)
    {
        void (*mallocHandler)() = NULL;
        void *result = NULL;

        for(;;)
        {
            mallocHandler = _malloc_alloc_oom_handler;
            if(!mallocHandler)
            {
                //FixMe
                std::exit(-1);
            }
            (*mallocHandler)();
            result = std::realloc(ptr, n);
            if(result)
            {
                return result;
            }
        }
    }

    // 第一级配置器
    typedef _malloc_alloc_template<0> malloc_alloc;

    // 包装配置器
    template<class T, class Alloc>
    struct simple_alloc
    {
        static T* allocate(size_t n)
        {
            return n == 0? static_cast<T*>(NULL) : static_cast<T*>(Alloc::allocate(n * sizeof(T)));
        }

        static T* allocate()
        {
            return static_cast<T*>(Alloc::allocate(sizeof(T)));
        }

        static void deallocate(T *p)
        {
            Alloc::deallocate(static_cast<void *>(p), sizeof(T));
        }

        static void deallocate(T *p, size_t n)
        {
            if(p != NULL)
                Alloc::deallocate(static_cast<void *>(p), sizeof(T) * n);
        }
    };


    // 第二级分配器, 没有为多线程做处理
    template <bool threads, int inst>
    class default_alloc_template
    {
        public:
            static void *allocate(size_t n);

            static void deallocate(void *p, size_t n);

            static void *reallocate(void *p, size_t oldSize, size_t newSize);

        private:
            static void *refill(size_t n);

            static char *chunk_alloc(size_t size, int &nobjs);

            static size_t ROUND_UP(size_t n)        // 将n上调为8的整数倍
            {
                return (n + _ALIGN - 1) & ~(_ALIGN - 1);
            }

            static size_t FREELIST_INDEX(size_t n)  // 计算出n所属链表的索引
            {
                return ((n + _ALIGN - 1) / _ALIGN - 1);
            }

            enum {_ALIGN = 8};                      // the enum hack
            enum {_MAX_BYTES = 128};
            enum {_NFREELISTS = _MAX_BYTES / _ALIGN};

            union obj
            {
                union obj *freeListLink;
                char client_data[1];
            };

            static obj *freeList[_NFREELISTS];      // 16个链表
            static char *startFree;
            static char *endFree;
            static size_t heapSize;
    };

    template <bool threads, int inst> typename default_alloc_template<threads, inst>::obj *default_alloc_template<threads, inst>::freeList[_NFREELISTS] = 
    {
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
        NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL
    };

    template <bool threads, int inst> char *default_alloc_template<threads, inst>::startFree = NULL;

    template <bool threads, int inst> char *default_alloc_template<threads, inst>::endFree = NULL;

    template <bool threads, int inst> size_t default_alloc_template<threads, inst>::heapSize = 0;

    template <bool threads, int inst>
    void *default_alloc_template<threads, inst>::allocate(size_t n)
    {
        obj *result = NULL;
        obj **myFreeList = NULL;
    
        // 大于128字节的直接调用第一级适配器分配
        if(n > static_cast<size_t>(_MAX_BYTES))
        {
            return malloc_alloc::allocate(n);
        }
        myFreeList = freeList + FREELIST_INDEX(n);
        result = *myFreeList;
        if(result == NULL)
        {
            return refill(ROUND_UP(n));
        }
        *myFreeList = result->freeListLink;
        return static_cast<void *>(result);
    }

    template <bool threads, int inst>
    void default_alloc_template<threads, inst>::deallocate(void *p, size_t size)
    {
        // 大于128字节用第一级适配器释放
        if(size > static_cast<size_t>(_MAX_BYTES))
        {
            malloc_alloc::deallocate(p, size);
            return;
        }
        obj **myFreeList = freeList + FREELIST_INDEX(size);
        obj *listNode = static_cast<obj *>(p);
        listNode->freeListLink = *myFreeList;
        *myFreeList = listNode;
    }

    template <bool threads, int inst>
    void *default_alloc_template<threads, inst>::reallocate(void *p, size_t oldSize, size_t newSize)
    {
        if(oldSize > static_cast<size_t>(_MAX_BYTES) && newSize > static_cast<size_t>(_MAX_BYTES))
        {
            return std::realloc(p, newSize);
        }

        if(ROUND_UP(newSize) == ROUND_UP(oldSize))
            return p;
        void *result = allocate(newSize);
        size_t copySize = newSize < oldSize? newSize : oldSize;
        std::memcpy(result, p, copySize);       // 模拟realloc内部操作, 复制原内容到新内存当中去
        deallocate(p, oldSize);
        return result;
    }

    // 重新填充大小为n的链表
    template <bool threads, int inst>
    void *default_alloc_template<threads, inst>::refill(size_t n)
    {
        int num = 20;   // 默认分配20块内存
        char *chunk = chunk_alloc(n, num);  // num传入chunk_alloc为引用

        if(num == 1)
        {
            return static_cast<void *>(chunk);
        }
        obj **myFreeList = freeList + FREELIST_INDEX(n);
        obj *currentObj, *nextObj;
        *myFreeList = nextObj = reinterpret_cast<obj *>(chunk + n);
        // 链接每块内存
        for(int i = 1;; ++i)
        {
            currentObj = nextObj;
            nextObj = reinterpret_cast<obj *>(reinterpret_cast<char *>(nextObj) + n);
            if(i == num - 1)
            {
                currentObj->freeListLink = NULL;
                break;
            }
            else
            {
                currentObj->freeListLink = nextObj;
            }
        }
        return static_cast<void *>(chunk);
    }

    template <bool threads, int inst>
    char *default_alloc_template<threads, inst>::chunk_alloc(size_t n, int &nobjs)
    {
        size_t totalBytes = static_cast<size_t>(nobjs) * n;
        size_t bytesLeft = endFree - startFree;
        char *result = NULL;

        // 完全足够
        if(totalBytes <= bytesLeft)
        {
            result = startFree;
            startFree += totalBytes;
            return result;
        }
        // 够分配数个对象
        else if(bytesLeft >= n)
        {
            nobjs = static_cast<int>(bytesLeft / n);
            totalBytes = static_cast<size_t>(nobjs) * n;
            result = startFree;
            startFree += totalBytes;
            return result;
        }
        // 一个都无法分配
        else
        {
            // 把剩下的部分先分配给内存链
            size_t bytesToGet = 2 * totalBytes + (heapSize >> 4);
            if(bytesLeft > 0)
            {
                obj **myFreeList = freeList + FREELIST_INDEX(bytesLeft);
                (reinterpret_cast<obj*>(startFree))->freeListLink = *myFreeList;
                *myFreeList = reinterpret_cast<obj*>(startFree);
            }
            startFree = static_cast<char*>(std::malloc(bytesToGet));
            // 山穷水尽，只好抽取内存链的空间
            if(startFree == NULL)
            {
                obj **myFreeList = NULL;
                for(size_t i = n; i <= _MAX_BYTES; i += _ALIGN)
                {
                    myFreeList = freeList + FREELIST_INDEX(i);
                    if(*myFreeList != NULL)
                    {
                        startFree = reinterpret_cast<char *>(*myFreeList);
                        endFree = startFree + i;
                        *myFreeList = (*myFreeList)->freeListLink;
                        return chunk_alloc(n, nobjs);
                    }
                }
                endFree = NULL;
            }
            startFree = static_cast<char *>(malloc_alloc::allocate(bytesToGet));
            endFree = startFree + bytesToGet;
            heapSize += bytesToGet;
            return chunk_alloc(n, nobjs);
        }
    }

#ifdef __USE_MALLOC
typedef malloc_alloc myAlloc;
#else
typedef default_alloc_template<false, 0> myAlloc;
#endif
}
#endif
