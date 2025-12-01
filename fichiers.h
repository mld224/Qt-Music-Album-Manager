#ifndef FICHIERS_H
#define FICHIERS_H

#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QMessageBox>
#include <QDomDocument>
#include "albums.h"

class fichiers
{
private:
    QString nom;
public:
    fichiers();
    QList<Album> chargerJSON(const QString &chemin);
    QList<Album> chargerXML(const QString &chemin);
    bool enregistrerJSON(const QString &chemin, const QList<Album> &albums);
    bool enregistrerXML(const QString &chemin, const QList<Album> &albums);

    QString getNom() const;
    void setNom(const QString &n);
};

#endif // FICHIERS_H
