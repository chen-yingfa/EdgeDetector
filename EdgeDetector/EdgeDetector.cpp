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
#include "image.h"

using Img = std::vector<std::vector<float> >;
using uchar = unsigned char;

namespace fs = std::filesystem;

void gaussian(Img& img) {
    
}

void processImages(std::string inputDir, std::string outputDir) {
    std::cout << "\nInput folder: " << inputDir << "\n";
    std::cout << "Output folder: " << outputDir << "\n\n";

    fs::directory_entry dirEntry(inputDir);

    assert(dirEntry.exists() && dirEntry.is_directory());

    fs::directory_iterator dirIt(inputDir);

    for (const auto& entry : dirIt) {
        std::string ext = entry.path().extension().string();
        if (ext == PNG_EXT) { // 只处理 png 文件
            std::string filename = entry.path().string();
            if (filename == "test.png") continue;

            std::cout << "Processing " << filename << "\n";

            int w, h;
            int compsPerPixel;

            Image* image;

            try {
                Image* image = new Image(filename.c_str());
                std::cout << "Loaded successfully\n";

                std::cout << image->width << " " << image->height << " "
                    << image->bytesPerPixel << " " << image->bytesPerRow << "\n";
                
                for (int i = 0; i < 10; ++i) {
                    std::cout << image->getColor(i, i).str() << "\n";
                }

                printf("%p\n", image->data);
                   
                image = image->subImage(100, 100, 400, 400);
                printf("%p\n", image->data);

                for (int i = 0; i < 10; ++i) {
                    std::cout << image->getColor(i, 20).str() << "\n";
                }
                image->save(outputDir + "\\test.png");

            } catch(std::string s) {
                std::cout << s << "\n";
            }
        }
    }
}


int main() {
    std::cout << __cplusplus << "\n";
    std::string inputDir, outputDir;
    std::cout << "Input folder path\n";
    std::getline(std::cin, inputDir);
    std::cout << "Output folder path\n";
    std::getline(std::cin, outputDir);
    processImages(inputDir, outputDir);
    
    std::cout << "Done processing all PNG files. Press ENTER to continue.\n";
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
