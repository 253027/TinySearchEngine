#include <workflow/WFTaskFactory.h>
#include <workflow/WFHttpServer.h>
#include <workflow/WFFacilities.h>
#include <workflow/HttpUtil.h>
#include <workflow/HttpMessage.h>
#include <workflow/WFConnection.h>
#include <workflow/Workflow.h>
#include <signal.h>
#include <iostream>
#include <string>
#include "./include/utility.h"
#include "./include/PrivatePtotocal.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <nlohmann/json.hpp>

static WFFacilities::WaitGroup wait_group(1);
using PrivateRequest = PrivateProtocal;
using PrivateResponse = PrivateProtocal;
using PrivateTask = WFNetworkTask<PrivateRequest, PrivateResponse>;

void singal_handel(int sig)
{
    if (sig != SIGINT)
        return;
    wait_group.done();
}

void PrivateProtocalCallback(PrivateTask *task)
{
    int state = task->get_state();
    int error = task->get_error();

    PrivateProtocal *resp = task->get_resp();
    void *body;
    size_t size;
    resp->getMessageBody(&body, &size);

    if (state == WFT_STATE_SUCCESS)
    {
        WFHttpTask *http_task = (WFHttpTask *)task->user_data;
        nlohmann::ordered_json js;
        js["id"] = std::to_string(*(int *)body);
        js["content"] = std::string((char *)body + 4, size - 4);
        std::string res = js.dump();
        std::cout << res << "\n";
        http_task->get_resp()->append_output_body(res.data(), res.size());
    }
    else
    {
        const char *str = WFGlobal::get_error_string(state, error);
        fprintf(stderr, "Error: %s\n", str);
    }
}

void process(WFHttpTask *task)
{
    protocol::HttpRequest *resquest = task->get_req();
    size_t size;
    const void *body;
    resquest->get_parsed_body(&body, &size);
    std::string message((char *)body, size);

    nlohmann::json js = nlohmann::json::parse(message);
    std::string id, content;
    id = js["id"], content = js["content"];
    char buf[(4 + content.size())];

    *(int *)buf = std::stoi(id);
    ::memcpy(buf + 4, content.data(), content.size());

    using NTF = WFNetworkTaskFactory<PrivateRequest, PrivateResponse>;

    PrivateTask *tcp_task = NTF::create_client_task(TT_TCP, "tcp://1.94.134.185:9190", 0, PrivateProtocalCallback);
    // PrivateTask *tcp_task = NTF::create_client_task(TT_TCP, "tcp://127.0.0.1:9191", 0, PrivateProtocalCallback);
    tcp_task->get_req()->setMessageBody(buf, sizeof(buf));
    tcp_task->user_data = task;

    *series_of(task) << tcp_task;
}

int main()
{
    //     pid_t pid = fork();
    //     if (pid < 0)
    //         exit(EXIT_FAILURE);
    //     if (pid > 0)
    //         exit(EXIT_SUCCESS);
    //     if (setsid() < 0)
    //         exit(EXIT_FAILURE);
    //     for (long x = sysconf(_SC_OPEN_MAX); x >= 0; x--)
    //         ::close(x);
    //     ::open("/dev/null", O_RDWR);
    //     dup(0);
    //     dup(0);

    signal(SIGINT, singal_handel);
    WFHttpServer server(&process);
    server.start(9190);
    wait_group.wait();
    return 0;
}