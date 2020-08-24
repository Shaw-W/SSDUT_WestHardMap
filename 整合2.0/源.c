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

/*��·�ṹ��*/
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

/*����ڵ�*/
struct Node
{
	Data data;
	struct Node* next;
};

/*����������ȡ�ļ�*/
struct Node* create()
{
	int number = 0;//��Ž�����
	struct Node* pHead = NULL;
	struct Node* pTail = NULL;
	unsigned short size;//����
	FILE* fin;
	fin = fopen("E:\\GTBL.dat", "rb");
	char address[20] = "E:\\GTBL.dat";
	if (fin == NULL)
	{
		printf("\n");
		printf("\n");
		printf("                                      �ļ���ȡʧ��,���ܲ�����\n");
		printf("                                              �����ַ:");
		scanf("%s", address);
		fin = fopen(address, "rb");
		if (fin != NULL)
			printf("                                       �ļ���ȡ�ɹ�\n");
	}
	else
	{
		printf("\n");
		printf("\n");
		printf("                                           �ļ���ȡ�ɹ�\n");
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
	printf("                                       �ѽ��ļ���ŵ�%sλ��\n", address);
	return pHead;
}
/*��ӡ����*/
void show(struct Node* phead)
{
	struct Node* p;
	p = phead;
	while (p)
	{
		printf("linkID:%d  ", p->data.linkid);
		printf("dispclass:%d   ", p->data.dispclass);
		printf("��·������%d   ", p->data.brunch);
		printf("description: %s\n", p->data.name);
		p = p->next;
	}
}

/*����ȡ��txt�ļ�����*/
void gettext(struct Node* phead)
{
	struct Node* p;
	p = phead;
	FILE* fpWrite = fopen("E:\\gettext.txt", "w");
	while (p)
	{
		fprintf(fpWrite, "linkid:%d   ", p->data.linkid);
		fprintf(fpWrite, "dispclass:%d   ", p->data.dispclass);
		fprintf(fpWrite, "��·������%d   ", p->data.brunch);
		fprintf(fpWrite, "description: %s\n", p->data.name);
		p = p->next;
	}
}

/*ɾ������*/
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

/*����*/
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
	printf("                ѡ��������Ҫ������ʽ��1.ð������  2.��������  3.ѡ������  4.����������\n");
	int choice1;
	scanf("%d", &choice1);

	/*ð��*/
	if (choice1 == 1)
	{
		printf("                       ѡ��������Ҫ��������ݣ�1.linkid  2.dispclass  3..brunch\n");
		int choice2;
		scanf("%d", &choice2);
		printf("                            ��ѡ���������������У�1.����     2.����\n");
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
			printf("����֮���ǰ��������Ϊ��\n");
			if (choice3 == 1)
			{
				for (i = 0; i < length; i++)
				{
					fprintf(fpWrite, "linkid:%d   ", (array + i)->data.linkid);
					fprintf(fpWrite, "dispclass:%d   ", (array + i)->data.dispclass);
					fprintf(fpWrite, "brunch��%d   ", (array + i)->data.brunch);
					fprintf(fpWrite, "description: %s\n", (array + i)->data.name);
					if (cnt <= 4)
					{
						printf("linkID:%d  ", (array + i)->data.linkid);
						printf("dispclass:%d   ", (array + i)->data.dispclass);
						printf("��·������%d   ", (array + i)->data.brunch);
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
					fprintf(fpWrite, "brunch��%d   ", (array + i)->data.brunch);
					fprintf(fpWrite, "description: %s\n", (array + i)->data.name);
					if (cnt <= 4)
					{
						printf("linkID:%d  ", (array + i)->data.linkid);
						printf("dispclass:%d   ", (array + i)->data.dispclass);
						printf("��·������%d   ", (array + i)->data.brunch);
						printf("description: %s\n", (array + i)->data.name);

					}
					cnt++;
				}
			}
		}

	}

	/*��������*/
	if (choice1 == 2)
	{
		printf("                       ѡ��������Ҫ��������ݣ�1.linkid  2.dispclass  3..brunch\n");
		int choice2;
		scanf("%d", &choice2);
		printf("                            ��ѡ���������������У�1.����     2.����\n");
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
			printf("   ����֮���ǰ��������Ϊ��\n");
			if (choice3 == 1)
			{
				for (i = 0; i < length; i++)
				{
					fprintf(fpWrite, "linkid:%d   ", (array + i)->data.linkid);
					fprintf(fpWrite, "dispclass:%d   ", (array + i)->data.dispclass);
					fprintf(fpWrite, "brunch��%d   ", (array + i)->data.brunch);
					fprintf(fpWrite, "description: %s\n", (array + i)->data.name);
					if (cnt <= 4)
					{
						printf("linkID:%d  ", (array + i)->data.linkid);
						printf("dispclass:%d   ", (array + i)->data.dispclass);
						printf("��·������%d   ", (array + i)->data.brunch);
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
					fprintf(fpWrite, "brunch��%d   ", (array + i)->data.brunch);
					fprintf(fpWrite, "description: %s\n", (array + i)->data.name);
					if (cnt <= 4)
					{
						printf("linkID:%d  ", (array + i)->data.linkid);
						printf("dispclass:%d   ", (array + i)->data.dispclass);
						printf("��·������%d   ", (array + i)->data.brunch);
						printf("description: %s\n", (array + i)->data.name);

					}
					cnt++;
				}
			}
		}
	}

	/*ѡ������*/
	if (choice1 == 3)
	{
		printf("                       ѡ��������Ҫ��������ݣ�1.linkid  2.dispclass  3..brunch\n");
		int choice2;
		scanf("%d", &choice2);
		printf("                            ��ѡ���������������У�1.����     2.����\n");
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
			printf("   ����֮���ǰ��������Ϊ��\n");
			if (choice3 == 1)
			{
				for (i = 0; i < length; i++)
				{
					fprintf(fpWrite, "linkid:%d   ", (array + i)->data.linkid);
					fprintf(fpWrite, "dispclass:%d   ", (array + i)->data.dispclass);
					fprintf(fpWrite, "brunch��%d   ", (array + i)->data.brunch);
					fprintf(fpWrite, "description: %s\n", (array + i)->data.name);
					if (cnt <= 4)
					{
						printf("linkID:%d  ", (array + i)->data.linkid);
						printf("dispclass:%d   ", (array + i)->data.dispclass);
						printf("��·������%d   ", (array + i)->data.brunch);
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
					fprintf(fpWrite, "brunch��%d   ", (array + i)->data.brunch);
					fprintf(fpWrite, "description: %s\n", (array + i)->data.name);
					if (cnt <= 4)
					{
						printf("linkID:%d  ", (array + i)->data.linkid);
						printf("dispclass:%d   ", (array + i)->data.dispclass);
						printf("��·������%d   ", (array + i)->data.brunch);
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

/*�������*/
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

	printf("��������Ҫ��ӵ�������Ϣ��\n");


	printf(" ��·linkid:");
	scanf("%d", &linkidTMP);

	printf(" ��·flag:");
	scanf("%d", &flagTMP);

	printf(" ��·brunch:");
	scanf("%d", &brunchTMP);

	printf(" ��·dispclass:");
	scanf("%d", &dispclassTMP);

	if (flagTMP == 1)
	{
		printf(" ��·name:");
		getchar();//���ջس�
		gets(nameTMP);
	}

	printf("��Ϣ�����... ...");

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

	printf("������");
	fclose(FA);
}
/*ɾ������*/
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
	printf("��������Ҫɾ����·��LinkID:");
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
		printf("δ���ҵ�Ҫɾ�������ݣ�\n");
		return;
	}
	else
	{
		printf("�����޸�... ...");
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
		printf("�޸����");
		fclose(fp);
	}
}
/*�޸�����*/
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
	printf("��������Ҫ���ĵ�ԭ·��LinkID:");
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
		printf("δ���ҵ�Ҫ���ĵ����ݣ�\n");
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
	printf("��������Ҫ���µ�������Ϣ��\n");

	printf(" ��·linkid:");
	scanf("%d", &linkidTMP);

	printf(" ��·flag:");
	scanf("%d", &flagTMP);

	printf(" ��·brunch:");
	scanf("%d", &brunchTMP);

	printf(" ��·dispclass:");
	scanf("%d", &dispclassTMP);

	if (flagTMP == 1)
	{
		printf(" ��·name:");
		getchar();//���ջس�
		gets(nameTMP);
	}

	printf("��Ϣ������... ...");

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

	printf("�������");
	fclose(FA);
}
/*id����*/
void Search_ID(struct Node* phead)
{
	int count = 0;
	unsigned int fLinkID = 0;
	FILE* pFile;
	pFile = fopen("History_LinkID.txt", "w+");//���ļ�
	printf("��������Ҫ���ҵ�·��LinkID:");
	scanf("%d", &fLinkID);
	FILE* phistory;
	phistory = fopen("Record_His.txt", "a+");
	fprintf(phistory, "������linkidΪ%d��·\n", fLinkID);
	struct Node* pFind = NULL;
	pFind = phead;
	while (pFind != NULL)
	{
		if (pFind->data.linkid == fLinkID)
		{
			if (count == 0)
			{
				printf("LinkIDΪ%d��·:\n", fLinkID);
				fprintf(pFile, "LinkIDΪ%d��·:\n", fLinkID);
			}
			count++;
			if (count < 6)
			{
				printf("dispclass:%d   ", pFind->data.dispclass);
				printf("��·������%d   ", pFind->data.brunch);
				printf("description: %s\n", pFind->data.name);
			}
			fprintf(pFile, "dispclass:%d   ��·������%d   description: %s\n", pFind->data.dispclass, pFind->data.brunch, pFind->data.name);
		}
		pFind = pFind->next;
	}
	if (count >= 6)
	{
		printf("����ʾǰ�������ݣ��������ݴ�ŵ�History_LinkID.txt��\n");
	}
	/*----------------���ƽ��--------------------*/
	if (count == 0)
	{
		printf("û������Ҫ���ҵ�LinkID��·!������ʾ���ƽ����\n");
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
						printf("LinkIDΪ%d��·:\n", fLinkID);
						fprintf(pFile, "LinkIDΪ%d��·:\n", fLinkID);
					}
					count++;
					if (count < 6)
					{
						printf("dispclass:%d   ", pFind->data.dispclass);
						printf("��·������%d   ", pFind->data.brunch);
						printf("description: %s\n", pFind->data.name);
					}
				}
				pFind = pFind->next;
			}
			if (count >= 6)
			{
				printf("����ʾǰ�������ݣ��������ݴ�ŵ�History_LinkID.txt��\n");
			}
			fLinkID++;
		}
	}
	fclose(pFile);
	fclose(phistory);
}
/*dispclass����*/
void Search_dispclass(struct Node* phead)
{
	int count = 0;
	int fdispclass = 0;
	FILE* pFile;
	pFile = fopen("History_dispclass.txt", "w+");//���ļ�
	printf("��������Ҫ���ҵ�·��dispclass:");
	scanf("%d", &fdispclass);
	FILE* phistory;
	phistory = fopen("Record_His.txt", "a+");
	fprintf(phistory, "������dispclassΪ%d��·\n", fdispclass);
	struct Node* pFind = NULL;
	pFind = phead;
	while (pFind != NULL)
	{
		if (pFind->data.dispclass == fdispclass)
		{
			if (count == 0)
			{
				printf("dispclassΪ%d��·:\n", fdispclass);
			}
			count++;
			if (count < 6)
			{
				printf("LinkID:%d   ", pFind->data.linkid);
				printf("��·������%d   ", pFind->data.brunch);
				printf("description: %s\n", pFind->data.name);
			}
			fprintf(pFile, "LinkID:%d   ��·������%d   description: %s\n", pFind->data.linkid, pFind->data.brunch, pFind->data.name);
		}
		pFind = pFind->next;
	}
	if (count >= 6)
	{
		printf("����ʾǰ�������ݣ��������ݴ�ŵ�History_dispclass.txt��\n");
	}
	/*----------------���ƽ��--------------------*/
	if (count == 0)
	{
		printf("û������Ҫ���ҵ�dispclass��·!������ʾ���ƽ����\n");
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
						printf("dispclassΪ%d��·:\n", fdispclass);
					}
					count++;
					if (count < 6)
					{
						printf("LinkID:%d   ", pFind->data.linkid);
						printf("��·������%d   ", pFind->data.brunch);
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
/*��·����*/
void Search_Brunch(struct Node* phead)
{
	int count = 0;
	int fBrunch = 0;
	FILE* pFile;
	pFile = fopen("History_Brunch.txt", "w+");//���ļ�
	printf("��������Ҫ���ҵ�·�Ĳ�·����:");
	scanf("%d", &fBrunch);
	FILE* phistory;
	phistory = fopen("Record_His.txt", "a+");
	fprintf(phistory, "������brunchΪ%d��·\n", fBrunch);
	struct Node* pFind = NULL;
	pFind = phead;
	while (pFind != NULL)
	{
		if (pFind->data.brunch == fBrunch)
		{
			if (count == 0)
			{
				printf("��·����Ϊ%d��·:\n", fBrunch);
				fprintf(pFile, "��·����Ϊ%d��·:\n", fBrunch);
			}
			count++;
			if (count < 6)
			{
				printf("LinkID:%d   ", pFind->data.linkid);
				printf("dispclass��%d   ", pFind->data.dispclass);
				printf("description: %s\n", pFind->data.name);
			}
			fprintf(pFile, "LinkID:%d   dispclass��%d   description: %s\n", pFind->data.linkid, pFind->data.dispclass, pFind->data.name);
		}
		pFind = pFind->next;
	}
	/*----------------���ƽ��--------------------*/
	if (count == 0)
	{
		printf("        û������Ҫ���ҵĲ�·������·!������ʾ���ƽ����\n");
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
						printf("��·����Ϊ%d��·:\n", fBrunch);
					}
					count++;
					if (count < 6)
					{
						printf("LinkID:%d   ", pFind->data.linkid);
						printf("dispclass��%d   ", pFind->data.dispclass);
						printf("description: %s\n", pFind->data.name);
					}
				}
				pFind = pFind->next;
			}
			if (count >= 6)
			{
				printf("����ʾǰ�������ݣ��������ݴ�ŵ�History_Brunch.txt��\n");
			}
			fBrunch++;
		}
	}
	if (count >= 6)
	{
		printf("����ʾǰ�������ݣ��������ݴ�ŵ�History_description.txt��\n");
	}
	fclose(pFile);
	fclose(phistory);
}
/*name����*/
void Search_Name(struct Node* phead)
{
	int count = 0;
	char fdescription[20];
	FILE* pFile;
	pFile = fopen("History_description.txt", "w+");//���ļ�
	printf("��������Ҫ���ҵ�·��description:");
	getchar();//���տո�
	gets(fdescription);
	FILE* phistory;
	phistory = fopen("Record_His.txt", "a+");
	fprintf(phistory, "������linkidΪ%s��·\n", fdescription);
	struct Node* pFind = NULL;
	pFind = phead;
	while (pFind != NULL)
	{
		if (strcmp(pFind->data.name, fdescription) == 0)
		{
			if (count == 0)
			{
				printf("descriptionΪ%s��·:\n", fdescription);
				fprintf(pFile, "descriptionΪ%s��·:\n", fdescription);
			}
			count++;
			if (count < 6)
			{
				printf("LinkID:%d   ", pFind->data.linkid);
				printf("dispclass��%d   ", pFind->data.dispclass);
				printf("��·����: %d\n", pFind->data.brunch);
			}
			fprintf(pFile, "LinkID:%d   dispclass��%d   ��·����: %d\n", pFind->data.linkid, pFind->data.dispclass, pFind->data.brunch);
		}
		pFind = pFind->next;
	}
	if (count >= 6)
	{
		printf("����ʾǰ�������ݣ��������ݴ�ŵ�History_description.txt��\n");
	}
	if (count == 0)
	{
		printf("û������Ҫ���ҵ�description��·!\n");
	}
	fclose(pFile);
	fclose(phistory);
}
/*������*/
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
		printf("                                      *        ��ӭʹ����Ӳ��ͼ����ѡ����       *\n");
		printf("                                      *        1.��ȡ�ļ�(E:\\GTBL.dat)           *\n");
		printf("                                      *        2.����                             *\n");
		printf("                                      *        3.����                             *\n");
		printf("                                      *        4.����                             *\n");
		printf("                                      *        5.����                             *\n");
		printf("                                      *        0.�˳�                             *\n");
		printf("                                      * * * * * * * * * * * * * * * * * * * * * * *\n");
		printf("                                      * �����빦����ţ�");
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
			printf("ѡ������Ҫ�ļ������ݣ�1.linkid  2.dispclass  3.��·����  4.��·����  5.����������\n");
			scanf_s("%d", &q);
			switch (q)
			{
			case 1:Search_ID(phead); break;
			case 2:Search_dispclass(phead); break;
			case 3:Search_Brunch(phead); break;
			case 4:Search_Name(phead); break;
			case 5:break;
			default:
				printf("                                      * �������ָ����������� *\n");
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
			printf("��ѡ�����ĸ��·�ʽ��1.���ӵ�·��Ϣ 2.ɾ����·��Ϣ 3.�޸ĵ�·��Ϣ 4.����������\n");
			scanf_s("%d", &t);
			switch (t)
			{
			case 1:Add_Data(); break;
			case 2:Del_Data(phead); break;
			case 3:Change_Data(phead); break;
			case 4:break;
			default:
				printf("                                      * �������ָ����������� *\n");
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
			printf("                                      *       ���趨����ר�����ã����� ���壩     *\n");
			printf("                                      *           1.��ɫ ��ɫ                     *\n");
			printf("                                      *           2.��ɫ ��ɫ                     *\n");
			printf("                                      *           3.��ɫ ��ɫ                     *\n");
			printf("                                      *           4.��ɫ ��ɫ                     *\n");
			printf("                                      *           5.��ɫ ��ɫ                     *\n");
			printf("                                      *           6.��ɫ ��ɫ                     *\n");
			printf("                                      *           7.��ɫ ��ɫ                     *\n");
			printf("                                      *           8.��ɫ ��ɫ                     *\n");
			printf("                                      *           9.����������                    *\n");
			printf("                                      *           0.��ɫ ��ɫ                     *\n");
			printf("                                      * * * * * * * * * * * * * * * * * * * * * * *\n");
			printf("                                      * �������ı�����");
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
				printf("                                      * �������ָ����������� *\n");
				Sleep(1000); system("CLS"); break;
			}
		}
		if (m < 0 || m>5)
		{
			printf("                                          * �������ָ����������� *\n");
			Sleep(1000);
			system("CLS");
		}
	}
	return 0;
}