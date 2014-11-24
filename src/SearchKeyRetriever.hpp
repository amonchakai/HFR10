/*
 * SearchKeyRetriever.hpp
 *
 *  Created on: 16 avr. 2014
 *      Author: pierre
 */

#ifndef SEARCHKEYRETRIEVER_HPP_
#define SEARCHKEYRETRIEVER_HPP_


#include <QtCore/QObject>

class SearchKeyRetriever : public QObject {
	Q_OBJECT;

	Q_PROPERTY( QString haskKey 		READ getHashKey        NOTIFY hashKeyChanged)

	private:

		QString							  m_HashKey;


	// ----------------------------------------------------------------------------------------------
	public:
		SearchKeyRetriever(QObject *parent = 0) : QObject(parent) {};
		virtual ~SearchKeyRetriever() {};


		inline const QString &getHashKey() const 			{ return m_HashKey; }


	// ----------------------------------------------------------------------------------------------

	public Q_SLOTS:
		void getKey();
		void checkReply();


	// ----------------------------------------------------------------------------------------------
	Q_SIGNALS:
		void hashKeyChanged();
};


#endif /* SEARCHKEYRETRIEVER_HPP_ */
