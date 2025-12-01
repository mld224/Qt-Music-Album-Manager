#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "fichiers.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QPixmap>
#include <QPlainTextEdit>
#include <QDomDocument>
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QApplication>
#include <QInputDialog>
#include <QTableWidgetItem>
#include <algorithm>



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), donneesModifiees(false)
{
    ui->setupUi(this);

    connect(ui->champNotes, &QPlainTextEdit::textChanged, this, &MainWindow::detecterModification);
    connect(ui->champFiltre, &QLineEdit::textChanged, this, &MainWindow::filtrerAlbums);



    connect(ui->listeAlbums, &QListWidget::currentRowChanged, this, &MainWindow::afficherAlbum);
    connect(ui->boutonSauvegarder, &QPushButton::clicked, this, &MainWindow::mettreAJourChamps);
    connect(ui->boutonAjouterMorceau, &QPushButton::clicked, this, &MainWindow::ajouterMorceau);
    connect(ui->boutonSupprimerMorceau, &QPushButton::clicked, this, &MainWindow::supprimerMorceau);
    connect(ui->boutonAjouterAlbum, &QPushButton::clicked, this, &MainWindow::ajouterAlbum);
    connect(ui->boutonSupprimerAlbum, &QPushButton::clicked, this, &MainWindow::supprimerAlbum);
    connect(ui->boutonQuitter, &QPushButton::clicked, this, &MainWindow::quitterApplication);

    connect(ui->actionCharger_JSON, &QAction::triggered, this, &MainWindow::chargerJSON);
    connect(ui->actionCharger_XML, &QAction::triggered, this, &MainWindow::chargerXML);
    connect(ui->actionEnregistrer_JSON, &QAction::triggered, this, &MainWindow::enregistrerJSON);
    connect(ui->actionEnregistrer_sous_JSON, &QAction::triggered, this, &MainWindow::enregistrerSousJSON);
    connect(ui->actionEnregistrer_XML, &QAction::triggered, this, &MainWindow::enregistrerXML);
    connect(ui->actionEnregistrer_sous_XML, &QAction::triggered, this, &MainWindow::enregistrerSousXML);
    connect(ui->actionFrancais, &QAction::triggered, this, &MainWindow::changerLangueFrancais);
    connect(ui->actionAnglais, &QAction::triggered, this, &MainWindow::changerLangueAnglais);

}

MainWindow::~MainWindow()
{
    delete ui;
}




void MainWindow::changerLangueFrancais()
{
    loadTranslator("fr");
}


void MainWindow::changerLangueAnglais()
{
    loadTranslator("en");
}


void MainWindow::loadTranslator(const QString &languageCode)
{
    bool loaded = false;
    if (languageCode == "fr") {

        loaded = translator.load("S2-01_fr_FR.qm");
    } else if (languageCode == "en") {
        loaded = translator.load("S2-01_en_US.qm");
    }

    if (!loaded) {
        QMessageBox::warning(this, tr("Error"), tr("Unable to load the translation file."));
    } else {

        qApp->removeTranslator(&translator);
        qApp->installTranslator(&translator);
        ui->retranslateUi(this);
    }
}


// Ouvre une fenêtre pour ajouter un nouvel album avec ses informations
 // L'album est ensuite ajouté à la liste principale et trié


void MainWindow::ajouterAlbum()
{
    QDialog dialog(this);
    dialog.setWindowTitle(tr("Add an album"));

    QFormLayout form(&dialog);

    QLineEdit *titreEdit = new QLineEdit(&dialog);
    QLineEdit *artisteEdit = new QLineEdit(&dialog);
    QLineEdit *compositeurEdit = new QLineEdit(&dialog);
    QLineEdit *anneeEdit = new QLineEdit(&dialog);
    QLineEdit *prixEdit = new QLineEdit(&dialog);
    QComboBox *categorieEdit = new QComboBox(&dialog);
    QComboBox *supportEdit = new QComboBox(&dialog);
    QPlainTextEdit *notesEdit = new QPlainTextEdit(&dialog);
    QPushButton *photoButton = new QPushButton(tr("Choose a photo"), &dialog);
    QString photoPath;

    categorieEdit->addItems({tr("pop"), tr("rock"), tr("blues"), tr("jazz"), tr("rap"), tr("classical"), tr("French variety"), tr("electronic")});
    supportEdit->addItems({tr("LP"), tr("Cassette"), tr("CD"), tr("MP3"), tr("AAC/M4A"), tr("FLAC")});

    form.addRow(tr("Title:"), titreEdit);
    form.addRow(tr("Artist:"), artisteEdit);
    form.addRow(tr("Composer:"), compositeurEdit);
    form.addRow(tr("Year:"), anneeEdit);
    form.addRow(tr("Price (€):"), prixEdit);
    form.addRow(tr("Category:"), categorieEdit);
    form.addRow(tr("Support:"), supportEdit);
    form.addRow(tr("Notes:"), notesEdit);
    form.addRow(photoButton);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);

    connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);
    connect(photoButton, &QPushButton::clicked, [&]() {
        photoPath = QFileDialog::getOpenFileName(this, tr("Open an image"), "", tr("Images (*.png *.jpg *.jpeg *.bmp)"));
    });

    if (dialog.exec() == QDialog::Accepted) {
        if (titreEdit->text().isEmpty() || artisteEdit->text().isEmpty() || anneeEdit->text().isEmpty() || prixEdit->text().isEmpty() || photoPath.isEmpty()) {
            QMessageBox::warning(this, tr("Error"), tr("All fields must be filled out, including the photo."));
            return;
        }

        Album nouvelAlbum;
        nouvelAlbum.setTitre(titreEdit->text());
        nouvelAlbum.setArtiste(artisteEdit->text());
        nouvelAlbum.setCompositeur(compositeurEdit->text());
        nouvelAlbum.setAnnee(anneeEdit->text().toInt());
        nouvelAlbum.setPrix(prixEdit->text().toDouble());
        nouvelAlbum.setCategorie(categorieEdit->currentText());
        nouvelAlbum.setSupport(supportEdit->currentText());
        nouvelAlbum.setNotes(notesEdit->toPlainText());
        nouvelAlbum.setPhoto(photoPath);

        albums.append(nouvelAlbum);
        trierAlbumsParOrdreAlphabetique();
        majListeAlbums();
        detecterModification();
    }
}


// Supprime l’album sélectionné après confirmation de l’utilisateur
// Met à jour l’interface après suppression
void MainWindow::supprimerAlbum()
{
    int index = ui->listeAlbums->currentRow();
    if (index < 0 || index >= albums.size()) return;

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Delete the album"), tr("Are you sure you want to delete this album?"),
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        albums.removeAt(index);
        majListeAlbums();
        reinitialiserFormulaire();
        detecterModification();
    }
}


// Ajoute un morceau à l’album actuellement sélectionné
// Ouvre une boîte de dialogue pour saisir les informations du morceau
void MainWindow::ajouterMorceau()
{
    int index = ui->listeAlbums->currentRow();
    if (index < 0 || index >= albums.size()) return;

    QDialog dialog(this);
    dialog.setWindowTitle(tr("Add a track"));

    QFormLayout form(&dialog);

    QLineEdit *titreEdit = new QLineEdit(&dialog);
    QLineEdit *minutesEdit = new QLineEdit(&dialog);
    QLineEdit *secondesEdit = new QLineEdit(&dialog);
    QLineEdit *appreciationEdit = new QLineEdit(&dialog);

    form.addRow(tr("Title:"), titreEdit);
    form.addRow(tr("Minutes:"), minutesEdit);
    form.addRow(tr("Seconds:"), secondesEdit);
    form.addRow(tr("Appreciation:"), appreciationEdit);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);

    connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Accepted) {
        if (titreEdit->text().isEmpty() || minutesEdit->text().isEmpty() || secondesEdit->text().isEmpty() || appreciationEdit->text().isEmpty()) {
            QMessageBox::warning(this, tr("Error"), tr("All fields must be filled out."));
            return;
        }

        Morceau nouveauMorceau;
        nouveauMorceau.setTitre(titreEdit->text());
        nouveauMorceau.setMinutes(minutesEdit->text().toInt());
        nouveauMorceau.setSecondes(secondesEdit->text().toInt());
        nouveauMorceau.setAppreciation(appreciationEdit->text().toInt());


        nouveauMorceau.setRang(albums[index].getMorceaux().size() + 1);

        albums[index].ajouterMorceau(nouveauMorceau);
        afficherAlbum(index);
        detecterModification();
    }
}



// Supprime un morceau sélectionné dans la table des morceaux après confirmation

void MainWindow::supprimerMorceau()
{
    int index = ui->listeAlbums->currentRow();
    if (index < 0 || index >= albums.size()) return;

    int morceauIndex = ui->tableMorceaux->currentRow();
    if (morceauIndex >= 0) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, tr("Delete the track"), tr("Are you sure you want to delete this track?"),
                                      QMessageBox::Yes | QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            albums[index].supprimerMorceau(morceauIndex);
            afficherAlbum(index);
            detecterModification();
        }
    }
}


// Ouvre un fichier JSON et charge les albums dans l’application
// Met à jour l’affichage et trie les albums
void MainWindow::chargerJSON()
{
    QString chemin = QFileDialog::getOpenFileName(this, tr("Open a JSON file"), "", tr("JSON Files (*.json)"));
    if (chemin.isEmpty()) return;

    albums = fichiers.chargerJSON(chemin);
    trierAlbumsParOrdreAlphabetique();
    cheminFichierActuel = chemin;
    majListeAlbums();
    detecterModification();
}

// Même chose que `chargerJSON` mais avec un fichier XML
void MainWindow::chargerXML()
{
    QString chemin = QFileDialog::getOpenFileName(this, tr("Open an XML file"), "", tr("XML Files (*.xml)"));
    if (chemin.isEmpty()) return;

    albums = fichiers.chargerXML(chemin);
    trierAlbumsParOrdreAlphabetique();
    cheminFichierActuel = chemin;
    majListeAlbums();
    detecterModification();
}


// Sauvegarde les données actuelles dans le fichier JSON ouvert
// Affiche une alerte si aucun fichier n’est ouvert
void MainWindow::enregistrerJSON()
{
    if (cheminFichierActuel.isEmpty()) {
        QMessageBox::warning(this, tr("Error"), tr("No file open to save changes."));
        return;
    }

    if (!cheminFichierActuel.endsWith(".json")) {
        QMessageBox::warning(this, tr("Error"), tr("The open file is not a JSON file. Use 'Save as JSON' to save in JSON format."));
        return;
    }

    if (!fichiers.enregistrerJSON(cheminFichierActuel, albums)) {
        QMessageBox::warning(this, tr("Error"), tr("Unable to save the file."));
    } else {
        donneesModifiees = false;
    }
}



// Sauvegarde dans le fichier XML ouvert
 // Gère les erreurs si le fichier n’est pas un XML
void MainWindow::enregistrerXML()
{
    if (cheminFichierActuel.isEmpty()) {
        QMessageBox::warning(this, tr("Error"), tr("No file open to save changes."));
        return;
    }

    if (!cheminFichierActuel.endsWith(".xml")) {
        QMessageBox::warning(this, tr("Error"), tr("The open file is not an XML file. Use 'Save as XML' to save in XML format."));
        return;
    }

    if (!fichiers.enregistrerXML(cheminFichierActuel, albums)){
        QMessageBox::warning(this, tr("Error"), tr("Unable to save the file."));
    } else {
        donneesModifiees = false;
    }
}


// Ouvre une boîte de dialogue pour choisir un nouveau fichier JSON
// Sauvegarde ensuite les albums dedans

void MainWindow::enregistrerSousJSON()
{
    QString chemin = QFileDialog::getSaveFileName(this, tr("Save as JSON"), "", tr("JSON Files (*.json)"));
    if (!chemin.isEmpty()) {
        cheminFichierActuel = chemin;
        if (!fichiers.enregistrerJSON(chemin, albums)) {
            QMessageBox::warning(this, tr("Error"), tr("Unable to save the file."));
        } else {
            donneesModifiees = false;
        }
    }
}

// Même chose que `enregistrerSousJSON` mais en XML

void MainWindow::enregistrerSousXML()
{
    QString chemin = QFileDialog::getSaveFileName(this, tr("Save as XML"), "", tr("XML Files (*.xml)"));
    if (!chemin.isEmpty()) {
        cheminFichierActuel = chemin;
        if (!fichiers.enregistrerXML(chemin, albums)) {
            QMessageBox::warning(this, tr("Error"), tr("Unable to save the file."));
        } else {
            donneesModifiees = false;
        }
    }
}

void MainWindow::detecterModification()
{
    donneesModifiees = true;
}






void MainWindow::filtrerAlbums(const QString &motif)
{
    for (int i = 0; i < ui->listeAlbums->count(); ++i) {
        QListWidgetItem *item = ui->listeAlbums->item(i);
        item->setHidden(!item->text().contains(motif, Qt::CaseInsensitive));
    }
}


// Met à jour l’interface graphique pour afficher toutes les infos de l’album sélectionné
// Affiche aussi la liste des morceaux et l’image de la pochette

void MainWindow::afficherAlbum(int index)
{
    if (index < 0 || index >= albums.size()) return;
    const Album &a = albums[index];

    ui->champTitre->setText(a.getTitre());
    ui->champArtiste->setText(a.getArtiste());
    ui->champCompositeur->setText(a.getCompositeur());
    ui->champAnnee->setText(QString::number(a.getAnnee()));
    ui->champPrix->setText(QString::number(a.getPrix()));
    ui->champCategorie->setCurrentText(a.getCategorie());
    ui->champSupport->setCurrentText(a.getSupport());
    ui->champNotes->setPlainText(a.getNotes());

    QPixmap image(a.getPhoto());
    ui->champImage->setPixmap(image.scaled(ui->champImage->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));

    ui->tableMorceaux->setRowCount(a.getMorceaux().size());
    int row = 0;
    for (const Morceau &m : a.getMorceaux()) {
        ui->tableMorceaux->setItem(row, 0, new QTableWidgetItem(QString::number(m.getRang())));
        ui->tableMorceaux->setItem(row, 1, new QTableWidgetItem(m.getTitre()));
        ui->tableMorceaux->setItem(row, 2, new QTableWidgetItem(QString("%1:%2").arg(m.getMinutes()).arg(m.getSecondes(), 2, 10, QLatin1Char('0'))));
        ui->tableMorceaux->setItem(row, 3, new QTableWidgetItem(QString::number(m.getAppreciation())));
        row++;
    }

    int totalMinutes = calculerTotalMinutes(a);
    int secondesRestantes = calculerSecondesRestantes(a);
    ui->labelTotalMinutes->setText(tr("Total: %1:%2").arg(totalMinutes).arg(secondesRestantes, 2, 10, QLatin1Char('0')));
}


void MainWindow::trierAlbumsParOrdreAlphabetique()
{
    std::sort(albums.begin(), albums.end(), [](const Album &a, const Album &b) {
        return a.getTitre().toLower() < b.getTitre().toLower();
    });
}

int MainWindow::calculerTotalMinutes(const Album &album) const
{
    int totalMinutes = 0;
    int totalSecondes = 0;
    for (const Morceau &m : album.getMorceaux()) {
        totalMinutes += m.getMinutes();
        totalSecondes += m.getSecondes();
    }
    totalMinutes += totalSecondes / 60;
    return totalMinutes;
}


// Calcule le total des minutes de tous les morceaux d’un album

int MainWindow::calculerSecondesRestantes(const Album &album) const
{
    int totalSecondes = 0;
    for (const Morceau &m : album.getMorceaux()) {
        totalSecondes += m.getSecondes();
    }
    return totalSecondes % 60;
}








void MainWindow::majListeAlbums()
{
    ui->listeAlbums->clear();
    for (const Album &a : albums) {
        ui->listeAlbums->addItem(a.getTitre());
    }
}

void MainWindow::reinitialiserFormulaire()
{
    ui->champTitre->clear();
    ui->champArtiste->clear();
    ui->champCompositeur->clear();
    ui->champAnnee->clear();
    ui->champPrix->clear();
    ui->champCategorie->setCurrentIndex(0);
    ui->champSupport->setCurrentIndex(0);
    ui->champNotes->clear();
    ui->tableMorceaux->clearContents();
    ui->champImage->clear();
}

// Met à jour l’album sélectionné avec les valeurs modifiées dans les champs
// Appelle la sauvegarde automatique si un fichier est ouvert

void MainWindow::mettreAJourChamps()
{
    int index = ui->listeAlbums->currentRow();
    if (index < 0 || index >= albums.size()) return;

    Album &a = albums[index];
    a.setTitre(ui->champTitre->text());
    a.setArtiste(ui->champArtiste->text());
    a.setCompositeur(ui->champCompositeur->text());
    a.setAnnee(ui->champAnnee->text().toInt());
    a.setPrix(ui->champPrix->text().toDouble());
    a.setCategorie(ui->champCategorie->currentText());
    a.setSupport(ui->champSupport->currentText());
    a.setNotes(ui->champNotes->toPlainText());

    if (!cheminFichierActuel.isEmpty()) {
        if (cheminFichierActuel.endsWith(".json")) {
            enregistrerJSON();
        } else if (cheminFichierActuel.endsWith(".xml")) {
            enregistrerXML();
        }
    }
    detecterModification();
}



void MainWindow::quitterApplication()
{
    if (donneesModifiees) {
        int reponse = QMessageBox::question(this, tr("Quit"), tr("There are unsaved changes. Do you want to save them before quitting?"));
        if (reponse == QMessageBox::Yes) {
            if (!cheminFichierActuel.isEmpty()) {
                if (cheminFichierActuel.endsWith(".json")) {
                    enregistrerJSON();
                } else if (cheminFichierActuel.endsWith(".xml")) {
                    enregistrerXML();
                }
            } else {
                QMessageBox::warning(this, tr("Error"), tr("No file open to save changes."));
            }
        }
    }
    QApplication::quit();
}
















