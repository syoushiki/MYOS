#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

// 定义两个互斥锁
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;

// 线程1的函数
void* thread1_func(void* arg) {
    printf("线程1: 尝试锁定 mutex1...\n");
    pthread_mutex_lock(&mutex1);
    printf("线程1: 已锁定 mutex1.\n");

    // 模拟处理时间
    sleep(1);

    printf("线程1: 尝试锁定 mutex2...\n");
    pthread_mutex_lock(&mutex2);
    printf("线程1: 已锁定 mutex2.\n");

    // 模拟处理时间
    sleep(1);

    // 解锁
    pthread_mutex_unlock(&mutex2);
    pthread_mutex_unlock(&mutex1);
    printf("线程1: 已解锁 mutex1 和 mutex2.\n");
    return NULL;
}

// 线程2的函数
void* thread2_func(void* arg) {
    printf("线程2: 尝试锁定 mutex2...\n");
    pthread_mutex_lock(&mutex2);
    printf("线程2: 已锁定 mutex2.\n");

    // 模拟处理时间
    sleep(1);

    printf("线程2: 尝试锁定 mutex1...\n");
    pthread_mutex_lock(&mutex1);
    printf("线程2: 已锁定 mutex1.\n");

    // 模拟处理时间
    sleep(1);

    // 解锁
    pthread_mutex_unlock(&mutex1);
    pthread_mutex_unlock(&mutex2);
    printf("线程2: 已解锁 mutex2 和 mutex1.\n");
    return NULL;
}

int main() {
    pthread_t thread1, thread2;

    // 创建线程1
    if (pthread_create(&thread1, NULL, thread1_func, NULL) != 0) {
        perror("创建线程1失败");
        exit(EXIT_FAILURE);
    }

    // 创建线程2
    if (pthread_create(&thread2, NULL, thread2_func, NULL) != 0) {
        perror("创建线程2失败");
        exit(EXIT_FAILURE);
    }

    // 等待线程结束（在死锁情况下，程序会一直阻塞在这里）
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    // 销毁互斥锁
    pthread_mutex_destroy(&mutex1);
    pthread_mutex_destroy(&mutex2);

    return 0;
}
