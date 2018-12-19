//#include<stdlib.h>
#include<stdio.h>
#include<time.h>
#include<ctime>
#include<random>
#include<Windows.h>
int g() { return 1; }
int f() { return g() -1; }
void showMessage(int input) {
	
	if (319 == input)
		MessageBox(NULL, "how!!!", NULL, MB_OK);
	else
		MessageBox(NULL, "ye ye", NULL, MB_OK);
}
int main() {
	printf("wellcome!!!");
	
	srand(time(0));
	int input = rand();
	
	showMessage(input);
}