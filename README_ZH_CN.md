# ReleaseButler

[English](./README.md) [简体中文](./README_ZH_CN.md)

**当前项目仍在开发阶段，所以软件会输出很多不必要信息**

~~ReleaseButler旨在简化Linux系统上的Github上的软件安装过程🤗。 它可以自动检测底层Linux发行版，从指定的GitHub项目主页获取对应的软件包并安装，同时也支持更新操作。~~

目前我是想要实现一个类似BSD系“类ports系统”那样的东西，就像Arch Linux的[ABS](https://wiki.archlinux.org/title/Arch_build_system)，或者像是Gentoo Linux的[protage](https://wiki.gentoo.org/wiki/Portage)。目前想要的甚至还没ABS那么复杂，Gentoo Linux那样标记USE变量更是想都没想了。

Todo:

- [ ] 使用JSON格式存储软件包的信息，(仍然在考虑使用什么格式存储信息)
- [ ] 使用`build`字段存储一些在install之后执行的工作。
- [ ] 使用`ready`字段存储一些install之前的准备工作。
- [ ] 我想要实现成如果不提供软件包的名字的话就默认直接clone仓库，准备开始编译它，但目前还没做这方面的检测。
- [ ] 想要在GitHub创建一个仓库存储软件包安装的json文件，这样用户可以直接import之后安装。话说这样就好像nix(但我就是因为懒得学习nix语法，我才执着自己写一个)
- [ ] 目前使用的是sudo安装软件，没做对当前软件的用户权限是否是非root用户，并且也没做对sudo的检测(准备实现成如果没有sudo就检测是否存在doas)。

---

⚠️: 你必须首先安装**sudo**而不是**open-doas**或是其他类似的软件，如果你不想强依赖于**sudo**可以选择修改源代码😀。或者我后续想起来会尝试添加对sudo或doas的检测。

---

## 特点:

**自动发行版检测：** ReleaseButler 确定运行环境的Linux发行版，确保与各种系统的兼容性。

**安装和更新**：使用单个命令轻松安装软件包。 ReleaseButler支持更新，使您安装的软件保持最新。(Todo)

**尽量适配各种情况**: 尝试引入各种字段以适配各种仓库的安装步骤。(Todo)

## 使用方法

### 安装软件:

```bash
$ relesebutler --install <homepage> --packname <name>
```

### 更新 (Todo)

```bash
$ relesebutler --update
```

---

note: ReleaseButler现在还不支持指定软件包的单独更新。🙃

---

## 支持的Linux发行版

- Debian/Ubuntu
- Fedora

---

## 如何编译

如果你在Debian OS下编译该项目:

```bash
$ sudo apt install libcurl4-openssl-dev clang
$ git clone https://github.com/suoyuan666/ReleaseButler.git
$ cd ReleaseButler
$ cmake -B build -DCMAKE_BUILD_TYPE=Release
$ cmake --build build -j `nproc`
```

如果是在其他平台下编译，原谅我懒了，自己去寻找libcurl4-openssl-dev这个包对应其他发型版的软件包名吧。😛

我在 **CMakeLists.txt** 中写明了使用`clang`编译，因为其中一些编译选项我使用的是`clang`的，如果你希望使用`gcc`编译，也许你还需要修改一下编译选项。就是这部分：

```CMakeLists
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    message(STATUS "Configuring Release build")
    # something come form https://airbus-seclab.github.io/c-compiler-security/clang_compilation.html
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -pipe -fPIE -Wall -Wextra -Wpedantic -Werror -Wthread-safety")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fstack-clash-protection -fstack-protector-all -fcf-protection=full -fvisibility=hidden -fsanitize=cfi")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -flto -fvisibility=hidden -fsanitize=cfi")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -fsanitize=integer -fsanitize-minimal-runtime -fno-sanitize-recover")
endif()
```