# connect
>connect是node.js平台一个非常流行的中间件。相对于node.js基础的http模块，只要非常简单的代码就能实现一个服务器。connect是一个非常简单的框架，能够将中间件粘合一起实现丰富的功能。本文基于[connect的github页面](https://github.com/senchalabs/connect)

## 使用
安装。
```js
npm install connect
```
创建app，用于保存添加的中间件。
```js
var app = connect();
```
使用中间件。connect的核心就是使用中间件，中间件保存在堆中，当请求符合，就会一个接一个地执行中间件，直到中间件没有调用next()。req/res就是使用node.js中的req/res。
```js
app.use(function middleware1(req, res, next) {
  // middleware 1
  next();
});
app.use(function middleware2(req, res, next) {
  // middleware 2
  next();
});
```
挂载中间件。中间件可以挂载到相应的URL上。
```js
app.use('/foo', function fooMiddleware(req, res, next) {
  // req.url starts with "/foo"
  next();
});
app.use('/bar', function barMiddleware(req, res, next) {
  // req.url starts with "/bar"
  next();
});
```
处理错误的中间件。注意有4个参数，第一个参数为错误对象。
```js
app.use(function onerror(err, req, res, next) {
  // an error occurred!
});
```
启动服务器（server），最后一步就是用.listen()方法启动http服务器
```js
var server = app.listen(port);
```
或者使用node.js的createServer方法。
```js
var server = http.createServer(app);
```

## 常用中间件

