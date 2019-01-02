//#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<ctime>
#include<random>
#include<conio.h>
#include<tchar.h>
#include<Windows.h>
struct b {
	int x;
	int y;
};

//int g() { return 1; }
//int f() { return g() -1; }
void showMessage(int input) {
	
	if (319 == input)
		MessageBox(NULL, "how!!!", "message", MB_OK);
	else
		MessageBox(NULL, "ye ye", "message", MB_OK);
}
int main() {
	printf("wellcome!!!");
	
	/*srand(time(0));
	int input = rand();*/
	b b1{ 3,3 };
	//DWORD pid = GetCurrentProcessId();
	TCHAR filename[] = TEXT("C:\\users\\shmay\\MyFileMappingObject");
	TCHAR objname[] = TEXT("myobj");
	HANDLE crfh = CreateFileA(filename, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	HANDLE fh = CreateFileMapping(crfh, NULL, PAGE_READWRITE, 0, 256, objname);
	printf("get last err %d", GetLastError());
	char* openFileH = (char*)MapViewOfFile(fh, FILE_MAP_ALL_ACCESS, 0, 0, 256);
	memcpy(openFileH, &b1, sizeof(b));
	b*bp = (b*)(openFileH);
	printf("%d %d ", bp->x, bp->y);
	/*showMessage(input);*/

}