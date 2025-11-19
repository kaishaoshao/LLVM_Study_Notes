#include <llvm/IR/PassManager.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Passes/PassPlugin.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/Support/raw_ostream.h>

using namespace llvm;

namespace {
    // 1. 定义pass类， 继承自PassInfoMixin
   class SabotagePass : public PassInfoMixin<SabotagePass> {
    // run 是 Pass的入口函数
   public:
   PreservedAnalyses run(Function &F, FunctionAnalysisManager &AM) {
    
    bool Changed = false;
    std::vector<Instruction*> WorkList; // 用来存我们要“干掉”的指令

      // --- 阶段一： 扫描
      for(auto &BB : F) {
        // 遍历 BB 中的所有指令I
        for (auto &I : BB) {
          // 检查： 这是一个二元运算符且是加法
          if (auto *BinOp = dyn_cast<BinaryOperator>(&I)) {
            if( BinOp->getOpcode() == Instruction::Add)
              WorkList.push_back(&I);
          }
        }
      }
      
      // --- 阶段二： 搞破坏
      for(auto &Inst : WorkList) {
        // 1. 初始化 Builder， 让它把新指令插在旧指令旁边
        IRBuilder<> Builder(Inst);

        // 2. 创建一个新的减法指令（Sub）
        Value *NewSub = Builder.CreateSub(Inst->getOperand(0), Inst->getOperand(1), "sabotage_sub");

        // 3. [核心魔法] replaceAllUsessWith (RAUW)
        // 告诉LLVM：所有用到旧指令结果的地方，现在全部改为用NewSub
        Inst->replaceAllUsesWith(NewSub);

        // 4. 删除旧指令
        Inst->eraseFromParent();

        // 打印一条日志让我们看着爽一下
        errs() << "😈 把一个 Add 变成了 Sub!\n";
        Changed = true;

      }

      // 如果我们修改了代码，返回PreservedAnalyses::none()
      // 告诉管理器：原本的分析结果可能失效了，需要重新分析
      return Changed ? PreservedAnalyses::none() : PreservedAnalyses::all();
    }

  }; // SabotagePass 
} // namespace

// 2. 注册Pass 
// 告诉opt工具：我有一个叫 “sabotage” 的插件
extern "C" LLVM_ATTRIBUTE_WEAK ::llvm::PassPluginLibraryInfo
llvmGetPassPluginInfo() {
    return {
      LLVM_PLUGIN_API_VERSION, "SabotagePass", LLVM_VERSION_STRING, 
      [](PassBuilder &PB) {
        PB.registerPipelineParsingCallback(
            [](StringRef Name, FunctionPassManager &FPM,
                ArrayRef<PassBuilder::PipelineElement>) {
                    if(Name == "sabotage") {
                        FPM.addPass(SabotagePass());
                        return true;
                    }
                    return false;
                }
        );
      }
    };
}