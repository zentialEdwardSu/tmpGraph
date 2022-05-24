//
// Created by EdwardSu on 5/23/2022.
//

#ifndef TMPGRAPH_STRUCT_DEFINE_H
#define TMPGRAPH_STRUCT_DEFINE_H

#include<stdbool.h>


typedef struct{
    bool inUse; // Mark if the Node is in use
    int nextRelID;// id of the next relation
    int nextPropID;// id of the new property
}Node_Disk;

typedef struct{
    bool inUse;
    int NodeID_Start;
    int pre_NodeID_Start;
    int next_NodeID_Start;
    int NodeID_End;
    int pre_NodeID_End;
    int next_NodeID_End;
}Relation_Disk;

typedef struct{
    bool inUse;
    int pre_PropID;
    int next_PropID;
    long PropBlock_ID_1;
    long PropBlock_ID_2;
    long PropBlock_ID_3;
    long PropBlock_ID_4;
}Property_Disk;



#endif //TMPGRAPH_STRUCT_DEFINE_H
