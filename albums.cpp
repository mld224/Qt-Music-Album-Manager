#include "albums.h"
#include <QJsonArray>

Album::Album() : annee(0), prix(0.0) {}

QString Album::getTitre() const {
    return titre;
}


QString Album::getArtiste() const {
    return artiste;
}


QString Album::getCompositeur() const {
    return compositeur;
}

int Album::getAnnee() const {
    return annee;
}

double Album::getPrix() const {
    return prix;
}


QString Album::getPhoto() const {
    return photo;
}


QString Album::getCategorie() const {
    return categorie;
}

QString Album::getSupport() const {
    return support;
}

QString Album::getNotes() const {
    return notes;
}

QList<Morceau> Album::getMorceaux() const {
    return morceaux;
}

void Album::setTitre(const QString &t) {
    titre = t;
}

void Album::setArtiste(const QString &a) {
    artiste = a;
}

void Album::setCompositeur(const QString &c) {
    compositeur = c;
}

void Album::setAnnee(int a) {
    annee = a;
}

void Album::setPrix(double p) {
    prix = p;
}

void Album::setPhoto(const QString &p) {
    photo = p;
}

void Album::setCategorie(const QString &c) {
    categorie = c;
}

void Album::setSupport(const QString &s) {
    support = s;
}

void Album::setNotes(const QString &n) {
    notes = n;
}

void Album::setMorceaux(const QList<Morceau> &m) {
    morceaux = m;
}

void Album::ajouterMorceau(const Morceau &morceau) {
    morceaux.append(morceau);
}


void Album::supprimerMorceau(int index) {
    morceaux.removeAt(index);
}


QJsonObject Album::toJson() const {
    QJsonObject obj;
    obj["title"] = titre;
    obj["artist"] = artiste;
    obj["composer"] = compositeur;
    obj["year"] = annee;
    obj["price"] = prix;
    obj["photo"] = photo;
    obj["category"] = categorie;
    obj["support"] = support;
    obj["notes"] = notes;

    QJsonArray morceauxArray;
    for (const Morceau &m : morceaux) {
        morceauxArray.append(m.toJson());
    }
    obj["tracks"] = morceauxArray;

    return obj;
}

Album Album::fromJson(const QJsonObject &obj) {
    Album album;
    album.setTitre(obj["title"].toString());
    album.setArtiste(obj["artist"].toString());
    album.setCompositeur(obj["composer"].toString());
    album.setAnnee(obj["year"].toInt());
    album.setPrix(obj["price"].toDouble());
    album.setPhoto(obj["photo"].toString());
    album.setCategorie(obj["category"].toString());
    album.setSupport(obj["support"].toString());
    album.setNotes(obj["notes"].toString());

    QList<Morceau> morceaux;
    QJsonArray morceauxArray = obj["tracks"].toArray();
    for (const QJsonValue &val : morceauxArray) {
        morceaux.append(Morceau::fromJson(val.toObject()));
    }
    album.setMorceaux(morceaux);

    return album;
}


QDomElement Album::toXml(QDomDocument &doc) const {
    QDomElement element = doc.createElement("album");
    element.setAttribute("title", titre);
    element.setAttribute("artist", artiste);
    element.setAttribute("composer", compositeur);
    element.setAttribute("year", annee);
    element.setAttribute("price", prix);
    element.setAttribute("photo", photo);
    element.setAttribute("category", categorie);
    element.setAttribute("support", support);
    element.setAttribute("notes", notes);

    for (const Morceau &m : morceaux) {
        element.appendChild(m.toXml(doc));
    }

    return element;
}

Album Album::fromXml(const QDomElement &element) {
    Album album;
    album.setTitre(element.attribute("title"));
    album.setArtiste(element.attribute("artist"));
    album.setCompositeur(element.attribute("composer"));
    album.setAnnee(element.attribute("year").toInt());
    album.setPrix(element.attribute("price").toDouble());
    album.setPhoto(element.attribute("photo"));
    album.setCategorie(element.attribute("category"));
    album.setSupport(element.attribute("support"));
    album.setNotes(element.attribute("notes"));

    QList<Morceau> morceaux;
    QDomNodeList morceauList = element.elementsByTagName("track");
    for (int i = 0; i < morceauList.count(); ++i) {
        QDomElement morceauElement = morceauList.at(i).toElement();
        morceaux.append(Morceau::fromXml(morceauElement));
    }
    album.setMorceaux(morceaux);

    return album;
}












