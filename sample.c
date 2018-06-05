#include<stdio.h>
#include<mxml.h>
#include"main.h"

int sample_test()  
{  
    FILE *fp;  
    mxml_node_t *tree,*node;  
  
    fp = fopen("debug_settings.xml", "r");  
    tree = mxmlLoadFile(NULL, fp,MXML_TEXT_CALLBACK);  
    fclose(fp);  
  
    mxml_node_t *id,*password;  
  
    node = mxmlFindElement(tree, tree, "note",NULL, NULL,MXML_DESCEND);  
  
    printf(" year:%s \n",mxmlElementGetAttr(node,"year"));  
    printf(" date:%s \n",mxmlElementGetAttr(node,"date"));  
    printf(" month:%s \n",mxmlElementGetAttr(node,"month"));  
  
    id = mxmlFindElement(node, tree, "id",NULL, NULL,MXML_DESCEND);  
    printf("[%s}\n",id->child->value.text.string);      
    password = mxmlFindElement(node, tree, "password",NULL, NULL,MXML_DESCEND);    
    printf("[%s]\n",password->child->value.text.string);    
  
    mxmlDelete(tree);  
  
    return 0 ;  
}  
