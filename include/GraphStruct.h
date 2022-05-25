//
// Created by EdwardSu on 5/23/2022.
//

#ifndef TMPGRAPH_GRAPHSTRUCT_H
#define TMPGRAPH_GRAPHSTRUCT_H

#include <stdint.h>

#define type_long_long 1
#define type_double 2
#define type_string 3
#define type_list 4
#define type_list_member_int 41
#define type_list_member_long 42
#define type_list_member_float 43
#define type_list_member_double 44
#define type_list_member_string 45

typedef unsigned char tmpgraph_Bool;
typedef uint8_t string_size;
typedef uint8_t data_type;

typedef struct Grap_Meta_Data_Disk{
    //65 bytes of name
    unsigned int Total_Node_num;
    unsigned int Total_relation_num;
    /*Record current ID*/
    unsigned int Current_NodeID;
    unsigned int Current_RelationID;
    unsigned int Current_PropertyID;

}Graph_Meta_Data_Disk;// 65 bytes of name + 5 uint16

/*Graph in Disk stage*/

/**
 * @brief Node struct in Disk
 */
typedef struct{
    tmpgraph_Bool inUse; // Mark if the Node is in use
    unsigned int nextRelID;// id of the next relation
    unsigned int nextPropID;// id of the new property
}Node_Disk;

typedef struct{
    tmpgraph_Bool inUse;

    unsigned int NodeID_Start;
    unsigned int pre_NodeID_Start;
    unsigned int next_NodeID_Start;

    unsigned int NodeID_End;
    unsigned int pre_NodeID_End;
    unsigned int next_NodeID_End;

    unsigned int relation_Property_ID;
}Relation_Disk;

typedef struct{
    tmpgraph_Bool inUse;
    unsigned int pre_PropID;
    unsigned int next_PropID;

    data_type PropType_A;
    data_type PropType_B;

    unsigned int PropName_A_ID;//ID to PropName_Disk
    unsigned int PropName_B_ID;//ID to PropName_Disk

    // 16 bytes
}Property_Disk;// + 8 + 8 bytes of data

typedef struct PropName_Disk{
    tmpgraph_Bool inUse;
//    string_size length;
    //65bytes of char
}PropName_Disk;

typedef struct{
    tmpgraph_Bool inUse;//if in use
    tmpgraph_Bool extend_string;// if the string size > 128 mark
    unsigned int extend_string_block_ID;
    string_size string_length;  //stick to 128 bit
}String_Disk;

//TODO: add type arr
//typedef struct{
//    tmpgraph_Bool inUse;// Mark if in use
//    tmpgraph_Bool extend_arr;// if the arr is size >256
//

typedef struct Unused_Node_list{
    unsigned int * unused_Node;
    unsigned int length;
}Unused_Node_list;

typedef struct Unused_Relation_list{
    unsigned int * unused_Relation;
    unsigned int length;
}Unused_Relation_list;

typedef struct Unused_Prop_list{
    unsigned int * unused_Property;
    unsigned int length;
}Unused_Prop_list;

typedef struct Unused_String_list{
    unsigned int * unused_String;
    unsigned int length;
}Unused_String_list;

typedef struct Unused_PropName_list{
    unsigned int * unused_PropName;
    unsigned int length;
}Unused_PropName_list;

/* Graph in memory state*/
typedef struct MNode{
    tmpgraph_Bool inUse;
    unsigned int RelationID;
    unsigned int NodeID_A;//start Node ID
    struct MNode *next_NodeA;//next MNode start with A
    struct MNode *pre_NodeA;//previous MNode start with A

    unsigned int NodeID_B;//end Node ID
    struct MNode *next_NodeB;//next MNode end with B
    struct MNode *pre_NodeB;//previous MNode end with B

    unsigned int relation_Property_ID;//Property Block of this MNode
}MNode;

typedef struct{
    tmpgraph_Bool inUse; // Mark if the Node is in use
    unsigned int nextRelID;// id of the next relation
    unsigned int nextPropID;// id of the new property
}MNode_info;

typedef struct {
    char *name;
    MNode *Graph;//Vertex Array
    unsigned int Nodenum;//vertex number (relation number)
    unsigned int Relanum;//Adjacency Node number (Node number)
    int kind;// 1 directed 0 undirected -1 undefined

    /*Record current ID*/
    unsigned int Current_NodeID;
    unsigned int Current_RelationID;
    unsigned int Current_PropertyID;

    /*Record unused length*/
    Unused_Node_list *unusedNodeList;
    Unused_Relation_list  *unusedRelationList;
    Unused_Prop_list *unusedPropList;
    Unused_String_list *unusedStringList;
    Unused_PropName_list *unusedPropNameList;
}MGraph;

typedef struct{
    tmpgraph_Bool inUse;
    unsigned int pre_PropID;
    unsigned int next_PropID;

    data_type PropType_A;
    data_type PropType_B;

    unsigned int PropName_A_ID;//ID to PropName_Disk
    unsigned int PropName_B_ID;//ID to PropName_Disk

    void *data_hook;//16 bytes hook
}Property;

typedef struct PropName{
    tmpgraph_Bool inUse;
    char *string_data;
    //65bytes of char
}PropName;

//TODO 定义 所有结构的大小
const unsigned int Size_Graph_META_Disk = sizeof(Size_Graph_META_Disk);
//const unsigned int Size_Node_Disk = sizeof
const unsigned int Size_PropName_Disk = sizeof(PropName_Disk)+65;
const unsigned int Size_Property_Disk = sizeof(Property_Disk)+16;
#endif //TMPGRAPH_GRAPHSTRUCT_H
