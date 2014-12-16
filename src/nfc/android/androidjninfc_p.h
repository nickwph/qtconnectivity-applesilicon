/***************************************************************************
**
** Copyright (C) 2013 Centria research and development
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtNfc module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef ANDROIDJNINFC_H
#define ANDROIDJNINFC_H

#include "qglobal.h"

#include <jni.h>

#define QT_USE_ANDROIDNFC_NAMESPACE using namespace ::AndroidNfc;
#define QT_BEGIN_ANDROIDNFC_NAMESPACE namespace AndroidNfc {
#define QT_END_ANDROIDNFC_NAMESPACE }

QT_BEGIN_ANDROIDNFC_NAMESPACE

class AndroidNfcListenerInterface
{
public:
    virtual ~AndroidNfcListenerInterface(){}
    virtual void newIntent(jobject intent) = 0;
};

class AttachedJNIEnv
{
public:
    AttachedJNIEnv();
    virtual ~AttachedJNIEnv();
    bool attached;
    JNIEnv *jniEnv;

private:
    Q_DISABLE_COPY(AttachedJNIEnv)
};

bool startDiscovery();
bool stopDiscovery();
bool isAvailable();
bool registerListener(AndroidNfcListenerInterface *listener);
bool unregisterListener(AndroidNfcListenerInterface *listener);
jobject getTag(JNIEnv *env, jobject intent);

QT_END_ANDROIDNFC_NAMESPACE

#endif
