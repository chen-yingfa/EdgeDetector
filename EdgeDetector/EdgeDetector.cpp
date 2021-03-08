// EdgeDetector.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <cstdio>
#include <memory>
#include <string>
#include <vector>
#include <thread>
#include <cassert>
#include <filesystem>

#include "globals.h"
#include "Gaussian.h"
#include "Canny.h"
#include "image.h"

using uchar = unsigned char;

namespace fs = std::filesystem;

Image* gaussian(Image* img) {
    return nullptr;
}

Image* canny(Image* img) {
    return nullptr;
}

void processImages(std::string inputDir, std::string outputDir) {
    std::cout << "\nInput folder: " << inputDir << "\n";
    std::cout << "Output folder: " << outputDir << "\n\n";

    fs::directory_entry dirEntry(inputDir);

    if (!dirEntry.exists()) {
        throw(std::string("Directory does not exist"));
    }

    if (!dirEntry.is_directory()) {
        throw(std::string("Path is not a directory"));
    }

    fs::directory_iterator dirIt(inputDir);

    Gaussian gaussian(20);
    Canny canny(20);

    // 遍历目录下所有文件
    int id = 0;
    for (const auto& entry : dirIt) {
        std::string ext = entry.path().extension().string();
        if (ext == PNG_EXT) { // 只处理 png 文件
            std::string filename = entry.path().filename().string();
            std::string path = entry.path().string();

            if (filename.find("test.png") != std::string::npos) {   // 跳过测试图片，REMOVE ON RELEASE!
                // continue; 
            }
            std::cout << "正在处理：" << filename << "\n";

            int w, h;
            int compsPerPixel;

            Image* image;

            try {
                Image* image = new Image(path.c_str());
                printf("成功载入图片（%dx%d）\n", image->width, image->height);

                // 执行边缘检测

                BWImage* grayscale = new BWImage(image);
                // grayscale->save(outputDir + "\\" + std::to_string(id) + "grayscale.png");
                BWImage* filtered = gaussian.process(grayscale);
                // filtered->save(outputDir + "\\" + std::to_string(id) + "filtered.png");
                BWImage* result = canny.process(filtered);

                
                result->save(outputDir + "\\" + filename);
                id++;

            } catch(std::string s) {
                std::cout << s << "\n";
            }
        }
    }
}


void promptDir() {
    // 请求用户输入：
    // - 存放图片的目录
    // - 输出图片的目标目录 
    std::string inputDir, outputDir;
    std::cout << "注：请输入绝对路径，略去引号\n";
    std::cout << "请输入存放图片的目录：\n";
    std::getline(std::cin, inputDir);
    std::cout << "结果存放到哪个目录？\n";
    std::getline(std::cin, outputDir);

    if (inputDir.size() < 2 || outputDir.size() < 2) {
        std::cout << "路径不合法\n";
        return;
    }

    if (inputDir == outputDir) {
        std::cout << "输入和输出目录不能相同\n";
        return;
    }

    try {
        processImages(inputDir, outputDir);
        std::cout << "所有 PNG 处理完毕.\n";
    }
    catch (std::string s) {
        std::cout << s << "\n";
    }
}

int mainMenu() {
    std::cout << "CPP version: " << __cplusplus << "\n";
    std::string cmd;
    while (true) {
        // 请求用户输入：
        // - 存放图片的目录
        // - 输出图片的目标目录
        std::cout << "（请输入对应的数字，无空格）\n";
        std::cout << "1: 对给定目录下的所有 PNG 进行编原检测\n";
        std::cout << "2: 退出\n";
        std::cout << ">>> ";
        std::getline(std::cin, cmd);
        
        if (cmd == "1") {
            promptDir();
            std::cout << "\n";
        }
        else if (cmd == "2") {
            break;
        }
    }
    return 0;
}

int main() {
    mainMenu();
    return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门使用技巧: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
