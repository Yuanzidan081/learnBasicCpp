**LearnBasicCpp**：记录学习C++的笔记和相关代码。

测试编译器：
+ g++ 7.3.0 
```bash
cmake -S . -B build -G "Unix Makefiles"
cmake --build build
```
+ MSVC 2019
```bash
cmake -S . -B build -G "Visual Studio 16 2019"
cmake --build build
````

# 1. [C++11/14标准](./Cpp_11_14_Houjie)

+ [x] [1_start](./Cpp_11_14_Houjie/1_start)
+ [x] [2_variadicTemplate](./Cpp_11_14_Houjie/2_variadicTemplate)
+ [x] [3_nullptr_auto](./Cpp_11_14_Houjie/3_nullptr_auto)
+ [x] [4_uniformInitialization](./Cpp_11_14_Houjie/4_uniformInitialization)
+ [x] [5_initializerList](./Cpp_11_14_Houjie/5_initializerList)
+ [x] [6_explicitCtors](./Cpp_11_14_Houjie/6_explicitCtors)
+ [x] [7_rangeBasedForStatement](./Cpp_11_14_Houjie/7_rangeBasedForStatement)
+ [x] [8_defaultAndDeleteInClass](./Cpp_11_14_Houjie/8_defaultAndDeleteInClass)
+ [x] [9_aliasTemplate](./Cpp_11_14_Houjie/9_aliasTemplate)
+ [x] [10_templateTemplateParameter](./Cpp_11_14_Houjie/10_templateTemplateParameter)
+ [x] [11_typeAliasAndNoexceptAndOverrideAndFinal](./Cpp_11_14_Houjie/11_typeAliasAndNoexceptAndOverrideAndFinal)
+ [x] [12_decltype](./Cpp_11_14_Houjie/12_decltype)
+ [x] [13_lambda](./Cpp_11_14_Houjie/13_lambda)
+ [x] [14_rightValue](./Cpp_11_14_Houjie/14_rightValue)
+ [x] [15_perfectForwarding](./Cpp_11_14_Houjie/15_perfectForwarding)
  
#  2. [内存管理](./MemoryManagement_Houjie)

+ [x] [1_memoryPrimitives](./MemoryManagement_Houjie/1_memoryPrimitives)    
+ [x] [2_overloadOperatorNewDelete](./MemoryManagement_Houjie/2_overloadOperatorNewDelete)
+ [x] [3_perClassAllocator](./MemoryManagement_Houjie/3_perClassAllocator)  
## Reference

### Blogs and Books

+ [Doug Lea's Home Page](https://gee.cs.oswego.edu/)主页下的[A Memory Allocator](https://gee.cs.oswego.edu/dl/html/malloc.html)
+ 《STL源码剖析》, 侯捷, Chapter2: allocator
+ 《Small Memory Software》, James Noble and Charles Weir, AW 2001
+ 《Modern C++ Design》, Andrei Alexandrescu, Chapter4: Small-Object Allocation

### Libraries

+ STL Allocations
  + [MSVC STL](https://github.com/microsoft/STL)
  + [LLVM libc++](https://github.com/llvm/llvm-project/tree/main/libcxx)
  + [GNU libstdc++](https://github.com/gcc-mirror/gcc/tree/master/libstdc++-v3)
+ [MFC CPlex+CFixedAlloc](https://github.com/MicrosoftDocs/cpp-docs/tree/main/docs/mfc)
  + 本地安装VS以后也可以在类似下面的路径找到：`C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Tools\MSVC\<version>\atlmfc\src\mfc`
+ Boost.Pool
  + [官方仓库](https://github.com/boostorg/pool)
  + [Boost 主库](https://github.com/boostorg/boost)
+ Loki SmallObjAllocator
  + [Loki 库（包含 SmallObjAllocator）](https://github.com/snaewe/loki-lib)
  + [SourceForge 历史版本](https://sourceforge.net/projects/loki-lib/)
+ VC malloc/free
  + 在[MSVC STL](https://github.com/microsoft/STL/tree/main/src)中，相关的文件包括`malloc.cpp`, `free.cpp`, `new.cpp`, `delete.cpp` 等
+ [jemalloc](https://github.com/jemalloc/jemalloc)
+ [tcmalloc](https://github.com/google/tcmalloc)
