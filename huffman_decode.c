/***************************************/
/*     HUFFMAN DECODING PROGRAM        */
/* CREATED BY ADEESHA SAVINDA DE SILVA */
/*           12 June 2016              */
/*     adeesha.savinda@gmail.com       */
/***************************************/

//Improved version with no buffer required.
//i.e. .huffman file of any size can be decoded back
#include <stdio.h>
#include <string.h>
#include<stdlib.h>

//structure used to define a node
typedef struct node_t {
	struct node_t *left, *right;
	int freq;
	char c;
} *node;

//global variables
int n_nodes = 0, qend = 1; 		//global variables for keep track of no.of nodes and end of the que
struct node_t pool[256] = {{0}};        //pool of nodes
node qqq[255], *q = qqq-1;      	//the priority que
char buf[1024];				//a string array of the codes for each letter

//function used to create a new node
node new_node(int freq, char c, node a, node b)
{
	node n = pool + n_nodes++;
	if (freq != 0){
		n->c = c;			//assign the character 'c' to the character of the node (eventually a leaf)
		n->freq = freq;			//assign frequency
	}
	else {
		n->left = a, n->right = b;	//if there is no frequency provided with the invoking
		n->freq = a->freq + b->freq;	//the removed nodes at the end of the que will be added to left and right
	}
	return n;
}

//function ued to insert a node into the priority que
void qinsert(node n)
{
	int j, i = qend++;
	while ((j = i / 2)) {
		if (q[j]->freq <= n->freq) break;
		q[i] = q[j], i = j;
	}
	q[i] = n;
}

node qremove()
{
	int i, l;
	node n = q[i = 1];

	if (qend < 2) return 0;
	qend--;
	while ((l = i * 2) < qend) {
		if (l + 1 < qend && q[l + 1]->freq < q[l]->freq) l++;
		q[i] = q[l], i = l;
	}
	q[i] = q[qend];
	return n;	//return the node
}

void import_table(FILE *fp_table, unsigned int *freq){
	char c;						//temporary variable
	int i = 0;

	while((c=fgetc(fp_table))!=EOF){
                freq[i++] = (unsigned char)c; 		//get the values from the .table file to update the huffman tree
	}
	for (i = 0; i < 128; i++)
		if (freq[i]) qinsert(new_node(freq[i], i, 0, 0));	//insert new nodes into the que if there is a frequency
	while (qend > 2)
		qinsert(new_node(0, 0, qremove(), qremove()));		//build the tree
}

void decode(FILE *fp_huffman,FILE *fp_out){
	int i=0,lim=0,j=0;
	char c;
	node n = q[1];

	fscanf(fp_huffman,"%d",&lim);			//get the length of the bit stream from header
	fseek(fp_huffman,1,SEEK_CUR);			//seek one position to avoid new line character of the header

	printf("Decoded : \n");
	for(i=0; i<lim; i++){
		if(j==0)
			c = fgetc(fp_huffman);
		//if the anding of the character with b1000 0000 is true then,
		if(c&128)	n = n->right;		//1 go right
		else 		n = n->left;            //else go left
                if(n->c){                               //until a leaf (node with a character) meets
                        putchar(n->c);                  //spit that character out and
                        fputc(n->c,fp_out);             //save the character in file
                        n = q[1];                       //reset the que
                }
		c = c<<1;				//shift the character by 1
		if(++j>7)
			j = 0;
	}

	putchar('\n');
	if (q[1] != n) printf("garbage input\n");	//the last node should end with a character which reset the que
}

int main(int argc, char* argv[]){
	FILE *fp_table,*fp_huffman,*fp_out;		//FIFO pointers
	char file_name[50]={0},temp[50]={0};		//file name
	unsigned int freq[128] = {0};			//frequency of the letters

	system("clear");
	printf("**********************************************************************\n");
	printf("                      COMMUNICATION  ENGINEERING\n");
	printf("                                SHU M.Eng\n");
	printf("                            -HUFFMAN DECODER-\n");
	printf("**********************************************************************\n\n");


	if( argc == 2 ) {
		strcpy(file_name,argv[1]);				//commandline argument directly allows to compress the file
		if(strstr(file_name,"huffman") == NULL){
			printf("\nERROR:wrong file format!\n");
			return 0;
		}
	}
   	else if( argc > 2 ) {
      		printf("Too many arguments supplied.\n");
   	}
   	else {
		printf("Please enter the file to be compressed\t: "); 	//else a prompt comes to enter the file name
		scanf("%s",file_name);
		if(strstr(file_name,"huffman") == NULL){
			printf("\nERROR:wrong file format!\n");
			return 0;
		}
   	}

	//import the frequency table and compressed from file and create the huffman tree
	if((fp_huffman = fopen(file_name,"r"))==NULL){
		printf("\nERROR: No such file\n");
		return 0;
	}
	strcat(file_name,".table");			//file pointer for .table file
	if((fp_table = fopen(file_name,"r"))==NULL){	//open the file stream
		printf("\nERROR: Frequency table cannot be found\n");
		return 0;
	}
	import_table(fp_table,freq);			//import the file and fills frequency array

	*strstr(file_name,".huffman") = '\0';		//search for .huffman and remove it
	strcpy(temp,"mkdir ");				//concatenating strings for the command
	strcat(temp,file_name);				//to make a directory with the file name
	system(strcat(temp,".decoded"));		//with .decoded at the end

	strcpy(temp,"./");				//concatenating string to save the
	strcat(temp,file_name);				//file with the original name in the .decoded derectory
	strcat(temp,".decoded/");
	if((fp_out = fopen(strcat(temp,file_name),"w"))==NULL){
		printf("ERROR:Creating decoded file failed\n");	//if any error occured durig creating file; exit
		return 0;
	}
	decode(fp_huffman,fp_out);			//decode the file and save

	fclose(fp_huffman);				//close the file pointer for .huffman file
	fclose(fp_table);				//close the file pointer for huffman frequency table file
	fclose(fp_out);					//close file pointer for output file
	return 0;
}
