#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include <QString>
#include <QFile>
#include <QTranslator>
#include <QDomDocument>
#include <QPixmap>
#include "albums.h"
#include "fichiers.h"
#include "ui_mainwindow.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void chargerJSON();
    void chargerXML();
    void enregistrerJSON();
    void enregistrerSousJSON();
    void enregistrerXML();
    void enregistrerSousXML();
    void quitterApplication();
    void detecterModification();
    void filtrerAlbums(const QString &motif);
    void afficherAlbum(int index);
    void ajouterMorceau();
    void supprimerMorceau();
    void ajouterAlbum();
    void supprimerAlbum();
    void mettreAJourChamps();
    void changerLangueFrancais();
    void changerLangueAnglais();

private:
    Ui::MainWindow *ui;
    QList<Album> albums;
    QString cheminFichierActuel;
    bool donneesModifiees;
    QTranslator translator;
    fichiers fichiers;

    void trierAlbumsParOrdreAlphabetique();
    void majListeAlbums();
    void reinitialiserFormulaire();
    void loadTranslator(const QString &languageCode);
    int calculerTotalMinutes(const Album &album) const;
    int calculerSecondesRestantes(const Album &album) const;
};
#endif // MAINWINDOW_H
