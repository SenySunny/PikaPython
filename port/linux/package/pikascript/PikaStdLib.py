from PikaObj import *


class MemChecker(TinyObj):
    def max():
        pass

    def now():
        pass

    def getMax() -> float:
        pass

    def getNow() -> float:
        pass

    def resetMax():
        pass


class SysObj(BaseObj):
    def type(arg: any):
        pass

    def remove(argPath: str):
        pass

    def int(arg: any) -> int:
        pass

    def float(arg: any) -> float:
        pass

    def str(arg: any) -> str:
        pass

    def iter(arg: any) -> any:
        pass

    def next(arg: any) -> any:
        pass

    def range(a1: int, a2: int, a3: int) -> any:
        pass

    def __set__(obj: any, key: any, val: any, obj_str: str):
        pass

    def __get__(obj: any, key: any) -> any:
        pass


class RangeObj(TinyObj):
    def __next__() -> any:
        pass


class StringObj(TinyObj):
    def __next__() -> any:
        pass

class PikaObj(TinyObj):
    pass
