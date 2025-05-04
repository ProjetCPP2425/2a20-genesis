#ifndef LOCATAIRE_H
#define LOCATAIRE_H
#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDate>


class locataires
{
private:
    QString nom_loc,mail_loc,statut,mode_p,prix,num_loc;
    QDate date_inscrit,date_fc;
    int id_loc;
public:
    locataires();
    locataires(int id_loc,QString nom_loc,QString mail_loc,QDate date_inscrit,QDate date_fc,QString statut,QString mode_p,QString prix,QString num_loc);

    //getters
    QString getNom(){return nom_loc;}
    QString getMail(){return mail_loc;}
    QDate getDin(){return date_inscrit;}
    QDate getDf(){return date_fc;}
    QString getStt(){return statut;}
    QString getMp(){return mode_p;}
    QString getPrix(){return prix;}
    int getID(){return id_loc;}
    QString get_num(){return num_loc;}

    //setters
    void setNom(QString n){nom_loc=n;}
    void setMail(QString m){mail_loc=m;}
    void setDin(QDate di){date_inscrit=di;}
    void setDf(QDate df){date_fc=df;}
    void setStt(QString st){statut=st;}
    void setMp(QString mp){mode_p=mp;}
    void setPrix(QString p){prix=p;}
    void setID(int id_loc){this->id_loc=id_loc;}
    void setNum(QString num_loc){num_loc=num_loc;}

    //fct
    bool aj();
    QSqlQueryModel * aff();
    bool supp(int id);
    bool modif(int id_loc);
    QMap<QString, int> stat_payement();  // Declaration
    //void sendSMS_Infobip(const QString& recipientPhone);

};




#endif // LOCATAIRE_H
