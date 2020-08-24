#include<stdio.h>
#include<string.h>
#include<windows.h>
#include<stdlib.h>
#define nmemb 7
#define His_len 10
#define GET_FLAG(__data__) ((__data__&0x80)>>7)
#define M_CHG_INT16(__data__) ((__data__>>8)|(__data__<<8))
#define M_CHG_INT32(__data__) ((__data__>>24)|((__data__&0xff0000)>>8)|((__data__&0xff00)<<8)|(__data__<<24))
typedef unsigned char  INT8U;
typedef signed char    INT8S;
typedef unsigned short INT16U;
typedef signed short   INT16S;
typedef unsigned long  INT32U;
typedef signed long    INT32S;

/*道路结构体*/
typedef struct Road
{
	unsigned short int size;
	unsigned int linkid;
	unsigned short int namesize;
	INT32U info;
	int flag;
	int brunch;
	int dispclass;
	char name[20];
}Data;

/*链表节点*/
struct Node
{
	Data data;
	struct Node* next;
};

/*创建链表，读取文件*/
struct Node* create()
{
	int number = 0;//存放结点个数
	struct Node* pHead = NULL;
	struct Node* pTail = NULL;
	unsigned short size;//开关
	FILE* fin;
	fin = fopen("E:\\GTBL.dat", "rb");
	char address[20] = "E:\\GTBL.dat";
	if (fin == NULL)
	{
		printf("\n");
		printf("\n");
		printf("                                      文件读取失败,功能不可用\n");
		printf("                                              输入地址:");
		scanf("%s", address);
		fin = fopen(address, "rb");
		if (fin != NULL)
			printf("                                       文件读取成功\n");
	}
	else
	{
		printf("\n");
		printf("\n");
		printf("                                           文件读取成功\n");
	}
	do
	{
		if (fread(&size, 2, 1, fin) == 1)
		{
			number++;
			struct Node* pNew = (struct Node*)malloc(sizeof(struct Node));
			pNew->data.size = M_CHG_INT16(size);
			fread(&pNew->data.linkid, sizeof(INT32U), 1, fin);
			pNew->data.linkid = M_CHG_INT32(pNew->data.linkid);
			fread(&(pNew->data.namesize), sizeof(INT16U), 1, fin);
			pNew->data.namesize = M_CHG_INT16(pNew->data.namesize);
			fread(&(pNew->data.info), sizeof(INT32U), 1, fin);
			pNew->data.info = M_CHG_INT32(pNew->data.info);
			pNew->data.flag = pNew->data.info >> 7 & 0x01;
			pNew->data.brunch = pNew->data.info >> 4 & 0x07;//0000 0111
			pNew->data.dispclass = pNew->data.info & 0x0f;//0000 1111
			if (1 == GET_FLAG(pNew->data.info))
			{
				fread(pNew->data.name, pNew->data.namesize + 2, 1, fin);
			}
			else {
				strcpy(pNew->data.name, "no name");
			}
			if (pHead == NULL)
			{
				pHead = pTail = pNew;
			}
			else
			{
				pTail->next = pNew;
				pTail = pNew;
				pTail->next = NULL;
			}
		}
		else
		{
			break;
		}
	} while (1);
	printf("                                       已将文件存放到%s位置\n", address);
	return pHead;
}
/*打印链表*/
void show(struct Node* phead)
{
	struct Node* p;
	p = phead;
	while (p)
	{
		printf("linkID:%d  ", p->data.linkid);
		printf("dispclass:%d   ", p->data.dispclass);
		printf("岔路口数：%d   ", p->data.brunch);
		printf("description: %s\n", p->data.name);
		p = p->next;
	}
}

/*将读取的txt文件保存*/
void gettext(struct Node* phead)
{
	struct Node* p;
	p = phead;
	FILE* fpWrite = fopen("E:\\gettext.txt", "w");
	while (p)
	{
		fprintf(fpWrite, "linkid:%d   ", p->data.linkid);
		fprintf(fpWrite, "dispclass:%d   ", p->data.dispclass);
		fprintf(fpWrite, "岔路口数：%d   ", p->data.brunch);
		fprintf(fpWrite, "description: %s\n", p->data.name);
		p = p->next;
	}
}

/*删除链表*/
void delete_list(struct Node* phead)
{
	struct Node* p, * q;
	p = phead;
	q = p->next;
	while (q != NULL)
	{
		p->next = q->next;
		free(q);
		q = p->next;
	}
	free(phead);
}

/*排序*/
void sort(struct Node* phead)
{
	struct Node* p;
	int length = 0;
	p = phead;
	while (p)
	{
		length++;
		p = p->next;
	}
	struct Node* array;
	array = (struct Node*)malloc(length * sizeof(struct Node));
	p = phead;
	int i = 0;
	while (p)
	{
		(array + i)->data.brunch = p->data.brunch;
		(array + i)->data.dispclass = p->data.dispclass;
		(array + i)->data.flag = p->data.flag;
		(array + i)->data.info = p->data.info;
		(array + i)->data.linkid = p->data.linkid;
		strcpy((array + i)->data.name, p->data.name);
		(array + i)->data.namesize = p->data.namesize;
		(array + i)->data.size = p->data.size;
		i++;
		p = p->next;
	}
	printf("                选择您所想要的排序方式：1.冒泡排序  2.插入排序  3.选择排序  4.返回主界面\n");
	int choice1;
	scanf("%d", &choice1);

	/*冒泡*/
	if (choice1 == 1)
	{
		printf("                       选择您所想要的排序根据：1.linkid  2.dispclass  3..brunch\n");
		int choice2;
		scanf("%d", &choice2);
		printf("                            请选择正序还是逆序排列：1.正序     2.逆序\n");
		int choice3;
		scanf("%d", &choice3);
		struct Node temp;
		int j = 0;
		if (choice2 == 1)
		{
			for (j = 0; j < length-1; j++)
			{
				for (i = 0; i < length - j-1; i++) {

					if ((array + i)->data.linkid > (array + 1 + i)->data.linkid)
					{
						temp = *(array + i);
						*(array + i) = *(array + i + 1);
						*(array + i + 1) = temp;
					}
				}
			}
		}
		if (choice2 == 2)
		{
			for (j = 0; j < length-1; j++)
			{
				for (i = 0; i < length - j-1; i++) {

					if ((array + i)->data.dispclass > (array + 1 + i)->data.dispclass)
					{
						temp = *(array + i);
						*(array + i) = *(array + i + 1);
						*(array + i + 1) = temp;
					}
				}
			}
		}
		if (choice2 == 3)
		{
			for (j = 0; j < length-1; j++)
			{
				for (i = 0; i < length - j-1; i++) {

					if ((array + i)->data.brunch > (array + 1 + i)->data.brunch)
					{
						temp = *(array + i);
						*(array + i) = *(array + i + 1);
						*(array + i + 1) = temp;
					}
				}
			}
		}
		FILE* fpWrite = fopen("sort1.txt", "w");
		if (fpWrite == NULL)
		{
			return 0;
		}
		else
		{
			int cnt = 0;
			printf("排序之后的前五条数据为：\n");
			if (choice3 == 1)
			{
				for (i = 0; i < length; i++)
				{
					fprintf(fpWrite, "linkid:%d   ", (array + i)->data.linkid);
					fprintf(fpWrite, "dispclass:%d   ", (array + i)->data.dispclass);
					fprintf(fpWrite, "brunch：%d   ", (array + i)->data.brunch);
					fprintf(fpWrite, "description: %s\n", (array + i)->data.name);
					if (cnt <= 4)
					{
						printf("linkID:%d  ", (array + i)->data.linkid);
						printf("dispclass:%d   ", (array + i)->data.dispclass);
						printf("岔路口数：%d   ", (array + i)->data.brunch);
						printf("description: %s\n", (array + i)->data.name);

					}
					cnt++;
				}
			}
			else
			{
				for (i = length - 1; i >= 0; i--)
				{
					fprintf(fpWrite, "linkid:%d   ", (array + i)->data.linkid);
					fprintf(fpWrite, "dispclass:%d   ", (array + i)->data.dispclass);
					fprintf(fpWrite, "brunch：%d   ", (array + i)->data.brunch);
					fprintf(fpWrite, "description: %s\n", (array + i)->data.name);
					if (cnt <= 4)
					{
						printf("linkID:%d  ", (array + i)->data.linkid);
						printf("dispclass:%d   ", (array + i)->data.dispclass);
						printf("岔路口数：%d   ", (array + i)->data.brunch);
						printf("description: %s\n", (array + i)->data.name);

					}
					cnt++;
				}
			}
		}

	}

	/*插入排序*/
	if (choice1 == 2)
	{
		printf("                       选择您所想要的排序根据：1.linkid  2.dispclass  3..brunch\n");
		int choice2;
		scanf("%d", &choice2);
		printf("                            请选择正序还是逆序排列：1.正序     2.逆序\n");
		int choice3;
		scanf("%d", &choice3);
		int i, j, p;
		struct Node t;
		if (choice2 == 1)
		{
			for (i = 1; i < length; i++)
			{
				t = *(array + i);
				p = i - 1;
				while ((p >= 0) && (t.data.linkid < (array + p)->data.linkid))
				{
					*(array + p + 1) = *(array + p);
					p--;
				}
				*(array + p + 1) = t;
			}
		}
		if (choice2 == 2)
		{
			for (i = 1; i < length; i++)
			{
				t = *(array + i);
				p = i - 1;
				while ((p >= 0) && (t.data.dispclass < (array + p)->data.dispclass))
				{
					*(array + p + 1) = *(array + p);
					p--;
				}
				*(array + p + 1) = t;
			}
		}
		if (choice2 == 3)
		{
			for (i = 1; i < length; i++)
			{
				t = *(array + i);
				p = i - 1;
				while ((p >= 0) && (t.data.brunch < (array + p)->data.brunch))
				{
					*(array + p + 1) = *(array + p);
					p--;
				}
				*(array + p + 1) = t;
			}
		}
		FILE* fpWrite = fopen("sort2.txt", "w");
		if (fpWrite == NULL)
		{
			return 0;
		}
		else
		{
			int cnt = 0;
			printf("   排序之后的前五条数据为：\n");
			if (choice3 == 1)
			{
				for (i = 0; i < length; i++)
				{
					fprintf(fpWrite, "linkid:%d   ", (array + i)->data.linkid);
					fprintf(fpWrite, "dispclass:%d   ", (array + i)->data.dispclass);
					fprintf(fpWrite, "brunch：%d   ", (array + i)->data.brunch);
					fprintf(fpWrite, "description: %s\n", (array + i)->data.name);
					if (cnt <= 4)
					{
						printf("linkID:%d  ", (array + i)->data.linkid);
						printf("dispclass:%d   ", (array + i)->data.dispclass);
						printf("岔路口数：%d   ", (array + i)->data.brunch);
						printf("description: %s\n", (array + i)->data.name);
					}
					cnt++;
				}
			}
			else
			{
				for (i = length - 1; i >= 0; i--)
				{
					fprintf(fpWrite, "linkid:%d   ", (array + i)->data.linkid);
					fprintf(fpWrite, "dispclass:%d   ", (array + i)->data.dispclass);
					fprintf(fpWrite, "brunch：%d   ", (array + i)->data.brunch);
					fprintf(fpWrite, "description: %s\n", (array + i)->data.name);
					if (cnt <= 4)
					{
						printf("linkID:%d  ", (array + i)->data.linkid);
						printf("dispclass:%d   ", (array + i)->data.dispclass);
						printf("岔路口数：%d   ", (array + i)->data.brunch);
						printf("description: %s\n", (array + i)->data.name);

					}
					cnt++;
				}
			}
		}
	}

	/*选择排序*/
	if (choice1 == 3)
	{
		printf("                       选择您所想要的排序根据：1.linkid  2.dispclass  3..brunch\n");
		int choice2;
		scanf("%d", &choice2);
		printf("                            请选择正序还是逆序排列：1.正序     2.逆序\n");
		int choice3;
		scanf("%d", &choice3);
		int i, j;
		struct Node t;
		if (choice2 == 1)
		{
			for (i = 0; i < length - 1; i++)
			{
				for (j = i + 1; j < length; j++)
				{
					if ((array + i)->data.linkid > (array + j)->data.linkid)
					{
						t = *(array + i);
						*(array + i) = *(array + j);
						*(array + j) = t;
					}
				}
			}
		}
		if (choice2 == 2)
		{
			for (i = 0; i < length - 1; i++)
			{
				for (j = i + 1; j < length; j++)
				{
					if ((array + i)->data.dispclass > (array + j)->data.dispclass)
					{
						t = *(array + i);
						*(array + i) = *(array + j);
						*(array + j) = t;
					}
				}
			}
		}
		if (choice2 == 3)
		{
			for (i = 0; i < length - 1; i++)
			{
				for (j = i + 1; j < length; j++)
				{
					if ((array + i)->data.brunch > (array + j)->data.brunch)
					{
						t = *(array + i);
						*(array + i) = *(array + j);
						*(array + j) = t;
					}
				}
			}
		}
		FILE* fpWrite = fopen("sort3.txt", "w");
		if (fpWrite == NULL)
		{
			return 0;
		}
		else
		{
			int cnt = 0;
			printf("   排序之后的前五条数据为：\n");
			if (choice3 == 1)
			{
				for (i = 0; i < length; i++)
				{
					fprintf(fpWrite, "linkid:%d   ", (array + i)->data.linkid);
					fprintf(fpWrite, "dispclass:%d   ", (array + i)->data.dispclass);
					fprintf(fpWrite, "brunch：%d   ", (array + i)->data.brunch);
					fprintf(fpWrite, "description: %s\n", (array + i)->data.name);
					if (cnt <= 4)
					{
						printf("linkID:%d  ", (array + i)->data.linkid);
						printf("dispclass:%d   ", (array + i)->data.dispclass);
						printf("岔路口数：%d   ", (array + i)->data.brunch);
						printf("description: %s\n", (array + i)->data.name);
					}
					cnt++;
				}
			}
			else
			{
				for (i = length - 1; i >= 0; i--)
				{
					fprintf(fpWrite, "linkid:%d   ", (array + i)->data.linkid);
					fprintf(fpWrite, "dispclass:%d   ", (array + i)->data.dispclass);
					fprintf(fpWrite, "brunch：%d   ", (array + i)->data.brunch);
					fprintf(fpWrite, "description: %s\n", (array + i)->data.name);
					if (cnt <= 4)
					{
						printf("linkID:%d  ", (array + i)->data.linkid);
						printf("dispclass:%d   ", (array + i)->data.dispclass);
						printf("岔路口数：%d   ", (array + i)->data.brunch);
						printf("description: %s\n", (array + i)->data.name);
					}
					cnt++;
				}
			}
		}
	}
	if (choice1 == 5) {}
	free(array);
}

/*添加数据*/
void Add_Data()
{
	unsigned short int sizeTMP;
	unsigned int linkidTMP;
	unsigned short int namesizeTMP;
	INT32U infoTMP = 0;
	int flagTMP;
	int brunchTMP;
	int dispclassTMP;
	char nameTMP[200];
	//INT16U TTS = 0;

	printf("请输入您要添加的数据信息：\n");


	printf(" 道路linkid:");
	scanf("%d", &linkidTMP);

	printf(" 道路flag:");
	scanf("%d", &flagTMP);

	printf(" 道路brunch:");
	scanf("%d", &brunchTMP);

	printf(" 道路dispclass:");
	scanf("%d", &dispclassTMP);

	if (flagTMP == 1)
	{
		printf(" 道路name:");
		getchar();//吸收回车
		gets(nameTMP);
	}

	printf("信息添加中... ...");

	struct Node* Tmp = (struct Node*)malloc(sizeof(struct Node));
	Tmp->data.linkid = linkidTMP;
	Tmp->data.flag = flagTMP;
	Tmp->data.brunch = brunchTMP;
	Tmp->data.dispclass = dispclassTMP;

	if (flagTMP == 1)
	{
		strcpy(Tmp->data.name, nameTMP);
		Tmp->data.namesize = strlen(Tmp->data.name);
	}
	else
	{
		Tmp->data.namesize = 0;
	}

	Tmp->next = NULL;
	sizeTMP = 14 + Tmp->data.namesize;
	Tmp->data.size = sizeTMP;

	FILE* FA = fopen("E:\\GTBL.dat", "ab+");

	sizeTMP = M_CHG_INT16(Tmp->data.size);
	fwrite(&sizeTMP, sizeof(INT16U), 1, FA);

	linkidTMP = M_CHG_INT32(Tmp->data.linkid);
	fwrite(&linkidTMP, sizeof(INT32U), 1, FA);

	namesizeTMP = M_CHG_INT16(Tmp->data.namesize);
	fwrite(&namesizeTMP, sizeof(INT16U), 1, FA);

	infoTMP = infoTMP | (Tmp->data.flag << 7) | (Tmp->data.brunch << 4) | Tmp->data.dispclass;
	infoTMP = M_CHG_INT32(infoTMP);
	fwrite(&infoTMP, sizeof(INT32U), 1, FA);

	if (Tmp->data.flag == 1)
	{
		strcpy(nameTMP, Tmp->data.name);
		fwrite(nameTMP, Tmp->data.namesize + 2, 1, FA);
	}

	/*TTS = M_CHG_INT16(TTS);
	fwrite(&TTS, sizeof(INT16U), 1, FA);*/

	printf("添加完成");
	fclose(FA);
}
/*删除数据*/
void Del_Data(struct Node* phead)
{
	int count = 0;
	unsigned int fLinkID = 0;
	unsigned short int sizeTMP;
	unsigned int linkidTMP;
	unsigned short int namesizeTMP;
	INT32U infoTMP = 0;
	int flagTMP;
	int brunchTMP;
	int dispclassTMP;
	char nameTMP[200];
	printf("请输入需要删除的路的LinkID:");
	scanf("%d", &fLinkID);
	struct Node* pNext = NULL;
	struct Node* pFind = NULL;
	struct Node* pRW = NULL;
	pRW = phead;
	pFind = phead;
	pNext = pFind->next;
	if (phead->data.linkid == fLinkID)
	{
		struct Node* Tmp = (struct Node*)malloc(sizeof(struct Node));

		if (phead->data.flag == 1 && phead->next->data.flag == 1)
		{
			strcpy(Tmp->data.name, phead->data.name);
			strcpy(phead->data.name, phead->next->data.name);
			strcpy(phead->next->data.name, Tmp->data.name);
		}
		else if (phead->data.flag == 1)
		{
			strcpy(phead->next->data.name, phead->data.name);
		}
		else if (phead->next->data.flag == 1)
		{
			strcpy(phead->data.name, phead->next->data.name);
		}

		Tmp->data.size = phead->data.size;
		phead->data.size = phead->next->data.size;
		phead->next->data.size = Tmp->data.size;

		Tmp->data.linkid = phead->data.linkid;
		phead->data.linkid = phead->next->data.linkid;
		phead->next->data.linkid = Tmp->data.linkid;

		Tmp->data.namesize = phead->data.namesize;
		phead->data.namesize = phead->next->data.namesize;
		phead->next->data.namesize = Tmp->data.namesize;

		Tmp->data.info = phead->data.info;
		phead->data.info = phead->next->data.info;
		phead->next->data.info = Tmp->data.info;

		Tmp->data.flag = phead->data.flag;
		phead->data.flag = phead->next->data.flag;
		phead->next->data.flag = Tmp->data.flag;

		Tmp->data.brunch = phead->data.brunch;
		phead->data.brunch = phead->next->data.brunch;
		phead->next->data.brunch = Tmp->data.brunch;

		Tmp->data.dispclass = phead->data.dispclass;
		phead->data.dispclass = phead->next->data.dispclass;
		phead->next->data.dispclass = Tmp->data.dispclass;
		free(Tmp);
	}
	while (pNext != NULL)
	{
		if (pNext->data.linkid == fLinkID)
		{
			pFind->next = pNext->next;
			free(pNext);
			pNext = pFind->next;
			count++;
		}
		else
		{
			pNext = pNext->next;
			pFind = pFind->next;
		}
	}
	if (count == 0)
	{
		printf("未能找到要删除的内容！\n");
		return;
	}
	else
	{
		printf("正在修改... ...");
		FILE* fp;
		fp = fopen("E:\\GTBL.dat", "wb+");

		while (pRW != NULL)
		{
			sizeTMP = M_CHG_INT16(pRW->data.size);
			fwrite(&sizeTMP, sizeof(sizeTMP), 1, fp);

			linkidTMP = M_CHG_INT32(pRW->data.linkid);
			fwrite(&linkidTMP, sizeof(linkidTMP), 1, fp);

			namesizeTMP = M_CHG_INT16(pRW->data.namesize);
			fwrite(&namesizeTMP, sizeof(namesizeTMP), 1, fp);

			infoTMP = M_CHG_INT32(pRW->data.info);
			fwrite(&infoTMP, sizeof(infoTMP), 1, fp);

			if (pRW->data.flag == 1)
			{
				strcpy(nameTMP, pRW->data.name);
				fwrite(nameTMP, pRW->data.namesize + 2, 1, fp);
			}
			pRW = pRW->next;
		}
		printf("修改完成");
		fclose(fp);
	}
}
/*修改数据*/
void Change_Data(struct Node* phead)
{
	int count = 0;
	unsigned int fLinkID = 0;
	int fdispclass = 0;
	int fBrunch = 0;
	char fdescription[200];
	unsigned short int sizeTMP;
	unsigned int linkidTMP;
	unsigned short int namesizeTMP;
	INT32U infoTMP = 0;
	int flagTMP;
	int brunchTMP;
	int dispclassTMP;
	char nameTMP[200];
	printf("请输入需要更改的原路的LinkID:");
	scanf("%d", &fLinkID);
	struct Node* pNext = NULL;
	struct Node* pFind = NULL;
	struct Node* pRW = NULL;
	pRW = phead;
	pFind = phead;
	pNext = pFind->next;
	if (phead->data.linkid == fLinkID)
	{
		struct Node* Tmp = (struct Node*)malloc(sizeof(struct Node));
		if (phead->data.flag == 1 && phead->next->data.flag == 1)
		{
			strcpy(Tmp->data.name, phead->data.name);
			strcpy(phead->data.name, phead->next->data.name);
			strcpy(phead->next->data.name, Tmp->data.name);
		}
		else if (phead->data.flag == 1)
		{
			strcpy(phead->next->data.name, phead->data.name);
		}
		else if (phead->next->data.flag == 1);
		{
			strcpy(phead->data.name, phead->next->data.name);
		}
		Tmp->data.size = phead->data.size;
		phead->data.size = phead->next->data.size;
		phead->next->data.size = Tmp->data.size;
		Tmp->data.linkid = phead->data.linkid;
		phead->data.linkid = phead->next->data.linkid;
		phead->next->data.linkid = Tmp->data.linkid;
		Tmp->data.namesize = phead->data.namesize;
		phead->data.namesize = phead->next->data.namesize;
		phead->next->data.namesize = Tmp->data.namesize;
		Tmp->data.info = phead->data.info;
		phead->data.info = phead->next->data.info;
		phead->next->data.info = Tmp->data.info;
		Tmp->data.flag = phead->data.flag;
		phead->data.flag = phead->next->data.flag;
		phead->next->data.flag = Tmp->data.flag;
		Tmp->data.brunch = phead->data.brunch;
		phead->data.brunch = phead->next->data.brunch;
		phead->next->data.brunch = Tmp->data.brunch;
		Tmp->data.dispclass = phead->data.dispclass;
		phead->data.dispclass = phead->next->data.dispclass;
		phead->next->data.dispclass = Tmp->data.dispclass;
		free(Tmp);
	}
	while (pNext != NULL)
	{
		if (pNext->data.linkid == fLinkID)
		{
			pFind->next = pNext->next;
			free(pNext);
			pNext = pFind->next;
			count++;
		}
		else
		{
			pNext = pNext->next;
			pFind = pFind->next;
		}
	}
	if (count == 0)
	{
		printf("未能找到要更改的内容！\n");
		return;
	}
	else
	{
		FILE* fp;
		fp = fopen("E:\\GTBL.dat", "wb+");

		while (pRW != NULL)
		{
			sizeTMP = M_CHG_INT16(pRW->data.size);
			fwrite(&sizeTMP, sizeof(sizeTMP), 1, fp);

			linkidTMP = M_CHG_INT32(pRW->data.linkid);
			fwrite(&linkidTMP, sizeof(linkidTMP), 1, fp);

			namesizeTMP = M_CHG_INT16(pRW->data.namesize);
			fwrite(&namesizeTMP, sizeof(namesizeTMP), 1, fp);

			infoTMP = M_CHG_INT32(pRW->data.info);
			fwrite(&infoTMP, sizeof(infoTMP), 1, fp);

			if (pRW->data.flag == 1)
			{
				strcpy(nameTMP, pRW->data.name);
				fwrite(nameTMP, pRW->data.namesize + 2, 1, fp);
			}
			pRW = pRW->next;
		}
		fclose(fp);
	}
	printf("请输入您要更新的数据信息：\n");

	printf(" 道路linkid:");
	scanf("%d", &linkidTMP);

	printf(" 道路flag:");
	scanf("%d", &flagTMP);

	printf(" 道路brunch:");
	scanf("%d", &brunchTMP);

	printf(" 道路dispclass:");
	scanf("%d", &dispclassTMP);

	if (flagTMP == 1)
	{
		printf(" 道路name:");
		getchar();//吸收回车
		gets(nameTMP);
	}

	printf("信息更新中... ...");

	struct Node* Tmp = (struct Node*)malloc(sizeof(struct Node));
	Tmp->data.linkid = linkidTMP;
	Tmp->data.flag = flagTMP;
	Tmp->data.brunch = brunchTMP;
	Tmp->data.dispclass = dispclassTMP;

	if (flagTMP == 1)
	{
		strcpy(Tmp->data.name, nameTMP);
		Tmp->data.namesize = strlen(Tmp->data.name);
	}
	else
	{
		Tmp->data.namesize = 0;
	}

	Tmp->next = NULL;
	sizeTMP = 14 + Tmp->data.namesize;
	Tmp->data.size = sizeTMP;

	FILE* FA = fopen("E:\\GTBL.dat", "ab+");

	sizeTMP = M_CHG_INT16(Tmp->data.size);
	fwrite(&sizeTMP, sizeof(INT16U), 1, FA);

	linkidTMP = M_CHG_INT32(Tmp->data.linkid);
	fwrite(&linkidTMP, sizeof(INT32U), 1, FA);

	namesizeTMP = M_CHG_INT16(Tmp->data.namesize);
	fwrite(&namesizeTMP, sizeof(INT16U), 1, FA);

	infoTMP = infoTMP | (Tmp->data.flag << 7) | (Tmp->data.brunch << 4) | Tmp->data.dispclass;
	infoTMP = M_CHG_INT32(infoTMP);
	fwrite(&infoTMP, sizeof(INT32U), 1, FA);

	if (Tmp->data.flag == 1)
	{
		strcpy(nameTMP, Tmp->data.name);
		fwrite(nameTMP, Tmp->data.namesize + 2, 1, FA);
	}

	/*TTS = M_CHG_INT16(TTS);
	fwrite(&TTS, sizeof(INT16U), 1, FA);*/

	printf("更新完成");
	fclose(FA);
}
/*id检索*/
void Search_ID(struct Node* phead)
{
	int count = 0;
	unsigned int fLinkID = 0;
	FILE* pFile;
	pFile = fopen("History_LinkID.txt", "w+");//打开文件
	printf("请输入需要查找的路的LinkID:");
	scanf("%d", &fLinkID);
	FILE* phistory;
	phistory = fopen("Record_His.txt", "a+");
	fprintf(phistory, "搜索了linkid为%d的路\n", fLinkID);
	struct Node* pFind = NULL;
	pFind = phead;
	while (pFind != NULL)
	{
		if (pFind->data.linkid == fLinkID)
		{
			if (count == 0)
			{
				printf("LinkID为%d的路:\n", fLinkID);
				fprintf(pFile, "LinkID为%d的路:\n", fLinkID);
			}
			count++;
			if (count < 6)
			{
				printf("dispclass:%d   ", pFind->data.dispclass);
				printf("岔路口数：%d   ", pFind->data.brunch);
				printf("description: %s\n", pFind->data.name);
			}
			fprintf(pFile, "dispclass:%d   岔路口数：%d   description: %s\n", pFind->data.dispclass, pFind->data.brunch, pFind->data.name);
		}
		pFind = pFind->next;
	}
	if (count >= 6)
	{
		printf("仅显示前五条数据，其余数据存放到History_LinkID.txt中\n");
	}
	/*----------------相似结果--------------------*/
	if (count == 0)
	{
		printf("没有你所要查找的LinkID的路!下面显示相似结果：\n");
		fLinkID = fLinkID - 5;
		for (int i = 0; i < 10; i++)
		{
			count = 0;
			pFind = phead;
			while (pFind != NULL)
			{
				if (pFind->data.linkid == fLinkID)
				{
					if (count == 0)
					{
						printf("LinkID为%d的路:\n", fLinkID);
						fprintf(pFile, "LinkID为%d的路:\n", fLinkID);
					}
					count++;
					if (count < 6)
					{
						printf("dispclass:%d   ", pFind->data.dispclass);
						printf("岔路口数：%d   ", pFind->data.brunch);
						printf("description: %s\n", pFind->data.name);
					}
				}
				pFind = pFind->next;
			}
			if (count >= 6)
			{
				printf("仅显示前五条数据，其余数据存放到History_LinkID.txt中\n");
			}
			fLinkID++;
		}
	}
	fclose(pFile);
	fclose(phistory);
}
/*dispclass检索*/
void Search_dispclass(struct Node* phead)
{
	int count = 0;
	int fdispclass = 0;
	FILE* pFile;
	pFile = fopen("History_dispclass.txt", "w+");//打开文件
	printf("请输入需要查找的路的dispclass:");
	scanf("%d", &fdispclass);
	FILE* phistory;
	phistory = fopen("Record_His.txt", "a+");
	fprintf(phistory, "搜索了dispclass为%d的路\n", fdispclass);
	struct Node* pFind = NULL;
	pFind = phead;
	while (pFind != NULL)
	{
		if (pFind->data.dispclass == fdispclass)
		{
			if (count == 0)
			{
				printf("dispclass为%d的路:\n", fdispclass);
			}
			count++;
			if (count < 6)
			{
				printf("LinkID:%d   ", pFind->data.linkid);
				printf("岔路口数：%d   ", pFind->data.brunch);
				printf("description: %s\n", pFind->data.name);
			}
			fprintf(pFile, "LinkID:%d   岔路口数：%d   description: %s\n", pFind->data.linkid, pFind->data.brunch, pFind->data.name);
		}
		pFind = pFind->next;
	}
	if (count >= 6)
	{
		printf("仅显示前五条数据，其余数据存放到History_dispclass.txt中\n");
	}
	/*----------------相似结果--------------------*/
	if (count == 0)
	{
		printf("没有你所要查找的dispclass的路!下面显示相似结果：\n");
		fdispclass = fdispclass - 5;
		for (int i = 0; i < 10; i++)
		{
			count = 0;
			pFind = phead;
			while (pFind != NULL)
			{
				if (pFind->data.dispclass == fdispclass)
				{
					if (count == 0)
					{
						printf("dispclass为%d的路:\n", fdispclass);
					}
					count++;
					if (count < 6)
					{
						printf("LinkID:%d   ", pFind->data.linkid);
						printf("岔路口数：%d   ", pFind->data.brunch);
						printf("description: %s\n", pFind->data.name);
					}
				}
				pFind = pFind->next;
			}
			fdispclass++;
		}
	}
	fclose(pFile);
	fclose(phistory);
}
/*岔路检索*/
void Search_Brunch(struct Node* phead)
{
	int count = 0;
	int fBrunch = 0;
	FILE* pFile;
	pFile = fopen("History_Brunch.txt", "w+");//打开文件
	printf("请输入需要查找的路的岔路口数:");
	scanf("%d", &fBrunch);
	FILE* phistory;
	phistory = fopen("Record_His.txt", "a+");
	fprintf(phistory, "搜索了brunch为%d的路\n", fBrunch);
	struct Node* pFind = NULL;
	pFind = phead;
	while (pFind != NULL)
	{
		if (pFind->data.brunch == fBrunch)
		{
			if (count == 0)
			{
				printf("岔路口数为%d的路:\n", fBrunch);
				fprintf(pFile, "岔路口数为%d的路:\n", fBrunch);
			}
			count++;
			if (count < 6)
			{
				printf("LinkID:%d   ", pFind->data.linkid);
				printf("dispclass：%d   ", pFind->data.dispclass);
				printf("description: %s\n", pFind->data.name);
			}
			fprintf(pFile, "LinkID:%d   dispclass：%d   description: %s\n", pFind->data.linkid, pFind->data.dispclass, pFind->data.name);
		}
		pFind = pFind->next;
	}
	/*----------------相似结果--------------------*/
	if (count == 0)
	{
		printf("        没有你所要查找的岔路口数的路!下面显示相似结果：\n");
		fBrunch = fBrunch - 5;
		for (int i = 0; i < 10; i++)
		{
			count = 0;
			pFind = phead;
			while (pFind != NULL)
			{
				if (pFind->data.brunch == fBrunch)
				{
					if (count == 0)
					{
						printf("岔路口数为%d的路:\n", fBrunch);
					}
					count++;
					if (count < 6)
					{
						printf("LinkID:%d   ", pFind->data.linkid);
						printf("dispclass：%d   ", pFind->data.dispclass);
						printf("description: %s\n", pFind->data.name);
					}
				}
				pFind = pFind->next;
			}
			if (count >= 6)
			{
				printf("仅显示前五条数据，其余数据存放到History_Brunch.txt中\n");
			}
			fBrunch++;
		}
	}
	if (count >= 6)
	{
		printf("仅显示前五条数据，其余数据存放到History_description.txt中\n");
	}
	fclose(pFile);
	fclose(phistory);
}
/*name检索*/
void Search_Name(struct Node* phead)
{
	int count = 0;
	char fdescription[20];
	FILE* pFile;
	pFile = fopen("History_description.txt", "w+");//打开文件
	printf("请输入需要查找的路的description:");
	getchar();//吸收空格
	gets(fdescription);
	FILE* phistory;
	phistory = fopen("Record_His.txt", "a+");
	fprintf(phistory, "搜索了linkid为%s的路\n", fdescription);
	struct Node* pFind = NULL;
	pFind = phead;
	while (pFind != NULL)
	{
		if (strcmp(pFind->data.name, fdescription) == 0)
		{
			if (count == 0)
			{
				printf("description为%s的路:\n", fdescription);
				fprintf(pFile, "description为%s的路:\n", fdescription);
			}
			count++;
			if (count < 6)
			{
				printf("LinkID:%d   ", pFind->data.linkid);
				printf("dispclass：%d   ", pFind->data.dispclass);
				printf("岔路口数: %d\n", pFind->data.brunch);
			}
			fprintf(pFile, "LinkID:%d   dispclass：%d   岔路口数: %d\n", pFind->data.linkid, pFind->data.dispclass, pFind->data.brunch);
		}
		pFind = pFind->next;
	}
	if (count >= 6)
	{
		printf("仅显示前五条数据，其余数据存放到History_description.txt中\n");
	}
	if (count == 0)
	{
		printf("没有你所要查找的description的路!\n");
	}
	fclose(pFile);
	fclose(phistory);
}
/*主函数*/
int main()
{
	struct Node* phead;
	int m, n = 1, p, q;
	while (n != 0)
	{
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("\n");
		printf("                                      * * * * * * * * * * * * * * * * * * * * * * *\n");
		printf("                                      *        欢迎使用西硬地图，请选择功能       *\n");
		printf("                                      *        1.读取文件(E:\\GTBL.dat)           *\n");
		printf("                                      *        2.排序                             *\n");
		printf("                                      *        3.检索                             *\n");
		printf("                                      *        4.更新                             *\n");
		printf("                                      *        5.设置                             *\n");
		printf("                                      *        0.退出                             *\n");
		printf("                                      * * * * * * * * * * * * * * * * * * * * * * *\n");
		printf("                                      * 请输入功能序号：");
		scanf_s("%d", &m);
		if (m == 0)
		{
			return 0;
		}
		if (m == 1)
		{
			Sleep(1000);
			system("CLS");
			phead = create();
			gettext(phead);
			delete_list(phead);
		}
		if (m == 2)
		{
			Sleep(1000);
			system("CLS");
			phead = create();
			sort(phead);
			delete_list(phead);
		}
		if (m == 3)
		{
			Sleep(1000);
			system("CLS");
			phead = create();
			printf("\n");
			printf("\n");
			printf("选择您想要的检索依据：1.linkid  2.dispclass  3.岔路口数  4.道路名称  5.返回主界面\n");
			scanf_s("%d", &q);
			switch (q)
			{
			case 1:Search_ID(phead); break;
			case 2:Search_dispclass(phead); break;
			case 3:Search_Brunch(phead); break;
			case 4:Search_Name(phead); break;
			case 5:break;
			default:
				printf("                                      * 输入错误指令，请重新输入 *\n");
				Sleep(1000); system("CLS"); break;
			}
			delete_list(phead);
		}
		if (m == 4)
		{
			Sleep(1000);
			system("CLS");
			phead = create();
			int t;
			printf("请选择您的更新方式：1.增加道路信息 2.删除道路信息 3.修改道路信息 4.返回主界面\n");
			scanf_s("%d", &t);
			switch (t)
			{
			case 1:Add_Data(); break;
			case 2:Del_Data(phead); break;
			case 3:Change_Data(phead); break;
			case 4:break;
			default:
				printf("                                      * 输入错误指令，请重新输入 *\n");
				Sleep(1000); system("CLS"); break;
			}
			delete_list(phead);
		}
		if (m == 5)
		{
			Sleep(1000);
			system("CLS");
			printf("\n");
			printf("\n");
			printf("\n");
			printf("\n");
			printf("\n");
			printf("                                      * * * * * * * * * * * * * * * * * * * * * * *\n");
			printf("                                      *       请设定您的专属设置（背景 字体）     *\n");
			printf("                                      *           1.黑色 红色                     *\n");
			printf("                                      *           2.绿色 黄色                     *\n");
			printf("                                      *           3.紫色 蓝色                     *\n");
			printf("                                      *           4.黑色 蓝色                     *\n");
			printf("                                      *           5.白色 紫色                     *\n");
			printf("                                      *           6.白色 绿色                     *\n");
			printf("                                      *           7.白色 蓝色                     *\n");
			printf("                                      *           8.蓝色 黑色                     *\n");
			printf("                                      *           9.返回主界面                    *\n");
			printf("                                      *           0.黑色 白色                     *\n");
			printf("                                      * * * * * * * * * * * * * * * * * * * * * * *\n");
			printf("                                      * 设置您的背景：");
			scanf_s("                                     %d", &p);
			switch (p)
			{
			case 0:   system("color 07"); system("CLS"); break;
			case 1:   system("color 04"); system("CLS"); break;
			case 2:   system("color 2E"); system("CLS"); break;
			case 3:   system("color 59"); system("CLS"); break;
			case 4:   system("color 01"); system("CLS"); break;
			case 5:   system("color 75"); system("CLS"); break;
			case 6:   system("color 72"); system("CLS"); break;
			case 7:   system("color F9"); system("CLS"); break;
			case 8:   system("color 30"); system("CLS"); break;
			case 9:break;
			default:
				printf("                                      * 输入错误指令，请重新输入 *\n");
				Sleep(1000); system("CLS"); break;
			}
		}
		if (m < 0 || m>5)
		{
			printf("                                          * 输入错误指令，请重新输入 *\n");
			Sleep(1000);
			system("CLS");
		}
	}
	return 0;
}