/**
 * @author EdwardSu
 * @file DiskUtils.c
 * @brief DiskUtils function
 */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>
#include<math.h>
#include"DiskUtils.h"
//#include"SONDS.h"

char *_connect_string(char *s1,char *s2){
    int s1_size = strlen(s1);
    int s2_size = strlen(s2);

    char *res = (char *)malloc((s1_size+s2_size+1));
    memcpy(res,s1,s1_size);
    char *tmp = res+s1_size;
    memcpy(tmp,s2,s2_size);
    res[s1_size+s2_size] = '\0';

    return res;
}

FILE *DiskUtils_get_META_filehandle_read(char *Graph_name){
    char *path = _connect_string(Graph_name,"/tmpgraphstore");

    return fopen(path,"rb");
}

FILE *DiskUtils_get_META_filehandle_write(char *Graph_name){
    char *path = _connect_string(Graph_name,"/tmpgraphstore");

    return fopen(path,"wb");
}
/**
 *
 * @brief Write META data to "(Graphname)/tmpGraphstore"
 * @note it won't close file
 * @param Graph
 * @param META_filehandle wb (Graphname)/tmpGraphstore
 * @return Diskutils_status OK/Error 1 / 0
 */
Diskutils_status DiskUtils_write_Gpah_META(MGraph *Graph, FILE *META_filehandle) {

    Graph_Meta_Data_Disk write_data;
    fwrite(Graph->name,65,1,META_filehandle);
    write_data.Total_Node_num = Graph->Relanum;
    write_data.Total_relation_num = Graph->Nodenum;
    write_data.Current_NodeID = Graph->Current_NodeID;
    write_data.Current_PropertyID = Graph->Current_PropertyID;
    write_data.Current_RelationID = Graph->Current_RelationID;

    fwrite(&write_data, sizeof(Graph_Meta_Data_Disk), 1, META_filehandle);

    return OK;
}

/**
 * @b
 * @brief Read META data from "(Graphname)/tmpGraphstore"
 * @note it won't close file
 * @param META_filehandle rb (Graphname)/tmpGraphstore
 * @return MGraph *
 */
MGraph *DiskUtils_read_Graph_META(FILE *META_filehandle){
    MGraph *Graph = (MGraph *)malloc(sizeof(MGraph));
    Graph_Meta_Data_Disk *tmp = (Graph_Meta_Data_Disk *)malloc(sizeof(Graph_Meta_Data_Disk));
    memset(tmp, 0,sizeof(Graph_Meta_Data_Disk));

    fread(Graph->name,65,1,META_filehandle);

    fread(tmp, sizeof(Graph_Meta_Data_Disk), 1, META_filehandle);


    Graph->Nodenum = tmp->Total_Node_num;
    Graph->Relanum = tmp->Total_relation_num;

    Graph->Graph = NULL;
    Graph->kind = -1;

    Graph->Current_NodeID = tmp->Current_NodeID;
    Graph->Current_PropertyID = tmp->Current_PropertyID;
    Graph->Current_RelationID = tmp->Current_RelationID;

    Graph->unusedNodeList = NULL;
    Graph->unusedRelationList = NULL;
    Graph->unusedPropList = NULL;
    Graph->unusedStringList = NULL;


    return Graph;
}



FILE *DiskUtils_get_Node_filehandle_read(char *Graph_name){
    char *path = _connect_string(Graph_name,"/tmpgraphstore.node");

    return fopen(path,"rb");
}

FILE *DiskUtils_get_Node_filehandle_write(char *Graph_name){
    char *path = _connect_string(Graph_name,"/tmpgraphstore.node");

    return fopen(path,"wb");
}

Diskutils_status DiskUtils_write_Graph_Node(MNode_info *Node, FILE *Node_Filehandle){
    int count = fwrite(Node,sizeof(Node_Disk),1,Node_Filehandle);

    return count>=1?OK : Error;
}

MNode_info *DiskUtils_read_Graph_Node(FILE *Node_Filehandle){
    MNode_info *Node_info= (MNode_info *)malloc(sizeof(MNode_info));

    int read_count = fread(Node_info,sizeof(Node_Disk),1,Node_Filehandle);

    if(read_count<1){
        free(Node_info);
        return NULL;
    }else{
        return Node_info;
    }
}



FILE *DiskUtils_get_Relation_filehandle_read(char *Graph_name){
    char *path = _connect_string(Graph_name,"/tmpgraphstore.relationstore");

    return fopen(path,"rb");
}

FILE *DiskUtils_get_Relation_filehandle_write(char *Graph_name){
    char *path = _connect_string(Graph_name,"/tmpgraphstore.relationstore");

    return fopen(path,"wb");
}

Diskutils_status DiskUtils_write_Relation(MNode *relation,FILE *Relation_filehandle){
    Relation_Disk *write_data = (Relation_Disk *)malloc(sizeof(Relation_Disk));

    write_data->inUse = relation->inUse;
    write_data->NodeID_Start = relation->NodeID_A;
    write_data->NodeID_End = relation->NodeID_B;
    write_data->next_NodeID_Start = relation->next_NodeA?relation->next_NodeA->RelationID:0;
    write_data->pre_NodeID_Start = relation->pre_NodeA?relation->pre_NodeA->RelationID:0;
    write_data->next_NodeID_End = relation->next_NodeB?relation->next_NodeB->RelationID:0;
    write_data->pre_NodeID_End = relation->pre_NodeB?relation->pre_NodeB->RelationID:0;
    write_data->relation_Property_ID = relation->relation_Property_ID;

    int write_count = fwrite(write_data,sizeof(Relation_Disk),1,Relation_filehandle);

    if(write_count < 1)
        return Error;

    free(write_data);
    return OK;
}

/**
 * @brief read relation from disk
 * @param Relation_filehandle must be adjusted ahead
 * @return MNode
 * @note you should give the return MNode it's relation ID
 */
MNode *DiskUtils_read_Relation(FILE *Relation_filehandle){
    MNode *res = (MNode *)malloc(sizeof(MNode));
    Relation_Disk *buf = (Relation_Disk *)malloc(sizeof(Relation_Disk));
    res->next_NodeA = NULL;
    res->pre_NodeA = NULL;
    res->next_NodeB = NULL;
    res->pre_NodeB = NULL;
    res->RelationID = 0;

    fread(buf,sizeof(Relation_Disk),1,Relation_filehandle);

    res->inUse = buf->inUse;
    res->NodeID_A = buf->NodeID_Start;
    res->NodeID_B = buf->NodeID_End;
    res->relation_Property_ID = buf->relation_Property_ID;

    free(buf);
    return res;
}



FILE *DiskUtils_get_PropName_filehandle_read(char *Graph_name){
    char *path = _connect_string(Graph_name,"/tmpgraphstore.propertystore.name");

    return fopen(path,"rb");
}

FILE *DiskUtils_get_PropName_filehandle_write(char *Graph_name){
    char *path = _connect_string(Graph_name,"/tmpgraphstore.propertystore.name");

    return fopen(path,"wb");
}

Diskutils_status DiskUtils_write_PropName(PropName *propname,FILE *PropName_filehandle){
    PropName_Disk *write_tmp = (PropName_Disk *)malloc(sizeof(PropName_Disk));

    write_tmp->inUse = propname->inUse;

    fwrite(write_tmp,sizeof(PropName_Disk),1,PropName_filehandle);

//    fseek(PropName_filehandle,sizeof(PropName_Disk),ftell)
    int write_count = 0;
    write_count = fwrite(propname->string_data,65,1,PropName_filehandle);

    free(write_tmp);

    return write_count>0?OK:Error;
}

PropName *DiskUtils_read_PropName(FILE *PropName_filehandle){
    char *str = (char *)malloc(65);
    memset(str,0,65);
    PropName *res = (PropName *)malloc(sizeof(PropName));

    PropName_Disk tmp;

    fread(&tmp,sizeof(PropName_Disk),1,PropName_filehandle);

    res->inUse = tmp.inUse;

    fread(str,65,1,PropName_filehandle);

    res->string_data = str;

    return res;
}



// TODO Poperty 简单加载到内存
FILE *DiskUtils_get_String_filehandle_read(char *Graph_name){
    char *path = _connect_string(Graph_name,"/tmpgraphstore.propertystore");

    return fopen(path,"rb");
}

FILE *DiskUtils_get_String_filehandle_write(char *Graph_name){
    char *path = _connect_string(Graph_name,"/tmpgraphstore.propertystore");

    return fopen(path,"wb");
}

Diskutils_status DiskUtils_write_Property(Property *property,FILE *Property_filehandle){
    int count = 0;
    count+= fwrite(property,sizeof(Property_Disk),1,Property_filehandle);
    count+= fwrite(property->data_hook,16,1,Property_filehandle);

    return count>1?OK:Error;
}

Property *DiskUtils_read_Property(FILE *Property_filehandle){
    Property *res = (Property *)malloc(sizeof(Property));
    void *data = malloc(16);

    fread(res,sizeof(Property_Disk),1,Property_filehandle);
    fread(data,16,1,Property_filehandle);

    res->data_hook = data;

    return res;
}

FILE *DiskUtils_get_Poperty_filehandle_read(char *Graph_name){
    char *path = _connect_string(Graph_name,"/tmpgraphstore.propertystore.string");

    return fopen(path,"rb");
}

FILE *DiskUtils_get_Poperty_filehandle_write(char *Graph_name){
    char *path = _connect_string(Graph_name,"/tmpgraphstore.propertystore.string");

    return fopen(path,"wb");
}

write_chunk_count DiskUtils_write_String(char *String,unsigned int StringID,FILE *String_filehandle){
    unsigned int string_length = strlen(String);
    unsigned int write_size = string_length;
    write_chunk_count count = (int) ceil(string_length/128);
    int round = count;
    unsigned int CurrentID = StringID;

    while(round>=1){
        write_size = round > 1? 128 : strlen(String);
        String_Disk write_data;
        write_data.inUse = 1;
        write_data.extend_string = round > 1? 1 : 0 ;
        write_data.extend_string_block_ID = round > 1? CurrentID+1 :CurrentID;
        write_data.string_length = write_size;

        fwrite(&write_data,sizeof(write_data),1,String_filehandle);

//        fseek(String_filehandle,sizeof(write_data), ftell(String_filehandle));

        fwrite(String,write_size,1,String_filehandle);

        if(round > 1){
            String+=128;
        }
        round--;
    }

    return count;
}

char *DiskUtils_read_String(FILE *String_filehandle){
    char *res = "\0";
    char *buf = (char *)malloc(129);
    String_Disk *tmp_st = (String_Disk *)malloc(sizeof(String_Disk));

    do {
        fread(tmp_st,sizeof(String_Disk),1,String_filehandle);

//        fseek(String_filehandle,sizeof(String_Disk),ftell(String_filehandle));

        fread(buf,128,1,String_filehandle);

        buf[129] = '\0';

        res = _connect_string(res,buf);

    }while(tmp_st->extend_string == 1);

    free(buf);
    return res;

}




/*for unused file*/

FILE *DiskUtils_get_Unused_PropName_filehandle_read(char *Graph_name){
    char *path = _connect_string(Graph_name,"/tmpgraphstore.propertystore.name.id");

    return fopen(path,"rb");
}

FILE *DiskUtils_get_Unused_PropName_filehandle_write(char *Graph_name){
    char *path = _connect_string(Graph_name,"/tmpgraphstore.propertystore.name.id");

    return fopen(path,"wb");
}

Diskutils_status DiskUtils_write_Unused_PropName_list(Unused_PropName_list *list,FILE *Unused_PropName_filehandle){
    int write_count = 0;
    write_count = fwrite(&list->length,sizeof(unsigned int),1,Unused_PropName_filehandle);

    if(write_count<1){
        return Error;
    }
    write_count = 0;

//    fseek(Unused_PropName_filehandle,sizeof(unsigned int), ftell(Unused_PropName_filehandle));

    write_count = fwrite(list->unused_PropName,(list->length*sizeof(unsigned int)),1,Unused_PropName_filehandle);

    if(write_count<1){
        return Error;
    }else{
        return OK;
    }
}

Unused_PropName_list *DiskUtils_read_Unused_PropName_list(FILE *Unused_PropName_filehandle){
    Unused_PropName_list *list = (Unused_PropName_list *)malloc(sizeof(Unused_PropName_list));

    fread(&list->length,sizeof(unsigned int),1,Unused_PropName_filehandle);

//    fseek(Unused_PropName_filehandle,sizeof(unsigned int), ftell(Unused_PropName_filehandle));

    fread(list->unused_PropName,(list->length*sizeof(unsigned int)),1,Unused_PropName_filehandle);

    return list;
}

FILE *DiskUtils_get_Unused_Node_filehandle_read(char *Graph_name){
    char *path = _connect_string(Graph_name,"/tmpgraphstore.node.id");

    return fopen(path,"rb");
}

FILE *DiskUtils_get_Unused_Node_filehandle_write(char *Graph_name){
    char *path = _connect_string(Graph_name,"/tmpgraphstore.node.id");

    return fopen(path,"wb");
}

Diskutils_status DiskUtils_write_Unused_Node_list(Unused_Node_list *list,FILE *Unused_Node_filehandle){
    int write_count = 0;
    write_count = fwrite(&list->length,sizeof(unsigned int),1,Unused_Node_filehandle);

    if(write_count<1){
        return Error;
    }
    write_count = 0;

//    fseek(Unused_Node_filehandle,sizeof(unsigned int), ftell(Unused_Node_filehandle));

    write_count = fwrite(list->unused_Node,(list->length*sizeof(unsigned int)),1,Unused_Node_filehandle);

    if(write_count<1){
        return Error;
    }else{
        return OK;
    }
}

Unused_Node_list *DiskUtils_read_Unused_Node_list(FILE *Unused_Node_filehandle){
    Unused_Node_list *list = (Unused_Node_list *)malloc(sizeof(Unused_Node_list));

    fread(&list->length,sizeof(unsigned int),1,Unused_Node_filehandle);

//    fseek(Unused_Node_filehandle,sizeof(unsigned int), ftell(Unused_Node_filehandle));

    fread(list->unused_Node,(list->length*sizeof(unsigned int)),1,Unused_Node_filehandle);

    return list;
}

FILE *DiskUtils_get_Unused_Relation_filehandle_read(char *Graph_name){
    char *path = _connect_string(Graph_name,"/tmpgraphstore.relationstore.id");

    return fopen(path,"rb");
}

FILE *DiskUtils_get_Unused_Relation_filehandle_write(char *Graph_name){
    char *path = _connect_string(Graph_name,"/tmpgraphstore.relationstore.id");

    return fopen(path,"wb");
}

Diskutils_status DiskUtils_write_Unused_Relation_list(Unused_Relation_list *list,FILE *Unused_Relation_filehandle){
    int write_count = 0;
    write_count = fwrite(&list->length,sizeof(unsigned int),1,Unused_Relation_filehandle);

    if(write_count<1){
        return Error;
    }
    write_count = 0;

//    fseek(Unused_Relation_filehandle,sizeof(unsigned int), ftell(Unused_Relation_filehandle));

    write_count = fwrite(list->unused_Relation,(list->length*sizeof(unsigned int)),1,Unused_Relation_filehandle);

    if(write_count<1){
        return Error;
    }else{
        return OK;
    }
}

Unused_Relation_list *DiskUtils_read_Unused_Relation_list(FILE *Unused_Relation_filehandle){
    Unused_Relation_list *list = (Unused_Relation_list *)malloc(sizeof(Unused_Relation_list));

    fread(&list->length,sizeof(unsigned int),1,Unused_Relation_filehandle);

//    fseek(Unused_Relation_filehandle,sizeof(unsigned int), ftell(Unused_Relation_filehandle));

    fread(list->unused_Relation,(list->length*sizeof(unsigned int)),1,Unused_Relation_filehandle);

    return list;
}

FILE *DiskUtils_get_Unused_Prop_filehandle_read(char *Graph_name){
    char *path = _connect_string(Graph_name,"/tmpgraphstore.propertystore.id");

    return fopen(path,"rb");
}

FILE *DiskUtils_get_Unused_Prop_filehandle_write(char *Graph_name){
    char *path = _connect_string(Graph_name,"/tmpgraphstore.propertystore.id");

    return fopen(path,"wb");
}

Diskutils_status DiskUtils_write_Unused_Prop_list(Unused_Prop_list *list,FILE *Unused_Prop_filehandle){
    int write_count = 0;
    write_count = fwrite(&list->length, sizeof(unsigned int), 1, Unused_Prop_filehandle);

    if(write_count<1){
        return Error;
    }
    write_count = 0;

//    fseek(Unused_Prop_filehandle, sizeof(unsigned int), ftell(Unused_Prop_filehandle));

    write_count = fwrite(list->unused_Property, (list->length*sizeof(unsigned int)), 1, Unused_Prop_filehandle);

    if(write_count<1){
        return Error;
    }else{
        return OK;
    }
}

Unused_Prop_list *DiskUtils_read_Unused_Prop_list(FILE *Unused_Prop_filehandle){
    Unused_Prop_list *list = (Unused_Prop_list *)malloc(sizeof(Unused_Prop_list));

    fread(&list->length,sizeof(unsigned int),1,Unused_Prop_filehandle);

//    fseek(Unused_Prop_filehandle,sizeof(unsigned int), ftell(Unused_Prop_filehandle));

    fread(list->unused_Property,(list->length*sizeof(unsigned int)),1,Unused_Prop_filehandle);

    return list;
}

FILE *DiskUtils_get_Unused_String_filehandle_read(char *Graph_name){
    char *path = _connect_string(Graph_name,"/tmpgraphstore.propertystore.string.id");

    return fopen(path,"rb");
}

FILE *DiskUtils_get_Unused_String_filehandle_write(char *Graph_name){
    char *path = _connect_string(Graph_name,"/tmpgraphstore.propertystore.string.id");

    return fopen(path,"wb");
}

Diskutils_status DiskUtils_write_String_Prop_list(Unused_String_list *list,FILE *Unused_String_filehandle){
    int write_count = 0;
    write_count = fwrite(&list->length, sizeof(unsigned int), 1, Unused_String_filehandle);

    if(write_count<1){
        return Error;
    }
    write_count = 0;

//    fseek(Unused_String_filehandle, sizeof(unsigned int), ftell(Unused_String_filehandle));

    write_count = fwrite(list->unused_String, (list->length*sizeof(unsigned int)), 1, Unused_String_filehandle);

    if(write_count<1){
        return Error;
    }else{
        return OK;
    }
}

Unused_String_list *DiskUtils_read_String_Prop_list(FILE *Unused_String_filehandle){
    Unused_String_list *list = (Unused_String_list *)malloc(sizeof(Unused_String_list));

    fread(&list->length, sizeof(unsigned int), 1, Unused_String_filehandle);

//    fseek(Unused_String_filehandle, sizeof(unsigned int), ftell(Unused_String_filehandle));

    fread(list->unused_String, (list->length*sizeof(unsigned int)), 1, Unused_String_filehandle);

    return list;
}