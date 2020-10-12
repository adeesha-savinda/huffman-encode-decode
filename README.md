# Compression of Text using Huffman Encoding
This method of encoding was introduced by David A. Huffman  In 1951. while he was a Ph.D. student at MIT. He was given the task to find an efficient way of encoding and came up with the idea of using a **frequency-sorted binary tree** and proved this method to be the most efficient. He published a paper in 1952 titled "[A Method for the Construction of Minimum Redundancy Codes](http://compression.ru/download/articles/huff/huffman_1952_minimum-redundancy-codes.pdf)".

## Data Compression
In communication; data compression is source coding where reduction of bits used is done. Overall aim is to use less number of bits to encode the data than the original number of bits. This encoding is usually done at the source of the data before it is transmitted.
Consider five symbols A , B , C , D and E to be transmitted which has a probability of occurring 0.5 , 0.2 , 0.1 , 0.1 and 0.1 respectively. Since there are 5 symbols, we have to use at least three bits to represent one symbol. But if we use the Huffman encoding method:   

- A – 000 can be represented as A – 0                                               
- B – 001 can be represented as B - 10 
- C - 010 can be represented as C - 010 
- D - 011 can be represented as D - 011
- E - 100 can be represented as E - 100 

Thus reducing the overall number of bits require to represent the text. Most Frequently occurring symbols with low number of bits and least frequently occurring symbols with higher number of bits.

### Lossless or Lossy Compression
The result of data compression can be either lossy or lossless. If the compression is lossy there will be a reduction in the bits where some of the unnecessary data will be removed and only an approximation of the original data will be recovered after decoding. Such as video and image compression algorythms.
In the other hand no information will be lost in the lossless data compression. The process of reducing the size of a data file is referred to as data compression. Lossless data compression is used in many applications. For example, it is used in the ZIP file.

## Huffman Tree and Encoding using Huffman
Huffman tree is a technique that is used to generate codes that are distinct to each other. Using this method, most occurring symbols will get the least number of bits and others accordingly.
1. Sort all the different symbols and their particular frequency or probability. 
2. Take the two lowest probability symbol and create a new common node with the probability equal to the sum of the two probability. 
3. Add the new node to the table instead of the lowest two symbols or nodes. 
4. Repeat step two and three until one symbol or node is remaining. 
5. Then allocate ‘0’ too all the left branches of all the nodes and ‘1’ to all right branches
6. Read the bits from top of the tree to the bottom to each symbol and record their particular bit pattern 

### A Practical Example
Consider the following centence

**this lesson is communication engineering**

|letter|no of occurences|letter|no of occurences|
|------|----------------|------|----------------|
