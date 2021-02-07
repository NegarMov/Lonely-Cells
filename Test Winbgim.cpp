#include <winbgim.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
struct cell
{
	int energy, x, y;
	char name[6];
	struct cell *next= NULL;
};
struct eBlock
{
	int energy, x, y;
	struct eBlock *next= NULL;
};
struct eBlock *createBlock(int x, int y, int energy)
{
	struct eBlock *newBlock= (struct eBlock*) malloc(sizeof(struct eBlock));
	if (newBlock==NULL) return NULL;
	newBlock->x= x, newBlock->y= y, newBlock->energy=energy;
	newBlock->next= NULL;
	return newBlock;
}
void addBlock(struct eBlock *head, struct eBlock* newBlock)
{
	struct eBlock *current= head;
	while (current->next) current= current->next;
	current->next= newBlock, newBlock->next= NULL;
}
struct cell *createCell(int x, int y, char name[], int energy)
{
	struct cell *newCell= (struct cell*) malloc(sizeof(struct cell));
	if (newCell==NULL) return NULL;
	newCell->x= x, newCell->y= y, newCell->energy=energy;
	strcpy(newCell->name, name);
	newCell->next= NULL;
	return newCell;
}
void addCell(struct cell *head, struct cell* newCell)
{
	struct cell *current= head;
	while (current->next) current= current->next;
	current->next= newCell, newCell->next= NULL;
}
void removeCell(struct cell *cur, struct cell *cellHead)
{
	struct cell *index= cellHead;
	while (index->next!=cur) index= index->next;
	index->next= cur->next;
}
void removeCellF(struct cell **cellHead, int celln)
{
	if (celln>1)
		*cellHead= (*cellHead)->next;
	else
		*cellHead= NULL;
}
int setEBlocks(struct eBlock **blockHead, char stat[], int n)
{
	int flag=0, blockn=0;
	for (int i=0; i<n; i++)
		for (int j=0; j<n; j++)
			if (stat[i*n+j]=='1')
			{
				blockn++;
				if (flag) addBlock(*blockHead, createBlock(j, i, 100));
				else 
				{
					*blockHead= createBlock(j, i, 100);
					flag=1;
				}
			}
	return blockn;
}
struct eBlock *findCurBlock (struct eBlock *blockHead, struct cell *cur)
{
	while (blockHead->x != cur->x || blockHead->y != cur->y) blockHead= blockHead->next;
	return blockHead;
}
void setGame_SP(struct cell **cellHead, char stat[], unsigned int n, int celln)
{
	int x, y, nameSize, flag;
	struct cell *cur;
	for (int i=0; i<celln; i++)
	{
		do
		{
			x= rand()%n, y= rand()%n, flag=1, cur= *cellHead;
			while (cur)
			{
				if (x==cur->x && y==cur->y) flag= 0;
				cur= cur->next;
			}
		} while (stat[y*n+x]=='3' || flag==0);
		nameSize= rand()%3+2;
		char *str= (char *) malloc((nameSize+1)*sizeof(char));
		const char charset[]= "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
		for (int j=0; j<nameSize; j++)
			str[j] = charset[rand()%(sizeof(charset)/sizeof(char)-1)];
		str[nameSize]= '\0';
		if (i) addCell(*cellHead, createCell(x, y, str, 0));
		else *cellHead= createCell(x, y, str, 0);
		free(str);
	}
}
void setGame_MP(struct cell **cellHead, struct cell **cellHead2, char stat[], unsigned int n, int celln)
{
	int x, y, nameSize, flag;
	struct cell *cur;
	for (int i=0; i<celln; i++)
	{
		do
		{
			x= rand()%n, y= rand()%n, flag=1, cur= *cellHead;
			while (cur)
			{
				if (x==cur->x && y==cur->y) flag= 0;
				cur= cur->next;
			}
		} while (stat[y*n+x]=='3' || flag==0);
		nameSize= rand()%3+2;
		char *str= (char *) malloc((nameSize+1)*sizeof(char));
		const char charset[]= "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
		for (int j=0; j<nameSize; j++)
			str[j] = charset[rand()%(sizeof(charset)/sizeof(char)-1)];
		str[nameSize]= '\0';
		if (i) addCell(*cellHead, createCell(x, y, str, 0));
		else *cellHead= createCell(x, y, str, 0);
		free(str);
	}
	for (int i=0; i<celln; i++)
	{
		do
		{
			x= rand()%n, y= rand()%n, flag=1, cur= *cellHead;
			while (cur)
			{
				if (x==cur->x && y==cur->y) flag= 0;
				cur= cur->next;
			}
			cur= *cellHead2;
			while (cur)
			{
				if (x==cur->x && y==cur->y) flag= 0;
				cur= cur->next;
			}
		} while (stat[y*n+x]=='3' || flag==0);
		nameSize= rand()%3+2;
		char *str= (char *) malloc((nameSize+1)*sizeof(char));
		const char charset[]= "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
		for (int j=0; j<nameSize; j++)
			str[j] = charset[rand()%(sizeof(charset)/sizeof(char)-1)];
		str[nameSize]= '\0';
		if (i) addCell(*cellHead2, createCell(x, y, str, 0));
		else *cellHead2= createCell(x, y, str, 0);
		free(str);
	}
}
void printCells (struct cell *cellHead, int n, int color)
{
	
	while (cellHead)
	{
		int y;
		if (cellHead->x%2==0) y= (n-1)*60+57 - cellHead->y*60;
		else y= (n-1)*60+87 - cellHead->y*60;
		setfillstyle(SLASH_FILL, color);
		circle(cellHead->x*51+66, y+3, 20);
		floodfill(cellHead->x*51+66, y+3, 0);
		outtextxy(cellHead->x*51+51, y, cellHead->name);
		cellHead= cellHead->next;
	}
}
void drawMap(char stat[], unsigned int n)
{
	int x= 51, y= 30, xtmp[]= {51, 81, 102, 81, 51, 30};
	int hexCorUp[]= {51, 30, 81, 30, 102, 60, 81, 90, 51, 90, 30, 60};
	int hexCorDown[]= {51, 30+y, 81, 30+y, 102, 60+y, 81, 90+y, 51, 90+y, 30, 60+y};
	for (int i=0; i<n; i++)
	{
		for (int j=0; j<n; j++)
		{
			switch(stat[(n-i-1)*n+j])
			{
				case '1':
					setfillstyle(INTERLEAVE_FILL, 10);
					break;
				case '2':
					setfillstyle(HATCH_FILL, 3);
					break;
				case '3':
					setfillstyle(SOLID_FILL, 15);
					break;
				case '4':
					setfillstyle(EMPTY_FILL, 0);
					break;
			}
			if (j%2==0) fillpoly(6, hexCorUp);
			else fillpoly(6, hexCorDown);
			for (int c=0; c<12; c+=2) hexCorDown[c]+=x, hexCorUp[c]+=x;
		}
		for (int c=1; c<12; c+=2) hexCorUp[c]+=y*2, hexCorDown[c]+=y*2;
		for (int c=0; c<12; c+=2) hexCorUp[c]= hexCorDown[c]= xtmp[c/2];
	}
}
int moveCell(int dir, int cellDec, struct cell *cellHead, char stat[], int n)
{
	struct cell *cur= cellHead, *tmp= cellHead;
	int counter=0, x, y;
	while (counter++!=cellDec) cur= cur->next;
	if (cur->x%2==1) y= (dir==1)? 1: (dir==2 || dir==5 || dir==6)? -1: 0, x= (dir==3 || dir==5)? 1: (dir==4 || dir==6)? -1: 0;
	else y= (dir==1 || dir==3 || dir==4)? 1: (dir==2)? -1: 0, x= (dir==3 || dir==5)? 1: (dir==4 || dir==6)? -1: 0;
	if (cur->y+y>=n || cur->x+x>=n || cur->y+y<0 || cur->x+x<0 || stat[(cur->y+y)*n+cur->x+x]=='3') return 0;
	while (tmp)
	{
		if (tmp->x==cur->x+x && tmp->y==cur->y+y) return 0;
		tmp= tmp->next;
	}
	cur->y= cur->y+y, cur->x= cur->x+x;
	return 1;
}
int moveCell_MP(int dir, int cellDec, struct cell *cellHead, struct cell* cellHead2, char stat[], int n)
{
	struct cell *cur= cellHead, *tmp= cellHead;
	int counter=0, x, y;
	while (counter++!=cellDec) cur= cur->next;
	if (cur->x%2==1) y= (dir==1)? 1: (dir==2 || dir==5 || dir==6)? -1: 0, x= (dir==3 || dir==5)? 1: (dir==4 || dir==6)? -1: 0;
	else y= (dir==1 || dir==3 || dir==4)? 1: (dir==2)? -1: 0, x= (dir==3 || dir==5)? 1: (dir==4 || dir==6)? -1: 0;
	if (cur->y+y>=n || cur->x+x>=n || cur->y+y<0 || cur->x+x<0 || stat[(cur->y+y)*n+cur->x+x]=='3') return 0;
	while (tmp)
	{
		if (tmp->x==cur->x+x && tmp->y==cur->y+y) return 0;
		tmp= tmp->next;
	}
	tmp= cellHead2;
	while (tmp)
	{
		if (tmp->x==cur->x+x && tmp->y==cur->y+y) return 0;
		tmp= tmp->next;
	}
	cur->y= cur->y+y, cur->x= cur->x+x;
	return 1;
}
int isfree(int x, int y, struct cell *cellHead)
{
	while (cellHead)
	{
		if (cellHead->x==x && cellHead->y==y) return 0;
		cellHead= cellHead->next;
	}
	return 1;
}
int main()
{
	FILE *map= fopen("map.bin", "r");
	srand(time(0));
	unsigned int n; int dec, cellDec, dir, decin, score[2]= {0,0}, celln[2], blockn=0, color[]= {5, 1}; struct cell *cellHead[2]; struct eBlock *blockHead= NULL;
	cellHead[0]= NULL, cellHead[1]= NULL;
	fread(&n, 4, 1, map);
	char stat[n*n];
	fread(stat, 1, n*n, map);
	initwindow(600, 600); setbkcolor(7); setcolor(0); cleardevice();
   	drawMap(stat, n);
	printf("[1]Load\n[2]New single player game\n[3]New Multiplayer game\n[4]Exit\n");
	scanf("%d", &dec);
	switch(dec)
	{
		case 1:
		{
			///Loading game:
			FILE *save= fopen("save.bin", "r");
			int sorm, counter=0;
			fread(&sorm, sizeof(int), 1, save);
			for (int c=0; c<sorm; c++)
			{
				fread(&celln[c], sizeof(int), 1, save);
				for (int i=0; i<celln[c]; i++)
				{
					int x, y, energy, namelen; char name[6];
					fread(&x, sizeof(int), 1, save);
					fread(&y, sizeof(int), 1, save);
					fread(&energy, sizeof(int), 1, save);
					fread(&namelen, sizeof(int), 1, save);
					fread(name, sizeof(char), namelen, save);
					name[namelen]='\0';
					if (i) addCell(cellHead[c], createCell(x, y, name, energy));
					else cellHead[c]= createCell(x, y, name, energy);
					printf("counter: %d\n", counter++);
				}
			}
			fread(&blockn, sizeof(int), 1, save);
			printf("Blockn: %d\n", blockn);
			int energy[blockn];
			fread(energy, sizeof(int), blockn, save);
			int cnt=0, flag=0;
			for (int i=0; i<n; i++)
				for (int j=0; j<n; j++)
					if (stat[i*n+j]=='1')
					{
						if (flag) addBlock(blockHead, createBlock(j, i, energy[cnt++]));
						else 
						{
							blockHead= createBlock(j, i, energy[cnt++]);
							flag=1;
						}
					}
			fread(score, sizeof(int), sorm, save);
			//setEBlocks(&blockHead, stat, n, &blockn);
			if (sorm==1) goto gameLoaded_SP;
			else goto gameLoaded_MP;
		}break;
		case 2:
		{
			///Running single player game:
			system("cls");
			printf("Enter the number of cells you want to have:\n");
			scanf("%d", &celln[0]);
			blockn= setEBlocks(&blockHead, stat, n);
			setGame_SP(&cellHead[0], stat, n, celln[0]);
			gameLoaded_SP: ;
			printCells(cellHead[0], n, 5);
			while (1)
			{
				kbhit();
				struct cell *cur= cellHead[0]; int counter=1;
				system("cls");
				printf("\033[1;35mScore: *%d*\033[m\nPlease choose one of your cells:\n", score[0]);
				while (cur)
				{
					printf("[%d] %s (%d, %d)	Energy: *%d*\n", counter++, cur->name, cur->x, cur->y, cur->energy);
					cur= cur->next;
				}
				scanf("%d", &cellDec);
				cellDec--;
				system("cls");
				taskForbidden_SP: ;
				printf("[1]Move\n[2]Split the cell\n[3]Boost energy\n[4]Save\n[5]Exit\n");
				scanf("%d", &decin);
				switch(decin)
				{
					case 1:
					{
						system("cls");
						printf("[1]North\n[2]South\n[3]Northeast\n[4]Northwest\n[5]Southeast\n[6]Southwest\n");
						scanf("%d", &dir);
						system("cls");
						int canMove= moveCell(dir, cellDec, cellHead[0], stat, n);
						while (!canMove)
						{
							printf("Can't move cell to the given direction, please choose another direction:\n");
							printf("[1]North\n[2]South\n[3]Northeast\n[4]Northwest\n[5]Southeast\n[6]Southwest\n");
							scanf("%d", &dir);
							system("cls");
							canMove= moveCell(dir, cellDec, cellHead[0], stat, n);
						}
						cleardevice(); drawMap(stat, n); printCells(cellHead[0], n, 5);
					}break;
					case 2:
					{
						int counter=0;
						cur= cellHead[0];
						while (counter++!=cellDec) cur= cur->next;
						if (cur->energy<80)
						{
							system("cls");
							printf("This cell has %d Energy. You need at least 80 energy to split your cell, please choose another task:\n", cur->energy);
							goto taskForbidden_SP;	
						}
						if (stat[(cur->y)*n + cur->x] != '2')
						{
							system("cls");
							printf("You can't do the chosen function at this current block, please choose another task:\n");
							goto taskForbidden_SP;
						}
						int ijOdd[]= {0,1, 0,-1, 1,0, 1,-1, -1,0, -1,-1}, ijEven[]= {0,1, 0,-1, 1,1, 1,0, -1,1, -1,0}, freen=0;
						for (int i=0; i<12; i+=2)
						{
							int x= (cur->x)+ijEven[i], y= ((cur->x)%2==0)? (cur->y)+ijEven[i+1]: (cur->y)+ijOdd[i+1];
							if (isfree(x, y, cellHead[0]) && stat[y*n+x]!='3' && x>=0 && y>=0 && x<n && y<n) freen++;
						}
						if (freen<2)
						{
							system("cls");
							printf("Adjacent blocks are full, please choose another task:\n", freen);
							goto taskForbidden_SP;
						}
						for (int j=0; j<2; j++)
						{
							int x, y;
							do
							{
								int i= (rand()%6)*2;
								x= (cur->x)+ijEven[i], y= ((cur->x)%2==0)? (cur->y)+ijEven[i+1]: (cur->y)+ijOdd[i+1];
							} while (!isfree(x, y, cellHead[0]) || stat[y*n+x]=='3' || x<0 || y<0 || x>=n || y>=n);
							int nameSize= rand()%3+2;
							char *str= (char *) malloc((nameSize+1)*sizeof(char));
							const char charset[]= "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
							for (int j=0; j<nameSize; j++)
								str[j] = charset[rand()%(sizeof(charset)/sizeof(char)-1)];
							str[nameSize]= '\0';
							if (celln[0]>1) addCell(cellHead[0], createCell(x, y, str, 40));
							else cellHead[0]= createCell(x, y, str, 40);
							free(str);
						}
						score[0]+= 80 - cur->energy;
						if (cellDec==0) removeCellF(&cellHead[0], celln[0]);
						else removeCell(cur, cellHead[0]);
						cleardevice(); drawMap(stat, n); printCells(cellHead[0], n, 5);
						celln[0]++;
					}break;
					case 3:
					{
						int counter=0;
						cur= cellHead[0];
						while (counter++!=cellDec) cur= cur->next;
						if (stat[(cur->y)*n + cur->x] != '1')
						{
							system("cls");
							printf("You can't do the chosen function at this current block, please choose another task:\n");
							goto taskForbidden_SP;
						}
						struct eBlock *curb = findCurBlock(blockHead, cur);
						if (curb->energy>=15) curb->energy-=15, cur->energy+=15, score[0]+=15;
						else cur->energy+= curb->energy, score[0]+=curb->energy, curb->energy=0;
					}break;
					case 4:
					{
						FILE *save= fopen("save.bin", "w+");
						struct cell *tmpc= cellHead[0]; struct eBlock *tmpb= blockHead;
						int sorm=1;
						fwrite(&sorm, sizeof(int), 1, save);
						fwrite(&celln[0], sizeof(int), 1, save);
						while (tmpc)
						{
							fwrite(&(tmpc->x), sizeof(int), 1, save);
							fwrite(&(tmpc->y), sizeof(int), 1, save);
							fwrite(&(tmpc->energy), sizeof(int), 1, save);
							int len= strlen(tmpc->name);
							fwrite(&len, sizeof(int), 1, save);
							fwrite(&(tmpc->name), sizeof(char), len, save);
							tmpc= tmpc->next;
						}
						fwrite(&blockn, sizeof(int), 1, save);
						while (tmpb)
						{
							fwrite(&(tmpb->energy), sizeof(int), 1, save);
							tmpb= tmpb->next;
						}
						fwrite(&score[0], sizeof(int), 1, save);
						fclose(save);
					}break;
					case 5:
					{
						fclose(map);
						return 0;
					}break;
				}
			}
		}break;
		case 3:
		{
			///Running multiplayer game:
			system("cls");
			printf("Enter the number of cells you want to have:\n");
			scanf("%d", &celln[0]);
			celln[1]= celln[0];
			blockn= setEBlocks(&blockHead, stat, n);
			setGame_MP(&cellHead[0], &cellHead[1], stat, n, celln[0]);
			gameLoaded_MP: ;
			printCells(cellHead[0], n, 5);
			printCells(cellHead[1], n, 1);
			while (1)
			{
				for (int P=0; P<2; P++)
				{
					kbhit();
					struct cell *cur= cellHead[P]; int counter=1;
					system("cls");
					if (P==0) printf("\033[1;35mPlayer %d		Score: *%d*\033[m\nPlease choose one of your cells:\n", P+1, score[P]);
					else printf("\033[1;34mPlayer %d		Score: *%d*\033[m\nPlease choose one of your cells:\n", P+1, score[P]);
					while (cur)
					{
						printf("[%d] %s (%d, %d)	Energy: *%d*\n", counter++, cur->name, cur->x, cur->y, cur->energy);
						cur= cur->next;
					}
					scanf("%d", &cellDec);
					cellDec--;
					system("cls");
					taskForbidden_MP: ;
					printf("[1]Move\n[2]Split the cell\n[3]Boost energy\n[4]Save\n[5]Exit\n");
					scanf("%d", &decin);
					switch(decin)
					{
						case 1:
						{
							system("cls");
							printf("[1]North\n[2]South\n[3]Northeast\n[4]Northwest\n[5]Southeast\n[6]Southwest\n");
							scanf("%d", &dir);
							system("cls");
							int canMove= moveCell_MP(dir, cellDec, cellHead[P], cellHead[1-P], stat, n);
							while (!canMove)
							{
								printf("Can't move cell to the given direction, please choose another direction:\n");
								printf("[1]North\n[2]South\n[3]Northeast\n[4]Northwest\n[5]Southeast\n[6]Southwest\n");
								scanf("%d", &dir);
								system("cls");
								canMove= moveCell_MP(dir, cellDec, cellHead[P], cellHead[1-P], stat, n);
							}
							cleardevice(); drawMap(stat, n); printCells(cellHead[P], n, color[P]); printCells(cellHead[1-P], n, color[1-P]);
						}break;
						case 2:
						{
							int counter=0;
							cur= cellHead[P];
							while (counter++!=cellDec) cur= cur->next;
							if (cur->energy<80)
							{
								system("cls");
								printf("This cell has %d Energy. You need at least 80 energy to split your cell, please choose another task:\n", cur->energy);
								goto taskForbidden_MP;	
							}
							if (stat[(cur->y)*n + cur->x] != '2')
							{
								system("cls");
								printf("You can't do the chosen function at this current block, please choose another task:\n");
								goto taskForbidden_MP;
							}
							int ijOdd[]= {0,1, 0,-1, 1,0, 1,-1, -1,0, -1,-1}, ijEven[]= {0,1, 0,-1, 1,1, 1,0, -1,1, -1,0}, freen=0;
							for (int i=0; i<12; i+=2)
							{
								int x= (cur->x)+ijEven[i], y= ((cur->x)%2==0)? (cur->y)+ijEven[i+1]: (cur->y)+ijOdd[i+1];
								if (isfree(x, y, cellHead[P]) && isfree(x, y, cellHead[1-P]) && stat[y*n+x]!='3' && x>=0 && y>=0 && x<n && y<n) freen++;
							}
							if (freen<2)
							{
								system("cls");
								printf("Adjacent blocks are full, please choose another task:\n", freen);
								goto taskForbidden_MP;
							}
							for (int j=0; j<2; j++)
							{
								int x, y;
								do
								{
									int i= (rand()%6)*2;
									x= (cur->x)+ijEven[i], y= ((cur->x)%2==0)? (cur->y)+ijEven[i+1]: (cur->y)+ijOdd[i+1];
								} while (!isfree(x, y, cellHead[P]) || !isfree(x, y, cellHead[1-P]) || stat[y*n+x]=='3' || x<0 || y<0 || x>=n || y>=n);
								int nameSize= rand()%3+2;
								char *str= (char *) malloc((nameSize+1)*sizeof(char));
								const char charset[]= "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
								for (int j=0; j<nameSize; j++)
									str[j] = charset[rand()%(sizeof(charset)/sizeof(char)-1)];
								str[nameSize]= '\0';
								if (celln[P]>1) addCell(cellHead[P], createCell(x, y, str, 40));
								else cellHead[P]= createCell(x, y, str, 40);
								free(str);
							}
							score[P]+= 80 - cur->energy;
							if (cellDec==0) removeCellF(&cellHead[P], celln[P]);
							else removeCell(cur, cellHead[P]);
							cleardevice(); drawMap(stat, n); printCells(cellHead[P], n, color[P]); printCells(cellHead[1-P], n, color[1-P]);
							celln[P]++;
						}break;
						case 3:
						{
							int counter=0;
							cur= cellHead[P];
							while (counter++!=cellDec) cur= cur->next;
							if (stat[(cur->y)*n + cur->x] != '1')
							{
								system("cls");
								printf("You can't do the chosen function at this current block, please choose another task:\n");
								goto taskForbidden_MP;
							}
							struct eBlock *curb = findCurBlock(blockHead, cur);
							if (curb->energy>=15) curb->energy-=15, cur->energy+=15, score[P]+=15;
							else cur->energy+= curb->energy, score[P]+=curb->energy, curb->energy=0;
						}break;
						case 4:
						{
							FILE *save= fopen("save.bin", "w+");
							int sorm=2;
							fwrite(&sorm, sizeof(int), 1, save);
							for (int i=0; i<2; i++)
							{
								struct cell *tmpc= cellHead[i]; 
								fwrite(&celln[i], sizeof(int), 1, save);
								while (tmpc)
								{
									fwrite(&(tmpc->x), sizeof(int), 1, save);
									fwrite(&(tmpc->y), sizeof(int), 1, save);
									fwrite(&(tmpc->energy), sizeof(int), 1, save);
									int len= strlen(tmpc->name);
									fwrite(&len, sizeof(int), 1, save);
									fwrite(&(tmpc->name), sizeof(char), len, save);
									tmpc= tmpc->next;
								}
							}
							struct eBlock *tmpb= blockHead;
							fwrite(&blockn, sizeof(int), 1, save);
							while (tmpb)
							{
								fwrite(&(tmpb->energy), sizeof(int), 1, save);
								tmpb= tmpb->next;
							}
							fwrite(score, sizeof(int), 2, save);
							fclose(save);
						}break;
						case 5:
						{
							fclose(map);
							return 0;
						}break;
					}
				}
			}
		}break;
		case 4:
		{
			///Exit
			fclose(map);
			return 0;
		}break;
	}
	return 0;
}
