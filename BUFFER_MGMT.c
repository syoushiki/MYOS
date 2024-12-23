#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <queue>
#include <fstream>
#include <string>
#include <unordered_map>
#include <sstream>
#include <filesystem>

const int BUFFER_SIZE = 10; // 缓冲区大小
std::queue<std::string> buffer; // 共享缓冲区
std::mutex mtx; // 互斥锁
std::condition_variable not_full; // 条件变量，缓冲区非满
std::condition_variable not_empty; // 条件变量，缓冲区非空
bool finished = false; // 生产者是否完成

// 文件处理模块
void processFileContent(const std::string& content) {
    std::unordered_map<std::string, int> wordCount;
    std::istringstream stream(content);
    std::string word;

    while (stream >> word) {
        ++wordCount[word];
    }

    // 输出单词统计结果
    for (const auto& pair : wordCount) {
        std::cout << pair.first << ": " << pair.second << std::endl;
    }
}

// 生产者线程
void producer(const std::string& directory) {
    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
        if (entry.is_regular_file()) {
            std::ifstream file(entry.path());
            std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
            file.close();

            std::unique_lock<std::mutex> lock(mtx);
            not_full.wait(lock, [] { return buffer.size() < BUFFER_SIZE; }); // 等待缓冲区非满
            buffer.push(content); // 将文件内容放入缓冲区
            not_empty.notify_one(); // 通知消费者
        }
    }

    // 生产者完成
    std::unique_lock<std::mutex> lock(mtx);
    finished = true;
    not_empty.notify_all(); // 通知所有消费者
}

// 消费者线程
void consumer() {
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);
        not_empty.wait(lock, [] { return !buffer.empty() || finished; }); // 等待缓冲区非空

        if (!buffer.empty()) {
            std::string content = buffer.front();
            buffer.pop();
            lock.unlock(); // 解锁以允许其他线程访问缓冲区

            processFileContent(content); // 处理文件内容
        }
        else if (finished) {
            break; // 如果生产者完成且缓冲区为空，退出循环
        }
    }
}

int main() {
    std::string directory = "path/to/your/directory"; // 指定要遍历的文件目录

    std::thread producerThread(producer, directory);
    const int consumerCount = 3; // 消费者线程数量
    std::vector<std::thread> consumers;

    for (int i = 0; i < consumerCount; ++i) {
        consumers.emplace_back(consumer);
    }

    producerThread.join(); // 等待生产者线程完成

    for (auto& consumerThread : consumers) {
        consumerThread.join(); // 等待所有消费者线程完成
    }

    return 0;
}
