#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <windows.h>

// grep, который рабоатет через аргументы -p | --path и -s | --str
// -p | --path - путь до файла
// -s | --str - строка для поиска в тексте
int main(int argc, char *argv[])
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	char* path_value = NULL;
	char* str_value = NULL;

	int found = 0;

	char buffer[4096];

	// 1. Проверка аргументов и их значений
	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--path") == 0)
		{
			if (i + 1 < argc) {
				path_value = argv[++i];
			}
			else {
				puts("ERROR. Write down a value after -p/--path");
				return 1;
			}
		}
		else if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--str") == 0)
		{
			if (i + 1 < argc) {
				str_value = argv[++i];
			}
			else {
				puts("ERROR. Write down a value -s/--str");
				return 1;
			}
		}
		else {
			printf("Unknown argument: %s\n", argv[i]);
			return 1;
		}
	}

	// 2. Проверка аргументов на их существование
	if (path_value == NULL)
	{
		puts("The path of file didn't write");
		return 1;
	}
	else if (str_value == NULL)
	{
		puts("The string didn't write");
		return 1;	
	}
	else {
		printf("The path - %s\nLook for string - %s\n\n", path_value, str_value);
	}


	// 3. Открытие и чтение текста из файла
	puts("Open the file by the path\n");

	FILE* file = fopen(path_value, "r");
	if (file == NULL) { perror("Cannot open the file"); return 1; }

	while (fgets(buffer, sizeof(buffer), file) != NULL)
	{
		if (strstr(buffer, str_value) != NULL)
		{
			found = 1;
			break;
		}
	}

	fclose(file);

	if (found != 0)
		printf("The word is %s found in text\n", str_value);
	else {
		printf("The word is %s didn't find in text\n", str_value);
		return 1;
	}

	return 0;
}