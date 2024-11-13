# ReleaseButler

[![GPL v2 license](https://img.shields.io/badge/license-GPL_2-blue.svg)](https://raw.githubusercontent.com/suoyuan666/tlog/master/LICENSE)

~~当前项目仍在开发阶段，所以软件会输出很多不必要信息~~(现在添加了 `verbose` 这个 option)

~~ReleaseButler 旨在简化Linux系统上的 Github 上的软件安装过程🤗。 它可以自动检测底层 Linux 发行版，从指定的 GitHub 项目主页获取对应的软件包并安装，同时也支持更新操作。~~

目前我是想要实现一个类似 BSD 系“类 ports 系统”那样的东西，就像 Arch Linux 的 [ABS](https://wiki.archlinux.org/title/Arch_build_system)，或者像是 Gentoo Linux 的 [protage](https://wiki.gentoo.org/wiki/Portage)。目前想要的甚至还没 ABS 那么复杂，Gentoo Linux 那样标记 USE 变量更是想都没想了。

TODO:

- [ ] 与 libapt-pkg 和 libdnf 等集成
- [ ] 在 GitHub 创建一个仓库存储软件包安装的json文件，用户可以直接 import 之后安装
- [ ] 支持更多的软件(源码)分发平台，但目前就只是做了对 GitHub 的处理

---

~~⚠️: 你必须首先安装**sudo**而不是**open-doas**或是其他类似的软件，如果你不想强依赖于**sudo**可以选择修改源代码😀。或者我后续想起来会尝试添加对sudo或doas的检测。~~ 我已经添加了对 `sudo` 和 `doas` 的检测。

> [!NOTE]
>
> 我这里假设了 `sudo` 和 `doas` 会存在于 /usr/bin 目录下

---

## 特点:

**自动发行版检测**：ReleaseButler 确定运行环境的Linux发行版，确保与各种系统的兼容性。

**安装和更新**：使用单个命令轻松安装软件包。 ReleaseButler 支持更新，使您安装的软件保持最新。

**尽量适配各种情况**：尝试引入各种字段以适配各种仓库的安装步骤。

## 使用方法

### 安装软件:

```bash
$ releasebutler [--verbose] install --package <name> --pakname <package name> --from <url>
```

`--package` 字段接受的是要安装的软件的名称，而 `packname` 字段是这个软件包名，例如 `fastfetch-linux-amd64.deb` 就是 `packname`

从命令行安装尚未支持所有字段（如 `install`，`build` 等）。

### 更新

```bash
$ relesebutler --update
```

### 解析外部引入的json文件

```bash
$ relesebutler --parse <file name>
```

note: 这个文件需要在 **~/.config/ReleaseButler/** 目录中

---

> [!NOTE]
> ReleaseButler 现在还不支持指定软件包的单独更新。🙃
>
> 使用命令行安装的软件是记录在 **~/.config/ReleaseButler/info.json**，但是在更新的时候实际上会遍历 **~/.config/ReleaseButler/** 目录下的所有json文件。
>
> 我尝试通过这种方式支持引入其他地方来的json文件。但文件名不能叫 **package.json**，这个文件用来记录已安装软件包的版本，遍历到了这个json文件也会跳过。

### 调试模式

你可以附加 `--verbose` 得到更多的调试信息输出

```bash
$ relesebutler --verbose
```

---

## 软件包信息文件的字段定义:

```json
{
   "fastfetch": {
      "pakname": "fastfetch-linux-amd64.deb",
      "url": "https://github.com/fastfetch-cli/fastfetch",
      "version": "2.13.2",
      "build" : [
         "touch something",
         "touch haaa"
      ],
      "install" : [
         "echo something > something"
      ],
      "clone": false,
      "download" : false
   }
}
```

- `pakname` 是 Release 页面的软件包名称。
- `build` 用于存储一些软件下载前的要执行操作。
- `install` 用于存储一些软件安装后要执行的操作。
- `clone` 表示是否直接 clone 这个仓库。
- `download` 表示是否只是单纯在从Release下载了软件包，而不是下载了之后调用系统软件包管理器安装它。

## 支持的Linux发行版

- Debian/Ubuntu/Deepin
- Fedora

---

## 如何编译

如果你在 Debian 发行版下编译该项目:

```bash
$ sudo apt install libcurl4-openssl-dev clang cmake
$ git clone https://github.com/suoyuan666/ReleaseButler.git
$ cd ReleaseButler
$ git submodule update --init --recursive
$ cmake -B build -DCMAKE_BUILD_TYPE=Release
$ cmake --build build -j `nproc`
```

如果是在其他平台下编译，原谅我懒了，自己去寻找 libcurl4-openssl-dev 这个包对应其他发型版的软件包名吧。😛

我尝试在 OpenSUSE Tumbleweed 中编译这个项目，我是用了下边的语句安装了所需的软件

```bash
$ sudo zypper install libcurl-devel clang18 llvm18-gold cmake
```
