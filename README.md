# File Scanner

一个用 **C 语言** 编写的递归文件扫描工具，支持 **按文件大小、类型、名称子串进行过滤**。项目结构清晰，模块化设计，适合作为 **Linux/C 系统编程练习或基础工具**。

---

## ✨ 功能特性

* 📁 递归扫描指定目录
* 🔍 多条件过滤（AND 逻辑）

  * 按最小文件大小过滤
  * 按文件类型过滤（普通文件 / 目录 / 符号链接）
  * 按文件名子串过滤
* 🧩 模块化设计

  * 文件信息抽象（`FileInfo`）
  * 通用链表容器（`List`）
  * 可扩展过滤器系统（`FilterSet` + 注册表）
* 🛠 适合扩展新的过滤规则

---

## 📂 项目结构

```text
.
├── src
│   ├── main.c                 # 程序入口
│   ├── scanner.c/.h           # 目录递归扫描
│   ├── file_info.c/.h         # 文件信息封装
│   ├── list.c/.h              # 通用单向链表
│   ├── filter.c/.h            # 过滤器系统
│   ├── filter_registry.c/.h   # 过滤器注册表
│   ├── options.c/.h           # 命令行参数解析
├── README.md
```

---

## 🔧 编译

推荐使用 **GCC + 严格警告选项**：

```bash
gcc -Wall -Wextra -Werror -g \
    src/*.c \
    -o file_scanner
```

> ⚠️ 强烈建议开启 `-Wall -Wextra -Werror`，可以在编译期避免大量 C 语言隐患。

---

## ▶️ 使用方法

```bash
./file_scanner <path> [options]
```

### 参数说明

| 参数               | 说明         |
| ---------------- | ---------- |
| `<path>`         | 起始扫描路径（必选） |
| `--min-size=N`   | 最小文件大小（字节） |
| `--type=regular` | 只匹配普通文件    |
| `--type=dir`     | 只匹配目录      |
| `--name=xxx`     | 文件名包含子串    |

### 示例

```bash
# 扫描 /etc 下大于 1KB 的文件
./file_scanner /etc --min-size=1024

# 扫描当前目录下的普通文件
./file_scanner . --type=regular

# 扫描 /usr 下文件名包含 conf 的文件
./file_scanner /usr --name=conf
```

---

## 🧠 设计说明

### 1️⃣ FileInfo（文件信息抽象）

* 封装 `lstat` 获取的文件元数据
* 自动解析文件类型
* 深拷贝路径与文件名，生命周期清晰

### 2️⃣ List（通用链表）

* 存储任意 `void*` 数据
* 支持自定义 `data_free` 回调
* 被用于：

  * 扫描结果集合
  * 过滤器集合

### 3️⃣ FilterSet（过滤器系统）

* 每个过滤规则由：

  ```c
  bool (*filter_func)(const FileInfo*, const void *arg);
  ```
* 所有过滤器 **AND 关系** 执行
* 参数 `arg` 由调用者分配，统一在销毁时释放

### 4️⃣ 过滤器注册表

```c
static FilterEntry g_filters[] = {
    { "size", filter_by_size },
    { "type", filter_by_type },
    { "name", filter_by_name },
};
```

* 新增过滤器只需：

  1. 实现函数
  2. 注册到表中

---

## 🚨 已避免的常见 C 语言陷阱

* ❌ 隐式函数声明（已通过头文件显式声明）
* ❌ 函数指针非法跳转
* ❌ basename 修改原字符串
* ❌ 内存所有权不清导致泄漏

---

## 🧪 调试

推荐使用 GDB：

```bash
gdb ./file_scanner
(gdb) run /etc --min-size=1024
```

---

## 📌 可扩展方向

* [ ] 支持最大文件大小（`--max-size`）
* [ ] 支持正则表达式文件名过滤
* [ ] 输出排序（按大小 / 时间）
* [ ] 并发扫描（`pthread`）
* [ ] JSON / CSV 输出

---

## 📜 License

MIT License

---

## 🙌 致谢

本项目用于学习与实践 **Linux / C 系统编程**，欢迎 Fork / Issue / PR。
