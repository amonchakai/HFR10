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

	Q_PROPERTY( QString savedlogin 	READ getUser   NOTIFY onUserChanged)


private:

	static QString 		m_User;


public:
	// ----------------------------------------------------------------------------------------------
	// member functions

    LoginController(QObject *parent = 0);
	virtual ~LoginController() {};


	const QString &getUser()								{return m_User; }


	// ----------------------------------------------------------------------------------------------
public Q_SLOTS:
	void login(const QString &login, const QString &password);
	void checkReply();
	bool isLogged();
	void logOut();


	// ----------------------------------------------------------------------------------------------

Q_SIGNALS:
	void complete();
	void onUserChanged();



private:

	void saveUserName();
	void loadUserName();
};


#endif /* LOGINCONTROLLER_HPP_ */
