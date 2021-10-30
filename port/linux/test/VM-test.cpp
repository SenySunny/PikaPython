#include "gtest/gtest.h"
extern "C" {
#include "BaseObj.h"
#include "PikaMain.h"
#include "PikaMath_Operator.h"
#include "PikaParser.h"
#include "PikaStdLib_SysObj.h"
#include "PikaVM.h"
#include "dataMemory.h"
#include "dataQueue.h"
#include "dataStrs.h"
}

TEST(VM, num1) {
    char* line = (char*)"1";
    Args* buffs = New_strBuff();
    char* pikaAsm = pikaParseLineToAsm(buffs, line, NULL);
    printf("%s", pikaAsm);
    PikaObj* self = newRootObj((char*)"root", New_PikaStdLib_SysObj);
    PikaObj* localArgs = pikaVM_runAsm(self, pikaAsm);
    obj_deinit(self);
    args_deinit(buffs);
    obj_deinit(localArgs);
    EXPECT_EQ(pikaMemNow(), 0);
}

TEST(VM, a_1) {
    char* line = (char*)"a = 1";
    Args* buffs = New_strBuff();
    char* pikaAsm = pikaParseLineToAsm(buffs, line, NULL);
    printf("%s", pikaAsm);
    PikaObj* self = newRootObj((char*)"root", New_PikaStdLib_SysObj);
    PikaObj* localArgs = pikaVM_runAsm(self, pikaAsm);
    ASSERT_EQ(args_getInt(localArgs->attributeList, (char*)"a"), 1);

    obj_deinit(self);
    obj_deinit(localArgs);
    args_deinit(buffs);
    EXPECT_EQ(pikaMemNow(), 0);
}

TEST(VM, a_1d1) {
    char* line = (char*)"a = 1.1";
    Args* buffs = New_strBuff();
    char* pikaAsm = pikaParseLineToAsm(buffs, line, NULL);
    printf("%s", pikaAsm);
    PikaObj* self = newRootObj((char*)"root", New_PikaStdLib_SysObj);
    PikaObj* localArgs = pikaVM_runAsm(self, pikaAsm);
    ASSERT_FLOAT_EQ(args_getFloat(localArgs->attributeList, (char*)"a"), 1.1);

    obj_deinit(self);
    obj_deinit(localArgs);
    args_deinit(buffs);
    EXPECT_EQ(pikaMemNow(), 0);
}

TEST(VM, str_xy) {
    char* line = (char*)"a = 'xy'";
    Args* buffs = New_strBuff();
    char* pikaAsm = pikaParseLineToAsm(buffs, line, NULL);
    printf("%s", pikaAsm);
    PikaObj* self = newRootObj((char*)"root", New_PikaStdLib_SysObj);
    PikaObj* localArgs = pikaVM_runAsm(self, pikaAsm);

    ASSERT_STREQ(args_getStr(localArgs->attributeList, (char*)"a"), (char*)"xy");

    obj_deinit(self);
    obj_deinit(localArgs);
    args_deinit(buffs);
    EXPECT_EQ(pikaMemNow(), 0);
}

TEST(VM, str_xy_space) {
    char* line = (char*)"a = 'xy '";
    Args* buffs = New_strBuff();
    char* pikaAsm = pikaParseLineToAsm(buffs, line, NULL);
    printf("%s", pikaAsm);
    PikaObj* self = newRootObj((char*)"root", New_PikaStdLib_SysObj);
    PikaObj* localArgs = pikaVM_runAsm(self, pikaAsm);

    ASSERT_STREQ(args_getStr(localArgs->attributeList, (char*)"a"), (char*)"xy ");

    obj_deinit(self);
    args_deinit(buffs);
    obj_deinit(localArgs);
    EXPECT_EQ(pikaMemNow(), 0);
}

TEST(VM, ref_a_b) {
    PikaObj* self = newRootObj((char*)"root", New_PikaStdLib_SysObj);
    Args* buffs = New_strBuff();

    PikaObj* localArgs = pikaVM_runAsm(
        self, pikaParseLineToAsm(buffs, (char*)"a = 'xy '", NULL));
    localArgs = pikaVM_runAsmWithLocalArgs(
        self, localArgs, pikaParseLineToAsm(buffs, (char*)"b = a", NULL));

    args_deinit(buffs);
    ASSERT_STREQ(args_getStr(localArgs->attributeList, (char*)"b"), (char*)"xy ");
    obj_deinit(self);
    obj_deinit(localArgs);
    EXPECT_EQ(pikaMemNow(), 0);
}

TEST(VM, Run_add) {
    PikaObj* self = newRootObj((char*)"root", New_PikaMath_Operator);
    Args* buffs = New_strBuff();

    PikaObj* localArgs = pikaVM_runAsm(
        self, pikaParseLineToAsm(buffs, (char*)"a = plusInt(1,2)", NULL));

    args_deinit(buffs);
    int a = args_getInt(localArgs->attributeList, (char*)"a");
    ASSERT_EQ(a, 3);
    obj_deinit(self);
    obj_deinit(localArgs);
    EXPECT_EQ(pikaMemNow(), 0);
}

TEST(VM, Run_add_multy) {
    PikaObj* self = newRootObj((char*)"root", New_PikaMath_Operator);
    Args* buffs = New_strBuff();

    PikaObj* localArgs =
        pikaVM_runAsm(self, pikaParseLineToAsm(buffs, (char*)"b = 2", NULL));
    localArgs = pikaVM_runAsmWithLocalArgs(
        self, localArgs,
        pikaParseLineToAsm(buffs, (char*)"a = plusInt(1,b)", NULL));

    args_deinit(buffs);
    int a = args_getInt(localArgs->attributeList, (char*)"a");
    ASSERT_EQ(a, 3);
    obj_deinit(self);
    obj_deinit(localArgs);
    EXPECT_EQ(pikaMemNow(), 0);
}

TEST(VM, Run_add_1_2_3) {
    PikaObj* self = newRootObj((char*)"root", New_PikaMath_Operator);
    Args* buffs = New_strBuff();

    PikaObj* localArgs = pikaVM_runAsm(
        self, pikaParseLineToAsm(buffs, (char*)"a = plusInt(1, plusInt(2,3) )",
                                 NULL));

    args_deinit(buffs);
    int a = args_getInt(localArgs->attributeList, (char*)"a");
    ASSERT_EQ(a, 6);
    obj_deinit(self);
    obj_deinit(localArgs);
    EXPECT_EQ(pikaMemNow(), 0);
}

TEST(VM, JEZ) {
    char* pikaAsm = (char*)
    "B0\n"
    "0 REF False\n"
    "0 JEZ 2\n"
    "B0\n"
    "B0\n";
    PikaObj* self = New_TinyObj(NULL);
    int lineAddr = 0;
    int size = strGetSize(pikaAsm);
    Args* sysRes = New_args(NULL);
    args_setErrorCode(sysRes, 0);
    args_setSysOut(sysRes, (char*)"");
    PikaObj* localArgs = New_TinyObj(NULL);
    lineAddr = pikaVM_runAsmLine(self, localArgs, pikaAsm, lineAddr);
    lineAddr = pikaVM_runAsmLine(self, localArgs, pikaAsm, lineAddr);
    lineAddr = pikaVM_runAsmLine(self, localArgs, pikaAsm, lineAddr);
    __clearInvokeQueues(localArgs);
    obj_deinit(self);
    args_deinit(sysRes);
    obj_deinit(localArgs);
    EXPECT_EQ(lineAddr, 26);
    EXPECT_EQ(pikaMemNow(), 0);
}

TEST(VM, JMP) {
    char* pikaAsm = (char*)
    "B0\n"
    "0 JMP 2\n"
    "B0\n"
    "B0\n";
    PikaObj* self = New_TinyObj(NULL);
    int lineAddr = 0;
    int size = strGetSize(pikaAsm);
    Args* sysRes = New_args(NULL);
    args_setErrorCode(sysRes, 0);
    args_setSysOut(sysRes, (char*)"");
    PikaObj* localArgs = New_TinyObj(NULL);
    lineAddr = pikaVM_runAsmLine(self, localArgs, pikaAsm, lineAddr);
    lineAddr = pikaVM_runAsmLine(self, localArgs, pikaAsm, lineAddr);
    __clearInvokeQueues(localArgs);
    obj_deinit(self);
    args_deinit(sysRes);
    obj_deinit(localArgs);
    EXPECT_EQ(lineAddr, 14);
    EXPECT_EQ(pikaMemNow(), 0);
}

TEST(VM, JMP_back1) {
    char* pikaAsm = (char*)
    "B0\n"
    "B0\n"
    "0 JMP -1\n"
    "B0\n"
    "B0\n";
    PikaObj* self = New_TinyObj(NULL);
    int lineAddr = 0;
    int size = strGetSize(pikaAsm);
    Args* sysRes = New_args(NULL);
    args_setErrorCode(sysRes, 0);
    args_setSysOut(sysRes, (char*)"");
    PikaObj* localArgs = New_TinyObj(NULL);
    lineAddr = pikaVM_runAsmLine(self, localArgs, pikaAsm, lineAddr);
    lineAddr = pikaVM_runAsmLine(self, localArgs, pikaAsm, lineAddr);
    lineAddr = pikaVM_runAsmLine(self, localArgs, pikaAsm, lineAddr);
    __clearInvokeQueues(localArgs);
    obj_deinit(self);
    args_deinit(sysRes);
    obj_deinit(localArgs);
    EXPECT_EQ(lineAddr, 0);
    EXPECT_EQ(pikaMemNow(), 0);
}

extern PikaMemInfo pikaMemInfo;
TEST(VM, WHILE) {
    pikaMemInfo.heapUsedMax = 0;
    Args* buffs = New_strBuff();
    char* lines =(char *)
        "a = 1\n"
        "b = 0\n"
        "while a:\n"
        "    b = 1\n"
        "    a = 0\n"
        "\n";
    printf("%s", lines);
    char* pikaAsm = pikaParseMultiLineToAsm(buffs, (char*)lines);
    printf("%s", pikaAsm);
    pikaMemInfo.heapUsedMax = 0;
    PikaObj* self = New_TinyObj(NULL);
    PikaObj* localArgs = pikaVM_runAsm(self, pikaAsm);
    EXPECT_EQ(args_getInt(localArgs->attributeList, (char*)"a"), 0);
    EXPECT_EQ(args_getInt(localArgs->attributeList, (char*)"b"), 1);
    obj_deinit(localArgs);
    args_deinit(buffs);
    obj_deinit(self);
    EXPECT_EQ(pikaMemNow(), 0);
}

TEST(VM, a_1_1) {
    char* line = (char*)"a = 1 + 1";
    Args* buffs = New_strBuff();
    char* pikaAsm = pikaParseLineToAsm(buffs, line, NULL);
    printf("%s", pikaAsm);
    PikaObj* self = newRootObj((char*)"root", New_PikaStdLib_SysObj);
    PikaObj* localArgs = pikaVM_runAsm(self, pikaAsm);

    int res = args_getInt(localArgs->attributeList, (char*)"a");
    obj_deinit(self);
    obj_deinit(localArgs);
    args_deinit(buffs);
    ASSERT_EQ(res, 2);
    EXPECT_EQ(pikaMemNow(), 0);
}

TEST(VM, a_1_1d1) {
    char* line = (char*)"a = 1 + 1.1";
    Args* buffs = New_strBuff();
    char* pikaAsm = pikaParseLineToAsm(buffs, line, NULL);
    printf("%s", pikaAsm);
    PikaObj* self = newRootObj((char*)"root", New_PikaStdLib_SysObj);
    PikaObj* localArgs = pikaVM_runAsm(self, pikaAsm);

    float res = args_getFloat(localArgs->attributeList, (char*)"a");
    obj_deinit(self);
    obj_deinit(localArgs);
    args_deinit(buffs);
    ASSERT_FLOAT_EQ(res, 2.1);
    EXPECT_EQ(pikaMemNow(), 0);
}

TEST(VM, a_jjcc) {
    char* line = (char*)"a = (1 + 1.1) * 3 - 2 /4.0";
    Args* buffs = New_strBuff();
    char* pikaAsm = pikaParseLineToAsm(buffs, line, NULL);
    printf("%s", pikaAsm);
    PikaObj* self = newRootObj((char*)"root", New_PikaStdLib_SysObj);
    PikaObj* localArgs = pikaVM_runAsm(self, pikaAsm);

    float res = args_getFloat(localArgs->attributeList, (char*)"a");
    obj_deinit(self);
    args_deinit(buffs);
    obj_deinit(localArgs);
    ASSERT_FLOAT_EQ(res, 5.8);
    EXPECT_EQ(pikaMemNow(), 0);
}

TEST(VM, while_a_1to10) {
    char* line = (char*)
    "a = 0\n"
    "while a < 10:\n"
    "    a = a + 1\n"
    "    print(a)\n"
    "\n";
    Args* buffs = New_strBuff();
    char* pikaAsm = pikaParseMultiLineToAsm(buffs, line);
    printf("%s", pikaAsm);
    PikaObj* self = newRootObj((char*)"root", New_PikaStdLib_SysObj);
    PikaObj* localArgs = pikaVM_runAsm(self, pikaAsm);

    int res = args_getInt(localArgs->attributeList, (char*)"a");
    obj_deinit(self);
    args_deinit(buffs);
    obj_deinit(localArgs);
    ASSERT_FLOAT_EQ(res, 10);
    EXPECT_EQ(pikaMemNow(), 0);
}

TEST(VM, while_a_d_x) {
    char* line = (char*)
    "mem = PikaStdLib.MemChecker()\n"
    "mem.x = 1\n"
    "print(mem.x)\n"
    "mem.x = 2\n"
    "print(mem.x)\n"
    "\n";
    Args* buffs = New_strBuff();
    char* pikaAsm = pikaParseMultiLineToAsm(buffs, line);
    printf("%s", pikaAsm);
    PikaObj* self = newRootObj((char*)"", New_PikaMain);
    PikaObj* localArgs = pikaVM_runAsm(self, pikaAsm);

    int res = args_getInt(localArgs->attributeList, (char*)"mem.x");
    obj_deinit(self);
    obj_deinit(localArgs);
    args_deinit(buffs);
    ASSERT_FLOAT_EQ(res, 2);
    EXPECT_EQ(pikaMemNow(), 0);
}

TEST(VM, DEF_instruct) {
    char* pikaAsm = (char*)
    "B0\n"
    "0 DEF test()\n"
    "B0\n"
    "0 JMP 1\n"
    "B1\n"
    "0 NUM 1\n"
    "0 RET\n"
    "B0\n";
    char* methodCode = (char*)
    "B1\n"
    "0 NUM 1\n"
    "0 RET\n"
    "B0\n";
    PikaObj* self = New_TinyObj(NULL);
    int lineAddr = 0;
    int size = strGetSize(pikaAsm);
    PikaObj* localArgs = pikaVM_runAsm(self, pikaAsm);
    char* methodPtr = (char*)obj_getPtr(self, (char*)"test");
    EXPECT_STREQ(methodCode, methodPtr);
    obj_deinit(self);
    EXPECT_EQ(pikaMemNow(), 0);
}

TEST(VM, RET_instruct) {
    char* pikaAsm = (char*)
    "B1\n"
    "0 NUM 13\n"
    "0 RET\n"
    "0 NUM 2\n"
    "0 RET\n"
    "B0\n";
    PikaObj* self = New_TinyObj(NULL);
    int lineAddr = 0;
    int size = strGetSize(pikaAsm);
    PikaObj* localArgs = pikaVM_runAsm(self, pikaAsm);
    Arg* returnArg = args_getArg(localArgs->attributeList, (char*)"return");
    int num = arg_getInt(returnArg);
    EXPECT_EQ(num, 13);
    obj_deinit(self);
    obj_deinit(localArgs);
    EXPECT_EQ(pikaMemNow(), 0);
}

TEST(VM, RUN_DEF) {
    char* pikaAsm = (char*)
    "B0\n"
    "0 DEF test()\n"
    "B0\n"
    "0 JMP 1\n"
    "B1\n"
    "0 NUM 1\n"
    "0 RET\n"
    "B0\n"
    "0 RUN test\n"
    "0 OUT a\n";
    PikaObj* self = New_TinyObj(NULL);
    PikaObj* localArgs = pikaVM_runAsm(self, pikaAsm);
    int num = obj_getInt(self, (char*)"a");
    obj_deinit(self);
    EXPECT_EQ(pikaMemNow(), 0);
}