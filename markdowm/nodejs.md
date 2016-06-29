# node.js
>Node.js 是一个基于 Chrome V8 引擎的 JavaScript 运行环境。Node.js 使用了一个事件驱动、非阻塞式 I/O 的模型，使其轻量又高效。Node.js 的包管理器 npm，是全球最大的开源库生态系统。本文仅讨论node.js的机制及自带模块。外部模块，比如connect/express等，不在此讨论。

## 配置
### package.json
>package.json是node.js/npm中最重要的配置，用于配置项目基本信息和依赖的模块。同时package.json必须放置在顶层目录。

**配置项**  

* name  
>必须要有。因为name在以后作为url的一部分，注意不要使用非安全的字符。如果想把项目注册到npm，先上[npm仓库](http://registry.npmjs.org/)看下name有没有被人先注册了。  

* version  
>version必须能被 node-semver解析，它被包在npm的依赖中。  

* description  
>描述信息，如果注册到npm仓库，那么description有助于别人搜索。

* keywords
>关键字，如果注册到npm仓库，那么description有助于别人搜索。

* homepage
>项目官网的url

* bugs
>项目提交问题的url、邮箱地址。例如：
>```
>{ "url" : "http://github.com/owner/project/issues",
>  "email" : "project@hostname.com"
>}
>```

* license
>许可证，可以简单的使用BSD/MIT这种。如果有更复杂的许可证，可以给出更多细节：
>```
>"license": [
>   {"type" : "mylicense",
>    "url" : "http://.../"}]
>```

* people fields: author, contributors
>项目的作者、贡献者。
>可以是对象形式:
>```
>{ "name" : "Barney Rubble"
>, "email" : "b@rubble.com"
>, "url" : "http://barnyrubble.tumblr.com/"
>}
>```
>或者字符串形式：
>"Barney Rubble <b@rubble.com> (http://barnyrubble.tumblr.com/)"

* files
>files是一个包含项目中的文件的数组。如果命名了一个文件夹，那也会包含文件夹中的文件。（除非被其他条件忽略了)你也可以提供一个.npmignore文件，让即使被包含在files字段中得文件被留下。其实就像.gitignore一样。

* main
>通过文件名指定模块的入口。

* bin(TBC)
>

* man
>指定一个单一的文件或者一个文件数组供man程序使用。

* directories
>标示出包的结构。
>库文件夹--directories.lib
>bin目录--directories.bin，如果有bin字段，这里无效
>man目录--directories.man
>doc目录--directories.doc
>example目录--directories.example
