# ReleaseButler
[简体中文](./REDME_ZH_CN.md)

ReleaseButler旨在简化Linux系统上的Github上的软件安装过程。 它可以自动检测底层Linux发行版，从指定的GitHub项目主页获取对应的软件包并安装，同时也支持更新操作。

## Feature:

**自动发行版检测：** ReleaseButler 确定运行环境的Linux发行版，确保与各种系统的兼容性。

**安装和更新**：使用单个命令轻松安装软件包。 ReleaseButler还支持更新，使您安装的软件保持最新。

## 安装

```bash
$ git clone https://github.com/your-usernameautoinstaller.git
$ cd autoinstaller
$ make install
```

## 使用方法

### 安装软件:

```bash
$ relesebutler --install <package> --form <url>
```

### 更新

```bash
$ relesebutler --update
```

---

note: ReleaseButler现在还不支持指定软件包的单独更新。

---

## 支持的Linux发行版

- Debian/Ubuntu
- Fedora

---

一些发行版（比如Arch Linux）有自己的用户软件仓库，很大程度上不会需要这个软件（我认为）。所以ReleaseButler目前只会支持Debian和Fedora（我自己用的是Debian，Fedora是我顺手加的支持，我自己没有测试过）。

---

## 如何编译

如果你在Debian OS下编译该项目:

```bash
$ sudo apt install libcurl4-openssl-dev
$ git clone https://github.com/your-usernameautoinstaller.git
$ cd autoinstaller
$ make relesebutler
```

如果是在其他平台下编译，原谅我懒了，自己去寻找libcurl4-openssl-dev这个包对应其他发型版的软件报名吧。