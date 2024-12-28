#include<stdio.h>
#include<string.h>
#include<cstdlib>
#define MAX_NAME_LEN 256
#define MAX_FILE_SIZE 1024

typedef enum { directory, file } FileType;

typedef struct File {
	char name[MAX_NAME_LEN];
	FileType type;
	union {
		struct File* children; // 对于目录类型
		char* content;         // 对于文件类型
	} data;
	int size; // 对于文件类型，表示文件大小
	struct File* parent;
	struct File* next;
} File;

File* root; // 文件系统的根目录
File* current_dir; // 当前工作目录

void init_root() {
	root = (File*)malloc(sizeof(File));
	strncpy(root->name, "/", MAX_NAME_LEN);
	root->type = directory;
	root->parent = NULL;
	root->next = NULL;
	root->data.children = NULL;
	current_dir = root;
}

void create(File* parent, const char* name, FileType type) {
	File* new_file = (File*)malloc(sizeof(File));
	strncpy(new_file->name, name, MAX_NAME_LEN);
	new_file->type = type;
	new_file->parent = parent;
	new_file->next =nullptr;
	new_file->size = 0;
	
	if (type == directory) {
		new_file->data.children = NULL;
	} else {
		new_file->data.content = (char*)malloc(MAX_FILE_SIZE);
		memset(new_file->data.content, 0, MAX_FILE_SIZE);
	}
	
	// 将新文件添加到父目录的子文件列表末尾
	
	
	if(parent->data.children==NULL){
		parent->data.children=new_file;	
	}else{
		File* child = parent->data.children;
		while (child->next) {
			child = child->next;
		}
		child->next = new_file;
	}
	
}

File* find(File* parent, const char* name) {
	File* child = parent->data.children;
	while (child) {
		if (strcmp(child->name, name) == 0) {
			//printf("it is %s\n",name);
			return child;
		}
		//printf("is %s\n",name);
		child = child->next;
	}
	return NULL; // 未找到文件
}

void read(File* file1) {
	if (file1->type == file) {
		printf("%s", file1->data.content);
	} else {
		printf("Cannot read a directory.\n");
	}
}

void write(File* file1) {
	if (file1->type == file) {
		char content[1500];
		printf("Enter what you want to write: ");
		fgets(content,sizeof(content),stdin);
		strncpy(file1->data.content, content, MAX_FILE_SIZE);
		file1->size = strlen(content);
		//test
		//printf("success %s",content);
	} else {
		printf("Cannot write to a directory.\n");
	}
}
//
void close(File* file) {
	// 在模拟文件管理系统中，关闭文件的操作可能不需要实现具体功能
	
}

void cd(const char* dir_name) {
	// 检查是否是返回上一级目录
	if (strcmp(dir_name, "..") == 0) {
		if (current_dir->parent != nullptr) { // 确保当前目录不是根目录
			current_dir = current_dir->parent;
		}
		return;
	}
	
	// 检查是否是返回根目录
	if (strcmp(dir_name, "/") == 0) {
		current_dir = root;
		return;
	}
	//
	//printf("old: current_dir:%s dir_name:%s\n",current_dir,dir_name);
	File* new_dir = find(current_dir, dir_name);
	if (new_dir!=nullptr && new_dir->type == directory) {
		current_dir = new_dir;
	} else {
		printf("Directory not found.\n");
	}
	//printf("new: current_dir:%s dir_name:%s\n",current_dir,dir_name);
}

void pwd() {
	if (current_dir == root) {
		printf("/");
	} else {
		File* dir = current_dir;
		char path[MAX_FILE_SIZE] = {0}; // 假设路径长度不会超过文件大小限制
		char temp[MAX_FILE_SIZE] = {0};
		
		// 从当前目录向上遍历到根目录，构建路径
		while (dir != root) {
			strcat(temp, dir->name); // 添加当前目录名
			strcat(temp, "/");       // 添加分隔符
			strcat(path, temp);      // 将当前目录名添加到路径的开始
			memset(temp, 0, sizeof(temp)); // 清空临时字符串
			dir = dir->parent;       // 移动到父目录
		}
		printf("%s\n", path); // 打印路径
	}
}


void ls(File* dir) {
	File* child = dir->data.children;
	while (child) {
		printf("%s\n", child->name);
		child = child->next;
	}
}

void mkdir(File* parent, const char* dir_name) {
	create(parent, dir_name, directory);
}
//
void rm(File* parent, const char* name) {
	File* child = parent->data.children;
	File* prev = NULL;
	while (child) {
		if (strcmp(child->name, name) == 0) {
			if (prev) {
				prev->next = child->next;
			} else {
				parent->data.children = child->next;
			}
			free(child);
			return;
		}
		prev = child;
		child = child->next;
	}
	printf("File or directory not found.\n");
}

void touch(File* parent, const char* file_name) {
	create(parent, file_name, file);
}
//
void exit_file_system() {
	// 释放所有分配的资源
	//
}

void init(){
	while (1)
	{
		start:
		printf("Enter command: ");
		char input[1024];
		char command[10];
		char filename[MAX_NAME_LEN];
		FileType type;
		fgets(input,sizeof(input),stdin);
		if (sscanf(input,"%s %s",command,filename)==2)
		{
			if(strcmp(command,"touch") == 0){//类似touch 1.txt
				touch(current_dir,filename);
				goto start;
			}else if(strcmp(command,"mkdir")==0){//类似mkdir mulu
				mkdir(current_dir,filename);
			}else if(strcmp(command,"write")==0){//write 1.txt /新开窗口
				File* curFile = find(current_dir,filename);
				if(curFile==NULL){
					printf("No such file\n");
				}else{
					write(curFile);
				}
				goto start;
			}else if(strcmp(command,"read")==0){
				File* curFile = find(current_dir,filename);
				if(curFile==NULL){
					printf("No such file\n");
				}else{
					read(curFile);
				}
			}else if(strcmp(command,"cd")==0){
				cd(filename);
			}else if(strcmp(command,"rm")==0){
				rm(current_dir,filename);
			}
		}else if(sscanf(input,"%s",command)==1){// ls
			if(strcmp(command,"ls")==0){
				ls(current_dir);
			}else if (strcmp(command,"exit")==0)
			{
				break;
			}else if(strcmp(command,"pwd")==0){
				pwd();
			}
			
		}
		
	}
	
}

int main(){
	init_root();
	init();
	
	return 0;
}
