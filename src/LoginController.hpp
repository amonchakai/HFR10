/*
 * LoginDelegate.hpp
 *
 *  Created on: 14 mars 2014
 *      Author: PierreL
 */

#ifndef LOGINCONTROLLER_HPP_
#define LOGINCONTROLLER_HPP_

#include <QtCore/QObject>

class LoginController : public QObject {
	Q_OBJECT

private:



public:
	// ----------------------------------------------------------------------------------------------
	// member functions

    LoginController(QObject *parent = 0);
	virtual ~LoginController() {};




	// ----------------------------------------------------------------------------------------------
public Q_SLOTS:
	void login(const QString &login, const QString &password);
	void checkReply();





	// ----------------------------------------------------------------------------------------------

Q_SIGNALS:
	void complete(const QString &info);

};


#endif /* LOGINCONTROLLER_HPP_ */
