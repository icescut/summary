# JavaScript V0.0.1
>本文基于JavaScript高级程序设计。目的是总结JavaScript中关键、重要的特性，而不在于列举JavaScript中的方方面面。

## 简介
JavaScript诞生于1995年，由Nepscape公司的Brendan Eich开发。1997年，由ECMA指定第39号委员会定义标准，称为ECMAScript，JavaScript是ECMAScript的一个实现。  
一个完整的JavaScript的实现由三部分组成（其实不然，node.js就没有DOM和BOM）： 

* 核心（ECMAScript)  
* DOM  
* BOM  

### ECMAScript
实际上这门语言并不依赖于web浏览器，web浏览器只是ECMAScript的宿主环境之一。组成部分：  

* 语法  
* 类型  
* 语句  
* 关键字  
* 保留字  
* 操作符  
* 对象  

**版本**  
第2版     1998  
第3版     1999  
第5版     2009  
第6版     2015  

所有浏览器都支持第3版，但只部分支持第5/6版。相对而言，node.js的支持度比较高。

### DOM
文档对象模型（DOM）是针对xml但经过扩展用于html的api.DOM把整个页面映射为一个多层节点结构。由W3C制定标准。
**DOM级别**  

* DOM1        1998   
>DOM1级由DOM core和DOM HTML组成

* DOM2  
>2级DOM引进了几个新DOM模块来处理新的接口类型：
>DOM视图：描述跟踪一个文档的各种视图（使用CSS样式设计文档前后）的接口；
>DOM事件：描述事件接口；
>DOM样式：描述处理基于CSS样式的接口；
>DOM遍历与范围：描述遍历和操作文档树的接口

* DOM3
>3级DOM通过引入统一方式载入和保存文档和文档验证方法对DOM进行进一步扩展，DOM3包
>含一个名为“DOM载入与保存”的新模块，DOM核心扩展后可支持XML1.0的所有内容，包括XM
>L Infoset、 XPath、和XML Base。

DOM不仅JavaScript语言可用，如svg/ MathML等也能操作DOM。所有浏览器都支持DOM1，但只部分支持DOM2/3。  

### BOM
BOM用于操作浏览器窗口。BOM没有相关的标准，使用原生JavaScript兼容各家浏览器十分困难，建议使用jQuery。

## script元素
对于嵌入JavaScript代码，所有属性都可以不写。对于包含外部文件，那么src是必需的。并且单标签`<script src='xxxx.js' />`是不行的，必须要写成标签对`<script></script>`。 如果同时写了src和嵌入代码，只会下载外部文件，嵌入代码将被忽略。只要不存在defer和async属性，浏览器就会按先后顺序解析。`<script>`标签一般放在`<\body>`之前。  

`<script>`中定义了6个属性：

* async  
>立即下载文件，但不妨碍页面中的其他操作。但不保证执行的先后顺序。一定会在load之前，但不确定在DOMContentLoaded之前或之后。  

* charset  
>很少用

* defer  
>脚本延迟至文档完全被解析和显示之后再执行。理论上会先于DOMContentLoaded事件，但实际上不能确定，最好只包含一个defer脚本或干脆不用。

* language  
>废弃

* src  
>外部文件，可以是外部域名之下的文件。

* type  
>默认为text/javascript  

## 基本概念
### 语法
**区分大小写**  
**标识符**  
所谓标识符，就是指定变量、函数、属性的名字或者函数的参数。标识符可以包含Unicode字符，但不推荐这样做。

* 第一个字符必须是一个字母、_、$
* 其它字符可以是字母、_、$或数字。

**注释**  
`//`或`/*...*/`

**严格模式**  
在代码顶部或函数第一条语句指明`"use strict"`。

**语句**  
ECMAScript的语句以一个分号结尾。如果省略分号，由解析器确定语句结尾，这样有两个问题，第一，因为解析器需要推测语句的结束，增大开销；第二，当压缩JavaScript代码时（删除多余空格），可能会导致错误。

### 关键字和保留字
关键字具有特定用途，保留字将来有可能用作关键字。两者都不能用作标识符。

### 变量
ECMAScript的变量是松散类型的，即可以保存任何类型的值。定义变量的时候，使用var操作符，后跟变量名，将变量定义为该作用域的局部变量。注意严格模式下必须使用var操作符，不能直接使用变量。

### 数据类型
5种基本数据类型：Undefined、Null、Boolean、Number、String。  
1种复杂数据类型：Object。  

**typeof**  
对一个值使用typeof操作符将返回字符串：  

* "undefined"  
* "boolean"  
* "string"  
* "number"  
* "object"      如果是对象或null  
* "function"    如果是函数  

#### Undefined 
Undefined只有一个值，在使用var声明变量但未对其加以初始化时，这个变量的值就是Undefinded。如果一个变量_未_声明，直接判断`变量===undefined`，会抛出ReferenceError，如果使用`typeof 变量==="undefined"`则不会有问题。所以声明未初始化和未声明还是有一些不同。  

#### Null
Null只有一个值，从逻辑角度讲，null值表示一个空指针。如果定义的变量将来准备用来保存对象，最好初始化为null而不是其他值。  

#### Boolean
Boolean只有两个值：true和false。注意true不一定等于1，false不一定等于0。要将一个值转换为Boolean可以用`Boolean()`或`!!`操作。其他类型转换为Boolean的规则：

* String        任何非空字符转换为true，空字符转换为false  
* Number        0和NaN转换为false，其余为true  
* Object        null转换为false，其余任何对象转换为true  
* Undefined     转换为false

#### Number
Number类型使用IEEE754表示，所以浮点数计算可能会丢失精度。  
一般表示十进制数；如果以0开头表示八进制数，如`var octnum = 070`，但八进制在严格模式下是无效的；如果以0x开头则是十六进制数，如`var hexnum = 0x1f`。  

*浮点数*，小数点后面至少有一个数字，虽然小数点前面可以没有数字，但不推荐。对于极大或极小的数字，可以使用e表示法，表示前面的数乘以10的后面的数的次幂。

*数值范围*，最小值为`Number.MIN_VALUE`，如果低于这个值则自动转换为`-Infinity`；最大值为`Number.MAX_VALUE`，如果高于这个值则自动转换为`Infinity`。并且可以使用`isFinite()`判断是否是非无穷。另外也可以用`Number.NEGETIVE_INFINITY`和`Number.POSITIVE_INFINITY`分别表示正负无穷。   

*NaN*，是一个特殊值，表示非数字，用于返回值而不抛出错误。任何涉及NaN的运算都会返回NaN，NaN与任何值都不相等，包括NaN本身。`0/0`等于NaN，但是`正数/0`等于Infinity，`负数/0`等于-Infinity。使用`isNaN()`是否不是一个数，即NaN或不能转换为数值的值。  

*数值转换*，有三个函数`Number()`、`parseInt()`、`parseFloat()`。  
Number函数的转换规则：

数据类型 | 转换规则  
-------- | ----------   
Boolean  | true=>1,false=>0  
null     | 0    
undefined | NaN   
String   | "1"=>1,"00011"=>11,"1.1"=>1.1,"0xf"=>15,""=>0,否则NaN   

Number()转换得不太合理，所以推荐使用parseInt()。它会忽略字符串前面的空格，直到找到第一个非空字符，如果第一个字符不是数字或负号，返回NaN，与Number()不同，空字符串将返回NaN。然后往后解析，遇到非数字将结束，包括点号。转换规则：  

* Boolean/null/undefined都返回NaN
* "1234blue"-->1234  
* ""-->NaN    
* "0xa"-->10    
* 22.5-->22    
* "070"-->56,注意在ECMAScript 5中不在支持八进制，结果将会是70。  

为了消除歧义，可以使用第二个参数指定进制。比如`var num = parsetInt("0xAF", 16)`或`var num = parsetInt("AF", 16)`，结果都是175。推荐做法是任何时候都指定进制。  
而parseFloat则是转换浮点数的函数，字符串的第一个小数点是有效的，但第二个小数点则无效。另外始终按十进制格式解析，所以没有第二个参数。    

#### String
String类型用于表示由0或多个16位Unicode字符组成的字符序列。用单、双引号都可以，但必须匹配。字符串是不可变的，所为字符串拼接，只是销毁原字符串，赋予新的字符串。  

*转义字符*，如`\n`、`\t`、`\\`、`\xnn`、`\unnn`。  

*转换为字符串*，第一种方法是使用自身的toString()方法，但null和undefined没有这个方法。使用数值的toString()方法时，可以以进制为参数，比如`num.toString(2)`，则输出二进制值。第二种方法是使用String()方法，可以避免不知道是否是null或undefined，导致产生错误。唯一区别就是null则返回"null"，undefined则返回"undefined"，其他都与toString()一样。


