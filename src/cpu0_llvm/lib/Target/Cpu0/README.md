### 目标注册
将Cpu0注册到LLVM架构中，让LLVM知道我们新增一个后端
```
touch Cpu0.h
touch Cpu0TargetMachine.cpp、Cpu0TatgetMachine.cpp
	# LLVMInitializaCpu0Target()
mkdir TargetInfo && cd TargetInfo
touch Cpu0TargetInfo.cpp
	# RegisterTarget接口，注册Cpu0 and cpu0el
cd ..
mkdir MCTargetDesc && cd MCTargetDesc    
touch MCTargetDesc.cpp && touch MCTargetDesc.h



```
