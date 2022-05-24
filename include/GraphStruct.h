//
// Created by EdwardSu on 5/23/2022.
//

#ifndef TMPGRAPH_GRAPHSTRUCT_H
#define TMPGRAPH_GRAPHSTRUCT_H

typedef unsigned char tmpgraph_Bool;

typedef struct{
    char *name; //name of Graph
    int Total_Node_num;
    int Total_relation_num;
}Grap_Meta_Data;
/*Graph in Disk stage*/

/**
 * @brief Node struct in Disk
 */
typedef struct{
    tmpgraph_Bool inUse; // Mark if the Node is in use
    int nextRelID;// id of the next relation
    int nextPropID;// id of the new property
}Node_Disk;

typedef struct{
    tmpgraph_Bool inUse;
    int NodeID_Start;
    int pre_NodeID_Start;
    int next_NodeID_Start;
    int NodeID_End;
    int pre_NodeID_End;
    int next_NodeID_End;
}Relation_Disk;

typedef struct{
    tmpgraph_Bool inUse;
    int pre_PropID;
    int next_PropID;
    long PropBlock_ID_1;
    long PropBlock_ID_2;
    long PropBlock_ID_3;
    long PropBlock_ID_4;
}Property_Disk;

/* Graph in memory state*/

/**
 * @brief Arc Node
 */
typedef struct ArcNode{
    int adjvex;//Index of this Arc node
    struct ArcNode * nextarc;//next Arc node
    int PropID;// ID of property of this relation
}ArcNode;

/**
 * @brief Vertex Node
 */
typedef struct VNode{
    int PropID;// ID of property of this Node
    ArcNode * firstarc;//Pointer to the ArcNode
}VNode;

typedef struct {
    VNode *vertices;//Vertex Array
    int vexnum,arcnum;//Log vertex number and Adjacency Node number
    int kind;//记录图的种类
}ALGraph;


#endif //TMPGRAPH_GRAPHSTRUCT_H
