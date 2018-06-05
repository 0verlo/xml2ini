/*************************************************************************
	> File Name: main.c
	> Author: 
	> Mail: 
	> Created Time: Mon 10 Oct 2016 05:01:35 PM CST
 ************************************************************************/

#include<stdio.h>
#include<mxml.h>
#include"main.h"

mxml_type_t type_cb(mxml_node_t *node)
{
	const char *type;
	/*
	* 你可以查看属性和/或使用XML元素名，所在层次，等等
	*/
	#if 0    
	mxml_attr_t	*elementAttrs;
	elementAttrs = node->value.element.attrs;
	if(NULL != elementAttrs)
		printf("%s\n",elementAttrs->value);
	#endif
	//printf("Name:%s \n",mxmlElementGetAttr(node,"Name")); 
	
	type = mxmlElementGetAttr(node, "type");
	if (type == NULL)
		type = node->value.element.name;
	
	if (!strcmp(type, "integer"))
		return (MXML_INTEGER);
	else if (!strcmp(type, "opaque"))
		return (MXML_OPAQUE);
	else if (!strcmp(type, "real"))
		return (MXML_REAL);
	else
		return (MXML_TEXT);
}


int main()
{
	FILE *fp;
	mxml_node_t *tree;
	mxml_node_t *node;
	mxml_node_t *nodenext;    
	mxml_node_t *nodedata;
	int fieldReadMark = 0;
	int dataReadMark = 0;
	char* pNodeValue;
	char buffer[35000] = {0};
	
	FILE * pFileOut;
	char* fileNameOut = "iqtoolConfigAll.ini";
	pFileOut = fopen(fileNameOut,"w");
	
	fp = fopen("target.xml", "r");
	if (fp == NULL){
		printf("Target.xml open failed,%s!\n",strerror(errno));
		printf("Please copy *.sav flie to this folder rename as target.xml and try again.\n");
		return 0;
	}
	tree = mxmlLoadFile(NULL, fp, type_cb);
	fclose(fp);
	
	node = tree;
	
	while(1){    
		//find title of each page
		node = mxmlFindElement(node, tree, "Page",NULL, NULL,MXML_DESCEND);  
		if(node){
			sprintf(buffer,"[%s]\n",mxmlElementGetAttr(node,"Name"));
			//small tree 2 get date in each page
			nodenext = node;
			nodedata = node;
			//write data into ini
			if(fputs(buffer, pFileOut) == EOF){
				printf("fputs() break in %s\n",buffer);
				break;
			}
			while(1){                      
				//find date in <Field></Field>
				nodenext = mxmlFindElement(nodenext, node, "Field",NULL, NULL,MXML_DESCEND);   
				if(nodenext) {
					sprintf(buffer,"    %s = %s\n",mxmlElementGetAttr(nodenext,"Name"),nodenext->child->value.text.string); 
					if(fputs(buffer, pFileOut) == EOF){
						printf("fputs() break in %s\n",buffer);
						break;
					}
				}
				else
					fieldReadMark = 1;                
				//some date store in <Data></Data>
				nodedata = mxmlFindElement(nodedata, node, "Data",NULL, NULL,MXML_DESCEND);   
				if(nodedata) {
					sprintf(buffer,"    %s = %s\n","Data",nodedata->child->value.text.string); 
					if(fputs(buffer, pFileOut) == EOF){
						printf("fputs() break in %s\n",buffer);
						break;
					}
				}
				else
					dataReadMark = 1;
				if((1 == dataReadMark) && (1 == fieldReadMark)){
					dataReadMark = 0;
					fieldReadMark = 0;
					break;
				}
			}            
		}
		else
			break;
	}
    

#if 0
	nodenext = node->child;        
	printf("nodenext = %d\n",nodenext);        
	printf("Name1:%s \n",mxmlElementGetAttr(node,"Name"));
	
	nodenext = nodenext->next;        
	printf("nodenext = %d\n",nodenext);        
	printf("Name2:%s \n",mxmlElementGetAttr(node,"Name"));
	
	node = tree->child;
	printf("node = %d",node);
	node = node->next;     
	printf("node = %d",node);   
	node = node->child;     
	printf("node = %d",node);
	printf("Movedone\n");
#endif
	fclose(pFileOut);
	mxmlDelete(tree);
	        
	printf(".sav to ini done\n");
	
	//sample_test();
	
	return 0;
}
