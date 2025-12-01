#ifndef ALBUMS_H
#define ALBUMS_H

#include <QString>
#include <QList>
#include <QJsonObject>
#include <QDomElement>
#include <QDomDocument>
#include "morceaux.h"

class Album
{
public:
    Album();

    QString getTitre() const;
    QString getArtiste() const;
    QString getCompositeur() const;
    int getAnnee() const;
    double getPrix() const;
    QString getPhoto() const;
    QString getCategorie() const;
    QString getSupport() const;
    QString getNotes() const;
    QList<Morceau> getMorceaux() const;

    void setTitre(const QString &titre);
    void setArtiste(const QString &artiste);
    void setCompositeur(const QString &compositeur);
    void setAnnee(int annee);
    void setPrix(double prix);
    void setPhoto(const QString &photo);
    void setCategorie(const QString &categorie);
    void setSupport(const QString &support);
    void setNotes(const QString &notes);
    void setMorceaux(const QList<Morceau> &morceaux);



    void ajouterMorceau(const Morceau &morceau);
    void supprimerMorceau(int index);

    QJsonObject toJson() const;
    static Album fromJson(const QJsonObject &obj);
    QDomElement toXml(QDomDocument &doc) const;
    static Album fromXml(const QDomElement &element);

private:
    QString titre;
    QString artiste;
    QString compositeur;
    int annee;
    double prix;
    QString photo;
    QString categorie;
    QString support;
    QString notes;
    QList<Morceau> morceaux;
};

#endif // ALBUMS_H
