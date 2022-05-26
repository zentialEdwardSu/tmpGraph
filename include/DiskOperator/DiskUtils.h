//
// Created by lin on 5/24/2022.
//

#ifndef TMPGRAPH_DISKUTILS_H
#define TMPGRAPH_DISKUTILS_H

#include<stdio.h>
#include"GraphStruct.h"

typedef enum{
    Error = 0,
    OK
}Diskutils_status;

typedef int write_chunk_count;

extern FILE *DiskUtils_get_META_filehandle_read(char *Graph_name);
extern FILE *DiskUtils_get_META_filehandle_write(char *Graph_name);
extern FILE *DiskUtils_get_Node_filehandle_read(char *Graph_name);
extern FILE *DiskUtils_get_Node_filehandle_write(char *Graph_name);
extern FILE *DiskUtils_get_String_filehandle_read(char *Graph_name);
extern FILE *DiskUtils_get_String_filehandle_write(char *Graph_name);
extern FILE *DiskUtils_get_Relation_filehandle_read(char *Graph_name);
extern FILE *DiskUtils_get_Relation_filehandle_write(char *Graph_name);
extern FILE *DiskUtils_get_PropName_filehandle_read(char *Graph_name);
extern FILE *DiskUtils_get_PropName_filehandle_write(char *Graph_name);
extern FILE *DiskUtils_get_String_filehandle_read(char *Graph_name);
extern FILE *DiskUtils_get_String_filehandle_write(char *Graph_name);

extern Diskutils_status DiskUtils_write_Property(Property *property,FILE *Property_filehandle);
extern Property *DiskUtils_read_Property(FILE *Property_filehandle);
extern Diskutils_status DiskUtils_write_Gpah_META(MGraph *Graph, FILE *META_filehandle);
extern MGraph *DiskUtils_read_Graph_META(FILE *META_filehandle);
extern Diskutils_status DiskUtils_write_Graph_Node(MNode_info *Node, FILE *Node_Filehandle);
extern MNode_info *DiskUtils_read_Graph_Node(FILE *Node_Filehandle);
extern write_chunk_count DiskUtils_write_String(char *String,unsigned int StringID,FILE *String_filehandle);
extern char *DiskUtils_read_String(FILE *String_filehandle);
extern Diskutils_status DiskUtils_write_Relation(MNode *relation,FILE *Relation_filehandle);
extern MNode *DiskUtils_read_Relation(FILE *Relation_filehandle);
extern Diskutils_status DiskUtils_write_PropName(PropName *propname,FILE *PropName_filehandle);
extern PropName *DiskUtils_read_PropName(FILE *PropName_filehandle);

/*For Unused file*/

extern FILE *DiskUtils_get_Unused_Node_filehandle_read(char *Graph_name);
extern FILE *DiskUtils_get_Unused_Node_filehandle_write(char *Graph_name);
extern FILE *DiskUtils_get_Unused_Relation_filehandle_read(char *Graph_name);
extern FILE *DiskUtils_get_Unused_Relation_filehandle_write(char *Graph_name);
extern FILE *DiskUtils_get_Unused_Prop_filehandle_read(char *Graph_name);
extern FILE *DiskUtils_get_Unused_Prop_filehandle_write(char *Graph_name);
extern FILE *DiskUtils_get_Unused_String_filehandle_read(char *Graph_name);
extern FILE *DiskUtils_get_Unused_String_filehandle_write(char *Graph_name);
extern FILE *DiskUtils_get_Unused_PropName_filehandle_read(char *Graph_name);
extern FILE *DiskUtils_get_Unused_PropName_filehandle_write(char *Graph_name);


extern Diskutils_status DiskUtils_write_Unused_Node_list(Unused_Node_list *list,FILE *Unused_Node_filehandle);
extern Unused_Node_list *DiskUtils_read_Unused_Node_list(FILE *Unused_Node_filehandle);
extern Diskutils_status DiskUtils_write_Unused_Relation_list(Unused_Relation_list *list,FILE *Unused_Relation_filehandle);
extern Unused_Relation_list *DiskUtils_read_Unused_Relation_list(FILE *Unused_Relation_filehandle);
extern Diskutils_status DiskUtils_write_Unused_Prop_list(Unused_Prop_list *list,FILE *Unused_Prop_filehandle);
extern Unused_Prop_list *DiskUtils_read_Unused_Prop_list(FILE *Unused_Prop_filehandle);
extern Diskutils_status DiskUtils_write_String_Prop_list(Unused_String_list *list,FILE *Unused_String_filehandle);
extern Unused_String_list *DiskUtils_read_String_Prop_list(FILE *Unused_String_filehandle);
extern Diskutils_status DiskUtils_write_Unused_PropName_list(Unused_PropName_list *list,FILE *Unused_PropName_filehandle);
extern Unused_PropName_list *DiskUtils_read_Unused_PropName_list(FILE *Unused_PropName_filehandle);
/* ↑ for unused file*/

#endif //TMPGRAPH_DISKUTILS_H
