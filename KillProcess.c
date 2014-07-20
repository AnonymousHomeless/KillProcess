#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <TLHelp32.h>

void locateProcess();				//查看进程
void killProcess(int iProcessId);	//杀死进程

int main()
{
	int a;
	int iProcessId;
	int LOOP = 1;
	char cFlag[10];

	locateProcess();
	while(LOOP)
	{
		printf("请输入你想杀死的进程ID：");
		scanf("%d", &iProcessId);
		killProcess(iProcessId);
		locateProcess();
		printf("输入exit退出，输入其他字符继续杀进程。\n");
		fflush(stdin);
		scanf("%s", cFlag);
		//gets(cFlag);
		if ((strcmp("exit",cFlag) == 0) || (strcmp("EXIT",cFlag) == 0))
		{
			LOOP = 0;
		}	
	}

	return 0;
}

void locateProcess()
{
	HANDLE hSnapshot;
	PROCESSENTRY32 pe32;

	hSnapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0);     //获得进程快照句柄
	
	if (hSnapshot == INVALID_HANDLE_VALUE)
	{
		printf("获得进程快照句柄失败！");
	}

	pe32.dwSize = sizeof(PROCESSENTRY32);

	//显示进程列表中的第一个进程
	if (!Process32First(hSnapshot, &pe32))               
	{
		printf("显示进程失败！");
		CloseHandle(hSnapshot);   //释放句柄
	}

	//显示接下的所有进程
	printf("进程ID\t父进程\t线程数\t优先级\t进程名\n"); 
	while(Process32Next(hSnapshot, &pe32))
	{
		printf("%u\t", pe32.th32ProcessID);		      //进程ID
		printf("%u\t", pe32.th32ParentProcessID);	  //父进程ID
		printf("%d\t", pe32.cntThreads);			  //线程数
		printf("%d\t", pe32.pcPriClassBase);		  //基本优先级
		printf("%S\t\n",pe32.szExeFile);				  //进程名
	}

	CloseHandle(hSnapshot);
}


void killProcess(int iProcessId)
{
	HANDLE hProcess;
	hProcess = OpenProcess( PROCESS_ALL_ACCESS, FALSE, (DWORD)iProcessId);
	TerminateProcess(hProcess, 0);
}
