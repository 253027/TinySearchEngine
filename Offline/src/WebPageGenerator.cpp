#include "../include/WebPageGenerator.h"
#include "../include/utility.h"

WebPageGenerator::WebPageGenerator(const std::string &filepath)
    : config(new Configuration(filepath))
{
    _dirpath = config->getFilePath("parseDirectoryPath");
    _dicPath = config->getFilePath("storedFilePath");
    _indexDicPath = config->getFilePath("indexStoreFilePath");
}

void WebPageGenerator::parse()
{
    using namespace tinyxml2;
    long long index = 0, sum = 0;

    // 创建网页库文件目录
    std::ofstream dicout, indexdicout;
    dicout.open(_dicPath, std::ios::trunc | std::ios::out);
    ERROR_CHECK(dicout.is_open() == false, "create %s failed", _dicPath.c_str());
    indexdicout.open(_indexDicPath, std::ios::trunc | std::ios::out);
    ERROR_CHECK(indexdicout.is_open() == false, "create %s failed", _indexDicPath.c_str());

    std::function<void(const std::string &)> dfs = [&](const std::string &filepath) -> void
    {
        DIR *dir = ::opendir(filepath.c_str());
        ERROR_CHECK(dir == NULL, "open directory failed. %s\n", errno);
        for (struct dirent *t = ::readdir(dir); t; t = readdir(dir))
        {
            if (filepath == "." || filepath == "..")
                continue;
            if (t->d_type == ::DT_DIR)
            {
                dfs(t->d_name);
                continue;
            }
            std::string filename = filepath + "/" + t->d_name;

            XMLDocument doc;
            doc.LoadFile(filename.c_str());
            ERROR_CHECK(doc.Error() == true, "parse file %s falied.%s\n", filename, doc.ErrorStr());

            XMLElement *root = doc.FirstChildElement("rss")->FirstChildElement("channel")->FirstChildElement("item");
            for (; root; root = root->NextSiblingElement("item"))
            {
                tinyxml2::XMLDocument res;
                XMLElement *title = root->FirstChildElement("title");
                XMLElement *link = root->FirstChildElement("link");
                XMLElement *description = root->FirstChildElement("description");
                // 设置根元素
                XMLElement *res_root = res.NewElement("doc");
                res.InsertEndChild(res_root);
                // 设置id子元素
                XMLElement *id = res.NewElement("id");
                std::string id_len = std::to_string(index);
                id->SetText(id_len.c_str());
                // 设置url子元素
                XMLElement *url = res.NewElement("url");
                url->SetText(link->GetText());
                // 设置title子元素
                XMLElement *res_title = res.NewElement("title");
                res_title->SetText(title->GetText());
                // 设置content子元素
                XMLElement *content = res.NewElement("content");
                std::regex tags("<[^>]*>");
                std::string cdata = description->GetText();
                cdata = std::regex_replace(cdata, tags, "");
                cdata = std::regex_replace(cdata, std::regex("[\\n\\r\\t]+"), ""); // 移除换行符、回车符和制表符
                cdata = std::regex_replace(cdata, std::regex(" +"), "");           // 移除额外的空格

                // // 去除空格
                // std::string line;
                // for (int i = 0; i < cdata.size(); i++)
                // {
                //     if (cdata[i] == -29 || cdata[i] == -128)
                //         continue;
                //     line += cdata[i];
                // }
                content->SetText(cdata.c_str());

                res_root->InsertEndChild(id);
                res_root->InsertEndChild(url);
                res_root->InsertEndChild(res_title);
                res_root->InsertEndChild(content);
                res.InsertEndChild(res_root);
                index++;

                // 创建一个 XMLPrinter 对象
                tinyxml2::XMLPrinter printer;
                // 将文档打印到 printer 对象，其中第一个参数为 true 表示格式化输出
                res.Print(&printer);
                // 将格式化后的字符串输出到标准输出
                // dicout
                dicout << printer.CStr() << "\n";
                long long size = strlen(printer.CStr()) + 1;
                indexdicout << (index - 1) << " " << sum << " " << size << std::endl;
                sum += size;
            }
        }
        closedir(dir);
    };
    dfs(_dirpath);
    dicout.close();
    indexdicout.close();
}

void WebPageGenerator::removeDuplicates()
{
    using PIS = std::pair<uint64_t, std::string>;
    using PSI = std::pair<std::string, double>;
    std::vector<PIS> hash;
    std::vector<PSI> top_word;
    std::ifstream in;
    std::ifstream dictionary;
    simhash::Simhasher work("./include/simhash/dict/jieba.dict.utf8",
                            "./include/simhash/dict/hmm_model.utf8",
                            "./include/simhash/dict/idf.utf8",
                            "./include/simhash/dict/stop_words.utf8");
    in.open(_indexDicPath, std::ios::in);
    ERROR_CHECK(in.is_open() == false, "open %s falied", _indexDicPath.c_str());
    dictionary.open(_dicPath, std::ios::in);
    ERROR_CHECK(dictionary.is_open() == false, "open %s failed", _dicPath.c_str());
    for (std::string line; std::getline(in, line);)
    {
        std::istringstream is(line);
        long long pos, len, id;
        is >> id >> pos >> len;
        dictionary.seekg(pos);
        std::string buf(len, '\0');
        dictionary.read(buf.data(), len);
        // work.extract(buf, top_word, 10ULL);
        uint64_t p = 0;
        work.make(buf, 10ULL, p);
        bool ok = true;
        for (int i = 0; i < hash.size(); i++)
        {
            if (!simhash::Simhasher::isEqual(hash[i].first, p, 3))
                continue;
            ok = false;
            break;
        }
        if (ok)
            hash.push_back(std::make_pair(p, buf));
    }

    using namespace tinyxml2;
    dictionary.close();

    int last_loc = std::find(_dicPath.rbegin(), _dicPath.rend(), '/') - _dicPath.rbegin();
    std::string copy_filename = _dicPath.substr(0, _dicPath.size() - last_loc) + "temp.dat";
    std::ofstream copy;
    copy.open(copy_filename, std::ios::out | std::ios::trunc);
    ERROR_CHECK(copy.is_open() == false, "create backupfile failed");

    std::ofstream dicout;
    dicout.open(_indexDicPath, std::ios::trunc | std::ios::out);
    ERROR_CHECK(dicout.is_open() == false, "crete backupfile index failed");

    long long pos = 0;
    for (int i = 0; i < hash.size(); i++)
    {
        XMLDocument doc;
        doc.Parse(hash[i].second.c_str());
        XMLElement *root = doc.FirstChildElement("doc")->FirstChildElement("id");
        root->SetText(std::to_string(i).c_str());
        // 创建一个 XMLPrinter 对象
        tinyxml2::XMLPrinter printer;
        // 将文档打印到 printer 对象，其中第一个参数为 true 表示格式化输出
        doc.Print(&printer);
        // 将格式化后的字符串输出到标准输出
        copy << printer.CStr() << "\n";
        int len = strlen(printer.CStr()) + 1;
        dicout << i << " " << pos << " " << len << "\n";
        pos += len;
    }
    copy.close();
    dicout.close();

    ::unlink(_dicPath.c_str());
    ERROR_CHECK(::rename(copy_filename.c_str(), _dicPath.c_str()) != 0, "rename ripepage.dat failed.");
}

void WebPageGenerator::createInvertIndex()
{
    // 网页库
    std::ifstream dictionary;
    dictionary.open(_dicPath, std::ios::in);
    ERROR_CHECK(dictionary.is_open() == false, "open % failed\n", _dicPath.c_str());
    // 网页偏移库
    std::ifstream index_dictionary;
    index_dictionary.open(STOP_WORD_PATH, std::ios::in);
    ERROR_CHECK(index_dictionary.is_open() == false, "open %s failed\n", STOP_WORD_PATH);
    // 倒排索引库
    int last_loc = std::find(_dicPath.rbegin(), _dicPath.rend(), '/') - _dicPath.rbegin();
    std::string invert_index_filepath = _dicPath.substr(0, _dicPath.size() - last_loc) + "inverted_index.dat";
    std::ofstream invert_index;
    invert_index.open(invert_index_filepath, std::ios::out | std::ios::trunc);
    ERROR_CHECK(invert_index.is_open() == false, "open %s failed\n", invert_index_filepath.c_str());

    last_loc = 0;
    std::unordered_map<std::string, int> total;
    // 每篇文档的词频
    std::unordered_map<int, std::unordered_map<std::string, int>> current;

    // 加载停用词数据结构
    std::unordered_set<std::string> stop_words;
    for (std::string line; std::getline(index_dictionary, line);)
        stop_words.insert(line);
    stop_words.insert(std::string(1, '\n'));
    index_dictionary.close();
    // 加载分词文件
    cppjieba::Jieba jieba(DICT_PATH, HMM_PATH, USER_DICT_PATH, IDF_PATH, STOP_WORD_PATH);

    index_dictionary.open(_indexDicPath, std::ios::in);
    ERROR_CHECK(index_dictionary.is_open() == false, "open %s failed", _indexDicPath.c_str());

    // 先统计所有单词
    for (std::string line; std::getline(index_dictionary, line);)
    {
        std::istringstream is(line);
        long long a, b, c;
        is >> a >> b >> c;
        std::string buf;
        buf.resize(c, '\0');
        dictionary.seekg(b);
        dictionary.read(buf.data(), c);
        std::vector<std::string> words; // 分词结果存储位置
        jieba.Cut(buf, words, true);
        int i = -1;
        for (int j = 0; j < words.size(); j++)
        {
            if (stop_words.count(words[j]))
                continue;
            words[++i] = words[j];
        }
        words.resize(i + 1);
        for (int j = 0; j < words.size(); j++)
        {
            ++total[words[j]];
            ++current[a][words[j]];
        }
        ++last_loc; // 统计文章总行数
    }

    std::unordered_map<std::string, std::set<std::pair<int, double>>> res;

    for (auto &x : current)
    {
        double sum = 0;
        std::vector<double> weight;
        auto &id = x.first;
        auto &content = x.second;
        for (auto &t : content)
        {
            double val = t.second * idf(last_loc, total[t.first]);
            sum += val * val, weight.push_back(val);
        }
        int index = 0;
        sum = sqrt(sum);
        for (auto &x : content)
            res[x.first].insert(std::make_pair(id, weight[index++] / sum));
    }

    for (auto &x : res)
    {
        invert_index << x.first;
        for (auto &range : x.second)
            invert_index << " " << range.first << " " << range.second;
        invert_index << "\n";
    }

    dictionary.close();
    index_dictionary.close();
    invert_index.close();
}

double WebPageGenerator::idf(int page_nums, int df_frequence)
{
    return ::log2((double)page_nums / (df_frequence + 1));
}
