# ubuntu
系统设置及软件安装
## 系统设置
### bios
先把bios设置为Legacy模式  
### 双系统
在win7上安装easyBCD，重写MBR以win7引导
## 更改源
> sudo /etc/apt/sources.list 
> sudo apt-get update 更新源 
> sudo apt-get upgrade 升级已安装的软件

15.04的网易源：  
```
deb http://mirrors.163.com/ubuntu/ vivid main restricted universe multiverse
deb http://mirrors.163.com/ubuntu/ vivid-security main restricted universe multiverse
deb http://mirrors.163.com/ubuntu/ vivid-updates main restricted universe multiverse
deb http://mirrors.163.com/ubuntu/ vivid-proposed main restricted universe multiverse
deb http://mirrors.163.com/ubuntu/ vivid-backports main restricted universe multiverse
deb-src http://mirrors.163.com/ubuntu/ vivid main restricted universe multiverse
deb-src http://mirrors.163.com/ubuntu/ vivid-security main restricted universe multiverse
deb-src http://mirrors.163.com/ubuntu/ vivid-updates main restricted universe multiverse
deb-src http://mirrors.163.com/ubuntu/ vivid-proposed main restricted universe multiverse
deb-src http://mirrors.163.com/ubuntu/ vivid-backports main restricted universe multiverse
```
其他源详见[Ubuntu论坛源列表](http://wiki.ubuntu.com.cn/%E6%BA%90%E5%88%97%E8%A1%A8)

## 安装软件

### pdnsd  
ubuntu初始的网速慢，需安装软件改善  
__安装__

> sudo apt-get install pdnsd

安装过程中，它会问你选择何种配置，此处选择manual。  
__配置__

> sudo vi /etc/pdnsd.conf

修改
```
server {
label="resolvconf";
}
```
为
```
server {
label="resolvconf";
ip=208.67.222.222;
ip=208.67.220.220;
timeout=30;
interval=30;
uptest=ping;
ping_timeout=50;
purge_cache=off;
}  
```
设置/etc/default/pdnsd的`START_DAEMON=yes`属性，否则启动不了  
__设置本机的DNS__

> sudo vi /etc/resolv.conf

在所有内容之前插入一行：`nameserver 127.0.0.1`  
特别的，如果你是ADSL拨号上网(通过pppoeconf配置)，那么还需要修改一下文件：  
> sudo vi /etc/ppp/peers/dsl-provider

在usepeerdns前加#，注释掉。防止ppp强制使用远程DNS，并覆盖 resolv.conf  
如果你启用了DHCP服务，那么要  

> sudo gedit /etc/dhcp3/dhclient.conf

去掉`#prepend domain-name-servers 127.0.0.1;`前面的“#“。  
__启动pdnsd__

> sudo /etc/init.d/pdnsd start

__简单测试__

> dig yahoo.com | grep time  

### 安装Git
> sudo apt-get install git-all  
> git --version

### node
> sudo apt-get install nodejs
> sudo apt-get install npm
或者从github下载源码安装

### tmux
> sudo apt-get install tmux

### jdk
> sudo apt-get install openjdk-8-jdk

### sublime-text3
> sudo add-apt-repository ppa:webupd8team/sublime-text-3
> sudo apt-get update
> sudo apt-get install sublime-text-installer

### chromium
> sudo apt-get install chromium-browser
