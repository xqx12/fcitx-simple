/***************************************************************************
 *   Copyright (C) 2012~2012 by CSSlayer                                   *
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

#ifndef _FCITX_SIMPLE_API_H_
#define _FCITX_SIMPLE_API_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <fcitx/instance.h>
#include "simple.h"

void FcitxSimpleInit(FcitxInstance* instance, FcitxSimpleEventHandler eventHandler, void* userData);

int FcitxSimpleSendKeyEvent(FcitxInstance* instance, boolean release, FcitxKeySym key, unsigned int state, unsigned int keycode);

void FcitxSimpleSetCurrentIM(FcitxInstance* instance, const char* name);

void FcitxSimpleTriggerStatus(FcitxInstance* instance, const char* name);

void FcitxSimpleTriggerMenuItem(FcitxInstance* instance, const char* name, int index);

void FcitxSimpleEnd(FcitxInstance* instance);

int FcitxSimpleSendAndHandle(FcitxInstance *instance, boolean release, FcitxKeySym key, unsigned int state, unsigned int keycode, void* fm);

#ifdef __cplusplus
}
#endif

#endif
