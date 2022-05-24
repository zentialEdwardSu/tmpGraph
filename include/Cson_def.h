/**
 * @file CSon.h
 * @author EdwardSu 
 * @brief Json decoder and Compiler base on C
 * @version 0.0.1
 * @date 2022-05-18
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#ifndef __CSON_H
#define __CSON_H

#include"SONDS.h"

#include"List.h"
typedef enum{
    base = 0,
    INT,
    DOUBLE,
    STRING,
    LIST,
    OBJ
}TYPE;

typedef struct Jobj{
    SONDS name;//Key for the Jobj obj
    TYPE type;//Mark the type of this Jobj
    struct Jobj *pre;//Point to the previous Jobj for same level
    struct Jobj *next;//Point to the next Jobj for same level
    void *Hook;//Value of the Jobj, where the data while be mounted
}Jobj;

Jobj *Jobj_Json_translater(char *json_string);
SONDS *Jobj_Json_Compiler(Jobj *jobj_base);
Jobj Jobj_newJobj();
Jobj *Jobj_initJobj();
TYPE Jobj_judge_type(char *str);
Jobj* Jobj_seekend(Jobj *obj);

Jobj *Jobj_appendOBJ(SONDS *obj_string);
Jobj *Jobj_OBJaddKV(SONDS *s);

List *Jobj_appendList(char *list_string);
Jobj *Jobj_ListaddELE(Jobj *data_hook,Type_data add_type,void *data);

SONDS *renderList(List *list_content);
SONDS *renderOBJ(Jobj *Hook_content,int no_head);

Jobj *Jobj_Getby_Key(Jobj *obj,SONDS key);
Jobj *Jobj_Getby_Path(Jobj *obj,SONDS *path);

/* Utility to jump whitespace and cr/lf */
static const char *skip(const char *in) {while (in && *in && (unsigned char)*in<=32) in++; return in;}

void Cson_Minify(char *json){
    char *into=json;
    while (*json)
    {
        if (*json==' ') json++;
        else if (*json=='\t') json++;	/* Whitespace characters. */
        else if (*json=='\r') json++;
        else if (*json=='\n') json++;
        else if (*json=='/' && json[1]=='/')  while (*json && *json!='\n') json++;	/* double-slash comments, to end of line. */
        else if (*json=='/' && json[1]=='*') {while (*json && !(*json=='*' && json[1]=='/')) json++;json+=2;}	/* multiline comments. */
        else if (*json=='\"'){*into++=*json++;while (*json && *json!='\"'){if (*json=='\\') *into++=*json++;*into++=*json++;}*into++=*json++;} /* string literals, which are \" sensitive. */
        else *into++=*json++;			/* All other characters. */
    }
    *into=0;	/* and null-terminate. */
}

/*create base for */
Jobj Jobj_newJobj(){
    Jobj json;
    json.name = newSONDS_nomore("BASE");
    json.type = base;
    json.next = NULL;
    json.Hook = NULL;
    json.Hook = NULL;

    return json;
}

/**
 * @brief 
 * @param null noinput
 * @return Jobj* 
 */
Jobj *Jobj_initJobj(){
    Jobj *newJobj = (Jobj *)malloc(sizeof(Jobj));
    newJobj->next = NULL;
    newJobj->pre = NULL;
    newJobj->Hook = NULL;
    return newJobj;
}

/**
 * @brief 
 * 
 * @param obj_string SONDS like "A":{"b":1,"c":{"d":2}}
 * @return Jobj* - struct in json A:{}
 */
Jobj *Jobj_appendOBJ(SONDS *obj_string){
    SONDS *tmp_e = spiltSONDS(obj_string,':');
    if(nullSONDS(&tmp_e[1])){
        printf("[ERROR] Null value for obj %s\n",tmp_e[0].data);
        return NULL;
    }

    Jobj *Json_object = Jobj_initJobj();
    Json_object->type = OBJ;
    Json_object->name = newSONDS_nomore("");


    Json_object->Hook = Jobj_OBJaddKV(obj_string);

    return Json_object;
}

/**
 * @brief Add Key &value to exsiting obj
 * 
 * @param s SONDS Str like {"a":1,"b":{"c":2}}
 * @attention must start with { and end with }
 * @return Jobj* 
 */
Jobj *Jobj_OBJaddKV(SONDS *s){
    Jobj *res = NULL;
//    if(s->data[0]!='{'){
//        printf("[WARN] Invaild format of input");
//        return NULL;
//    }
    SONDS *tmp_s=NULL,*tmp_e=NULL,tmp;
    tmp = divideSONDS(s,1,-1);/*remove { and }*/
    Jobj *lastJobj=NULL,*newJobj=NULL;
    int flag = 0;
    while(true){
        lastJobj = newJobj;

        tmp_s = spiltSONDS_keepStruct(&tmp,',');/*get a slice obj from all*/
        if(nullSONDS(tmp_s)) break;
        tmp = tmp_s[1];
        tmp_e = spiltSONDS(&tmp_s[0],':');/*get name and value8*/

        newJobj = Jobj_initJobj();
        newJobj->name = tmp_e[0];
        newJobj->pre = lastJobj;
        if(newJobj->pre) newJobj->pre->next = newJobj;

        newJobj->type = Jobj_judge_type(standardizeSONDS(&tmp_e[1]));
        SONDS *tmp_value = &tmp_e[1];
        switch(newJobj->type){
            case INT:case DOUBLE:case STRING: newJobj->Hook = tmp_value; break;
            case LIST:
                newJobj->Hook = Jobj_appendList(standardizeSONDS(tmp_value));
                break;
            case OBJ:
                newJobj->Hook = Jobj_appendOBJ(tmp_value);
                break;
            case base:
                break;
        }

        if(!flag){
            flag = 1;
            res = newJobj;
        }

    }

    return res;
}

/**
 * @brief Add new List to jobj
 * @attention
 *  Input_example ["a","b"]
 * 
 * @param jobj where the List will be appended (it must be a hook for an Jobj)
 * @param list_string string List started with '[' and end with ']'
 *  
 * @return List* pointer to the new List
 */
List *Jobj_appendList(char *list_string){
    //Make obj_string SONDS to use SONDS tools
    if(*list_string != '['){
        printf("[WARN] Invaild imput format\n");
        return NULL;
    }
    Str transfer_list = newSONDS_nomore(list_string);
    
    transfer_list = divideSONDS(&transfer_list,1,-1);//clear [ and ]
    List * list = List_initList(EX_RANGE,type_Str);
    while(1)
    {
        Str *temp = spiltSONDS(&transfer_list,',');
        transfer_list = temp[1];
        if(nullSONDS(&temp[0]))  break;

        SONDS *tmp_value = &temp[0];
        switch(list->type){
            case type_int:case type_double:case type_Str: List_append(list,(&temp[0])); break;
            case type_list: 
                List_append(list,Jobj_appendList(standardizeSONDS(&temp[0])));
                break;
            case type_jobj:
                List_append(list,Jobj_appendOBJ(&temp[0]));
                break;
        }
    }

    return list;
}

/*Add an element to exsiting list*/
Jobj *Jobj_ListaddELE(Jobj *data_hook,Type_data add_type,void *data){

    List *temp_list = (List *)data_hook->Hook;
    if(temp_list->type!=add_type){
        printf("[WARN] Type of data in list and data to append doesn't match");
        return data_hook;
    }

    List_append(temp_list,data);
    return data_hook;
}

/**
 * @brief Build A Cson struct from standard & minified json string
 * 
 * @param json_string 
 * @return Jobj
 */
Jobj *Jobj_Json_translater(char *json_string){
    printf("Receive : %s",json_string);
    if(*json_string!='{'){
        printf("[Warn] invalid input string\n");
        return NULL;
    }

    Jobj *res = Jobj_appendOBJ(newSONDS_P_nomore(json_string));
    res->name = newSONDS_nomore("BASE");

    return res;

}

/**
 * @brief compile a Jobj struct to Json string 
 * 
 * @param jobj_base pointer to the BASE of the Jobj 
 * @return SONDS* 
 */
SONDS *Jobj_Json_Compiler(Jobj *jobj_base){
//    if(jobj_base->type!=base){
//        printf("[WARN] Invaild Jobj type,except 0 get %d\n",jobj_base->type);
//        return NULL;
//    }

    SONDS *res = newSONDS_P("");
    Jobj *cursor = (Jobj *)jobj_base->Hook;

    addSONDS(res,renderOBJ(cursor,0));

//    addCharSONDS(res,"}");

    return res;
}

/**
 * @brief compile a Jobj obj to Str
 * 
 * @param Hook_content 
 * @return *SONDS 
 */
SONDS *renderOBJ(Jobj *Hook_content,int no_head){
    SONDS *res = newSONDS_P("{");
    Jobj *cursor = Hook_content;
    if(cursor->type==OBJ)
        cursor = (Jobj *)cursor->Hook;
    while(cursor)
    {
        if(!no_head){
            addSONDS(res,&cursor->name);
            addCharSONDS(res,":");
        }
        switch(cursor->type){
            case INT:case DOUBLE:case STRING:
                addSONDS(res,(SONDS *)cursor->Hook);
                break;
            case OBJ:
                addSONDS(res,renderOBJ((Jobj *)cursor->Hook,0));
                break;
            case LIST:
                addSONDS(res,renderList((List *)cursor->Hook));
                break;
        }

        if(cursor->next) {
            addCharSONDS(res,",");
        }
        cursor = cursor->next;
    }

    addCharSONDS(res,"}");

    return res;
}

/**
 * @brief compile a list to SONDS
 * 
 * @param list_content 
 * @return SONDS *
 */
SONDS *renderList(List *list_content){
    SONDS *res = newSONDS_P("[");
    int index = 0;
    int diff;
    for(;diff = list_content->currentLength - index;index++)
    {
        switch(list_content->type){
            case type_int:case type_double:case type_Str:
                addSONDS(res,(SONDS *)List_getItem(list_content,index));
                break;
            case type_jobj:
                addSONDS(res,renderOBJ((Jobj *)(List_getItem(list_content,index)),0));
                break;               
            case type_list:
                addSONDS(res,renderList((List *)List_getItem(list_content,index)));
                break;
        }
        if(diff >1)
            addCharSONDS(res,",");
    }
    addCharSONDS(res,"]");
    return res;
}


/**
 * @brief 
 * 
 * @param obj Jobj to be found K&V 
 * @param path x-Path style path using '.' to querery in obj and '[int]' to querery in list ,root is BASE
 * 
 * @param NOTE >>> Input_example {A:[B:{C:1,D:2},E:{F:3}]} Querey <D> BASE.A[0].D
 * @return Jobj* 
 */
Jobj *Jobj_Getby_Path(Jobj *obj,SONDS *path){
    Jobj *res = obj;
    SONDS *s = path;
    SONDS *tmp = spiltSONDS(s,'.');
    if(!SONDS_checkSame(&tmp[0],&res->name)){
        res = Jobj_Getby_Key(res,tmp[0]);
    }
    int index = -1;
    SONDS tmp_name = tmp[0];
    SONDS tmp_last =tmp[1];
    while(1){
        if(tmp_name.length==0)  return res;
        tmp = spiltSONDS(&tmp_last,'.');
        tmp_name = tmp[0];
        tmp_last = tmp[1];

        index = kmpSONDS(&tmp_name,newSONDS_P_nomore("["));
        if(index>=0){
            SONDS *tmp_nameIndex = spiltSONDS(&tmp_name,'[');
            SONDS tmp_num_index = divideSONDS(&tmp_nameIndex[1],0,0);
            tmp_name = tmp_nameIndex[0];
            Jobj *tmp_obj = Jobj_Getby_Key(res,tmp_name);

            if(tmp_obj->type!=LIST){
                printf("[WARN] Invaild input path, no list here");
                return NULL;
            }
            res = (Jobj *)List_getItem((List *)tmp_obj->Hook,atoi(tmp_num_index.data));
        }else{
            res = Jobj_Getby_Key(res,tmp_name);
        }

    }
}

/**
 * @brief Use key to Query a Cson struct
 * 
 * @param obj 
 * @param key SONDS name of a jobj item
 * @return Jobj* 
 */
Jobj *Jobj_Getby_Key(Jobj *obj,SONDS key){
    List *stack = List_initList(5,type_jobj);
    Jobj *cursor = obj;
    
    while(cursor)
    {
        if(SONDS_checkSame(&key,&cursor->name))   return cursor;

        if(cursor->type == OBJ){
            List_append(stack,(void *)cursor);
            cursor = (Jobj *)cursor->Hook;
        }else{
            if (cursor->next){
                cursor = cursor->next;
            }else{
                if(stack->currentLength){
                    cursor = (Jobj *)List_pop(stack,-1);
                }else{
                    return NULL;
                }
            }   
        }
    }



}

TYPE Jobj_judge_type(char *str){
    switch(str[0]){
        case '"':return STRING;break;
        case '[':return LIST;break;
        case '{':return OBJ;break;
    }

    while(*str!='\0')
    {
        if (*str=='.')  return DOUBLE;
        str++;
    }

    return INT;
}

/**
 * @brief seek to the end of a obj
 * 
 * @param obj start of obj
 * @return Jobj* 
 */
Jobj* Jobj_seekend(Jobj *obj){
    while (obj->next)
    {
        obj = obj->next;
    }
    return obj;
}

#endif