/***************************************/
/*     HUFFMAN DECODING PROGRAM        */
/* CREATED BY ADEESHA SAVINDA DE SILVA */
/*           12 June 2016              */
/*     adeesha.savinda@gmail.com       */
/***************************************/

#include <stdio.h>
#include <string.h>

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
char *code[128] = {0}, buf[1024];	//a string array of the codes for each letter
int input_data=0,output_data=0;

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

//go along the builded huffman tree and assign the code for each character
void build_code(node n, char *s, int len)
{
	static char *out = buf;
	if (n->c) {
		s[len] = 0;		//if the provided node is a leaf (end node)
		strcpy(out, s);		//it contains a character
		code[(int)n->c] = out;	//therefore the code is copied in to the relevant character.
		out += len + 1;		//out pointer is incremented
		return;
	}

	s[len] = '0'; build_code(n->left,  s, len + 1);	//recurring is used to write out the code
	s[len] = '1'; build_code(n->right, s, len + 1);	//if right add a 1 and if right add a 0
}

void import_file(FILE *fp_in, unsigned int *freq){
	char c,s[16]={0};		//temporary variables
	int i = 0;
	printf("File Read:\n");
	while((c=fgetc(fp_in))!=EOF){
                freq[(int)c]++;		//read the file character by character and increment the particular frequency
		putchar(c);
	}
	for (i = 0; i < 128; i++)
		if (freq[i]) qinsert(new_node(freq[i], i, 0, 0));//insert new nodes into the que if there is a frequency
	while (qend > 2)
		qinsert(new_node(0, 0, qremove(), qremove()));	//build the tree
	build_code(q[1], s, 0);		//build the code for the characters
}

void encode(FILE* fp_in, FILE* fp_out, unsigned int *freq ){

	char in,c,temp[20] = {0};
	int i,j=0,k=0,lim=0;
	rewind(fp_in);
	for(i=0; i<128; i++){
		if(freq[i])	lim += (freq[i]*strlen(code[i]));
	}
	output_data = lim;			//The output data is equal to the limit
	fprintf(fp_out,"%04d\n",lim);
	printf("\nEncoded:\n");
	for(i=0; i<lim; i++){
		if(temp[j] == '\0'){
			in = fgetc(fp_in);
			strcpy(temp,code[in]);
			printf("%s",code[in]);
			j = 0;
		}
		if(temp[j] == '1')
                        c = c|(1<<(7-k));       //shifts 1 to relevant position and OR with the temporary char
                else if(temp[j] == '0')
                        c = c|(0<<(7-k));       //shifts 0 to relevant position and OR with the temporary char
                else
                        printf("ERROR: Wrong input!\n");
                k++;                            // k is used to divide the string into 8 bit chunks and save
		j++;
		if(((i+1)%8 == 0) || (i==lim-1)){
                        k = 0;                  //reset k
                        fputc(c,fp_out);        //save the character
                        c = 0;                  //reset character
                }
	}
	putchar('\n');
}

void print_code(unsigned int *freq){
	int i;
	printf("\n---------CODE TABLE---------\n----------------------------\nCHAR  FREQ  CODE\n----------------------------\n");
	for(i=0; i<128; i++){
		if(isprint((char)i)&&code[i]!=NULL&&i!=' ')
			printf("%-4c  %-4d  %16s\n",i,freq[i],code[i]);
		else if(code[i]!=NULL){
			switch(i){
				case '\n':
					printf("\\n  ");
					break;
				case ' ':
					printf("\' \' ");
					break;
				case '\t':
					printf("\\t  ");
					break;
				default:
					printf("%0X  ",(char)i);
					break;
			}
			printf("  %-4d  %16s\n",freq[i],code[i]);
		}
	}
	printf("----------------------------\n");

}

int main(int argc, char* argv[]){
	FILE *fp_in, *fp_out;				//FIFO pointers
	char file_name[50]={0};				//file name
	unsigned int freq[128] = {0},i;			//frequency of the letters

	system("clear");
	printf("**********************************************************************\n");
	printf("                      COMMUNICATION  ENGINEERING\n");
	printf("                                 SHU M.Eng\n");
	printf("                             -HUFFMAN ENCODER-\n");
	printf("**********************************************************************\n\n");


	if( argc == 2 ) {
		strcpy(file_name,argv[1]);		//commandline argument directly allows to compress the file
	}
   	else if( argc > 2 ) {
      		printf("Too many arguments supplied.\n");
   	}
   	else {
		printf("Please enter the file to be compressed\t: "); //else a prompt comes to enter the file name
		scanf("%s",file_name);
   	}

	if(strlen(file_name)>=50){ 
		printf("ERROR: Enter a file name less than 50 chars");
		return 0;
	}

	//import the file into the program and update the huffman tree
	if((fp_in = fopen(file_name,"r"))==NULL){	//open the file stream
		printf("\nERROR: No such file\n");
		return 0;
	}
	import_file(fp_in,freq);			//import the file and fills frequency array


	print_code(freq);				//print the code table

	//Encode and save the encoded file
	strcat(file_name,".huffman");			//encoded file is saved in .huffman extension
	fp_out = fopen(file_name,"w");
	encode(fp_in,fp_out,freq);

	fclose(fp_in);
	fclose(fp_out);

	strcat(file_name,".table");			//write the frequency table in .table extension
	fp_out = fopen(file_name,"w");
	for(i=0; i<128; i++){
		fprintf(fp_out,"%c",(char)freq[i]);	//write the frequency to .table file
	}

	for(i=0; i<128; i++)	input_data += freq[i];	//calculate input bytes

	fclose(fp_out);					//close the output file pointer
	printf("\nInput bytes:\t\t%d\n",input_data);
	output_data = (output_data%8)? (output_data/8)+1 : (output_data/8);
	printf("Output bytes:\t\t%d\n",output_data);

	printf("\nCompression ratio:\t%.2f%%\n\n\n",((double)(input_data-output_data)/input_data)*100);
	return 0;
}
