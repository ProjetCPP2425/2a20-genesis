#include "statistique.h"
#include "ui_statistique.h"
#include <QtCharts/QBarSeries>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QVBoxLayout>
#include <QSqlQuery>
#include <QMap>
#include <QColor>
#include <QPushButton>
#include <QTextToSpeech>
#include <QComboBox>
#include <QLabel>

QT_USE_NAMESPACE

statistique::statistique(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::statistique)
{
    ui->setupUi(this);
    this->showMaximized();

    // 📊 Données catégories
    QMap<QString, int> categoryCounts;
    QSqlQuery query("SELECT CATEGORIE FROM BOUTIQUE");
    while (query.next()) {
        QString cat = query.value(0).toString().trimmed();
        categoryCounts[cat]++;
    }

    // 🌈 Couleurs pour les barres
    QList<QColor> blueShades = {
        QColor("#1E88E5"), QColor("#42A5F5"), QColor("#64B5F6"),
        QColor("#90CAF9"), QColor("#BBDEFB"), QColor("#E3F2FD")
    };

    QBarSeries *series = new QBarSeries();
    QStringList categoryLabels;
    int colorIndex = 0;
    for (auto it = categoryCounts.begin(); it != categoryCounts.end(); ++it) {
        QBarSet *set = new QBarSet(it.key());
        *set << it.value();
        set->setColor(blueShades[colorIndex % blueShades.size()]);
        series->append(set);
        categoryLabels << it.key();
        colorIndex++;
    }

    // 📈 Graphique
    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Répartition des boutiques par catégorie");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append("Catégories");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setLabelFormat("%.0f");
    axisY->setTitleText("Nombre de boutiques");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignTop);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // 🔊 Synthèse vocale
    QTextToSpeech *speech = new QTextToSpeech(this);
    QPushButton *btnLire = new QPushButton("🔊 Lire les statistiques", this);
    btnLire->setStyleSheet("background-color: #1565C0; color: white; font-weight: bold; padding: 8px; border-radius: 6px;");

    // 🌍 Sélecteur de langue
    QLabel *labelLangue = new QLabel("Langue :", this);
    QComboBox *comboLangue = new QComboBox(this);

    QList<QLocale> locales = speech->availableLocales();
    for (const QLocale &locale : locales) {
        comboLangue->addItem(QLocale::languageToString(locale.language()), locale);
    }

    connect(comboLangue, QOverload<int>::of(&QComboBox::currentIndexChanged), this, [=](int index) {
        QLocale selectedLocale = comboLangue->itemData(index).toLocale();
        speech->setLocale(selectedLocale);
    });

    connect(btnLire, &QPushButton::clicked, this, [=]() {
        QString texte;
        for (auto it = categoryCounts.begin(); it != categoryCounts.end(); ++it) {
            int nombre = it.value();
            QString pluriel = (nombre > 1) ? "boutiques" : "boutique";
            texte += QString("Il y a %1 %2 dans la catégorie %3. ").arg(nombre).arg(pluriel).arg(it.key());
        }
        speech->say(texte);
    });

    // 🔙 Retour
    QPushButton *btnRetour = new QPushButton("↩ Retour vers l'accueil", this);
    btnRetour->setStyleSheet("background-color: #0D47A1; color: white; font-weight: bold; padding: 8px; border-radius: 6px;");
    connect(btnRetour, &QPushButton::clicked, this, &statistique::close);

    // 📐 Layout
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(chartView);
    layout->addWidget(labelLangue);
    layout->addWidget(comboLangue);
    layout->addWidget(btnLire);
    layout->addWidget(btnRetour);
    setLayout(layout);
}

statistique::~statistique()
{
    delete ui;
}
