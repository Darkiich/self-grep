#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <windows.h>

void paintAndPrintText(char* text, int color)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(hConsole, color);
	printf("%s", text);
	SetConsoleTextAttribute(hConsole, 7);
}

// grep, который рабоатет через аргументы -p | --path и -s | --str
// -p | --path - путь до файла
// -s | --str - строка для поиска в тексте
int main(int argc, char *argv[])
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	char* paths[100];
	int paths_count = 0;

	char* path_value = NULL;
	char* str_value = NULL;
	int showAllTextInfo = 1;

	int found = 0;

	char buffer[4096];

	int foundInput = 0;
	int line_number = 0;

	DWORD start_time, end_time;

	// 1. Проверка аргументов и их значений
	for (int i = 1; i < argc; i++)
	{
		if (strcmp(argv[i], "-p") == 0 || strcmp(argv[i], "--path") == 0)
		{
			if (i + 1 < argc) {
				paths[paths_count++] = argv[++i];
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
		else if (strcmp(argv[i], "-sm") == 0 || strcmp(argv[i], "--show-more") == 0)
		{
			if (i + 1 < argc) {
				showAllTextInfo = strtol(argv[++i], NULL, 10);
			}
			else {
				puts("ERROR. Write down a value -sm/--show-more");
				return 1;
			}
		}
		else {
			printf("Unknown argument: %s\n", argv[i]);
			return 1;
		}
	}

	// 2. Проверка аргументов на их существование
	if (paths_count == 0)
	{
		puts("The path of file didn't write");
		return 1;
	}
	else if (str_value == NULL)
	{
		puts("The string didn't write");
		return 1;	
	}

	if (showAllTextInfo) {
		printf("Files to search:\n");
		for (int i = 0; i < paths_count; i++) {
			printf("  - ");
			paintAndPrintText(paths[i], 11); // Голубой
			printf("\n");
		}
		printf("Look for string - ");
		paintAndPrintText(str_value, 14); // Желтый
		printf("\n\n");
	}


	// 3. Открытие и чтение текста из файла
	start_time = GetTickCount();

	for (int file_idx = 0; file_idx < paths_count; file_idx++)
	{
		if (showAllTextInfo) {
			printf("\nOpen the file: ");
			paintAndPrintText(paths[file_idx], 11); // Голубой
			printf(" by the path....\n");
		}

		FILE* file = fopen(paths[file_idx], "r");
		if (file == NULL) {
			printf("Cannot open file: ");
			paintAndPrintText(paths[file_idx], 12); // Красный
			printf("\n");
			continue;
		}

		line_number = 0;
		int file_found = 0;

		while (fgets(buffer, sizeof(buffer), file) != NULL)
		{
			line_number++;

			if (strstr(buffer, str_value) != NULL)
			{
				found = 1;
				file_found = 1;
				foundInput++;

				if (showAllTextInfo) {
					char line_number_str[16];
					itoa(line_number, line_number_str, 10);

					printf("Line ");
					paintAndPrintText(line_number_str, 14); // Желтый
					printf(": %s", buffer);
				}
			}
		}

		fclose(file);

		// Выводим результат для текущего файла
		if (showAllTextInfo) {
			puts("");
			if (file_found) {
				printf("File ");
				paintAndPrintText(paths[file_idx], 11);
				printf(": word found\n");
			}
			else {
				printf("File ");
				paintAndPrintText(paths[file_idx], 11);
				printf(": word not found\n");
			}
		}
	}

	end_time = GetTickCount();

	if (found != 0 && showAllTextInfo == 1) {
		printf("\nThe word is ");
		paintAndPrintText(str_value, 10); // Зеленый
		printf(" found in text %d times\n", foundInput);

		paintAndPrintText("The word found for", 11); // Голубой
		printf(" %lu ms\n", end_time - start_time);
	}
	else if (found != 0 && showAllTextInfo == 0) {
		printf("Found %d times\n", foundInput);
	}
	else if (found == 0 && showAllTextInfo == 0) {
		printf("Not found\n");
	}
	else {
		printf("The word is ");
		paintAndPrintText(str_value, 12); // Красный
		printf(" didn't find in text\n");
	}

	return 0;
}