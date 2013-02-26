/*
    Persons Model
    Copyright (C) 2012  Martin Klapetek <martin.klapetek@gmail.com>
    Copyright (C) 2012  Aleix Pol Gonzalez <aleixpol@blue-systems.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/


#ifndef PERSONS_MODEL_H
#define PERSONS_MODEL_H

#include "kpeople_export.h"

#include <QStandardItemModel>

class PersonsPresenceModel;
class PersonsModelContactItem;
class KJob;
class QUrl;
struct PersonsModelPrivate;

namespace Nepomuk2 { class Resource; }

class KPEOPLE_EXPORT PersonsModel : public QStandardItemModel
{
    Q_OBJECT
    Q_DISABLE_COPY(PersonsModel)

public:
    enum ContactType {
        Email,
        IM,
        Phone,
        MobilePhone,
        Postal
    };

    enum Role {
        ContactTypeRole = Qt::UserRole,
        ContactIdRole,
        ContactsCountRole,
        UriRole,
        NameRole,
        LabelRole,
        EmailRole,
        NickRole,
        PhoneRole,
        IMRole,
        PhotoRole,
        IMAccountUriRole,
        IMAccountRole,
        IMAccountTypeRole,
        IMContactRole,
        BlockedRole,
        StatusRole,
        ResourceTypeRole,
        ContactActionsRole,
        ContactGroupsRole,
        ContactCanTextChatRole,
        ContactCanAudioCallRole,
        ContactCanVideoCallRole,
        ContactCanFileTransferRole
    };

    enum ResourceType {
        Person,
        Contact
    };

    /**
     * @p initialize set it to false if you don't want it to use nepomuk values
     *               useful for unit testing.
     */
    PersonsModel(QObject *parent = 0, bool init = true, const QString &customQuery = QString());

    /**
     * The @p contactUri will be removed from @p personUri and it will be added to a new
     * empty pimo:Person instance.
     */
    Q_SCRIPTABLE void unmerge(const QUrl &contactUri, const QUrl &personUri);

    /** Merge all the contacts inside the @p persons */
    void merge(const QList<QUrl> &persons);

    /** this one is because QML is not smart enough to understand what's going on */
    Q_SCRIPTABLE void merge(const QVariantList &persons);

    /** Creates a pimo:person with contacts as groundingOccurances */
    void createPersonFromContacts(const QList<QUrl> &contacts);

    /**
     * Creates a pimo:person from indexes, checking if one of them isn't person already and
     * adding the contacts to it if it is
     */
    void createPersonFromIndexes(const QList<QModelIndex> &indexes);

    /** Adds contacts to existing PIMO:Person */
    void addContactsToPerson(const QUrl &personUri, const QList<QUrl> &contacts);

    /** Removes given contacts from existing PIMO:Person */
    void removeContactsFromPerson(const QUrl &personUri, const QList<QUrl> &contacts);

    /** Removes the link between all contacts, removes the pimo:person but leaves the contacts intact */
    void removePerson(const QUrl &uri);
    //FIXME: maybe merge with ^ ?
    void removePersonFromModel(const QModelIndex &index);

    QModelIndex indexForUri(const QUrl &uri) const;

    void createPerson(const Nepomuk2::Resource &res);
    void createContact(const Nepomuk2::Resource &res);
    PersonsModelContactItem* contactForIMAccount(const QUrl &uri) const;

private slots:
    void jobFinished(KJob *job);
    void query(const QString &queryString);

signals:
    void peopleAdded();

private:
    QModelIndex findRecursively(int role, const QVariant &value, const QModelIndex &idx = QModelIndex()) const;

    friend class PersonsPresenceModel;

//     PersonsModelPrivate * const d_ptr;
//     Q_DECLARE_PRIVATE(PersonsModel);
};

#endif // PERSONS_MODEL_H
