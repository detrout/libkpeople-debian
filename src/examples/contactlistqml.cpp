/*
    Persons Model Example
    Copyright (C) 2012 Aleix Pol Gonzalez <aleixpol@blue-systems.com>

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
#include <KApplication>
#include <KCmdLineArgs>
#include <KDebug>
#include <QDeclarativeView>
#include <qdeclarativeerror.h>

#include <kdeclarative.h>

int main(int argc, char** argv)
{
    KCmdLineArgs::init(argc, argv, QByteArray("sweetpeople"), "", ki18n("SweetPeople - KPeople Example"), "0.1");

    KApplication app;

    QDeclarativeView view;
    KDeclarative decl;
    decl.setDeclarativeEngine(view.engine());
    decl.initialize();
    decl.setupBindings();
    view.setSource(QUrl("qrc:/qml/Main.qml"));
    view.setResizeMode(QDeclarativeView::SizeRootObjectToView);
    kDebug() << view.errors();
    view.show();

    app.exec();
}
