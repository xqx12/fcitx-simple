/***************************************************************************
 *   Copyright (C) 2012~2013 by CSSlayer                                   *
 *   wengxt@gmail.com                                                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin St, Fifth Floor, Boston, MA 02110-1301, USA.              *
 ***************************************************************************/

#include <locale.h>
#include <libintl.h>
#include <getopt.h>
#include <unistd.h>

#include <fcitx/instance.h>
#include "instance-internal.h"
#include <fcitx/hook.h>
#include "simple-api.h"
#include "hotkey-list.h"

#include <fcitx-utils/memory.h>

FcitxInstance* instance = NULL;
typedef struct _KEYSTR {
    unsigned int key;
    unsigned int status;
}KEYSTR,*pKEYSTR;

static void usage() {
    fprintf(stderr,
            "Usage: fcitx-testbed [OPTION] <addonname> [inputfile]\n"
            "\t-d <dir>\tsandbox directorys\n"
            "\t-i <imname>\tim name to use\n"
            "\t-h\t\tdisplay this help and exit\n"
            "<addonname> is a comma separated list\n"
            "if inputfile is not specified, it will read stdin as input\n");
    /*FcitxMemoryPool *pool = fcitx_memory_pool_create();*/
}

static void TestbedCallback(void* arg, FcitxSimpleEvent* event) {
#define TESTBED_CASE(NAME) \
    case SSE_##NAME: \
        fprintf(stderr, "UI:" #NAME "\n");
    FcitxInputState* input = FcitxInstanceGetInputState(instance);
    switch (event->type) {
        TESTBED_CASE(ShowInputWindow) {
            char* candidateword = FcitxUICandidateWordToCString(instance);
            char* str = FcitxInstanceProcessOutputFilter(instance, candidateword);
            if (str) {
                free(candidateword);
                candidateword = str;
            }
            fprintf(stderr, "CANDIDATE:%s\n", candidateword);
            free(candidateword);
            break;
        }
        TESTBED_CASE(CloseInputWindow)
            break;
        TESTBED_CASE(RegisterComplexStatus)
            break;
        TESTBED_CASE(RegisterStatus)
            break;
        TESTBED_CASE(UpdateComplexStatus)
            break;
        TESTBED_CASE(UpdateStatus)
            break;
        TESTBED_CASE(RegisterMenu)
            break;
        TESTBED_CASE(ShowMenu)
            break;
        TESTBED_CASE(CommitString){
            fprintf(stderr, "COMMIT:%s\n", event->commitString);
            break;
        }
        TESTBED_CASE(UpdatePreedit) {
            char* clientPreedit = FcitxUIMessagesToCString(FcitxInputStateGetClientPreedit(input));
            char* str = FcitxInstanceProcessOutputFilter(instance, clientPreedit);
            if (str) {
                free(clientPreedit);
                clientPreedit = str;
            }
            fprintf(stderr, "PREEDIT:%s\n", clientPreedit);
            free(clientPreedit);
            break;
        }
    }

#undef TESTBED_CASE
}

int ParseKeyStr(pKEYSTR keystr)
{
    if( !instance ) return -1;

    if( !keystr ) return -1;
    /*FcitxKeySym sym = FcitxKey_None;*/
    /*unsigned int state = 0;*/
    /*FcitxHotkeyParseKey(buf, &sym, &state);*/

    FcitxAddon **pmodule;
    for( pmodule = (FcitxAddon**) utarray_front(&(instance->eventmodules));
            pmodule != NULL; 
            pmodule = (FcitxAddon**) utarray_next(&instance->eventmodules, pmodule)) {
        /*printf("%x\n", (*pmodule)->addonInstance);*/
        printf("key is %x: %x\n", keystr->key, keystr->status);

        FcitxSimpleSendAndHandle(instance, false, (keystr->key), keystr->status, 0, (*pmodule)->addonInstance);
    }

}
int ParseKey(char* buf)
{
    if( !instance ) return -1;

    FcitxKeySym sym = FcitxKey_None;
    unsigned int state = 0;
    FcitxHotkeyParseKey(buf, &sym, &state);

    FcitxAddon **pmodule;
    for( pmodule = (FcitxAddon**) utarray_front(&(instance->eventmodules));
            pmodule != NULL; 
            pmodule = (FcitxAddon**) utarray_next(&instance->eventmodules, pmodule)) {
        /*printf("%x\n", (*pmodule)->addonInstance);*/
        printf("key is %s: %d\n", buf, sym);

        FcitxSimpleSendAndHandle(instance, false, sym, state, 0, (*pmodule)->addonInstance);
    }

}

char* getKeyStringbyInt(int index)
{
    if( index > 0x20 && index <= 0x7e) {
        char *p;
        p = malloc(sizeof(char)*2);
        p[0] = index;
        p[1] = '\0';
        return p;
    }
    int i = 0;
    while(1) {
        if(!xkeyList[i].code)
            break;

        if(xkeyList[i].code == index)
            return strdup(xkeyList[i].strKey);

        i++;
    }
    return NULL;
}

int SetCurrentIM(char *imname)
{
	if( !instance ) {
		printf("SetCurrentIM failed\n");
		return -1;
	}
	FcitxProfile* profile = FcitxInstanceGetProfile(instance);
	if( profile->imList) {
		free(profile->imList);
	}

	asprintf(&profile->imList, "%s:True", imname);
	FcitxInstanceUpdateIMList(instance);
	FcitxInstanceSwitchIMByName(instance, imname);
}

int main(int argc, char* argv[])
{
    char* localedir = fcitx_utils_get_fcitx_path("localedir");
    setlocale(LC_ALL, "");
    bindtextdomain("fcitx", localedir);
    free(localedir);
    bind_textdomain_codeset("fcitx", "UTF-8");
    textdomain("fcitx");

    FcitxLogSetLevel(FCITX_DEBUG);
    int c;
    char* addonList = NULL, *sandboxDirectory = NULL;
    char *buf = NULL, *buf1 = NULL;
    FILE* fp = NULL;
    char* enableAddon = NULL;
    char* imname = NULL;
    int ret = 1;
    int fd = -1;
    while ((c = getopt(argc, argv, "d:i:h")) != EOF) {
        switch (c) {
            case 'i':
                imname = strdup(optarg);
                break;
            case 'd':
                sandboxDirectory = strdup(optarg);
                break;
            case 'h':
            default:
                goto option_error_end;
        }
    }

    if(imname==NULL)
        imname = strdup("pinyin");

    /* processs [addon list] */
    /*if (optind >= argc)*/
        /*goto option_error_end;*/
    /*addonList = strdup(argv[optind]);*/
    addonList = strdup("fcitx-pinyin");

    /* script file */
    if (optind  < argc) {
        fp = fopen(argv[optind ], "rt");
    }
    else {
        fp = stdin;
    }
    if (!fp) {
        goto option_error_end;
    }

    if (!addonList) {
        goto option_error_end;
    }

    sem_t sem;
    sem_init(&sem, 0, 0);
    asprintf(&enableAddon, "fcitx-simple-module,fcitx-simple-frontend,%s", addonList);

    /* reset optind, since FcitxInstanceCreatePause will use getopt again */
    optind = 1;

    char* args[] = {
        argv[0],
        "-D",
        "--disable",
        "all",
        "--enable",
        enableAddon,
        "--ui",
        "fcitx-simple-ui"
    };

    char temp[] = "/tmp/fcitx_sandbox_XXXXXX";
    if (sandboxDirectory) {
        setenv("XDG_CONFIG_HOME", sandboxDirectory, 1);
    } else {
        /*
         * we make a file on purpose, since XDG_CONFIG_HOME should be a directory
         * hence this will prevent every write operation under XDG_CONFIG_HOME
         */
        fd = mkstemp(temp);
        if (fd == -1) {
            setenv("XDG_CONFIG_HOME", "/", 1);
        }
        else {
            close(fd);
            setenv("XDG_CONFIG_HOME", temp, 1);
        }
    }

    instance = FcitxInstanceCreatePauseSingleThread(&sem, 8, args, -1);
    if (sem_trywait(&sem) == 0)
        goto option_error_end;

    FcitxSimpleInit(instance, TestbedCallback, NULL);
    FcitxInstanceStart(instance);
    size_t len = 0;

    if (imname) {
        /*FcitxSimpleSetCurrentIM(instance, imname);*/
		SetCurrentIM(imname);
    }

    unsigned char buffer[1024];
    int numreaded= 0 ;
    numreaded = fread(buffer, sizeof(KEYSTR), 100, fp);
    if(!numreaded) return -2;
    /*klee_make_symbolic(buffer, sizeof(buffer), "keybuf");*/
    int i=0;
    for( i=0; i<numreaded; i++)
    {
        pKEYSTR keystr = (pKEYSTR)(buffer+sizeof(KEYSTR)*i);
        if(!keystr)continue; // if not this check, FcitxHotkeyParseKey will crash.
        ParseKeyStr(keystr);
        usleep(1000);
    }

    /*FcitxSimpleEnd(instance);*/
    /*FcitxInstanceWaitForEnd(instance);*/
    ret = 0;
option_error_end:
    if (fd >= 0)
        unlink(temp);

    if (fp)
        fclose(fp);

    if (ret)
        usage();

    /*fcitx_utils_free(imname);*/
    /*fcitx_utils_free(buf);*/
    /*fcitx_utils_free(buf1);*/
    /*fcitx_utils_free(enableAddon);*/
    /*fcitx_utils_free(addonList);*/
    /*fcitx_utils_free(sandboxDirectory);*/
    return ret;
}
// kate: indent-mode cstyle; space-indent on; indent-width 0;
