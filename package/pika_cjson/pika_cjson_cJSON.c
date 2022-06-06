#include "pika_cjson_cJSON.h"
#include "cJSON.h"

void pika_cjson_cJSON_parse(PikaObj* self, char* value) {
    cJSON* item = cJSON_Parse(value);
    if (NULL == item) {
        obj_setErrorCode(self, 3);
        __platform_printf("Error: cJSON parse faild.\r\n");
        return;
    }
    obj_setPtr(self, "item", item);
    obj_setInt(self, "needfree", 1);
}

char* pika_cjson_cJSON_print(PikaObj* self) {
    cJSON* item = obj_getPtr(self, "item");
    char* res = cJSON_Print(item);
    obj_setStr(self, "_buf", res);
    cJSON_free(res);
    return obj_getStr(self, "_buf");
}

void pika_cjson_cJSON___del__(PikaObj* self) {
    cJSON* item = obj_getPtr(self, "item");
    if (obj_getInt(self, "needfree") == 1) {
        cJSON_Delete(item);
    }
}

PikaObj* pika_cjson_cJSON_getObjectItem(PikaObj* self, char* string) {
    cJSON* item = obj_getPtr(self, "item");
    cJSON* subItem = cJSON_GetObjectItem(item, string);

    /* create subCJSON */
    PikaObj* subCJSON = newNormalObj(New_pika_cjson_cJSON);

    /* init the subCJSON */
    obj_setPtr(subCJSON, "item", subItem);
    obj_setInt(subCJSON, "needfree", 0);

    return subCJSON;
}

void pika_cjson_cJSON___init__(PikaObj* self) {
    obj_setInt(self, "cJSON_Invalid", 0);
    obj_setInt(self, "cJSON_False", 1);
    obj_setInt(self, "cJSON_True", 2);
    obj_setInt(self, "cJSON_NULL", 3);
    obj_setInt(self, "cJSON_Number", 4);
    obj_setInt(self, "cJSON_String", 5);
    obj_setInt(self, "cJSON_Array", 6);
    obj_setInt(self, "cJSON_Object", 7);
    obj_setInt(self, "cJSON_Raw", 8);
}

PikaObj* pika_cjson_cJSON_getChild(PikaObj* self) {
    cJSON* item = obj_getPtr(self, "item");
    cJSON* resItem = item->child;

    /* create subCJSON */
    PikaObj* resCJSON = newNormalObj(New_pika_cjson_cJSON);

    /* init the subCJSON */
    obj_setPtr(resCJSON, "item", resItem);
    obj_setInt(resCJSON, "needfree", 0);

    return resCJSON;
}

PikaObj* pika_cjson_cJSON_getNext(PikaObj* self) {
    cJSON* item = obj_getPtr(self, "item");
    cJSON* resItem = item->next;

    /* create subCJSON */
    PikaObj* resCJSON = newNormalObj(New_pika_cjson_cJSON);

    /* init the subCJSON */
    obj_setPtr(resCJSON, "item", resItem);
    obj_setInt(resCJSON, "needfree", 0);

    return resCJSON;
}

PikaObj* pika_cjson_cJSON_getPrev(PikaObj* self) {
    cJSON* item = obj_getPtr(self, "item");
    cJSON* resItem = item->prev;

    /* create subCJSON */
    PikaObj* resCJSON = newNormalObj(New_pika_cjson_cJSON);

    /* init the subCJSON */
    obj_setPtr(resCJSON, "item", resItem);
    obj_setInt(resCJSON, "needfree", 0);

    return resCJSON;
}

char* pika_cjson_cJSON_getString(PikaObj* self) {
    cJSON* item = obj_getPtr(self, "item");
    return item->string;
}

int pika_cjson_cJSON_getType(PikaObj* self) {
    cJSON* item = obj_getPtr(self, "item");
    return item->type;
}

float pika_cjson_cJSON_getValueDouble(PikaObj* self) {
    cJSON* item = obj_getPtr(self, "item");
    return item->valuedouble;
}

int pika_cjson_cJSON_getValueInt(PikaObj* self) {
    cJSON* item = obj_getPtr(self, "item");
    return item->valueint;
}

char* pika_cjson_cJSON_getValueString(PikaObj* self) {
    cJSON* item = obj_getPtr(self, "item");
    return item->valuestring;
}

Arg* pika_cjson_cJSON_getValue(PikaObj* self) {
    cJSON* item = obj_getPtr(self, "item");
    int type = item->type;
    if (type == cJSON_Invalid) {
        return arg_setNull(NULL);
    }
    if (type == cJSON_False) {
        return arg_setInt(NULL, "", 0);
    }
    if (type == cJSON_True) {
        return arg_setInt(NULL, "", 1);
    }
    if (type == cJSON_NULL) {
        return arg_setNull(NULL);
    }
    if (type == cJSON_Number) {
        return arg_setFloat(NULL, "", item->valuedouble);
    }
    if (type == cJSON_String) {
        return arg_setStr(NULL, "", item->valuestring);
    }
    return arg_setNull(NULL);
}

PikaObj* pika_cjson_cJSON_getArrayItem(PikaObj* self, int index) {
    cJSON* item = obj_getPtr(self, "item");
    cJSON* subItem = cJSON_GetArrayItem(item, index);

    /* create subCJSON */
    PikaObj* subCJSON = newNormalObj(New_pika_cjson_cJSON);

    /* init the subCJSON */
    obj_setPtr(subCJSON, "item", subItem);
    obj_setInt(subCJSON, "needfree", 0);

    return subCJSON;
}

int pika_cjson_cJSON_getArraySize(PikaObj* self) {
    cJSON* item = obj_getPtr(self, "item");
    return cJSON_GetArraySize(item);
}

int pika_cjson_cJSON_isArray(PikaObj* self) {
    cJSON* item = obj_getPtr(self, "item");
    return cJSON_IsArray(item);
}

int pika_cjson_cJSON_isBool(PikaObj* self) {
    cJSON* item = obj_getPtr(self, "item");
    return cJSON_IsBool(item);
}

int pika_cjson_cJSON_isFalse(PikaObj* self) {
    cJSON* item = obj_getPtr(self, "item");
    return cJSON_IsFalse(item);
}

int pika_cjson_cJSON_isInvalid(PikaObj* self) {
    cJSON* item = obj_getPtr(self, "item");
    return cJSON_IsInvalid(item);
}

int pika_cjson_cJSON_isNull(PikaObj* self) {
    cJSON* item = obj_getPtr(self, "item");
    return cJSON_IsNull(item);
}

int pika_cjson_cJSON_isNumber(PikaObj* self) {
    cJSON* item = obj_getPtr(self, "item");
    return cJSON_IsNumber(item);
}

int pika_cjson_cJSON_isObject(PikaObj* self) {
    cJSON* item = obj_getPtr(self, "item");
    return cJSON_IsObject(item);
}

int pika_cjson_cJSON_isRaw(PikaObj* self) {
    cJSON* item = obj_getPtr(self, "item");
    return cJSON_IsRaw(item);
}

int pika_cjson_cJSON_isString(PikaObj* self) {
    cJSON* item = obj_getPtr(self, "item");
    return cJSON_IsString(item);
}

int pika_cjson_cJSON_isTrue(PikaObj* self) {
    cJSON* item = obj_getPtr(self, "item");
    return cJSON_IsTrue(item);
}
