/*
 * DialogSMP.hpp
 *
 *  Created on: 12 juil. 2015
 *      Author: pierre
 */

#ifndef DIALOGSMP_HPP_
#define DIALOGSMP_HPP_


#include <bb/cascades/CustomControl>

using namespace bb::cascades;

namespace bb
{
    namespace cascades
    {
        class Dialog;
        class TextField;
        class CheckBox;
    }
}



class DialogSearch : public QObject
{
    Q_OBJECT

public:
    DialogSearch(QObject * parent = 0);
    virtual ~DialogSearch();

    void setVisible(bool visibility);


Q_SIGNALS:
    void validated(const QString& keywords, const QString& author, bool filter);


private slots:


    void hideDialog();
    void validDialog();

private:
    // Whether the custom dialog is visible or not
    bool mVisible;

    // The custom dialog
    Dialog      *mCustomDialog;
    TextField   *m_KeywordsTextField;
    TextField   *m_AuthorTextField;
    CheckBox    *m_FilterCheckBox;


};


#endif /* DIALOGSMP_HPP_ */
