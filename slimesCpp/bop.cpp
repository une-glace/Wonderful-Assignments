#include "actor.h"
#include "battlefield.h"
#include <easy2d/easy2d.h>

using namespace easy2d;


int main() {
    HumanActor a;
    //SunnyActor b; // 晴天队AI
    RainyActor b;   // 雨天队AI
    BattleField field(100);
    field.Reset();
    field.SetActor(&a, &b); 
    field.Run();
    return 0;
}
/*#include <iostream>
#include <fstream>
#include <filesystem>

int main() {
    std::ifstream file("res\\Background.png");
    if (file) {
        std::cout << "文件存在，并且可以被访问。" << std::endl;
    }
    else {
        std::cout << "文件不存在，或者无法访问。" << std::endl;
        std::cout << "当前工作目录: " << std::filesystem::current_path() << std::endl;
    }
    return 0;
}*/