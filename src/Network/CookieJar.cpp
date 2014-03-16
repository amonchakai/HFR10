/*
 * CookieJar.cpp
 *
 *  Created on: 15 mars 2014
 *      Author: PierreL
 */

#include "CookieJar.hpp"



CookieJar *CookieJar::m_This = NULL;


CookieJar::CookieJar(QObject *parent) : QNetworkCookieJar(parent) {}

CookieJar *CookieJar::get() {
	if(m_This == NULL)
		m_This = new CookieJar(NULL);

	return m_This;
}

CookieJar::~CookieJar() {

}

/*
 * Code for Loading & Saving from
 * https://qt.gitorious.org/qt/qt/source/6ede9f99f98ed511b78935f7c6537470dce94239:demos/browser/cookiejar.cpp#Lundefined
 */

void CookieJar::saveToDisk() {

}


void CookieJar::loadFromDisk() {

}


