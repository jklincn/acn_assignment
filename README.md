# 实验环境

- WSL 1.2.5.0
- Ubuntu 22.04.3 LTS
- ns-3.40
- g++ 11.4.0
- cmake  3.22.1
- Python 3.10.12

# 快速开始

## 下载源代码

```
git clone 
```

## 安装 ns-3

（适用于Ubuntu 22.04）

```
cd acn_assignment
sudo apt update
sudo apt install g++ python3 cmake ninja-build
wget https://www.nsnam.org/releases/ns-allinone-3.40.tar.bz2
tar xfj ns-allinone-3.40.tar.bz2
cd ns-allinone-3.40/ns-3.40
./ns3 configure --enable-examples --enable-tests
./ns3 build
# （可选）测试编译是否成功
./test.py
./ns3 run hello-simulator
```

