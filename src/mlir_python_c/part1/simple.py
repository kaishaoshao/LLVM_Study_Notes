# python ctypes库加载动态库，调用其中的函数
import ctypes

module = ctypes.CDLL("./libsimple.so")

module.main.argtypes = []
module.main.restype = ctypes.c_int

print(module.main())
