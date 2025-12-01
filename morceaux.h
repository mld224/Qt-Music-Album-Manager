#ifndef MORCEAUX_H
#define MORCEAUX_H

#include <QString>
#include <QJsonObject>
#include <QDomElement>
#include <QDomDocument>

class Morceau
{
public:
    Morceau();

    int getRang() const;
    QString getTitre() const;
    int getMinutes() const;
    int getSecondes() const;
    int getAppreciation() const;

    void setRang(int rang);
    void setTitre(const QString &titre);
    void setMinutes(int minutes);
    void setSecondes(int secondes);
    void setAppreciation(int appreciation);


    QJsonObject toJson() const;
    static Morceau fromJson(const QJsonObject &obj);
    QDomElement toXml(QDomDocument &doc) const;
    static Morceau fromXml(const QDomElement &element);

private:
    int rang;
    QString titre;
    int minutes;
    int secondes;
    int appreciation;
};

#endif // MORCEAUX_H
