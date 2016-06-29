# grunt summary V0.0.1

## plugins
### wiredep
wiredep是一个联合grunt和bower的插件。通过读取HTML/CSS/JAVASCRIPT中的标志性注释和bower.json中的库信息，在标志性注释中动态注入bower管理的库文件。

**入门**
安装插件
```js
npm install --save-dev grunt-wiredep
```
将插入写入Gruntfile.js。
```js
wiredep: {
  //需要一个target，名字不重要
  task: {
    src: [
      'app/views/**/*.html',   // .html support...
      'app/views/**/*.jade',   // .jade support...
      'app/styles/main.scss',  // .scss & .sass support...
      'app/config.yml'         // and .yml & .yaml support out of the box!
    ],
    options: {
      // See wiredep's configuration documentation for the options
      // you may pass:
      // https://github.com/taptapship/wiredep#configuration
    }
  }
}
```
在文件中写上标志性注释。
```html
<!-- bower:css -->
<!-- endbower -->
<!-- bower:js -->
<!-- endbower -->
```
安装bower库。--save或在dependencies中写入对应的版本，但是--save-dev不起作用。
```js
bower install --save bootstrap
```
运行grunt wiredep之后。
```html
<!-- bower:js -->
<script src="bower_components/jquery/dist/jquery.js"></script>
<script src="bower_components/bootstrap/dist/js/bootstrap.js"></script>
<!-- endbower -->
```

**注意**
如果是bootstrap，引入不了css。需要在bower.json中通过overrides特殊写明。
```js
"dependencies": {
    "bootstrap": "~3.3.6"
  },
  "overrides": {
    "bootstrap": {
      "main": [
        "dist/js/bootstrap.js",
        "dist/css/bootstrap.css",
        "less/bootstrap.less"
      ]
    }
  }
```
