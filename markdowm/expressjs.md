# express
>express是基于 Node.js 平台，快速、开放、极简的 web 开发框架。express 4.x已经不基于connect。本文基于[express中文官网](http://www.expressjs.com.cn/)，其实express的内容完全可以放在一个页面中，方便查询（相对于官网）。

## 安装
1. 从头开始
```js
npm install express
```
2. 使用应用生成器
```js
npm install express-generator -g
```
然后使用express myapp就能生成一个myapp目录，项目结构都已搭建好。
可以使用express -h查看所有命令选项，因为默认使用jade模板，可以改为其他模板

## 使用
```js
//引入express模块
var express = require('express');
//创建app对象
var app = express();

//基本url使用中间件
app.get('/', function (req, res) {
  res.send('Hello World!');
});

//启动服务器
var server = app.listen(3000, function () {
  var host = server.address().address;
  var port = server.address().port;

  console.log('Example app listening at http://%s:%s', host, port);
});
```
上面的代码启动一个服务并监听从 3000 端口进入的所有连接请求。他将对所有 (/) URL 或 路由 返回 “Hello World!” 字符串。对于其他所有路径全部返回 404 Not Found。
用node app.js启动应用。然后在localhost:3000查看

## 路由
>路由（Routing）是由一个 URI（或者叫路径）和一个特定的 HTTP 方法（GET、POST 等）组成的，涉及到应用如何响应客户端对某个网站节点的访问。每一个路由都可以有一个或者多个处理器函数，当匹配到路由时，这些函数将被执行。路由的定义由如下结构组成：app.METHOD(PATH, HANDLER)。其中，app 是一个 express 实例；METHOD 是某个 HTTP 请求方式中的一个；PATH是服务器端的路径；HANDLER是当路由匹配到时需要执行的函数。

**路由方法**
* get
* post
* put
* head
* delete
* options
* trace
* copy
* lock
* mkcol
* move
* purge
* propfind
* proppatch
* unlock
* report
* mkactivity
* checkout
* merge
* m-search
* notify
* subscribe
* unsubscribe
* patch
* search
* connect
app.all() 是一个特殊的路由方法，没有任何 HTTP方法与其对应，它的作用是对于一个路径上的所有请求加载中间件。

**路由路径**
>路由路径和请求方法一起定义了请求的端点，它可以是字符串、字符串模式或者正则表达式。

1. 使用字符串
```js
// 匹配根路径的请求
app.get('/', function (req, res) {
  res.send('root');
});

// 匹配 /about 路径的请求
app.get('/about', function (req, res) {
  res.send('about');
});

// 匹配 /random.text 路径的请求
app.get('/random.text', function (req, res) {
  res.send('random.text');
});
```
2. 使用字符串模式
```js
// 匹配 acd 和 abcd
app.get('/ab?cd', function(req, res) {
  res.send('ab?cd');
});

// 匹配 abcd、abbcd、abbbcd等
app.get('/ab+cd', function(req, res) {
  res.send('ab+cd');
});

// 匹配 abcd、abxcd、abRABDOMcd、ab123cd等
app.get('/ab*cd', function(req, res) {
  res.send('ab*cd');
});

// 匹配 /abe 和 /abcde
app.get('/ab(cd)?e', function(req, res) {
 res.send('ab(cd)?e');
});
```
3. 使用正则表达式
```js
// 匹配任何路径中含有 a 的路径：
app.get(/a/, function(req, res) {
  res.send('/a/');
});

// 匹配 butterfly、dragonfly，不匹配 butterflyman、dragonfly man等
app.get(/.*fly$/, function(req, res) {
  res.send('/.*fly$/');
});
```

**托管静态文件**
>express.static函数是express内置的路由处理函数。被托管的图片、js、css等文件会被挂载在一个虚拟目录上，这些就可以通过url访问了。

挂载到项目根目录，可以多次调用，这样则按顺序查找。
```js
app.use(express.static('public'));
app.use(express.static('files'));
```
挂载到虚拟目录
```js
app.use('/static', express.static('public'));
```
现在可以通过带有 “/static” 前缀的地址来访问 public 目录下面的文件了。
```js
http://localhost:3000/static/images/kitten.jpg
```

## 错误处理

## 最佳实践(TBC)
>主要基于[express/examples](https://github.com/expressjs/express/tree/4.13.1/examples)

* 无需通过 res.render() 渲染 HTML。你可以通过 res.sendFile() 直接对外输出 HTML 文件。如果你需要对外提供的资源文件很多，可以使用 express.static() 中间件。
