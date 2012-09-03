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

struct PersonsModelPrivate;
class KJob;
class PersonsModelItem;
class PersonsModelContactItem;
class QUrl;

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
        ContactsCount,
        UriRole,
        NameRole,
        EmailRole,
        NickRole,
        PhoneRole,
        IMRole,
        PhotoRole
    };

    /**
     * @p initialize set it to false if you don't want it to use nepomuk values
     *               useful for unit testing.
     */
    explicit PersonsModel(QObject *parent = 0, bool initialize = true);

    /**
     * The @p contactUri will be removed from @p personUri and it will be added to a new 
     * empty pimo:Person instance.
     */
    Q_SCRIPTABLE void unmerge(const QUrl& contactUri, const QUrl& personUri);

    /** Merge all the contacts inside the @p persons */
    void merge(const QList<QUrl>& persons);

    /** this one is because QML is not smart enough to understand what's going on */
    Q_SCRIPTABLE void merge(const QVariantList& persons);

    QModelIndex indexForUri(const QUrl& uri) const;
private slots:
    void jobFinished(KJob*);
    void query();

signals:
    void peopleAdded();

private:
    QModelIndex findRecursively(int role, const QVariant& value, const QModelIndex& idx=QModelIndex()) const;
    
    PersonsModelPrivate * const d_ptr;
    Q_DECLARE_PRIVATE(PersonsModel)
};

#endif // PERSONS_MODEL_H
