# Toy

参考：https://www.zhihu.com/people/qing-hua-yu-95

## [ch-1](https://zhuanlan.zhihu.com/p/639344994)

### 例子

```python
def func(var a, var b) {
  return a + b;
}

def main() {
  var a = 1;
  var b = 2;
  print(func(a, b));
}
```

### 词法分析 - Lexer解析

```
tok_def: "main", "func"
tok_var: "a", "b"
tok_identifier:"+","print"
tok_number: "1","2"
tok_brancket_open: "{","{"
```

### 语法分析

toy语法规则

* 程序（module）由函数（function）组成
* 函数由 `prototype` 和 `block`（{}中的内容）组成
* rototype 由函数名和参数列表组成
* block 由表达式列表组成

 抽象语法树

```
module
  ├── Function
  │   ├── proto type
  │   │   ├── name
  │   │   └── varlist
  │   │       ├── a
  │   │       └── b
  │   └── block
  │       ├── return
  │       └── +
  │           ├── a
  │           └── b
```

`function` 节点为例来看一下 AST 的生成过程。

* function 是由 prototype 和 block 这两个节点组成的：`definition ::= prototype block`，因此，`parseDefinition()` 它会调用 `parsePrototype()` 和 `parseBlock()`来生成 prototype 和 block 节点。
* block，`block ::= { expression_list }`，只有当 `{}` 中的内容识别出是表达式列表的时候，它才是一个正确的block，而 expression_list 则会被构造成 block 节点 -- `ExprASTList` 并返回给 parseDefinition() 用于构造 function 节点 -- `FunctionAST`。
* ExprASTList 构造的前提是 `expression_list ::= block_expr ; expression_list`，即一组数量>=1并以 `;` 分隔的 `block_expr`。
* block_expr 又可以表示为三种节点，`block_expr ::= decl | "return" | expr`，它们以 token 来区分，例如 `var a = 1;`，它会调用 `parseDeclaration()` 来创建变量节点。


（
