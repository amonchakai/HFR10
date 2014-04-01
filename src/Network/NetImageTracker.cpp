/*
 * NetImageTracker.cpp
 *
 *  Created on: 22 mars 2014
 *      Author: PierreL
 */

#include "NetImageTracker.h"
#include <bb/cascades/ImageTracker>

using namespace bb;
using namespace bb::cascades;

NetImageTracker::NetImageTracker(QObject *parent) :
        ImageTracker(parent)
{
    mIsCreated = false;

    connect(this, SIGNAL(creationCompleted()), this, SLOT(onCreationCompleted()));
    connect(WebResourceManager::get(), SIGNAL(onImageReady(const QString &, const QString &)), this, SLOT(onImageReady(const QString&, const QString&)));
}

void NetImageTracker::onCreationCompleted()
{
    mIsCreated = true;

    if (!mSource.isEmpty()) {
    	WebResourceManager::get()->getImage(mSource);
    }
}

void NetImageTracker::onImageReady(const QString imageName, const QString filePath) {
	if (imageName.compare(mSource) == 0) {

        // Set the path to the image that is now downloaded and cached in the data folder on the device.
        QUrl url = QUrl(filePath);
        setImageSource(url);
    } else if (imageName.compare("loading") == 0) {

        // If we don't have an image to display, let's display a loading image
        QUrl url = QUrl("asset:///images/default_avatar.png");
        setImageSource(url);
    }
}

void NetImageTracker::setSource(const QString &source) {
    if (!source.isEmpty() && mSource.compare(source) != 0) {
        mSource = source;

        WebResourceManager::get()->getImage(mSource);

        emit sourceChanged(mSource);
    }
}

QString NetImageTracker::source() {
    return mSource;
}

