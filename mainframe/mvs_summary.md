# MVS总结
总结MVS相关的知识，比如TSO/ISPF等。  
参考:  
* [Z/OS basic skills](https://www.ibm.com/support/knowledgecenter/zosbasics/com.ibm.zos.zconcepts/toc.htm)。    
* [DFSORT](https://www.ibm.com/support/knowledgecenter/SSLTBW_2.3.0/com.ibm.zos.v2r3.ice/ice.htm)  
* [CICS reference](https://www.ibm.com/support/knowledgecenter/SSGMCP_5.4.0/reference/reference_new.html)  
* [COBOL](https://www.ibm.com/support/knowledgecenter/SS6SG3_5.2.0/welcome.html)  
* [JCL Reference](https://www.ibm.com/support/knowledgecenter/SSLTBW_2.1.0/com.ibm.zos.v2r1.ieab600/toc.htm)  
* [JCL User guide](https://www.ibm.com/support/knowledgecenter/SSLTBW_1.13.0/com.ibm.zos.r13.ieab500/iea2b570.htm)  
* [SDSF](https://www.ibm.com/support/knowledgecenter/SSLTBW_2.3.0/com.ibm.zos.v2r3.isfa600/toc.htm)  
* [DB2](http://www-01.ibm.com/support/docview.wss?uid=swg27047206)  
* [REXX](https://www.ibm.com/support/knowledgecenter/SSLTBW_2.1.0/com.ibm.zos.v2r1.ikja300/toc.htm)  
* [DFSMS](https://www.ibm.com/support/knowledgecenter/SSLTBW_2.1.0/com.ibm.zos.v2r1.ida/ida.htm)  
* [z/OS ISPF Edit and Edit Macros](https://www.ibm.com/support/knowledgecenter/SSLTBW_1.13.0/com.ibm.zos.r13.f54em00/toc.htm)  
* [ISPF](https://www.ibm.com/support/knowledgecenter/SSLTBW_2.3.0/com.ibm.zos.v2r3.isp/isp.htm)  


## 按键

* Enter - 换行    
* ctrl - 回车  
* Attn - 中止，当执行forground命令卡住时，可以用Attn中止。  
* shift+F12 - 显示上一条命令。  

## DATA SET

### 记录
参考：https://www.ibm.com/support/knowledgecenter/zosbasics/com.ibm.zos.zconcepts/zconcepts_159.htm  
记录分为定长、变长、不定长三种。  
对于QSAM文件，块是一次IO的单位。  
```
F - 相当于一条记录一个块:
<------> 记录
======== 

FB - 一个块包含多条记录：
<------><------><------> 记录
======================== 块 

VB：
==============================================
LLBBllbbxxxxxxxxxxxxxllbbxxxxxxxxxllbbxxxxxxxx
==============================================
|<--                   块                  -->
|<>|块控制信息
    |<--   记录  -->|
    |<>|记录控制信息	
        |<-- 内容-->|
```
LL为块的长度，ll为记录的长度，BB和bb为系统保留使用的。  
VB定义时最好指定BLOCK的大小，使用`BLKSIZE=27998`

常用的如下：  

* FB - 定长的BLOCK  
* VB - 变长的BLOCK  
* FBA - 定长的BLOCK，带打印控制位  
* U - 不定长  

#### RDW
变长记录前面4位被称为record descriptor word (RDW)。  
变长块前面4位被称为block descriptor word (BDW)。  
可以通过DFSORT的`OUTFIL BUILD=(1,4)`查看RDW，前两个字节为两进制的记录长度。
比如`X'0008'`表示共8个字节，注意包含了4个字节的RDW，所在内容的长度为4个字节。  

### 报表
报表如果使用ANSI控制符，则在第一列作为控制符的位置：  

* 1 - 表示新页的开始。  
* 空格 - 正常输出。  
* 0 - 先打印一个空行再打印该行。  
* \- - 先打印两个空行再打印该行。  

### PDS
PDS的目录空间分配，每个目录空间可以分配6个MEMBER。  

## TSO/E
TSO是Time Sharing Option的缩写，是MVS是命令输入处理器。  

### 命令
可以使用`TSO HELP 命令名`查看详细帮助。  

* ALLOCATE  
    分配文件。 格式：  
    ```
    >>-+-ALLOCATE-+------------------------------------------------->
       '-ALLOC----'   
    
    >--+-+-+-DATASET-+--(--+-*----------+--)-+--+------------------------+-+-><
       | | '-DSNAME--'     | .--------. |    |  '-+-FILE---+--(--name--)-' |   
       | |                 | V        | |    |    '-DDNAME-'               |   
       | |                 '---dsname-+-'    |                             |   
       | '-DUMMY-----------------------------'                             |   
       '-+-FILE---+--(--name--)--+-----------------------------------+-----'   
         '-DDNAME-'              +-+-DATASET-+--(--+-*----------+--)-+         
                                 | '-DSNAME--'     | .--------. |    |         
                                 |                 | V        | |    |         
                                 |                 '---dsname-+-'    |         
                                 '-DUMMY-----------------------------'         
    
    
    >>-+---------------------+--+---------------------------+------><
       +-OLD-----------------+  '-DATACLAS(data_class_name)-'   
       +-SHR-----------------+                                  
       +-MOD-----------------+                                  
       +-NEW-----------------+                                  
       '-SYSOUT--+---------+-'                                  
                 '-(class)-'                                    
    
    
    >>-+---------------------------------+-------------------------->
       '-MGMTCLAS(management_class_name)-'   
    
    >--+------------------------------+--+---------------------+---->
       '-STORCLAS(storage_class_name)-'  '-VOLUME(serial_list)-'   
    
    >--+----------------------------------------------------------+-><
       '-SPACE(quantity-+------------+-)-+-BLOCK(value)---------+-'   
                        '-,increment-'   +-AVBLOCK--(--value--)-+     
                                         +-TRACKS---------------+     
                                         '-CYLINDERS------------'     
    
    
    >>-+-----------------+--+----------------+--+--------------+---->
       '-AVGREC(-+-U-+-)-'  '-BLKSIZE(value)-'  '-DIR(integer)-'   
                 +-K-+                                             
                 '-M-'                                             
    
    >--+---------------+--+---------------------------------+------->
       '-ALTFILE(name)-'  '-DEST(-+-destination---------+-)-'   
                                  '-destination.user_id-'       
    
                  .-NOHOLD-.                                      
    >--+-------+--+--------+--+------------+--+---------------+----->
       '-REUSE-'  '-HOLD---'  '-UNIT(type)-'  +-UCOUNT(count)-+   
                                              '-PARALLEL------'   
    
    >--+-------------+---------------------------------------------><
       '-LABEL(type)-'   
    
    
    >>-+---------------------+--+------------------------+---------->
       '-ACCODE(access_code)-'  '-POSITION(sequence_no.)-'   
    
    >--+---------------+--+---------+--+-------------------+-------->
       '-MAXVOL(count)-'  '-PRIVATE-'  '-VSEQ(vol_seq_no.)-'   
    
    >--+-----------------------+--+------------------+-------------->
       +-LIKE(model_dsname)----+  '-REFDD(file_name)-'   
       '-USING(attr_list_name)-'                         
    
    >--+--------------------------------------+--+---------+-------><
       '-SECMODEL(profile_name-+----------+-)-'  '-RELEASE-'   
                               '-,GENERIC-'                    
    
    
    >>-+-------+--+-----------+--+--------------------------+------->
       '-ROUND-'  +-KEEP------+  +-BUFL(buffer_length)------+   
                  +-DELETE----+  '-BUFNO(number_of_buffers)-'   
                  +-CATALOG---+                                 
                  '-UNCATALOG-'                                 
    
    >--+------------------------------------+----------------------->
       '-LRECL(-+-logical_record_length-+-)-'   
                +-X---------------------+       
                '-nnnnnK----------------'       
    
       .-NCP(1)-----------------------------.   
    >--+------------------------------------+----------------------><
       '-NCP--(--no._of_channel_programs--)-'   
    
    
                   .------------------------.                       
                   V                        |                       
    >>-+--------+----+--------------------+-+--+----------------+--->
       +-INPUT--+    +-EXPDT(year_day)----+    '-BFALN(-+-F-+-)-'   
       '-OUTPUT-'    '-RETPD(no._of_days)-'             '-D-'       
    
    >--+--------------------+--+------------------+----------------->
       |        .-,-----.   |  '-EROPT(-+-ACC-+-)-'   
       |        V       |   |           +-SKP-+       
       '-OPTCD(---+-A-+-+-)-'           '-ABE-'       
                  +-B-+                               
                  +-C-+                               
                  +-E-+                               
                  +-F-+                               
                  +-H-+                               
                  +-J-+                               
                  +-Q-+                               
                  +-R-+                               
                  +-T-+                               
                  +-W-+                               
                  '-Z-'                               
    
    >--+----------------+------------------------------------------><
       '-BFTEK(-+-S-+-)-'   
                +-E-+       
                +-A-+       
                '-R-'       
    
    
    >>-+--------------------+--+---------------+-------------------->
       |        .-,-----.   |  '-DIAGNS(TRACE)-'   
       |        V       |   |                      
       '-RECFM(---+-A-+-+-)-'                      
                  +-B-+                            
                  +-D-+                            
                  +-F-+                            
                  +-M-+                            
                  +-S-+                            
                  +-T-+                            
                  +-U-+                            
                  '-V-'                            
    
    >--+----------------------+------------------------------------->
       '-LIMCT(search_number)-'   
    
    >--+-----------------------------------+------------------------>
       '-BUFOFF(-+-block_prefix_length-+-)-'   
                 '-L-------------------'       
    
    >--+------------------+----------------------------------------><
       '-DSORG(-+-DA--+-)-'   
                +-DAU-+       
                +-PO--+       
                +-POU-+       
                +-PS--+       
                '-PSU-'       
    
    
    >>-+--------------+--+-----------------------------+------------>
       '-DEN(-+-0-+-)-'  |                .-NOCOMP-.   |   
              +-1-+      +-TRTCH(-+-C--+--+--------+-)-+   
              +-2-+      |        +-E--+  '-COMP---'   |   
              +-3-+      |        +-T--+               |   
              '-4-'      |        '-ET-'               |   
                         '-KEYLEN(bytes)---------------'   
    
    >--+----------------+--+------------------+--+---------+-------><
       '-KEYOFF(offset)-'  '-RECORG(-+-ES-+-)-'  '-PROTECT-'   
                                     +-KS-+                    
                                     +-LS-+                    
                                     '-RR-'                    
    
    
    >>-+----------------------------------------+--+---------+------>
       '-COPIES(nnn-+-----------------------+-)-'  +-BURST---+   
                    |     .-,-----------.   |      '-NOBURST-'   
                    |     V             |   |                    
                    '-,-(---group_value-+-)-'                    
    
    >--+----------------------------+------------------------------->
       |        .---------------.   |   
       |        V               |   |   
       '-CHARS(---,--table_name-+-)-'   
    
    >--+---------------------------------+-------------------------><
       '-FLASH(overlay_name-+--------+-)-'   
                            '-copies-'       
    
    
    >>-+-------------------------------+---------------------------->
       '-MODIFY(module_name-+------+-)-'   
                            '-,trc-'       
    
    >--+----------------------------+--+-------------------+-------->
       '-FCB(image_id-+---------+-)-'  '-FORMS(forms_name)-'   
                      +-,ALIGN--+                              
                      '-,VERIFY-'                              
    
    >--+--------------------------------------+--------------------><
       |         .------------------------.   |   
       |         V                        |   |   
       '-OUTDES(---output_descriptor_name-+-)-'   
    
    
    >>-+---------------------+--+---------------------+------------->
       '-SPIN(-+-UNALLOC-+-)-'  '-SEGMENT(page_count)-'   
               '-NO------'                                
    
    >--+-------------------------------+---------------------------->
       '-DSNTYPE(-+-LIBRARY-+----+-+-)-'   
                  |         +-,1-+ |       
                  |         '-,2-' |       
                  +-PDS------------+       
                  +-HFS------------+       
                  +-PIPE-----------+       
                  +-LARGE----------+       
                  +-BASIC----------+       
                  +-EXTREQ---------+       
                  '-EXTPREF--------'       
    
    >--+-----------------------------------+-----------------------><
       '-UCS(universal_character_set_name)-'   
    
    
    >>-+------------------------------+----------------------------->
       '-WRITER(external_writer_name)-'   
    
    >--+-----------------------+------------------------------------>
       |       .-----------.   |   
       |       V           |   |   
       '-PATH(---/pathname-+-)-'   
    
    >--+------------------------------------+----------------------><
       |           .-KEEP---. .-,KEEP---.   |   
       '-PATHDISP(-+--------+-+---------+-)-'   
                   '-DELETE-' '-,DELETE-'       
    
    
    >>-+---------------------------------------+-------------------->
       |           .-----------------------.   |   
       |           V                       |   |   
       '-PATHMODE(---file_access_attribute-+-)-'   
    
    >--+-----------------------------+------------------------------>
       |           .-------------.   |   
       |           V             |   |   
       '-PATHOPTS(---file_option-+-)-'   
    
    >--+------------------------+--+----------------+--------------->
       |           .-BINARY-.   |  '-RLS(-+-NRI-+-)-'   
       '-FILEDATA(-+-TEXT---+-)-'         '-CR--'       
                   '-RECORD-'                           
    
    >--+------------------+----------------------------------------><
       '-EATTR(-+-NO--+-)-'   
                '-OPT-' 
    ```
    例子：  
    ```
    新建文件
    ALLOCATE DA(rexx.exec) NEW DIR(10) SPACE(50,20) DSORG(po)
      RECFM(v,b) LRECL(255) BLKSIZE(6120)
      
    "ALLOC DA('IBMUSER.BAT.SOURCE4') FI(OUTDD) LIKE('IBMUSER.BAT.SOURCE3')"
    
    为已有文件分配一个名字
    ALLOC FILE(SYSEXEC) DATASET(rexx.exec,'isp.phone.exec') SHR REUSE
    ```

* ALTLIB  
    操作用于隐式执行EXEC的alternative libraries。  
    alternative libraries有三个层次，按先后顺序查找：  
    
    > * user level，预先分配SYSUEXEC和SYSUPROC。  
    > * application level  
    > * system level，预先分配SYSEXEC和SYSPROC。  
    
    操作：  
    
    > * ACTIVATE - 激活某个层次  
    > * DEACTIVATE - 排除某个层次  
    > * DISPLAY - 显示现在的顺序  
    > * RESET - 重置顺序  
    
    例子：  
    ```
    ALTLIB ACTIVATE APPLICATION(exec) DATASET(new.exec)	# 激活application level，并指定PDS
    ALTLIB ACTIVATE USER(exec)							# 激活user level
    ALTLIB DEACTIVATE SYSTEM(exec)						# 排除system level
    ALTLIB RESET										# 重置顺序
    ```

* CANCEL  
    用于取消一个已提交的JOB。  
    例子：  
    ```
    cancel je024a1
    ```

* DELETE  
    删除DATA SET。  
    例子：  
    ```
    DEL 'IBMUSER.TEST.IOT2'
    DEL 'IBMUSER.TEST.*'	只匹配所有非VSAM
    DEL 'IBMUSER.TEST.KSDS' CLUSTER		VSAM不能用通配且要声明CLUSTER
    ```

* EDIT
    编辑文件，没什么卵用，是远不如ISPF编辑。  

* EXEC  
    执行CLIST或REXX。  

* EXECUTIL  
    用于SYSEXEC和SYSPROC的设置及跟踪的控制。  
    
    例子：  
    ```
    EXECUTIL SEARCHDD(NO)	设置只搜索SYSPROC，不搜索SYSEXEC
    EXECUTIL TS				开始跟踪
    EXECUTIL TE 			结束跟踪
    ```

* FREE  
    取消文件的分配。  
    例子：  
    ```
    free dataset(proga.load)
    free file(in1)
    free all
    ```

* HELP  
    查看帮助。格式：  
    ```
    >>-+-HELP-+--+-----------------------------------+-------------><
       '-H----'  '-+-command_name----+--| Operands |-'   
                   '-subcommand_name-'                   
    
    Operands
    
       .-ALL--------------------------.   
    |--+------------------------------+-----------------------------|
       +-FUNCTION---------------------+   
       +-SYNTAX-----------------------+   
       +-OPERANDS-+-----------------+-+   
       |          |   .---------.   | |   
       |          |   V         |   | |   
       |          '-(---operand-+-)-' |   
       +-POSITIONAL(nn)---------------+   
       |        .------------.        |   
       |        V            |        |   
       '-MSGID(---identifier-+-)------' 
    ```

* LISTALC   
    显示当前已经分配的数据集。  
    格式：  
    ```
    >>-+-LISTALC-+--+--------+--+---------+--+---------+------------>
       '-LISTA---'  '-STATUS-'  '-HISTORY-'  '-MEMBERS-'   
    
    >--+----------+------------------------------------------------><
       '-SYSNAMES-'
    ```
    如果使用HISTORY参数，则显示创建及失效日期。  
    如果使用MEMBERS参数，则显示PDS的MEMBER。  
    比如`TSO LISTALC STATUS`，结果为：  
    ```
    --DDNAME---DISP--
    SYS1.UADS        
      SYSUADS  KEEP  
    SYS1.BRODCAST    
      SYSLBC   KEEP
    ...
    ```  

* LISTBC  
    显示通告。  

* LISTCAT  
    显示DATASET信息，特别对于VSAM能显示比较详细的设置。  
    格式：  
    ```
    >>-+-LISTCAT-+--+--------------------------------------+-------->
       '-LISTC---'  '-CATALOG(catalog_name-+-----------+-)-'   
                                           '-/password-'       
    
    >--+----------------------+------------------------------------->
       '-+-OUTFILE-+-(ddname)-'   
         '-OFILE---'              
    
    >--+------------------------------------------+--+---------+---->
       |          .---------------------------.   |  '-CLUSTER-'   
       |          V                           |   |                
       +-ENTRIES(---entry_name--+-----------+-+-)-+                
       |                        '-/password-'     |                
       '-+-LEVEL-+-(level)------------------------'                
         '-LVL---'                                                 
    
    >--+------+--+-------+--+-------+--+---------+------------------>
       '-DATA-'  +-INDEX-+  +-SPACE-+  +-NONVSAM-+   
                 '-IX----'  '-SPC---'  '-NVSAM---'   
    
    >--+-------------+--+---------------------+--+-----------+------>
       +-USERCATALOG-+  +-GENERATIONDATAGROUP-+  +-PAGESPACE-+   
       '-UCAT--------'  '-GDG-----------------'  '-PGSPC-----'   
    
    >--+-------+--+----------------+--+------------------+---------->
       '-ALIAS-'  '-CREATION(days)-'  '-EXPIRATION(days)-'   
    
       .-NAME-------.                                             
    >--+------------+--+----------+--+----------+--+----------+----><
       +-ALL--------+  '-DATACLAS-'  '-MGMTCLAS-'  '-STORCLAS-'   
       +-VOLUME-----+                                             
       +-ALLOCATION-+                                             
       '-HISTORY----'  
    ```
    例子：  
    ```
    TSO LISTCAT ENTRIES('IBMUSER.TEST.KSDS') ALL
    ```

* LISTDS  
    显示DATASET的信息，特别是LRECL/RECFM等信息。  
    格式：  
    ```
                 .----------.                               
                 V          |                               
    >>-LISTDS-(----data_set-+--)--+--------+--+---------+----------->
                                  '-STATUS-'  '-HISTORY-'   
    
    >--+---------+--+-------+--+-----------------------+------------>
       '-MEMBERS-'  '-LABEL-'  '-CATALOG(catalog_name)-'   
    
    >--+-------+---------------------------------------------------><
       '-LEVEL-'  
    ```
    当使用MEMBERS选项，会列出MEMBER列表。  
    例子：  
    ```
    LISTDS 'IBMUSER.BAT.SOURCE' MEMBERS
    IBMUSER.BAT.SOURCE         
    --RECFM-LRECL-BLKSIZE-DSORG
      FB    80    27920   PO   
    --VOLUMES--                
      USR001   
    --MEMBERS--
      COMP5    
      DYT1     
      DYT2     
      DYT3     
      DYT4     
      GETALLQ  
      MQGETT   
      RPT04    
      SENDMQ   
      VSTEST   
    ```

* PROFILE  
    显示或更改用户的PROFILE。  
    如使用`PROFILE NOPREFIX`取消自动添加用户ID作为文件前缀。  

* RECEIVE  
    用于接收别的NODE传来的文件。  

* RENAME  
    重命名。  
    例子：  
    ```
    rename *.data,*.text
    REN 'ELIZBETH(LIZ)' (SUZIE) ALIAS 给PDS MEMBER设置一个ALIAS
    ```

* SEND  
    发送信息给其他用户。  
    格式：  
    ```
                                     .-2----------.       
                         .-OPERATOR(-+-route_code-+-)-.   
    >>-+-SEND-+--'text'--+----------------------------+------------->
       '-SE---'          +-USER(-+-*-----------+-)----+   
                         |       | .---------. |      |   
                         |       | V         | |      |   
                         |       '---user_id-+-'      |   
                         '-CN(-+-console_name-+-)-----'   
                               '-INTERNAL-----'           
    
       .-NOW---.  .-NOWAIT-.   
    >--+-------+--+--------+---------------------------------------><
       +-LOGON-+  '-WAIT---'   
       '-SAVE--' 
    ```

* SMCOPY  
    复制文件内容到另一个文件。  
    例子：  
    ```
    smcopy fromdataset('sys1.clist(zlogon)')
           todataset(test.clist(zlogon))
    ```

* STATUS  
    显示作业的状态。  

* SUBMIT  
    提交作业。  

* TRANSMIT  
    发送信息或文件给其他用户。其他用户使用RECEIVE接收。  

## ISPF
ISPF是Interactive System Productivity Facility的缩写。在TSO的基础上提供了：  

1. 显示服务  
2. 变量服务  
3. 表服务  
4. 会话测试设备 

### 通用命令
* SWAP LIST - 看开了几屏。  

### 数据集命令
**主命令**  

**行命令**  

* CO - 复制数据集，可以使用`S *`进行全选，或手工在MEMBER前面输入S进行选择。  

### 编辑命令(Edit)
如下的命令解释适用于VIEW/EDIT MEMBER。  

**Picture String**  

**主命令**  

* AUTOSAVE - 自动保存  
* BROWSE - 浏览另一个文件但不退出当前的编辑  
* BOUNDS - 指定列边界  
* CANCEL - 取消修改并退出  
* CAPS - 自动转换为大写  
* CHANGE - 替换  
    ```
                                     .-.ZFIRST--.ZLAST-.   
    >>-+-CHANGE-+--string1--string2--+-----------------+------------>
       +-CHA----+                    '-labela--labelb--'   
       +-CHG----+                                          
       '-C------'                                          
    
       .-NEXT--.  .-CHARS--.                                    
    >--+-------+--+--------+--+----+--+---------------------+------><
       +-ALL---+  +-PREFIX-+  +-X--+  +-start_col-----------+   
       +-FIRST-+  +-SUFFIX-+  '-NX-'  '-left_col--right_col-'   
       +-LAST--+  '-WORD---'                                    
       '-PREV--'
    ```
    当为PREFIX仅匹配为一个单词的开头。  
    当为SUFFIX仅匹配为一个单词的结束。  
    当为WORD仅匹配为整个单词。  
    
    当为X仅匹配被隐藏的内容。  
    当为NX仅匹配不被隐藏的内容。  

* COMPARE - 与其他文件比较  
    如果仅输入COMPARE然后回车则是对COMPARE的参数进行设置。  
* COPY - 复制其他文件内容到当前文件  
* CREATE - 根据当前文件内容创建文件  
* CUT - 复制行到剪贴板  
    只输入CUT则是复制全部行。  
* DELETE - 删除行  
    ```
    >>-+-DELETE-+--+-+-ALL------------+--+-X--+-+------------------><
       '-DEL----'  | '-labela--labelb-'  '-NX-' |   
                   '-ALL--labela--labelb--------'  
                   
    ```
可以使用`DEL ALL X`删除所有隐藏的行。  
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
* FIND - 查找  
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
* FLIP - 隐藏的显示，显示的隐藏  
* HEX - 显示十六进制字符  
* HIDE - 不显示隐藏行的信息  
* HILITE - 高亮显示  
* IMACRO - 指定初始化MACRO  
* LOCATE - 定位行  
    ```
    >>-LOCATE--+-label---+-----------------------------------------><
               '-linenum-' 
               
    ```
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
* SAVE - 保存  
* SORT - 排序  
* SUBMIT - 提交  
* TABS- 查找TABS  
* UNDO - 撤销  
* VIEW - 查看另一个文件但不退出当前的编辑  
* C - 替换字符。  
  使用`C XX OO .A .B ALL`，对.A和.B标签之间的XX全部进行替换。  
  可以使用PICTURE STRING进行通配选择，比如`C P'=' ' ' 1 6 ALL`，把1到6列的所有字符替换为空格。  

**行命令**  

* ( - 左移位(Column Shift Left)  
* ) - 右移位(Column Shift Right)  
* < - 左移位(Data Shift Left)  
* > - 右移位(Data Shift Right)  
* A - 指定复制、移动到当前行的下面  
* B - 指定复制、移动到当前行的上面  
* BOUNDS - 指定列边界  
* C - 复制行  
* COLS - 显示列数  
* D - 删除行  
* F - 显示第一行(被隐藏的)  
* I - 插入行  
* L - 显示最后一行(被隐藏的)  
* LC - 转换当前行为小写  
* M - 移动行  
* MASK - 定义MASK，当使用I/TE/TS时会自动在新行添加MASK字符  
* MD - 把==MSG>、=NOTE=、=COLS>、======的内容转换为真正的内容。  
* O - 用于配合复制、移动来合并两行  
* R - 重复行  
* S - 显示隐藏的行  
* TABS - 建立一行的跳转位置，当按下回车时光标跳转的位置，COBOL中一般设置12、20、35、50列。  
* TE - 插入多个空行用于输入  
* TF - 合并行  
* TS - 由光标处拆分行  
* UC - 转换当前行为大写  
* X - 隐藏行  

### Edit Macro
在编辑文件之前，有一个Initial Macro的输入项，可以填入对文件初始化的EXEC。放在SYSPROC/SYSEXEC中，里面是CLIST或REXX语句，对文件进行操作。  
```
  Workstation  Help                                        
ssssssssssssssssssssssssssssssssssssssssssssssssssssssssss 
                     EDIT Entry Panel                      
                                                           
Object Name:                                               
'LYQA07.TEST.IOT2'                                         
* No workstation connection                                
  Initial Macro  . . ISRDASH                               
  Profile Name . . .           (Blank defaults to Type)    
  Format Name  . . .                                       
  Panel Name . . . .           (Leave blank for default)   
                                                           
  Options                                                  
     Confirm Cancel/Move/Replace                           
     EDIT Mixed Mode                                       
     EDIT host file on Workstation                         
     Preserve VB record length                             
  /  Warn on First Data Change                             
                                                           
Press ENTER to continue. Press CANCEL to cancel action.    
```

**Initial Macro编写**   
必须以ISREDIT MACRO开头。  
```
ISREDIT MACRO
  SET &COUNT = 1                      /* Start loop counter         */
  DO WHILE &COUNT <= 9                /* Loop up to 9 times         */
    ISREDIT FIND 'TEST - # '          /* Search for 'TEST-#'        */
    SET &RETCODE = &LASTCC            /* Save the FIND return code  */
    IF &RETCODE = 0 THEN              /*                            */ -
      DO                              /* If the string is found,    */
        ISREDIT CHANGE '#' '&COUNT'   /* change '#' to the value    */
        SET &COUNT = &COUNT + 1       /* of '&COUNT', increment     */
      END                             /* the counter by one, and    */
    ELSE                              /* continue the loop.         */ -
      SET &COUNT = 10                 /* If the string is not       */
  END                                 /* found, set the counter to  */
EXIT CODE (0)                         /* exit the loop.             */
```

