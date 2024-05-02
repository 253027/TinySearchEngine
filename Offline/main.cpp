#include "./include/DictionaryGenerator.h"

int main()
{
    DictionaryGenerator generator("/home/aa/桌面/code/Search_Engine/origin/stop_words_eng.txt");
    generator.parseEnglish("/home/aa/桌面/code/Search_Engine/Offline/test");
    return 0;
}