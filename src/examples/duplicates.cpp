/*
    Duplicates Example
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
#include <QApplication>
#include <QTreeView>
#include <QDebug>
#include <QTextStream>

#include <personsmodel.h>
#include <duplicatesfinder_p.h>
#include <matchessolver_p.h>
#include <personsmodelfeature.h>

#include <cstdio>
#include <iostream>

using namespace KPeople;

class ResultPrinter : public QObject
{
    Q_OBJECT
    public slots:
        void print(KJob *j) {
            QList<Match> res = ((DuplicatesFinder *) j)->results();
            std::cout << "Results:" << std::endl;
            for (QList<Match>::iterator it = res.begin(); it != res.end();) {
                QStringList roles;
                QStringList rA, rB;
                Q_FOREACH (int i, it->role) {
                    roles += m_model->roleNames()[i];
                    rA += variantToString(it->indexA.data(it->role.first()));
                    rB += variantToString(it->indexB.data(it->role.first()));
                }
                std::cout << "\t- " << qPrintable(roles.join(", ")) << ": " << it->indexA.row() << " " << it->indexB.row()
                          << " because: " << qPrintable(rA.join(", ")) << " // " << qPrintable(rB.join(", ")) << '.' << std::endl;
                bool remove = false;
                if (m_action == Ask) {
                    for (char ans=' '; ans != 'y' && ans != 'n';) {
                        std::cout << "apply? (y/n) ";
                        std::cin >> ans;
                        remove = ans == 'n';
                    }
                }
                if (remove) {
                    it = res.erase(it);
                } else {
                    ++it;}
            }

            if ((m_action == Apply || m_action == Ask) && !res.isEmpty()) {
                MatchesSolver *s = new MatchesSolver(res, m_model, this);
                connect(s, SIGNAL(finished(KJob*)), this, SLOT(matchesSolverDone(KJob*)));
                s->start();
            } else {
                QCoreApplication::instance()->quit();
            }
        }

        void matchesSolverDone(KJob *job) {
            if (job->error() == 0) {
                std::cout << "Matching successfully finished" << std::endl;
            } else {
                std::cout << "Matching failed with error: " << job->error() << std::endl;
            }
            QCoreApplication::instance()->quit();
        }

    private:
        QString variantToString(const QVariant &data) {
            if (data.type() == QVariant::List) {
                QList<QVariant> list = data.toList();
                QStringList strings;
                Q_FOREACH (const QVariant &v, list) {
                    strings += variantToString(v);
                }
                return "("+strings.join(", ")+")";
            } else {
                return data.toString();
            }
        }

    public:
        enum MatchAction { Apply, NotApply, Ask };
        MatchAction m_action;
        PersonsModel *m_model;
};

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    PersonsModel model;

    model.startQuery(QList<PersonsModelFeature>() << PersonsModelFeature::emailModelFeature()
                                                  << PersonsModelFeature::imModelFeature());

    ResultPrinter r;
    r.m_model = &model;
    r.m_action = app.arguments().contains("--apply") ? ResultPrinter::Apply
               : app.arguments().contains("--ask") ? ResultPrinter::Ask
               : ResultPrinter::NotApply;

    DuplicatesFinder *f = new DuplicatesFinder(&model);
    QObject::connect(f, SIGNAL(finished(KJob*)), &r, SLOT(print(KJob*)));
    QObject::connect(&model, SIGNAL(modelInitialized()), f, SLOT(start()));

    app.exec();
}

#include "duplicates.moc"
