#include "simple-api.h"
#include "simple-common.h"
#include "fcitx-simple-module.h"
#include "fcitx-simple-ui.h"
#include "fcitx-simple-frontend.h"

static inline boolean FcitxSimpleSendRequest(FcitxInstance* instance, FcitxSimpleRequest* request, void* result) {
    int fd = FcitxSimpleGetFD(instance);
    FcitxSimpleCallQueue* queue = FcitxSimpleGetQueue(instance);
    FcitxSimpleCallQueueItem* item = FcitxSimpleCallQueueEnqueue(queue, request, result);
    if (fd < 0)
        return false;
    char c = 0;
    write(fd, &c, sizeof(c));
    sem_wait(&item->sem);
    free(item);
    return true;
}

FCITX_EXPORT_API
void FcitxSimpleInit(FcitxInstance* instance, FcitxSimpleEventHandler eventHandler, void* userData) {
    /* these function cache the result, hence we pre-cache before start */
    FcitxSimpleGetFD(instance);
    FcitxSimpleGetQueue(instance);
    FcitxSimpleFrontendInitIC(instance);
    FcitxSimpleSetEventCallback(instance, eventHandler, userData);
}

FCITX_EXPORT_API
int FcitxSimpleSendKeyEvent(FcitxInstance *instance, boolean release, FcitxKeySym key, unsigned int state, unsigned int keycode)
{
    FcitxSimpleRequest request;
    request.type = release ? SE_KeyEventRelease : SE_KeyEventPress;
    request.key = key;
    request.state = state;
    request.keycode = keycode;

    int result = 0;
    FcitxSimpleSendRequest(instance, &request, &result);

    return result;
}

FCITX_EXPORT_API
void FcitxSimpleSetCurrentIM(FcitxInstance *instance, const char *name)
{
    FcitxSimpleRequest request;
    request.type = SE_SetCurrentIM;
    request.imname = name;

    FcitxSimpleSendRequest(instance, &request, NULL);
}

FCITX_EXPORT_API
void FcitxSimpleTriggerMenuItem(FcitxInstance* instance, const char* name, int index)
{
    FcitxSimpleRequest request;
    request.type = SE_TriggerMenuItem;
    request.menu.name = name;
    request.menu.index = index;

    FcitxSimpleSendRequest(instance, &request, NULL);
}

FCITX_EXPORT_API
void FcitxSimpleTriggerStatus(FcitxInstance* instance, const char* name)
{
    FcitxSimpleRequest request;
    request.type = SE_TriggerStatus;
    request.statusName = name;

    FcitxSimpleSendRequest(instance, &request, NULL);
}

FCITX_EXPORT_API
void FcitxSimpleEnd(FcitxInstance* instance)
{
    FcitxSimpleRequest request;
    request.type = SE_End;

    FcitxSimpleSendRequest(instance, &request, NULL);
}

FCITX_EXPORT_API
int FcitxSimpleSendAndHandle(FcitxInstance *instance, boolean release, unsigned int key, 
        unsigned int state, unsigned int keycode, void* fm)
{
    FcitxSimpleRequest request;
    request.type = release ? SE_KeyEventRelease : SE_KeyEventPress;
    request.key = (FcitxKeySym )key;
    request.state = state;
    request.keycode = keycode;

#ifdef USE_QUEUE
    int result = 0;
    /*FcitxSimpleSendRequest(instance, &request, &result);*/
    FcitxSimpleCallQueue* queue = FcitxSimpleGetQueue(instance);
    FcitxSimpleCallQueueItem *item = FcitxSimpleCallQueueEnqueue(queue, &request, &result);

    /*SimpleModuleProcessEvent( (void*)fm );*/
    /*FcitxSimpleFrontendProcessKey(instance, &request);*/
    FcitxSimpleCallQueueItem *item1 = NULL;
    item1 = FcitxSimpleCallQueueDequeue(queue);
    int *res = (int*) item1->result;
    switch(item1->request->type) {
        case SE_KeyEventRelease:
        case SE_KeyEventPress:
            printf("process key--\n");
            *res = FcitxSimpleFrontendProcessKey(instance, item1->request);
            break;
    }

#else

    int result = 0;
    /*FcitxSimpleFrontendProcessKey(instance, &request);*/
    switch(request.type) {
        case SE_KeyEventRelease:
        case SE_KeyEventPress:
            printf("process key--\n");
            result = FcitxSimpleFrontendProcessKey(instance, &request);
            break;
    }
#endif
    

    return result;
}

