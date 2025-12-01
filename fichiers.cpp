#include "fichiers.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QMessageBox>
#include <QDomDocument>

fichiers::fichiers() : nom("gestionnaire de fichiers") {}

// Charge une liste d’albums depuis un fichier JSON donné en paramètre
// Utilisé pour importer des données JSON dans l'application

QList<Album> fichiers::chargerJSON(const QString &chemin) {
    QList<Album> albums;
    QFile fichier(chemin);
    if (!fichier.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(nullptr, QObject::tr("Error"), QObject::tr("Unable to open the file."));
        return albums;
    }

    QByteArray donnees = fichier.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(donnees);
    QJsonArray tableau = doc.array();

    for (const QJsonValue &val : tableau) {
        albums.append(Album::fromJson(val.toObject()));
    }

    return albums;
}


// Charge une liste d’albums depuis un fichier XML donné
// Sert à importer des albums depuis un fichier XML

QList<Album> fichiers::chargerXML(const QString &chemin) {
    QList<Album> albums;
    QFile fichier(chemin);
    if (!fichier.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(nullptr, QObject::tr("Error"), QObject::tr("Unable to open the file."));
        return albums;
    }

    QDomDocument doc;
    if (!doc.setContent(&fichier)) {
        QMessageBox::warning(nullptr, QObject::tr("Error"), QObject::tr("Invalid XML file."));
        return albums;
    }

    QDomElement racine = doc.documentElement();
    QDomNodeList liste = racine.elementsByTagName("album");

    for (int i = 0; i < liste.count(); ++i) {
        QDomElement el = liste.at(i).toElement();
        albums.append(Album::fromXml(el));
    }

    return albums;
}

// Sauvegarde la liste des albums dans un fichier JSON
// Utilisé lors de l’enregistrement (ou enregistrement sous) au format JSON

bool fichiers::enregistrerJSON(const QString &chemin, const QList<Album> &albums) {
    QFile fichier(chemin);
    if (!fichier.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(nullptr, QObject::tr("Error"), QObject::tr("Unable to save the file."));
        return false;
    }

    QJsonArray tableau;
    for (const Album &a : albums) {
        tableau.append(a.toJson());
    }

    QJsonDocument doc(tableau);
    fichier.write(doc.toJson());
    return true;
}

// Sauvegarde la liste des albums dans un fichier XML ;
// Utilisé pour l’enregistrement au format XM


bool fichiers::enregistrerXML(const QString &chemin, const QList<Album> &albums) {
    QFile fichier(chemin);
    if (!fichier.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(nullptr, QObject::tr("Error"), QObject::tr("Unable to save the file."));
        return false;
    }

    QDomDocument doc;
    QDomElement racine = doc.createElement("albums");
    doc.appendChild(racine);

    for (const Album &a : albums) {
        racine.appendChild(a.toXml(doc));
    }

    QTextStream stream(&fichier);
    doc.save(stream, 4);
    return true;
}


QString fichiers::getNom() const {
    return nom;
}

void fichiers::setNom(const QString &n) {
    nom = n;
}










