#ifndef HEAD_HOOK_PROTOCOL
#define HEAD_HOOK_PROTOCOL

struct URLProtocol;
typedef struct HookProtocolCallBack 
{
    void(*on_write_begin)(void *opeque, const unsigned char *buf, int *req_write_size);
    void(*on_write_end)(void *opeque, int req_write_size, int writ_size);

    void(*on_read_begin)(void *opeque, int *req_read_size);
    void(*on_read_end)(void *opeque, const unsigned char *buf, int req_read_size, int *read_size);

    void* opeque;
}HookProtocolCallBack;

enum PROTOCOL_TYPE
{
    PROTOCOL_BEGIN = -1,

    PROTOCOL_RTMP = 0,
    PROTOCOL_RTMPT = 1,

    PROTOCOL_TCP = 2,
    PROTOCOL_UDP = 3,

    PROTOCOL_HTTP = 4,
    PROTOCOL_HTTPPROXY = 5,

    PROTOCOL_FILE = 6,
    PROTOCOL_FTP = 7,

    PROTOCOL_END,
};

void unregister_hook_protocol(void);
int  register_hook_protocol_by_type(enum PROTOCOL_TYPE hook_protocol_type, HookProtocolCallBack* call_back);

#endif