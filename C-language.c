#include "stdio.h"
#include "string.h"
#include "openssl/crypto.h"
#include "stdlib.h"

#define SHA256_DIGEST_LENGTH 32
struct block
{
	unsigned char prevHash[SHA256_DIGEST_LENGTH];
	int blockData;
	struct block * link;
}*head;

void addBlock(int data);
void verifyChain(void);
void alterNthBlock(int n, int newData);
void hackChain(void);
unsigned char *toString(struct block blocks);
void printAllBlock(void);

void addBlock(int data)
{
	if (head == NULL)
	{
		head = malloc(sizeof(struct block));
		SHA256("", sizeof(""), head->prevHash);
		head->blockData = data;
		return;
	}
	struct block * currentBlock = head;
	while(currentBlock->link)
		currentBlock = currentBlock->link;
	struct block * newBlock = malloc(sizeof(struct block));
	currentBlock->link = newBlock;
	newBlock->blockData = data;
	SHA256(toString(*currentBlock), sizeof(*currentBlock), newBlock->prevHash);
}

void verifyChain(void )
{
	if (head == NULL)
	 {
	 	printf("BlockChain is emty! try again after adding some blocks");
	 	return;
	 }
	 struct block *curr = head->link;
	 struct block *prev = head;
	 int count = 1;
	 while(curr)
	 {
	 	printf("%d\t[%d]\t",count++, curr->blockData);
	 	hashPrinter(SHA256(toString(*prev), sizeof(*prev), NULL),SHA256_DIGEST_LENGTH);
	 	printf(" - ");
	 	hashPrinter(curr->prevHash,SHA256_DIGEST_LENGTH);
	 	if(hashCompare(SHA256(toString(*prev), sizeof(*prev), NULL), curr->prevHash))
	 		printf(" Verified!\n");
	 	else
	 		printf("Verification Failed !\n");
	 	prev = curr;
	 	curr = curr->link;
	 }
}
void alterNthBlock(int n, int newData)
{
	struct block * curr = head;
	if(curr == NULL)
	{
		printf("Nth block does ot exist! \n");
		return;
	}
	int count = 1;
	while(count != n)
	{
		if(curr->link == NULL && count != n)
		{
			printf("Nth block does not exist!\n");
			return;
		}
		else if(count == n)
			break;
		curr = curr->link;
		count++;
	}
	printf("before: ");
	printBlock(curr);
	curr->blockData = newData;
	printf("\n After");
	printBlock(curr);
	printf("\n");
}

void hackChain(void)
{
	struct block * curr = head;
	struct block * prev;

	if(curr == NULL)
	{
		printf("BlockChain is emty!\n");
		return;
	}

	while(1)
	{
		prev = curr;
		curr = curr->link;
		if(curr == NULL)
			return;
		if(hashCompare(SHA256(toString(*prev), sizeof(*prev), NULL), curr->prevHash))
		{
			hashPrinter(
				SHA256(toString(*prev), sizeof(*prev), curr->prevHash),
				SHA256_DIGEST_LENGTH);
			printf("\n");
		}
	}
}

unsigned char *toString(struct block blocks)
{
	unsigned char *str = malloc(sizeof(unsigned char )*sizeof(blocks));
	memcpy(str, &blocks, sizeof(blocks));
	return str;
}
void hashPrinter(unsigned char hash[], int length)
{
	for(int i = 0; i < length; i++)
	{
		printf("%02x",hash[i]);
	}
}
int hashCompare(unsigned char *str1, unsigned char *str2)
{
	for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
		if(str1[i] != str2[i])
			return 0;
	return 1;
}
void printBlock(struct block *blocks)
{
	printf("%p]\n", blocks);
	hashPrinter(blocks->prevHash, sizeof(blocks->prevHash));
	printf("\t[%d]\t", blocks->blockData);
	printf("%p\n",blocks->link);
}
void printAllBlock(void)
{
	struct block * curr = head;
	int count = 0;
	while(curr)
	{
		printBlock(curr);
		curr = curr->link;
	}
}
void main(void)
{
	int c, n, r;
	printf("1. addBlock \n");
	printf("2. add n random block \n");
	printf("3. alterNthBlock \n");
	printf("4. printAllBlock\n");
	printf("5. verifyChain\n");
	printf("6. hackChain\n");
	while(1)
	{
		printf("Choice: ");
		scanf("%d",&c);
		switch(c)
		{
			case 1:
				printf("Enter data: \n");
				scanf("%d", &n);
				addBlock(n);
				break;
			case 2:
				printf("How many blocks to enter?: ");
				scanf("%d", &n);
				for(int i = 0; i < n; i++)
				{
					r= rand()%(n*10);
					printf("entering: %d\n",r);
					addBlock(r);
				}
				break;
			case 3:
				printf("wwhich block to alter?: \n");
				scanf("%d", &n);
				printf("enter value: ");
				scanf("%d", &r);
				alterNthBlock(n,r);
				break;
			case 4:
				printAllBlock();
				break;
			case 5:
				verifyChain();
				break;
			case 6:
				hackChain();
				break;
			default : break;
		}

	}
}
// gcc C-language.c -o blockchain.o -lcrypto
// ./blockchain.o