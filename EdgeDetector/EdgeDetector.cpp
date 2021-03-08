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

    Gaussian gaussian(1);

    // 遍历目录下所有文件
    for (const auto& entry : dirIt) {
        std::string ext = entry.path().extension().string();
        if (ext == PNG_EXT) { // 只处理 png 文件
            std::string filename = entry.path().string();

            if (filename.find("test.png") != std::string::npos) {   // 跳过测试图片，REMOVE ON RELEASE!
                continue; 
            }
            std::cout << "正在处理：" << filename << "\n";

            int w, h;
            int compsPerPixel;

            Image* image;

            try {
                Image* image = new Image(filename.c_str());
                std::cout << "成功载入图片\n";

                std::cout << image->width << " " << image->height << " "
                    << image->bytesPerPixel << " " << image->bytesPerRow << "\n";

                // save a small part of the picture
                // image = image->subImage(image->height / 10, image->width / 10, image->height / 2, image->width / 2);

                // perform edge detection on image

                BWImage* bw = new BWImage(image);
                image = bwToColor(bw);

                image->save(outputDir + "\\test.png");


                /*
                gaussian.setImage(bw);

                BWImage* filtered = gaussian.execute();

                image = bwToColor(filtered);
                image->save(outputDir + "\\filtered_test.png");*/


                Canny canny(1);
                //BWImage* nbw = new BWImage(image);
                canny.setImage(bw);
                BWImage* cool = canny.execute();

                image = bwToColor(cool);
                image->save(outputDir + "\\NEWfiltered_test.png");

            } catch(std::string s) {
                std::cout << s << "\n";
            }
        }
    }
}


int main() {
    std::cout << "CPP version: " << __cplusplus << "\n";

    // 请求用户输入：
    // - 存放图片的目录
    // - 输出图片的目标目录 
    std::string inputDir, outputDir;
    std::cout << "注：输入路径，略去引号\n";
    std::cout << "请输入存放图片的目录：\n";
    std::getline(std::cin, inputDir);
    std::cout << "请输入结果的目标目录：\n";
    std::getline(std::cin, outputDir);

    try {
        processImages(inputDir, outputDir);
        std::cout << "所有 PNG 处理完毕.\n";
    }
    catch (std::string s) {
        std::cout << s << "\n";
    }
    
    std::cout << "结束。按任意键退出……\n";
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
