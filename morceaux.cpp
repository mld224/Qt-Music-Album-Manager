#include "morceaux.h"

Morceau::Morceau() : rang(0), minutes(0), secondes(0), appreciation(0) {}

int Morceau::getRang() const {
    return rang;
}

QString Morceau::getTitre() const {
    return titre;
}

int Morceau::getMinutes() const {
    return minutes;
}

int Morceau::getSecondes() const {
    return secondes;
}

int Morceau::getAppreciation() const {
    return appreciation;
}


void Morceau::setRang(int r) {
    rang = r;
}

void Morceau::setTitre(const QString &t) {
    titre = t;
}

void Morceau::setMinutes(int m) {
    minutes = m;
}

void Morceau::setSecondes(int s) {
    secondes = s;
}

void Morceau::setAppreciation(int a) {
    appreciation = a;
}

// Convertit un morceau en objet JSON pour l’export ou la sauvegarde dans un fichier

QJsonObject Morceau::toJson() const {
    QJsonObject obj;
    obj["rank"] = rang;
    obj["title"] = titre;
    obj["minutes"] = minutes;
    obj["seconds"] = secondes;
    obj["appreciation"] = appreciation;
    return obj;
}

// Crée un morceau à partir d’un objet JSON (utilisé lors du chargement)

Morceau Morceau::fromJson(const QJsonObject &obj) {
    Morceau morceau;
    morceau.setRang(obj["rank"].toInt());
    morceau.setTitre(obj["title"].toString());
    morceau.setMinutes(obj["minutes"].toInt());
    morceau.setSecondes(obj["seconds"].toInt());
    morceau.setAppreciation(obj["appreciation"].toInt());
    return morceau;
}

// Convertit un morceau en élément XML pour sauvegarde

QDomElement Morceau::toXml(QDomDocument &doc) const {
    QDomElement element = doc.createElement("track");
    element.setAttribute("rank", rang);
    element.setAttribute("title", titre);
    element.setAttribute("minutes", minutes);
    element.setAttribute("seconds", secondes);
    element.setAttribute("appreciation", appreciation);
    return element;
}

// Crée un morceau à partir d’un élément XML (utilisé pour charger depuis un fichier XML)

Morceau Morceau::fromXml(const QDomElement &element) {
    Morceau morceau;
    morceau.setRang(element.attribute("rank").toInt());
    morceau.setTitre(element.attribute("title"));
    morceau.setMinutes(element.attribute("minutes").toInt());
    morceau.setSecondes(element.attribute("seconds").toInt());
    morceau.setAppreciation(element.attribute("appreciation").toInt());
    return morceau;
}
