#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <cstdio>
#include <filesystem>

// 最大文件名长度
constexpr int MAX_FILE_NAME_LENGTH = 256;
// 最大行长度
constexpr int MAX_LINE_LENGTH = 1024;
// 缓冲区大小
constexpr int BUFFER_SIZE = 10;

// 数据单元结构体，用于传递文件信息
struct DataUnit {
    char* text;
    int lineNumber;
    char fileName[MAX_FILE_NAME_LENGTH];
};

// 共享缓冲区类
class Buffer {
public:
    Buffer() : front(0), rear(0), count(0) {}
    ~Buffer() {
        clearBuffer();
    }

    // 将数据放入缓冲区
    void put(DataUnit* dataUnit) {
        std::unique_lock<std::mutex> lock(mutex);
        while (count == BUFFER_SIZE) {
            notFull.wait(lock);
        }
        buffer[rear] = *dataUnit;
        rear = (rear + 1) % BUFFER_SIZE;
        count++;
        notEmpty.notify_one();
    }

    // 从缓冲区取出数据
    DataUnit* get() {
        std::unique_lock<std::mutex> lock(mutex);
        while (count == 0) {
            notEmpty.wait(lock);
        }
        DataUnit* dataUnit = new DataUnit(buffer[front]);
        front = (front + 1) % BUFFER_SIZE;
        count--;
        notFull.notify_one();
        return dataUnit;
    }

    // 清空缓冲区，释放资源
    void clearBuffer() {
        for (int i = 0; i < count; ++i) {
            delete[] buffer[(front + i) % BUFFER_SIZE].text;
            buffer[(front + i) % BUFFER_SIZE].text = nullptr;
        }
        front = 0;
        rear = 0;
        count = 0;
    }

private:
    DataUnit buffer[BUFFER_SIZE];
    int front;
    int rear;
    int count;
    std::mutex mutex;
    std::condition_variable notFull;
    std::condition_variable notEmpty;
};

// 生产者线程函数
void producer(Buffer* buffer, const std::string& directory) {
    try {
        // 遍历目录下的文件
        for (const auto& entry : std::filesystem::directory_iterator(directory)) {
            if (entry.is_regular_file()) {
                std::string filePath = entry.path().string();
                std::string fileName = entry.path().filename().string();
                FILE* fileStream = std::fopen(filePath.c_str(), "r");
                if (fileStream) {
                    int lineNumber = 0;
                    char line[MAX_LINE_LENGTH];
                    while (std::fgets(line, MAX_LINE_LENGTH, fileStream)) {
                        lineNumber++;
                        DataUnit* dataUnit = new DataUnit();
                        dataUnit->text = new char[strlen(line) + 1];
                        strcpy(dataUnit->text, line);
                        dataUnit->lineNumber = lineNumber;
                        strcpy(dataUnit->fileName, fileName.c_str());
                        buffer->put(dataUnit);
                    }
                    std::fclose(fileStream);
                }
            }
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Producer error: " << e.what() << std::endl;
    }
}

// 文件处理模块类
class FileProcessor {
public:
    // 单词统计函数
    static std::vector<std::pair<std::string, int>> countWords(const char* text) {
        std::vector<std::pair<std::string, int>> wordCount;
        std::string currentWord;
        for (int i = 0; text[i]; ++i) {
            if (isspace(text[i])) {
                if (!currentWord.empty()) {
                    addWordToCount(wordCount, currentWord);
                    currentWord.clear();
                }
            }
            else {
                currentWord += text[i];
            }
        }
        if (!currentWord.empty()) {
            addWordToCount(wordCount, currentWord);
        }
        return wordCount;
    }

    // 文本格式转换函数
    static std::string convertTextFormat(const char* text) {
        std::string convertedText;
        for (int i = 0; text[i]; ++i) {
            convertedText += toupper(text[i]);
        }
        return convertedText;
    }

private:
    static void addWordToCount(std::vector<std::pair<std::string, int>>& wordCount, const std::string& word) {
        for (auto& pair : wordCount) {
            if (pair.first == word) {
                pair.second++;
                return;
            }
        }
        wordCount.push_back({ word, 1 });
    }
};

// 消费者线程函数
void consumer(Buffer* buffer) {
    try {
        while (true) {
            DataUnit* dataUnit = buffer->get();
            if (!dataUnit) {
                break;
            }
            // 调用文件处理模块进行处理
            auto wordCount = FileProcessor::countWords(dataUnit->text);
            // 输出单词统计结果（可根据需求修改）
            for (const auto& pair : wordCount) {
                std::cout << "Word: " << pair.first << ", Count: " << pair.second << std::endl;
            }
            delete[] dataUnit->text;
            delete dataUnit;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Consumer error: " << e.what() << std::endl;
    }
}

// 主线程函数
int main() {
    std::string directory = "/path/to/files";
    Buffer buffer;
    // 创建生产者线程，假设创建 2 个
    std::vector<std::thread> producers;
    for (int i = 0; i < 2; ++i) {
        producers.push_back(std::thread(producer, &buffer, directory));
    }
    // 创建消费者线程，假设创建 3 个
    std::vector<std::thread> consumers;
    for (int i = 0; i < 3; ++i) {
        consumers.push_back(std::thread(consumer, &buffer));
    }

    // 等待生产者线程结束
    for (auto& producer : producers) {
        producer.join();
    }

    // 通知消费者线程结束
    DataUnit* endSignal = nullptr;
    buffer.put(endSignal);

    // 等待消费者线程结束
    for (auto& consumer : consumers) {
        consumer.join();
    }

    return 0;
}
