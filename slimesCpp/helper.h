#ifndef HELPER_H
#define HELPER_H

#include <map>
#include <random>
#include <iostream>
#include <algorithm>


// 类似Python的print函数，可以填入任意类型的值，两个参数之间会自动加空格
// 例如：LOG(Whose(actor->GetName()), pet.petName + ": HP", pet.health); LOG("\n");
// 效果为： Player's L: HP 100\n
// LOG为标准输出，ERR会输出错误并终止程序
// 也可以配合使用std::stringstream，作为PRINT的第一个参数
// 例如：std::stringstream ss; PRINT(ss, Whose(actor->GetName()), pet.petName + ": HP", pet.health); PRINT(ss, "\n");
// 使用ss.str()得到 "Player's L: HP 100\n"
template<typename T, typename... Args>
std::ostream& PRINT(std::ostream& out, const T& t) {
    return out << t;
}

template<typename T, typename... Args>
std::ostream& PRINT(std::ostream& out, const T& t, const Args&... rest) {
    out << t << ' ';
    return PRINT(out, rest...);
}

#define LOG(...) PRINT(std::cout, __VA_ARGS__)
#define ERR(...) do {   \
    PRINT(std::cerr, __VA_ARGS__);   \
    exit(-1);   \
} while (false)


// 生成[low, high)（左闭右开）的均匀分布随机数，只能填入int, unsigned等整数类型
template <typename T>
T RandomWithRange(T low, T high) {  // [low, high)
    std::random_device dev;
    std::uniform_int_distribution<T> u(low, high - 1);
    std::default_random_engine e(dev());
    return u(e);
}

// 随机打乱std::vector的元素排列
template <typename T>
void ShuffleVector(std::vector<T>& vec) {
    std::random_device dev;
    std::default_random_engine e(dev());
    std::shuffle(vec.begin(), vec.end(), e);
}

// 给定概率，返回该概率下事件是否发生
// 例如：某个AI决定以40%的概率选择小绿，否则选择小蓝
// 代码实现：RandomWithProbability(.4) ? Pet::G : Pet::B
bool RandomWithProbability(double prob);


// 重载了std::map，当访问不存在的Key时会返回给定的默认值
// 例如：DefaultedMap<std::pair<Weather_T, Type_T>, int, 100> weatherRatio;
// 则 weatherRatio[{Weather::Normal, Type::Fire}] 会返回默认值100(%)，表示普通天气火属性技能伤害威力不变
// 注意：只有[]运算符保证返回的是默认值
template <typename K, typename V, V D>
class DefaultedMap : public std::map<K, V> {
public:
    DefaultedMap() = default;
    DefaultedMap(std::initializer_list<std::pair<const K, V>> init) : std::map<K, V>(init) {}
    V& operator[](const K& key) {
        auto iter = this->find(key);
        if (iter == this->end()) {
            return (std::map<K, V>::operator[](key) = D);
        }
        else {
            return iter->second;
        }
    }
};

#endif //HELPER_H
