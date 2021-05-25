//=======     19          ==================================
/*
19. ������� ���� ������
���������, ������� ���������� ������������ ������ ������ ����� �����,
����������� ����, ������� ������������ � shell.
��������� ������� �����:

	 *   ������������� ������������������ ����� �������� ����� /,
		 ������� ����� �����; �������� - ������ ������������������.
	?    ������������� ������ ������ �������.
	/    �� ����� �����������.
	����� ������ ������ ������������� ������ ����.

   ������� * � ? � ������� ����� ����������� � ����� ���������� � � ����� �������.

 ����� ��������� ������ ����� � ����������� ����� ���� ������ � ������� ��������,
 ��������������� �������. ���� ����� ������ ���, ��������� ������ ����������� ��� ������.

 �����: ����������� readdir, ����� ������� ��� ����� ������ � ������� ��������,
		� �������� �� ��� ��������������� �������.
*/


#include <stdio.h>
//#include <sys / types.h>
#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <limits.h>
#include <glob.h>

#define SIZE 256
#define FIASCO -1
#define SUCCESS 0
#define FAILURE 2
#define BAD_SYMBOL 1

int main(int argc, char** argv) {
	char pattern[SIZE]; //������
	struct dirent* entry;
	//struct dirent {
	//	ino_t d_ino; / *inode number * /
	//		off_t d_off; / *offset to the next dirent* /
	//		����������� �������� d_reclen; / *length of this record * /
	//		����������� char d_type;       / *type of file; not supported; 
	//		                                   by all file system types* /
	//		char d_name[256];              / *filename with '/0' * /
	//};



	DIR* dir;
	int i = 0, length, count = 0, j, patternLength;
	int flag = 0;
	//1. ������� ������
	printf("Enter pattern: ");
	fgets(pattern, SIZE, stdin);
	patternLength = strlen(pattern) - 1;
	pattern[patternLength] = 0;
	for (i = 0; patternLength > i; i++) {
		if (pattern[i] == '/') {
			printf("Symbol / is not supported pattern\n");
			exit(BAD_SYMBOL);
		}
	}
	//2. ������� ������� ����������
	dir = opendir(".");  //���������� ��������� �� directory stream �������
	//��� ���������� ����� ������ ������ � ���������� ��� ������������� ������ ���������� NULL
	//: ������������� ERRNO: EACCES,ENOMEM, ENFILE, ENOENT etc
	if (!dir) {
		perror("opendir");
		exit(FAILURE);
	}
	//3.����� ������ ���������-� �������
//������� readdir () ���������� ��������� �� ��������� dirent
	while (NULL != (entry = readdir(dir))) {
		length = strlen(entry->d_name);
		j = 0;
		flag = 0;  //������� ���� ������
		for (i = 0; (length > i) && (patternLength > j); i++) {
			if (('?' != pattern[j]) && ('*' != pattern[j])) {
				if (pattern[j] != entry->d_name[i]) break;
				j++;
			}
			else if ('?' == pattern[j]) {
				j++;
			}
			else { //*
				while (patternLength > j) {
					j++;
					if ('*' != pattern[j]) break;
				}//�� * ������ ����������
				if (patternLength == j) {
					flag = 1;
					break;
				}//���� ����� * ���� ?
				if (pattern[j] == '?') {
					j++;
					continue;
				}
				while (length > i) {
					if (pattern[j] == entry->d_name[i]) {
						break;
					}
					i++;
				}
				j++;
			}
		}
		if (length == i) {
			while (patternLength > j) {
				if ('*' != pattern[j])
					break;
				j++;
			}
			if (patternLength == j) flag = 1;
		}
		if ((1 == flag) || (i == length && j == patternLength)) {
			printf("%s\n", entry->d_name);
			count++;
		}
	}
	if (!count) {
		printf("Pattern was = %s\n", pattern);
	}
	closedir(dir);
	return SUCCESS;
}

