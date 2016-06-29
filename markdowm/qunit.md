# QUnit
> QUnit是一个javascript单元测试框架。最开始由jQuery团队开发并服务于jQuery，现在可以用于任何javascript测试。本文以[qunit cookbook](http://qunitjs.com/cookbook/)为基础。

## 使用
通过npm或bower安装qunit。
```js
npm install --save-dev qunitjs
```
```js
bower install --save-dev qunit
```
在html文件中引入qunit.js（注意不要加defer）和qunt.css。
在qunit.js引入的地方之后编写测试代码，如：
```js
<script>
    QUnit.test('a test', function(assert){
        assert.ok('1' == 1, "1 == 1?");
    });
</script>
```
在html页面中添加两个div，一个为div#qunit，一个为div#qunit-fixture，前者用于放输出的结果，后者用于保持每个单元测试的独立性（如果有dom操作，应该在qunit-fixture上操作，因为其每个测试开始之前都会重置）。

## 测试结果
第一部分为一些选项，帮助你更好的筛选结果：
1. Hide passed tests
如果有很多结果，可以很方便的只显示失败的结果。
2. Check for Globals
如果两次测试之间有改变window的属性，则失败，用于预防不小心污染全局变量。
3. No try-catch
在try-catch之外运行测试，因为代码中抛出的error会中止qunit运行。
第二部分显示navigator.userAgent的内容。比如：
> QUnit 1.23.1; Mozilla/5.0 (Windows NT 6.1) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/51.0.2704.84 Safari/537.36

第三部分是测试的汇总信息，花费多长时间、多少个通过、多少个失败。
第四部分是测试的详细结果。
1. 点击测试case可以展开显示断言的详细情况。
2. 每个case之后有一个rerun的按钮，可以重新运行单独的case
3. case的描述之后有显示有多少个通过，多少个失败。

## 断言
QUnit的基本组成单元，用于判断实际结果是否与期望相符。
### ok(truthy [, message])
如果truthy为true，则通过，否则失败。符合javascript的隐式转换规则。

### equal(actual, expected [, message])
相当于判断actual==expected，符合javascript的隐式转换规。

### deepEqual(actual, expected [, message])
相当于判断actual==expected，并且如果是对象，则比较对象内容。

## 同步回调
因为回调函数可能没有被调用，导致其中的断言没有被执行，这样不会有错误，但却是不正确的。这个时候可以使用assert.expect(times)判断这个case中有多少个断言应该被执行。
```js
QUnit.test( "a test", function( assert ) {
  assert.expect( 2 );
  function calc( x, operation ) {
    return operation( x );
  }
  var result = calc( 2, function( x ) {
    assert.ok( true, "calc() calls operation function" );
    return x * x;
  });
  assert.equal( result, 4, "2 square equals 4" );
});
```

## 异步回调 
因为QUnit不会等待异步的结果返回，所以断言会直接失败。所以要显式告诉sync代码的开始和结束。
```js
QUnit.test( "asynchronous test: async input focus", function( assert ) {
  var done = assert.async();
  var input = $("#test-input").focus();
  setTimeout(function() {
    assert.equal( document.activeElement, input[0], "Input was focused" );
    done();
  });
});
```

## 测试用户动作
QUnit没有模拟用户动作的能力，一般是通过jQuery的trigger和triggerHandler来模拟。
```js
//定义一个KeyLogger事件记录器，记录keydown发生时的keyCode
function KeyLogger( target ) {
  this.target = target;
  this.log = [];
  var that = this;
  this.target.off( "keydown" ).on( "keydown", function( event ) {
    that.log.push( event.keyCode );
  });
}

QUnit.test( "keylogger api behavior", function( assert ) {
  var doc = $( document ),
    keys = new KeyLogger( doc );
  // 触发keydown事件，并且keyCode为9
  doc.trigger( $.Event( "keydown", { keyCode: 9 } ) );
  // Verify expected behavior
  assert.deepEqual( keys.log, [ 9 ], "correct key was logged" );
});
```

## 保持测试的原子性
置于#qunit-fixture中的元素，在每次测试之后都会重置。
```js
QUnit.test( "Appends a div", function( assert ) {
  var fixture = $( "#qunit-fixture" );
  fixture.append( "<div>hello!</div>" );
  assert.equal( $( "div", fixture ).length, 1, "div added successfully!" );
});
 
QUnit.test( "Appends a span", function( assert ) {
  var fixture = $( "#qunit-fixture" );
  fixture.append("<span>hello!</span>" );
  assert.equal( $( "span", fixture ).length, 1, "span added successfully!" );
});
```

## 分组测试
有时需要在逻辑上把test cases组合在一起，这时可以使用QUnit.module(),在一个QUnit.module('A')之后的QUnit.test属于分组A，直到需要另一个QUnit.module('X')。
另一个作用是使用beforeEach和afterEach在每次测试前初始化及测试后清理环境。
```js
QUnit.module( "group a" );
QUnit.test( "a basic test example", function( assert ) {
  assert.ok( true, "this test is fine" );
});
QUnit.test( "a basic test example 2", function( assert ) {
  assert.ok( true, "this test is fine" );
});
 
QUnit.module( "group b" );
QUnit.test( "a basic test example 3", function( assert ) {
  assert.ok( true, "this test is fine" );
});
QUnit.test( "a basic test example 4", function( assert ) {
  assert.ok( true, "this test is fine" );
});
```

beforeEach和afterEach相当于jUnit中的setUp和tearDown.
```js
QUnit.module( "module", {
  beforeEach: function( assert ) {
    assert.ok( true, "one extra assert per test" );
  }, afterEach: function( assert ) {
    assert.ok( true, "and one extra assert after each test" );
  }
});
QUnit.test( "test with beforeEach and afterEach", function(assert) {
  assert.expect( 2 );
});
```

## 自定义断言
可以通过在QUnit.assert对象上添加方法来自定义断言。通过pushResult方法返回断言的结果。注意push方法已经过时。
```js
QUnit.assert.mod2 = function( value, expected, message ) {
    var actual = value % 2;
    this.pushResult( {
        result: actual === expected,
        actual: actual,
        expected: expected,
        message: message
    } );
};
 
QUnit.test( "mod2", function( assert ) {
    assert.expect( 2 );
 
    assert.mod2( 2, 0, "2 % 2 == 0" );
    assert.mod2( 3, 1, "3 % 2 == 1" );
});
```

## 提高效率
1. 点击页面上的"Hide passed tests"，如果上次运行已经pass，并且这次检测到代码无变化，将不会运行。

## 检查异常
通过throws断言。第一个参数是一个可能抛出异常的区块。第二个参数是期望结果，可以是异常函数、异常对象、匹配字符串的正则式、返回布尔值的回调函数。
```js
QUnit.test( "throws", function( assert ) {

  function CustomError( message ) {
    this.message = message;
  }

  CustomError.prototype.toString = function() {
    return this.message;
  };

  assert.throws(
    function() {
      throw "error"
    },
    "throws with just a message, not using the 'expected' argument"
  );

  assert.throws(
    function() {
      throw new CustomError("some error description");
    },
    /description/,
    "raised error message contains 'description'"
  );

  assert.throws(
    function() {
      throw new CustomError();
    },
    CustomError,
    "raised error is an instance of CustomError"
  );

  assert.throws(
    function() {
      throw new CustomError("some error description");
    },
    new CustomError("some error description"),
    "raised error instance matches the CustomError instance"
  );

  assert.throws(
    function() {
      throw new CustomError("some error description");
    },
    function( err ) {
      return err.toString() === "some error description";
    },
    "raised error instance satisfies the callback function"
  );
});
```

## API
**assert**  
> async() 提示QUnit等待异步的结果  
> deepEqual 对于原生类型，相当于===。对于数组、对象等则会比较属性。  
> equal 相当于==  
> pushResult 用于自定义断言方法，返回断言结果  
> throws 用于测试是否抛出异常  

**callback(TBC)**  
供其他工具使用，如CI服务器  
> begin  
> done  
> log 比如用于发送消息给PhantomJS  
> moduleDone  
> moduleStart  
> testDone  
> testStart  
  
**工具**  
> assert  
> config 设置配置项，如是否自动运行、是否折叠、是否隐藏通过的cases  
> extend  
> stack  

**test**  
> module  
> only 只运行这个test，其他均不运行，也不显示在页面上。  
> skip 不运行，但会在页面上显示skiped。  
> test  

## 结合grunt
如同一般的grunt插件，非常简单，安装的时候会自动下载PhantomJS.
TBC:测试使用服务器方式
```js
npm install grunt-contrib-qunit --save-dev
```

结果类似如下：
```js
Running "qunit:all" (qunit) task
Testing test/test1.html F.
>> hello test
>> Message:  1 == '2'?
>> Actual: false
>> Expected: true
>> file:///F:/ABao/work/github/qunit-test/grunt/test/test1.js:2:14

Testing test/test2.html .OK
Warning: 1/3 assertions failed (57ms) Use --force to continue.

Aborted due to warnings.
```
