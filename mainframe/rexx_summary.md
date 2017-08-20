# REXX总结
REXX是REstructured eXtended eXecutor的缩写。REXX是一种主要用于主机上的交互式语言，可以方便运行TSO或MVS其他地址空间中的命令。  
REXX能够与TSO/MVS/ISPF进行交互。一个REXX程序称为EXEC。  

**优点**  

* 容易读写  
* 书写自由(相对COBOL)  
* 大量方便的内部函数  
* 方便调试  

REXX由下面几部分组成：  

1. 指令(instruction)  
2. 内置函数(BIF)  
3. 外部函数(TSO/E external functions)  
4. 数据栈函数(Data stack functions)  

可以使用编译后的程序，主要可以提高性能。  
可以使用¬表示not，也可以使用\，为方便输入使用\。  

如果不在TSO/E地址空间中运行(比如通过JCL运行)，不能使用TSO/E命令、ISPF命令、ISPF/PDF edit命令。  
屏幕相关的命令则与特殊DD关联：  

* SAY - SYSTSPRT  
* PULL - SYSTSIN  
* TRACE - SYSTSPRT  
* PARSE EXTERNAL - SYSTSIN  

## 概念

### 使用REXX
1. 把REXX程序放在SYSPROC或SYSEXEC指定的PDS下面，使用`TSO MEMBER名`调用。  
    可以使用下面的REXX设置：  
    ```
    /****************************** REXX *******************************/
    /* This exec is an example of how to allocate a private PDS named  */
    /* USERID.REXX.EXEC to the beginning of a concatenation to SYSEXEC */
    /* that consists of one other data set named 'ISP.PHONE.EXEC'.  To */
    /* make sure that SYSEXEC is available, the exec issues EXECUTIL   */
    /* SEARCHDD(yes) command.  After the ALLOCATE command executes, a  */
    /* message indicates whether the command was successful or not.    */
    /*******************************************************************/
      "EXECUTIL SEARCHDD(yes)"   /* to ensure that SYSEXEC is available*/
    
      "ALLOC FILE(SYSEXEC) DATASET(rexx.exec,",
             "'isp.phone.exec') SHR REUSE"
    
      IF RC = 0 THEN
        SAY 'Allocation to SYSEXEC completed.'
      ELSE
        SAY 'Allocation to SYSEXEC failed.'
    ```
2. 使用`TSO EX 'PDS名(MEMBER名)'`调用。  
3. 使用ALTLIB建立EXEC查找的位置`TSO ALTLIB ACTIVATE APPLICATION(exec) DATASET(new.exec)`

## 语法
使用`/*`作为注释的开始，`*/`作为注释的结束。  
为了和CLIST作区分，第一行必须是带有REXX字样的注释。  
关键字及变量不区分大小写，但用引号括起来的字符串区分大小写。  
字符串的写法与python的普通字符串类似。  
通常一行写一条指令，但也可以用逗号表示下一行为续行，用分号都示语句结束从而在一行写多条语句。  
REXX不强制要求指令从第几列开始。  
使用`||`拼接字符串，不在中间添加空格。  

指令类型：  
1. 关键字(Keyword)，如`SAY`。  
2. 赋值(Assignment)，如`number = 4;number = 4 + 4`。  
3. 标签(Label)，如`Game1:`，可以使用SIGNAL指令跳转。  
4. 空行或注释。  
5. 命令(Command)。  

变量无需指明特定的类型，使用之前先赋值，比如`big = 10000000`。  
变量名最长可以有250个字符，不能是数字开头，可以是字母、数字或特殊字符`@#$?!._`。  
数据类型有：  

* 整数 - 比如12  
* 小数 - 比如12.5  
* 科学计数法 - 比如1.25E2  
* 字符串 - '123'  

### 表达式

#### 数学运算
数学运算符有：  

* \+ - 加  
* \- - 减  
* \* - 乘  
* / - 除，如`7 / 2 -> 3.5`  
* % - 返回商的整数，如`7 % 2 -> 3`    
* // - 返回余数，如`7 // 2 -> 1`    
* \*\* - 返回幂 

例子：  
```
 PARSE ARG cost percent_tax

 total = cost + (cost * percent_tax)       /* Add tax to cost.     */
 price = ((total * 100 + .5) % 1) / 100    /* Round and remove     */
                                           /* extra decimal places.*/
 SAY 'Your total cost is $'price'.'
```

注意加法并不是字符串拼接，只能用于数值。  

#### 比较运算
与C++类似，1表示true，0表示false。  

比较运算符：  

* == - 严格相等  
* = - 相等  
* \== - 不严格相等  
* \= - 不相等  
* > - 大于  
* < - 小于  
* >< - 不相等  
* >= - 大于等于  
* \< - 不小于  
* <= - 小于等于  
* \> - 不大于  

严格相等表示前后空格和大小写都是一致，而相等则不要求。  

例子：  
```
'WORD' = word              /* returns 1 */
'word ' \== word           /* returns 1 */
'word' == 'word'           /* returns 1 */
 4e2 \== 400               /* returns 1 */
 4e2 \= 100                /* returns 1 */
```

#### 逻辑运算
逻辑运算与比较运算类似，都返回1或0。  

逻辑运算符：  

* & - 与运算  
* | - 包容性或运算，最少一个为真时为1  
* && - 排除性或运算，仅有一个为真时为1  
* \ - 非  

例子：  
```
 PARSE ARG season snowing broken_leg

 IF ((season = 'winter') | (snowing ='yes')) & (broken_leg ='no')
    THEN SAY 'Go skiing.'
 ELSE
    SAY 'Stay home.'
	
IF ((season = 'winter') | (snowing ='yes')) & (broken_leg ='no') THEN
      \______________/      \____________/       \_____________/
           false                 true                 true
              \___________________/                    /
                     true                             /
                       \_____________________________/
                                     true
```

#### 连接运算符
有三种方式连接：  
1. 使用空格分开，连接之后的字符串中间会有一个空格  
2. 使用||拼接，连接之后的字符串中间不会有空格  
2. 直接写在一起，连接之后的字符串中间不会有空格。注意不能连接两个变量，这样会认为是一个变量名。  
```
SAY true      blue  /* result is TRUE BLUE */
(8 / 2)||(3 * 3)    /* result is 49        */
per_cent'%'         /* if per_cent = 50, result is 50%  */
```

## 指令

### 调试
使用TRACE指令进行调式，后面的指令就会被跟踪，并在屏幕上输出详细步骤。   
TRACE的选项(后面有详细解释)：  

* TRACE I - TRACE Intermediates，显示详细的中间步骤。  
* TRACE R - TRACE Result，只显示每一指令的结果。    

显示的信息：  

* >V> - Variable value  
* >L> - Literal value  
* >O> - Operation result  

例子：  
```
000001 /************************* REXX *****************************/
000002 /*  This exec uses the TRACE instruction to show how an     */
000003 /*  expression is evaluated, operation by operation.        */
000004 /*********************************************************  */
000005  x = 9
000006  y = 2
000007  TRACE I
000008
000009  IF x + 1 > 5 * y THEN
000010     SAY 'x is big enough.'
000011  ELSE NOP                  /* No operation on the ELSE path */
运行之后在屏幕显示
9 *-* IF x + 1 > 5 * y
    >V>   "9"
    >L>   "1"
    >O>   "10"
    >L>   "5"
    >V>   "2"
    >O>   "10"
    >O>   "0"
```

### 流程控制

#### 条件分支
使用`IF/THEN/ELSE`进行两路条件分支控制。  
可以使用`NOP`表示没有操作。使用`DO...END`表示一个语句块。IF语句可以嵌套。    
格式：  
```
IF expression THEN instruction
              ELSE instruction
IF expression THEN
   instruction
ELSE
   instruction
IF expression
  THEN
    instruction
  ELSE
    instruction
IF expression THEN instruction; ELSE instruction
IF expression THEN
   instruction
ELSE NOP
```

例子：  
```
IF weather = rainy THEN
   SAY 'Find a good book.'
ELSE
   DO
     SAY 'Would you like to play tennis or golf?'
     PULL answer
   END
   
 weather = 'fine'
 tenniscourt = 'occupied'

 IF weather = 'fine' THEN
    SAY 'What a lovely day!'
    IF tenniscourt = 'free' THEN
       SAY 'Shall we play tennis?'
 ELSE
    SAY 'Shall we take our raincoats?'
```

使用`SELECT/WHEN/OTHERWISE/END`进行多路条件分支控制。类似COBOL的EVALUATE。  
格式：  
```
SELECT
   WHEN  expression  THEN  instruction
   WHEN  expression  THEN  instruction
   WHEN  expression  THEN  instruction
   OTHERWISE
     instruction(s)
END
```

例子：  
```
 PARSE ARG age sex .

 SELECT
   WHEN age < 5 THEN                /* person younger than 5       */
     status = 'BABY'
   WHEN age < 13 THEN               /* person between 5 and 12     */
     DO
       IF sex = 'M' THEN            /* boy between 5 and 12        */
          status = 'BOY'
       ELSE                         /* girl between 5 and 12       */
          status = 'GIRL'
     END
   WHEN age < 20 THEN               /* person between 13 and 19    */
     status = 'TEENAGER'
   OTHERWISE
     IF sex = 'M' THEN              /* man 20 or older             */
       status = 'MAN'
     ELSE                           /* woman 20 or older           */
       status = 'WOMAN'
 END
```

#### 循环
循环又分为重复性循环(repetitive loops)和条件性循环(conditional loops)。循环是可以嵌套的。  

**重复性循环**  
循环的次数是固定的。  
例子：  
```
/* 重复性循环，循环5次 */
DO 5
  SAY 'Hello!'
END

/* 重复性循环，循环5次，使用number作为控制变量 */
DO number = 1 TO 5
  SAY 'Loop' number
  SAY 'Hello!'
END

/* 重复性循环，循环5次，使用number作为控制变量，每次增长2 */
DO number = 1 TO 10 BY 2
  SAY 'Loop' number
  SAY 'Hello!'
END
```

**无限循环**  
可以使用`DO FOREVER`进行无限循环。可以使用`EXIT`或`LEAVE`跳出循环。  

例子：  
```
   DO FOREVER
     SAY 'Enter the name of the next data set or a blank to end.'
     PULL dataset_name
     IF dataset_name = '' THEN
       EXIT
     ELSE
       DO
        "PRINTDS DA("dataset_name")"
         SAY dataset_name 'printed.'
       END
   END
```

**LEAVE**  
LEAVE跳出循环，执行END之后的语句。类似Java中的break。  

例子：  
```
   DO FOREVER
     SAY 'Enter the name of the next data set.'
     SAY 'When there are no more data sets, enter QUIT.'
     PULL dataset_name
     IF dataset_name = 'QUIT' THEN
        LEAVE
     ELSE
        DO
          "PRINTDS DA("dataset_name")"
           SAY dataset_name 'printed.'
        END
   END
   SAY 'Good-bye.'
```

**ITERATE**  
ITERATE不执行剩余的步骤从DO开始执行。类似Java中的continue。  

例子：  
```
DO count = 1 TO 10
  IF count = 8
    THEN
      ITERATE
    ELSE
      SAY 'Number' count
END
```

**条件性循环**  
根据条件决定循环什么时候结束。  
格式：  
```
/* 类似java中的while，当条件为真是继续执行循环体 */ 
DO WHILE  expression                    /* expression must be true */
   instruction(s)
END
/* 类似java中的do-while，循环体至少执行一次 */ 
DO UNTIL  expression                   /* expression must be false */
   instruction(s)
END
```

例子：  
```
  SAY 'Enter the name of a data set to print.'
  SAY 'If there are no data sets, enter QUIT.'
  PULL dataset_name
  DO WHILE dataset_name \= 'QUIT'
     "PRINTDS DA("dataset_name")"
     SAY dataset_name 'printed.'
     SAY 'Enter the name of the next data set.'
     SAY 'When there are no more data sets, enter QUIT.'
     PULL dataset_name
  END
  SAY 'Good-bye.'
  
   password = 'abracadabra'
   time = 0
   DO UNTIL (answer = password) | (time = 3)
     SAY 'What is the password?'
     PULL answer
     time = time + 1
   END
```

**组合循环**  
可以组合重复性循环和条件性循环。只要其中一个满足结束循环的条件，循环就会结束。  

例子：  
```
quantity = 20
DO number = 1 TO 10 WHILE quantity < 50
  quantity = quantity + number
  SAY 'Quantity = 'quantity '  (Loop 'number')'
END

quantity = 20
DO number = 1 TO 10 UNTIL quantity > 50
  quantity = quantity + number
  SAY 'Quantity = 'quantity '  (Loop 'number')'
END
```

#### 中断语句
**EXIT**  
EXIT会导致一个EXEC结束并返回调用它的地方。  
EXIT可以返回一个值给调用程序，如果是作为subroutine调用的，通过特殊变量RESULT返回，
如果是作为function调用的，通过特殊变量RC返回。  

例子：  
```
SAY 'How many months of experience do you have?  Please enter'
SAY 'the months as a number.'
PULL month

SAY 'Can you supply 3 references?  Please answer Y or N.'
PULL reference

SAY 'Are you available to start work tomorrow?  Please answer Y or N.'
PULL tomorrow

IF (month > 24) & (reference = 'Y') & (tomorrow = 'Y') THEN
  job = 1                                   /* person gets the job */
ELSE
  job = 0                           /* person does not get the job */

EXIT job
```

**CALL/RETURN**  
使用CALL会中断当前，而把控制交给内部或外部subroutine。  
RETURN则把控制交还给调用的ROUTINE。  

例子：  
```
...
CALL SUB1	<== 1
...			<== 5
EXIT		<== 6

SUB1:		<== 2
...			<== 3
RETURN		<== 4

REXX.EXEC(MAIN):
	...
	CALL SUB1	<== 1
	...			<== 4
	EXIT		<== 5
REXX.EXEC(SUB1):
	...			<== 2
	RETURN		<== 3
```

**SIGNAL**  
SIGNAL类似于CALL，跳转到一个标签，但与CALL不同的是，SIGNAL不会再控制权交回到调用的ROUTINE。  

例子：  
```
IF expression THEN
   SIGNAL Emergency	<== 1
ELSE 
   instructions
   
Emergency:			<== 2
   ...				<== 3
```

### Subroutines and Functions
子程序与函数是一些指令，用于接收数据，处理数据，返回数据。  
可以是内部的(使用label作为开始)，与可以是外部的(作为PDS的MEMBER)。  
子程序与函数有不同：  

1. 调用的方式
    ```
    CALL subroutine_name arg1, arg2, arg3
    x = function_name(arg1, arg2, arg3)
    
    ```
2. 返回值的方式不同。  
    ```
    子程序：
    	使用RETURN返回
    	RETURN value
    	
    	使用RESULT特殊变量接收
    	SAY 'The answer is' RESULT
    
    函数：
    	使用RETURN或EXIT返回
    	RETURN value / EXIT value
    	直接通过赋值接收
    	x = function(argument1, argument2,...)
    ```

如果返回值可有可无则使用子程序，如果需要作为表达式一部分则使用函数。  

**保护变量**   
使用PROCEDURE指令可以使subroutine中的变量变为local的，不与被调用的routine共享。  
此外可以其后加`EXPOSE 变量名`暴露某个或几个变量。  
```
 number1 = 10
 CALL subroutine
 SAY number1 number2                      /* displays  10  NUMBER2 */
 EXIT

 subroutine: PROCEDURE
 number1 = 7
 number2 = 5
 RETURN
 
 number1 = 10
 CALL subroutine
 SAY number1 number2                        /* displays 7  NUMBER2 */
 EXIT

 subroutine: PROCEDURE EXPOSE number1
 number1 = 7
 number2 = 5
 RETURN
```

**传参**  
使用ARG指令按顺序接收参数，也可以使用ARG内置函数。  
```
CALL SUB1 A, B, C
FUNC1(A, B, C)

ARG X, Y, Z

X = ARG(1)
Y = ARG(2)
```

**返回值**  
通过RETURN返回值，可以返回多个值。  
```
RETURN value1 value2 value3
```

REXX中有三种函数：  

1. BIF  
2. 自定义函数  
3. 函数包(Function packages)  

例子：  
```
REXX.EXEC(MAX):
 PARSE ARG number1, number2, number3 .

 IF number1 > number2 THEN
    IF number1 > number3 THEN
       greatest = number1
    ELSE
       greatest = number3
 ELSE
    IF number2 > number3 THEN
       greatest = number2
    ELSE
       greatest = number3

 RETURN greatest
 
REXX.EXEC(TESTMAX):
 biggest = MAX(45,-2,199)
 SAY biggest
```

**BIF**  
REXX中内置了很多有用的函数。  
*数学函数*：  

* ABS	Returns the absolute value of the input number.  
* DIGITS	Returns the current setting of NUMERIC DIGITS.  
* FORM	Returns the current setting of NUMERIC FORM.  
* FUZZ	Returns the current setting of NUMERIC FUZZ.  
* MAX	Returns the largest number from the list specified, formatted according to the current NUMERIC settings.  
* MIN	Returns the smallest number from the list specified, formatted according to the current NUMERIC settings.  
* RANDOM	Returns a quasi-random, non-negative whole number in the range specified.  
    格式：  
    ```
    >>-RANDOM(--+----------------------------+--)------------------><
                +-max------------------------+      
                '-+-min,-+-+-----+-+-------+-'      
                  '-,----' '-max-' '-,seed-' 
                  
    ```
    是闭区间，默认为0~999。  
* SIGN	Returns a number that indicates the sign of the input number.  
* TRUNC	Returns the integer part of the input number, and optionally a specified number of decimal places.  
    格式：  
    ```
    >>-TRUNC(number--+----+--)-------------------------------------><
                     '-,n-'  
                     
    ```
    n为截断的小数位，默认为0。  

*比较函数*：

* COMPARE	Returns 0 if the two input strings are identical. Otherwise, returns the position of the first character that does not match.  
* DATATYPE	Returns a string indicating the input string is a particular data type, such as a number or character.  
    格式：  
    ```
    >>-DATATYPE(string--+-------+--)-------------------------------><
                        '-,type-'
    ```
    如果是数值返回NUM，如果是字符串返回CHAR。  
    例子：  
    ```
    DATATYPE(' 12 ')         ->   'NUM'
    DATATYPE('')             ->   'CHAR'
    DATATYPE('123*')         ->   'CHAR'
    DATATYPE('12.3','N')     ->    1
    DATATYPE('12.3','W')     ->    0
    DATATYPE('Fred','M')     ->    1
    DATATYPE('','M')         ->    0
    DATATYPE('Fred','L')     ->    0
    DATATYPE('?20K','s')     ->    1
    DATATYPE('BCd3','X')     ->    1
    DATATYPE('BC d3','X')    ->    1
    
    ```
* SYMBOL	Returns this state of the symbol (variable, literal, or bad).  

*转换函数*：  

* B2X	Returns a string, in character format, that represents the input binary string converted to hexadecimal. (Binary to hexadecimal)  
    例子：  
    ```
    B2X('11000011')    ->   'C3'
    B2X('10111')       ->   '17'
    B2X('101')         ->   '5'
    B2X('1 1111 0000') ->   '1F0'
    
    ```
* C2D	Returns the decimal value of the binary representation of the input string. (Character to Decimal)  
* C2X	Returns a string, in character format, that represents the input string converted to hexadecimal. (Character to Hexadecimal)  
* D2C	Returns a string, in character format, that represents the input decimal number converted to binary. (Decimal to Character)  
* D2X	Returns a string, in character format, that represents the input decimal number converted to hexadecimal. (Decimal to Hexadecimal)  
* X2B	Returns a string, in character format, that represents the input hexadecimal string converted to binary. (Hexadecimal to binary)  
* X2C	Returns a string, in character format, that represents the input hexadecimal string converted to character. (Hexadecimal to Character)  
* X2D	Returns the decimal representation of the input hexadecimal string. (Hexadecimal to Decimal)  

*格式化函数*：  

* CENTER/ CENTRE	Returns a string of a specified length with the input string centered in it, with pad characters added as necessary to make up the length.  
    例子：  
    ```
    CENTER(abc,7)               ->    '  ABC  '
    CENTER(abc,8,'-')           ->    '--ABC---'
    CENTRE('The blue sky',8)    ->    'e blue s'
    CENTRE('The blue sky',7)    ->    'e blue '
    
    ```
* COPIES	Returns the specified number of concatenated copies of the input string.  
    例子：  
    ```
    COPIES('abc',3)    ->    'abcabcabc'
    COPIES('abc',0)    ->    ''
    
    ```
* FORMAT	Returns the input number, rounded and formatted.  
    例子：  
    ```
    FORMAT('3',4)            ->    '   3'
    FORMAT('1.73',4,0)       ->    '   2'
    FORMAT('1.73',4,3)       ->    '   1.730'
    FORMAT('-.76',4,1)       ->    '  -0.8'
    FORMAT('3.03',4)         ->    '   3.03'
    FORMAT(' - 12.73',,4)    ->    '-12.7300'
    FORMAT(' - 12.73')       ->    '-12.73'
    FORMAT('0.000')          ->    '0'
    FORMAT('12345.73',,,2,2)    ->    '1.234573E+04'
    FORMAT('12345.73',,3,,0)    ->    '1.235E+4'
    FORMAT('1.234573',,3,,0)    ->    '1.235'
    FORMAT('12345.73',,,3,6)    ->    '12345.73'
    FORMAT('1234567e5',,3,0)    ->    '123456700000.000'
    
    ```
* JUSTIFY *	Returns a specified string formatted by adding pad characters between words to justify to both margins.  
    例子：  
    ```
    JUSTIFY('The blue sky',14)       ->    'The  blue  sky'
    JUSTIFY('The blue sky',8)        ->    'The blue'
    JUSTIFY('The blue sky',9)        ->    'The  blue'
    JUSTIFY('The blue sky',9,'+')    ->    'The++blue'
    
    ```
* LEFT	Returns a string of the specified length, truncated or padded on the right as needed.  
    例子：  
    ```
    LEFT('abc d',8)        ->    'abc d   '
    LEFT('abc d',8,'.')    ->    'abc d...'
    LEFT('abc  def',7)     ->    'abc  de'
    
    ```
* RIGHT	Returns a string of the specified length, truncated or padded on the left as needed.  
* SPACE	Returns the words in the input string with a specified number of pad characters between each word.  

*字符串处理函数*：  

* ABBREV	Returns a string indicating if one string is equal to the specified number of leading characters of another string.  
* DELSTR	Returns a string after deleting a specified number of characters, starting at a specified point in the input string.  
    例子：  
    ```
    DELSTR('abcd',3)       ->    'ab'
    DELSTR('abcde',3,2)    ->    'abe'
    DELSTR('abcde',6)      ->    'abcde'
    
    ```
* DELWORD	Returns a string after deleting a specified number of words, starting at a specified word in the input string.  
    例子：  
    ```
    DELWORD('Now is the  time',2,2)  ->  'Now time'
    DELWORD('Now is the time ',3)    ->  'Now is '
    DELWORD('Now is the  time',5)    ->  'Now is the  time'
    DELWORD('Now is   the time',3,1) ->  'Now is   time'
    
    ```
* FIND *	Returns the word number of the first word of a specified phrase found within the input string.  
    例子：  
    ```
    FIND('now is the time','is the time')    ->    2
    FIND('now is  the time','is    the')     ->    2
    FIND('now is  the time','is  time ')     ->    0
    
    ```
* INDEX *	Returns the character position of the first character of a specified string found in the input string.  
    例子：  
    ```
    INDEX('abcdef','cd')      ->    3
    INDEX('abcdef','xd')      ->    0
    INDEX('abcdef','bc',3)    ->    0
    INDEX('abcabc','bc',3)    ->    5
    INDEX('abcabc','bc',6)    ->    0
    
    ```
* INSERT	Returns a character string after inserting one input string into another string after a specified character position.  
    例子：  
    ```
    INSERT(' ','abcdef',3)         ->    'abc def'
    INSERT('123','abc',5,6)        ->    'abc  123   '
    INSERT('123','abc',5,6,'+')    ->    'abc++123+++'
    INSERT('123','abc')            ->    '123abc'
    INSERT('123','abc',,5,'-')     ->    '123--abc'
    
    ```
* LASTPOS	Returns the starting character position of the last occurrence of one string in another.  
    例子：  
    ```
    LASTPOS(' ','abc def ghi')      ->    8
    LASTPOS(' ','abcdefghi')        ->    0
    LASTPOS('xy','efgxyz')          ->    4
    LASTPOS(' ','abc def ghi',7)    ->    4
    
    ```
* LENGTH	Returns the length of the input string.  
    例子：  
    ```
    LENGTH('abcdefgh')    ->    8
    LENGTH('abc defg')    ->    8
    LENGTH('')            ->    0
    
    ```
* OVERLAY	Returns a string that is the target string overlaid by a second input string.  
    例子：  
    ```
    OVERLAY(' ','abcdef',3)         ->    'ab def'
    OVERLAY('.','abcdef',3,2)       ->    'ab. ef'
    OVERLAY('qq','abcd')            ->    'qqcd'
    OVERLAY('qq','abcd',4)          ->    'abcqq'
    OVERLAY('123','abc',5,6,'+')    ->    'abc+123+++'
    
    ```
* POS	Returns the character position of one string in another.  
    例子：  
    ```
    POS('day','Saturday')       ->    6
    POS('x','abc def ghi')      ->    0
    POS(' ','abc def ghi')      ->    4
    POS(' ','abc def ghi',5)    ->    8
    
    ```
* REVERSE	Returns a character string, the characters of which are in reverse order (swapped end for end).  
    例子：  
    ```
    REVERSE('ABc.')    ->    '.cBA'
    REVERSE('XYZ ')    ->    ' ZYX'
    
    ```
* STRIP	Returns a character string after removing leading or trailing characters or both from the input string.  
    例子：  
    ```
    STRIP('  ab c  ')        ->    'ab c'
    STRIP('  ab c  ','L')    ->    'ab c  '
    STRIP('  ab c  ','t')    ->    '  ab c'
    STRIP('12.7000',,0)      ->    '12.7'
    STRIP('0012.700',,0)     ->    '12.7'
    
    ```
* SUBSTR	Returns a portion of the input string beginning at a specified character position.  
    例子：  
    ```
    SUBSTR('abc',2)          ->    'bc'
    SUBSTR('abc',2,4)        ->    'bc  '
    SUBSTR('abc',2,6,'.')    ->    'bc....'
    
    ```
* SUBWORD	Returns a portion of the input string starting at a specified word number.  
    例子：  
    ```
    SUBWORD('Now is the  time',2,2)    ->    'is the'
    SUBWORD('Now is the  time',3)      ->    'the  time'
    SUBWORD('Now is the  time',5)      ->    ''
    
    ```
* TRANSLATE	Returns a character string with each character of the input string translated to another character or unchanged.  
    例子：  
    ```
    TRANSLATE('abcdef')                    ->    'ABCDEF'
    TRANSLATE('abbc','&','b')              ->    'a&&c'
    TRANSLATE('abcdef','12','ec')          ->    'ab2d1f'
    TRANSLATE('abcdef','12','abcd','.')    ->    '12..ef'
    TRANSLATE('APQRV',,'PR')               ->    'A Q V'
    TRANSLATE('APQRV',XRANGE('00'X,'Q'))   ->    'APQ  '
    TRANSLATE('4123','abcd','1234')        ->    'dabc'
    
    ```
* VERIFY	Returns a number indicating whether an input string is composed only of characters from another input string or returns the character position of the first unmatched character.  
* WORD	Returns a word from an input string as indicated by a specified number.  
    例子：  
    ```
    WORD('Now is the time',3)    ->    'the'
    WORD('Now is the time',5)    ->    ''
    
    ```
* WORDINDEX	Returns the character position in an input string of the first character in the specified word.  
例子：  
    ```
    WORDINDEX('Now is the time',3)    ->    8
    WORDINDEX('Now is the time',6)    ->    0
    
    ```
* WORDLENGTH	Returns the length of a specified word in the input string.  
    例子：  
    ```
    WORDLENGTH('Now is the time',2)       ->    2
    WORDLENGTH('Now comes the time',2)    ->    5
    WORDLENGTH('Now is the time',6)       ->    0
    
    ```
* WORDPOS	Returns the word number of the first word of a specified phrase in the input string.  
    例子：  
    ```
    WORDPOS('the','now is the time')              ->  3
    WORDPOS('The','now is the time')              ->  0
    WORDPOS('is the','now is the time')           ->  2
    WORDPOS('is   the','now is the time')         ->  2
    WORDPOS('is   time ','now is   the time')     ->  0
    WORDPOS('be','To be or not to be')            ->  2
    WORDPOS('be','To be or not to be',3)          ->  6
    
    ```
* WORDS	Returns the number of words in the input string.  
    例子：  
    ```
    WORDS('Now is the time')    ->    4
    WORDS(' ')                  ->    0
    
    ```

*其它函数*：  

* ADDRESS	Returns the name of the environment to which commands are currently being sent.  
* ARG	Returns an argument string or information about the argument strings to a program or internal routine.  
* BITAND	Returns a string composed of the two input strings logically ANDed together, bit by bit.  
* BITOR	Returns a string composed of the two input strings logically ORed together, bit by bit.  
* BITXOR	Returns a string composed of the two input strings eXclusive ORed together, bit by bit.  
* CONDITION	Returns the condition information, such as name and status, associated with the current trapped condition.  
* DATE	Returns the date in the default format (dd mon yyyy) or in one of various optional formats.  
* ERRORTEXT	Returns the error message associated with the specified error number.  
* EXTERNALS *	Returns the number of elements in the terminal input buffer. In TSO/E, this function always returns a 0.  
* LINESIZE *	Returns the current terminal line width minus 1.  
* QUEUED	Returns the number of lines remaining in the external data queue at the time when the function is invoked.  
* SOURCELINE	Returns either the line number of the last line in the source file or the source line specified by a number.  
* TIME	Returns the local time in the default 24-hour clock format (hh:mm:ss) or in one of various optional formats.  
* TRACE	Returns the trace actions currently in effect.  
* USERID *	Returns the TSO/E user ID, if the REXX exec is running in the TSO/E address space.  
* VALUE	Returns the value of a specified symbol and optionally assigns it a new value.  
* XRANGE	Returns a string of all 1-byte codes (in ascending order) between and including specified starting and ending values.  

### 输入输出
**屏幕**  
使用SAY指令输出提示到屏幕。  
使用PULL指令接收屏幕输入，可以一次接收一个或接收多个(使用空格分开)。使用`PARSE PULL`防止转换为全大写。  

**参数**  
可以在执行EXEC时通过参数输入，比如`EXEC rexx.exec(add) '42 21' exec`，在程序中通过`ARG number1 number2`接收。  
如果执行时给的参数过少，会报错，如果过多，剩余的参数会全部接收到最后一个变量中。  
为防止过多参数可以写`ARG number1 number2 .`，这样会省略多出的参数。  
但`ARG . number1 number2`则是省略第一个参数。  
使用`PARSE ARG`防止转换为全大写。  

例子：  
```
Input
1. 115 -23 66 5.8
ARG first second third
2. .2 0 569 2E6
ARG first second third fourth
3. 13 13 13 13
ARG first second third fourth fifth
4. Weber Joe 91
ARG lastname firstname score
5. Baker Amanda Marie 95
PARSE ARG lastname firstname score
6. Callahan Eunice 88 62
PARSE ARG lastname firstname score

ANSWERS
first = 115, second = -23, third = 66 5.8
first = .2, second = 0, third = 569, fourth = 2E6
first = 13, second = 13, third = 13, fourth = 13, fifth = null
lastname = WEBER, firstname = JOE, score = 91
lastname = Baker, firstname = Amanda, score = Marie 95
lastname = Callahan, firstname = Eunice, score = 88
```

### 解释命令
使用INTERPRET将一个字符串解析为命令。  
```
action = 'DO 3; SAY "Hello!"; END'
INTERPRET action                               /* results in:
                                                          Hello!
                                                          Hello!
                                                          Hello!   */
```

### 处理数据

#### 组合变量(Compound Variable)
组合变量类似于一维数组。下标不需要是数字。使用点号来分隔层次。  
对于组合变量，**第一部分总是一个名字**，剩余部分会替换为前面定义的变量，如果前面没定义会，则替换为大写的名字。  
```
first = 'Fred'
last = 'Higgins'
employee = first.last
                             /* EMPLOYEE is assigned FIRST.Higgins */
SAY employee.first.middle.last
                          /* Displays EMPLOYEE.Fred.MIDDLE.Higgins */
						  
a = 3                              /* assigns '3' to variable 'A'  */
b = 4                              /*         '4' to          'B'  */
c = 'last'                         /*      'last' to          'C'  */
a.b = 2                            /*         '2' to         'A.4' */
a.c = 5                            /*         '5' to      'A.last' */
x.a.b = 'cv3d'                     /*      'cv3d' to       'X.3.4' */
```

在循环中使用：  
```
DO i = 1 TO 6
   SAY 'Enter an employee name.'
   PARSE PULL employee.i
END
```

#### Stem
Stem是组合变量的前面部分(第一部分及第一个点号)。  
比如：  
```
employee. = 'Nobody'	<== 则所有组合变量(未定义过)都将初始化为Nobody
SAY employee.5                          /* Displays 'Nobody'       */
SAY employee.10                         /* Displays 'Nobody'       */
SAY employee.oldest                     /* Displays 'Nobody'       */

employee.new = 'Clark, Evans'
SAY employee.new                        /* Displays 'Clark, Evans' */
```

#### 解析数据
PULL从data stack中读取数据，如果data stack为空则从屏幕读取数据。  
PULL可以接收多个数据，在前面加PARSE则不会转换为大写。  

使用`PARSE VAR`或`PARSE VALUE`把一个字符串拆分为多个字符串，默认以空格作为分隔标志。  
```
quote = 'Knowledge is power.'
PARSE VAR quote word1 word2 word3
                                    /* word1 contains 'Knowledge'  */
                                    /* word2 contains 'is'         */
                                    /* word3 contains 'power.'     */
PARSE VALUE 'Knowledge is power.' WITH word1 word2 word3
                                    /* word1 contains 'Knowledge'  */
                                    /* word2 contains 'is'         */
                                    /* word3 contains 'power.'     */
/* 以点号表示跳过 */
PARSE VALUE 'Value with Periods in it.' WITH  pattern . type .
                                /* pattern contains 'Value'        */
                                /* type contains 'Periods'         */
              /* the periods replace the words "with" and "in it." */
phrase = 'To be, or not to be?'       /* phrase containing comma   */
/* 以逗号分隔 */
PARSE VAR phrase part1 ',' part2      /* template containing comma */
                                      /*   as string separator     */
                                /* part1 contains 'To be'          */
                                /* part2 contains ' or not to be?' */
/* 以变量分隔 */
separator = ','
phrase = 'To be, or not to be?'
PARSE VAR phrase part1 (separator) part2
                                /* part1 contains 'To be'          */
                                /* part2 contains ' or not to be?' */
/* 根据位置拆分，无符号数字或=数字表示绝对定位 */
quote = 'Ignorance is bliss.'
         ....+....1....+....2

PARSE VAR quote part1 5 part2
                              /* part1 contains 'Igno'             */
                              /* part2 contains 'rance is bliss.'  */
quote = 'Ignorance is bliss.'
         ....+....1....+....2

PARSE VAR quote 1 part1 =5 part2
                              /* part1 contains 'Igno'             */
                              /* part2 contains 'rance is bliss.'  */
quote = 'Ignorance is bliss.'
         ....+....1....+....2

PARSE VAR quote 1 part1 10 11 part2 13 14 part3 19 1 part4 20
                           /* part1 contains 'Ignorance'           */
                           /* part2 contains 'is'                  */
                           /* part3 contains 'bliss'               */
                           /* part4 contains 'Ignorance is bliss.' */
/* 根据位置拆分，有符号数字表示相对定位，+表示往右，-表示往左 */
quote = 'Ignorance is bliss.'
         ....+....1....+....2

PARSE VAR quote part1 +5 part2 +5 part3 +5 part4
                                /* part1 contains 'Ignor'          */
                                /* part2 contains 'ance '          */
                                /* part3 contains 'is bl'          */
                                /* part4 contains 'iss.'           */
quote = 'Ignorance is bliss.'
         ....+....1....+....2

movex = 3                       /* variable position               */
PARSE VAR quote part5 +10 part6 +3 part7 -(movex) part8
                                /* part5 contains 'Ignorance '     */
                                /* part6 contains 'is '            */
                                /* part7 contains 'bliss.'         */
                                /* part8 contains 'is bliss.'      */
```

## TSO/E

### TSO/E 命令
当一个命令执行完后，通过特殊变量RC得到返回码。  
通过在单引号或双引号中书写命令的方式调用TSO/E命令，比如`"ALLOC DA(NEW.DATA) LIKE(OLD.DATA) NEW"`。  
变量不能直接在引号中使用，必须通过拼接的方式：  
```
name = "'project.rel1.new'"
"LISTDS" name "STATUS"

name = project.rel1.new
"LISTDS '"name"' STATUS"
```

### 是否允许提示
当输入的TSO/E命令不全，会有相应的提示的。可以设置是否提示。  
```
/* 打开提示 */
saveprompt = PROMPT('ON')
/* 关闭提示 */
saveprompt = PROMPT('OFF')
/* 调用时显示要求提示 */
EXEC mynew.exec(create) exec prompt
```

### 调用其他EXEC
使用EXEC命令显示调用其他EXEC。  
当其他EXEC位于SYSPROC/SYSEXEC定义的PDS中，可以通过%<member>隐式调用。  
```
"EXEC myrexx.exec(calc) '24 55 12 38' exec"
SAY 'The result is' RC

"%calc 24 55 12 38"
SAY 'The result is' RC
```

## 命令环境
Host Command Environment就是执行命令的环境。  
可用的环境有：  

* TSO  
* MVS  
* LINK  
* LINKMVS  
* LINKPGM  
* ATTACH  
* ATTCHMVS  
* ATTCHPGM  
* ISPEXEC  
* ISREDIT  
* CONSOLE  
* CPICOMM   
* LU62  
* APPCMVS  

### 改变命令环境
```
ADDRESS ispexec		/* 下面所有指令都在ispexec环境下执行 */

ADDRESS ispexec "edit DATASET("dsname")"	/* 只针对当前命令，不影响后面命令 */
```

### 检查命令环境
```
x = ADDRESS()	/* 得到当前的环境 */

ARG dsname
SUBCOM ISPEXEC	/* 使用SUBCOM检查该环境是否可用 */
IF RC=0 THEN
  ADDRESS ISPEXEC "SELECT PGM(ISREDIT)"    /* select ISPF/PDF edit */
ELSE
  "EDIT" dsname                        /* use TSO/E line mode edit */
```

### DEBUG

#### 使用TRACE
有两个命令，`TRACE C`，`TRACE E`。  
C表示command，所有的命令都会被跟踪。  
E表示error，只有命令返回非0的结果才会显示跟踪信息。  

#### 使用特殊变量RC和SIGL
RC为返回码，如果是正常的则为0。  
SIGL为控制权发生转换的位置(行号)，比如CALL语句。  

#### 交互式调试

**开始调试**  
使用?选项或`EXECUTIL TS`。  
```
TRACE ?R

当遇到TSO/E命令时会显示如下信息，按下回车就可以继续往下。
+++ Interactive trace.    TRACE OFF to end debug, ENTER to continue. +++
```

**调试操作**  
1. 当按下回车是继续执行。  
2. 输入其他指令。  
3. 输入=执行上一次指令。  

**结束调试**  
1. 使用`TRACE OFF`。  
2. 运行的末尾。  
3. 使用`TRACE ?`。  
4. 使用`EXECUTIL TE`。  

### TSO/E External Functions
除非TSO/E命令，还提供了一些有用的函数。  

#### LISTDSI
LISTDSI即list dataset information。  
```
x = LISTDSI("'proj5.rexx.exec'") 	/* x得到返回码，0表示正常，4表示有部分信息无法获取 */
SAY SYSVOLUME
```
通过特殊的变量得到DATASET的信息：  

* SYSDSNAME	Data set name  
* SYSVOLUME	Volume serial ID  
* SYSUNIT	Device unit on which volume resides  
* SYSDSORG	Data set organization: PS, PSU, DA, DAU, IS, ISU, PO, POU, VS  
* SYSRECFM	Record format; three-character combination of the following: U, F, V, T, B, S, A, M  
* SYSLRECL	Logical record length  
* SYSBLKSIZE	Block size  
* SYSKEYLEN	Key length  
* SYSALLOC	Allocation, in space units  
* SYSUSED	Allocation used, in space units  
* SYSUSEDPAGES	Used space of a partitioned data set extended (PDSE) in 4K pages.  
* SYSPRIMARY	Primary allocation in space units  
* SYSSECONDS	Secondary allocation in space units  
* SYSUNITS	Space units: CYLINDER, TRACK, BLOCK  
* SYSEXTENTS	Number of extents allocated  
* SYSCREATE	Creation date:  Year/day format, for example: 1985/102  
* SYSREFDATE	Last referenced date:  Year/day format, for example: 1985/107 (Specifying DIRECTORY causes the date to be updated.)  
* SYSEXDATE	Expiration date:  Year/day format, for example: 1985/365  
* SYSPASSWORD	Password indication: NONE, READ, WRITE  
* SYSRACFA	RACF? indication: NONE, GENERIC, DISCRETE  
* SYSUPDATED	Change indicator: YES, NO  
* SYSTRKSCYL	Tracks per cylinder for the unit identified in the SYSUNIT variable  
* SYSBLKSTRK	Blocks per track for the unit identified in the SYSUNIT variable  
* SYSADIRBLK	Directory blocks allocated - returned only for partitioned data sets when DIRECTORY is specified  
* SYSUDIRBLK	Directory blocks used - returned only for partitioned data sets when DIRECTORY is specified  
* SYSMEMBERS	Number of members - returned only for partitioned data sets when DIRECTORY is specified  
* SYSREASON	LISTDSI reason code  
* SYSMSGLVL1	First-level message if an error occurred  
* SYSMSGLVL2	Second-level message if an error occurred  
* SYSDSSMS	Information about the type of a data set provided by DFSMS/MVS.  
* SYSDATACLASS	SMS data class name  
* SYSSTORCLASS	SMS storage class name  
* SYSMGMTCLASS	SMS management class name  

#### MSG
对TSO/E信息的控制，如果MSG没使用，错误和非错误信息都会显示。  
```
status = MSG('OFF')    /* status is set to the previous setting of */
                       /*  MSG and sets the current setting to OFF */
```

#### MVSVAR
获取MVA的变量。比如：`appclu = MVSVAR('SYSAPPCLU')`。  
变量有：  

* SYSAPPCLU	the APPC/MVS logical unit (LU) name  
* SYSDFP	the level of MVS/Data Facility Product (MVS/DFP)  
* SYSMVS	the level of the base control program (BCP) component of z/OS?  
* SYSNAME	the name of the system your REXX exec is running on, as specified in the SYSNAME statement in SYS1.PARMLIB member IEASYSxx  
* SYSSECLAB	the security label (SECLABEL) name of the TSO/E session  
* SYSSMFID	identification of the system on which System Management Facilities (SMF) is active  
* SYSSMS	indicator whether DFSMS/MVS is available to your REXX exec  
* SYSCLONE	MVS system symbol representing its system name  
* SYSPLEX	the MVS sysplex name as found in the COUPLExx or LOADxx member of SYS1.PARMLIB  
* SYMDEF	symbolic variables of your MVS system  

#### OUTTRAP
把TSO/E命令输出的信息保存在stem中。  
```
X = OUTTRAP('VAR.')					/* 把信息保存在VAR.中 */                                                     
"LISTC"                                                                 
SAY 'THE NUMBER OF LINES TRAPPED IS' VAR.0    /* VAR.0保存的是总行数 */   
LINES = VAR.0 + 1                                                       
"LISTDS 'IBMUSER.BAT.SOURCE' MEMBERS"                                   
SAY 'THE NUMBER OF LINES TRAPPED IS' VAR.0  
DO I = LINES TO VAR.0                                                   
  SAY VAR.I                                  /* VAR.1 VAR.2 VAR.3... 依次保存输出的行 */   
END                                                                     

x = OUTTRAP('OFF')       /* 中止TRAP */
```

#### PROMPT
当输入的TSO/E命令不全，会有相应的提示的。可以设置是否提示。  
```
/* 打开提示 */
saveprompt = PROMPT('ON')
/* 关闭提示 */
saveprompt = PROMPT('OFF')
/* 调用时显示要求提示 */
EXEC mynew.exec(create) exec prompt
```

#### SYSDSN
使用SYSDSN判断一个DATA SET是否可用，如果可用，则返回`OK`。  
```
DO FOREVER
  SAY 'Enter a Data Set Name'
  PARSE UPPER PULL dsname
  IF SYSDSN(dsname) ¬= 'OK' THEN ITERATE
  FC = LISTDSI(dsname)
  IF SYSDSORG ¬= 'PO' THEN ITERATE
  SAY 'Okay: ' dsname 'is ' SYSDSORG
  LEAVE
END
```

其它信息：  

* MEMBER SPECIFIED, BUT DATASET IS NOT PARTITIONED  
* MEMBER NOT FOUND  
* DATASET NOT FOUND  
* ERROR PROCESSING REQUESTED DATASET  
* PROTECTED DATASET  
* VOLUME NOT ON SYSTEM  
* UNAVAILABLE DATASET  
* INVALID DATASET NAME, data-set-name:  
* MISSING DATASET NAME  

#### SYSVAR
SYSVAR用于获取系统信息。  
```
proc = SYSVAR(sysproc)
```

**用户信息**   

* SYSPREF	Prefix as defined in user profile  
* SYSPROC	SYSPROC returns the current procedure name (either the LOGON procedure name, the Started Task procedure name, or 'INIT' for a batch job). For more information, see z/OS TSO/E REXX Reference.  
* SYSUID	User ID of current session  

**终端信息**  

* SYSLTERM	Number of lines available on screen  
* SYSWTERM	Width of screen  

**语言**  

* SYSPLANG	Primary language for translated messages  
* SYSSLANG	Secondary language for translated messages  
* SYSDTERM	Whether DBCS is supported for this terminal  
* SYSKTERM	Whether Katakana is supported for this terminal  

**EXEC信息**  

* SYSENV	Whether exec is running in foreground or background  
* SYSICMD	Name by which exec was implicitly invoked  
* SYSISPF	Whether ISPF is available for exec  
* SYSNEST	Whether exec was invoked from another exec or CLIST. Invocation could be implicit or explicit.  
* SYSPCMD	Name of most recently executed command  
* SYSSCMD	Name of most recently executed subcommand  

**系统信息**  

* SYSCPU	Number of CPU seconds used during session in the form: seconds.hundredths of seconds  
* SYSHSM	Level of Data Facility Hierarchical Storage Manager (DFHSM) installed  
* SYSJES	Name and level of JES installed  
* SYSLRACF	Level of RACF? installed  
* SYSRACF	Whether RACF is available  
* SYSNODE	Network node name of the installation's JES  
* SYSSRV	Number of system resource manager (SRM) service units used during session  
* SYSTERMID	Terminal ID of the terminal where the REXX exec was started  
* SYSTSOE	Level of TSO/E installed in the form: version release modification_number  

**Console Session**   

* SOLDISP	Whether solicited messages (command responses) should be displayed at terminal  
* UNSDISP	Whether unsolicited messages should be displayed at terminal  
* SOLNUM	The number of solicited messages (command responses) to be held in message table  
* UNSNUM	The number of unsolicited messages to be held in message table  
* MFTIME	Whether time stamp should be displayed with messages  
* MFOSNM	Whether originating system name should be displayed with messages  
* MFJOB	Whether originating job name or job ID should be displayed with messages  
* MFSNMJBX	Whether system name and job name should be excluded from display of retrieved messages  

### Data Stack
data stack是TSO/E中用于保存信息的数据结构。data stack结合了栈和队列的特性。  
每个用户都有自己的data stack。可用于在不同EXEC之间传递数据。大小是没有限制的。  

#### 添加元素
使用PUSH添加元素到data stack的顶部。  
使用QUEUE添加元素到data stack的尾部。  
```
elem1 = 'String 1 for the data stack'
PUSH elem1

elemA = 'String A for the data stack'
QUEUE elemA
```

#### 删除元素
使用PULL或PARSE PULL从data stack的顶部移除元素，被移除的元素保存在变量中。 
```
PULL stackitem
PARSE PULL stackitem
SAY stackitem
``` 

#### 元素个数
使用QUEUED函数获得data stack中元素的个数。  
```
number = QUEUED()
DO number
  PULL element
  SAY element
END
```

#### Buffer
因为所有EXEC都是共用一个data stack，可以使用Buffer来对data stack进行扩展。  
Buffer是在data stack的顶部新开辟一个空间，所以QUEUE操作不会在原data stack尾部添加。  

**创建Buffer**  
使用MAKEBUF创建Buffer。  
```
'MAKEBUF'
PUSH 'newX'
QUEUE 'newY' /* 注意这里的QUEUE并不是添加在data stack尾部 */
```

**删除元素**  
同样使用PULL删除元素，但不会对Buffer下面的原data stack数据进行保护，一旦Buffer的元素被删除光了就会删除原data stack的元素。  

**删除Buffer**  
使用DROPBUF删除最近创建的Buffer，如果指定数字则删除特定的Buffer及其后创建的Buffer，
比如`DROPBUF 2`删除第2个创建的Buffer及其后创建的Buffer。如果使用`DROPBUF 0`则会清空data stack。  

**Buffer的数量**  
使用`QBUF`返回Buffer的数量。  
```
'MAKEBUF'

'MAKEBUF'

'QBUF'
SAY 'The number of buffers is' RC                        /* RC = 2 */
```

**Buffer中元素的数量**  
使用`QELEM`返回Buffer中元素的数量，不计算data stack中元素的数量。  
```
PUSH A
'MAKEBUF'
PUSH B
PUSH C
'QELEM'
SAY 'The number of elements is' RC                       /* RC = 2 */
```

#### 保护数据
可以创建一个私人的data stack，不会对原data stack产生影响。  

**NEWSTACK**  
使用NEWSTACK创建一个私人的data stack，除非使用DELSTACK，否则不会影响原data stack。  
```
PUSH 'oldA'
PUSH 'old1'
'NEWSTACK'
QUEUE 'newY'
PUSH 'newX'
```

**DELSTACK**  
删除私人的data stack，注意如果没有创建私人的data stack，将删除原始的data stack。  

**QSTACK**  
返回data stack的总数，包含原始的data stack。  
```
'NEWSTACK'

'NEWSTACK'

'QSTACK'
SAY 'The number of stacks is' RC                  /* RC contains 3 */
```

## Function Packages
Function Packages是一组子程序或函数，性能比外部子程序(external routines)要好。Function Packages是由程序语言写的。   
有三种Function Packages：  

* User packages，默认名字是IRXFUSER。   
* Local packages，默认名字是IRXFLOC。  
* System packages  

**函数的搜索顺序**  

* 内部函数  
* BIF  
* Function Packages  
* Load libraries  
* 外部函数  

## 文件处理
使用EXECIO命令处理文件，信息可以存在data stack中用于顺序或随机处理。  
文件必须为顺序文件或PDS的MEMBER，使用ALLOC命令对文件进行了分配。比如：  
```
"ALLOC DA(io.data) F(myindd) SHR REUSE"
"EXECIO  *  DISKR  myindd  (FINIS"
```

EXECIO能够做什么：  

* 读  
* 写  
* 打开文件  
* 清空文件  
* 复制一个文件的内容到另一个文件  
* 从组合变量中复制信息或拷贝信息到组合变量  
* 在顺序文件末尾添加信息  
* 更新文件中的记录  

格式：  
```
>>-EXECIO--+-lines-+-------------------------------------------->
           '-*-----'   

>--+-DISKW--ddname--+-----------------+---------------+--------><
   |                '-| Write Parms |-'               |   
   '-+-DISKR--+-ddname-+---------+-+----------------+-'   
     '-DISKRU-'        '-linenum-' '-| Read Parms |-'     

Write Parms

|--+--------------------------------------------------+---------|
   '-(--+---------------+--+------+--+-------+--+---+-'   
        '-STEM var-name-'  '-OPEN-'  '-FINIS-'  '-)-'     

Read Parms

|--+-------------------------------------------------------+----|
   '-(-+---------------+-+------+-+-------+-+------+-+---+-'   
       +-FIFO----------+ '-OPEN-' '-FINIS-' '-SKIP-' '-)-'     
       +-LIFO----------+                                       
       '-STEM var-name-' 
```

**读写**  

* DISKR - 只读  
* DISKRU - 读+更新  
* DISKW - 写  

**参数**  

* FINIS - 执行完命令之后就关闭文件  
* OPEN - 打开文件，比如`"EXECIO 0 DISKR myindd (OPEN"`。  
* STEM - 把记录读到STEM中或把STEM写到文件中，比如`"EXECIO  *  DISKR  myindd  (STEM newvar."`。如果想清空STEM，可以使用`DROP newvar`。
对于写操作，newvar.0没有什么用处。  
* SKIP - 跳过数行，不把它们读入data stack或STEM，比如`"EXECIO  24  DISKR  myindd  (SKIP"`。  
* LIFO - 后进先出。  
* FIFO - 先进先出，默认，无需指明。  

### 读文件
默认所有的记录都会读取到data stack中。  
```
"EXECIO * DISKR  myindd (FINIS"		/* 从myindd中读所有记录 */

"EXECIO 0  DISKR mydd (OPEN"		/* 0表示不读任何记录 */
"EXECIO 25 ..."						/* 25表示读25条记录 */
"EXECIO * ..."						/* *表示读所有记录 */

"EXECIO  5  DISKR  myindd  100  (FINIS"	/* 5表示读5条记录，100表示从第100行开始读 */
```

### 写文件
`*`会令持续往文件中写入，直接到遇到一个空行，如果data stack读完了都没有遇到空行，则会从屏幕读入。
这种情况下应该使用QUEUE往data stack中追加一个空行。或者使用QUEUED得到实际要写入的行数而不使用`*`也是一个方法。  
```
"ALLOC DA(out.data) F(myoutdd) OLD REUSE"
"EXECIO * DISKW myoutdd (FINIS"		/* 把所有记录写到文件myoutdd中 */
"EXECIO 0  DISKW  myoutdd … (OPEN"	/* 只是打开文件，不做任何写的动作 */
"EXECIO 0 DISKW myoutdd … (FINIS"	/* 清空文件，但文件的DISP不能是MOD */
"EXECIO 25  DISKW  …"				/* 只把前25行写入文件 */
```

### 返回码
返回码保存在RC中。  

* 0 - 正常  
* 1 - DISKW的时候，数据截断了  
* 2 - DISKR/DISKRU的时候，还未读到指定行数的行时，文件就结束了  
* 4 - 空文件  
* 20 - 严重错误  

### 复制整个文件
```
"ALLOC DA(my.input) F(datain) SHR REUSE"
"ALLOC DA(new.input) F(dataout) LIKE(my.input) NEW"
"NEWSTACK"               /* Create a new data stack for input only */
"EXECIO * DISKR datain (FINIS"
QUEUE '' /* Add a null line to indicate the end of the information */
"EXECIO * DISKW dataout (FINIS"
"DELSTACK"                            /* Delete the new data stack */
"FREE F(datain dataout)"
```

### 复制文件部分记录
```
/* 复制10行 */
"ALLOC DA('dept5.standard.heading') F(indd) SHR REUSE"
"ALLOC DA(office.memo(jan15)) F(outdd) SHR REUSE"
"EXECIO 10 DISKR indd (FINIS STEM DATA."
"EXECIO 10 DISKW outdd (FINIS STEM DATA."
/* 追加5行 */
"ALLOC DA(weekly.input(mar28)) F(myindd) SHR REUSE"
"ALLOC DA(yearly.output) F(myoutdd) MOD"
"EXECIO 5 DISKR myindd (FINIS"
"EXECIO 5 DISKW myoutdd (FINIS"
```

### 更新
注意更新的记录的长度不变，如果过长，会截断。  
```
/* 更新一行 */
"ALLOC DA('dept5.employee.list') F(updatedd) OLD"
"EXECIO 1 DISKRU updatedd 2 (LIFO"
PULL line
PUSH 'Crandall, Amy         AMY           5500'
"EXECIO 1 DISKW updatedd (FINIS"
"FREE F(updatedd)"
/* 更新多行 */
"ALLOC DA('dept5.employee.list') F(updatedd) OLD"
"EXECIO 1 DISKRU updatedd 2 (LIFO"
PULL line
PUSH 'Crandall, Amy         AMY           5500'
"EXECIO 1 DISKW updatedd"
"EXECIO 1 DISKRU updatedd 5 (LIFO"
PULL line
PUSH 'Leone, Mary           MARYL         5530'
"EXECIO 1 DISKW updatedd (FINIS"
 "FREE F(updatedd)"
/* 更新多行 */
  PARSE ARG name numlines  /* Get data set name and size from user */

  "ALLOC DA("name") F(updatedd) OLD"
  eof = 'NO'                        /* Initialize end-of-file flag */

  DO i = 1 to numlines WHILE eof = 'NO'
    "EXECIO 1 DISKRU updatedd" /* Queue the next line on the stack */
    IF RC = 2 THEN            /* Return code indicates end-of-file */
       eof = 'YES'
    ELSE
      DO
        PARSE PULL line
        SAY 'Please make changes to the following line.'
        SAY 'If you have no changes, press ENTER.'
        SAY line
        PARSE PULL newline
        IF newline = '' THEN NOP
        ELSE
          DO
            PUSH newline
            "EXECIO 1 DISKW updatedd"
          END
      END
  END
```

### JCL调用REXX
```
//TMP       EXEC PGM=IKJEFT01,DYNAMNBR=30,REGION=4096K
//SYSEXEC   DD   DSN=IBMUSER.EXEC,DISP=SHR            
//SYSTSPRT  DD   SYSOUT=A                             
//SYSTSIN   DD   *                                    
 %TEST                                               
/*                                                    
```

## 使用Edit Macro
使用Edit Macro的时候不能用`TSO MEMBER名`的方式使用，必须在一个VIEW或EDIT的MEMBER中在主命令行上直接敲`MEMBER名`。  
使用其它命令之前，必须先使用`"MACRO ()"`，如果没有特定的initial macro，则用一个()就行了。  
在REXX中有两种方法调用edit macro:  
```
1. 改变地址空间
ADDRESS ISREDIT
"MACRO"
"(LNE) = LINE 5"

2. 使用ISREDIT前缀
ISREDIT (LNE) = LINE 5"
```

格式：`ISREDIT (变量名) = 命令`。  

### 常用命令

* BLKSIZE - 查询BLKSIZE  
    ```
    >>-ISREDIT--(varname)-- = --BLKSIZE----------------------------><
    ```

* BOUNDS - 查询或设置边界  
    格式：  
    ```
    >>-ISREDIT--+-BOUNDS-+--+-----------------------------+--------><
                +-BOUND--+  '-+-left_col-+--+-right_col-+-'   
                +-BNDS---+    '-*--------'  '-*---------'     
                +-BND----+                                    
                '-BOU----' 
    ```
    例子：  
    ```
    "(L,R) = BND" 	/* 获取边界 */
    "BND 10 60"	  	/* 设置边界，左边界为10，右边界为60，可以用*表示边界不变 */
    ```

* CANCEL - 取消修改并退出  
* CAPS - 查询或设置大写模式  
* CHANGE - 替换  
    格式：  
    ```
                                          .-.ZFIRST--.ZLAST-.   
    >>-ISREDIT--CHANGE--string1--string2--+-----------------+------->
                                          '-labela--labelb--'   
    
       .-NEXT--.  .-CHARS--.                                    
    >--+-------+--+--------+--+----+--+---------------------+------><
       +-ALL---+  +-PREFIX-+  +-X--+  +-start_col-----------+   
       +-FIRST-+  +-SUFFIX-+  '-NX-'  '-left_col--right_col-'   
       +-LAST--+  '-WORD---'                                    
       '-PREV--'   
    ```
    返回码为4表示字符串没找到，8表示有错误，一般是替换字符串长于被替换字符串，而该行无法容纳。  

* CHANGE_COUNTS - 查询替换的次数  
    格式：  
    ```
    >>-ISREDIT--(var1,var2)-- = --CHANGE_COUNTS--------------------><
    ```
    var1为成功替换的个数，var2为未成功替换的个数。  

* COMPARE - 与其他文件比较  
* COPY - 复制其他文件内容到当前文件  
    格式：  
    ```
    >>-ISREDIT--COPY--+-member---------+--+-AFTER--+--label--------->
                      +-(member)-------+  '-BEFORE-'          
                      +-dsname---------+                      
                      +-dsname(member)-+                      
                      '-pathname-------'                      
    
    >--+--------------------------+--+--------+--------------------><
       '-start_line--+----------+-'  +-ASCII--+   
                     '-end_line-'    +-EBCDIC-+   
                                     '-UTF8---' 
    ```
* CREATE - 根据当前文件内容创建文件  
* CURSOR - 查询或设置光标的位置  
    格式：  
    ```
    >>-ISREDIT--(var1,var2)-- = --CURSOR---------------------------><
    
    >>-ISREDIT--CURSOR-- = -+-linenum-+--+-----+-------------------><
                            '-label---'  '-col-'
    ```
* CUT - 复制行到剪贴板  
* DATA_CHANGED - 查询数据是否改变，返回值为YES或NO。  
* DATA_WIDTH - 返回文件定义的记录长度  
    根据NUMBER模式会有不同
    | NUMBER MODE | FB | VB |
    |:------------|:---|:---|
    | OFF | LRECL | LRECL - 4 |
    | ON STD | LRECL - 8 | LRECL - 12 |
    | ON COB | LRECL - 6 | N/A |
    | ON STD COB | LRECL - 14 | N/A |

* DATASET - 查询使用DATA SET  
    格式：  
    ```
    >>-ISREDIT--(var1,var2,var3)-- = --DATASET---------------------><
    ```
    一般取第一个就可以了。  

* DELETE - 删除行  
* DISPLAY_COLS - 获取当前屏幕可以看见的第一列和最后一列  
    格式：  
    ```
    >>-ISREDIT--(var1,var2)-- = --DISPLAY_COLS---------------------><
    ```
* DISPLAY_LINES - 获取当前屏幕可以看见的第一行和最后一行   
    格式：  
    ```
    >>-ISREDIT--(var1,var2)-- = --DISPLAY_LINES--------------------><
    ```
* DOWN - 下翻  
    格式：  
    ```
    
    >>-ISREDIT--DOWN--amt------------------------------------------><
    amt
    The number of lines (0-9999) to scroll, or one of these operands:
    MAX
    Scrolls to the end of data in the specified direction.
    HALF
    Displays the next sequential half panel of data.
    PAGE
    Displays the next sequential full panel of data.
    CURSOR
    Scrolls until the line on which the cursor is located becomes the first data line on the panel.
    DATA
    Scrolls until the last data line on the current panel of data becomes the first data line on the next panel of data.
    ```
返回码2表示到底了，4表示没有可见行，8表示没有数据。  

* EDIT - 编辑另一个文件但不退出当前的编辑  
* EDITSET - 编辑器参数设置  
* END - 退出编辑  
* EXCLUDE - 隐藏行  
    ```
                             .-.ZFIRST--.ZLAST-.  .-NEXT--.   
    >>-+-EXCLUDE--+--string--+-----------------+--+-------+--------->
       +-EXCLUDED-+          '-labela--labelb--'  +-ALL---+   
       +-EXC------+                               +-FIRST-+   
       +-EX-------+                               +-LAST--+   
       '-X--------'                               '-PREV--'   
    
       .-CHARS--.                            
    >--+--------+--+---------------------+-------------------------><
       +-PREFIX-+  +-start_col-----------+   
       +-SUFFIX-+  '-left_col--right_col-'   
       '-WORD---' 
    ```
* EXCLUDE_COUNTS - 查询隐藏的行数。  
    格式：  
    ```
    >>-ISREDIT--(var1,var2)-- = --EXCLUDE_COUNTS-------------------><
    ```
    var1为查找到的字符串的个数，var2为隐藏的行数  
* FIND - 查找  
    格式：  
    ```
                         .-.ZFIRST--.ZLAST-.  .-NEXT--.   
    >>-+-FIND-+--string--+-----------------+--+-------+------------->
       '-F----'          '-labela--labelb--'  +-ALL---+   
                                              +-FIRST-+   
                                              +-LAST--+   
                                              '-PREV--'   
    
       .-CHARS--.                                    
    >--+--------+--+----+--+---------------------+-----------------><
       +-PREFIX-+  +-X--+  +-start_col-----------+   
       +-SUFFIX-+  '-NX-'  '-left_col--right_col-'   
       '-WORD---' 
    ```
* FIND_COUNTS - 查询查找到的个数。  
    格式：  
    ```
    >>-ISREDIT--(var1,var2)-- = --FIND_COUNTS----------------------><
    ```
    var1为查找到的字符串的个数，var2为查找到的字符串的行数  

* FLIP - 隐藏的显示，显示的隐藏  
* HEX - 显示十六进制字符  
* HIDE - 不显示隐藏行的信息  
* HILITE - 高亮显示  
* INSERT - 插入空行，为输出做准备  
    格式：  
    ```
    >>-ISREDIT--INSERT--+-label---+--+----------+------------------><
                        '-linenum-'  '-numlines-' 
    ```

* IMACRO - 指定初始化MACRO  
* LABEL - 查询或设置label  
    格式：  
    ```
    >>-ISREDIT--(var1,var2)-- = --LABEL--+-label---+---------------><
                                         '-linenum-'   
    
    >>-ISREDIT--LABEL--+-labelname-+-- = --label--+-------+--------><
                       '-linenum---'              '-level-'   
    ```
    第一个参数为level，没怎么用到。  

* LEFT - 左翻  
    格式：  
    ```
    >>-ISREDIT--LEFT--amt------------------------------------------><
    
    amt参考DOWN
    ```

* LINE - 获取或修改一行的内容  
    格式：  
    ```
    >>-ISREDIT--(varname)-- = --LINE--+-linenum-+------------------><
                                      '-label---'   
    
    >>-ISREDIT--LINE--+-linenum-+-- = --data-----------------------><
                      '-label---' 
    ```
    例子：  
    ```
    ISREDIT LINE 7 = (NEWDAT)							/* 使用变量赋值行 */
    ISREDIT LINE 1 = < 40 '&STR(/*)' 70 '&STR(*/)' >	
    ISREDIT LINE 2 = LINE + //							/* 前两列覆盖为// */
    ISREDIT LINE 3 = MASKLINE + (VAR)
    ```

* LINE_AFTER - 增加一行(在某行后面)  
    格式：  
    ```
                                              .-DATALINE-.         
    >>-ISREDIT--LINE_AFTER--+-linenum-+-- = --+----------+--data---><
                            '-label---'       +-INFOLINE-+         
                                              +-MSGLINE--+         
                                              '-NOTELINE-'   
    ```
    例子：  
    ```
    ISREDIT LINE_AFTER 4 = (NEWDAT)
    ISREDIT LINE_AFTER 0 = "This is the new top line of the data"	/* 在第一行插入 */
    ```

* LINE_BEFORE - 增加一行(在某行前面)  
* LINE_STATUS - 查询行的修改状态，使用数字表示。  
* LINENUM - 返回行号  
* LRECL - 查询LRECL，如果是VB，不包含RDW。  

* LOCATE - 定位行  
    ```
    >>-LOCATE--+-label---+-----------------------------------------><
               '-linenum-' 
    ```
* MASKLINE - 查询或设置MASKLINE  
* MEMBER - 查询当前的MEMBER名，如果是QSAM则返回空。  
* MOVE - 把另一个文件的内容移动到当前文件中  
* NONUMBER - 关闭行号  
* NULLS - 如何处理每行后面的空格。对代码书写有影响，如果是OFF，则在一行后面写的代码位置不会变，而另外两个则会往前移动(与前面非空字符有一个空格或无空格)。    
    ```
                  .-ON--STD-----.   
    >>-+-NULLS-+--+-------------+----------------------------------><
       +-NULL--+  |     .-ALL-. |   
       '-NUL---'  +-ON--+-----+-+   
                  +-STD---------+   
                  +-ALL---------+   
                  '-OFF---------' 
    ```
    > * STD - 后面的空格当成只有一个空格  
    > * ALL - 后面的空格当成没有  
    > * OFF - 后面的空格保留

* PASTE - 把剪贴板内容复制到当前文件  
* PROFILE - 查看profile  
* RECFM - 查询RECFM  
    格式：  
    ```
    var1
    The name of a variable to contain the type of record format of the data being edited, either F or V:
    F
    Fixed-length records.
    V
    Variable-length records.
    var2
    The name of a variable to contain the remaining record format information of the data being edited, in the combination of M, A, S, BM, BA, BS, BSM, or BSA:
    B
    Blocked records.
    S
    Standard or spanned records.
    M
    Machine print control character records.
    A
    ASA print control character records.
    
    ```
* PRESERVE - 对VB文件有影响，当为ON保留后面的空格，当为OFF不保留后面的空格，如果长度为0保留一个空格。  
* RCHANGE - 再执行一次替换  
* RECOVERY - 开启可恢复  
* RENUM - 重排行号  
* REPLACE - 使用当前编辑的内容覆盖存在的文件  
* RESET - 重置显示的内容  
    ```
                                .-.ZFIRST--.ZLAST-.   
    >>-+-RESET-+--+----------+--+-----------------+----------------><
       '-RES---'  +-CHANGE---+  '-labela--labelb--'   
                  +-COMMAND--+                        
                  +-ERROR----+                        
                  +-EXCLUDED-+                        
                  +-FIND-----+                        
                  +-HIDE-----+                        
                  +-LABEL----+                        
                  +-SOURCE---+                        
                  +-SPECIAL--+                        
                  '-ALL------' 
    ```
* RFIND - 再执行一次查找  
* RIGHT - 右翻  
* SAVE_LENGTH - 查询或设置变长记录的长度  
    格式：  
    ```
    >>-ISREDIT--(varname)-- = --SAVE_LENGTH--+-label---+-----------><
                                             '-linenum-'   
    >>-ISREDIT--SAVE_LENGTH--+-linenum-+-- = --value---------------><
                             '-label---'  
    ```
* SAVE - 保存  
* SEEK - 查找一个字符串，但不改变隐藏的状态。  
    格式：  
    ```
                              .-.ZFIRST--.ZLAST-.  .-NEXT--.   
    >>-ISREDIT--SEEK--string--+-----------------+--+-------+-------->
                              '-labela--labelb--'  +-ALL---+   
                                                   +-FIRST-+   
                                                   +-LAST--+   
                                                   '-PREV--'   
    
       .-CHARS--.                                    
    >--+--------+--+----+--+---------------------+-----------------><
       +-PREFIX-+  +-X--+  +-start_col-----------+   
       +-SUFFIX-+  '-NX-'  '-left_col--right_col-'   
       '-WORD---'  
    ```

* SEEK_COUNTS - SEEK命令的计数  
    格式：  
    ```
    >>-ISREDIT--(var1,var2)-- = --SEEK_COUNTS----------------------><
    ```
    var1为查找到的字符串的个数，var2为查找到的字符串的行数  

* SORT - 排序  
* SUBMIT - 提交  
* SESSION - 查询打开状态：VIEW/EDIT/EDIF/VIIF  
* TABS- 查找TABS  
* UNDO - 撤销  
* VIEW - 查看另一个文件但不退出当前的编辑  
* UP - 上翻  
* VOLUME - 查询VOLUME信息  
* XSTATUS - 查询或设置隐藏的状态。  
    格式：  
    ```
    >>-ISREDIT--(varname)-- = --XSTATUS--+-linenum-+---------------><
                                         '-label---'   
    >>-ISREDIT--XSTATUS--+-linenum-+-- = --+-X--+------------------><
                         '-label---'       '-NX-' 
                         
    ```

## ISPF
通过ISPEXEC开头的命令调用ISPF的服务。格式为`ISPEXEC service-name parameters`。  
主要服务：  

* DISPLAY  
> * DISPLAY - 显示PANEL或信息  
> * SETMSG  

* File tailoring  
* Library access  

* PDF
> * BROWSE  
> * EDIF  
> * EDIT - 编辑一个DATA SET，如ISPEXEC EDIT DATASET('ISPFPROJ.FTOUTPUT(TELOUT)')    
> * VIEW  
> * VIIF  

* TABLE  
对整个TABLE
> * TBCLOSE  
> * TBCREATE  
> * TBEND - 关闭不保存  
> * TBERASE  
> * TBOPEN  
> * TBQUERY  
> * TBSAVE  
> * TBSORT  
> * TBSTATS  
对行
> * TBADD  
> * TBBOTTOM  
> * TBDELETE  
> * TBEXIST - 是否存在  
> * TBGET  
> * TBMOD - 更新，如果不存在则插入  
> * TBPUT - 更新存在的记录  
> * TBSARG - 为SCAN设置参数  
> * TBSCAN  
> * TBSKIP
> * TBTOP  
> * TBVCLEAR    

* 变量  
All Functions
> * VERASE - 从shared pool或profile pool中删除变量  
> * VGET - 从shared pool或profile pool中获得变量  
> * VPUT - 从shared pool或profile pool中更新变量  

* 其它  
> * GETMSG  
> * LIST  
> * LOG  



### 对话参数(dialog variables)
使用VGET/VPUT得到和设置对话参数。  


## 例子

### 新建DATASET
新建一个DATE SET，以一个现有的DATA SET为参考。  
```
/***************************** REXX ********************************/
/* This exec reallocates a data set with more space.  It receives  */
/* as arguments the names of a base data set and a new data set.   */
/* It uses the SYSDSN function to ensure the base data set exists, */
/* uses the LISTDSI function to set variables with attributes of   */
/* the base data set, doubles the primary space variable and then  */
/* uses the variables as input to the ALLOCATE command to          */
/* reallocate a new data set.                                      */
/*******************************************************************/

 PARSE ARG baseds newds            /* Receive the data set names   */
                                   /*   with quotes, if any.       */
 IF SYSDSN(baseds) = 'OK' THEN
   DO                              /* If the base data set exists, */
     x = LISTDSI(baseds)           /*  use the LISTDSI function.   */
     IF x = 0 THEN                 /* If the function code is 0,   */
       CALL alc                    /*  call an internal subroutine.*/
     ELSE
       DO                          /* Else, display the system     */
         SAY sysmsglvl1            /* messages and codes for LISTDS*/
         SAY sysmsglvl2
         SAY 'Function code from LISTDSI is' x
         SAY 'Sysreason code from LISTDSI is' sysreason
       END
   END
 ELSE
   SAY 'Data set' baseds 'not found.'
 EXIT

 alc:
 newprimary = 2 * sysprimary       /* Compute new primary space.   */
 "ALLOC DA("newds") NEW SPACE("newprimary sysseconds") LIKE("baseds")"
                                   /* Allocate the new data set.   */
 IF RC = 0 THEN               /* If return code from allocate is 0 */
   SAY 'Data set' newds 'was allocated.'
 ELSE
   SAY 'Data set' newds 'was not allocated. Return code was' RC
```

### 编译程序
```
/* REXX FOR TSOID.ONL/BAT.SOURCE   COMPILE*/                     
/* COMMAND SAMPLE: 1. CC; 2. TSO CC MBR O/B */                          
ARG MBR FOB                                                             
ADDRESS TSO                                                             
MYUID = SYSVAR(SYSUID)                                                  
ADDRESS ISPEXEC                                                         
/* -S- Dataset and member name preparing           */                   
"ISREDIT MACRO ()"                                                      
IF MBR = '' THEN "ISREDIT (MBR)= MEMBER "                               
DSN = ''                                                                
DOB = ''                                                                
"ISREDIT (DSN)= DATASET"                                                
IF DSN = '' THEN                                                        
 DO                                                                     
   IF FOB = 'O' && FOB = 'B' && FOB = 'o' && FOB = 'b' THEN             
      NOP                                                               
   ELSE                                                                 
    DO                                                                  
      DO UNTIL FOB = 'O' && FOB = 'B' && FOB = 'o' && FOB = 'b'         
         SAY 'Your Program ' MBR ' is ''O''nline or ''B''atch ? '       
         PARSE PULL FOB                                                 
       END                                                              
    END                                                                 
   DOB = 'BAT'                                                          
   IF FOB = 'O' && FOB = 'o' THEN DOB = 'ONL'                           
   DSN = MYUID'.'DOB'.SOURCE'                                           
 END                                                                    
ELSE                                                                    
   PARSE VAR DSN DSYS '.' DOB '.' DLIB                                  
/* -S- CHECK PROGRAM IS A MAP OR DB2 OR JUST COBOL */                   
PGMNAME = DSN'('MBR')'                                                  
/*ADDRESS ISPEXEC */                                                    
"VIEW DATASET('"PGMNAME"') MACRO(CHKTPDB)"                              
ISPEXEC VGET (PGMTYP) PROFILE                                           
/*                                                                      
IF PGMTYP = 'D' THEN DO                                                 
   SAY 'DB2 PGM IS NOT SUPPORTING, YET.'                                
   EXIT(4)                                                              
 END                                                                    
*/                                                                      
/* -E- */                                                               
/* -S- DETERMINE COMPILE PROC TO USE               */                   
IF DOB = 'ONL' THEN                                                     
   IF PGMTYP = 'D' THEN                                                 
    DO                                                                  
      CPROC='LDB2CMD'                                                   
      JOBNUM='D'                                                        
    END                                                                 
   ELSE                                                                 
    IF PGMTYP = 'M' THEN                                                
     DO                                                                 
       CPROC='LASMMAP'                                                  
       JOBNUM='M'                                                       
     END                                                                
    ELSE                                                                
     DO                                                                 
       CPROC='LCOBCMD'                                                  
       JOBNUM='O'                                                       
     END                                                                
ELSE                                                                    
   IF PGMTYP = 'D' THEN                                                 
    DO                                                                  
      CPROC='LDB2BAT'                                                   
      JOBNUM='D'                                                        
    END                                                                 
   ELSE                                                                 
    DO                                                                  
      CPROC='LCOBBAT'                                                   
      JOBNUM='B'                                                        
    END                                                                 
/* -E- */                                                               
ADDRESS TSO                                                             
I=1                                                                     
TM=TIME('N')                                                            
PARSE VAR TM HH':'MM':'SS                                               
TLXX=SYSVAR(SYSUID)"."SYSVAR(SYSUID)'.I'HH||MM||SS                      
JOBNAME=SYSVAR(SYSUID)||JOBNUM                                          
IM.I="//"||JOBNAME||" JOB (ACCT),";i=i+1                                
IM.I="//      CLASS=A,REGION=1M,MSGCLASS=X,NOTIFY=&SYSUID,";I=I+1       
IM.I="//      MSGLEVEL=(1,1)";I=I+1                                     
IM.I="//      JCLLIB  ORDER=ZLYQ.USERLIB";i=i+1                         
IM.I="//      SET LBAPL="||MYUID;i=i+1                                  
IM.I="//      SET OLIB="||DOB;i=i+1                                     
IM.I="//      SET BLIB="||DOB;i=i+1                                     
IM.I="//"||MBR||"  EXEC PROC="||CPROC||",MBR="||MBR;i=i+1               
IF PGMTYP = 'D' THEN                                                    
 DO                                                                     
IM.I="//BIND.SYSTSIN DD *";i=i+1                                        
IM.I="  DSN SYSTEM(DB9G)";i=i+1                                         
 IF DOB = 'BAT' THEN                                                    
  DO                                                                    
   IM.I="  BIND PACKAGE(EXB) MEMBER("||MBR||") OWNER(LYEXUSR) -";i=i+1  
  END                                                                   
  ELSE DO                                                               
   IM.I="  BIND PACKAGE(EXO) MEMBER("||MBR||") OWNER(LYEXUSR) -";i=i+1  
  END                                                                   
IM.I="       VALIDATE(BIND)   ISOLATION(CS)  RELEASE(C) -";i=i+1        
IM.I="       ACTION(REPLACE) -";i=i+1                                   
IM.I="       ENCODING(EBCDIC) EXPLAIN(NO) ";i=i+1                       
 IF DOB = 'BAT' THEN                                                    
  DO                                                                    
   IM.I="  BIND PLAN("||MBR||") -";i=i+1                                
   IM.I="       PKLIST(EXB."||MBR||") OWNER(LYEXUSR) -";i=i+1           
   IM.I="       VALIDATE(BIND) ISOLATION(CS) ACTION(REPLACE) -";i=i+1   
   IM.I="       ACQUIRE(USE)   ENCODING(EBCDIC) EXPLAIN(NO)";i=i+1      
  END                                                                   
IM.I="  END";i=i+1                                                      
  END                                                                   
IM.I="//*";i=i+1                                                        
/*                                                                      
IM.I="//*";i=i+1                                                        
 IF DOB = 'BAT' & SUBSTR(MYUID,1,4) = 'LYQU' THEN DO                    
    IM.I="//BIND.SYSIN   DD *";i=i+1                                    
    IM.I="  GRANT EXECUTE ON PLAN "||MBR||" TO "||MYUID;i=i+1           
 END                                                                    
   IM.I="  BIND PLAN(LYEXUSR) -";i=i+1                                  
   IM.I="       PKLIST(EXB.*)  OWNER(LYEXUSR) -";i=i+1                  
*/ */                                                                   
"ALLOC FI(ICE) SPACE(1) TRACK NEW UNIT(3390) LRECL(80) ,                
  RECFM(F B) REU BLKSIZE(27920) DA('"TLXX"') REU DELETE"                
"EXECIO * DISKW ICE ( STEM IM. FINIS "                                  
"SUBMIT '"TLXX"'"                                                       
"FREE FI(ICE)"                                                          
EXIT(RC)                                                                
END                                                                     
```