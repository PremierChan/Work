#include <stdio.h>

#include <time.h>
#include <unistd.h>

void time_out (void);
int change_fail(void);
FILE* lni(const char*);
int testing(const char*);
FILE* change_stream(const char*);
//int test_line()

//int testing_file(const char*);

int main(int argc, const char *argv[]){

	FILE* fp = NULL;

	char buf[100] = {};

	int a =0;

	if(!(fp = lni(argv[1])))
		goto ERROR;

	
	while(fgetc(buf,21,fp)){
		
		a++;	
		
	}
	printf("%d \n",a);

	time_out();


	




ERROR:{	
	
	return 0;
	 }
}

void time_out (void){

	struct tm *time_now;
	time_t t_sec = time(NULL);
	time_now = localtime(&t_sec);

	int year = time_now ->tm_year + 1900;
	int mon = time_now ->tm_mon;
	int day = time_now ->tm_mday;
	int hour = time_now ->tm_hour;
	int min = time_now ->tm_min;
	int sec = time_now ->tm_sec;
	
	printf("%4d-%2d-%2d  %2d:%2d:%2d \n"
			,year,mon,day,hour,min,sec);
}

FILE* change_stream(const char* argv){

	FILE * fp = NULL;
	fp = freopen(argv,"a+",stdout);
	return fp;
}


int change_fail(void){
	
	puts("***************************************");
	puts("没有指定输出文件，结果将会打印到屏幕");
	puts("打印3次后自动结束，或Ctrl + C 强制结束");
	for(int freq = 0; freq < 3; freq++){
		sleep(1);
		printf("[%d] ",freq+1);
		time_out();
	}
	return -1;
}


int testing(const char* argv){

	if(fopen(argv,"r"))
		return 1;
	return 0;
}

FILE* lni(const char* argv){

	if(!argv)
		goto lni_error;
	else if(!testing(argv)){

		puts("你想创建该文件么？(y/n)");
		if(getchar() == 'y' || getchar() == 'Y')
			return change_stream(argv);
		else
			goto lni_error;
	}
	printf("文件存将在  行继续打印\n");
	return change_stream(argv);

lni_error:{

	change_fail();
	return NULL;
	}
}		
