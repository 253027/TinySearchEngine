#include <workflow/WFTaskFactory.h>
#include <workflow/WFHttpServer.h>
#include <workflow/WFFacilities.h>
#include <workflow/HttpUtil.h>
#include <workflow/HttpMessage.h>
#include <workflow/WFConnection.h>
#include <workflow/Workflow.h>
#include <workflow/TLVMessage.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <iostream>
#include <string>
#include <fcntl.h>
#include "./include/utility.h"
#include "./include/json.hpp"
#include <spdlog/spdlog.h>

static WFFacilities::WaitGroup wait_group(1);

using WFTLVServer = WFServer<protocol::TLVRequest, protocol::TLVResponse>;
using WFTLVTask = WFNetworkTask<protocol::TLVRequest, protocol::TLVResponse>;
using tlv_callback_t = std::function<void(WFTLVTask *)>;

WFTLVTask *create_tlv_task(const char *host, unsigned short port, tlv_callback_t callback)
{
    auto *task = WFNetworkTaskFactory<protocol::TLVRequest, protocol::TLVResponse>::create_client_task(TT_TCP, host, port, 0, std::move(callback));
    task->set_keep_alive(60 * 1000);
    return task;
}

void singal_handel(int sig)
{
    if (sig != SIGINT)
        return;
    wait_group.done();
}

void PrivateProtocalCallback(WFTLVTask *task)
{
    int state = task->get_state();
    int error = task->get_error();

    protocol::TLVRequest *resp = task->get_resp();

    std::string *value = resp->get_value();
    spdlog::info("回调函数执行 类型 {0:d}", resp->get_type());

    if (state == WFT_STATE_SUCCESS)
    {
        WFHttpTask *http_task = (WFHttpTask *)task->user_data;
        nlohmann::ordered_json json;
        json["id"] = std::to_string(resp->get_type());
        json["content"] = *resp->get_value();
        http_task->get_resp()->append_output_body(json.dump().data(), json.dump().size());
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

    spdlog::info("content {0:d}", content.size());

    // PrivateTask *tcp_task = NTF::create_client_task(TT_TCP, "tcp://1.94.134.185:9190", 1, PrivateProtocalCallback);
    WFTLVTask *tcp_task = create_tlv_task("127.0.0.1", 9191, PrivateProtocalCallback);
    spdlog::info("任务创建成功 {0:d}", content.size());

    tcp_task->get_req()->set_value(content); // 14
    if (id == "1")
        tcp_task->get_req()->set_type(1);
    else if (id == "2")
        tcp_task->get_req()->set_type(2);
    else
        tcp_task->get_req()->set_type(404);

    tcp_task->user_data = task;

    *series_of(task) << tcp_task;
}

int main()
{
    // pid_t pid = fork();
    // if (pid < 0)
    //     exit(EXIT_FAILURE);
    // if (pid > 0)
    //     exit(EXIT_SUCCESS);
    // if (setsid() < 0)
    //     exit(EXIT_FAILURE);
    // for (long x = sysconf(_SC_OPEN_MAX); x >= 0; x--)
    //     ::close(x);
    // ::open("/dev/null", O_RDWR);
    // dup(0);
    // dup(0);

    struct WFServerParams params = HTTP_SERVER_PARAMS_DEFAULT;
    /* for safety, limit request size to 8MB. */
    params.request_size_limit = 8 * 1024 * 1024;
    // params.peer_response_timeout = 100;

    signal(SIGINT, singal_handel);
    WFHttpServer server(&params, &process);
    server.start(9190);
    wait_group.wait();
    return 0;
}